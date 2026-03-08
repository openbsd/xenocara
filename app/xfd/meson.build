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
  'xfd',
  'c',
  version: '1.1.5',
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

deps = []

# Internationalization & localization support
use_gettext = cc.has_function('gettext')
if use_gettext == false
  dep_libintl = cc.find_library('intl', required: false)
  use_gettext = cc.has_function('gettext', dependencies: dep_libintl)
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

# Checks for pkg-config packages
dep_libxaw     = dependency('xaw7', required: true)
dep_fontconfig = dependency('fontconfig', required: true)
dep_libxft     = dependency('xft', required: true)
dep_libxrender = dependency('xrender', required: true)
dep_libxmu     = dependency('xmu', required: true)
dep_libx11     = dependency('x11', required: true)
dep_xproto = dependency('xproto', required: true, version: '>= 7.0.25')
deps += [dep_libxaw, dep_fontconfig, dep_libxft, dep_libxrender, dep_libxmu,
         dep_libx11, dep_xproto]

# Optional dependencies
if get_option('xkb').enabled()
  dep_libxkbfile = dependency('xkbfile', required: true)
  deps += dep_libxkbfile
  conf.set('XKB', 1, description: 'Define to use XkbStdBell')
endif
summary('xkb', get_option('xkb').enabled())

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

add_project_arguments('-DXRENDER', language: ['c'])
add_project_arguments('-D_BSD_SOURCE', language: ['c'])
add_project_arguments('-D_CONST_X_STRING', language: ['c'])

executable(
  'xfd',
  [config_h, 'grid.c', 'grid.h', 'gridP.h', 'xfd.c'],
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

# App default files
install_data(
 'app-defaults/Xfd',
 install_dir: appdefaultdir,
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'xfd.man',
  input: 'man/xfd.man',
  output: 'xfd.1',
  command: [
    prog_sed,
    '-e', 's/__xorgversion__/"@0@" "X Version 11"/'.format(package_string),
    '-e', 's/__appmansuffix__/1/g',
    '-e', 's/__libmansuffix__/3/g',
    '-e', 's/__filemansuffix__/5/g',
    '-e', 's/__miscmansuffix__/7/g',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('mandir') / 'man1',
)
