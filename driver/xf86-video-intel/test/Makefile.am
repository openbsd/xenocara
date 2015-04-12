stress_TESTS = \
	basic-fillrect \
	basic-tiledrect \
	basic-stippledrect \
	basic-rectangle \
	basic-string \
	basic-copyarea \
	basic-copyarea-size \
	basic-putimage \
	basic-lines \
	basic-stress \
	cursor-test \
	render-fill \
	render-trapezoid \
	render-trapezoid-image \
	render-fill-copy \
	render-composite-solid \
	render-composite-solid-mask \
	render-copyarea \
	render-copyarea-mask \
	render-copyarea-size \
	render-copy-alphaless \
	mixed-stress \
	shm-test \
	$(NULL)

if DRI2
stress_TESTS += \
	dri2-race \
	dri2-swap \
	dri2-test \
	$(NULL)
endif

if X11_DRI3
stress_TESTS += \
	dri3-test \
	present-test \
	$(NULL)
endif
check_PROGRAMS = $(stress_TESTS)

noinst_PROGRAMS = lowlevel-blt-bench

AM_CFLAGS = @CWARNFLAGS@ $(X11_CFLAGS) $(DRM_CFLAGS)
LDADD = libtest.la $(X11_LIBS) $(DRM_LIBS) $(CLOCK_GETTIME_LIBS)

noinst_LTLIBRARIES = libtest.la
libtest_la_SOURCES = \
	test.h \
	test_display.c \
	test_image.c \
	test_log.c \
	test_render.c \
	$(NULL)

if DRI2
libtest_la_SOURCES += \
	dri2.c \
	dri2.h \
	$(NULL)
endif

if X11_DRI3
libtest_la_SOURCES += \
	dri3.c \
	dri3.h \
	$(NULL)
AM_CFLAGS += $(X11_DRI3_CFLAGS)
LDADD += $(X11_DRI3_LIBS)
endif

vsync.avi: mkvsync.sh
	./mkvsync.sh $@

clean-vsync-avi:
	rm -rf vsync.avi .build.tmp

EXTRA_DIST = README mkvsync.sh tearing.mp4 virtual.conf
clean-local: clean-vsync-avi
