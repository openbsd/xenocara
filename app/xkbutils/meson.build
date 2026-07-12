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
  'xkbutils',
  'c',
  version: '1.0.7',
  license: 'HPND-sell-variant AND HPND-DEC AND MIT',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

conf = configuration_data()
conf.set_quoted('PACKAGE_NAME', meson.project_name())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())
package_string = ' '.join(meson.project_name(), meson.project_version())

cc = meson.get_compiler('c')

# Replacement for XORG_DEFAULT_OPTIONS
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Checks for pkg-config packages
dep_xproto     = dependency('xproto', required: true, version: '>= 7.0.17')
dep_inputproto = dependency('inputproto', required: true)
dep_libx11     = dependency('x11', required: true)
dep_libxaw     = dependency('xaw7', required: true)
dep_libxt      = dependency('xt', required: true)

deps = {
  'xkbvleds': [dep_xproto, dep_libxaw, dep_libxt, dep_libx11],
  'xkbbell':  [dep_xproto, dep_inputproto, dep_libx11],
  'xkbwatch': [dep_xproto, dep_libxaw, dep_libxt, dep_libx11],
}

sources = {
  'xkbvleds': ['xkbvleds.c', 'LED.c', 'LED.h', 'LEDP.h', 'utils.c', 'utils.h'],
  'xkbbell':  ['xkbbell.c'],
  'xkbwatch': ['xkbwatch.c', 'LED.c', 'LED.h', 'LEDP.h', 'utils.c', 'utils.h'],
}

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

add_project_arguments('-D_CONST_X_STRING', language: ['c'])

prog_sed = find_program('sed')

foreach p : ['xkbvleds', 'xkbbell', 'xkbwatch']
  executable(
    p,
    [config_h, sources[p]],
    dependencies: deps[p],
    install: true,
  )

  # Man pages
  custom_target(
    f'@p@.man',
    input: f'man/@p@.man',
    output: f'@p@.1',
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
endforeach
