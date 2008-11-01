BUILT_SOURCES =
CLEANFILES =
include $(top_srcdir)/RadeonHD.am

EXTRA_DIST = Imakefile

AM_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@ @WARN_CFLAGS@

driversdir = ${moduledir}/drivers
drivers_LTLIBRARIES = radeonhd_drv.la

radeonhd_drv_la_LDFLAGS = -module -avoid-version
radeonhd_drv_la_CFLAGS = $(AM_CFLAGS) @PEDANTIC_CFLAGS@

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
	rhd_mc.h \
	rhd_shadow.c \
	rhd_randr.c \
	rhd_dig.c \
	rhd_ddia.c \
	rhd_atomout.c \
	rhd_atompll.c \
	rhd_atomcrtc.c \
	rhd_biosscratch.c \
	rhd_biosscratch.h \
	rhd_atombios.h \
	rhd_atomout.h \
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
	rhd_cs.c \
	rhd_cs.h \
	r5xx_accel.c \
	r5xx_accel.h \
	r5xx_xaa.c \
	r5xx_regs.h \
	r5xx_3dregs.h \
	radeon_3d.c \
	rhd_video.c \
	rhd_video.h \
	radeon_textured_videofuncs.c

nodist_radeonhd_drv_la_SOURCES = \
	git_version.h

if USE_EXA
radeonhd_drv_la_SOURCES += r5xx_exa.c radeon_exa_render.c
endif

if USE_DRI
radeonhd_drv_la_SOURCES += rhd_dri.c rhd_dri.h radeon_dri.h radeon_drm.h
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

if MAINTAINER_MODE
SRCMAN = $(top_srcdir)/man/radeonhd.man
CLEANFILES += radeonhd.man.new
radeonhd.man.new: rhd_id.c $(SRCMAN)
	echo "Generating radeon.man.new..."; \
	(	$(SED) -n '1,/^\.\\" START_DEVICE_LIST marker - do not delete/p' "$(SRCMAN)"; \
		$(SED)	-e '1,/^[ 	]*\/\* START_DEVICE_LIST marker - do not delete \*\/ *$$/d' \
			-e '/^[ 	]*\/\* END_DEVICE_LIST marker - do not delete \*\/ *$$/,$$d' \
			-e '/^[ 	]\{1,\}xf86Msg(X_NONE,/d' \
			-e '/^[ 	]\{1,\}\/\*/d' \
			-e 's/^[ 	]\{1,\}"\\t\([A-Z0-9]\{1,\}\) \{1,\}: \(.*;\)\\n"$$/.TP 8\n.B \1\n\2/' \
			-e 's/^[ 	]\{1,\}"\\t\\t\(.*\)\.\\n"\();\)\{0,1\}$$/\1/' \
			-e 's/^[ 	]\{1,\}"\\t\([A-Z0-9]\{1,\}\) \{1,\}: \(.*\)\.\\n"\();\)\{0,1\}$$/.TP 8\n.B \1\n\2/' \
			"$(srcdir)/rhd_id.c"; \
		$(SED) -n '/^\.\\" END_DEVICE_LIST marker - do not delete/,$$p' "$(SRCMAN)" \
	) > radeonhd.man.new

SRCREADME = $(top_srcdir)/README
CLEANFILES += README.new
README.new: rhd_id.c $(top_srcdir)/README
	echo "Generating README.new..."; \
	(	$(SED) -n '1,/^BEGIN_DEVICE_LIST$$/p' "$(SRCREADME)"; \
		$(SED)	-e '1,/^[ 	]*\/\* START_DEVICE_LIST marker - do not delete \*\/ *$$/d' \
			-e '/^[ 	]*\/\* END_DEVICE_LIST marker - do not delete \*\/ *$$/,$$d' \
			-e '/^[ 	]\{1,\}xf86Msg(X_NONE,/d' \
			-e '/^[ 	]\{1,\}\/\*/d' \
			-e 's/^[ 	]\{1,\}"\\t\([A-Z0-9]\{1,\}\) \{1,\}: \(.*;\)\\n"$$/  * \1:\t\2/' \
			-e 's/^[ 	]\{1,\}"\\t\\t\(.*\)\.\\n"\();\)\{0,1\}$$/         \t\1/' \
			-e 's/^[ 	]\{1,\}"\\t\([A-Z0-9]\{1,\}\) \{1,\}: \(.*\)\.\\n"\();\)\{0,1\}$$/  * \1:\t\2/' \
			"$(srcdir)/rhd_id.c"; \
		$(SED) -n '/^END_DEVICE_LIST$$/,$$p' "$(SRCREADME)" \
	) > README.new

# Update man page and README from rhd_id.c RHDIdentify() output.
# The exit(1) makes sure the updates do not go unnoticed.
all-local: radeonhd.man.new README.new
	@changed=""; \
	if diff -u "$(SRCMAN)" radeonhd.man.new; then \
		echo "radeon(4) man page is current."; \
	else \
                changed="$$changed $(SRCMAN)"; \
		mv -f radeonhd.man.new "$(SRCMAN)"; \
	fi; \
	if diff -u "$(SRCREADME)" README.new; then \
		echo "README file is current."; \
	else \
                changed="$$changed $(SRCREADME)"; \
		mv -f README.new "$(SRCREADME)"; \
	fi; \
	if test "x$$changed" = "x"; then :; else \
		echo "WARNING: Due to changes to src/rhd_id.c, some files differ from the original ones."; \
		echo "         Please verify the changes to the following files"; \
		for f in $$changed; do echo "              $$f"; done; \
		echo "         and commit them together with your changes to"; \
		echo "              $(srcdir)/rhd_id.c"; \
		exit 1; \
	fi
endif
