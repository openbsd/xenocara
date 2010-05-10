noinst_PROGRAMS = \
	intel_gtt \
	intel_statuspage \
	intel_hotplug \
	intel_lid \
	intel_audio

intel_gtt_SOURCES = \
	gtt.c \
	reg_dumper.h \
	util.c

intel_hotplug_SOURCES = \
	hotplug.c \
	reg_dumper.h \
	xprintf.c

intel_statuspage_SOURCES = \
	statuspage.c \
	reg_dumper.h \
	util.c

intel_lid_SOURCES = \
	lid.c \
	reg_dumper.h \
	util.c

intel_audio_SOURCES = \
	audio.c \
	reg_dumper.h \
	util.c

intel_hotplug_LDADD = $(PCIACCESS_LIBS)
intel_gtt_LDADD = $(PCIACCESS_LIBS)
intel_statuspage_LDADD = $(PCIACCESS_LIBS)
intel_lid_LDADD = $(PCIACCESS_LIBS)
intel_audio_LDADD = $(PCIACCESS_LIBS)

AM_CFLAGS = $(PCIACCESS_CFLAGS) $(CWARNFLAGS) \
	-I$(srcdir)/.. -DREG_DUMPER

