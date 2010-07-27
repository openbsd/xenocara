if BUILD_KDRIVEFBDEVLIB
FBDEV_SUBDIRS = fbdev
endif

if KDRIVEWSCONS
WSCONS_SUBDIRS = wscons
endif

if XFAKESERVER
XFAKE_SUBDIRS = fake
endif

if XEPHYR
XEPHYR_SUBDIRS = ephyr
endif

if KDRIVELINUX
LINUX_SUBDIRS = linux
endif

if KDRIVEOPENBSD
OPENBSD_SUBDIRS = openbsd
endif

SERVER_SUBDIRS = 		\
	$(FBDEV_SUBDIRS)	\
	$(WSCONS_SUBDIRS)	\
	$(XEPHYR_SUBDIRS)       \
	$(XFAKE_SUBDIRS)

SUBDIRS =			\
	src			\
	$(LINUX_SUBDIRS)	\
	$(OPENBSD_SUBDIRS)	\
	$(SERVER_SUBDIRS)

DIST_SUBDIRS = fbdev ephyr src linux fake openbsd wscons

relink:
	$(AM_V_at)for i in $(SERVER_SUBDIRS) ; do $(MAKE) -C $$i relink || exit 1 ; done
