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
# Version should match the current Randr version. XRRQueryVersion
# returns the version from randr.h, NOT the version we set here. But we
# try to keep these the same.  Note that the library has an extra
# digit in the version number to track changes which don't affect the
# protocol, so Xrandr version l.n.m corresponds to protocol version l.n
#
project(
  'libXrandr',
  'c',
  version: '1.5.5',
  license: 'HPND-sell-variant',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

# Replacement for XORG_DEFAULT_OPTIONS
cc = meson.get_compiler('c')
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Replacement for XORG_CHECK_MALLOC_ZERO
add_project_arguments('-DMALLOC_0_RETURNS_NULL', language: 'c')

# Check randr configuration, strip extra digits from package version to
# find the required protocol version
libXrandr_version = meson.project_version()
libXrandr_vers_components = libXrandr_version.split('.')
randrext_version = '.'.join(libXrandr_vers_components[0],
                            libXrandr_vers_components[1])

# Obtain compiler/linker options for dependencies
dep_libx11      = dependency('x11', required: true, version: '>= 1.6')
dep_randrproto  = dependency('randrproto', required: true,
                             version: '>=' + randrext_version)
dep_libxext     = dependency('xext', required: true)
dep_xextproto   = dependency('xextproto', required: true)
dep_libxrender  = dependency('xrender', required: true)
dep_renderproto = dependency('renderproto', required: true)

libXrandr_sources = [
  'src/Xrandr.c',
  'src/XrrConfig.c',
  'src/XrrCrtc.c',
  'src/XrrMode.c',
  'src/XrrOutput.c',
  'src/XrrProperty.c',
  'src/XrrScreen.c',
  'src/XrrProvider.c',
  'src/XrrProviderProperty.c',
  'src/XrrMonitor.c'
]

lib = library(
  'Xrandr',
  libXrandr_sources,
  include_directories: 'include/X11/extensions',
  dependencies: [dep_randrproto, dep_libxext, dep_xextproto,
                 dep_libxrender, dep_renderproto, dep_libx11],
  version: '2.2.0',
  install: true,
)

install_headers(
  'include/X11/extensions/Xrandr.h',
  subdir: 'X11/extensions',
)

pkg = import('pkgconfig')
pkg.generate(
  lib,
  description: 'X RandR Library',
  filebase: 'xrandr',
  requires: ['xproto', 'randrproto'],
  url: 'https://gitlab.freedesktop.org/xorg/lib/libxrandr/'
)

prog_sed = find_program('sed')

libXrandr_manpages = [
  'Xrandr',
  'XRRQueryExtension',
  'XRRQueryVersion',
  'XRRGetScreenInfo',
  'XRRFreeScreenConfigInfo',
  'XRRSetScreenConfig',
  'XRRSetScreenConfigAndRate',
  'XRRConfigRotations',
  'XRRConfigTimes',
  'XRRConfigSizes',
  'XRRConfigRates',
  'XRRConfigCurrentConfiguration',
  'XRRConfigCurrentRate',
  'XRRRootToScreen',
  'XRRSelectInput'
]

lib_man_suffix = get_option('lib_man_suffix')
foreach man: libXrandr_manpages
  custom_target(
    f'@man@.man',
    input: f'man/@man@.man',
    output: f'@man@.@lib_man_suffix@',
    command: [
      prog_sed,
      '-e', 's/__vendorversion__/"libXrandr @0@" "X Version 11"/'.format(meson.project_version()),
      '-e', f's/__libmansuffix__/@lib_man_suffix@/g',
      '@INPUT@',
    ],
    capture: true,
    install: true,
    install_dir: get_option('prefix') / get_option('mandir') / f'man@lib_man_suffix@',
  )
endforeach
