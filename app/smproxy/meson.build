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
  'smproxy',
  'c',
  version: '1.0.8',
  license: 'MIT-Open-Group',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

cc = meson.get_compiler('c')

conf = configuration_data()
conf.set_quoted('PACKAGE_STRING',
                ' '.join(meson.project_name(), meson.project_version()))

# Replaces AC_USE_SYSTEM_EXTENSIONS
if host_machine.system() == 'sunos'
    system_extensions = '__EXTENSIONS__'
elif host_machine.system() == 'netbsd'
    system_extensions = '_NETBSD_SOURCE'
else
    system_extensions = '_GNU_SOURCE'
endif
conf.set(system_extensions, 1,
         description: 'Enable non-standardized system API extensions')

# Replacement for XORG_DEFAULT_OPTIONS
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Checks for library functions.
conf.set('HAVE_ASPRINTF', cc.has_function('asprintf') ? '1' : false)
conf.set('HAVE_MKSTEMP', cc.has_function('mkstemp') ? '1' : false)
conf.set('HAVE_MKTEMP', cc.has_function('mktemp') ? '1' : false)

# Checks for pkg-config packages
dep_libsm = dependency('sm', required: true)
dep_libice = dependency('ice', required: true)
dep_libxt = dependency('xt', required: true)
dep_libxmuu = dependency('xmuu', required: true)

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

executable(
  'smproxy',
  [config_h, 'save.c', 'smproxy.c', 'smproxy.h'],
  dependencies: [dep_libsm, dep_libice, dep_libxt, dep_libxmuu],
  install: true
)

prog_sed = find_program('sed')

custom_target(
  'smproxy.man',
  input: 'man/smproxy.man',
  output: 'smproxy.1',
  command: [
    prog_sed,
    '-e', 's/__xorgversion__/"smproxy @0@" "X Version 11"/'.format(meson.project_version()),
    '-e', 's/__appmansuffix__/1/g',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('mandir') / 'man1',
)
