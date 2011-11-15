XCOMM!/bin/sh
XCOMM 
XCOMM $OpenBSD: Xstartup.cpp,v 1.5 2011/11/15 20:53:45 matthieu Exp $
XCOMM
if [ -f /etc/nologin ]; then
        /usr/X11R6/bin/xmessage -file /etc/nologin -timeout 30 -center
        exit 1
fi
exec BINDIR/sessreg  -a -w WTMP_FILE -u UTMP_FILE \
	-x XDMCONFIGDIR/Xservers -l $DISPLAY -h "" $USER
