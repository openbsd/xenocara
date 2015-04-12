if XVMC
lib_LTLIBRARIES=libIntelXvMC.la
endif

SUBDIRS = shader

libIntelXvMC_la_SOURCES = \
	intel_xvmc.c \
        intel_xvmc.h \
        intel_xvmc_private.h \
        intel_xvmc_dump.c \
	i830_reg.h \
	i915_reg.h \
        i915_structs.h \
        i915_program.h \
        i915_xvmc.c \
        i915_xvmc.h \
	brw_defines.h \
	brw_structs.h \
        i965_reg.h \
        i965_xvmc.c \
        xvmc_vld.c \
        intel_batchbuffer.c \
        intel_batchbuffer.h \
	$(NULL)

AM_CFLAGS = $(XVMCLIB_CFLAGS) $(XORG_CFLAGS) -I$(top_srcdir)/src -DTRUE=1 -DFALSE=0

libIntelXvMC_la_LDFLAGS = -version-number 1:0:0
libIntelXvMC_la_LIBADD = $(XVMCLIB_LIBS) -lpthread
