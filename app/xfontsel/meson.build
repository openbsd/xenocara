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
  'xfontsel',
  'c',
  version: '1.1.2',
  license: 'X11 AND SMLNJ AND MIT',
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
dep_libxmu = dependency('xmu', required: true)
dep_libxt  = dependency('xt')
dep_libx11 = dependency('x11', required: true)
dep_xproto = dependency('xproto', required: true, version: '>= 7.0.17')
deps = [dep_libxaw, dep_libxmu, dep_libxt, dep_libx11, dep_xproto]

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

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])
add_project_arguments('-D_CONST_X_STRING', language: ['c'])

xfontsel_sources = [
  config_h,
  'ULabel.c',
  'ULabel.h',
  'ULabelP.h',
  'xfontsel.c',
]

executable(
  'xfontsel',
  xfontsel_sources,
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
  'app-defaults/XFontSel',
  install_dir: appdefaultdir,
  install_tag: 'runtime',
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'xfontsel.man',
  input: 'man/xfontsel.man',
  output: 'xfontsel.1',
  command: [
    prog_sed,
    '-e', 's/__xorgversion__/"xfontsel @0@" "X Version 11"/'.format(meson.project_version()),
    '-e', 's/__appmansuffix__/1/g',
    '-e', f's%__apploaddir__%@appdefaultdir@%g',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('mandir') / 'man1',
  install_tag: 'man',
)
