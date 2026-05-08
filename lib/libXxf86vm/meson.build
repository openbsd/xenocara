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
  'libXxf86vm',
  'c',
  version: '1.1.7',
  license: 'X11',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

# Replacement for XORG_CHECK_MALLOC_ZERO
add_project_arguments('-DMALLOC_0_RETURNS_NULL', language: 'c')

# Obtain compiler/linker options for dependencies
dep_xproto      = dependency('xproto', required: true)
dep_libx11      = dependency('x11', required: true, version: '>= 1.6')
dep_xextproto   = dependency('xextproto', required: true)
dep_libxext     = dependency('xext', required: true)
dep_xf86vmproto = dependency('xf86vidmodeproto', required: true,
                              version: '>= 2.2.99.1')

lib = library(
  'Xxf86vm',
  'src/XF86VMode.c',
  include_directories: 'include',
  dependencies: [dep_xproto, dep_libx11, dep_xextproto, dep_libxext, dep_xf86vmproto],
  version: '1.0.0',
  install: true,
)

install_headers(
  'include/X11/extensions/xf86vmode.h',
  subdir: 'X11/extensions',
)

pkg = import('pkgconfig')
pkg.generate(
  name: 'Xxf86vm',
  description: 'XFree86 Video Mode Extension Library',
  filebase: 'xxf86vm',
  libraries: '-L${libdir} -lXxf86vm',
  requires: ['xf86vidmodeproto'],
  requires_private: [dep_libx11, dep_libxext],
  url: 'https://gitlab.freedesktop.org/xorg/lib/libxxf86vm/'
)

prog_sed = find_program('sed')

libXxf86vm_manpages = [
  'XF86VM',
  'XF86VidModeQueryExtension',
  'XF86VidModeQueryVersion',
  'XF86VidModeGetModeLine',
  'XF86VidModeGetAllModeLines',
  'XF86VidModeAddModeLine',
  'XF86VidModeDeleteModeLine',
  'XF86VidModeModModeLine',
  'XF86VidModeSwitchMode',
  'XF86VidModeSwitchToMode',
  'XF86VidModeLockModeSwitch',
  'XF86VidModeGetMonitor',
  'XF86VidModeGetViewPort',
  'XF86VidModeSetViewPort',
  'XF86VidModeValidateModeLine',
  'XF86VidModeSetClientVersion',
  'XF86VidModeGetDotClocks',
  'XF86VidModeGetGamma',
  'XF86VidModeSetGamma',
  'XF86VidModeSetGammaRamp',
  'XF86VidModeGetGammaRamp',
  'XF86VidModeGetGammaRampSize',
  'XF86VidModeGetPermissions'
]

lib_man_suffix = get_option('lib_man_suffix')
foreach man: libXxf86vm_manpages
  custom_target(
    f'@man@.man',
    input: f'man/@man@.man',
    output: f'@man@.@lib_man_suffix@',
    command: [
      prog_sed,
      '-e', 's/__vendorversion__/"libXxf86vm @0@" "X Version 11"/'.format(meson.project_version()),
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
