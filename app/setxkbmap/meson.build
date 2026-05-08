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
  'setxkbmap',
  'c',
  version: '1.3.5',
  license: 'HPND',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

conf = configuration_data()
conf.set_quoted('PACKAGE_VERSION', meson.project_version())

# Replacement for XORG_DEFAULT_OPTIONS
cc = meson.get_compiler('c')
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

xkbconfigroot = get_option('xkb-config-root')
if xkbconfigroot == ''
  xkeyboard_config_dep = dependency('xkeyboard-config', required: false)
  if xkeyboard_config_dep.found()
    xkbconfigroot = xkeyboard_config_dep.get_variable(pkgconfig: 'xkb_base')
  else
    xkbconfigroot = get_option('prefix') / get_option('datadir') / 'X11' / 'xkb'
  endif
endif
conf.set_quoted('DFLT_XKB_CONFIG_ROOT', xkbconfigroot)

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language : ['c'])


# Checks for pkg-config packages
dep_libxkbfile = dependency('xkbfile', required: true)
dep_libx11     = dependency('x11', required: true)
# xrandr is needed for the XWayland check
dep_libxrandr  = dependency('xrandr', required: true)

executable(
  'setxkbmap',
  [config_h, 'setxkbmap.c'],
  dependencies: [dep_libxkbfile, dep_libx11, dep_libxrandr],
  install: true
)

prog_sed = find_program('sed')

custom_target(
  'setxkbmap.man',
  input: 'man/setxkbmap.man',
  output: 'setxkbmap.1',
  command: [
    prog_sed,
    '-e', 's/__xorgversion__/"setxkbmap @0@" "X Version 11"/'.format(meson.project_version()),
    '-e', 's/__appmansuffix__/1/g',
    '-e', 's/__miscmansuffix__/7/g',
    '-e', f's|__xkbconfigroot__|"@xkbconfigroot@"|g',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('prefix') / get_option('mandir') / 'man1',
)
