if XVMC
lib_LTLIBRARIES=libchromeXvMC.la libchromeXvMCPro.la

libchromeXvMC_la_SOURCES = \
	viaLowLevel.c \
	driDrawable.c \
	viaXvMC.c \
	xf86dri.c \
	viaLowLevel.h \
	driDrawable.h \
	viaXvMCPriv.h \
	xf86dri.h \
	xf86dristr.h \
	vldXvMC.h
libchromeXvMCPro_la_SOURCES = \
	viaLowLevelPro.c \
	driDrawable.c \
	viaXvMC.c \
	xf86dri.c \
	viaLowLevel.h \
	driDrawable.h \
	viaXvMCPriv.h \
	xf86dri.h \
	xf86dristr.h \
	vldXvMC.h

libchromeXvMC_la_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@ @XVMC_CFLAGS@ -I$(top_srcdir)/src -I$(top_srcdir)/src/xvmc -DTRUE=1 -DFALSE=0
libchromeXvMC_la_LDFLAGS = -version-number 1:0:0
libchromeXvMC_la_LIBADD = @DRI_LIBS@ @XVMC_LIBS@

libchromeXvMCPro_la_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@ @XVMC_CFLAGS@ -I$(top_srcdir)/src -I$(top_srcdir)/src/xvmc -DTRUE=1 -DFALSE=0
libchromeXvMCPro_la_LDFLAGS = -version-number 1:0:0
libchromeXvMCPro_la_LIBADD = @DRI_LIBS@ @XVMC_LIBS@
else
EXTRA_DIST = \
	driDrawable.c \
	driDrawable.h \
	viaLowLevelPro.c \
	viaLowLevel.c \
	viaLowLevel.h \
	viaXvMC.c \
	viaXvMCPriv.h \
	xf86dri.c \
	xf86dri.h \
	xf86dristr.h
endif
