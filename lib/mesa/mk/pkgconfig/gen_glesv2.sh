#!/bin/sh
#
# $OpenBSD: gen_glesv2.sh,v 1.1 2020/08/26 06:02:41 jsg Exp $
#
# Copyright (c) 2010,2011 Jasper Lievisse Adriaanse <jasper@openbsd.org>
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# Generate pkg-config file for libGLESv2

usage() {
	echo "usage: ${0##*/} -c current_directory -o obj_directory"
	exit 1
}

curdir=
objdir=
while getopts "c:o:" flag; do
	case "$flag" in
		c)
			curdir=$OPTARG
			;;
		o)
			objdir=$OPTARG
			;;
		*)
			usage
			;;
	esac
done

[ -n "${curdir}" ] || usage
if [ ! -d "${curdir}" ]; then
	echo "${0##*/}: ${curdir}: not found"
	exit 1
fi
[ -n "${objdir}" ] || usage
if [ ! -w "${objdir}" ]; then
	echo "${0##*/}: ${objdir}: not found or not writable"
	exit 1
fi

lib_version=$(cat ${curdir}/../../VERSION)

pc_file="${objdir}/glesv2.pc"
cat > ${pc_file} << __EOF__
prefix=/usr/X11R6
libdir=\${prefix}/lib
includedir=\${prefix}/include

Name: glesv2
Description: Mesa OpenGL ES 2.0 library
Version: ${lib_version}
Libs: -L\${libdir} -lGLESv2
Libs.private: -lpthread -pthread -lm
Cflags: -I\${includedir}
__EOF__
