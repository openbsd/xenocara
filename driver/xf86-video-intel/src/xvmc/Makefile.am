if XVMC
lib_LTLIBRARIES=libI810XvMC.la libIntelXvMC.la
endif

SUBDIRS = shader

libI810XvMC_la_SOURCES = I810XvMC.c \
			 I810XvMC.h

libI810XvMC_la_CFLAGS = @CWARNFLAGS@ @XORG_CFLAGS@ @DRM_CFLAGS@ @DRI_CFLAGS@ \
	-I$(top_srcdir)/src -DTRUE=1 -DFALSE=0
libI810XvMC_la_LDFLAGS = -version-number 1:0:0
libI810XvMC_la_LIBADD = @DRI_LIBS@ @DRM_LIBS@ @XVMCLIB_LIBS@

libIntelXvMC_la_SOURCES = intel_xvmc.c \
        intel_xvmc.h                   \
        intel_xvmc_dump.c              \
        i915_structs.h                 \
        i915_program.h                 \
        i915_xvmc.c                    \
        i915_xvmc.h                    \
        i965_xvmc.c                    \
        i965_xvmc.h                    \
        xvmc_vld.c                     \
        xvmc_vld.h                     \
        intel_batchbuffer.c            \
        intel_batchbuffer.h            \
        dri2.c                         \
        dri2.h	

libIntelXvMC_la_CFLAGS = @XORG_CFLAGS@ @DRM_CFLAGS@ @DRI_CFLAGS@ \
	@XVMCLIB_CFLAGS@ -I$(top_srcdir)/src -DTRUE=1 -DFALSE=0
libIntelXvMC_la_LDFLAGS = -version-number 1:0:0
libIntelXvMC_la_LIBADD = @DRI_LIBS@ @DRM_LIBS@ @XVMCLIB_LIBS@ -lpthread -ldrm_intel
