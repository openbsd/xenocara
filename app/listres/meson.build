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
  'listres',
  'c',
  version: '1.0.7',
  license: 'X11',
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

# Checks for pkg-config packages
dep_libxaw = dependency('xaw7', required: true)
dep_xproto = dependency('xproto', required: true)
dep_libxt  = dependency('xt', required: true)
dep_libxmu = dependency('xmu', required: true)
deps = [dep_libxaw, dep_xproto, dep_libxt, dep_libxmu]

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

add_project_arguments('-D_CONST_X_STRING', language: ['c'])

executable(
  'listres',
  [config_h, 'listres.c'],
  dependencies: deps,
  install: true
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'listres.man',
  input: 'man/listres.man',
  output: 'listres.1',
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
