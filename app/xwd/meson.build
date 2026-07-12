# SPDX-License-Identifier: MIT
#
# Copyright (c) 2026, Oracle and/or its affiliates.
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
  'xwd',
  'c',
  version: '1.0.10',
  license: 'MIT-open-group AND HPND-sell-variant',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

conf = configuration_data()
package_string = ' '.join(meson.project_name(), meson.project_version())
conf.set_quoted('PACKAGE_STRING', package_string)

cc = meson.get_compiler('c')

# Replacement for XORG_DEFAULT_OPTIONS
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Optional dependencies
dep_xkbfile = dependency('xkbfile', required: get_option('xkb'))
conf.set('XKB', dep_xkbfile.found() ? '1' : false,
  description: 'Define to use XkbStdBell')

# Checks for pkg-config packages
dep_libx11  = dependency('x11', required: true)
dep_xproto  = dependency('xproto', required: true, version: '>= 7.0.25')
deps = [dep_xkbfile, dep_libx11, dep_xproto]

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

xwd_sources = [
  'clientwin.c',
  'clientwin.h',
  'dsimple.c',
  'dsimple.h',
  'list.c',
  'list.h',
  'multiVis.c',
  'multiVis.h',
  'wsutils.h',
  'xwd.c',
]

executable(
  'xwd',
  xwd_sources,
  dependencies: deps,
  install: true,
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'xwd.man',
  input: 'man/xwd.man',
  output: 'xwd.1',
  command: [
    prog_sed,
    '-e', f's/__xorgversion__/"@package_string@" "X Version 11"/',
    '-e', 's/__appmansuffix__/1/g',
    '-e', 's/__miscmansuffix__/7/g',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('mandir') / 'man1',
  install_tag: 'man',
)
