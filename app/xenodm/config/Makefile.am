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

BUILT_SOURCES = Xservers.ws xenodm-config Xreset Xresources Xsession Xstartup

CLEANFILES = $(BUILT_SOURCES) Xservers

EXTRA_DIST = Xservers.ws.cpp Xservers.fs \
	xenodm-config.cpp Xreset.cpp Xresources.cpp Xsession.cpp Xstartup.cpp

Xservers: Xservers.$(SERVERSTYPE)
	ln -s Xservers.$(SERVERSTYPE) Xservers

# Defines used when cpp'ing config files to replace settings in the files

#if USE_XPM
XPMDEFINES = -DXPM -DBITMAPDIR=$(XENODM_PIXMAPDIR) -DXENODM_PIXMAP=$(XENODM_PIXMAP) \
        -DXENODM_BWPIXMAP=$(XENODM_BWPIXMAP)
#endif

#if HAVE_MKTEMP_COMMAND
MKTEMP_DEFINES = -DMKTEMP_COMMAND=$(MKTEMP_COMMAND)
#endif

CPP_FILES_FLAGS = -DBINDIR="$(bindir)" -DDEFAULTVT="$(DEFAULTVT)" \
    -DUTMP_FILE="$(UTMP_FILE)" -DWTMP_FILE="$(WTMP_FILE)" \
    -DXENODMDIR="$(XENODMLIBDIR)" -DXENODMLOGDIR="$(XENODMLOGDIR)" \
    -DXENODMXAUTHDIR="$(XENODMXAUTHDIR)" \
    -DXENODMCONFIGDIR="$(XENODMCONFIGDIR)" \
    -DXENODMSCRIPTDIR="$(XENODMSCRIPTDIR)" -DSU="$(SU)" \
    $(XPMDEFINES) -DSHELL_CMD="$(SHELL_CMD)" $(MKTEMP_DEFINES)

Xservers.ws: $(srcdir)/Xservers.ws.cpp
	$(AM_V_GEN)$(RAWCPP) $(RAWCPPFLAGS) $(CPP_FILES_FLAGS) < $(srcdir)/Xservers.ws.cpp | $(CPP_SED_MAGIC) > $@
