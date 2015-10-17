XCOMM!SHELL_CMD
XCOMM
XCOMM $OpenBSD: Xsession.cpp,v 1.14 2015/10/17 08:25:11 matthieu Exp $

XCOMM redirect errors to a file in user's home directory if we can

errfile="$HOME/.xsession-errors"
if ( umask 077 && cp /dev/null "$errfile" 2> /dev/null )
then
	exec > "$errfile" 2>&1
else
#ifdef MKTEMP_COMMAND
	mktemp=MKTEMP_COMMAND
	for errfile in "${TMPDIR-/tmp}/xses-$USER" "/tmp/xses-$USER"
	do
		if ef="$( umask 077 && $mktemp "$errfile.XXXXXX" 2> /dev/null)"
		then
			exec > "$ef" 2>&1
			mv "$ef" "$errfile" 2> /dev/null
			break
		fi
	done
#else
XCOMM Since this system doesn't have a mktemp command to allow secure
XCOMM creation of files in shared directories, no fallback error log
XCOMM is being used.   See https://bugs.freedesktop.org/show_bug.cgi?id=5898
XCOMM
XCOMM 	for errfile in "${TMPDIR-/tmp}/xses-$USER" "/tmp/xses-$USER"
XCOMM	do
XCOMM		if ( umask 077 && cp /dev/null "$errfile" 2> /dev/null )
XCOMM		then
XCOMM			exec > "$errfile" 2>&1
XCOMM			break
XCOMM		fi
XCOMM	done

	exec > /dev/null 2>&1

#endif
fi

XCOMM if we have private ssh key(s), start ssh-agent and add the key(s)
id1=$HOME/.ssh/identity
id2=$HOME/.ssh/id_dsa
id3=$HOME/.ssh/id_rsa
id4=$HOME/.ssh/id_ecdsa
id5=$HOME/.ssh/id_ed25519
if [ -z "$SSH_AGENT_PID" ];
then
	if [ -x /usr/bin/ssh-agent ] && [ -f $id1 -o -f $id2 -o -f $id3 -o -f $id4 -o -f $id5 ];
	then
		eval `ssh-agent -s`
		ssh-add < /dev/null
	fi
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

XCOMM The startup script is not intended to have arguments.

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
do_exit
