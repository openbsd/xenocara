# SPDX-License-Identifier: MIT
# Copyright © 2022 Intel Corporation

project(
  'libXvMC',
  'c',
  version : '1.0.15',
  meson_version : '>= 0.62',
  default_options : ['default_library=both'],
)

cc = meson.get_compiler('c')

dep_dl = dependency('dl', required : false)
if not dep_dl.found()
  dep_dl = cc.find_library('dl', required : false)
endif

deps = [
  dependency('x11', version : '>= 1.6'),
  dependency('xext'),
  dependency('xv'),
  dependency('xextproto'),
  dependency('videoproto'),
  dep_dl,
]

conf = configuration_data()
conf.set('MALLOC_0_RETURNS_NULL', 1)

if cc.has_function('shmat')
  conf.set('HAVE_SHMAT', true)
endif

configure_file(
  configuration : conf,
  output : 'config.h',
)

install_headers(
  'include/X11/extensions/vldXvMC.h',
  'include/X11/extensions/XvMClib.h',
  subdir : 'X11/extensions',
)

install_data(
  'XvMC_API.txt',
  install_dir : get_option('datadir') / 'doc' / 'libXvMC',
)

pkg = import('pkgconfig')

inc = include_directories('include')

libXvMC = library(
  'XvMC',
  'src/XvMC.c',
  c_args : ['-DHAVE_CONFIG_H'],
  include_directories : inc,
  dependencies : deps,
  version : '1.0.0',
  darwin_versions  : [ '2.0.0', '2.0.0' ],
  install : true,
)

pkg.generate(
  libXvMC,
  filebase : 'xvmc',
  description : 'The XvMC Library',
  requires : [ 'videoproto', 'xextproto' ],
  version : meson.project_version(),
)

ext_libXvMC = declare_dependency(
  link_with : libXvMC,
  include_directories : inc,
)

meson.override_dependency('xvmc', ext_libXvMC)

sysconfdir = join_paths(get_option('prefix'), get_option('sysconfdir'))

libXvMCW = library(
  'XvMCW',
  'wrapper/XvMCWrapper.c',
  include_directories : inc,
  c_args : [
    '-DHAVE_CONFIG_H',
    '-DXVMC_CONFIGDIR=@0@/X11'.format(sysconfdir),
    '-DXVMC_SOVERSION=".1.0"',
    '-DXV_SOVERSION=".1.0"',
  ],
  dependencies : deps,
  version : '1.0.0',
  darwin_versions  : [ '2.0.0', '2.0.0' ],
  install : true,
)

ext_libXvMCW = declare_dependency(
  link_with : libXvMCW,
  include_directories : inc,
)

pkg.generate(
  libXvMCW,
  name : 'XvMCWrapper',
  filebase : 'xvmc-wrapper',
  description : 'XvMC wrapper library',
  requires : [ 'videoproto', 'xextproto' ],
  version : meson.project_version(),
)

meson.override_dependency('xvmc-wrapper', ext_libXvMCW)

