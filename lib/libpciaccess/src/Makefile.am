# 
# (C) Copyright IBM Corporation 2006
# All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# on the rights to use, copy, modify, merge, publish, distribute, sub
# license, and/or sell copies of the Software, and to permit persons to whom
# the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
# IBM AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

AM_CFLAGS = @PCIACCESS_CFLAGS@

lib_LTLIBRARIES = libpciaccess.la

if LINUX
OS_SUPPORT = linux_sysfs.c linux_devmem.c linux_devmem.h
endif

if FREEBSD
OS_SUPPORT = freebsd_pci.c
endif

if OPENBSD
OS_SUPPORT = openbsd_pci.c
endif

if SOLARIS
OS_SUPPORT = solx_devfs.c pci_tools.h
endif

libpciaccess_la_SOURCES = common_bridge.c \
	common_iterator.c \
	common_init.c \
	common_interface.c \
	common_capability.c \
	common_device_name.c \
	common_map.c \
	pciaccess_private.h \
	$(OS_SUPPORT)

INCLUDES = -I$(top_srcdir)/include

libpciaccess_la_LIBADD = @PCIACCESS_LIBS@

libpciaccess_la_LDFLAGS = -version-number 0:10:2 -no-undefined

libpciaccessincludedir = $(includedir)
libpciaccessinclude_HEADERS = \
    $(top_srcdir)/include/pciaccess.h

noinst_PROGRAMS = scanpci

scanpci_SOURCES = scanpci.c
if SOLARIS
scanpci_LDADD = libpciaccess.la -ldevinfo
else
scanpci_LDADD = libpciaccess.la
endif
