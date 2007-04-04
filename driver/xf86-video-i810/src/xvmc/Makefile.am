if DRI
lib_LTLIBRARIES=libI810XvMC.la
libI810XvMC_la_SOURCES = I810XvMC.c \
			 I810XvMC.h

libI810XvMC_la_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@ -I$(top_srcdir)/src -DTRUE=1 -DFALSE=0
libI810XvMC_la_LDFLAGS = -version-number 1:0:0
endif