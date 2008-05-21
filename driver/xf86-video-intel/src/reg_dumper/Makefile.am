noinst_PROGRAMS = intel_reg_dumper intel_idle intel_stepping

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

intel_stepping_SOURCES = \
	stepping.c

intel_reg_dumper_LDADD = $(PCIACCESS_LIBS)
intel_idle_LDADD = $(PCIACCESS_LIBS)
intel_stepping_LDADD = $(PCIACCESS_LIBS)

AM_CFLAGS = $(PCIACCESS_CFLAGS) $(WARN_CFLAGS) \
	-I$(srcdir)/.. -DREG_DUMPER

