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
  'xclock',
  'c',
  version: '1.2.0',
  license: 'MIT-open-group AND HPND AND MIT',
  license_files: 'COPYING',
  meson_version: '>= 1.3.0',
)

conf = configuration_data()
conf.set_quoted('PACKAGE_STRING',
                ' '.join(meson.project_name(), meson.project_version()))

cc = meson.get_compiler('c')

# Replacement for XORG_DEFAULT_OPTIONS
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Checks for library functions.
conf.set('HAVE_XLOCALE_H', cc.has_header('xlocale.h') ? '1' : false)
conf.set('HAVE_STRLCPY', cc.has_function('strlcpy') ? '1' : false)
conf.set('HAVE_GETPID', cc.has_function('getpid') ? '1' : false)
conf.set('HAVE_NL_LANGINFO', cc.has_function('nl_langinfo') ? '1' : false)

# Replacement for AM_ICONV
dep_iconv = dependency('iconv', required: false)
conf.set('HAVE_ICONV',
         cc.has_function('iconv', dependencies: dep_iconv) ? '1' : false)
iconv_const = cc.compiles('''
#include <iconv.h>
#include <string.h>

int main(int argc, char **argv) {
  iconv_t cd_utf8_to_88591 = iconv_open ("ISO8859-1", "UTF-8");
  if (cd_utf8_to_88591 != (iconv_t)(-1)) {
    const char *input = "This is only a test";
    const char *inptr = input;
    size_t inbytesleft = strlen (input);
    char output[32];
    char *outptr = output;
    size_t outbytesleft = sizeof (output);
    size_t result = iconv(cd_utf8_to_88591,
                          &inptr, &inbytesleft,
                          &outptr, &outbytesleft);
    return (int) result;
  }
}
''',
  dependencies: dep_iconv,
  werror: true,
)
conf.set('ICONV_CONST', iconv_const ? 'const' : '')

# Required dependencies
dep_libxaw = dependency('xaw7', required: true)
dep_libxmu = dependency('xmu', required: true)
dep_libx11 = dependency('x11', required: true)
dep_xproto = dependency('xproto', required: true, version: '>= 7.0.17')
deps = [dep_libxaw, dep_libxmu, dep_libx11, dep_xproto, dep_iconv]

# Optional dependencies
extra_deps = []
if get_option('xft').enabled()
  dep_libxrender = dependency('xrender', required: true)
  dep_libxft = dependency('xft', required: true)
  deps += [dep_libxrender, dep_libxft]
  # Solaris 11 has sqrt in libc but needs libm for sincos
  dep_libm = cc.find_library('m', required : false)
  sincos_prefix = ['#define _GNU_SOURCE', '#include <math.h>']
  have_sincos = cc.has_function('sincos', prefix: sincos_prefix)
  if have_sincos != true
    have_sincos = cc.has_function('sincos', dependencies: dep_libm,
                                  prefix: sincos_prefix)
    if have_sincos == true
      extra_deps += dep_libm
    elif cc.has_function('sqrt', prefix: '#include <math.h>') != true
      extra_deps += dep_libm
    endif
  endif
  conf.set('HAVE_SINCOS', have_sincos ?  '1' : false,
           description: 'Define to 1 if you have the sincos function.')
  conf.set('XRENDER', 1, description: 'Define to use X Render Extension')
  conf.set('XFREE86_FT2', 1, description: 'Define to use Xft2 library')
endif
summary('xft', get_option('xft').enabled())

if get_option('xkb').enabled()
  dep_libxkbfile = dependency('xkbfile', required: true)
  deps += dep_libxkbfile
  conf.set('XKB', 1, description: 'Define to use XkbStdBell')
endif
summary('xkb', get_option('xkb').enabled())

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

add_project_arguments('-D_CONST_X_STRING', language: ['c'])

executable(
  'xclock',
  [config_h, 'Clock.c', 'Clock.h', 'ClockP.h', 'xclock.c'],
  dependencies: [deps, extra_deps],
  install: true
)

# Find directory for installing app-defaults files
appdefaultdir = get_option('appdefaultdir')
if appdefaultdir == ''
  dep_libxt  = dependency('xt', required: true)
  appdefaultdir = dep_libxt.get_variable('appdefaultdir')
endif
summary('appdefaultdir', appdefaultdir)

# App default files  (*.ad)
install_data(
 ['app-defaults/XClock', 'app-defaults/XClock-color'],
 install_dir: appdefaultdir,
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'xclock.man',
  input: 'man/xclock.man',
  output: 'xclock.1',
  command: [
    prog_sed,
    '-e', 's/__xorgversion__/"xclock @0@" "X Version 11"/'.format(meson.project_version()),
    '-e', 's/__appmansuffix__/1/g',
    '-e', 's/__libmansuffix__/3/g',
    '-e', 's/__filemansuffix__/5/g',
    '-e', 's/__miscmansuffix__/7/g',
    '-e', f's|__apploaddir__|@appdefaultdir@|g',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('mandir') / 'man1',
)
