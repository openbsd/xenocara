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
  'editres',
  'c',
  version: '1.1.1',
  license: 'MIT-open-group',
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

# Required dependencies
dep_libxaw = dependency('xaw7', required: true)
dep_libx11 = dependency('x11', required: true)
dep_libxt  = dependency('xt', required: true, version: '>= 1.0.99.1')
dep_libxmu = dependency('xmu', required: true, version: '>= 1.3.0')
dep_xproto = dependency('xproto', required: true, version: '>= 7.0.17')
deps = [dep_libxaw, dep_libx11, dep_libxt, dep_libxmu, dep_xproto]

# Optional dependencies
if get_option('xkb').enabled()
  dep_libxkbfile = dependency('xkbfile', required: true)
  deps += dep_libxkbfile
endif
conf.set('XKB', get_option('xkb').enabled() ? 1 : false,
         description: 'Define to use XkbStdBell')
summary('xkb', get_option('xkb').enabled())

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

add_project_arguments('-D_CONST_X_STRING', language: ['c'])

editres_sources = [
  config_h,
  'actions.c',
  'comm.c',
  'editres.c',
  'editresP.h',
  'geometry.c',
  'handler.c',
  'setvalues.c',
  'svpopup.c',
  'utils.c',
  'widgets.c',
  'wtree.c',
]

executable(
  'editres',
  editres_sources,
  dependencies: deps,
  install: true
)

# Find directory for installing app-defaults files
appdefaultdir = get_option('appdefaultdir')
if appdefaultdir == ''
  dep_libxt  = dependency('xt', required: true)
  appdefaultdir = dep_libxt.get_variable('appdefaultdir')
endif
summary('appdefaultdir', appdefaultdir)

# App default files  (*.ad)
install_data(
 ['app-defaults/Editres', 'app-defaults/Editres-color'],
 install_dir: appdefaultdir,
 install_tag: 'runtime',
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'editres.man',
  input: 'man/editres.man',
  output: 'editres.1',
  command: [
    prog_sed,
    '-e', f's/__xorgversion__/"@package_string@" "X Version 11"/',
    '-e', 's/__appmansuffix__/1/g',
    '-e', 's/__miscmansuffix__/7/g',
    '-e', f's|__apploaddir__|@appdefaultdir@|g',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('mandir') / 'man1',
  install_tag: 'man',
)
