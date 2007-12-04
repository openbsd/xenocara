/*
 * Copyright 2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Egbert Eich   <eich@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * This tool is here to help create a connector mapping table.
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <pci/pci.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "git_version.h"

#ifndef ULONG
typedef unsigned int ULONG;
# define ULONG ULONG
#endif
#ifndef UCHAR
typedef unsigned char UCHAR;
# define UCHAR UCHAR
#endif
#ifndef USHORT
typedef unsigned short USHORT;
# define USHORT USHORT
#endif

#include "atombios.h"

typedef int Bool;
#define FALSE 0
#define TRUE 1
typedef unsigned char CARD8;
typedef unsigned short CARD16;
typedef unsigned int CARD32;
#define VBIOS_BASE 0xC0000
#define VBIOS_MAXSIZE 0x10000
#define DEV_MEM "/dev/mem"
#define TARGET_HW_I2C_CLOCK 25 /*  kHz */

/* Some register names */
enum {
    /* DAC A */
    DACA_ENABLE                    = 0x7800,
    DACA_SOURCE_SELECT             = 0x7804,
    DACA_AUTODETECT_CONTROL        = 0x7828,
    DACA_FORCE_OUTPUT_CNTL         = 0x783C,
    DACA_FORCE_DATA                = 0x7840,
    DACA_POWERDOWN                 = 0x7850,
    DACA_CONTROL1                  = 0x7854,
    DACA_CONTROL2                  = 0x7858,
    DACA_COMPARATOR_ENABLE         = 0x785C,
    DACA_COMPARATOR_OUTPUT         = 0x7860,

    /* DAC B */
    DACB_ENABLE                    = 0x7A00,
    DACB_SOURCE_SELECT             = 0x7A04,
    DACB_AUTODETECT_CONTROL        = 0x7A28,
    DACB_FORCE_OUTPUT_CNTL         = 0x7A3C,
    DACB_FORCE_DATA                = 0x7A40,
    DACB_POWERDOWN                 = 0x7A50,
    DACB_CONTROL1                  = 0x7A54,
    DACB_CONTROL2                  = 0x7A58,
    DACB_COMPARATOR_ENABLE         = 0x7A5C,
    DACB_COMPARATOR_OUTPUT         = 0x7A60,

    /* TMDSA */
    TMDSA_CNTL                     = 0x7880,
    TMDSA_SOURCE_SELECT            = 0x7884,
    TMDSA_COLOR_FORMAT             = 0x7888,
    TMDSA_FORCE_OUTPUT_CNTL        = 0x788C,
    TMDSA_BIT_DEPTH_CONTROL        = 0x7894,
    TMDSA_DCBALANCER_CONTROL       = 0x78D0,
    TMDSA_DATA_SYNCHRONIZATION_R500 = 0x78D8,
    TMDSA_DATA_SYNCHRONIZATION_R600 = 0x78DC,
    TMDSA_TRANSMITTER_ENABLE       = 0x7904,
    TMDSA_LOAD_DETECT              = 0x7908,
    TMDSA_MACRO_CONTROL            = 0x790C, /* r5x0 and r600: 3 for pll and 1 for TX */
    TMDSA_PLL_ADJUST               = 0x790C, /* rv6x0: pll only */
    TMDSA_TRANSMITTER_CONTROL      = 0x7910,
    TMDSA_TRANSMITTER_ADJUST       = 0x7920, /* rv6x0: TX part of macro control */

    /* LVTMA */
    LVTMA_CNTL                     = 0x7A80,
    LVTMA_SOURCE_SELECT            = 0x7A84,
    LVTMA_BIT_DEPTH_CONTROL        = 0x7A94,
    LVTMA_DATA_SYNCHRONIZATION     = 0x7AD8,
    LVTMA_PWRSEQ_REF_DIV           = 0x7AE4,
    LVTMA_PWRSEQ_DELAY1            = 0x7AE8,
    LVTMA_PWRSEQ_DELAY2            = 0x7AEC,
    LVTMA_PWRSEQ_CNTL              = 0x7AF0,
    LVTMA_PWRSEQ_STATE             = 0x7AF4,
    LVTMA_LVDS_DATA_CNTL           = 0x7AFC,
    LVTMA_MODE                     = 0x7B00,
    LVTMA_TRANSMITTER_ENABLE       = 0x7B04,
    LVTMA_MACRO_CONTROL            = 0x7B0C,
    LVTMA_TRANSMITTER_CONTROL      = 0x7B10,

    /* I2C */
    /* R5XX */
    R5_DC_I2C_STATUS1           = 0x7D30,
    R5_DC_I2C_RESET             = 0x7D34,
    R5_DC_I2C_CONTROL1          = 0x7D38,
    R5_DC_I2C_CONTROL2          = 0x7D3C,
    R5_DC_I2C_CONTROL3          = 0x7D40,
    R5_DC_I2C_DATA              = 0x7D44,
    R5_DC_I2C_INTERRUPT_CONTROL = 0x7D48,
    R5_DC_I2C_ARBITRATION       = 0x7D50,

    /* R6XX */
    R6_DC_I2C_CONTROL		   = 0x7D30,  /* (RW) */
    R6_DC_I2C_ARBITRATION             = 0x7D34,  /* (RW) */
    R6_DC_I2C_INTERRUPT_CONTROL       = 0x7D38,  /* (RW) */
    R6_DC_I2C_SW_STATUS	           = 0x7d3c,  /* (RW) */
    R6_DC_I2C_DDC1_SPEED              = 0x7D4C,  /* (RW) */
    R6_DC_I2C_DDC1_SETUP              = 0x7D50,  /* (RW) */
    R6_DC_I2C_DDC2_SPEED              = 0x7D54,  /* (RW) */
    R6_DC_I2C_DDC2_SETUP              = 0x7D58,  /* (RW) */
    R6_DC_I2C_DDC3_SPEED              = 0x7D5C,  /* (RW) */
    R6_DC_I2C_DDC3_SETUP              = 0x7D60,  /* (RW) */
    R6_DC_I2C_TRANSACTION0            = 0x7D64,  /* (RW) */
    R6_DC_I2C_TRANSACTION1            = 0x7D68,  /* (RW) */
    R6_DC_I2C_DATA                    = 0x7D74,  /* (RW) */
    R6_DC_I2C_DDC4_SPEED              = 0x7DB4,  /* (RW) */
    R6_DC_I2C_DDC4_SETUP              = 0x7DBC,  /* (RW) */

    DC_GPIO_DDC4_MASK              = 0x7E00,  /* (RW) */
    DC_GPIO_DDC4_A                 = 0x7E04,  /* (RW) */
    DC_GPIO_DDC4_EN                = 0x7E08,  /* (RW) */
    DC_GPIO_DDC1_MASK              = 0x7E40,  /* (RW) */
    DC_GPIO_DDC1_A                 = 0x7E44,  /* (RW) */
    DC_GPIO_DDC1_EN                = 0x7E48,  /* (RW) */
    DC_GPIO_DDC1_Y                 = 0x7E4C,  /* (RW) */
    DC_GPIO_DDC2_MASK              = 0x7E50,  /* (RW) */
    DC_GPIO_DDC2_A                 = 0x7E54,  /* (RW) */
    DC_GPIO_DDC2_EN                = 0x7E58,  /* (RW) */
    DC_GPIO_DDC2_Y                 = 0x7E5C,  /* (RW) */
    DC_GPIO_DDC3_MASK              = 0x7E60,  /* (RW) */
    DC_GPIO_DDC3_A                 = 0x7E64,  /* (RW) */
    DC_GPIO_DDC3_EN                = 0x7E68,  /* (RW) */
    DC_GPIO_DDC3_Y                 = 0x7E6C,  /* (RW) */

    /* RS69x I2C */
    RS69_DC_I2C_CONTROL		   = 0x7D30,  /* (RW) */
    RS69_DC_I2C_UNKNOWN_2	   = 0x7D34,  /* (RW) */
    RS69_DC_I2C_INTERRUPT_CONTROL  = 0x7D38,  /* (RW) */
    RS69_DC_I2C_SW_STATUS	   = 0x7d3c,  /* (RW) */
    RS69_DC_I2C_UNKNOWN_1          = 0x7d40,
    RS69_DC_I2C_DDC_SETUP_Q        = 0x7D44,  /* (RW) */
    RS69_DC_I2C_DATA		   = 0x7D58,  /* (RW) */
    RS69_DC_I2C_TRANSACTION0       = 0x7D48,  /* (RW) */
    RS69_DC_I2C_TRANSACTION1       = 0x7D4C,  /* (RW) */

    /* HPD */
    DC_GPIO_HPD_Y                  = 0x7E9C
};

typedef enum _chipType {
    RHD_R500 = 1,
    RHD_RS690,
    RHD_R600
} chipType;

/* for RHD_R500/R600 */
chipType ChipType;

typedef struct _tableVersion
{
    CARD8 crev;
    CARD8 frev;
} tableVersion;

typedef struct _atomDataTables
{
    union {
        void                            *base;
        ATOM_FIRMWARE_INFO              *FirmwareInfo;
        ATOM_FIRMWARE_INFO_V1_2         *FirmwareInfo_V_1_2;
        ATOM_FIRMWARE_INFO_V1_3         *FirmwareInfo_V_1_3;
        ATOM_FIRMWARE_INFO_V1_4         *FirmwareInfo_V_1_4;
    } FirmwareInfo;
    tableVersion FirmwareInfoVersion;
    ATOM_GPIO_I2C_INFO			*GPIO_I2C_Info;
    tableVersion GPIO_I2C_InfoVersion;
} atomDataTables, *atomDataTablesPtr;

atomDataTables AtomData;


/*
 * Match pci ids against data and some callbacks
 */
struct RHDDevice {
    CARD16 vendor;
    CARD16 device;
    int bar;
    chipType type;
} rhdDevices[] = {

    { 0x1002, 0x7100, 2, RHD_R500},
    { 0x1002, 0x7101, 2, RHD_R500},
    { 0x1002, 0x7102, 2, RHD_R500},
    { 0x1002, 0x7103, 2, RHD_R500},
    { 0x1002, 0x7104, 2, RHD_R500},
    { 0x1002, 0x7105, 2, RHD_R500},
    { 0x1002, 0x7106, 2, RHD_R500},
    { 0x1002, 0x7108, 2, RHD_R500},
    { 0x1002, 0x7109, 2, RHD_R500},
    { 0x1002, 0x710A, 2, RHD_R500},
    { 0x1002, 0x710B, 2, RHD_R500},
    { 0x1002, 0x710C, 2, RHD_R500},
    { 0x1002, 0x710E, 2, RHD_R500},
    { 0x1002, 0x710F, 2, RHD_R500},
    { 0x1002, 0x7140, 2, RHD_R500},
    { 0x1002, 0x7141, 2, RHD_R500},
    { 0x1002, 0x7142, 2, RHD_R500},
    { 0x1002, 0x7143, 2, RHD_R500},
    { 0x1002, 0x7144, 2, RHD_R500},
    { 0x1002, 0x7145, 2, RHD_R500},
    { 0x1002, 0x7146, 2, RHD_R500},
    { 0x1002, 0x7147, 2, RHD_R500},
    { 0x1002, 0x7149, 2, RHD_R500},
    { 0x1002, 0x714A, 2, RHD_R500},
    { 0x1002, 0x714B, 2, RHD_R500},
    { 0x1002, 0x714C, 2, RHD_R500},
    { 0x1002, 0x714D, 2, RHD_R500},
    { 0x1002, 0x714E, 2, RHD_R500},
    { 0x1002, 0x714F, 2, RHD_R500},
    { 0x1002, 0x7151, 2, RHD_R500},
    { 0x1002, 0x7152, 2, RHD_R500},
    { 0x1002, 0x7153, 2, RHD_R500},
    { 0x1002, 0x715E, 2, RHD_R500},
    { 0x1002, 0x715F, 2, RHD_R500},
    { 0x1002, 0x7180, 2, RHD_R500},
    { 0x1002, 0x7181, 2, RHD_R500},
    { 0x1002, 0x7183, 2, RHD_R500},
    { 0x1002, 0x7186, 2, RHD_R500},
    { 0x1002, 0x7187, 2, RHD_R500},
    { 0x1002, 0x7188, 2, RHD_R500},
    { 0x1002, 0x718A, 2, RHD_R500},
    { 0x1002, 0x718B, 2, RHD_R500},
    { 0x1002, 0x718C, 2, RHD_R500},
    { 0x1002, 0x718D, 2, RHD_R500},
    { 0x1002, 0x718F, 2, RHD_R500},
    { 0x1002, 0x7193, 2, RHD_R500},
    { 0x1002, 0x7196, 2, RHD_R500},
    { 0x1002, 0x719B, 2, RHD_R500},
    { 0x1002, 0x719F, 2, RHD_R500},
    { 0x1002, 0x71C0, 2, RHD_R500},
    { 0x1002, 0x71C1, 2, RHD_R500},
    { 0x1002, 0x71C2, 2, RHD_R500},
    { 0x1002, 0x71C3, 2, RHD_R500},
    { 0x1002, 0x71C4, 2, RHD_R500},
    { 0x1002, 0x71C5, 2, RHD_R500},
    { 0x1002, 0x71C6, 2, RHD_R500},
    { 0x1002, 0x71C7, 2, RHD_R500},
    { 0x1002, 0x71CD, 2, RHD_R500},
    { 0x1002, 0x71CE, 2, RHD_R500},
    { 0x1002, 0x71D2, 2, RHD_R500},
    { 0x1002, 0x71D4, 2, RHD_R500},
    { 0x1002, 0x71D5, 2, RHD_R500},
    { 0x1002, 0x71D6, 2, RHD_R500},
    { 0x1002, 0x71DA, 2, RHD_R500},
    { 0x1002, 0x71DE, 2, RHD_R500},
    { 0x1002, 0x7200, 2, RHD_R500},
    { 0x1002, 0x7210, 2, RHD_R500},
    { 0x1002, 0x7211, 2, RHD_R500},
    { 0x1002, 0x7240, 2, RHD_R500},
    { 0x1002, 0x7243, 2, RHD_R500},
    { 0x1002, 0x7244, 2, RHD_R500},
    { 0x1002, 0x7245, 2, RHD_R500},
    { 0x1002, 0x7246, 2, RHD_R500},
    { 0x1002, 0x7247, 2, RHD_R500},
    { 0x1002, 0x7248, 2, RHD_R500},
    { 0x1002, 0x7249, 2, RHD_R500},
    { 0x1002, 0x724A, 2, RHD_R500},
    { 0x1002, 0x724B, 2, RHD_R500},
    { 0x1002, 0x724C, 2, RHD_R500},
    { 0x1002, 0x724D, 2, RHD_R500},
    { 0x1002, 0x724E, 2, RHD_R500},
    { 0x1002, 0x724F, 2, RHD_R500},
    { 0x1002, 0x7280, 2, RHD_R500},
    { 0x1002, 0x7281, 2, RHD_R500},
    { 0x1002, 0x7283, 2, RHD_R500},
    { 0x1002, 0x7284, 2, RHD_R500},
    { 0x1002, 0x7287, 2, RHD_R500},
    { 0x1002, 0x7288, 2, RHD_R500},
    { 0x1002, 0x7289, 2, RHD_R500},
    { 0x1002, 0x728B, 2, RHD_R500},
    { 0x1002, 0x728C, 2, RHD_R500},
    { 0x1002, 0x7290, 2, RHD_R500},
    { 0x1002, 0x7291, 2, RHD_R500},
    { 0x1002, 0x7293, 2, RHD_R500},
    { 0x1002, 0x7297, 2, RHD_R500},
    { 0x1002, 0x791E, 2, RHD_RS690},
    { 0x1002, 0x791F, 2, RHD_RS690},
    { 0x1002, 0x796C, 2, RHD_R500},
    { 0x1002, 0x796D, 2, RHD_R500},
    { 0x1002, 0x796E, 2, RHD_R500},
    { 0x1002, 0x796F, 2, RHD_R500},
    { 0x1002, 0x9400, 2, RHD_R600},
    { 0x1002, 0x9401, 2, RHD_R600},
    { 0x1002, 0x9402, 2, RHD_R600},
    { 0x1002, 0x9403, 2, RHD_R600},
    { 0x1002, 0x94C0, 2, RHD_R600},
    { 0x1002, 0x94C1, 2, RHD_R600},
    { 0x1002, 0x94C3, 2, RHD_R600},
    { 0x1002, 0x94C4, 2, RHD_R600},
    { 0x1002, 0x94C6, 2, RHD_R600},
    { 0x1002, 0x94C7, 2, RHD_R600},
    { 0x1002, 0x94C8, 2, RHD_R600},
    { 0x1002, 0x94C9, 2, RHD_R600},
    { 0x1002, 0x94CB, 2, RHD_R600},
    { 0x1002, 0x9580, 2, RHD_R600},
    { 0x1002, 0x9581, 2, RHD_R600},
    { 0x1002, 0x9583, 2, RHD_R600},
    { 0x1002, 0x9586, 2, RHD_R600},
    { 0x1002, 0x9587, 2, RHD_R600},
    { 0x1002, 0x9588, 2, RHD_R600},
    { 0x1002, 0x9589, 2, RHD_R600},
    { 0x1002, 0x958A, 2, RHD_R600},
    { 0x1002, 0x958B, 2, RHD_R600},
    { 0, 0, 0, 0 }
};

/*
 *
 */
static struct pci_dev *
DeviceLocate(struct pci_dev *devices, int bus, int dev, int func)
{
    struct pci_dev *device;

    for (device = devices; device; device = device->next)
	if ((device->bus == bus) && (device->dev == dev) &&
	    (device->func == func))
	    return device;
    return NULL;
}

/*
 *
 */
static struct RHDDevice *
DeviceMatch(struct pci_dev *device)
{
    int i;

    for (i = 0; rhdDevices[i].vendor; i++)
	if ((rhdDevices[i].vendor == device->vendor_id) &&
	    (rhdDevices[i].device == device->device_id))
	    return (rhdDevices + i);

    return NULL;
}

/*
 *
 */
static void *
MapBar(struct pci_dev *device, int ioBar, int devMem)
{
    void *map;

    if (!device->base_addr[ioBar] || !device->size[ioBar])
	return NULL;

    map = mmap(0, device->size[ioBar], PROT_WRITE | PROT_READ, MAP_SHARED,
	       devMem, device->base_addr[ioBar]);
    /* printf("Mapped IO at 0x%08llX (BAR %1d: 0x%08llX)\n",
       device->base_addr[io_bar], io_bar, device->size[io_bar]); */

    return map;
}

/*
 *
 */
CARD32
RegRead(void *map, int offset)
{
    return *(volatile CARD32 *)((CARD8 *) map + offset);
}

/*
 *
 */
void
RegWrite(void *map, int offset, CARD32 value)
{
    *(volatile CARD32 *)((CARD8 *) map + offset) = value;
}

/*
 *
 */
void
RegMask(void *map, int offset, CARD32 value, CARD32 mask)
{
    CARD32 tmp;

    tmp = RegRead(map, offset);
    tmp &= ~mask;
    tmp |= (value & mask);
    RegWrite(map, offset, tmp);
}

/*
 *
 */
static void
HPDReport(void *map)
{
    int HPD = RegRead(map, DC_GPIO_HPD_Y);

    printf("  HotPlug:");
    if (!(HPD & 0x0101) && !((ChipType == RHD_R600) && (HPD & 0x00010000)))
	printf(" RHD_HPD_NONE ");
    else {
	if (HPD & 0x1)
	    printf(" RHD_HPD_0");

	if (HPD & 0x100)
	    printf(" RHD_HPD_1");

	if ((ChipType == RHD_R600) && (HPD & 0x00010000))
	    printf(" RHD_HPD_2");
    }
    printf("\n");
}

/*
 *
 */
static Bool
DACALoadDetect(void *map)
{
    CARD32 CompEnable, Control1, Control2, DetectControl, Enable;
    CARD8 ret;

    CompEnable = RegRead(map, DACA_COMPARATOR_ENABLE);
    Control1 = RegRead(map, DACA_CONTROL1);
    Control2 = RegRead(map, DACA_CONTROL2);
    DetectControl = RegRead(map, DACA_AUTODETECT_CONTROL);
    Enable = RegRead(map, DACA_ENABLE);

    RegWrite(map, DACA_ENABLE, 1);
    RegMask(map, DACA_AUTODETECT_CONTROL, 0, 0x3);
    RegMask(map, DACA_CONTROL2, 0, 0x1);

    RegMask(map, DACA_CONTROL2, 0, 0x100);

    RegWrite(map, DACA_FORCE_DATA, 0);
    RegMask(map, DACA_CONTROL2, 0x1, 0x1);

    RegMask(map, DACA_COMPARATOR_ENABLE, 0x00070000, 0x00070000);
    RegWrite(map, DACA_CONTROL1, 0x00050802);
    RegMask(map, DACA_POWERDOWN, 0, 0x1); /* Shut down Bandgap Voltage Reference Power */
    usleep(5);

    RegMask(map, DACA_POWERDOWN, 0, 0x01010100); /* Shut down RGB */

    RegWrite(map, DACA_FORCE_DATA, 0x1e6); /* 486 out of 1024 */
    usleep(200);

    RegMask(map, DACA_POWERDOWN, 0x01010100, 0x01010100); /* Enable RGB */
    usleep(88);

    RegMask(map, DACA_POWERDOWN, 0, 0x01010100); /* Shut down RGB */

    RegMask(map, DACA_COMPARATOR_ENABLE, 0x100, 0x100);
    usleep(100);

    /* Get RGB detect values
     * If only G is detected, we could have a monochrome monitor,
     * but we don't bother with this at the moment.
     */
    ret = (RegRead(map, DACA_COMPARATOR_OUTPUT) & 0x0E) >> 1;

    RegMask(map, DACA_COMPARATOR_ENABLE, CompEnable, 0x00FFFFFF);
    RegWrite(map, DACA_CONTROL1, Control1);
    RegMask(map, DACA_CONTROL2, Control2, 0x1FF);
    RegMask(map, DACA_AUTODETECT_CONTROL, DetectControl, 0xFF);
    RegMask(map, DACA_ENABLE, Enable, 0xFF);

    return (ret & 0x07);
}

/*
 *
 */
static Bool
DACBLoadDetect(void *map)
{
    CARD32 CompEnable, Control1, Control2, DetectControl, Enable;
    CARD8  ret;

    CompEnable = RegRead(map, DACB_COMPARATOR_ENABLE);
    Control1 = RegRead(map, DACB_CONTROL1);
    Control2 = RegRead(map, DACB_CONTROL2);
    DetectControl = RegRead(map, DACB_AUTODETECT_CONTROL);
    Enable = RegRead(map, DACB_ENABLE);

    RegWrite(map, DACB_ENABLE, 1);
    RegMask(map, DACB_AUTODETECT_CONTROL, 0, 0x3);
    RegMask(map, DACB_CONTROL2, 0, 0x1);

    RegMask(map, DACB_CONTROL2, 0, 0x100);

    RegWrite(map, DACB_FORCE_DATA, 0);
    RegMask(map, DACB_CONTROL2, 0x1, 0x1);

    RegMask(map, DACB_COMPARATOR_ENABLE, 0x00070000, 0x00070000);
    RegWrite(map, DACB_CONTROL1, 0x50802);
    RegMask(map, DACB_POWERDOWN, 0, 0x1); /* Shut down Bandgap Voltage Reference Power */
    usleep(5);

    RegMask(map, DACB_POWERDOWN, 0, 0x01010100); /* Shut down RGB */

    RegWrite(map, DACB_FORCE_DATA, 0x1e6); /* 486 out of 1024 */
    usleep(200);

    RegMask(map, DACB_POWERDOWN, 0x01010100, 0x01010100); /* Enable RGB */
    usleep(88);

    RegMask(map, DACB_POWERDOWN, 0, 0x01010100); /* Shut down RGB */

    RegMask(map, DACB_COMPARATOR_ENABLE, 0x100, 0x100);
    usleep(100);

    /*
     * Get RGB detect values
     * If only G is detected, we could have a monochrome monitor,
     * but we don't bother with this at the moment.
     */
    ret = (RegRead(map, DACB_COMPARATOR_OUTPUT) & 0x0E) >> 1;

    RegMask(map, DACB_COMPARATOR_ENABLE, CompEnable, 0xFFFFFF);
    RegWrite(map, DACB_CONTROL1, Control1);
    RegMask(map, DACB_CONTROL2, Control2, 0x1FF);
    RegMask(map, DACB_AUTODETECT_CONTROL, DetectControl, 0xFF);
    RegMask(map, DACB_ENABLE, Enable, 0xFF);

    return (ret & 0x07);
}

/*
 *
 */
static Bool
TMDSALoadDetect(void *map)
{
    CARD32 Enable, Control, Detect;
    Bool ret;

    Enable = RegRead(map, TMDSA_TRANSMITTER_ENABLE);
    Control = RegRead(map, TMDSA_TRANSMITTER_CONTROL);
    Detect = RegRead(map, TMDSA_LOAD_DETECT);

    /* r500 needs a tiny bit more work :) */
    if (ChipType < RHD_R600) {
	RegMask(map, TMDSA_TRANSMITTER_ENABLE, 0x3, 0x3);
	RegMask(map, TMDSA_TRANSMITTER_CONTROL, 0x1, 0x3);
    }

    RegMask(map, TMDSA_LOAD_DETECT, 0x1, 0x1);
    usleep(1);
    ret = RegRead(map, TMDSA_LOAD_DETECT) & 0x10;
    RegMask(map, TMDSA_LOAD_DETECT, Detect, 0x1);

    if (ChipType < RHD_R600) {
	RegWrite(map, TMDSA_TRANSMITTER_ENABLE, Enable);
	RegWrite(map, TMDSA_TRANSMITTER_CONTROL, Control);
    }

    return ret;
}

/*
 *
 */
static void
LoadReport(void *map)
{
    Bool DACA, DACB, TMDSA;

    DACA = DACALoadDetect(map);
    DACB = DACBLoadDetect(map);
    TMDSA =TMDSALoadDetect(map);

    printf("  Load Detection:");
    if (!DACA && !DACB && !TMDSA)
	printf(" RHD_OUTPUT_NONE ");
    else {
	if (DACA)
	    printf(" RHD_OUTPUT_DACA");

	if (DACB)
	    printf(" RHD_OUTPUT_DACB");

	if (TMDSA)
	    printf(" RHD_OUTPUT_TMDSA");
    }
    printf("\n");
}

/*
 *
 */
CARD32
getDDCSpeed(void)
{
    CARD32 clock, ret;

    switch  (AtomData.FirmwareInfoVersion.crev) {
	case 1:
	    clock = AtomData.FirmwareInfo.FirmwareInfo->ulDefaultEngineClock;
	    break;
	case 2:
	    clock = AtomData.FirmwareInfo.FirmwareInfo_V_1_2->ulDefaultEngineClock;
	    break;
	case 3:
	    clock = AtomData.FirmwareInfo.FirmwareInfo_V_1_3->ulDefaultEngineClock;
	    break;
	case 4:
	    clock = AtomData.FirmwareInfo.FirmwareInfo_V_1_4->ulDefaultEngineClock;
	    break;
	default:
	    /* no AtomBIOS info; use save default */
	    clock = 70000;
    }
    clock *= 10;

    switch (ChipType) {
	case RHD_R500:
	case RHD_RS690:
	    ret = (0x7F << 8)
		+ (clock) / (4 * 0x7F * TARGET_HW_I2C_CLOCK);
	    break;
	case RHD_R600:
	    ret = (clock) / TARGET_HW_I2C_CLOCK;
	    break;
	default:
	    ret = 0;
    }
#ifdef DEBUG
    printf("%s: Clock: %i Prescale: 0x%x\n",__func__,clock,ret);
#endif
    return ret;
}

/*
 * R600 DDC defines.
 */
enum _r6xxI2CBits {
    /* R6_DC_I2C_TRANSACTION0 */
    R6_DC_I2C_RW0   = (0x1 << 0),
    R6_DC_I2C_STOP_ON_NACK0         = (0x1 << 8),
    R6_DC_I2C_ACK_ON_READ0  = (0x1 << 9),
    R6_DC_I2C_START0        = (0x1 << 12),
    R6_DC_I2C_STOP0         = (0x1 << 13),
    R6_DC_I2C_COUNT0        = (0xff << 16),
    /* R6_DC_I2C_TRANSACTION1 */
    R6_DC_I2C_RW1   = (0x1 << 0),
    R6_DC_I2C_STOP_ON_NACK1         = (0x1 << 8),
    R6_DC_I2C_ACK_ON_READ1  = (0x1 << 9),
    R6_DC_I2C_START1        = (0x1 << 12),
    R6_DC_I2C_STOP1         = (0x1 << 13),
    R6_DC_I2C_COUNT1        = (0xff << 16),
    /* R6_DC_I2C_DATA */
    R6_DC_I2C_DATA_RW       = (0x1 << 0),
    R6_DC_I2C_DATA_BIT      = (0xff << 8),
    R6_DC_I2C_INDEX         = (0xff << 16),
    R6_DC_I2C_INDEX_WRITE   = (0x1 << 31),
    /* R6_DC_I2C_CONTROL */
    R6_DC_I2C_GO    = (0x1 << 0),
    R6_DC_I2C_SOFT_RESET    = (0x1 << 1),
    R6_DC_I2C_SEND_RESET    = (0x1 << 2),
    R6_DC_I2C_SW_STATUS_RESET       = (0x1 << 3),
    R6_DC_I2C_SDVO_EN       = (0x1 << 4),
    R6_DC_I2C_SDVO_ADDR_SEL         = (0x1 << 6),
    R6_DC_I2C_DDC_SELECT    = (0x7 << 8),
    R6_DC_I2C_TRANSACTION_COUNT     = (0x3 << 20),
    R6_DC_I2C_SW_DONE_INT   = (0x1 << 0),
    R6_DC_I2C_SW_DONE_ACK   = (0x1 << 1),
    R6_DC_I2C_SW_DONE_MASK  = (0x1 << 2),
    R6_DC_I2C_DDC1_HW_DONE_INT      = (0x1 << 4),
    R6_DC_I2C_DDC1_HW_DONE_ACK      = (0x1 << 5),
    R6_DC_I2C_DDC1_HW_DONE_MASK     = (0x1 << 6),
    R6_DC_I2C_DDC2_HW_DONE_INT      = (0x1 << 8),
    R6_DC_I2C_DDC2_HW_DONE_ACK      = (0x1 << 9),
    R6_DC_I2C_DDC2_HW_DONE_MASK     = (0x1 << 10),
    R6_DC_I2C_DDC3_HW_DONE_INT      = (0x1 << 12),
    R6_DC_I2C_DDC3_HW_DONE_ACK      = (0x1 << 13),
    R6_DC_I2C_DDC3_HW_DONE_MASK     = (0x1 << 14),
    R6_DC_I2C_DDC4_HW_DONE_INT      = (0x1 << 16),
    R6_DC_I2C_DDC4_HW_DONE_ACK      = (0x1 << 17),
    R6_DC_I2C_DDC4_HW_DONE_MASK     = (0x1 << 18),
    /* R6_DC_I2C_SW_STATUS */
    R6_DC_I2C_SW_STATUS_BIT         = (0x3 << 0),
    R6_DC_I2C_SW_DONE       = (0x1 << 2),
    R6_DC_I2C_SW_ABORTED    = (0x1 << 4),
    R6_DC_I2C_SW_TIMEOUT    = (0x1 << 5),
    R6_DC_I2C_SW_INTERRUPTED        = (0x1 << 6),
    R6_DC_I2C_SW_BUFFER_OVERFLOW    = (0x1 << 7),
    R6_DC_I2C_SW_STOPPED_ON_NACK    = (0x1 << 8),
    R6_DC_I2C_SW_SDVO_NACK  = (0x1 << 10),
    R6_DC_I2C_SW_NACK0      = (0x1 << 12),
    R6_DC_I2C_SW_NACK1      = (0x1 << 13),
    R6_DC_I2C_SW_NACK2      = (0x1 << 14),
    R6_DC_I2C_SW_NACK3      = (0x1 << 15),
    R6_DC_I2C_SW_REQ        = (0x1 << 18)
};

/*
 *
 */
static Bool
R6xxI2CSetupStatus(void *map, int channel)
{
    channel &= 0xf;
    CARD16 i2c_speed;

    i2c_speed = getDDCSpeed();
    if (!i2c_speed)
	return FALSE;

    switch (channel) {
    case 0:
	RegMask(map, DC_GPIO_DDC1_MASK, 0x0, 0xffff);
	RegMask(map, DC_GPIO_DDC1_A, 0x0, 0xffff);
	RegMask(map, DC_GPIO_DDC1_EN, 0x0, 0xffff);
	RegMask(map, R6_DC_I2C_DDC1_SPEED, (i2c_speed << 16) | 2,
		0xFFFF00FF);
	RegWrite(map, R6_DC_I2C_DDC1_SETUP, 0x30000000);
	break;
    case 1:
	RegMask(map, DC_GPIO_DDC2_MASK, 0x0, 0xffff);
	RegMask(map, DC_GPIO_DDC2_A, 0x0, 0xffff);
	RegMask(map, DC_GPIO_DDC2_EN, 0x0, 0xffff);
	RegMask(map, R6_DC_I2C_DDC2_SPEED, (i2c_speed << 16) | 2,
		0xffff00ff);
	RegWrite(map, R6_DC_I2C_DDC2_SETUP, 0x30000000);
	break;
    case 2:
	RegMask(map, DC_GPIO_DDC3_MASK, 0x0, 0xffff);
	RegMask(map, DC_GPIO_DDC3_A, 0x0, 0xffff);
	RegMask(map, DC_GPIO_DDC3_EN, 0x0, 0xffff);
	RegMask(map, R6_DC_I2C_DDC3_SPEED, (i2c_speed << 16) | 2,
		0xffff00ff);
	RegWrite(map, R6_DC_I2C_DDC3_SETUP, 0x30000000);
	break;
    case 3:
	RegMask(map, DC_GPIO_DDC4_MASK, 0x0, 0xffff);
	RegMask(map, DC_GPIO_DDC4_A, 0x0, 0xffff);
	RegMask(map, DC_GPIO_DDC4_EN, 0x0, 0xffff);
	RegMask(map, R6_DC_I2C_DDC4_SPEED, (i2c_speed << 16) | 2,
		0xffff00ff);
	RegWrite(map, R6_DC_I2C_DDC4_SETUP, 0x30000000);
	break;
    default:
	return FALSE;
    }
    RegWrite(map, R6_DC_I2C_CONTROL, channel << 8);
    RegMask(map, R6_DC_I2C_INTERRUPT_CONTROL, 0x2, 0x2);
    RegMask(map, R6_DC_I2C_ARBITRATION, 0, 0xff);
    return TRUE;
}

/*
 *
 */
static Bool
R6xxI2CStatus(void *map)
{
    int count = 800;
    CARD32 val;

    while (--count) {

	usleep(1000);

   	val = RegRead(map, R6_DC_I2C_SW_STATUS);
	if (val & R6_DC_I2C_SW_DONE)
	    break;
    }
    RegMask(map, R6_DC_I2C_INTERRUPT_CONTROL, R6_DC_I2C_SW_DONE_ACK,
	    R6_DC_I2C_SW_DONE_ACK);

#ifdef DEBUG
	fprintf(stderr, "I2CStatus: %x\n",val);
#endif
    if (!count || (val & (R6_DC_I2C_SW_STOPPED_ON_NACK
			  | R6_DC_I2C_SW_NACK0 | R6_DC_I2C_SW_NACK1 | 0x3)))
	return FALSE; /* 2 */
    return TRUE; /* 1 */
}

/*
 *
 */
static Bool
R6xxDDCProbe(void *map, int Channel, unsigned char slave)
{
    Bool ret = FALSE;
    CARD32 data;

    if (!R6xxI2CSetupStatus(map, Channel))
	return FALSE;

    RegMask(map, R6_DC_I2C_CONTROL, 0, 0x00300000); /* 1 Transaction */

    RegMask(map, R6_DC_I2C_TRANSACTION0, /* only slave */
	    R6_DC_I2C_STOP_ON_NACK0 | R6_DC_I2C_START0
	    | R6_DC_I2C_STOP0 | (0 << 16), 0x00ffffff);

    data = R6_DC_I2C_INDEX_WRITE | ( slave << 8 ) | (0 << 16);
	RegWrite(map, R6_DC_I2C_DATA, data);

    RegMask(map, R6_DC_I2C_CONTROL, R6_DC_I2C_GO, R6_DC_I2C_GO);

    ret = R6xxI2CStatus(map);

    RegMask(map, R6_DC_I2C_CONTROL, 0x2, 0xff);
    usleep(1000);
    RegWrite(map, R6_DC_I2C_CONTROL, 0);

    return ret;
}

enum _rhdRS69I2CBits {
    /* RS69_DC_I2C_TRANSACTION0 */
    RS69_DC_I2C_RW0   = (0x1 << 0),
    RS69_DC_I2C_STOP_ON_NACK0         = (0x1 << 8),
    RS69_DC_I2C_START0        = (0x1 << 12),
    RS69_DC_I2C_STOP0         = (0x1 << 13),
    /* RS69_DC_I2C_TRANSACTION1 */
    RS69_DC_I2C_RW1   = (0x1 << 0),
    RS69_DC_I2C_START1        = (0x1 << 12),
    RS69_DC_I2C_STOP1         = (0x1 << 13),
    /* RS69_DC_I2C_DATA */
    RS69_DC_I2C_DATA_RW       = (0x1 << 0),
    RS69_DC_I2C_INDEX_WRITE   = (0x1 << 31),
    /* RS69_DC_I2C_CONTROL */
    RS69_DC_I2C_GO    = (0x1 << 0),
    RS69_DC_I2C_TRANSACTION_COUNT     = (0x3 << 20),
    RS69_DC_I2C_SW_DONE_ACK   = (0x1 << 1),
    /* RS69_DC_I2C_SW_STATUS */
    RS69_DC_I2C_SW_DONE       = (0x1 << 2),
    RS69_DC_I2C_SW_STOPPED_ON_NACK    = (0x1 << 8),
    RS69_DC_I2C_SW_NACK0      = (0x1 << 12),
    RS69_DC_I2C_SW_NACK1      = (0x1 << 13)
};

/*
 *
 */
static Bool
RS69I2CStatus(void *map)
{
    int count = 800;
    volatile CARD32 val;

    while (--count) {

	usleep(10);
	val = RegRead(map, RS69_DC_I2C_SW_STATUS);
#ifdef DEBUG
	fprintf(stderr,"I2CStatus : 0x%x %i\n",(unsigned int)val,count);
#endif
	if (val & RS69_DC_I2C_SW_DONE)
	    break;
    }
    RegMask(map, RS69_DC_I2C_INTERRUPT_CONTROL, RS69_DC_I2C_SW_DONE_ACK,
	    RS69_DC_I2C_SW_DONE_ACK);
    if (!count || (val & (RS69_DC_I2C_SW_STOPPED_ON_NACK
			  | RS69_DC_I2C_SW_NACK0 | RS69_DC_I2C_SW_NACK1
			  | 0x3)))
	return FALSE; /* 2 */
    return TRUE; /* 1 */
}

/*
 *
 */
static Bool
RS69I2CSetupStatus(void *map, int line)
{
    CARD32 ddc;
    CARD16 prescale;

    prescale = getDDCSpeed();
    if (!prescale)
	return FALSE;

    RegMask(map, 0x28, 0x200, 0x200);
    RegMask(map, RS69_DC_I2C_UNKNOWN_1, prescale << 16 | 0x2, 0xffff00ff);
    /* add SDVO handling later */
    switch (AtomData.GPIO_I2C_Info->asGPIO_Info[line & 0xf]
	    .usClkMaskRegisterIndex) {
	case 0x1f90:
	    ddc = 0; /* ddc1 */
	    break;
	case 0x1f94: /* ddc2 */
	    ddc = 1;
	    break;
	default:
	    ddc = 2; /* ddc3 */
	    break;
    }
#ifdef DEBUG
    printf("DDC: line: %i -> %i port: %x\n",line,ddc,
	   AtomData.GPIO_I2C_Info->asGPIO_Info[line & 0xf]
	   .usClkMaskRegisterIndex);
#endif
    RegMask(map, RS69_DC_I2C_CONTROL, ddc << 8, 0xff << 8);
    RegWrite(map, RS69_DC_I2C_DDC_SETUP_Q, 0x30000000);
    RegMask(map, RS69_DC_I2C_CONTROL, (line & 0x3) << 16, 0xff << 16);
    RegMask(map, RS69_DC_I2C_INTERRUPT_CONTROL, 0x2, 0x2);
    RegMask(map, RS69_DC_I2C_UNKNOWN_2, 0x2, 0xff);

    return TRUE;
}

/*
 *
 */
static Bool
RS69DDCProbe(void *map, int Channel, unsigned char slave)
{
    Bool ret = FALSE;
    CARD32 data;

    if (!RS69I2CSetupStatus(map, Channel))
	return FALSE;

    RegMask(map, RS69_DC_I2C_CONTROL, 0, RS69_DC_I2C_TRANSACTION_COUNT); /* 1 Transaction */

    RegMask(map, RS69_DC_I2C_TRANSACTION0, /* only slave */
	    RS69_DC_I2C_STOP_ON_NACK0 | RS69_DC_I2C_START0
	    | RS69_DC_I2C_STOP0 | (0 << 16), 0x00ffffff);

    data = RS69_DC_I2C_INDEX_WRITE | ( slave << 8 ) | (0 << 16);
    RegWrite(map, RS69_DC_I2C_DATA, data);

    RegMask(map, RS69_DC_I2C_CONTROL, RS69_DC_I2C_GO, RS69_DC_I2C_GO);

    ret = RS69I2CStatus(map);

    RegMask(map, RS69_DC_I2C_CONTROL, 0x2, 0xff);
    usleep(1000);
    RegWrite(map, RS69_DC_I2C_CONTROL, 0);

    return ret;
}

enum _rhdR5xxI2CBits {
 /* R5_DC_I2C_STATUS1 */
    R5_DC_I2C_DONE	 = (0x1 << 0),
    R5_DC_I2C_NACK	 = (0x1 << 1),
    R5_DC_I2C_HALT	 = (0x1 << 2),
    R5_DC_I2C_GO	 = (0x1 << 3),
 /* R5_DC_I2C_RESET */
    R5_DC_I2C_SOFT_RESET	 = (0x1 << 0),
    R5_DC_I2C_ABORT	 = (0x1 << 8),
 /* R5_DC_I2C_CONTROL1 */
    R5_DC_I2C_START	 = (0x1 << 0),
    R5_DC_I2C_STOP	 = (0x1 << 1),
    R5_DC_I2C_RECEIVE	 = (0x1 << 2),
    R5_DC_I2C_EN	 = (0x1 << 8),
    R5_DC_I2C_PIN_SELECT	 = (0x3 << 16),
 /* R5_DC_I2C_CONTROL2 */
    R5_DC_I2C_ADDR_COUNT	 = (0x7 << 0),
    R5_DC_I2C_DATA_COUNT	 = (0xf << 8),
    R5_DC_I2C_PRESCALE_LOWER	 = (0xff << 16),
    R5_DC_I2C_PRESCALE_UPPER	 = (0xff << 24),
 /* R5_DC_I2C_CONTROL3 */
    R5_DC_I2C_DATA_DRIVE_EN	 = (0x1 << 0),
    R5_DC_I2C_DATA_DRIVE_SEL	 = (0x1 << 1),
    R5_DC_I2C_CLK_DRIVE_EN	 = (0x1 << 7),
    R5_DC_I2C_RD_INTRA_BYTE_DELAY	 = (0xff << 8),
    R5_DC_I2C_WR_INTRA_BYTE_DELAY	 = (0xff << 16),
    R5_DC_I2C_TIME_LIMIT	 = (0xff << 24),
 /* R5_DC_I2C_DATA */
    R5_DC_I2C_DATA_BIT	 = (0xff << 0),
 /* R5_DC_I2C_INTERRUPT_CONTROL */
    R5_DC_I2C_INTERRUPT_STATUS	 = (0x1 << 0),
    R5_DC_I2C_INTERRUPT_AK	 = (0x1 << 8),
    R5_DC_I2C_INTERRUPT_ENABLE	 = (0x1 << 16),
 /* R5_DC_I2C_ARBITRATION */
    R5_DC_I2C_SW_WANTS_TO_USE_I2C	 = (0x1 << 0),
    R5_DC_I2C_SW_CAN_USE_I2C	 = (0x1 << 1),
    R5_DC_I2C_SW_DONE_USING_I2C	 = (0x1 << 8),
    R5_DC_I2C_HW_NEEDS_I2C	 = (0x1 << 9),
    R5_DC_I2C_ABORT_HDCP_I2C	 = (0x1 << 16),
    R5_DC_I2C_HW_USING_I2C	 = (0x1 << 17)
};

/*
 *
 */
static Bool
R5xxI2CStatus(void *map)
{
    int count = 800;
    CARD32 res;

    while (count-- != 0) {
	usleep (1000);

	if (((RegRead(map, R5_DC_I2C_STATUS1))
	     & R5_DC_I2C_GO) != 0)
	    continue;
	res = RegRead(map, R5_DC_I2C_STATUS1);
#ifdef DEBUG
	fprintf(stderr, "I2CStatus: %x\n",res);
#endif
	if (res & R5_DC_I2C_DONE)
	    return TRUE;
	else
	    return FALSE;
    }

    RegMask(map, R5_DC_I2C_RESET, R5_DC_I2C_ABORT, 0xff00);
    return FALSE;
}

/*
 *
 */
static Bool
R5xxDDCProbe(void *map, int Channel, unsigned char slave)
{
    Bool ret = FALSE;
    CARD32 SaveControl1, save_494;
    CARD16 prescale;

    prescale = getDDCSpeed();
    if (!prescale)
	return FALSE;

    RegMask(map, 0x28, 0x200, 0x200);

    SaveControl1 = RegRead(map, R5_DC_I2C_CONTROL1);
    save_494 = RegRead(map, 0x494);
    RegMask(map, 0x494, 1, 1);

    RegMask(map, R5_DC_I2C_ARBITRATION, R5_DC_I2C_SW_WANTS_TO_USE_I2C,
	    R5_DC_I2C_SW_WANTS_TO_USE_I2C);

    RegMask(map, R5_DC_I2C_STATUS1, R5_DC_I2C_DONE
	       | R5_DC_I2C_NACK
	       | R5_DC_I2C_HALT, 0xff);
    RegMask(map, R5_DC_I2C_RESET, R5_DC_I2C_SOFT_RESET, 0xffff);
    RegWrite(map, R5_DC_I2C_RESET, 0);

    RegMask(map, R5_DC_I2C_CONTROL1,
	    (Channel & 0x0f) << 16 | R5_DC_I2C_EN,
	    R5_DC_I2C_PIN_SELECT | R5_DC_I2C_EN);
    /* addr_count = 1; data_count = 1 */
    RegWrite(map, R5_DC_I2C_CONTROL2, prescale << 16 | 0x101);
    /* time limit 30 */
    RegMask(map, R5_DC_I2C_CONTROL3, 0x30 << 24, 0xff << 24);

    RegWrite(map, R5_DC_I2C_DATA, slave);  /* slave */
    RegWrite(map, R5_DC_I2C_DATA, 0);

    RegMask(map, R5_DC_I2C_CONTROL1,
	    R5_DC_I2C_START | R5_DC_I2C_STOP, 0xff);
    RegMask(map, R5_DC_I2C_STATUS1, R5_DC_I2C_GO, 0xff);
    ret = R5xxI2CStatus(map);

    RegMask(map, R5_DC_I2C_STATUS1,
	    R5_DC_I2C_DONE
	    | R5_DC_I2C_NACK
	    | R5_DC_I2C_HALT, 0xff);
    RegMask(map, R5_DC_I2C_RESET, R5_DC_I2C_SOFT_RESET, 0xff);
    RegWrite(map,R5_DC_I2C_RESET, 0);

    RegMask(map, R5_DC_I2C_ARBITRATION,
	    R5_DC_I2C_SW_DONE_USING_I2C, 0xff00);

    RegWrite(map, R5_DC_I2C_CONTROL1, SaveControl1);
    RegWrite(map, 0x494, save_494);
    RegMask(map, 0x28, 0, 0x200);

    return ret;
}

/*
 *
 */
static Bool
DDCProbe(void *map, int Channel, unsigned char slave)
{
    switch (ChipType) {
	case RHD_R500:
	    return R5xxDDCProbe(map, Channel, slave);
	case RHD_RS690:
	    return RS69DDCProbe(map, Channel, slave);
	case RHD_R600:
	    return R6xxDDCProbe(map, Channel, slave);
	default:
	    return FALSE;
    }
}

/*
 *
 */
#define EDID_SLAVE 0xA0

static void
DDCReport(void *map)
{
    Bool Chan0, Chan1, Chan2, Chan3;

    Chan0 = DDCProbe(map, 0, EDID_SLAVE);
    Chan1 = DDCProbe(map, 1, EDID_SLAVE);
    Chan2 = DDCProbe(map, 2, EDID_SLAVE);
    if (ChipType >= RHD_R600)
	Chan3 = DDCProbe(map, 3, EDID_SLAVE);
    else
	Chan3 = FALSE;

    printf("  DDC:");
    if (!Chan0 && !Chan1 && !Chan2 && !Chan3)
	printf(" RHD_DDC_NONE ");
    else {
	if (Chan0)
	    printf(" RHD_DDC_0");

	if (Chan1)
	    printf(" RHD_DDC_1");

	if (Chan2)
	    printf(" RHD_DDC_2");

	if (Chan3)
	    printf(" RHD_DDC_3");
    }
    printf("\n");
}

/*
 *
 */
static void
DDCScanBus(void *map)
{
    int channel;
    unsigned char slave;
    int max_chan = ((ChipType >= RHD_R600) ? 3 : 2);

    for (channel = 0; channel < max_chan; channel ++) {
	int state = 0;

	for (slave = 0x8; slave < 0x78; slave++ ) {

	    if (DDCProbe(map, channel, slave << 1)) {
		if (state == 0) {
		    printf("  DDC Line[%i]: Slaves: ", channel);
		    state = 1;
		}
		printf("%x ", slave << 1);
	    }
	}
	if (state == 1)
	    printf("\n");
    }
}

/*
 *
 */
static void
LVDSReport(void *map)
{
    Bool Bits24 = FALSE, DualLink = FALSE, Fpdi = FALSE;

    if (ChipType == RHD_R600) {
	/* printf("No information for LVTMA on R600 has been made available yet.\n"); */
	return;
    }

    if (!(RegRead(map, LVTMA_CNTL) & 0x1) ||
	(RegRead(map, LVTMA_MODE) & 0x1))
	return;

    printf("  LVDS Info:\n");

    DualLink = RegRead(map, LVTMA_CNTL) & 0x01000000;
    Bits24 = RegRead(map, LVTMA_LVDS_DATA_CNTL) & 0x1;
    Fpdi = RegRead(map, LVTMA_LVDS_DATA_CNTL) & 0x1;

    printf("\t%dbits, %s link, %s Panel found.\n",
	   Bits24 ? 24 : 18,
	   DualLink ? "dual" : "single",
	   Fpdi ? "FPDI" : "LDI");

    printf("\tPower Timing: 0x%03X, 0x%03X, 0x%02X, 0x%02X, 0x%03X\n",
	   RegRead(map, LVTMA_PWRSEQ_REF_DIV) & 0xFFF,
	   (RegRead(map, LVTMA_PWRSEQ_REF_DIV) >> 16) & 0xFFF,
	   ((RegRead(map, LVTMA_PWRSEQ_DELAY1) & 0xFF) * 2 + 1) / 5,
	   (((RegRead(map, LVTMA_PWRSEQ_DELAY1) >> 8) & 0xFF) * 2 + 1)/ 5,
	   (RegRead(map, LVTMA_PWRSEQ_DELAY2) & 0xFFF) << 2);

    printf("\tMacro: 0x%08X, Clock Pattern: 0x%04X\n",
	   RegRead(map, LVTMA_MACRO_CONTROL),
	   (RegRead(map, LVTMA_TRANSMITTER_CONTROL) >> 16) & 0x3FF);
}

/*
 *
 */
Bool
WriteToFile(char *name, unsigned char *buffer, int size)

{
    int fd = open(name, O_CREAT | O_TRUNC | O_WRONLY,S_IRUSR | S_IWUSR);
    int ct = 0;

    if (fd < 0) {
	fprintf(stderr,"Cannot open file %s: %s\n",name,strerror(errno));
	goto error;
    } else {
	while (1) {
	    int ret = write(fd, buffer + ct, size - ct);
	    if (ret < 0) {
		if (errno == EAGAIN || errno == EINVAL)
		    continue;
		else {
		    fprintf(stderr,"Cannot write output file: %s\n",
			    strerror(errno));
		    close (fd);
		    goto error;
		}
	    } else {
		ct += ret;
		if (ct == size)
		    break;
	    }
	}
	close (fd);
	return TRUE;
    }
    error:
	return FALSE;
}

/*
 *
 */
unsigned char *
GetVBIOS(int *size)
{
    int i;
    unsigned char *rombase;
    char chksm = 0;
    int saved_errno;
    int fd;

    if ((fd = open(DEV_MEM, O_RDONLY)) < 0) {
	fprintf(stderr,"Cannot open " DEV_MEM " (%s),\n",strerror(errno));
	return FALSE;
    }
    rombase = mmap((caddr_t)0, VBIOS_MAXSIZE, PROT_READ, MAP_SHARED, fd,
		   VBIOS_BASE);
    saved_errno = errno;

    close (fd);

    if (rombase == MAP_FAILED) {
	fprintf(stderr,"Cannot map (0x%08x:0x%x) (%s)\n",VBIOS_BASE,
		 VBIOS_MAXSIZE,
		 strerror(saved_errno));
	return FALSE;
    }

    if (rombase[0] != 0x55 || rombase[1] != 0xaa) {
	fprintf(stderr,"No BIOS Signature found!\n");
    } else {
	*size = rombase[2] * 512;
	for (i = 0; i < *size; i++) {
	    chksm += rombase[i];
	}
	if (chksm)
	    fprintf(stderr,"Warning: VBIOS chksum incorrect!\n");
    }
    return rombase;
}

/*
 *
 */
void
FreeVBIOS(unsigned char *rombase, int size)
{
    munmap(rombase,size);
}

/*
 *
 */
static int
AnalyzeCommonHdr(ATOM_COMMON_TABLE_HEADER *hdr)
{
    if (hdr->usStructureSize == 0xaa55)
        return FALSE;

    return TRUE;
}

/*
 *
 */
static int
AnalyzeRomHdr(unsigned char *rombase,
              ATOM_ROM_HEADER *hdr,
              int *data_offset)
{
    if (AnalyzeCommonHdr(&hdr->sHeader) == -1) {
        return FALSE;
    }

    *data_offset = hdr->usMasterDataTableOffset;

    return TRUE;
}

/*
 *
 */
static int
AnalyzeRomDataTable(unsigned char *base, int offset,
                    void *ptr,short *size)
{
    ATOM_COMMON_TABLE_HEADER *table = (ATOM_COMMON_TABLE_HEADER *)
        (base + offset);

   if (!*size || AnalyzeCommonHdr(table) == -1) {
       if (*size) *size -= 2;
       *(void **)ptr = NULL;
       return FALSE;
   }
   *size -= 2;
   *(void **)ptr = (void *)(table);
   return TRUE;
}

/*
 *
 */
static Bool
GetAtomBiosTableRevisionAndSize(ATOM_COMMON_TABLE_HEADER *hdr,
                                   CARD8 *contentRev,
                                   CARD8 *formatRev,
                                   short *size)
{
    if (!hdr)
        return FALSE;

    if (contentRev) *contentRev = hdr->ucTableContentRevision;
    if (formatRev) *formatRev = hdr->ucTableFormatRevision;
    if (size) *size = (short)hdr->usStructureSize
                   - sizeof(ATOM_COMMON_TABLE_HEADER);
    return TRUE;
}

static Bool
AnalyzeMasterDataTable(unsigned char *base,
                       ATOM_MASTER_DATA_TABLE *table)
{
    ATOM_MASTER_LIST_OF_DATA_TABLES *data_table =
        &table->ListOfDataTables;
    short size;

    if (!AnalyzeCommonHdr(&table->sHeader))
        return FALSE;
    if (!GetAtomBiosTableRevisionAndSize(&table->sHeader,NULL,NULL,&size))
        return FALSE;

    AnalyzeRomDataTable(base,data_table->FirmwareInfo,&(AtomData.FirmwareInfo.base),&size);
    GetAtomBiosTableRevisionAndSize(AtomData.FirmwareInfo.base,
                                   &AtomData.FirmwareInfoVersion.crev,
                                   &AtomData.FirmwareInfoVersion.frev,
				    NULL);
    AnalyzeRomDataTable(base,data_table->GPIO_I2C_Info,&(AtomData.GPIO_I2C_Info),&size);
    GetAtomBiosTableRevisionAndSize((ATOM_COMMON_TABLE_HEADER *)AtomData.GPIO_I2C_Info,
                                   &AtomData.GPIO_I2C_InfoVersion.crev,
                                   &AtomData.GPIO_I2C_InfoVersion.frev,
				    NULL);

    return TRUE;
}

void
print_help(const char* progname, const char* message, const char* msgarg)
{
	if (message != NULL)
	    fprintf(stderr, "%s %s\n", message, msgarg);
	fprintf(stderr, "Usage: %s [options] PCI-tag\n"
			"       Options: -d: dumpBios\n"
			"                -s: scanDDCBus\n"
			"       PCI-tag: bus:dev.func\n\n",
		progname);
}

/*
 *
 */
static Bool
InterpretATOMBIOS(unsigned char *base)
{
    int  data_offset;
    unsigned short atom_romhdr_off =  *(unsigned short*)
        (base + OFFSET_TO_POINTER_TO_ATOM_ROM_HEADER);

    ATOM_ROM_HEADER *atom_rom_hdr =
        (ATOM_ROM_HEADER *)(base + atom_romhdr_off);
    if (memcmp("ATOM",&atom_rom_hdr->uaFirmWareSignature,4)) {
        fprintf(stderr,"No AtomBios signature found\n");
        return FALSE;
    }
    if (!AnalyzeRomHdr(base, atom_rom_hdr, &data_offset)) {
        fprintf(stderr, "RomHeader invalid\n");
        return FALSE;
    }
    if (!AnalyzeMasterDataTable(base, (ATOM_MASTER_DATA_TABLE *)
				   (base + data_offset))) {
        fprintf(stderr, "ROM Master Table invalid\n");
        return FALSE;
    }
    return TRUE;
}

/*
 *
 */
int
main(int argc, char *argv[])
{
    struct pci_dev *device;
    struct pci_access *pciAccess;
    struct RHDDevice *rhdDevice;
    int devMem;
    void *io;
    int bus, dev, func;
    int ret;
    int saved_errno;
    Bool dumpBios, deviceSet, scanDDCBus;
    int i;
    unsigned char *rombase;
    int size;

    printf("%s: v%s, %s\n",
	   "rhd_conntest", PACKAGE_VERSION, GIT_MESSAGE);

    /* init libpci */
    pciAccess = pci_alloc();
    pci_init(pciAccess);
    pci_scan_bus(pciAccess);

    if (argc < 2) {
	print_help(argv[0], "Missing argument: please provide a PCI tag\n",
		   "");
	return 1;
    }

    for (i = 1; i < argc; i++) {
	if (!strncmp("-d",argv[i],3)) {
	    dumpBios = TRUE;
	} else if (!strncmp("-s",argv[i],3)) {
	    scanDDCBus = TRUE;
	} else if (!strncmp("-",argv[i],1)) {
	    print_help(argv[0], "Unknown option", argv[i]);
	    return 1;
	} else {
	    ret = sscanf(argv[i], "%x:%x.%x", &bus, &dev, &func);
	    if (ret != 3) {
		ret = sscanf(argv[i], "%x:%x:%x", &bus, &dev, &func);
		if (ret != 3) {
		    ret = sscanf(argv[i], "%d:%d.%d", &bus, &dev, &func);
		    if (ret != 3)
			ret = sscanf(argv[i], "%d:%d:%d", &bus, &dev, &func);
		}
	    }
	    if (ret != 3) {
	        print_help(argv[0], "Unable to parse the PCI tag argument: ",
			   argv[i]);
	        return 1;
	    }
	    deviceSet = TRUE;
	}
    }

    if (deviceSet) {
	/* find our toy */
	device = DeviceLocate(pciAccess->devices, bus, dev, func);
	if (!device) {
	    fprintf(stderr, "Unable to find PCI device at %02X:%02X.%02X.\n",
		    bus, dev, func);
	    return 1;
	}

	rhdDevice = DeviceMatch(device);
	if (!rhdDevice) {
	    fprintf(stderr,
		    "Unknown device: 0x%04X:0x%04X (%02X:%02X.%02X).\n",
		    device->vendor_id, device->device_id, bus, dev, func);
	    return 1;
	}
    }

    rombase = GetVBIOS(&size);
    if (!rombase) {
	fprintf(stderr, "Cannot get VBIOS. Are we root?\n");
	return 1;
    }
    if (!InterpretATOMBIOS(rombase)) {
	fprintf(stderr, "Cannot analyze AtomBIOS\n");
	return 1;
    }

    if (dumpBios) {
	char name[1024] = "posted.vga.rom";

	if (deviceSet) {
	    snprintf(name, 1023, "%04X.%04X.%04X.vga.rom",
		     device->device_id,
		     pci_read_word(device, PCI_SUBSYSTEM_VENDOR_ID),
		     pci_read_word(device, PCI_SUBSYSTEM_ID));
	}
	WriteToFile(name, rombase, size);

    }

    if (!deviceSet)
	return 0;

    if (rhdDevice->bar > 5) {
	fprintf(stderr, "Program error: No acceptable BAR defined for this device.\n");
	return 1;
    }

    printf("Checking connectors on 0x%04X, 0x%04X, 0x%04X  (@%02X:%02X:%02X):\n",
	   device->device_id, pci_read_word(device, PCI_SUBSYSTEM_VENDOR_ID),
	   pci_read_word(device, PCI_SUBSYSTEM_ID),
	   device->bus, device->dev, device->func);

    /* make sure we can actually read DEV_MEM before we do anything else */
    devMem = open(DEV_MEM, O_RDWR);
    if (devMem < 0) {
	fprintf(stderr, "Unable to open "DEV_MEM": %s.\n", strerror(errno));
	return errno;
    }

    io = MapBar(device, rhdDevice->bar, devMem);
    saved_errno = errno;
    close (devMem);
    if (!io) {
	fprintf(stderr, "Unable to map IO memory: %s.\n",
		strerror(saved_errno));
	return 1;
    }

    ChipType = rhdDevice->type;

    LoadReport(io);
    HPDReport(io);
    DDCReport(io);

    LVDSReport(io);
    if (scanDDCBus)
	DDCScanBus(io);

    FreeVBIOS(rombase, size);

    return 0;
}
