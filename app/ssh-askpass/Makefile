# $OpenBSD: Makefile,v 1.5 2007/01/07 14:04:46 matthieu Exp $

.include	<bsd.own.mk>
X11BASE?=	/usr/X11R6
PROG=		ssh-askpass
SRCS=		drawing.c dynlist.c resources.c x11-ssh-askpass.c
MANDIR=		${X11BASE}/man/cat

CLASS=		SshAskpass
VERSION=	1.2.0
DATE=		February 14, 2001
APP_DEFAULTS=	$(CLASS)-default.ad

CPPFLAGS+=	-I${X11BASE}/include -I.
LDADD+=		-L${X11BASE}/lib -lXt -lSM -lICE -lX11 -lXau -lXdmcp -lXinerama

CLEANFILES+=	${CLASS}.ad ${CLASS}_ad.h ssh-askpass.1

ssh-askpass.1: x11-ssh-askpass.man.in
	sed -e 's#@NAME@#$(PROG)#g' -e 's#@VERSION@#$(VERSION)#g' \
	    -e 's#@DATE@#$(DATE)#g' < $(.CURDIR)/x11-ssh-askpass.man.in > $@

$(CLASS)_ad.h: $(CLASS).ad
	cat $(CLASS).ad | grep -v '^[   ]*$$' |\
	  awk '/^[      ]*!/ { sub("^[  ]*![ ]?","", $$0); printf("/* %s */\n", $$0); next } ! /^[      ]*!/ { printf("\"%s\",\n", $$0) }' \
          >$@

$(CLASS).ad: $(APP_DEFAULTS)
	rm -f $(CLASS).ad
	ln -s $(.CURDIR)/$(APP_DEFAULTS) $(CLASS).ad

afterinstall:
	$(INSTALL) $(INSTALL_COPY)  -o $(BINOWN) -g $(BINGRP) $(CLASS).ad \
		$(DESTDIR)/etc/X11/app-defaults/$(CLASS)

beforedepend: $(CLASS)_ad.h

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
