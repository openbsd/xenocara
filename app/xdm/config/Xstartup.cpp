XCOMM!/bin/sh
XCOMM 
XCOMM $OpenBSD: Xstartup.cpp,v 1.3 2010/03/28 09:33:02 matthieu Exp $
XCOMM
BINDIR/sessreg  -a -w WTMP_FILE -u UTMP_FILE \
	-x XDMCONFIGDIR/Xservers -l $DISPLAY -h "" $USER
if [ -f /etc/nologin ]; then
        /usr/X11R6/bin/xmessage -file /etc/nologin -timeout 30 -center
        exit 1
fi
BINDIR/sessreg  -a -w "/var/log/wtmp" -u "/var/run/utmp" \
	-x XDMCONFIGDIR/Xservers -l $DISPLAY -h "" $USER
