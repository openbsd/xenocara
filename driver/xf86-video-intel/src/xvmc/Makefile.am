if XVMC
lib_LTLIBRARIES=libIntelXvMC.la
endif

SUBDIRS = shader

libIntelXvMC_la_SOURCES = intel_xvmc.c \
        intel_xvmc.h                   \
        intel_xvmc_dump.c              \
        i915_structs.h                 \
        i915_program.h                 \
        i915_xvmc.c                    \
        i915_xvmc.h                    \
        i965_xvmc.c                    \
        xvmc_vld.c                     \
        intel_batchbuffer.c            \
        intel_batchbuffer.h

AM_CFLAGS = @XORG_CFLAGS@ @DRM_CFLAGS@ @DRI_CFLAGS@ \
	@XVMCLIB_CFLAGS@ -I$(top_srcdir)/src -DTRUE=1 -DFALSE=0

libIntelXvMC_la_LDFLAGS = -version-number 1:0:0
libIntelXvMC_la_LIBADD = @DRI_LIBS@ @DRM_LIBS@ @XVMCLIB_LIBS@ @DRMINTEL_LIBS@ -lpthread
