#

include $(top_srcdir)/cpprules.in

xenodmscriptdir = $(XENODMSCRIPTDIR)
xenodmconfigdir = $(XENODMCONFIGDIR)

xenodmconfig_DATA = \
	xenodm-config \
	Xresources \
	Xservers

xenodmscript_SCRIPTS = \
	Xreset \
	Xsession \
	Xstartup

dist_xenodmscript_SCRIPTS = \
	GiveConsole \
	TakeConsole \
	Xsetup_0

pixmapdir = $(XENODM_PIXMAPDIR)

dist_pixmap_DATA = OpenBSD_15bpp.xpm OpenBSD_1bpp.xpm \
		OpenBSD_4bpp.xpm OpenBSD_8bpp.xpm

BUILT_SOURCES = Xservers xenodm-config Xreset Xresources Xsession Xstartup

CLEANFILES = $(BUILT_SOURCES) Xservers

EXTRA_DIST = Xservers.cpp \
	xenodm-config.cpp Xreset.cpp Xresources.cpp Xsession.cpp Xstartup.cpp

# Defines used when cpp'ing config files to replace settings in the files

XPMDEFINES = -DXPM -DBITMAPDIR=$(XENODM_PIXMAPDIR) -DXENODM_PIXMAP=$(XENODM_PIXMAP) \
        -DXENODM_BWPIXMAP=$(XENODM_BWPIXMAP)

CPP_FILES_FLAGS = -DBINDIR="$(bindir)" -DDEFAULTVT="$(DEFAULTVT)" \
    -DUTMP_FILE="$(UTMP_FILE)" -DWTMP_FILE="$(WTMP_FILE)" \
    -DXENODMDIR="$(XENODMLIBDIR)" -DXENODMLOGDIR="$(XENODMLOGDIR)" \
    -DXENODMXAUTHDIR="$(XENODMXAUTHDIR)" \
    -DXENODMCONFIGDIR="$(XENODMCONFIGDIR)" \
    -DXENODMSCRIPTDIR="$(XENODMSCRIPTDIR)" \
    $(XPMDEFINES) -DSHELL_CMD="$(SHELL_CMD)"

Xservers: $(srcdir)/Xservers.cpp
	$(AM_V_GEN)$(RAWCPP) $(RAWCPPFLAGS) $(CPP_FILES_FLAGS) < $(srcdir)/Xservers.cpp | $(CPP_SED_MAGIC) > $@
