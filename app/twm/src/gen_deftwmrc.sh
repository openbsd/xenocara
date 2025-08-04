#!/bin/sh

rm -f ${1}
echo '/*' >>${1}
echo ' * This file is generated automatically from the default' >>${1}
echo ' * twm bindings file system.twmrc by the twm Makefile.' >>${1}
echo ' */' >>${1}
echo '' >>${1}
echo 'const unsigned char *defTwmrc[] = {' >>${1}
sed \
    -e '/^#/d' \
    -e 's/"/\\"/g' \
    -e 's/^/    (const unsigned char *) "/' \
    -e 's/$/",/' \
    <${2} \
    >>${1}
echo '    (const unsigned char *) 0 };' >>${1}
