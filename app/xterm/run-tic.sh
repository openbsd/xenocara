#!/bin/sh
# $XTermId: run-tic.sh,v 1.1 2006/08/10 21:42:07 tom Exp $
#
# Run tic, either using ncurses' extension feature or filtering out harmless
# messages for the extensions which are otherwise ignored by other versions of
# tic.

TMP=run-tic$$.log
VER=`tic -V 2>/dev/null`
OPT=

case .$VER in
.ncurses*)
	OPT="-x"
	;;
esac

echo "** tic $OPT" "$@"
tic $OPT "$@" 2>$TMP
RET=$?

fgrep -v 'Unknown Capability' $TMP >&2
rm -f $TMP

exit $RET
