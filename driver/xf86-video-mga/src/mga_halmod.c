/* $XFree86$ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86Module.h"

#ifdef XFree86LOADER

#define HAL_MAJOR_VERSION 1
#define HAL_MINOR_VERSION 0
#define HAL_PATCHLEVEL 0

static MODULESETUPPROTO(halSetup);

static XF86ModuleVersionInfo halVersRec =
{
	"mga_hal",
	MODULEVENDORSTRING,
	MODINFOSTRING1,
	MODINFOSTRING2,
	XORG_VERSION_CURRENT,
	HAL_MAJOR_VERSION, HAL_MINOR_VERSION, HAL_PATCHLEVEL,
	ABI_CLASS_VIDEODRV,			/* This is a video driver */
	ABI_VIDEODRV_VERSION,
	MOD_CLASS_NONE,
	{0,0,0,0}
};

/*
 * This is the module init data.
 * Its name has to be the driver name followed by ModuleData.
 */
_X_EXPORT XF86ModuleData mga_halModuleData = { &halVersRec, halSetup, NULL };

static pointer
halSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
	return (pointer)1;
}

#endif /* XFree86LOADER */
