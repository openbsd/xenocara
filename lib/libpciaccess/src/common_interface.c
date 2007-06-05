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
 * \file common_interface.c
 * Platform independent interface glue.
 *
 * \author Ian Romanick <idr@us.ibm.com>
 */

#include <stdlib.h>
#include <errno.h>

#include "pciaccess.h"
#include "pciaccess_private.h"

#ifdef __linux__
#include <byteswap.h>

#if __BYTE_ORDER == __BIG_ENDIAN
# define LETOH_16(x)   bswap_16(x)
# define HTOLE_16(x)   bswap_16(x)
# define LETOH_32(x)   bswap_32(x)
# define HTOLE_32(x)   bswap_32(x)
#else
# define LETOH_16(x)   (x)
# define HTOLE_16(x)   (x)
# define LETOH_32(x)   (x)
# define HTOLE_32(x)   (x)
#endif

#else

#include <sys/endian.h>

#define LETOH_16(x)	le16toh(x)
#define HTOLE_16(x)	htole16(x)
#define LETOH_32(x)	le32toh(x)
#define HTOLE_32(x)	htole32(x)

#endif

/**
 * Read a device's expansion ROM.
 * 
 * Reads the device's expansion ROM and stores the data in the memory pointed
 * to by \c buffer.  The buffer must be at least \c pci_device::rom_size
 * bytes.
 *
 * \param dev    Device whose expansion ROM is to be read.
 * \param buffer Memory in which to store the ROM.
 * 
 * \return
 * Zero on success or an \c errno value on failure.
 */
int
pci_device_read_rom( struct pci_device * dev, void * buffer )
{
    if ( (dev == NULL) || (buffer == NULL) ) {
	return EFAULT;
    }


    return (pci_sys->methods->read_rom)( dev, buffer );
}


/**
 * Probe a PCI device to learn information about the device.
 * 
 * Probes a PCI device to learn various information about the device.  Before
 * calling this function, the only public fields in the \c pci_device
 * structure that have valid values are \c pci_device::domain,
 * \c pci_device::bus, \c pci_device::dev, and \c pci_device::func.
 * 
 * \param dev  Device to be probed.
 * 
 * \return
 * Zero on succes or an \c errno value on failure.
 */
int
pci_device_probe( struct pci_device * dev )
{
    if ( dev == NULL ) {
	return EFAULT;
    }


    return (pci_sys->methods->probe)( dev );
}


/**
 * Map the specified BAR so that it can be accessed by the CPU.
 *
 * Maps the specified BAR for acces by the processor.  The pointer to the
 * mapped region is stored in the \c pci_mem_region::memory pointer for the
 * BAR.
 *
 * \param dev          Device whose memory region is to be mapped.
 * \param region       Region, on the range [0, 5], that is to be mapped.
 * \param write_enable Map for writing (non-zero).
 * 
 * \return
 * Zero on success or an \c errno value on failure.
 *
 * \sa pci_device_unmap_region
 */
int
pci_device_map_region( struct pci_device * dev, unsigned region,
		       int write_enable )
{
    if ( dev == NULL ) {
	return EFAULT;
    }

    if ( (region > 5) || (dev->regions[ region ].size == 0) ) {
	return ENOENT;
    }

    if ( dev->regions[ region ].memory != NULL ) {
	return 0;
    }
    
    return (pci_sys->methods->map)( dev, region, write_enable );
}


/**
 * Map the specified memory range so that it can be accessed by the CPU.
 *
 * Maps the specified memory range for access by the processor.  The pointer
 * to the mapped region is stored in \c addr.  In addtion, the
 * \c pci_mem_region::memory pointer for the BAR will be updated.
 *
 * \param dev          Device whose memory region is to be mapped.
 * \param base         Base address of the range to be mapped.
 * \param size         Size of the range to be mapped.
 * \param write_enable Map for writing (non-zero).
 * \param addr         Location to store the mapped address.
 * 
 * \return
 * Zero on success or an \c errno value on failure.
 *
 * \sa pci_device_unmap_memory_range, pci_device_map_region
 */
int
pci_device_map_memory_range(struct pci_device *dev, pciaddr_t base,
			    pciaddr_t size, int write_enable, 
			    void **addr)
{
    unsigned region;
    int err = 0;


    *addr = NULL;

    if (dev == NULL) {
	return EFAULT;
    }


    for (region = 0; region < 6; region++) {
	const struct pci_mem_region const* r = &dev->regions[region];

	if (r->size != 0) {
	    if ((r->base_addr <= base) && ((r->base_addr + r->size) > base)) {
		if ((base + size) > (r->base_addr + r->size)) {
		    return E2BIG;
		}

		break;
	    }
	}
    }

    if (region > 5) {
	return ENOENT;
    }

    if (dev->regions[region].memory == NULL) {
	err = (*pci_sys->methods->map)(dev, region, write_enable);
    }
    
    if (err == 0) {
	const pciaddr_t offset = base - dev->regions[region].base_addr;

	*addr = ((uint8_t *)dev->regions[region].memory) + offset;
    }

    return err;
}


/**
 * Unmap the specified BAR so that it can no longer be accessed by the CPU.
 *
 * Unmaps the specified BAR that was previously mapped via
 * \c pci_device_map_region.
 *
 * \param dev          Device whose memory region is to be mapped.
 * \param region       Region, on the range [0, 5], that is to be mapped.
 * 
 * \return
 * Zero on success or an \c errno value on failure.
 *
 * \sa pci_device_map_region
 */
int
pci_device_unmap_region( struct pci_device * dev, unsigned region )
{
    if ( dev == NULL ) {
	return EFAULT;
    }

    if ( (region > 5) || (dev->regions[ region ].size == 0) ) {
	return ENOENT;
    }

    if ( dev->regions[ region ].memory == NULL ) {
	return 0;
    }
    
    return (pci_sys->methods->unmap)( dev, region );
}


/**
 * Unmap the specified memory range so that it can no longer be accessed by the CPU.
 *
 * Unmaps the specified memory range that was previously mapped via
 * \c pci_device_map_memory_range.
 *
 * \param dev          Device whose memory is to be unmapped.
 * \param memory       Pointer to the base of the mapped range.
 * \param size         Size, in bytes, of the range to be unmapped.
 * 
 * \return
 * Zero on success or an \c errno value on failure.
 *
 * \sa pci_device_map_memory_range, pci_device_unmap_region
 */
int
pci_device_unmap_memory_range(struct pci_device *dev, void *memory,
			      pciaddr_t size)
{
    unsigned region;


    if (dev == NULL) {
	return EFAULT;
    }

    for (region = 0; region < 6; region++) {
	const struct pci_mem_region const* r = &dev->regions[region];
	const uint8_t *const mem = r->memory;

	if (r->size != 0) {
	    if ((mem <= memory) && ((mem + r->size) > memory)) {
		if ((memory + size) > (mem + r->size)) {
		    return E2BIG;
		}

		break;
	    }
	}
    }

    if (region > 5) {
	return ENOENT;
    }

    return (dev->regions[region].memory != NULL)
	? (*pci_sys->methods->unmap)(dev, region)
	: 0;
}


/**
 * Read arbitrary bytes from device's PCI config space
 *
 * Reads data from the device's PCI configuration space.  As with the system
 * read command, less data may be returned, without an error, than was
 * requested.  This is particuarly the case if a non-root user tries to read
 * beyond the first 64-bytes of configuration space.
 *
 * \param dev         Device whose PCI configuration data is to be read.
 * \param data        Location to store the data
 * \param offset      Initial byte offset to read
 * \param size        Total number of bytes to read
 * \param bytes_read  Location to store the actual number of bytes read.  This
 *                    pointer may be \c NULL.
 *
 * \returns
 * Zero on success or an errno value on failure.
 *
 * \note
 * Data read from PCI configuartion space using this routine is \b not
 * byte-swapped to the host's byte order.  PCI configuration data is always
 * stored in little-endian order, and that is what this routine returns.
 */
int
pci_device_cfg_read( struct pci_device * dev, void * data,
		     pciaddr_t offset, pciaddr_t size, 
		     pciaddr_t * bytes_read )
{
    pciaddr_t  scratch;

    if ( (dev == NULL) || (data == NULL) ) {
	return EFAULT;
    }

    return pci_sys->methods->read( dev, data, offset, size,
				   (bytes_read == NULL) 
				   ? & scratch : bytes_read );
}


int
pci_device_cfg_read_u8( struct pci_device * dev, uint8_t * data,
			pciaddr_t offset )
{
    pciaddr_t bytes;
    int err = pci_device_cfg_read( dev, data, offset, 1, & bytes );
    
    if ( (err == 0) && (bytes != 1) ) {
	err = ENXIO;
    }

    return err;
}


int
pci_device_cfg_read_u16( struct pci_device * dev, uint16_t * data,
			 pciaddr_t offset )
{
    pciaddr_t bytes;
    int err = pci_device_cfg_read( dev, data, offset, 2, & bytes );
    
    if ( (err == 0) && (bytes != 2) ) {
	err = ENXIO;
    }

    *data = LETOH_16( *data );
    return err;
}


int
pci_device_cfg_read_u32( struct pci_device * dev, uint32_t * data,
			 pciaddr_t offset )
{
    pciaddr_t bytes;
    int err = pci_device_cfg_read( dev, data, offset, 4, & bytes );

    if ( (err == 0) && (bytes != 4) ) {
	err = ENXIO;
    }

    *data = LETOH_32( *data );
    return err;
}


/**
 * Write arbitrary bytes to device's PCI config space
 *
 * Writess data to the device's PCI configuration space.  As with the system
 * write command, less data may be written, without an error, than was
 * requested.
 *
 * \param dev         Device whose PCI configuration data is to be written.
 * \param data        Location of the source data
 * \param offset      Initial byte offset to write
 * \param size        Total number of bytes to write
 * \param bytes_read  Location to store the actual number of bytes written.
 *                    This pointer may be \c NULL.
 *
 * \returns
 * Zero on success or an errno value on failure.
 *
 * \note
 * Data written to PCI configuartion space using this routine is \b not
 * byte-swapped from the host's byte order.  PCI configuration data is always
 * stored in little-endian order, so data written with this routine should be
 * put in that order in advance.
 */
int
pci_device_cfg_write( struct pci_device * dev, const void * data,
		      pciaddr_t offset, pciaddr_t size, 
		      pciaddr_t * bytes_written )
{
    pciaddr_t  scratch;

    if ( (dev == NULL) || (data == NULL) ) {
	return EFAULT;
    }

    return pci_sys->methods->write( dev, data, offset, size,
				    (bytes_written == NULL) 
				    ? & scratch : bytes_written );
}


int
pci_device_cfg_write_u8(struct pci_device *dev, uint8_t data,
			pciaddr_t offset)
{
    pciaddr_t bytes;
    int err = pci_device_cfg_write(dev, & data, offset, 1, & bytes);

    if ( (err == 0) && (bytes != 1) ) {
	err = ENOSPC;
    }


    return err;
}
  

int
pci_device_cfg_write_u16(struct pci_device *dev, uint16_t data,
			 pciaddr_t offset)
{
    pciaddr_t bytes;
    const uint16_t temp = HTOLE_16(data);
    int err = pci_device_cfg_write( dev, & temp, offset, 2, & bytes );

    if ( (err == 0) && (bytes != 2) ) {
	err = ENOSPC;
    }


    return err;
}


int
pci_device_cfg_write_u32(struct pci_device *dev, uint32_t data,
			 pciaddr_t offset)
{
    pciaddr_t bytes;
    const uint32_t temp = HTOLE_32(data);
    int err = pci_device_cfg_write( dev, & temp, offset, 4, & bytes );

    if ( (err == 0) && (bytes != 4) ) {
	err = ENOSPC;
    }


    return err;
}


int
pci_device_cfg_write_bits( struct pci_device * dev, uint32_t mask, 
			   uint32_t data, pciaddr_t offset )
{
    uint32_t  temp;
    int err;

    err = pci_device_cfg_read_u32( dev, & temp, offset );
    if ( ! err ) {
	temp &= ~mask;
	temp |= data;

	err = pci_device_cfg_write_u32(dev, temp, offset);
    }

    return err;
}
