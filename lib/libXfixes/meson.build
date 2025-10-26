# SPDX-License-Identifier: MIT
#
# Copyright (c) 2025, Oracle and/or its affiliates.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

#
# Version should match the current XFixes version. XFixesQueryVersion
# returns the version from xfixeswire.h, NOT the version we set here. But we
# try to keep these the same.  Note that the library has an extra
# digit in the version number to track changes which don't affect the
# protocol, so Xfixes version l.n.m corresponds to protocol version l.n,
# that 'revision' number appears in Xfixes.h and has to be manually
# synchronized.
#
project(
  'libXfixes',
  'c',
  version: '6.0.2',
  license: 'HPND-sell-variant AND MIT',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

# Replacement for XORG_DEFAULT_OPTIONS
cc = meson.get_compiler('c')
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Check fixesext configuration, strip extra digits from package version to
# find the required protocol version
libXfixes_version = meson.project_version()
libXfixes_vers_components = libXfixes_version.split('.')
fixesext_version = '.'.join(libXfixes_vers_components[0],
                            libXfixes_vers_components[1])

# Obtain compiler/linker options for dependencies
dep_xproto     = dependency('xproto', required: true, version: '>= 7.0.22')
dep_fixesproto = dependency('fixesproto', required: true,
                            version: '>=' + fixesext_version)
dep_xextproto   = dependency('xextproto', required: true)
dep_libx11      = dependency('x11', required: true, version: '>= 1.6')

lib_sources = [
  'src/Cursor.c',
  'src/Disconnect.c',
  'src/Region.c',
  'src/SaveSet.c',
  'src/Selection.c',
  'src/Xfixes.c',
]

lib = library(
  'Xfixes',
  lib_sources,
  include_directories: 'include/X11/extensions/',
  dependencies: [dep_xproto, dep_fixesproto, dep_xextproto, dep_libx11],
  version: '3.1.0',
  install: true,
)

install_headers(
  'include/X11/extensions/Xfixes.h',
  subdir: 'X11/extensions',
)

pkg = import('pkgconfig')
pkg.generate(
  lib,
  description: 'X Fixes Library',
  filebase: 'xfixes',
  requires: [ 'xproto', 'fixesproto' ],
  url: 'https://gitlab.freedesktop.org/xorg/lib/libxfixes/'
)

prog_sed = find_program('sed')

foreach man : ['Xfixes']
  custom_target(
    f'@man@.man',
    input : f'man/@man@.man',
    output : f'@man@.3',
    command : [
      prog_sed,
      '-e', 's/__xorgversion__/"libXfixes @0@" "X Version 11"/'.format(meson.project_version()),
      '-e', 's/__libmansuffix__/3/g',
      '@INPUT@',
    ],
    capture : true,
    install : true,
    install_dir : get_option('prefix') / get_option('mandir') / 'man3',
  )
endforeach
