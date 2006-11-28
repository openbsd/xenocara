XCOMM!SHELL_CMD
XCOMM
XCOMM $Xorg: Xsession,v 1.4 2000/08/17 19:54:17 cpqbld Exp $
XCOMM $OpenBSD: Xsession.cpp,v 1.2 2006/11/28 15:41:12 matthieu Exp $
XCOMM $XFree86: xc/programs/xdm/config/Xsession,v 1.2 1998/01/11 03:48:32 dawes Exp $

XCOMM redirect errors to a file in user's home directory if we can
for errfile in "$HOME/.xsession-errors" "${TMPDIR-/tmp}/xses-$USER" "/tmp/xses-$USER"
do
	case "$errfile" in
	"/tmp/*" | "/var/tmp/*")
		errfile=`mktemp ${errfile}.XXXXXXXXXX` || break;
		;;
	esac
	if ( umask 077 && cp /dev/null "$errfile" 2> /dev/null )
	then
		exec > "$errfile" 2>&1
		break
	fi
done

XCOMM if we have private ssh key(s), start ssh-agent and add the key(s)
id1=$HOME/.ssh/identity
id2=$HOME/.ssh/id_dsa
id3=$HOME/.ssh/id_rsa
if [ -x /usr/bin/ssh-agent ] && [ -f $id1 -o -f $id2 -o -f $id3 ];
then
	eval `ssh-agent -s`
	ssh-add < /dev/null
fi

do_exit() {
	if [ "$SSH_AGENT_PID" ]; then
		ssh-add -D < /dev/null
		eval `ssh-agent -s -k`
	fi
	exit
}

case $# in
1)
	case $1 in
	failsafe)
		BINDIR/xterm -geometry 80x24-0-0
		do_exit
		;;
	esac
esac

XCOMM  The startup script is not intended to have arguments.

startup=$HOME/.xsession
resources=$HOME/.Xresources

if [ -s "$startup" ]; then
	if [ -x "$startup" ]; then
		"$startup"
	else
		/bin/sh "$startup"
	fi
else
	if [ -f "$resources" ]; then
		BINDIR/xrdb -load "$resources"
	fi
#if defined(__SCO__) || defined(__UNIXWARE__)
        [ -r /etc/default/xdesktops ] && {
                . /etc/default/xdesktops
        }

        [ -r /etc/default/xdm ] && {
                . /etc/default/xdm
        }

        XCOMM Allow the user to over-ride the system default desktop
        [ -r $HOME/.xdmdesktop ] && {
                . $HOME/.xdmdesktop
        }

        [ -n "$XDESKTOP" ] && {
                exec `eval $XDESKTOP`
        }
#endif
	BINDIR/xterm &
	BINDIR/fvwm
fi
