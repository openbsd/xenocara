XCOMM!SHELL_CMD
XCOMM $Xorg: xinitrc.cpp,v 1.3 2000/08/17 19:54:30 cpqbld Exp $
XCOMM $OpenBSD: xinitrc.cpp,v 1.2 2006/11/26 17:17:57 matthieu Exp $

userresources=$HOME/.Xresources
usermodmap=$HOME/.Xmodmap
sysresources=XINITDIR/.Xresources
sysmodmap=XINITDIR/.Xmodmap

XCOMM merge in defaults and keymaps

if [ -f $sysresources ]; then
    XRDB -merge $sysresources
fi

if [ -f $sysmodmap ]; then
    XMODMAP $sysmodmap
fi

if [ -f $userresources ]; then
    XRDB -merge $userresources
fi

if [ -f $usermodmap ]; then
    XMODMAP $usermodmap
fi

XCOMM if we have private ssh key(s), start ssh-agent and add the key(s)
id1=$HOME/.ssh/identity
id2=$HOME/.ssh/id_dsa
id3=$HOME/.ssh/id_rsa
if [ -x /usr/bin/ssh-agent ] && [ -f $id1 -o -f $id2 -o -f $id3 ];
then
	eval `ssh-agent -s`
	ssh-add < /dev/null
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
