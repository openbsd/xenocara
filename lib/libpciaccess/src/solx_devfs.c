/*
 * Copyright (c) 2007, Sun Microsystems, Inc.
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

/*
 * Solaris devfs interfaces
 */

#define _GNU_SOURCE

#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dirent.h>
#include <errno.h>
#include <sys/pci.h>
#include <assert.h>
#include <libdevinfo.h>
#include "pci_tools.h"

#include "pciaccess.h"
#include "pciaccess_private.h"

#define	PCI_NEXUS_1	"/devices/pci@0,0:reg"
#define	MAX_DEVICES	256
#define	CELL_NUMS_1275		(sizeof(pci_regspec_t)/sizeof(uint_t))
typedef union {
	uint8_t bytes[16 * sizeof (uint32_t)];
	uint32_t dwords[16];
} pci_conf_hdr_t;

typedef struct i_devnode {
	uint8_t bus;
	uint8_t dev;
	uint8_t func;
	di_node_t node;
}i_devnode_t;

static int root_fd = -1;
static int xsvc_fd = -1;
/*
 * Read config space in native processor endianness.  Endian-neutral
 * processing can then take place.  On big endian machines, MSB and LSB
 * of little endian data end up switched if read as little endian.
 * They are in correct order if read as big endian.
 */
#if defined(__sparc)
#define	NATIVE_ENDIAN	PCITOOL_ACC_ATTR_ENDN_BIG
#elif defined(__x86)
#define	NATIVE_ENDIAN	PCITOOL_ACC_ATTR_ENDN_LTL
#else
#error "ISA is neither __sparc nor __x86"
#endif

/*
 * Identify problematic southbridges.  These have device id 0x5249 and
 * vendor id 0x10b9.  Check for revision ID 0 and class code 060400 as well.
 * Values are little endian, so they are reversed for SPARC.
 *
 * Check for these southbridges on all architectures, as the issue is a
 * southbridge issue, independent of processor.
 *
 * If one of these is found during probing, skip probing other devs/funcs on
 * the rest of the bus, since the southbridge and all devs underneath will
 * otherwise disappear.
 */
#if (NATIVE_ENDIAN == PCITOOL_ACC_ATTR_ENDN_BIG)
#define	U45_SB_DEVID_VID	0xb9104952
#define	U45_SB_CLASS_RID	0x00000406
#else
#define	U45_SB_DEVID_VID	0x524910b9
#define	U45_SB_CLASS_RID	0x06040000
#endif

#define	DEBUGON	0




static int pci_device_solx_devfs_read_rom( struct pci_device * dev,
    void * buffer );

static int pci_device_solx_devfs_probe( struct pci_device * dev );

static int pci_device_solx_devfs_map_region( struct pci_device * dev,
    unsigned region, int write_enable );

static int pci_device_solx_devfs_unmap_region( struct pci_device * dev,
    unsigned region );

static int pci_device_solx_devfs_read( struct pci_device * dev, void * data,
    pciaddr_t offset, pciaddr_t size, pciaddr_t * bytes_read );

static int pci_device_solx_devfs_write( struct pci_device * dev,
    const void * data, pciaddr_t offset, pciaddr_t size,
    pciaddr_t * bytes_wrtten );

static int
probe_dev(int fd, pcitool_reg_t *prg_p, struct pci_system *pci_sys);

static int
do_probe(int fd, struct pci_system *pci_sys);

static void
pci_system_solx_devfs_destroy( void );

static int
get_config_header(int fd, uint8_t bus_no, uint8_t dev_no, uint8_t func_no,
    pci_conf_hdr_t *config_hdr_p);

int
pci_system_solx_devfs_create( void );




static const struct pci_system_methods solx_devfs_methods = {
    .destroy = pci_system_solx_devfs_destroy,
    .destroy_device = NULL,
    .read_rom = pci_device_solx_devfs_read_rom,
    .probe = pci_device_solx_devfs_probe,
    .map = pci_device_solx_devfs_map_region,
    .unmap = pci_device_solx_devfs_unmap_region,

    .read = pci_device_solx_devfs_read,
    .write = pci_device_solx_devfs_write,

    .fill_capabilities = pci_fill_capabilities_generic
};

/*
 * Rlease all the resources
 * Solaris version
 */
static void
pci_system_solx_devfs_destroy( void )
{
	/*
	 * the memory allocated in create routines
	 * will be freed in pci_system_init
	 * It is more reasonable to free them here
	 */
	if (root_fd >= 0) {
		close(root_fd);
		root_fd = -1;
	}

	if (xsvc_fd >= 0) {
		close(xsvc_fd);
		xsvc_fd = -1;
	}

}
/*
 * Attempt to access PCI subsystem using Solaris's devfs interface.
 * Solaris version
 */
int
pci_system_solx_devfs_create( void )
{
	int err = 0;


	if (root_fd >= 0)
		return (err);
	/* If the directory "/sys/bus/pci/devices" exists,
	 * then the PCI subsystem can be accessed using
	 * this interface.
	 */
   	if ((root_fd = open(PCI_NEXUS_1, O_RDWR)) == -1) {
		(void) fprintf(stderr,
		    "Could not open nexus node %s: %s\n",
		    PCI_NEXUS_1, strerror(errno));

			err = errno;

			return (err);
	} else {
		/*
		 * Only allow MAX_DEVICES exists
		 * I will fix it later to get
		 * the total devices first
		 */
		if ((pci_sys = calloc(1, sizeof (struct pci_system))) != NULL) {
			pci_sys->methods = &solx_devfs_methods;
			if ((pci_sys->devices =
			    calloc(MAX_DEVICES,
			    sizeof (struct pci_device_private))) != NULL) {
				(void) do_probe(root_fd, pci_sys);
			}
			else {
				err = errno;
				free(pci_sys);
				pci_sys = NULL;
			}
		} else {
			err = errno;
		}
		
	}
	
	return (err);
}

/*
 * Retrieve first 16 dwords of device's config header, except for the first
 * dword.  First 16 dwords are defined by the PCI specification.
 */
static int
get_config_header(int fd, uint8_t bus_no, uint8_t dev_no, uint8_t func_no,
    pci_conf_hdr_t *config_hdr_p)
{
	pcitool_reg_t cfg_prg;
	int i;
	int rval = 0;

	/* Prepare a local pcitool_reg_t so as to not disturb the caller's. */
	cfg_prg.offset = 0;
	cfg_prg.acc_attr = PCITOOL_ACC_ATTR_SIZE_4 + NATIVE_ENDIAN;
	cfg_prg.bus_no = bus_no;
	cfg_prg.dev_no = dev_no;
	cfg_prg.func_no = func_no;
	cfg_prg.barnum = 0;
	cfg_prg.user_version = PCITOOL_USER_VERSION;

	/* Get dwords 1-15 of config space. They must be read as uint32_t. */
	for (i = 1; i < (sizeof (pci_conf_hdr_t) / sizeof (uint32_t)); i++) {
		cfg_prg.offset += sizeof (uint32_t);
		if ((rval =
		    ioctl(fd, PCITOOL_DEVICE_GET_REG, &cfg_prg)) != 0) {
			break;
		}
		config_hdr_p->dwords[i] = (uint32_t)cfg_prg.data;
	}

	return (rval);
}


/*
 * Probe device's functions.  Modifies many fields in the prg_p.
 */
static int
probe_dev(int fd, pcitool_reg_t *prg_p, struct pci_system *pci_sys)
{
	pci_conf_hdr_t	config_hdr;
	boolean_t	multi_function_device;
	int8_t		func;
	int8_t		first_func = 0;
	int8_t		last_func = PCI_REG_FUNC_M >> PCI_REG_FUNC_SHIFT;
	int		rval = 0;


	/*
	 * Loop through at least func=first_func.  Continue looping through
	 * functions if there are no errors and the device is a multi-function
	 * device.
	 *
	 * (Note, if first_func == 0, header will show whether multifunction
	 * device and set multi_function_device.  If first_func != 0, then we
	 * will force the loop as the user wants a specific function to be
	 * checked.
	 */
	for (func = first_func, multi_function_device = B_FALSE;
	    ((func <= last_func) &&
		((func == first_func) || (multi_function_device)));
	    func++) {
		prg_p->func_no = func;

		/*
		 * Four things can happen here:
		 *
		 * 1) ioctl comes back as EFAULT and prg_p->status is
		 *    PCITOOL_INVALID_ADDRESS.  There is no device at this
		 *    location.
		 *
		 * 2) ioctl comes back successful and the data comes back as
		 *    zero.  Config space is mapped but no device responded.
		 *
		 * 3) ioctl comes back successful and the data comes back as
		 *    non-zero.  We've found a device.
		 *
		 * 4) Some other error occurs in an ioctl.
		 */

		prg_p->status = PCITOOL_SUCCESS;
		prg_p->offset = 0;
		prg_p->data = 0;
		prg_p->user_version = PCITOOL_USER_VERSION;
		if (((rval = ioctl(fd, PCITOOL_DEVICE_GET_REG, prg_p)) != 0) ||
		    (prg_p->data == 0xffffffff)) {

			/*
			 * Accept errno == EINVAL along with status of
			 * PCITOOL_OUT_OF_RANGE because some systems
			 * don't implement the full range of config space.
			 * Leave the loop quietly in this case.
			 */
			if ((errno == EINVAL) ||
			    (prg_p->status == PCITOOL_OUT_OF_RANGE)) {
				break;
			}

			/*
			 * Exit silently with ENXIO as this means that there are
			 * no devices under the pci root nexus.
			 */
			else if ((errno == ENXIO) &&
			    (prg_p->status == PCITOOL_IO_ERROR)) {
				break;
			}

			/*
			 * Expect errno == EFAULT along with status of
			 * PCITOOL_INVALID_ADDRESS because there won't be
			 * devices at each stop.  Quit on any other error.
			 */
			else if (((errno != EFAULT) ||
			    (prg_p->status != PCITOOL_INVALID_ADDRESS)) &&
			    (prg_p->data != 0xffffffff)) {

				break;

			/*
			 * If no function at this location,
			 * just advance to the next function.
			 */
			} else {
				rval = 0;
			}

		/*
		 * Data came back as 0.
		 * Treat as unresponsive device amd check next device.
		 */
		} else if (prg_p->data == 0) {
			rval = 0;
			break;	/* Func loop. */

		/* Found something. */
		} else {
			config_hdr.dwords[0] = (uint32_t)prg_p->data;

			/* Get the rest of the PCI header. */
			if ((rval = get_config_header(fd, prg_p->bus_no,
			    prg_p->dev_no, prg_p->func_no, &config_hdr)) !=
			    0) {
				break;
			}

			/*
			 * Special case for the type of Southbridge found on
			 * Ultra-45 and other sun4u fire workstations.
			 */
			if ((config_hdr.dwords[0] == U45_SB_DEVID_VID) &&
			    (config_hdr.dwords[2] == U45_SB_CLASS_RID)) {
				rval = ECANCELED;
				break;
			}

			/*
			 * Found one device with bus numer, device number and
			 * function number.
			 */

			/*
			 * Domain is peer bus??
			 */
			pci_sys->devices[pci_sys->num_devices].base.domain = 0;
			pci_sys->devices[pci_sys->num_devices].base.bus =
			    prg_p->bus_no;
			pci_sys->devices[pci_sys->num_devices].base.dev =
			    prg_p->dev_no;
			pci_sys->devices[pci_sys->num_devices].base.func = func;
			/*
			 * for the format of device_class, see struct pci_device;
			 */
			pci_sys->devices[pci_sys->num_devices].base.device_class =
			    config_hdr.dwords[2]>>8;
			pci_sys->devices[pci_sys->num_devices].base.revision =
			    (uint8_t)(config_hdr.dwords[2] & 0xff);
			pci_sys->devices[pci_sys->num_devices].base.vendor_id =
			    (uint16_t)(config_hdr.dwords[0] & 0xffff);		
			pci_sys->devices[pci_sys->num_devices].base.device_id =
			    (uint16_t)((config_hdr.dwords[0]>>16) & 0xffff);		
			pci_sys->devices[pci_sys->num_devices].base.subvendor_id =
			    (uint16_t)(config_hdr.dwords[11] & 0xffff);		
			pci_sys->devices[pci_sys->num_devices].base.subdevice_id =
			    (uint16_t)((config_hdr.dwords[11]>>16) & 0xffff);		
			pci_sys->devices[pci_sys->num_devices].header_type =
			    (uint8_t)(((config_hdr.dwords[3])&0xff0000)>>16);
#if DEBUGON
			fprintf(stderr, "busno = %x, devno = %x, funcno = %x\n",
			    prg_p->bus_no, prg_p->dev_no, func);
#endif

			pci_sys->num_devices++;

			/*
			 * Accomodate devices which state their
			 * multi-functionality only in their function 0 config
			 * space.  Note multi-functionality throughout probing
			 * of all of this device's functions.
			 */
			if (config_hdr.bytes[PCI_CONF_HEADER] &
			    PCI_HEADER_MULTI) {
				multi_function_device = B_TRUE;
			}
		}
	}

	return (rval);
}

/*
 * Solaris version
 * Probe a given nexus config space for devices.
 *
 * fd is the file descriptor of the nexus.
 * input_args contains commandline options as specified by the user.
 */
static int
do_probe(int fd, struct pci_system *pci_sys)
{
	pcitool_reg_t prg;
	uint32_t bus;
	uint8_t dev;
	uint32_t last_bus = PCI_REG_BUS_M >> PCI_REG_BUS_SHIFT;
	uint8_t last_dev = PCI_REG_DEV_M >> PCI_REG_DEV_SHIFT;
	uint8_t first_bus = 0;
	uint8_t first_dev = 0;
	int rval = 0;

	prg.barnum = 0;	/* Config space. */

	/* Must read in 4-byte quantities. */
	prg.acc_attr = PCITOOL_ACC_ATTR_SIZE_4 + NATIVE_ENDIAN;

	prg.data = 0;

	/*
	 * Loop through all valid bus / dev / func combinations to check for
	 * all devices, with the following exceptions:
	 *
	 * When nothing is found at function 0 of a bus / dev combination, skip
	 * the other functions of that bus / dev combination.
	 *
	 * When a found device's function 0 is probed and it is determined that
	 * it is not a multifunction device, skip probing of that device's
	 * other functions.
	 */
	for (bus = first_bus; ((bus <= last_bus) && (rval == 0)); bus++) {
		prg.bus_no = (uint8_t)bus;
		for (dev = first_dev;
		    ((dev <= last_dev) && (rval == 0)); dev++) {
			prg.dev_no = dev;
			rval = probe_dev(fd, &prg, pci_sys);
		}

		/*
		 * Ultra-45 southbridge workaround:
		 * ECANCELED tells to skip to the next bus.
		 */
		if (rval == ECANCELED) {
			rval = 0;
		}
	}
	if (pci_sys->num_devices > MAX_DEVICES) {
		(void) fprintf(stderr, "pci devices reach maximu number\n");
	}

	return (rval);
}

static int
find_target_node(di_node_t node, void *arg)
{
	int *regbuf = NULL;
	int len = 0;
	uint32_t busno, funcno, devno;
	i_devnode_t *devnode;
	void *prop = DI_PROP_NIL;
	int i;

	devnode = (i_devnode_t *)arg;

	/*
	 * Test the property funtions, only for testing
	 */
	/*
	(void) fprintf(stderr, "start of node 0x%x\n", node->nodeid);
	while ((prop = di_prop_hw_next(node, prop)) != DI_PROP_NIL) {
		(void) fprintf(stderr, "name=%s: ", di_prop_name(prop));
		len = 0;
		if (!strcmp(di_prop_name(prop), "reg")) {
			len = di_prop_ints(prop, &regbuf);
		}
		for (i = 0; i < len; i++) {
			fprintf(stderr, "0x%0x.", regbuf[i]);
		}
		fprintf(stderr, "\n");
	}
	(void) fprintf(stderr, "end of node 0x%x\n", node->nodeid);
	*/
	
	len = di_prop_lookup_ints(DDI_DEV_T_ANY, node, "reg",
	    &regbuf);

	if (len <= 0) {
#if DEBUGON
		fprintf(stderr, "error = %x\n", errno);
		fprintf(stderr, "can not find assigned-address\n");
#endif
		return (DI_WALK_CONTINUE);
	}
	busno = PCI_REG_BUS_G(regbuf[0]);
	devno = PCI_REG_DEV_G(regbuf[0]);
	funcno = PCI_REG_FUNC_G(regbuf[0]);

	if ((busno == devnode->bus) &&
	    (devno == devnode->dev) &&
	    (funcno == devnode->func)) {
		devnode->node = node;
		
		return (DI_WALK_TERMINATE);
	}

	return (DI_WALK_CONTINUE);
}

/*
 * Solaris version
 */
static int
pci_device_solx_devfs_probe( struct pci_device * dev )
{
	uint8_t  config[256];
	int err;
	di_node_t rnode;
	i_devnode_t args;
	int *regbuf;
	pci_regspec_t *reg;
	int i;
	pciaddr_t bytes;
	int len = 0;

	err = pci_device_solx_devfs_read( dev, config, 0, 256, & bytes );
	args.node = DI_NODE_NIL;
	if ( bytes >= 64 ) {
		struct pci_device_private *priv =
		     (struct pci_device_private *) dev;

		dev->vendor_id =
		    (uint16_t)config[0] + ((uint16_t)config[1] << 8);
		dev->device_id =
		    (uint16_t)config[2] + ((uint16_t)config[3] << 8);
		dev->device_class = (uint32_t)config[9] +
		    ((uint32_t)config[10] << 8) +
		    ((uint16_t)config[11] << 16);
		/*
		 * device class code is already there.
		 * see probe_dev function.
		 */
		dev->revision = config[8];
		dev->subvendor_id =
		    (uint16_t)config[44] + ((uint16_t)config[45] << 8);
		dev->subdevice_id =
		    (uint16_t)config[46] + ((uint16_t)config[47] << 8);
		dev->irq = config[60];

		priv->header_type = config[14];
		/*
		 * starting to find if it is MEM/MEM64/IO
		 * using libdevinfo
		 */
		if ((rnode = di_init("/", DINFOCPYALL)) == DI_NODE_NIL) {
			(void) fprintf(stderr, "di_init failed: $s\n",
			    strerror(errno));
			err = errno;
		} else {
			args.bus = dev->bus;
			args.dev = dev->dev;
			args.func = dev->func;
			(void) di_walk_node(rnode, DI_WALK_CLDFIRST,
			    (void *)&args, find_target_node);
			di_fini(rnode);
		}
	}
	if (args.node != DI_NODE_NIL) {
		/*
		 * It will success for sure, because it was
		 * successfully called in find_target_node
		 */
		len = di_prop_lookup_ints(DDI_DEV_T_ANY, args.node,
		    "assigned-addresses",
		    &regbuf);

	}

	if (len <= 0)
		return (err);


	/*
	 * how to find the size of rom???
	 * if the device has expansion rom,
	 * it must be listed in the last
	 * cells because solaris find probe
	 * the base address from offset 0x10
	 * to 0x30h. So only check the last
	 * item.
	 */
	reg = (pci_regspec_t *)&regbuf[len - CELL_NUMS_1275];
	if (PCI_REG_REG_G(reg->pci_phys_hi) ==
	    PCI_CONF_ROM) {
		/*
		 * rom can only be 32 bits
		 */
		dev->rom_size = reg->pci_size_low;
		len = len - CELL_NUMS_1275;
	}
	else {
		/*
		 * size default to 64K and base address
		 * default to 0xC0000
		 */
		dev->rom_size = 0x10000;
	}

	/*
	 * solaris has its own BAR index. To be sure that
	 * Xorg has the same BAR number as solaris. ????
	 */
	for (i = 0; i < len; i = i + CELL_NUMS_1275) {
		int ent = i/CELL_NUMS_1275;

		reg = (pci_regspec_t *)&regbuf[i];

		/*
		 * non relocatable resource is excluded
		 * such like 0xa0000, 0x3b0. If it is met,
		 * the loop is broken;
		 */
		if (!PCI_REG_REG_G(reg->pci_phys_hi))
			break; 		


		if (reg->pci_phys_hi & PCI_PREFETCH_B) {
			dev->regions[ent].is_prefetchable = 1;
		}

		switch (reg->pci_phys_hi & PCI_REG_ADDR_M) {
		case PCI_ADDR_IO:
			dev->regions[ent].is_IO = 1;
			break;
		case PCI_ADDR_MEM32:
			break;
		case PCI_ADDR_MEM64:
			dev->regions[ent].is_64 = 1;
			break;
		}
		/*
		 * We split the shift count 32 into two 16 to
		 * avoid the complaining of the compiler
		 */
		dev->regions[ent].base_addr = reg->pci_phys_low +
		    ((reg->pci_phys_mid << 16) << 16);
		dev->regions[ent].size = reg->pci_size_low +
		    ((reg->pci_size_hi << 16) << 16);
	}
		 

    return (err);
}

/*
 * Solaris version: read the ROM data
 */
static int
pci_device_solx_devfs_read_rom( struct pci_device * dev, void * buffer )
{
	void *prom = MAP_FAILED;

	if (xsvc_fd < 0) {
		if ((xsvc_fd = open("/dev/xsvc", O_RDWR)) < 0) {
		    (void) fprintf(stderr, "can not open xsvc driver\n");
		
			return (-1);
		}
	}

	prom = mmap(NULL, dev->rom_size,
	    PROT_READ, MAP_SHARED,
	    xsvc_fd, 0xC0000);

	if (prom == MAP_FAILED) {
		(void) fprintf(stderr, "map rom base =0xC0000 failed");
		return (-1);
	}
	(void) bcopy(prom, buffer, dev->rom_size);
	
	
	/*
	 * Still used xsvc to do the user space mapping
	 */
	return (0);

}

/*
 * solaris version: Read the configurations space of the devices
 */
static int
pci_device_solx_devfs_read( struct pci_device * dev, void * data,
			     pciaddr_t offset, pciaddr_t size,
			     pciaddr_t * bytes_read )
{
	pcitool_reg_t cfg_prg;
	int err = 0;
	int i = 0;

	cfg_prg.offset = offset;
	cfg_prg.acc_attr = PCITOOL_ACC_ATTR_SIZE_1 + NATIVE_ENDIAN;
	cfg_prg.bus_no = dev->bus;
	cfg_prg.dev_no = dev->dev;
	cfg_prg.func_no = dev->func;
	cfg_prg.barnum = 0;
	cfg_prg.user_version = PCITOOL_USER_VERSION;
	*bytes_read = 0;

	for (i = 0; i < size; i = i + PCITOOL_ACC_ATTR_SIZE(PCITOOL_ACC_ATTR_SIZE_1)) {

		cfg_prg.offset = offset + i;
		if ((err = ioctl(root_fd, PCITOOL_DEVICE_GET_REG,
		    &cfg_prg)) != 0) {
			fprintf(stderr, "read bdf<%x,%x,%x,%x> config space failure\n",
			    cfg_prg.bus_no,
			    cfg_prg.dev_no,
			    cfg_prg.func_no,
			    cfg_prg.offset);
			fprintf(stderr, "Failure cause = %x\n", err);
			break;
		}

		((uint8_t *)data)[i] = (uint8_t)cfg_prg.data;
		/*
		 * DWORDS Offset or bytes Offset ??
		 */
	}
	*bytes_read = i;

	return (err);
}

/*
 * Solaris version
 */
static int
pci_device_solx_devfs_write( struct pci_device * dev, const void * data,
			     pciaddr_t offset, pciaddr_t size,
			     pciaddr_t * bytes_written )
{
	pcitool_reg_t cfg_prg;
	int err = 0;
	int cmd;


	if ( bytes_written != NULL ) {
		*bytes_written = 0;
	}

	cfg_prg.offset = offset;
	switch (size) {
		case 1:
		cfg_prg.acc_attr = PCITOOL_ACC_ATTR_SIZE_1 + NATIVE_ENDIAN;
		break;
		case 2:
		cfg_prg.acc_attr = PCITOOL_ACC_ATTR_SIZE_2 + NATIVE_ENDIAN;
		break;
		case 4:
		cfg_prg.acc_attr = PCITOOL_ACC_ATTR_SIZE_4 + NATIVE_ENDIAN;
		break;
		case 8:
		cfg_prg.acc_attr = PCITOOL_ACC_ATTR_SIZE_8 + NATIVE_ENDIAN;
		break;
		default:
			assert(0);

	}
	cfg_prg.bus_no = dev->bus;
	cfg_prg.dev_no = dev->dev;
	cfg_prg.func_no = dev->func;
	cfg_prg.barnum = 0;
	cfg_prg.user_version = PCITOOL_USER_VERSION;
	cfg_prg.data = *((uint64_t *)data);
	/*
	 * Check if this device is bridge device.
	 * If it is, it is also a nexus node???
	 * It seems that there is no explicit
	 * PCI nexus device for X86, so not applicable
	 * from pcitool_bus_reg_ops in pci_tools.c
	 */
	cmd = PCITOOL_DEVICE_SET_REG;

	if ((err = ioctl(root_fd, cmd, &cfg_prg)) != 0) {
		return (err);
	}
	*bytes_written = size;

	return (err);
}


/*
 * Solaris Version
 */
static int
pci_device_solx_devfs_map_region( struct pci_device * dev, unsigned region,
    int write_enable )
{

	if (xsvc_fd < 0) {
		if ((xsvc_fd = open("/dev/xsvc", O_RDWR)) < 0) {
		    (void) fprintf(stderr, "can not open xsvc driver\n");
		
			return (-1);
		}
	}

	dev->regions[region].memory = mmap(NULL, dev->regions[region].size,
	    (write_enable) ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED,
	    xsvc_fd, dev->regions[region].base_addr);

	if (dev->regions[region].memory == MAP_FAILED) {
		dev->regions[region].memory = 0;
	
		(void) fprintf(stderr, "map rom region =%x failed",
		    dev->regions[region].base_addr);
		return (-1);
	}
	
	/*
	 * Still used xsvc to do the user space mapping
	 */
	return (0);
}


/*
 * Solaris version
 */
static int
pci_device_solx_devfs_unmap_region( struct pci_device * dev, unsigned region )
{
    int err = 0;

    if ( munmap( dev->regions[ region ].memory, dev->regions[ region ].size )
	 == -1 ) {
	err = errno;
    }

    dev->regions[ region ].memory = NULL;

    return (err);
}
