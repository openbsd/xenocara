/*
 * Copyright (c) 2018, Damien Zammit
 * Copyright (c) 2017, Joan Lledó
 * Copyright (c) 2009, 2012 Samuel Thibault
 * Heavily inspired from the freebsd, netbsd, and openbsd backends
 * (C) Copyright Eric Anholt 2006
 * (C) Copyright IBM Corporation 2006
 * Copyright (c) 2008 Juan Romero Pardines
 * Copyright (c) 2008 Mark Kettenis
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/mman.h>
#include <string.h>
#include <strings.h>
#include <hurd.h>
#include <hurd/pci.h>
#include <hurd/paths.h>

#include "x86_pci.h"
#include "pciaccess.h"
#include "pciaccess_private.h"

/*
 * Hurd PCI access using RPCs.
 *
 * Some functions are shared with the x86 module to avoid repeating code.
 */

/* Server path */
#define _SERVERS_BUS_PCI	_SERVERS_BUS "/pci"

/* File names */
#define FILE_CONFIG_NAME "config"
#define FILE_ROM_NAME "rom"

/* Level in the fs tree */
typedef enum {
    LEVEL_NONE,
    LEVEL_DOMAIN,
    LEVEL_BUS,
    LEVEL_DEV,
    LEVEL_FUNC
} tree_level;

struct pci_system_hurd {
    struct pci_system system;
};

static int
pci_device_hurd_probe(struct pci_device *dev)
{
    uint8_t irq;
    int err, i;
    struct pci_bar regions[6];
    struct pci_xrom_bar rom;
    struct pci_device_private *d;
    size_t size;
    char *buf;

    /* Many of the fields were filled in during initial device enumeration.
     * At this point, we need to fill in regions, rom_size, and irq.
     */

    err = pci_device_cfg_read_u8(dev, &irq, PCI_IRQ);
    if (err)
        return err;
    dev->irq = irq;

    /* Get regions */
    buf = (char *)&regions;
    size = sizeof(regions);
    d = (struct pci_device_private *)dev;
    err = pci_get_dev_regions(d->device_port, &buf, &size);
    if(err)
        return err;

    if((char*)&regions != buf)
    {
        /* Sanity check for bogus server.  */
        if(size > sizeof(regions))
        {
            vm_deallocate(mach_task_self(), (vm_address_t) buf, size);
            return EGRATUITOUS;
        }

        memcpy(&regions, buf, size);
        vm_deallocate(mach_task_self(), (vm_address_t) buf, size);
    }

    for(i=0; i<6; i++)
    {
        if(regions[i].size == 0)
            continue;

        dev->regions[i].base_addr = regions[i].base_addr;
        dev->regions[i].size = regions[i].size;
        dev->regions[i].is_IO = regions[i].is_IO;
        dev->regions[i].is_prefetchable = regions[i].is_prefetchable;
        dev->regions[i].is_64 = regions[i].is_64;
    }

    /* Get rom info */
    buf = (char *)&rom;
    size = sizeof(rom);
    err = pci_get_dev_rom(d->device_port, &buf, &size);
    if(err)
        return err;

    if((char*)&rom != buf)
    {
        /* Sanity check for bogus server.  */
        if(size > sizeof(rom))
        {
            vm_deallocate(mach_task_self(), (vm_address_t) buf, size);
            return EGRATUITOUS;
        }

        memcpy(&rom, buf, size);
        vm_deallocate(mach_task_self(), (vm_address_t) buf, size);
    }

    d->rom_base = rom.base_addr;
    dev->rom_size = rom.size;

    return 0;
}

/*
 * Read `nbytes' bytes from `reg' in device's configuretion space
 * and store them in `buf'.
 *
 * It's assumed that `nbytes' bytes are allocated in `buf'
 */
static int
pciclient_cfg_read(mach_port_t device_port, int reg, char *buf,
                   size_t * nbytes)
{
    int err;
    size_t nread;
    char *data;

    data = buf;
    nread = *nbytes;
    err = pci_conf_read(device_port, reg, &data, &nread, *nbytes);
    if (err)
        return err;

    if (data != buf) {
        if (nread > *nbytes)	/* Sanity check for bogus server.  */ {
                vm_deallocate(mach_task_self(), (vm_address_t) data, nread);
                return EGRATUITOUS;
        }

        memcpy(buf, data, nread);
        vm_deallocate(mach_task_self(), (vm_address_t)data, nread);
    }

    *nbytes = nread;

    return 0;
}

/* Write `nbytes' bytes from `buf' to `reg' in device's configuration space */
static int
pciclient_cfg_write(mach_port_t device_port, int reg, char *buf,
                    size_t * nbytes)
{
    int err;
    size_t nwrote;

    err = pci_conf_write(device_port, reg, buf, *nbytes, &nwrote);

    if (!err)
        *nbytes = nwrote;

    return err;
}

/*
 * Read up to `size' bytes from `dev' configuration space to `data' starting
 * at `offset'. Write the amount on read bytes in `bytes_read'.
 */
static int
pci_device_hurd_read(struct pci_device *dev, void *data,
    pciaddr_t offset, pciaddr_t size, pciaddr_t *bytes_read)
{
    int err;
    struct pci_device_private *d;

    *bytes_read = 0;
    d = (struct pci_device_private *)dev;
    while (size > 0) {
        size_t toread = 1 << (ffs(0x4 + (offset & 0x03)) - 1);
        if (toread > size)
            toread = size;

        err = pciclient_cfg_read(d->device_port, offset, (char*)data,
                                 &toread);
        if (err)
            return err;

        offset += toread;
        data = (char*)data + toread;
        size -= toread;
        *bytes_read += toread;
    }
    return 0;
}

/*
 * Write up to `size' bytes from `data' to `dev' configuration space starting
 * at `offset'. Write the amount on written bytes in `bytes_written'.
 */
static int
pci_device_hurd_write(struct pci_device *dev, const void *data,
    pciaddr_t offset, pciaddr_t size, pciaddr_t *bytes_written)
{
    int err;
    struct pci_device_private *d;

    *bytes_written = 0;
    d = (struct pci_device_private *)dev;
    while (size > 0) {
        size_t towrite = 4;
        if (towrite > size)
            towrite = size;
        if (towrite > 4 - (offset & 0x3))
            towrite = 4 - (offset & 0x3);

        err = pciclient_cfg_write(d->device_port, offset, (char*)data,
                                  &towrite);
        if (err)
            return err;

        offset += towrite;
        data = (const char*)data + towrite;
        size -= towrite;
        *bytes_written += towrite;
    }
    return 0;
}

/*
 * Copy the device's firmware in `buffer'
 */
static int
pci_device_hurd_read_rom(struct pci_device * dev, void * buffer)
{
    ssize_t rd;
    int romfd;
    char server[NAME_MAX];

    snprintf(server, NAME_MAX, "%s/%04x/%02x/%02x/%01u/%s", _SERVERS_BUS_PCI,
             dev->domain, dev->bus, dev->dev, dev->func, FILE_ROM_NAME);

    romfd = open(server, O_RDONLY | O_CLOEXEC);
    if (romfd == -1)
        return errno;

    rd = read(romfd, buffer, dev->rom_size);
    if (rd != dev->rom_size) {
        close(romfd);
        return errno;
    }

    close(romfd);

    return 0;
}

/*
 * Each device has its own server where send RPC's to.
 *
 * Deallocate the port before destroying the device.
 */
static void
pci_device_hurd_destroy(struct pci_device *dev)
{
    struct pci_device_private *d = (struct pci_device_private*) dev;

    mach_port_deallocate (mach_task_self (), d->device_port);
}

/* Walk through the FS tree to see what is allowed for us */
static int
enum_devices(const char *parent, struct pci_device_private **device,
                int domain, int bus, int dev, int func, tree_level lev)
{
    int err, ret;
    DIR *dir;
    struct dirent *entry;
    char path[NAME_MAX];
    char server[NAME_MAX];
    uint32_t reg;
    size_t toread;
    mach_port_t device_port;

    dir = opendir(parent);
    if (!dir)
        return errno;

    while ((entry = readdir(dir)) != 0) {
        snprintf(path, NAME_MAX, "%s/%s", parent, entry->d_name);
        if (entry->d_type == DT_DIR) {
            if (!strncmp(entry->d_name, ".", NAME_MAX)
                || !strncmp(entry->d_name, "..", NAME_MAX))
                continue;

            errno = 0;
            ret = strtol(entry->d_name, 0, 16);
            if (errno)
                return errno;

            /*
             * We found a valid directory.
             * Update the address and switch to the next level.
             */
            switch (lev) {
            case LEVEL_DOMAIN:
                domain = ret;
                break;
            case LEVEL_BUS:
                bus = ret;
                break;
            case LEVEL_DEV:
                dev = ret;
                break;
            case LEVEL_FUNC:
                func = ret;
                break;
            default:
                return -1;
            }

            err = enum_devices(path, device, domain, bus, dev, func, lev+1);
            if (err == EPERM)
                continue;
        }
        else {
            if (strncmp(entry->d_name, FILE_CONFIG_NAME, NAME_MAX))
                /* We are looking for the config file */
                continue;

            /* We found an available virtual device, add it to our list */
            snprintf(server, NAME_MAX, "%s/%04x/%02x/%02x/%01u/%s",
                     _SERVERS_BUS_PCI, domain, bus, dev, func,
                     entry->d_name);
            device_port = file_name_lookup(server, 0, 0);
            if (device_port == MACH_PORT_NULL)
                return errno;

            toread = sizeof(reg);
            err = pciclient_cfg_read(device_port, PCI_VENDOR_ID, (char*)&reg,
                                     &toread);
            if (err)
                return err;
            if (toread != sizeof(reg))
                return -1;

            (*device)->base.domain = domain;
            (*device)->base.bus = bus;
            (*device)->base.dev = dev;
            (*device)->base.func = func;
            (*device)->base.vendor_id = PCI_VENDOR(reg);
            (*device)->base.device_id = PCI_DEVICE(reg);

            toread = sizeof(reg);
            err = pciclient_cfg_read(device_port, PCI_CLASS, (char*)&reg,
                                     &toread);
            if (err)
                return err;
            if (toread != sizeof(reg))
                return -1;

            (*device)->base.device_class = reg >> 8;
            (*device)->base.revision = reg & 0xFF;

            toread = sizeof(reg);
            err = pciclient_cfg_read(device_port, PCI_SUB_VENDOR_ID,
                                     (char*)&reg, &toread);
            if (err)
                return err;
            if (toread != sizeof(reg))
                return -1;

            (*device)->base.subvendor_id = PCI_VENDOR(reg);
            (*device)->base.subdevice_id = PCI_DEVICE(reg);

            (*device)->device_port = device_port;

            (*device)++;
        }
    }

    return 0;
}

static const struct pci_system_methods hurd_pci_methods = {
    .destroy = pci_system_x86_destroy,
    .destroy_device = pci_device_hurd_destroy,
    .read_rom = pci_device_hurd_read_rom,
    .probe = pci_device_hurd_probe,
    .map_range = pci_device_x86_map_range,
    .unmap_range = pci_device_x86_unmap_range,
    .read = pci_device_hurd_read,
    .write = pci_device_hurd_write,
    .fill_capabilities = pci_fill_capabilities_generic,
    .open_legacy_io = pci_device_x86_open_legacy_io,
    .close_io = pci_device_x86_close_io,
    .read32 = pci_device_x86_read32,
    .read16 = pci_device_x86_read16,
    .read8 = pci_device_x86_read8,
    .write32 = pci_device_x86_write32,
    .write16 = pci_device_x86_write16,
    .write8 = pci_device_x86_write8,
    .map_legacy = pci_device_x86_map_legacy,
    .unmap_legacy = pci_device_x86_unmap_legacy,
};

_pci_hidden int
pci_system_hurd_create(void)
{
    struct pci_device_private *device;
    int err;
    struct pci_system_hurd *pci_sys_hurd;
    size_t ndevs;
    mach_port_t pci_server_port;

    /* If we can open pci cfg io ports on hurd,
     * we are the arbiter, therefore try x86 method first */
    err = pci_system_x86_create();
    if (!err)
        return 0;

    pci_sys_hurd = calloc(1, sizeof(struct pci_system_hurd));
    if (pci_sys_hurd == NULL) {
        x86_disable_io();
        return ENOMEM;
    }
    pci_sys = &pci_sys_hurd->system;

    pci_sys->methods = &hurd_pci_methods;

    pci_server_port = file_name_lookup(_SERVERS_BUS_PCI, 0, 0);
    if (!pci_server_port) {
        /* Fall back to x86 access method */
        return pci_system_x86_create();
    }

    /* The server gives us the number of available devices for us */
    err = pci_get_ndevs (pci_server_port, &ndevs);
    if (err) {
        mach_port_deallocate (mach_task_self (), pci_server_port);
        /* Fall back to x86 access method */
        return pci_system_x86_create();
    }
    mach_port_deallocate (mach_task_self (), pci_server_port);

    pci_sys->num_devices = ndevs;
    pci_sys->devices = calloc(ndevs, sizeof(struct pci_device_private));
    if (pci_sys->devices == NULL) {
        x86_disable_io();
        free(pci_sys_hurd);
        pci_sys = NULL;
        return ENOMEM;
    }

    device = pci_sys->devices;
    err = enum_devices(_SERVERS_BUS_PCI, &device, -1, -1, -1, -1,
                       LEVEL_DOMAIN);
    if (err)
        return pci_system_x86_create();

    return 0;
}
