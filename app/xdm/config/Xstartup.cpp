XCOMM!/bin/sh
XCOMM 
XCOMM $OpenBSD: Xstartup.cpp,v 1.4 2011/01/01 20:39:44 matthieu Exp $
XCOMM
if [ -f /etc/nologin ]; then
        /usr/X11R6/bin/xmessage -file /etc/nologin -timeout 30 -center
        exit 1
fi
BINDIR/sessreg  -a -w WTMP_FILE -u UTMP_FILE \
	-x XDMCONFIGDIR/Xservers -l $DISPLAY -h "" $USER
