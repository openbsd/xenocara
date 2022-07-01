XCOMM!SHELL_CMD
XCOMM $OpenBSD: xinitrc.cpp,v 1.14 2022/07/01 20:42:06 naddy Exp $

userresources=$HOME/.Xresources
usermodmap=$HOME/.Xmodmap
sysresources=XINITDIR/.Xresources
sysmodmap=XINITDIR/.Xmodmap

XCOMM merge in defaults and keymaps

if [ -f $sysresources ]; then
#ifdef __APPLE__
    if [ -x /usr/bin/cpp ] ; then
        XRDB -merge $sysresources
    else
        XRDB -nocpp -merge $sysresources
    fi
#else
    XRDB -merge $sysresources
#endif
fi

if [ -f $sysmodmap ]; then
    XMODMAP $sysmodmap
fi

if [ -f "$userresources" ]; then
#ifdef __APPLE__
    if [ -x /usr/bin/cpp ] ; then
        XRDB -merge "$userresources"
    else
        XRDB -nocpp -merge "$userresources"
    fi
#else
    XRDB -merge "$userresources"
#endif
fi

if [ -f "$usermodmap" ]; then
    XMODMAP "$usermodmap"
fi

XCOMM if we have private ssh key(s), start ssh-agent and add the key(s)

if [ -z "$SSH_AGENT_PID" ] && [ -x /usr/bin/ssh-agent ]
then
	for k in id_rsa id_ecdsa id_ecdsa_sk id_ed25519 id_ed25519_sk id_dsa
	do
		if [ -f "$HOME/.ssh/$k" ]; then
			eval `ssh-agent -s`
			ssh-add < /dev/null
			break
		fi
	done
fi

XCOMM start some nice programs

XCLOCK -geometry 50x50-1+1 &
XCONSOLE -iconic &
XTERM -geometry 80x24 &
WM || XTERM

if [ "$SSH_AGENT_PID" ]; then
	ssh-add -D < /dev/null
	eval `ssh-agent -s -k`
fi
