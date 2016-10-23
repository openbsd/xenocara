XCOMM!/bin/sh
XCOMM 
XCOMM $OpenBSD: Xstartup.cpp,v 1.1 2016/10/23 08:30:37 matthieu Exp $
XCOMM
if [ -f /etc/nologin ]; then
        /usr/X11R6/bin/xmessage -file /etc/nologin -timeout 30 -center
        exit 1
fi
exec BINDIR/sessreg  -a -w WTMP_FILE -u UTMP_FILE \
	-x XENODMCONFIGDIR/Xservers -l $DISPLAY -h "" $USER
