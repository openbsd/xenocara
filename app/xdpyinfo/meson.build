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
  'xdpyinfo',
  'c',
  version: '1.4.0',
  license: 'MIT-open-group',
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
## Required
dep_libxext       = dependency('xext', required: true)
dep_libx11        = dependency('x11', required: true)
dep_libxtst       = dependency('xtst', required: true)
dep_libx11_xcb    = dependency('x11-xcb', required: true)
dep_libxcb        = dependency('xcb', required: true)
dep_xproto        = dependency('xproto', required: true, version: '>= 7.0.22')

deps = [
  dep_libxext, dep_libxtst, dep_libx11_xcb, dep_libx11, dep_libxcb,
  dep_xproto
]

## Optional
dep_libxxf86vm    = dependency('xxf86vm', required: false, disabler: true)
dep_libxi         = dependency('xi', required: false, disabler: true)
dep_libxrender    = dependency('xrender', required: false, disabler: true)
dep_libxcomposite = dependency('xcomposite', required: false, disabler: true)
dep_libxpresent   = dependency('xpresent', required: false, disabler: true)
dep_libxrandr     = dependency('xrandr', required: false, version: '>= 1.2',
                               disabler: true)

if get_option('dga').allowed()
  dep_libxxf86dga = dependency('xxf86dga', required: false, disabler: true)
else
  dep_libxxf86dga = disabler()
endif
summary('dga', dep_libxxf86dga.found())

if get_option('dmx').allowed()
  dep_libdmx = dependency('dmx', required: false, disabler: true)
else
  dep_libdmx = disabler()
endif
summary('dmx', dep_libdmx.found())

if get_option('xf86misc').allowed()
  dep_libxxf86misc = dependency('xxf86misc', required: false, disabler: true)
else
  dep_libxxf86misc = disabler()
endif
summary('xf86misc', dep_libxxf86misc.found())

if get_option('xinerama').allowed()
  dep_libxinerama = dependency('xinerama', required: false, disabler: true)
else
  dep_libxinerama = disabler()
endif
summary('xinerama', dep_libxinerama.found())

optional_deps = [
  dep_libxxf86vm, dep_libxxf86dga, dep_libxxf86misc, dep_libxi, dep_libxrender,
  dep_libxcomposite, dep_libxinerama, dep_libdmx, dep_libxpresent, dep_libxrandr
]

foreach d: optional_deps
  if d.found()
    deps += d
  endif
endforeach

headers_to_check = {
  'X11/extensions/multibuf.h':    [dep_libxext],
  'X11/extensions/XShm.h':        [dep_libxext],
  'X11/extensions/XKB.h':         [], # dep_libx11 already included below
  'X11/XKBlib.h':                 [], # dep_libx11 already included below
  'X11/extensions/xf86vmode.h':   [dep_libxxf86vm],
  'X11/extensions/xf86vmstr.h':   [dep_libxxf86vm],
  'X11/extensions/xf86vmproto.h': [dep_libxxf86vm],
  'X11/extensions/Xxf86dga.h':    [dep_libxxf86dga],
  'X11/extensions/xf86dgaproto.h':[dep_libxxf86dga],
  'X11/extensions/xf86dga.h':     [dep_libxxf86dga],
  'X11/extensions/xf86dgastr.h':  [dep_libxxf86dga],
  'X11/extensions/xf86misc.h':    [dep_libxxf86misc],
  'X11/extensions/xf86mscstr.h':  [dep_libxxf86misc],
  'X11/extensions/XInput.h':      [dep_libxi],
  'X11/extensions/Xrender.h':     [dep_libxrender],
  'X11/extensions/Xcomposite.h':  [dep_libxcomposite],
  'X11/extensions/Xinerama.h':    [dep_libxinerama],
  'X11/extensions/dmxext.h':      [dep_libdmx],
  'X11/extensions/record.h':      [dep_libxtst],
  'X11/extensions/Xpresent.h':    [dep_libxpresent, dep_libxrandr]
}

foreach h: headers_to_check.keys()
  conf.set(
    'HAVE_' + h.to_upper().underscorify(),
    cc.check_header(
      h,
      dependencies: [headers_to_check[h], dep_libx11, dep_xproto],
      prefix: ['#include <X11/Xlib.h>', '#include <X11/Xproto.h>'],
      required: false
    ) ? '1' : false
  )
endforeach

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

executable(
  'xdpyinfo',
  [config_h, 'xdpyinfo.c'],
  dependencies: deps,
  install: true
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'xdpyinfo.man',
  input: 'man/xdpyinfo.man',
  output: 'xdpyinfo.1',
  command: [
    prog_sed,
    '-e', 's/__xorgversion__/"@0@" "X Version 11"/'.format(package_string),
    '-e', 's/__appmansuffix__/1/g',
    '-e', 's/__miscmansuffix__/7/g',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('mandir') / 'man1',
)
