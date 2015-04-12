#  Copyright 2005 Adam Jackson.
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  on the rights to use, copy, modify, merge, publish, distribute, sub
#  license, and/or sell copies of the Software, and to permit persons to whom
#  the Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice (including the next
#  paragraph) shall be included in all copies or substantial portions of the
#  Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
#  ADAM JACKSON BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
#  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SUBDIRS = brw fb

AM_CFLAGS = \
	@CWARNFLAGS@ \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/render_program \
	$(XORG_CFLAGS) \
	$(UDEV_CFLAGS) \
	$(DRM_CFLAGS) \
	$(NULL)

if VALGRIND
AM_CFLAGS += $(VALGRIND_CFLAGS)
endif

noinst_LTLIBRARIES = libsna.la
libsna_la_LDFLAGS = -pthread
libsna_la_LIBADD = $(UDEV_LIBS) -lm $(DRM_LIBS) brw/libbrw.la fb/libfb.la ../../libobj/libcompat.la

libsna_la_SOURCES = \
	atomic.h \
	blt.c \
	compiler.h \
	kgem.c \
	kgem.h \
	rop.h \
	sna.h \
	sna_accel.c \
	sna_acpi.c \
	sna_blt.c \
	sna_composite.c \
	sna_cpu.c \
	sna_cpuid.h \
	sna_damage.c \
	sna_damage.h \
	sna_display.c \
	sna_display_fake.c \
	sna_driver.c \
	sna_glyphs.c \
	sna_gradient.c \
	sna_io.c \
	sna_module.h \
	sna_render.c \
	sna_render.h \
	sna_render_inline.h \
	sna_reg.h \
	sna_stream.c \
	sna_trapezoids.h \
	sna_trapezoids.c \
	sna_trapezoids_boxes.c \
	sna_trapezoids_imprecise.c \
	sna_trapezoids_mono.c \
	sna_trapezoids_precise.c \
	sna_tiling.c \
	sna_transform.c \
	sna_threads.c \
	sna_vertex.c \
	sna_video.c \
	sna_video.h \
	sna_video_overlay.c \
	sna_video_sprite.c \
	sna_video_textured.c \
	gen2_render.c \
	gen2_render.h \
	gen3_render.c \
	gen3_render.h \
	gen4_common.c \
	gen4_common.h \
	gen4_render.c \
	gen4_render.h \
	gen4_source.c \
	gen4_source.h \
	gen4_vertex.c \
	gen4_vertex.h \
	gen5_render.c \
	gen5_render.h \
	gen6_common.c \
	gen6_common.h \
	gen6_render.c \
	gen6_render.h \
	gen7_render.c \
	gen7_render.h \
	gen8_eu.c \
	gen8_eu.h \
	gen8_render.c \
	gen8_render.h \
	gen8_vertex.c \
	gen8_vertex.h \
	$(NULL)

if DRI2
AM_CFLAGS += $(DRI2_CFLAGS)
libsna_la_SOURCES += sna_dri2.c
libsna_la_LIBADD += $(DRI2_LIBS) @CLOCK_GETTIME_LIBS@
endif

if DRI3
AM_CFLAGS += $(DRI3_CFLAGS)
libsna_la_SOURCES += sna_dri3.c
libsna_la_LIBADD += $(DRI3_LIBS)
endif

if PRESENT
AM_CFLAGS += $(PRESENT_CFLAGS)
libsna_la_SOURCES += sna_present.c
libsna_la_LIBADD += $(PRESENT_LIBS)
endif

if XVMC
libsna_la_SOURCES += \
	sna_video_hwmc.h \
	sna_video_hwmc.c \
	$(NULL)
endif

if FULL_DEBUG
libsna_la_SOURCES += \
	kgem_debug.c \
	kgem_debug.h \
	kgem_debug_gen2.c \
	kgem_debug_gen3.c \
	kgem_debug_gen4.c \
	kgem_debug_gen5.c \
	kgem_debug_gen6.c \
	kgem_debug_gen7.c \
	$(NULL)
endif

if HAVE_DOT_GIT
git_version.h: $(top_srcdir)/.git/HEAD $(shell sed -e '/ref:/!d' -e 's#ref: *#$(top_srcdir)/.git/#' < $(top_srcdir)/.git/HEAD)
	@echo "Recording git-tree used for compilation: `git describe`"
	@V=`git describe`; echo "static const char git_version[] = \"$$V\";" > git_version.h
sna_driver.c: git_version.h
endif

AM_CFLAGS += @NOWARNFLAGS@
