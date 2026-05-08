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
  'mkfontscale',
  'c',
  version: '1.2.4',
  license: 'MIT AND MIT-open-group AND HPND-sell-variant',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

conf = configuration_data()
package_string = ' '.join(meson.project_name(), meson.project_version())
conf.set_quoted('PACKAGE_STRING', package_string)

cc = meson.get_compiler('c')

# Replaces AC_USE_SYSTEM_EXTENSIONS
if host_machine.system() == 'sunos'
    add_project_arguments('-D__EXTENSIONS__', language: ['c'])
elif host_machine.system() == 'netbsd'
    add_project_arguments('-D_OPENBSD_SOURCE', language: ['c'])
else
    add_project_arguments('-D_GNU_SOURCE', language: ['c'])
endif

# Replacement for XORG_DEFAULT_OPTIONS
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Checks for system functions / libraries
conf.set('HAVE_VASPRINTF', cc.has_function('vasprintf') ? '1' : false)

# Checks for pkg-config packages
dep_fontenc  = dependency('fontenc', required: true)
dep_freetype = dependency('freetype2', required: true)
dep_xproto   = dependency('xproto', required: true, version: '>= 7.0.25')
dep_zlib     = dependency('zlib', required: true)
mkfontscale_deps = [dep_fontenc, dep_freetype, dep_xproto, dep_zlib]

if get_option('bzip2') == true
  dep_bz2    = cc.find_library('bz2', required: true)
  mkfontscale_deps += dep_bz2
  conf.set('X_BZIP2_FONT_COMPRESSION', 1,
           description: 'Support bzip2 for bitmap fonts')
endif
summary('bzip2', get_option('bzip2'))

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

# mkfontdir script
mkfontdir_conf = configuration_data()
mkfontdir_conf.set('bindir', get_option('prefix') / get_option('bindir'))
mkfontdir = configure_file(
  input: 'mkfontdir.in',
  output: 'mkfontdir',
  configuration: mkfontdir_conf,
  install: true,
  install_dir: get_option('bindir'),
  install_mode: 'rwxr-xr-x',
  install_tag: 'runtime',
)

# mkfontscale compiled binary
mkfontscale_sources = [
  config_h,
  'constlist.c',
  'constlist.h',
  'data.h',
  'hash.c',
  'hash.h',
  'ident.c',
  'ident.h',
  'list.c',
  'list.h',
  'mkfontscale.c',
]

executable(
  'mkfontscale',
  mkfontscale_sources,
  dependencies: mkfontscale_deps,
  install: true
)

# Man pages
prog_sed = find_program('sed')
foreach p : ['mkfontdir', 'mkfontscale']
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
