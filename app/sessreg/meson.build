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
  'sessreg',
  'c',
  version: '1.1.4',
  license: 'MIT AND MIT-open-group',
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
    system_extensions = '_OPENBSD_SOURCE'
else
    system_extensions = '_GNU_SOURCE'
endif
conf.set(system_extensions, 1,
         description: 'Enable non-standardized system API extensions')

# Replacement for XORG_DEFAULT_OPTIONS
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Checks for header files.
conf.set('HAVE_LASTLOG_H', cc.has_header('lastlog.h') ? '1' : false)
conf.set('HAVE_PWD_H', cc.has_header('pwd.h') ? '1' : false)
conf.set('HAVE_UTMP_H', cc.has_header('utmp.h') ? '1' : false)
conf.set('HAVE_UTMPX_H', cc.has_header('utmpx.h') ? '1' : false)
conf.set('HAVE_SYS_PARAM_H', cc.has_header('sys/param.h') ? '1' : false)

# Checks for typedefs, structures, and compiler characteristics.
if cc.has_header('utmp.h')
  foreach m : ['ut_host', 'ut_id', 'ut_pid', 'ut_type']
    conf.set(
      'HAVE_STRUCT_UTMP_' + m.to_upper(),
      cc.has_member(
        'struct utmp',
        m,
        prefix: ['#include <sys/types.h>', '#include <utmp.h>']
      ) ? '1' : false
    )
  endforeach
endif
if cc.has_header('utmpx.h') 
  conf.set(
    'HAVE_STRUCT_UTMPX_UT_SYSLEN',
    cc.has_member(
      'struct utmpx',
      'ut_syslen',
      prefix: ['#include <sys/types.h>', '#include <utmpx.h>']
    ) ? '1' : false
  )
endif
conf.set(
  'HAVE_STRUCT_LASTLOG',
  cc.has_type(
    'struct lastlog',
    prefix: [
      '#include <sys/types.h>',
      cc.has_header('utmp.h') ? '#include <utmp.h>' : '',
      cc.has_header('lastlog.h') ? '#include <lastlog.h>' : '',
    ]
  ) ? '1' : false
)

# Checks for library functions.
conf.set('HAVE_PUTUTLINE', cc.has_function('pututline') ? '1' : false)
conf.set('HAVE_UPDWTMPX', cc.has_function('updwtmpx') ? '1' : false)
conf.set('HAVE_UTMPXNAME', cc.has_function('utmpxname') ? '1' : false)

# Obtain compiler/linker options for dependencies
dep_xproto = dependency('xproto', required: true, version: '>= 7.0.25')

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

executable(
  'sessreg',
  [config_h, 'sessreg.h', 'sessreg.c'],
  dependencies: [dep_xproto],
  install: true
)

prog_sed = find_program('sed')

filenames_sed_in = cc.preprocess(
  'man/filenames.sed.c',
  output: 'filenames.sed.in'
)

filenames_sed = custom_target(
  'filenames.sed',
  input: filenames_sed_in,
  output: 'filenames.sed',
  command: [prog_sed, '-n', '-f', files('man/extract.sed'), '@INPUT@'],
  capture: true,
  install: false
)

custom_target(
  'sessreg.man',
  input: 'man/sessreg.man',
  output: 'sessreg.1',
  depends: filenames_sed,
  command: [
    prog_sed,
    '-e', 's/__xorgversion__/"sessreg @0@" "X Version 11"/'.format(meson.project_version()),
    '-e', 's/__appmansuffix__/1/g',
    '-e', 's/__libmansuffix__/3/g',    
    '-e', 's/__filemansuffix__/5/g',
    '-f', filenames_sed,
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('prefix') / get_option('mandir') / 'man1',
)
