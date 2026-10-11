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
  'xload',
  'c',
  version: '1.2.2',
  license: 'X11 AND MIT',
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

# Checks for pkg-config packages
dep_libxaw = dependency('xaw7', required: true)
dep_libxmu = dependency('xmu', required: true)
dep_libxt  = dependency('xt', required: true)
dep_libx11 = dependency('x11', required: true)
dep_xproto = dependency('xproto', required: true, version: '>= 7.0.17')
deps = [dep_libxaw, dep_libxmu, dep_libxt, dep_libx11, dep_xproto]

# Checks for library functions.
conf.set('HAVE_ASPRINTF',
         cc.has_function('asprintf', args: system_extensions,
                         prefix: '#include <stdio.h>') ? '1' : false)

# Internationalization & localization support
libintl_prefix = '#include <libintl.h>'
use_gettext = cc.has_function('gettext', prefix: libintl_prefix)
if use_gettext == false
  dep_libintl = cc.find_library('intl', required: false)
  use_gettext = cc.has_function('gettext', dependencies: dep_libintl,
                                prefix: libintl_prefix)
  if use_gettext == true
    deps += dep_libintl
  endif
endif
conf.set('USE_GETTEXT', use_gettext ? 1 : false,
         description: 'Define to 1 if you want to use the gettext() function.')
summary('Use gettext', use_gettext)
subdir('po')

# where to install localized messages
localedir = get_option('prefix') / get_option('localedir')
conf.set_quoted('LOCALEDIR', localedir)


### How to check load average on various OS'es:

# getloadavg: 4.3BSD-Reno & later, glibc 2.2 & later, Solaris 7 & later
# BSD & GNU libc use <stdlib.h>, Solaris requires <sys/loadavg.h>
getloadavg_prefix = [ '#include <stdlib.h>' ]
if cc.check_header('sys/loadavg.h')
  getloadavg_prefix += '#include <sys/loadavg.h>'
endif
conf.set('HAVE_GETLOADAVG',
         cc.has_function('getloadavg', args: system_extensions,
                         prefix: getloadavg_prefix) ? '1' : false)

# Replacement for AC_CHECK_HEADERS
foreach h: ['sys/loadavg.h', 'paths.h', 'protocols/rwhod.h']
  have_h_name = 'HAVE_' + h.to_upper().underscorify()
  have_h = cc.check_header(
    h,
    required: false,
    args: system_extensions,
    prefix: '#include <sys/types.h>',
  )
  conf.set(
    have_h_name,
    have_h ? '1' : false,
    description: f'Define to 1 if you have the <@h@> header file.',
  )
endforeach

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

add_project_arguments('-D_CONST_X_STRING', language: ['c'])

xload_sources = [
  config_h,
  'get_load.c',
  'get_rload.c',
  'xload.c',
  'xload.h',
]

executable(
  'xload',
  xload_sources,
  dependencies: deps,
  install: true
)

# Find directory for installing app-defaults files
appdefaultdir = get_option('appdefaultdir')
if appdefaultdir == ''
  appdefaultdir = dep_libxt.get_variable('appdefaultdir')
endif
summary('appdefaultdir', appdefaultdir)

# App default files  (*.ad)
install_data(
 'app-defaults/XLoad',
 install_dir: appdefaultdir,
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'xload.man',
  input: 'man/xload.man',
  output: 'xload.1',
  command: [
    prog_sed,
    '-e', f's/__xorgversion__/"@package_string@" "X Version 11"/',
    '-e', 's/__appmansuffix__/1/g',
    '-e', 's/__drivermansuffix__/4/g',
    '-e', 's/__miscmansuffix__/7/g',
    '-e', f's%__apploaddir__%@appdefaultdir@%g',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('mandir') / 'man1',
)
