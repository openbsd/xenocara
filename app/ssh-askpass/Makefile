# $OpenBSD: Makefile,v 1.14 2014/08/13 21:02:16 matthieu Exp $
.include	<bsd.xconf.mk>

PROG=		ssh-askpass
SRCS=		drawing.c dynlist.c resources.c x11-ssh-askpass.c
MANDIR=		${X11BASE}/man/man

CLASS=		SshAskpass
VERSION=	1.2.0
DATE=		February 14, 2001
APP_DEFAULTS=	$(CLASS)-default.ad

CPPFLAGS+=	-I${X11BASE}/include -I.
LDADD+=		-L${X11BASE}/lib -lXt -lSM -lICE -lXinerama -lX11 -lxcb -lXau -lXdmcp -lXext

CLEANFILES+=	${CLASS}.ad ${CLASS}_ad.h ssh-askpass.1

ssh-askpass.1: x11-ssh-askpass.man.in
	sed -e 's#@NAME@#$(PROG)#g' -e 's#@VERSION@#$(VERSION)#g' \
	    -e 's#@DATE@#$(DATE)#g' < $(.CURDIR)/x11-ssh-askpass.man.in > $@

$(CLASS)_ad.h: $(CLASS).ad
	grep -v '^[   ]*$$' $(CLASS).ad |\
	  awk '/^[      ]*!/ { sub("^[  ]*![ ]?","", $$0); printf("/* %s */\n", $$0); next } ! /^[      ]*!/ { printf("\"%s\",\n", $$0) }' \
          >$@

$(CLASS).ad: $(APP_DEFAULTS)
	rm -f $(CLASS).ad
	ln -s $(.CURDIR)/$(APP_DEFAULTS) $(CLASS).ad

# XXX why isn't $(CLASS)_ad.h being build before .depend?
x11-ssh-askpass.o: $(CLASS)_ad.h

afterinstall:
	${INSTALL_DATA} $(CLASS).ad \
		$(DESTDIR)${X11BASE}/share/X11/app-defaults/$(CLASS)

beforedepend: $(CLASS)_ad.h

obj: _xenocara_obj

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
