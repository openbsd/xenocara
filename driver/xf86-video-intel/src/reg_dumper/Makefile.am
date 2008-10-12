noinst_PROGRAMS = intel_reg_dumper intel_idle intel_stepping intel_hotplug

intel_reg_dumper_SOURCES = \
	main.c \
	reg_dumper.h \
	xprintf.c \
	../i830_debug.c

intel_idle_SOURCES = \
	idle.c \
	reg_dumper.h \
	xprintf.c \
	../i830_debug.c

intel_hotplug_SOURCES = \
	hotplug.c \
	reg_dumper.h \
	xprintf.c \
	../i830_debug.c

intel_stepping_SOURCES = \
	stepping.c

intel_hotplug_LDADD = $(PCIACCESS_LIBS)
intel_reg_dumper_LDADD = $(PCIACCESS_LIBS)
intel_idle_LDADD = $(PCIACCESS_LIBS)
intel_stepping_LDADD = $(PCIACCESS_LIBS)

AM_CFLAGS = $(PCIACCESS_CFLAGS) $(WARN_CFLAGS) \
	-I$(srcdir)/.. -DREG_DUMPER

