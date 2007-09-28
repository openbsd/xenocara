XCOMM!SHELL_CMD

XCOMM $Xorg: startx.cpp,v 1.3 2000/08/17 19:54:29 cpqbld Exp $
XCOMM
XCOMM This is just a sample implementation of a slightly less primitive
XCOMM interface than xinit.  It looks for user .xinitrc and .xserverrc
XCOMM files, then system xinitrc and xserverrc files, else lets xinit choose
XCOMM its default.  The system xinitrc should probably do things like check
XCOMM for .Xresources files and merge them in, startup up a window manager,
XCOMM and pop a clock and serveral xterms.
XCOMM
XCOMM Site administrators are STRONGLY urged to write nicer versions.
XCOMM
XCOMM $XFree86: xc/programs/xinit/startx.cpp,v 3.16tsi Exp $

#if defined(__SCO__) || defined(__UNIXWARE__)

XCOMM Check for /usr/bin/X11 and BINDIR in the path, if not add them.
XCOMM This allows startx to be placed in a place like /usr/bin or /usr/local/bin
XCOMM and people may use X without changing their PATH.
XCOMM Note that we put our own bin directory at the front of the path, and
XCOMM the standard SCO path at the back, since if you are using the Xorg
XCOMM server theres a pretty good chance you want to bias the Xorg clients
XCOMM over the old SCO X11R5 clients.

XCOMM First our compiled path

bindir=BINDIR
scobindir=/usr/bin/X11

case $PATH in
  *:$bindir | *:$bindir:* | $bindir:*) ;;
  *) PATH=$bindir:$PATH ;;
esac

XCOMM Now the "SCO" compiled path

case $PATH in
  *:$scobindir | *:$scobindir:* | $scobindir:*) ;;
  *) PATH=$PATH:$scobindir ;;
esac

XCOMM Bourne shell does not automatically export modified environment variables
XCOMM so export the new PATH just in case the user changes the shell
export PATH

XCOMM Set up the XMERGE env var so that dos merge is happy under X

if [ -f /usr/lib/merge/xmergeset.sh ]; then
	. /usr/lib/merge/xmergeset.sh
elif [ -f /usr/lib/merge/console.disp ]; then
	XMERGE=`cat /usr/lib/merge/console.disp`
	export XMERGE
fi

userclientrc=$HOME/.startxrc
sysclientrc=LIBDIR/sys.startxrc
scouserclientrc=$HOME/.xinitrc
scosysclientrc=XINITDIR/xinitrc
#else
userclientrc=$HOME/.xinitrc
sysclientrc=XINITDIR/xinitrc
#endif

userserverrc=$HOME/.xserverrc
sysserverrc=XINITDIR/xserverrc
defaultclient=XTERM
defaultserver=XSERVER
defaultclientargs=""
defaultserverargs=""
clientargs=""
serverargs=""

if [ -f $userclientrc ]; then
    defaultclientargs=$userclientrc
elif [ -f $sysclientrc ]; then
    defaultclientargs=$sysclientrc
#if defined(__SCO__) || defined(__UNIXWARE__)
elif [ -f $scouserclientrc ]; then
    defaultclientargs=$scouserclientrc
elif [ -f $scosysclientrc ]; then
    defaultclientargs=$scosysclientrc
#endif
fi

#if defined(__SCO__) || defined(__UNIXWARE__)

XCOMM SCO -t option: do not start an X server
case $1 in
  -t)   if [ -n "$DISPLAY" ]; then
                REMOTE_SERVER=TRUE
                shift
        else
                echo "DISPLAY environment variable not set"
                exit 1
        fi
        ;;
esac
#endif

if [ -f $userserverrc ]; then
    defaultserverargs=$userserverrc
elif [ -f $sysserverrc ]; then
    defaultserverargs=$sysserverrc
fi

whoseargs="client"
while [ x"$1" != x ]; do
    case "$1" in
    XCOMM '' required to prevent cpp from treating "/*" as a C comment.
    /''*|\./''*)
	if [ "$whoseargs" = "client" ]; then
	    if [ x"$clientargs" = x ]; then
		client="$1"
	    else
		clientargs="$clientargs $1"
	    fi
	else
	    if [ x"$serverargs" = x ]; then
		server="$1"
	    else
		serverargs="$serverargs $1"
	    fi
	fi
	;;
    --)
	whoseargs="server"
	;;
    *)
	if [ "$whoseargs" = "client" ]; then
	    clientargs="$clientargs $1"
	else
	    XCOMM display must be the FIRST server argument
	    if [ x"$serverargs" = x ] && @@
		 expr "$1" : ':[0-9][0-9]*$' > /dev/null 2>&1; then
		display="$1"
	    else
		serverargs="$serverargs $1"
	    fi
	fi
	;;
    esac
    shift
done

XCOMM process client arguments
if [ x"$client" = x ]; then
    XCOMM if no client arguments either, use rc file instead
    if [ x"$clientargs" = x ]; then
	client="$defaultclientargs"
    else
	client=$defaultclient
    fi
fi

XCOMM process server arguments
if [ x"$server" = x ]; then
    XCOMM if no server arguments or display either, use rc file instead
    if [ x"$serverargs" = x -a x"$display" = x ]; then
	server="$defaultserverargs"
    else
	server=$defaultserver
    fi
fi

if [ x"$XAUTHORITY" = x ]; then
    XAUTHORITY=$HOME/.Xauthority
    export XAUTHORITY
fi

removelist=

XCOMM set up default Xauth info for this machine
case `uname` in
Linux*)
	if [ -z "`hostname --version 2>&1 | grep GNU`" ]; then
		hostname=`hostname -f`
	else
		hostname=`hostname`
	fi
	;;
*)
	hostname=`hostname`
	;;
esac

authdisplay=${display:-:0}
#if defined(HAS_COOKIE_MAKER) && defined(MK_COOKIE)
mcookie=`MK_COOKIE`
#else
mcookie=`dd if=/dev/arandom bs=16 count=1 2>/dev/null | hexdump -e \\"%08x\\"`
if test x"$mcookie" = x; then
                echo "Couldn't create cookie"
                exit 1
fi
#endif
dummy=0

XCOMM create a file with auth information for the server. ':0' is a dummy.
xserverauthfile=$HOME/.serverauth.$$
trap "rm -f $xserverauthfile" ERR HUP INT QUIT ILL TRAP KILL BUS TERM
xauth -q -f $xserverauthfile << EOF
add :$dummy . $mcookie
EOF
serverargs=${serverargs}" -auth "${xserverauthfile}

XCOMM now add the same credentials to the client authority file
XCOMM if '$displayname' already exists do not overwrite it as another
XCOMM server man need it. Add them to the '$xserverauthfile' instead.
for displayname in $authdisplay $hostname$authdisplay; do
     authcookie=`XAUTH list "$displayname" @@
       | sed -n "s/.*$displayname[[:space:]*].*[[:space:]*]//p"` 2>/dev/null;
    if [ "z${authcookie}" = "z" ] ; then
        XAUTH -q << EOF 
add $displayname . $mcookie
EOF
	removelist="$displayname $removelist"
    else
        dummy=$(($dummy+1));
        XAUTH -q -f $xserverauthfile << EOF
add :$dummy . $authcookie
EOF
    fi
done

#if defined(__SCO__) || defined(__UNIXWARE__)
if [ "$REMOTE_SERVER" = "TRUE" ]; then
        exec SHELL_CMD ${client}
else
        XINIT $client $clientargs -- $server $display $serverargs
fi
#else
XINIT $client $clientargs -- $server $display $serverargs
#endif

if [ x"$removelist" != x ]; then
    XAUTH remove $removelist
fi
if [ x"$xserverauthfile" != x ]; then
    rm -f $xserverauthfile
fi
    
/*
 * various machines need special cleaning up
 */
#ifdef __linux__
if command -v deallocvt > /dev/null 2>&1; then
    deallocvt
fi
#endif

#ifdef macII
Xrepair
screenrestore
#endif

#if defined(sun)
kbd_mode -a
#endif
