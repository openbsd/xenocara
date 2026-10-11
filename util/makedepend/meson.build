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
  'makedepend',
  'c',
  version: '1.0.10',
  license: 'MIT-open-group AND HPND-sell-variant',
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


# Checks for functions
func_prefix = ['#include <stdio.h>', '#include <stdlib.h>',
               '#include <unistd.h>', '#include <sys/stat.h>']
funcs = ['rename', 'fchmod', 'reallocarray']
foreach f: funcs
  has_func = cc.has_function(f, args: system_extensions, prefix: func_prefix)
  have_f_name = 'HAVE_' + f.to_upper().underscorify()
  conf.set(have_f_name, has_func ? '1' : false,
    description: f'Define to 1 if you have the \'@f@\' function.')
endforeach

conf.set_quoted('OBJSUFFIX', host_machine.system() == 'windows' ? '.obj' : '.o')
conf.set_quoted('INCLUDEDIR', '/usr/include')

config_h = configure_file(output: 'makedepend-config.h', configuration: conf)
add_project_arguments('-DHAVE_CONFIG_H', language: ['c'])

makedepend_sources = [
  'include.c',
  'main.c',
  'parse.c',
  'pr.c',
  'cppsetup.c',
  'ifparser.c',
  'ifparser.h',
  'def.h',
  'imakemdep.h',
]

makedepend = executable(
  'makedepend',
  [config_h, makedepend_sources],
  install: true
)

# Man page
prog_sed = find_program('sed')

custom_target(
  'makedepend.man',
  input: 'makedepend.man',
  output: 'makedepend.1',
  command: [
    prog_sed,
    '-e', f's/__xorgversion__/"@package_string@" "X Version 11"/',
    '@INPUT@',
  ],
  capture: true,
  install: true,
  install_dir: get_option('mandir') / 'man1',
  install_tag: 'man',
)



### Test cases - mostly for bug fixes submitted with a test case so far

test_env = {
 'MAKEDEPEND': makedepend.full_path(),
 'test_srcdir': meson.current_source_dir() / 'tests',
 'test_builddir': meson.current_build_dir() / 'tests',
}

# Test for https://bugs.freedesktop.org/show_bug.cgi?id=28045
test (
  'bugzilla-28045',
  files('tests/28045/makedep.sh')[0],
  env: test_env,
)

# Test for https://gitlab.freedesktop.org/xorg/util/makedepend/issues/1
test (
  'gitlab-1',
  files('tests/1/makedep.sh')[0],
  env: test_env,
)
