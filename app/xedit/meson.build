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
  'xedit',
  'c',
  version: '1.2.5',
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
dep_libxt  = dependency('xt', version: '>= 1.0')
dep_libx11 = dependency('x11', required: true)
dep_xproto = dependency('xproto', required: true)
dep_libm   = cc.find_library('m', required: false)
deps = [dep_libxaw, dep_libxmu, dep_libxt, dep_libx11, dep_xproto, dep_libm]

lispdir = get_option('lispdir')
if lispdir == ''
  lispdir = get_option('prefix') / get_option('libdir') / 'X11' / 'xedit' / 'lisp'
endif

conf.set(
  'HAVE_DECL__SC_PAGESIZE',
  cc.has_header_symbol('unistd.h', '_SC_PAGESIZE') ? '1' : false,
)
conf.set(
  'HAVE_GETPAGESIZE',
  cc.has_function('getpagesize') ? '1' : false,
)

need_realpath = not cc.has_function('realpath')
conf.set('NEED_REALPATH', need_realpath  ? '1' : false)

need_unsetenv = not cc.has_function('unsetenv')
conf.set('NEED_UNSETENV', need_unsetenv  ? '1' : false)

config_h = configure_file(output: 'config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])


#
#    libmp.a
#

libmp_sources = [
  'lisp/mp/mp.c',
  'lisp/mp/mp.h',
  'lisp/mp/mpi.c',
  'lisp/mp/mpr.c',
]

libmp = static_library(
  'mp',
  [config_h, libmp_sources],
  install: false
)

#
#    liblisp.a
#

liblisp_sources = [
  'lisp/bytecode.c',
  'lisp/bytecode.h',
  'lisp/core.c',
  'lisp/core.h',
  'lisp/debugger.c',
  'lisp/debugger.h',
  'lisp/format.c',
  'lisp/format.h',
  'lisp/hash.c',
  'lisp/hash.h',
  'lisp/helper.c',
  'lisp/helper.h',
  'lisp/internal.h',
  'lisp/io.c',
  'lisp/io.h',
  'lisp/lisp.c',
  'lisp/lisp.h',
  'lisp/math.c',
  'lisp/math.h',
  'lisp/package.c',
  'lisp/package.h',
  'lisp/pathname.c',
  'lisp/pathname.h',
  'lisp/private.h',
  'lisp/read.c',
  'lisp/read.h',
  'lisp/regex.c',
  'lisp/regex.h',
  'lisp/require.c',
  'lisp/require.h',
  'lisp/stream.c',
  'lisp/stream.h',
  'lisp/string.c',
  'lisp/string.h',
  'lisp/struct.c',
  'lisp/struct.h',
  'lisp/time.c',
  'lisp/time.h',
  'lisp/write.c',
  'lisp/write.h',
  'lisp/xedit.c',
  'lisp/xedit.h',
]

if need_unsetenv
  liblisp_sources += 'lisp/getenv.c'
  liblisp_sources += 'lisp/setenv.c'
endif

liblisp = static_library(
  'lisp',
  [config_h, liblisp_sources],
  c_args: ['-DLISP', '-DLISPDIR="' + lispdir + '"'],
  dependencies: deps,
  include_directories: include_directories('lisp/re', 'lisp/mp'),
  install: false,
)

#
#     libre.a
#

libre_sources = [
  'lisp/re/re.c',
  'lisp/re/rec.c',
  'lisp/re/re.h',
  'lisp/re/reo.c',
  'lisp/re/rep.h',
]

libre = static_library(
  're',
  [config_h, libre_sources],
  install: false
)

#
#     xedit
#

xedit_sources =	[
  'commands.c',
  'hash.c',
  'hook.c',
  'ispell.c',
  'lisp.c',
  'options.c',
  'tags.c',
  'util.c',
  'util.h',
  'xedit.c',
  'xedit.h',
]

if need_realpath
  xedit_sources += 'realpath.c'
endif

executable(
  'xedit',
  xedit_sources,
  c_args: ['-D_BSD_SOURCE', '-DXEDIT'],
  dependencies: deps,
  include_directories: include_directories('lisp/re'),
  install: true,
  link_with: [libre, liblisp, libmp]
)


# lisp/lsp
lisp_lsp_sources = [
  'hash.c',
  'lisp/lsp.c',
]

if need_realpath
  lisp_lsp_sources += 'realpath.c'
endif

executable(
  'lsp',
  lisp_lsp_sources,
  c_args: ['-DLISP', '-DLISPDIR="' + lispdir + '"', '-D_BSD_SOURCE'],
  dependencies: [dep_libm],
  include_directories: include_directories('lisp/re', 'lisp/mp'),
  install: false,
  link_with: [liblisp, libre, libmp],
)

# re/tests
subdir('lisp/re')

# Find directory for installing app-defaults files
appdefaultdir = get_option('appdefaultdir')
if appdefaultdir == ''
  appdefaultdir = dep_libxt.get_variable('appdefaultdir')
endif
summary('appdefaultdir', appdefaultdir)

# App default files
install_data(
  ['app-defaults/Xedit-color', 'app-defaults/Xedit'],
  install_dir: appdefaultdir,
  install_tag: 'runtime',
)


lisp_data = [
  'lisp/modules/lisp.lsp',
  'lisp/modules/xedit.lsp',
  'lisp/modules/syntax.lsp',
  'lisp/modules/indent.lsp',
]
install_data(
  lisp_data,
  install_dir: lispdir,
  install_tag: 'runtime',
)


progmodes_data = [
  'lisp/modules/progmodes/auto.lsp',
  'lisp/modules/progmodes/c.lsp',
  'lisp/modules/progmodes/html.lsp',
  'lisp/modules/progmodes/imake.lsp',
  'lisp/modules/progmodes/lisp.lsp',
  'lisp/modules/progmodes/make.lsp',
  'lisp/modules/progmodes/man.lsp',
  'lisp/modules/progmodes/patch.lsp',
  'lisp/modules/progmodes/perl.lsp',
  'lisp/modules/progmodes/python.lsp',
  'lisp/modules/progmodes/rpm.lsp',
  'lisp/modules/progmodes/sgml.lsp',
  'lisp/modules/progmodes/sh.lsp',
  'lisp/modules/progmodes/xconf.lsp',
  'lisp/modules/progmodes/xlog.lsp',
  'lisp/modules/progmodes/xrdb.lsp',
]
install_data(
  progmodes_data,
  install_dir: lispdir / 'progmodes',
  install_tag: 'runtime',
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'xedit.man',
  input: 'man/xedit.man',
  output: 'xedit.1',
  command: [
    prog_sed,
    '-e', f's/__vendorversion__/"@package_string@" "X Version 11"/',
    '-e', 's/__appmansuffix__/1/g',
    '-e', 's/__libmansuffix__/3/g',
    '-e', 's/__miscmansuffix__/7/g',
    '-e', f's%__apploaddir__%@appdefaultdir@%g',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('mandir') / 'man1',
  install_tag: 'man',
)
