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
# Version should match the current XPresent version. XPresentQueryVersion
# returns the version from xpresentwire.h, NOT the version we set here. But we
# try to keep these the same.  Note that the library has an extra
# digit in the version number to track changes which don't affect the
# protocol, so Xpresent version l.n.m corresponds to protocol version l.n,
# that 'revision' number appears in Xpresent.h and has to be manually
# synchronized.
#
project(
  'libXpresent',
  'c',
  version: '1.0.2',
  license: 'MIT AND HPND-sell-variant',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

# Replacement for XORG_DEFAULT_OPTIONS
cc = meson.get_compiler('c')
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Check presentext configuration, strip extra digits from package version to
# find the required protocol version
libXpresent_version = meson.project_version()
libXpresent_vers_components = libXpresent_version.split('.')
presentext_version = '.'.join(libXpresent_vers_components[0],
                              libXpresent_vers_components[1])

# Obtain compiler/linker options for dependencies
dep_xproto       = dependency('xproto', required: true)
dep_presentproto = dependency('presentproto', required: true,
                              version: '>=' + presentext_version)
dep_xextproto    = dependency('xextproto', required: true)
dep_libx11       = dependency('x11', required: true)
dep_libxext      = dependency('xext', required: true)
dep_libxfixes    = dependency('xfixes', required: true)
dep_libxrandr    = dependency('xrandr', required: true)

lib = library(
  'Xpresent',
  'src/Xpresent.c',
  include_directories: 'include',
  dependencies: [dep_xproto, dep_presentproto, dep_xextproto, dep_libx11,
                 dep_libxext, dep_libxfixes, dep_libxrandr],
  version: '1.0.0',
  install: true,
)

install_headers(
  'include/X11/extensions/Xpresent.h',
  subdir: 'X11/extensions',
)

pkg = import('pkgconfig')
pkg.generate(
  lib,
  description: 'X Present Library',
  filebase: 'xpresent',
  requires: ['xproto', 'presentproto'],
  url: 'https://gitlab.freedesktop.org/xorg/lib/libxpresent/'
)

prog_sed = find_program('sed')

lib_man_suffix = get_option('lib_man_suffix')
foreach man : ['Xpresent', 'XPresentFreeInput', 'XPresentNotifyMSC',
               'XPresentPixmap', 'XPresentQueryCapabilities',
               'XPresentQueryExtension', 'XPresentQueryVersion',
               'XPresentSelectInput', 'XPresentVersion']
  custom_target(
    f'@man@.man',
    input : f'man/@man@.man',
    output : f'@man@.@lib_man_suffix@',
    command : [
      prog_sed,
      '-e', 's/__xorgversion__/"libXpresent @0@" "X Version 11"/'.format(meson.project_version()),
      '-e', f's/__libmansuffix__/@lib_man_suffix@/g',
      '@INPUT@',
    ],
    capture : true,
    install : true,
    install_dir : get_option('prefix') / get_option('mandir') / f'man@lib_man_suffix@',
  )
endforeach
