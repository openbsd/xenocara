/*
 * Copyright © 2007 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Zhenyu Wang <zhenyu.z.wang@intel.com>
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "i830.h"

#define SUBSYS_ANY (~0)

#define DMIID_DIR "/sys/class/dmi/id/"
#define DMIID_FILE(x) (DMIID_DIR # x)

typedef struct {
    int chipType;
    int subsysVendor;
    int subsysCard;
    void (*hook)(I830Ptr);
} i830_quirk, *i830_quirk_ptr;

enum i830_dmi_data_t {
    bios_vendor,
    bios_version,
    bios_date,
    sys_vendor,
    product_name,
    product_version,
    product_serial,
    product_uuid,
    board_vendor,
    board_name,
    board_version,
    board_serial,
    board_asset_tag,
    chassis_vendor,
    chassis_type,
    chassis_version,
    chassis_serial,
    chassis_asset_tag,
    dmi_data_max,
};

static char *i830_dmi_data[dmi_data_max];

#define I830_DMI_FIELD_FUNC(field) \
static void i830_dmi_store_##field(void) \
{\
    FILE *f = NULL;\
    f = fopen(DMIID_FILE(field), "r");\
    if (f == NULL) {\
	xfree(i830_dmi_data[field]); i830_dmi_data[field] = NULL;\
	return;\
    }\
    fread(i830_dmi_data[field], 64, 1, f);\
    fclose(f);\
}

I830_DMI_FIELD_FUNC(bios_vendor);
I830_DMI_FIELD_FUNC(bios_version);
I830_DMI_FIELD_FUNC(bios_date);
I830_DMI_FIELD_FUNC(sys_vendor);
I830_DMI_FIELD_FUNC(product_name);
I830_DMI_FIELD_FUNC(product_version);
I830_DMI_FIELD_FUNC(product_serial);
I830_DMI_FIELD_FUNC(product_uuid);
I830_DMI_FIELD_FUNC(board_vendor);
I830_DMI_FIELD_FUNC(board_name);
I830_DMI_FIELD_FUNC(board_version);
I830_DMI_FIELD_FUNC(board_serial);
I830_DMI_FIELD_FUNC(board_asset_tag);
I830_DMI_FIELD_FUNC(chassis_vendor);
I830_DMI_FIELD_FUNC(chassis_type);
I830_DMI_FIELD_FUNC(chassis_version);
I830_DMI_FIELD_FUNC(chassis_serial);
I830_DMI_FIELD_FUNC(chassis_asset_tag);

static void i830_dmi_scan(void)
{
    int i;

    for (i = 0; i < dmi_data_max; i++) {
	i830_dmi_data[i] = xcalloc(64, sizeof(char));
	if (!i830_dmi_data[i]) {
	    int j;
	    for (j = 0; j < i; j++) {
		xfree(i830_dmi_data[j]);
		i830_dmi_data[i] = NULL;
	    }
	    return;
	}
    }

    i830_dmi_store_bios_vendor();
    i830_dmi_store_bios_version();
    i830_dmi_store_bios_date();
    i830_dmi_store_sys_vendor();
    i830_dmi_store_product_name();
    i830_dmi_store_product_version();
    i830_dmi_store_product_serial();
    i830_dmi_store_product_uuid();
    i830_dmi_store_board_vendor();
    i830_dmi_store_board_name();
    i830_dmi_store_board_version();
    i830_dmi_store_board_serial();
    i830_dmi_store_board_asset_tag();
    i830_dmi_store_chassis_vendor();
    i830_dmi_store_chassis_type();
    i830_dmi_store_chassis_version();
    i830_dmi_store_chassis_serial();
    i830_dmi_store_chassis_asset_tag();
}

#define DMIID_DUMP(field) \
    ErrorF("\t" # field ": %s", i830_dmi_data[field] ?\
	    i830_dmi_data[field] : "unknown")

static void i830_dmi_dump(void)
{
    ErrorF("i830_dmi_dump:\n");
    DMIID_DUMP(bios_vendor);
    DMIID_DUMP(bios_version);
    DMIID_DUMP(bios_date);
    DMIID_DUMP(sys_vendor);
    DMIID_DUMP(product_name);
    DMIID_DUMP(product_version);
    DMIID_DUMP(product_serial);
    DMIID_DUMP(product_uuid);
    DMIID_DUMP(board_vendor);
    DMIID_DUMP(board_name);
    DMIID_DUMP(board_version);
    DMIID_DUMP(board_serial);
    DMIID_DUMP(board_asset_tag);
    DMIID_DUMP(chassis_vendor);
    DMIID_DUMP(chassis_type);
    DMIID_DUMP(chassis_version);
    DMIID_DUMP(chassis_serial);
    DMIID_DUMP(chassis_asset_tag);
}

static void quirk_pipea_force (I830Ptr pI830)
{
    pI830->quirk_flag |= QUIRK_PIPEA_FORCE;
}

static void quirk_ignore_tv (I830Ptr pI830)
{
    pI830->quirk_flag |= QUIRK_IGNORE_TV;
}

static void quirk_ignore_lvds (I830Ptr pI830)
{
    pI830->quirk_flag |= QUIRK_IGNORE_LVDS;
}

static void quirk_mac_mini (I830Ptr pI830)
{
    pI830->quirk_flag |= QUIRK_IGNORE_MACMINI_LVDS;
}

static void quirk_lenovo_tv_dmi (I830Ptr pI830)
{
    /* X60, X60s has no TV output.
     * Z61 has S-video TV output.
     * And they have same subsys ids...
     *
     * http://www-307.ibm.com/pc/support/site.wss/MIGR-45120.html
     * http://www.thinkwiki.org/wiki/List_of_DMI_IDs
     */
    if (!i830_dmi_data[bios_version]) {
	ErrorF("Failed to load DMI info, X60 TV quirk not applied.\n");
	return;
    }
    if (!strncmp(i830_dmi_data[bios_version], "7B", 2))
	pI830->quirk_flag |= QUIRK_IGNORE_TV;
}

static void quirk_ivch_dvob (I830Ptr pI830)
{
	pI830->quirk_flag |= QUIRK_IVCH_NEED_DVOB;
}

/* keep this list sorted by OEM, then by chip ID */
static i830_quirk i830_quirk_list[] = {
    /* Aopen mini pc */
    { PCI_CHIP_I945_GM, 0xa0a0, SUBSYS_ANY, quirk_ignore_lvds },
    { PCI_CHIP_I965_GM, 0xa0a0, SUBSYS_ANY, quirk_ignore_lvds },
    { PCI_CHIP_I965_GM, 0x8086, 0x1999, quirk_ignore_lvds },

    /* Apple Mac mini has no lvds, but macbook pro does */
    { PCI_CHIP_I945_GM, 0x8086, 0x7270, quirk_mac_mini },

    /* Clevo M720R has no tv output */
    { PCI_CHIP_I965_GM, 0x1558, 0x0721, quirk_ignore_tv },

    /* Dell Latitude X1 */
    { PCI_CHIP_I915_GM, 0x1028, 0x01a3, quirk_ignore_tv },
    /* Dell XPS 1330 */
    { PCI_CHIP_I965_GM, 0x1028, 0x0209, quirk_ignore_tv },

    /* Lenovo Napa TV (use dmi)*/
    { PCI_CHIP_I945_GM, 0x17aa, SUBSYS_ANY, quirk_lenovo_tv_dmi },
    /* Lenovo T61 has no TV output */
    { PCI_CHIP_I965_GM, 0x17aa, 0x20b5, quirk_ignore_tv },
    /* Lenovo 3000 v200 */
    { PCI_CHIP_I965_GM, 0x17aa, 0x3c18, quirk_ignore_tv },

    /* Panasonic Toughbook CF-Y4 has no TV output */
    { PCI_CHIP_I915_GM, 0x10f7, 0x8338, quirk_ignore_tv },
    /* Panasonic Toughbook CF-Y7 has no TV output */
    { PCI_CHIP_I965_GM, 0x10f7, 0x8338, quirk_ignore_tv },

    /* Toshiba Satellite U300 has no TV output */
    { PCI_CHIP_I965_GM, 0x1179, 0xff50, quirk_ignore_tv },
    /* Toshiba i830M laptop (fix bug 11148) */
    { PCI_CHIP_I830_M, 0x1179, 0xff00, quirk_ivch_dvob },

    /* Samsung Q35 has no TV output */
    { PCI_CHIP_I945_GM, 0x144d, 0xc504, quirk_ignore_tv },
    /* Samsung Q45 has no TV output */
    { PCI_CHIP_I965_GM, 0x144d, 0xc510, quirk_ignore_tv },

    /* Dell Inspiron 510m needs pipe A force quirk */
    { PCI_CHIP_I855_GM, 0x1028, 0x0164, quirk_pipea_force },

    /* ThinkPad X40 needs pipe A force quirk */
    { PCI_CHIP_I855_GM, 0x1014, 0x0557, quirk_pipea_force },

    /* Sony vaio PCG-r600HFP (fix bug 13722) */
    { PCI_CHIP_I830_M, 0x104d, 0x8100, quirk_ivch_dvob },

    { 0, 0, 0, NULL },
};

void i830_fixup_devices(ScrnInfoPtr scrn)
{
    I830Ptr pI830 = I830PTR(scrn);
    i830_quirk_ptr p = i830_quirk_list;
    int i;

    i830_dmi_scan();

    if (0)
	i830_dmi_dump();

    while (p && p->chipType != 0) {
	if (DEVICE_ID(pI830->PciInfo) == p->chipType &&
		SUBVENDOR_ID(pI830->PciInfo) == p->subsysVendor &&
		(SUBSYS_ID(pI830->PciInfo) == p->subsysCard ||
		 p->subsysCard == SUBSYS_ANY))
	    p->hook(pI830);
	++p;
    }

    for (i = 0; i < dmi_data_max; i++)
	if (i830_dmi_data[i])
	    xfree(i830_dmi_data[i]);
}
