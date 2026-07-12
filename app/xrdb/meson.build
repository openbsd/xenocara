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
  'xrdb',
  'c',
  version: '1.2.3',
  license: 'MIT-open-group AND HPND-DEC',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

conf = configuration_data()
package_string = ' '.join(meson.project_name(), meson.project_version())
conf.set_quoted('PACKAGE_STRING', package_string)

# Replaces AC_USE_SYSTEM_EXTENSIONS
if host_machine.system() == 'sunos'
    system_extensions = '-D__EXTENSIONS__'
elif host_machine.system() == 'netbsd'
    system_extensions = '-D_OPENBSD_SOURCE'
else
    system_extensions = '-D_GNU_SOURCE'
endif
add_project_arguments(system_extensions, language: ['c'])

cc = meson.get_compiler('c')

# Replacement for XORG_DEFAULT_OPTIONS
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Checks for system functions
funcs_to_check = {
  'mkstemp':       '#include <stdlib.h>',
  'asprintf':      '#include <stdio.h>',
  'reallocarray':  '#include <stdlib.h>',
}
foreach f, p: funcs_to_check
  has_func = cc.has_function(f, args: system_extensions, prefix: p)
  have_f_name = 'HAVE_' + f.to_upper().underscorify()
  conf.set(have_f_name, has_func ? '1' : false,
    description: f'Define to 1 if you have the \'@f@\' function.')
endforeach

# Find MAXHOSTNAMELEN definition
# Common hidey holes:
#  BSD & Linux - <sys/param.h>
#  Solaris - <netdb.h>

foreach h: ['sys/param.h', 'netdb.h']
  found = cc.has_header_symbol(
    h,
    'MAXHOSTNAMELEN',
    args: system_extensions,
    required: false,
  )
  need_h_name = 'NEED_' + h.to_upper().underscorify()
  conf.set(need_h_name, found ? '1' : false,
    description: f'Define to 1 if you need <@h@> to define MAXHOSTNAMELEN')
endforeach

# xrdb needs to know where to find cpp at runtime - previously set as CppCmd
# in Imake config files for each OS
cpp = get_option('cpp')
if cpp == ''
  # check $PATH first
  cpp = find_program('cpp', required: false)
  if not cpp.found()
    cpp = find_program('cpp', required: true,
      dirs: ['/bin', '/usr/bin', '/usr/lib', '/usr/libexec', '/usr/ccs/lib',
             '/usr/ccs/lbin', '/lib'])
  endif
  cpp_path = cpp.full_path()
else
  cpp_path = cpp.replace(',', '","')
endif
summary('cpp', cpp)
conf.set('CPP', f'"@cpp_path@"', description: 'Path to CPP program')

# Checks for pkg-config packages
dep_libxmuu = dependency('xmuu', required: true)
dep_libx11  = dependency('x11', required: true)
dep_xproto  = dependency('xproto', required: true, version: '>= 7.0.25')
deps = [dep_libxmuu, dep_libx11, dep_xproto]

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

executable(
  'xrdb',
  [config_h, 'xrdb.c'],
  dependencies: deps,
  install: true
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'xrdb.man',
  input: 'man/xrdb.man',
  output: 'xrdb.1',
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
