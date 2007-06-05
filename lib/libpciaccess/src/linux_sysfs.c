/*
 * (C) Copyright IBM Corporation 2006
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
 * IBM AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * \file linux_sysfs.c
 * Access PCI subsystem using Linux's sysfs interface.  This interface is
 * available starting somewhere in the late 2.5.x kernel phase, and is the
 * prefered method on all 2.6.x kernels.
 *
 * \author Ian Romanick <idr@us.ibm.com>
 */

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dirent.h>
#include <errno.h>

#include "pciaccess.h"
#include "pciaccess_private.h"

static int pci_device_linux_sysfs_read_rom( struct pci_device * dev,
    void * buffer );

static int pci_device_linux_sysfs_probe( struct pci_device * dev );

static int pci_device_linux_sysfs_map_region( struct pci_device * dev,
    unsigned region, int write_enable );

static int pci_device_linux_sysfs_unmap_region( struct pci_device * dev,
    unsigned region );

static int pci_device_linux_sysfs_read( struct pci_device * dev, void * data,
    pciaddr_t offset, pciaddr_t size, pciaddr_t * bytes_read );

static int pci_device_linux_sysfs_write( struct pci_device * dev,
    const void * data, pciaddr_t offset, pciaddr_t size,
    pciaddr_t * bytes_wrtten );

static const struct pci_system_methods linux_sysfs_methods = {
    .destroy = NULL,
    .destroy_device = NULL,
    .read_rom = pci_device_linux_sysfs_read_rom,
    .probe = pci_device_linux_sysfs_probe,
    .map = pci_device_linux_sysfs_map_region,
    .unmap = pci_device_linux_sysfs_unmap_region,

    .read = pci_device_linux_sysfs_read,
    .write = pci_device_linux_sysfs_write,

    .fill_capabilities = pci_fill_capabilities_generic
};

#define SYS_BUS_PCI "/sys/bus/pci/devices"


static int populate_entries(struct pci_system * pci_sys);


/**
 * Attempt to access PCI subsystem using Linux's sysfs interface.
 */
int
pci_system_linux_sysfs_create( void )
{
    int err = 0;
    struct stat st;


    /* If the directory "/sys/bus/pci/devices" exists, then the PCI subsystem
     * can be accessed using this interface.
     */
    
    if ( stat( SYS_BUS_PCI, & st ) == 0 ) {
	pci_sys = calloc( 1, sizeof( struct pci_system ) );
	if ( pci_sys != NULL ) {
	    pci_sys->methods = & linux_sysfs_methods;
	    err = populate_entries(pci_sys);
	}
	else {
	    err = ENOMEM;
	}
    }
    else {
	err = errno;
    }

    return err;
}


/**
 * Filter out the names "." and ".." from the scanned sysfs entries.
 *
 * \param d  Directory entry being processed by \c scandir.
 *
 * \return
 * Zero if the entry name matches either "." or "..", non-zero otherwise.
 *
 * \sa scandir, populate_entries
 */
static int
scan_sys_pci_filter( const struct dirent * d )
{
    return !((strcmp( d->d_name, "." ) == 0) 
	     || (strcmp( d->d_name, ".." ) == 0));
}


int
populate_entries( struct pci_system * p )
{
    struct dirent ** devices;
    int n;
    int i;
    int err;


    n = scandir( SYS_BUS_PCI, & devices, scan_sys_pci_filter, alphasort );
    if ( n > 0 ) {
	p->num_devices = n;
	p->devices = calloc( n, sizeof( struct pci_device_private ) );

	if (p->devices != NULL) {
	    for (i = 0 ; i < n ; i++) {
		uint8_t config[48];
		pciaddr_t bytes;
		unsigned dom, bus, dev, func;
		struct pci_device_private *device =
			(struct pci_device_private *) &p->devices[i];


		sscanf(devices[i]->d_name, "%04x:%02x:%02x.%1u",
		       & dom, & bus, & dev, & func);

		device->base.domain = dom;
		device->base.bus = bus;
		device->base.dev = dev;
		device->base.func = func;


		err = pci_device_linux_sysfs_read(& device->base, config, 0,
						  48, & bytes);
		if ((bytes == 48) && !err) {
		    device->base.vendor_id = (uint16_t)config[0]
			+ ((uint16_t)config[1] << 8);
		    device->base.device_id = (uint16_t)config[2]
			+ ((uint16_t)config[3] << 8);
		    device->base.device_class = (uint32_t)config[9]
			+ ((uint32_t)config[10] << 8)
			+ ((uint32_t)config[11] << 16);
		    device->base.revision = config[8];
		    device->base.subvendor_id = (uint16_t)config[44]
			+ ((uint16_t)config[45] << 8);
		    device->base.subdevice_id = (uint16_t)config[46]
			+ ((uint16_t)config[47] << 8);
		}

		if (err) {
		    break;
		}
	    }
	}
	else {
	    err = ENOMEM;
	}
    }

    if (err) {
	free(p->devices);
	p->devices = NULL;
    }

    return err;
}


static int
pci_device_linux_sysfs_probe( struct pci_device * dev )
{
    char     name[256];
    uint8_t  config[256];
    char     resource[512];
    int fd;
    pciaddr_t bytes;
    unsigned i;
    int err;


    err = pci_device_linux_sysfs_read( dev, config, 0, 256, & bytes );
    if ( bytes >= 64 ) {
	struct pci_device_private *priv = (struct pci_device_private *) dev;

	dev->irq = config[60];
	priv->header_type = config[14];


	/* The PCI config registers can be used to obtain information
	 * about the memory and I/O regions for the device.  However,
	 * doing so requires some tricky parsing (to correctly handle
	 * 64-bit memory regions) and requires writing to the config
	 * registers.  Since we'd like to avoid having to deal with the
	 * parsing issues and non-root users can write to PCI config
	 * registers, we use a different file in the device's sysfs
	 * directory called "resource".
	 * 
	 * The resource file contains all of the needed information in
	 * a format that is consistent across all platforms.  Each BAR
	 * and the expansion ROM have a single line of data containing
	 * 3, 64-bit hex values:  the first address in the region,
	 * the last address in the region, and the region's flags.
	 */
	snprintf( name, 255, "%s/%04x:%02x:%02x.%1u/resource",
		  SYS_BUS_PCI,
		  dev->domain,
		  dev->bus,
		  dev->dev,
		  dev->func );
	fd = open( name, O_RDONLY );
	if ( fd != -1 ) {
	    char * next;
	    pciaddr_t  low_addr;
	    pciaddr_t  high_addr;
	    pciaddr_t  flags;


	    bytes = read( fd, resource, 512 );
	    resource[511] = '\0';

	    close( fd );

	    next = resource;
	    for ( i = 0 ; i < 6 ; i++ ) {

		dev->regions[i].base_addr = strtoull( next, & next, 16 );
		high_addr = strtoull( next, & next, 16 );
		flags = strtoull( next, & next, 16 );
		    
		if ( dev->regions[i].base_addr != 0 ) {
		    dev->regions[i].size = (high_addr 
					    - dev->regions[i].base_addr) + 1;

		    dev->regions[i].is_IO = (flags & 0x01);
		    dev->regions[i].is_64 = (flags & 0x04);
		    dev->regions[i].is_prefetchable = (flags & 0x08);
		}
	    }

	    low_addr = strtoull( next, & next, 16 );
	    high_addr = strtoull( next, & next, 16 );
	    flags = strtoull( next, & next, 16 );
	    if ( low_addr != 0 ) {
		dev->rom_size = (high_addr - low_addr) + 1;
	    }
	}
    }

    return err;
}


static int
pci_device_linux_sysfs_read_rom( struct pci_device * dev, void * buffer )
{
    char name[256];
    int fd;
    struct stat  st;
    int err = 0;
    size_t total_bytes;


    snprintf( name, 255, "%s/%04x:%02x:%02x.%1u/rom",
	      SYS_BUS_PCI,
	      dev->domain,
	      dev->bus,
	      dev->dev,
	      dev->func );
    
    fd = open( name, O_RDWR );
    if ( fd == -1 ) {
	return errno;
    }


    if ( fstat( fd, & st ) == -1 ) {
	close( fd );
	return errno;
    }


    /* This is a quirky thing on Linux.  Even though the ROM and the file
     * for the ROM in sysfs are read-only, the string "1" must be written to
     * the file to enable the ROM.  After the data has been read, "0" must be
     * written to the file to disable the ROM.
     */
    write( fd, "1", 1 );
    lseek( fd, 0, SEEK_SET );

    for ( total_bytes = 0 ; total_bytes < st.st_size ; /* empty */ ) {
	const int bytes = read( fd, (char *) buffer + total_bytes,
				st.st_size - total_bytes );
	if ( bytes == -1 ) {
	    err = errno;
	    break;
	}
	else if ( bytes == 0 ) {
	    break;
	}

	total_bytes += bytes;
    }
	

    lseek( fd, 0, SEEK_SET );
    write( fd, "0", 1 );

    close( fd );
    return err;
}


static int
pci_device_linux_sysfs_read( struct pci_device * dev, void * data,
			     pciaddr_t offset, pciaddr_t size,
			     pciaddr_t * bytes_read )
{
    char name[256];
    pciaddr_t temp_size = size;
    int err = 0;
    int fd;


    if ( bytes_read != NULL ) {
	*bytes_read = 0;
    }

    /* Each device has a directory under sysfs.  Within that directory there
     * is a file named "config".  This file used to access the PCI config
     * space.  It is used here to obtain most of the information about the
     * device.
     */
    snprintf( name, 255, "%s/%04x:%02x:%02x.%1u/config",
	      SYS_BUS_PCI,
	      dev->domain,
	      dev->bus,
	      dev->dev,
	      dev->func );

    fd = open( name, O_RDONLY );
    if ( fd == -1 ) {
	return errno;
    }


    while ( temp_size > 0 ) {
	const ssize_t bytes = pread64( fd, data, temp_size, offset );

	/* If zero bytes were read, then we assume it's the end of the
	 * config file.
	 */
	if ( bytes <= 0 ) {
	    err = errno;
	    break;
	}

	temp_size -= bytes;
	offset += bytes;
	data += bytes;
    }
    
    if ( bytes_read != NULL ) {
	*bytes_read = size - temp_size;
    }

    close( fd );
    return err;
}


static int
pci_device_linux_sysfs_write( struct pci_device * dev, const void * data,
			     pciaddr_t offset, pciaddr_t size,
			     pciaddr_t * bytes_written )
{
    char name[256];
    pciaddr_t temp_size = size;
    int err = 0;
    int fd;


    if ( bytes_written != NULL ) {
	*bytes_written = 0;
    }

    /* Each device has a directory under sysfs.  Within that directory there
     * is a file named "config".  This file used to access the PCI config
     * space.  It is used here to obtain most of the information about the
     * device.
     */
    snprintf( name, 255, "%s/%04x:%02x:%02x.%1u/config",
	      SYS_BUS_PCI,
	      dev->domain,
	      dev->bus,
	      dev->dev,
	      dev->func );

    fd = open( name, O_WRONLY );
    if ( fd == -1 ) {
	return errno;
    }


    while ( temp_size > 0 ) {
	const ssize_t bytes = pwrite64( fd, data, temp_size, offset );

	/* If zero bytes were written, then we assume it's the end of the
	 * config file.
	 */
	if ( bytes <= 0 ) {
	    err = errno;
	    break;
	}

	temp_size -= bytes;
	offset += bytes;
	data += bytes;
    }
    
    if ( bytes_written != NULL ) {
	*bytes_written = size - temp_size;
    }

    close( fd );
    return err;
}


/**
 * Map a memory region for a device using the Linux sysfs interface.
 * 
 * \param dev          Device whose memory region is to be mapped.
 * \param region       Region, on the range [0, 5], that is to be mapped.
 * \param write_enable Map for writing (non-zero).
 * 
 * \return
 * Zero on success or an \c errno value on failure.
 *
 * \sa pci_device_map_region, pci_device_linux_sysfs_unmap_region
 *
 * \todo
 * Some older 2.6.x kernels don't implement the resourceN files.  On those
 * systems /dev/mem must be used.  On these systems it is also possible that
 * \c mmap64 may need to be used.
 */
static int
pci_device_linux_sysfs_map_region( struct pci_device * dev, unsigned region,
				   int write_enable )
{
    char name[256];
    int fd;
    int err = 0;
    const int prot = (write_enable) ? (PROT_READ | PROT_WRITE) : PROT_READ;


    snprintf( name, 255, "%s/%04x:%02x:%02x.%1u/resource%u",
	      SYS_BUS_PCI,
	      dev->domain,
	      dev->bus,
	      dev->dev,
	      dev->func,
	      region );

    fd = open( name, (write_enable) ? O_RDWR : O_RDONLY );
    if ( fd == -1 ) {
	return errno;
    }


    dev->regions[ region ].memory = mmap( NULL, dev->regions[ region ].size,
					  prot, MAP_SHARED, fd, 0 );
    if ( dev->regions[ region ].memory == MAP_FAILED ) {
	err = errno;
	dev->regions[ region ].memory = NULL;
    }

    close( fd );
    return err;
}


/**
 * Unmap the specified region using the Linux sysfs interface.
 *
 * \param dev          Device whose memory region is to be mapped.
 * \param region       Region, on the range [0, 5], that is to be mapped.
 *
 * \return
 * Zero on success or an \c errno value on failure.
 *
 * \sa pci_device_unmap_region, pci_device_linux_sysfs_map_region
 *
 * \todo
 * Some older 2.6.x kernels don't implement the resourceN files.  On those
 * systems /dev/mem must be used.  On these systems it is also possible that
 * \c mmap64 may need to be used.
 */
static int
pci_device_linux_sysfs_unmap_region( struct pci_device * dev, unsigned region )
{
    int err = 0;

    if ( munmap( dev->regions[ region ].memory, dev->regions[ region ].size )
	 == -1 ) {
	err = errno;
    }

    dev->regions[ region ].memory = NULL;

    return err;
}
