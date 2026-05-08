# SPDX-License-Identifier: MIT
#
# Copyright (c) 2025, Pierre Le Marre
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
    'xkbcomp',
    'c',
    default_options: ['buildtype=debugoptimized', 'c_std=gnu99'],
    version: '1.5.0',
    license: 'HPND',
    license_files: 'COPYING',
    meson_version: '>= 1.1.0',
)

cc = meson.get_compiler('c')

cflags = [
    '-fno-strict-aliasing',
    '-Wall',
    '-Wpointer-arith',
    '-Wmissing-declarations',
    '-Wformat=2',
    '-Wstrict-prototypes',
    '-Wmissing-prototypes',
    '-Wnested-externs',
    '-Wbad-function-cast',
    '-Wold-style-definition',
    '-Wunused',
    '-Wuninitialized',
    '-Wshadow',
    '-Wmissing-noreturn',
    '-Wmissing-format-attribute',
    '-Wredundant-decls',
    '-Werror=implicit',
    '-Werror=nonnull',
    '-Werror=init-self',
    '-Werror=main',
    '-Werror=missing-braces',
    '-Werror=sequence-point',
    '-Werror=return-type',
    '-Werror=trigraphs',
    '-Werror=array-bounds',
    '-Werror=write-strings',
    '-Werror=address',
    '-Werror=int-to-pointer-cast',
    '-Werror=pointer-to-int-cast',
]
add_project_arguments(cc.get_supported_arguments(cflags), language: 'c')

bison = find_program('bison', 'win_bison', required: false, version: '>= 2.3a')
if bison.found()
    yacc = bison
    yacc_gen = generator(
        bison,
        output: ['@BASENAME@.c', '@BASENAME@.h'],
        arguments: ['--defines=@OUTPUT1@', '-o', '@OUTPUT0@', '@INPUT@'],
    )
else
    byacc = find_program('byacc', required: false)
    if byacc.found()
        yacc = byacc
        yacc_gen = generator(
            byacc,
            output: ['@BASENAME@.c', '@BASENAME@.h'],
            arguments: ['-H', '@OUTPUT1@', '-o', '@OUTPUT0@', '@INPUT@'],
        )
    else
        error('Could not find a compatible YACC program (bison or byacc)')
    endif
endif

# The XKB config root.
XKBCONFIGROOT = get_option('xkb-config-root')
if XKBCONFIGROOT == ''
    xkeyboard_config_dep = dependency('xkeyboard-config', required: false)
    if xkeyboard_config_dep.found()
        XKBCONFIGROOT = xkeyboard_config_dep.get_variable(pkgconfig: 'xkb_base')
    else
        XKBCONFIGROOT = get_option('prefix') / get_option('datadir') / 'X11' / 'xkb'
    endif
endif

# TODO
# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
# m4_ifndef([XORG_MACROS_VERSION],
#           [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
# XORG_MACROS_VERSION(1.8)
# XORG_DEFAULT_OPTIONS
# TODO: AC_SYS_LARGEFILE

# Checks for pkg-config packages
xkbcomp_deps = {
    'x11': [],
    'xkbfile': [],
}
x11_dep = dependency('x11', version: xkbcomp_deps['x11'])
xkbfile_dep = dependency('xkbfile', version: xkbcomp_deps['xkbfile'])
xproto_dep = dependency('xproto', version: '>= 7.0.17')

# config.h.
xkbcomp_version = meson.project_version()
configh_data = configuration_data()
configh_data.set_quoted('PACKAGE_VERSION', meson.project_version())
configh_data.set_quoted('XKBCONFIGROOT', XKBCONFIGROOT)
configh_data.set_quoted('DFLT_XKB_CONFIG_ROOT', XKBCONFIGROOT)
# Like AC_USE_SYSTEM_EXTENSIONS, what #define to use to get extensions
# beyond the base POSIX function set.
if host_machine.system() == 'sunos'
    system_extensions = '__EXTENSIONS__'
else
    system_extensions = '_GNU_SOURCE'
endif
configh_data.set(system_extensions, 1)
system_ext_define = '#define ' + system_extensions

if cc.has_header_symbol('stdio.h', 'asprintf', prefix: system_ext_define)
    configh_data.set('HAVE_ASPRINTF', 1)
endif
if cc.has_header_symbol('stdlib.h', 'reallocarray', prefix: system_ext_define)
    configh_data.set('HAVE_REALLOCARRAY', 1)
endif
if cc.has_header_symbol('stdlib.h', 'recallocarray', prefix: system_ext_define)
    configh_data.set('HAVE_RECALLOCARRAY', 1)
endif

config_h = configure_file(output: 'config.h', configuration: configh_data)
add_project_arguments('-DHAVE_CONFIG_H', language: 'c')

xkbcomp = executable(
    'xkbcomp',
    'action.c',
    'action.h',
    'alias.c',
    'alias.h',
    'compat.c',
    'compat.h',
    config_h,
    'expr.c',
    'expr.h',
    'geometry.c',
    'indicators.c',
    'indicators.h',
    'keycodes.c',
    'keycodes.h',
    'keymap.c',
    'keytypes.c',
    'listing.c',
    'misc.c',
    'misc.h',
    'parseutils.c',
    'parseutils.h',
    'symbols.c',
    'tokens.h',
    'utils.c',
    'utils.h',
    'vmod.c',
    'vmod.h',
    'xkbcomp.c',
    'xkbcomp.h',
    yacc_gen.process('xkbparse.y'),
    'xkbpath.c',
    'xkbpath.h',
    'xkbscan.c',
    include_directories: include_directories('.'),
    dependencies: [xkbfile_dep, x11_dep, xproto_dep],
    install: true,
    install_dir: join_paths(get_option('prefix'), get_option('bindir')),
)

subdir('man')

pkgconfig_data = configuration_data()
pkgconfig_data.set('prefix', get_option('prefix'))
pkgconfig_data.set('exec_prefix', '${prefix}')
pkgconfig_data.set('bindir', join_paths('${exec_prefix}', get_option('bindir')))
pkgconfig_data.set('datarootdir', join_paths('${prefix}', get_option('datadir')))
pkgconfig_data.set('datadir', '${datarootdir}')
pkgconfig_data.set('XKBCONFIGROOT', XKBCONFIGROOT)
pkgconfig_data.set('PACKAGE_VERSION', meson.project_version())
xkbcomp_deps_list = []
foreach dep, versions : xkbcomp_deps
    if versions.length() == 0
        xkbcomp_deps_list += dep
    else
        foreach version : versions
            xkbcomp_deps_list += [dep, version]
        endforeach
    endif
endforeach
pkgconfig_data.set('XKBCOMP_EXTRA_DEPS', ' '.join(xkbcomp_deps_list))
configure_file(
    input: 'xkbcomp.pc.in',
    output: 'xkbcomp.pc',
    configuration: pkgconfig_data,
    install_dir: join_paths(get_option('prefix'),
                            get_option('libdir'),
                            'pkgconfig'),
)

summary(
    {
        'backend': meson.backend(),
        'buildtype': get_option('buildtype'),
        'id': cc.get_id(),
        'c_args': get_option('c_args'),
        'c_link_args': get_option('c_link_args'),
        'yacc': yacc.full_path() + ' ' + yacc.version(),
    },
    section: 'Compiler',
)
summary(
    {
        'prefix': get_option('prefix'),
        'bindir': get_option('bindir'),
        'libdir': get_option('libdir'),
        'datadir': get_option('datadir'),
        'xkb-config-root': XKBCONFIGROOT,
    },
    section: 'Directories',
)
