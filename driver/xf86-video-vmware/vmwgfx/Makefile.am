
if BUILD_VMWGFX
noinst_LTLIBRARIES = libvmwgfx.la
libvmwgfx_la_CFLAGS = $(CWARNFLAGS) $(XORG_CFLAGS) @LIBDRM_CFLAGS@ @XATRACKER_CFLAGS@ -I$(top_srcdir)/src -I$(top_srcdir)/saa
libvmwgfx_la_LIBADD = @LIBDRM_LIBS@ $(top_builddir)/saa/libsaa.la\
	@XATRACKER_LIBS@
libvmwgfx_la_DEPENDENCIES = $(top_builddir)/saa/libsaa.la

libvmwgfx_la_SOURCES = \
	svga3d_reg.h \
	vmwgfx_driver.c \
	vmwgfx_driver.h \
	vmwgfx_drm.h \
	vmwgfx_crtc.c \
	vmwgfx_output.c \
	vmwgfx_dri2.c \
	vmwgfx_tex_video.c \
	vmwgfx_saa.c \
	vmwgfx_saa.h \
	vmwgfx_saa_priv.h \
	vmwgfx_drmi.c \
	vmwgfx_drmi.h \
	vmwgfx_overlay.c \
	vmwgfx_ctrl.c \
	vmwgfx_ctrl.h \
	vmwgfx_xa_composite.c \
	vmwgfx_xa_surface.c \
	vmwgfx_hosted.c \
	vmwgfx_hosted.h \
	vmwgfx_hosted_priv.h \
	vmwgfx_xmir.c \
	vmwgfx_xwayland.c \
	wsbm_util.h
endif
