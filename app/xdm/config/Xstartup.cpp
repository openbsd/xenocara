XCOMM!/bin/sh
XCOMM 
XCOMM $OpenBSD: Xstartup.cpp,v 1.2 2007/08/05 19:37:40 matthieu Exp $
XCOMM
XCOMM Register a login (derived from GiveConsole as follows:)
XCOMM
if [ -f /etc/nologin ]; then
        /usr/X11R6/bin/xmessage -file /etc/nologin -timeout 30 -center
        exit 1
fi
BINDIR/sessreg  -a -w "/var/log/wtmp" -u "/var/run/utmp" \
	-x XDMCONFIGDIR/Xservers -l $DISPLAY -h "" $USER
