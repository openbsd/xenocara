if XVMC
lib_LTLIBRARIES=libviaXvMC.la libviaXvMCPro.la

libviaXvMC_la_SOURCES = \
	driDrawable.c \
	driDrawable.h \
	viaLowLevel.c \
	viaLowLevel.h \
	viaXvMC.c \
	viaXvMCPriv.h \
	xf86dri.c \
	xf86dri.h \
	xf86dristr.h

libviaXvMCPro_la_SOURCES = \
	driDrawable.c \
	driDrawable.h \
	viaLowLevelPro.c \
	viaLowLevel.h \
	viaXvMC.c \
	viaXvMCPriv.h \
	xf86dri.c \
	xf86dri.h \
	xf86dristr.h

AM_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@ -I$(top_srcdir)/src -DTRUE=1 -DFALSE=0
libviaXvMC_la_LDFLAGS = @DRI_LIBS@ -version-number 1:0:0
libviaXvMCPro_la_LDFLAGS = @DRI_LIBS@ -version-number 1:0:0
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