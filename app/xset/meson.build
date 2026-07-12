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
  'xset',
  'c',
  version: '1.2.6',
  license: 'MIT-open-group',
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
conf.set('HAVE_USLEEP',
  cc.has_function(
    'usleep',
    args: system_extensions,
    prefix: '#include <unistd.h>'
  ) ? '1' : false,
  description: 'Define to 1 if you have the \'usleep\' function.',
)

# Checks for pkg-config packages
dep_libxmuu = dependency('xmuu', required: true)
dep_libx11  = dependency('x11', required: true)
dep_xproto  = dependency('xproto', required: true, version: '>= 7.0.17')
deps = [dep_libxmuu, dep_libx11, dep_xproto]

dep_libxext = dependency('xext', required: false)
if dep_libxext.found()
  added_to_deps = false
  foreach h: ['X11/extensions/dpms.h', 'X11/extensions/MITMisc.h']
    have_h_name = 'HAVE_' + h.to_upper().underscorify()
    have_h = cc.check_header(
      h,
      required: false,
      dependencies: [dep_libxext, deps],
      prefix: '#include <X11/Xlib.h>',
    )
    conf.set(
      have_h_name,
      have_h ? '1' : false,
      description: f'Define to 1 if you have the <@h@> header file.',
    )
    if have_h and not added_to_deps
      deps += dep_libxext
      added_to_deps = true
    endif
  endforeach
endif

have_xkblib_h = cc.check_header(
  'X11/XKBlib.h',
  required: false,
  dependencies: deps,
  prefix: '#include <X11/Xlib.h>',
)
conf.set(
  'HAVE_X11_XKBLIB_H',
   have_xkblib_h ? '1' : false,
   description: 'Define to 1 if you have the <X11/XKBlib.h> header file.',
)
summary('xkb', have_xkblib_h)

use_xf86misc = false
if get_option('xf86misc').allowed()
  dep_xf86misc = dependency('xxf86misc', required: get_option('xf86misc'))
  if dep_xf86misc.found()
    found = 0
    foreach h: ['X11/extensions/xf86misc.h', 'X11/extensions/xf86mscstr.h']
      have_h_name = 'HAVE_' + h.to_upper().underscorify()
      have_h = cc.check_header(
        h,
        required: false,
        dependencies: [dep_xf86misc, deps],
        prefix: ['#include <X11/Xlib.h>', '#include <X11/Xproto.h>']
      )
      conf.set(
        have_h_name,
        have_h ? '1' : false,
        description: f'Define to 1 if you have the <@h@> header file.',
      )
      found += have_h ? 1 : 0
    endforeach
    if found == 2
      deps += dep_xf86misc
      use_xf86misc = true
    endif
  endif
endif
summary('xf86misc', use_xf86misc)


use_fontcache = false
if get_option('fontcache').allowed()
  dep_fontcache = dependency('xfontcache', required: get_option('fontcache'))
  if dep_fontcache.found()
    found = 0
    foreach h: ['X11/extensions/fontcache.h', 'X11/extensions/fontcacheP.h']
      have_h_name = 'HAVE_' + h.to_upper().underscorify()
      have_h = cc.check_header(
        h,
        required: false,
        dependencies: [dep_fontcache, deps],
        prefix: ['#include <X11/Xlib.h>', '#include <X11/Xproto.h>']
      )
      conf.set(
        have_h_name,
        have_h ? '1' : false,
        description: f'Define to 1 if you have the <@h@> header file.',
      )
      found += have_h ? 1 : 0
    endforeach
    if found == 2
      deps += dep_fontcache
      use_fontcache = true
    endif
  endif
endif
summary('fontcache', use_fontcache)

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

add_project_arguments('-D_BSD_SOURCE', language: ['c'])

executable(
  'xset',
  [config_h, 'xset.c'],
  dependencies: deps,
  install: true
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'xset.man',
  input: 'man/xset.man',
  output: 'xset.1',
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
