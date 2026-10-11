# SPDX-License-Identifier: MIT
#
# Copyright (c) 2025, 2026, Oracle and/or its affiliates.
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
  'xwininfo',
  'c',
  version: '1.1.7',
  license: 'MIT AND MIT-open-group AND HPND-sell-variant',
  license_files: 'COPYING',
  meson_version: '>= 1.3.0',
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

# Replacement for AM_ICONV
dep_iconv = dependency('iconv', required: false)
conf.set('HAVE_ICONV',
         cc.has_function('iconv', args: system_extensions,
                         prefix: '#include <iconv.h>',
                         dependencies: dep_iconv) ? '1' : false)
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
  args: system_extensions,
  dependencies: dep_iconv,
  werror: true,
)
conf.set('ICONV_CONST', iconv_const ? 'const' : '')

# Checks for library functions
conf.set('HAVE_STRLCAT',
         cc.has_function('strlcat', args: system_extensions,
                         prefix: '#include <string.h>') ? '1' : false)

# Use POSIX strnlen or the implementation supplied in this module
extra_sources = []
have_strnlen = cc.has_function('strnlen', args: system_extensions,
                               prefix: '#include <string.h>')
conf.set('HAVE_STRNLEN', have_strnlen ? '1' : false,
  description: 'Define to 1 if you have a working strnlen function.')
if not have_strnlen
  extra_sources += ['strnlen.h', 'strnlen.c']
endif

# Check option for using the XCB ICCCM helper functions
dep_xcb_icccm = dependency(
  'xcb-icccm',
  required: get_option('xcb-icccm'),
  version: '>= 0.3.8',
)
conf.set('USE_XCB_ICCCM', dep_xcb_icccm.found() ? '1' : false,
  description: 'Define to 1 to call xcb-icccm library functions instead of local replacements')
summary('xcb-icccm', dep_xcb_icccm.found())

# Check option for using the XCB Error helper functions
dep_xcb_errors = dependency(
  'xcb-errors',
  required: get_option('xcb-errors'),
  version: '>= 1.0',
)
conf.set('USE_XCB_ERRORS', dep_xcb_errors.found() ? '1' : false,
  description: 'Define to 1 to call xcb-errors library functions instead of local replacements')
summary('xcb-errors', dep_xcb_errors.found())

# Obtain compiler/linker options for xwininfo dependencies
dep_xcb       = dependency('xcb', required: true, version: '>= 1.6')
dep_xcb_shape = dependency('xcb-shape', required: true)
deps = [dep_xcb_shape, dep_xcb, dep_xcb_icccm, dep_xcb_errors, dep_iconv]

# Even when using xcb, xproto is still required for Xfuncproto.h
# and libX11 headers for cursorfont.h
dep_libx11 = dependency('x11', required: true).partial_dependency(includes: true)
dep_xproto = dependency('xproto', required: true, version: '>= 7.0.25')
deps += [dep_libx11, dep_xproto]

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

xwininfo_sources = [
  config_h,
  'clientwin.c',
  'clientwin.h',
  'dsimple.c',
  'dsimple.h',
  'xwininfo.c',
  extra_sources,
]

executable(
  'xwininfo',
  xwininfo_sources,
  dependencies: deps,
  install: true
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'xwininfo.man',
  input: 'man/xwininfo.man',
  output: 'xwininfo.1',
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
