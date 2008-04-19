BUILT_SOURCES =
CLEANFILES =
include $(top_srcdir)/RadeonHD.am

EXTRA_DIST = Imakefile

AM_CFLAGS = @XORG_CFLAGS@ @WARN_CFLAGS@
radeonhd_drv_la_LTLIBRARIES = radeonhd_drv.la
radeonhd_drv_la_LDFLAGS = -module -avoid-version
radeonhd_drv_la_CFLAGS = $(AM_CFLAGS) @PEDANTIC_CFLAGS@
radeonhd_drv_ladir = @moduledir@/drivers

radeonhd_drv_la_SOURCES = \
	rhd_atombios.c \
	rhd_i2c.c \
	rhd_crtc.c \
	rhd_connector.c \
	rhd_cursor.c \
	rhd_dac.c \
	rhd_driver.c \
	rhd_edid.c \
	rhd_helper.c \
	rhd_id.c \
	rhd_lut.c \
	rhd_lvtma.c \
	rhd_modes.c \
	rhd_monitor.c \
	rhd_output.c \
	rhd_pll.c \
	rhd_tmds.c \
	rhd_vga.c \
	rhd_mc.c \
	rhd_shadow.c \
	rhd_randr.c \
	rhd_dig.c \
	rhd_ddia.c \
	rhd_atombios.h \
	rhd.h \
	rhd_i2c.h \
	rhd_card.h \
	rhd_crtc.h \
	rhd_connector.h \
	rhd_cursor.h \
	rhd_lut.h \
	rhd_modes.h \
	rhd_monitor.h \
	rhd_output.h \
	rhd_pll.h \
	rhd_randr.h \
	rhd_regs.h \
	rhd_vga.h \
	rhd_shadow.h \
	rhd_mc.h \
	r5xx_accel.c \
	r5xx_accel.h \
	r5xx_xaa.c \
	r5xx_2dregs.h

nodist_radeonhd_drv_la_SOURCES = \
	git_version.h

if USE_EXA
radeonhd_drv_la_SOURCES += r5xx_exa.c
endif

if ATOM_BIOS
radeonhd_drv_la_SOURCES += \
	AtomBios/includes/atombios.h

INCLUDES = -I$(srcdir)/AtomBios/includes

endif

if ATOM_BIOS_PARSER
radeonhd_drv_la_SOURCES += \
	rhd_atomwrapper.h
endif

if ATOM_BIOS_PARSER

noinst_LTLIBRARIES = libatom.la
libatom_la_CFLAGS = $(AM_CFLAGS) @ATOMBIOS_CFLAGS@ -DDRIVER_PARSER -DDISABLE_EASF -DENABLE_ALL_SERVICE_FUNCTIONS
# libatom_la_LDFLAGS = -module -avoid-version

libatom_la_SOURCES = \
	rhd_atomwrapper.c \
	rhd_atomwrapper.h \
	AtomBios/CD_Operations.c \
	AtomBios/Decoder.c \
	AtomBios/hwserv_drv.c \
	AtomBios/includes/atombios.h \
	AtomBios/includes/CD_binding.h \
	AtomBios/includes/CD_Common_Types.h \
	AtomBios/includes/CD_Definitions.h \
	AtomBios/includes/CD_hw_services.h \
	AtomBios/includes/CD_Opcodes.h \
	AtomBios/includes/CD_Structs.h \
	AtomBios/includes/Decoder.h \
	AtomBios/includes/ObjectID.h \
	AtomBios/includes/regsdef.h

radeonhd_drv_la_LIBADD = libatom.la

endif
