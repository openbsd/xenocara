#!/bin/sh

# Test case for bug https://gitlab.freedesktop.org/xorg/util/makedepend/issues/1

set -e

if [ "x$test_srcdir" = "x" ]; then
    test_srcdir=..
fi

if [ "x$test_builddir" = "x" ]; then
    test_builddir=..
fi

if [ "x$MAKEDEPEND" = "x" ]; then
    MAKEDEPEND=makedepend
fi

mkdir -p ${test_builddir}/1
cd ${test_builddir}/1

pwd

# Create test conditions:
#  - directory named "one" containing file one.cpp which includes def.h in the same directory
#  - directory named "two" containing file two.cpp which includes def.h in the same directory
mkdir -p one two

# 2 headers with the same name but in different directories
touch one/def.h two/def.h
cp ${test_srcdir}/1/one.cpp one/
cp ${test_srcdir}/1/two.cpp two/

# two/two.cpp depends on two/def.h (not the one seen during exploration in previous directory)
$MAKEDEPEND -f- -I. one/one.cpp two/two.cpp | grep "two/two.o: one/def.h" && false


# Clean up
rm -rf one two
