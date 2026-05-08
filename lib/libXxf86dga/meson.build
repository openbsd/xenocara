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

project(
  'libXxf86dga',
  'c',
  version: '1.1.7',
  license: 'X11',
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

# Obtain compiler/linker options for dependencies
dep_xproto       = dependency('xproto', required: true)
dep_libx11       = dependency('x11', required: true, version: '>= 1.6')
dep_xextproto    = dependency('xextproto', required: true)
dep_libxext      = dependency('xext', required: true)
dep_xf86dgaproto = dependency('xf86dgaproto', required: true,
                              version: '>= 2.0.99.2')

lib = library(
  'Xxf86dga',
  ['src/XF86DGA.c', 'src/XF86DGA2.c'],
  include_directories: 'include',
  dependencies: [dep_xproto, dep_libx11, dep_xextproto, dep_libxext, dep_xf86dgaproto],
  version: '1.0.0',
  install: true,
)

install_headers(
  'include/X11/extensions/Xxf86dga.h',
  'include/X11/extensions/xf86dga1.h',
  subdir: 'X11/extensions',
)

pkg = import('pkgconfig')
pkg.generate(
  name: 'Xxf86dga',
  description: 'XFree86 Direct Graphics Access Extension Library',
  filebase: 'xxf86dga',
  libraries: '-L${libdir} -lXxf86dga',
  requires: ['xf86dgaproto'],
  requires_private: [dep_libx11, dep_libxext],
  url: 'https://gitlab.freedesktop.org/xorg/lib/libxxf86dga/'
)

prog_sed = find_program('sed')

libXxf86dga_manpages = [
  'XDGA',
  'XF86DGA',
  'XFree86-DGA',
  'XDGAQueryExtension',
  'XDGAQueryVersion',
  'XDGAQueryModes',
  'XDGASetMode',
  'XDGAOpenFramebuffer',
  'XDGACloseFramebuffer',
  'XDGASetViewport',
  'XDGAInstallColormap',
  'XDGACreateColormap',
  'XDGASelectInput',
  'XDGAFillRectangle',
  'XDGACopyArea',
  'XDGACopyTransparentArea',
  'XDGAGetViewportStatus',
  'XDGASync',
  'XDGASetClientVersion',
  'XDGAChangePixmapMode',
  'XDGAKeyEventToXKeyEvent'
]

lib_man_suffix = get_option('lib_man_suffix')
foreach man: libXxf86dga_manpages
  custom_target(
    f'@man@.man',
    input: f'man/@man@.man',
    output: f'@man@.@lib_man_suffix@',
    command: [
      prog_sed,
      '-e', 's/__vendorversion__/"libXxf86dga @0@" "X Version 11"/'.format(meson.project_version()),
      '-e', 's/__appmansuffix__/1/g',
      '-e', 's/__filemansuffix__/5/g',
      '-e', f's/__libmansuffix__/@lib_man_suffix@/g',
      '-e', 's/__xservername__/Xorg/g',
      '-e', 's/__xconfigfile__/xorg.conf/g',
      '@INPUT@',
    ],
    capture: true,
    install: true,
    install_dir: get_option('prefix') / get_option('mandir') / f'man@lib_man_suffix@',
  )
endforeach
