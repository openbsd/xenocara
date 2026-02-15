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
# Version should match the current XDamage version. XDamageQueryVersion
# returns the version from damagewire.h, NOT the version we set here. But we
# try to keep these the same.  Note that the library has an extra
# digit in the version number to track changes which don't affect the
# protocol, so Xdamage version l.n.m corresponds to protocol version l.n
#
project(
  'libXdamage',
  'c',
  version: '1.1.7',
  license: 'HPND-sell-variant',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

# Replacement for XORG_DEFAULT_OPTIONS
cc = meson.get_compiler('c')
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Check damageext configuration, strip extra digits from package version to
# find the required protocol version
libXdamage_version = meson.project_version()
libXdamage_vers_components = libXdamage_version.split('.')
damageext_version = '.'.join(libXdamage_vers_components[0],
                             libXdamage_vers_components[1])

# Obtain compiler/linker options for dependencies
dep_damageproto = dependency('damageproto', required: true,
                             version: '>=' + damageext_version)
dep_libxfixes   = dependency('xfixes', required: true)
dep_fixesproto  = dependency('fixesproto', required: true)
dep_xextproto   = dependency('xextproto', required: true)
dep_libx11      = dependency('x11', required: true)
dep_xproto      = dependency('xproto', required: true, version: '>= 7.0.22')

lib = library(
  'Xdamage',
  [ 'src/Xdamage.c' ],
  include_directories: 'include/X11/extensions/',
  dependencies: [dep_damageproto, dep_libxfixes, dep_fixesproto,
                 dep_xextproto, dep_libx11, dep_xproto],
  version: '1.1.0',
  install: true,
)

install_headers(
  'include/X11/extensions/Xdamage.h',
  subdir: 'X11/extensions',
)

pkg = import('pkgconfig')
pkg.generate(
  lib,
  description: 'X Damage Library',
  filebase: 'xdamage',
  requires: [ 'xproto', 'damageproto', 'xfixes' ],
  url: 'https://gitlab.freedesktop.org/xorg/lib/libxdamage/'
)
