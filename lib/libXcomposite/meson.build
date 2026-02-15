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
# Version should match the current XComposite version. XCompositeQueryVersion
# returns the version from xcompositewire.h, NOT the version we set here.
# But we try to keep these the same.  Note that the library has an extra
# digit in the version number to track changes which don't affect the
# protocol, so Xcomposite version l.n.m corresponds to protocol version l.n
# that 'revision' number appears in Xcomposite.h and has to be manually
# synchronized.
#

project(
  'libXcomposite',
  'c',
  version: '0.4.7',
  license: 'HPND-sell-variant AND MIT',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

# Replacement for XORG_DEFAULT_OPTIONS
cc = meson.get_compiler('c')
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Check composite configuration, strip extra digits from package version to
# find the required protocol version
libXcomposite_version = meson.project_version()
libXcomposite_vers_components = libXcomposite_version.split('.')
compositeext_version = '.'.join(libXcomposite_vers_components[0],
                                libXcomposite_vers_components[1])

# Obtain compiler/linker options for dependencies
dep_compositeproto  = dependency('compositeproto', required: true,
                             version: '>=' + compositeext_version)
dep_xproto          = dependency('xproto', required: true, version: '>= 7.0.22')
dep_libx11          = dependency('x11', required: true)
dep_libxfixes       = dependency('xfixes', required: true)

lib = library(
  'Xcomposite',
  'src/Xcomposite.c',
  include_directories: 'include',
  dependencies: [dep_compositeproto, dep_xproto, dep_libx11, dep_libxfixes],
  version: '1.0.0',
  install: true,
)

install_headers(
  'include/X11/extensions/Xcomposite.h',
  subdir: 'X11/extensions',
)

pkg = import('pkgconfig')
pkg.generate(
  lib,
  description: 'X Composite Extension Library',
  filebase: 'xcomposite',
  requires: ['xproto', 'compositeproto'],
  url: 'https://gitlab.freedesktop.org/xorg/lib/libxcomposite/'
)

build_docs = false
xmlto = find_program('xmlto', required: get_option('docs'))
if xmlto.found()
  if xmlto.version().version_compare('>= 0.0.20')
    build_docs = true
  elif get_option('docs').enabled()
    error('xmlto >= 0.0.20 required if -Ddocs=enabled, found', xmlto.version())
  endif
endif

if build_docs
  prog_sed = find_program('sed')
  build_docs = prog_sed.found()
endif
summary('Build documentation', build_docs)

if build_docs
  lib_man_suffix = get_option('lib_man_suffix')

  XComposite_mansuff = custom_target(
    output: 'Xcomposite.__libmansuffix__',
    input: 'man/Xcomposite.xml',
    command: [xmlto, 'man', '@INPUT@', '-o', '@OUTDIR@'],
    install: false,
  )
  XComposite_man = custom_target(
    output: 'Xcomposite.man',
    input: XComposite_mansuff,
    command: ['mv', '@INPUT@', '@OUTPUT@'],
    install: false,
  )
  custom_target(
    input: XComposite_man,
    output: f'Xcomposite.@lib_man_suffix@',
    command: [
      prog_sed,
      '-e', 's/__vendorversion__/"libXcomposite @0@" "X Version 11"/'.format(meson.project_version()),
      '-e', f's/__libmansuffix__/@lib_man_suffix@/g',
      '@INPUT@',
    ],
    capture: true,
    install: true,
    install_dir: get_option('mandir') / f'man@lib_man_suffix@',
  )

  libXcomposite_shadowmanpages = [
    'XCompositeQueryExtension',
    'XCompositeQueryVersion',
    'XCompositeVersion',
    'XCompositeRedirectWindow',
    'XCompositeRedirectSubwindows',
    'XCompositeUnredirectWindow',
    'XCompositeUnredirectSubwindows',
    'XCompositeCreateRegionFromBorderClip',
    'XCompositeNameWindowPixmap',
    'XCompositeGetOverlayWindow',
    'XCompositeReleaseOverlayWindow'
  ]

  foreach man: libXcomposite_shadowmanpages
    custom_target(
      input: f'man/@man@.man',
      output: f'@man@.@lib_man_suffix@',
      command: [
        prog_sed,
        '-e', f's/__libmansuffix__/@lib_man_suffix@/g',
        '@INPUT@',
      ],
      capture: true,
      install: true,
      install_dir: get_option('prefix') / get_option('mandir') / f'man@lib_man_suffix@',
    )
  endforeach
endif
