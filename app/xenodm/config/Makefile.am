#

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


CLEANFILES = $(BUILT_SOURCES) Xservers

