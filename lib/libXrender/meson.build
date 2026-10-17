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

#
# Version should match the current Render version. XRenderQueryVersion
# returns the version from render.h, NOT the version we set here. But we
# try to keep these the same.  Note that the library has an extra
# digit in the version number to track changes which don't affect the
# protocol, so Xrender version l.n.m corresponds to protocol version l.n
#
project(
  'libXrender',
  'c',
  version: '0.9.13',
  license: 'HPND-sell-variant',
  license_files: 'COPYING',
  meson_version: '>= 1.1.0',
)

doxygen_conf = configuration_data({
  'PACKAGE_BRIEF':  'Library for the Render Extension to the X11 protocol',
  'PACKAGE_NAME':    meson.project_name(),
  'PACKAGE_VERSION': meson.project_version(),
})
configure_file(
  output: 'Doxyfile',
  input: 'Doxyfile.in',
  install: false,
  configuration: doxygen_conf
)

# Replacement for XORG_DEFAULT_OPTIONS
cc = meson.get_compiler('c')
if cc.has_argument('-fno-strict-aliasing')
  add_project_arguments('-fno-strict-aliasing', language: 'c')
endif

# Replacement for XORG_CHECK_MALLOC_ZERO
add_project_arguments('-DMALLOC_0_RETURNS_NULL', language: 'c')

# Check render configuration, strip extra digits from package version to
# find the required protocol version
libXrender_version = meson.project_version()
libXrender_vers_components = libXrender_version.split('.')
renderext_version = '.'.join(libXrender_vers_components[0],
                             libXrender_vers_components[1])

# Obtain compiler/linker options for dependencies
dep_xproto       = dependency('xproto', required: true, version: '>= 7.0.22')
dep_libx11       = dependency('x11', required: true, version: '>= 1.6')
dep_renderproto  = dependency('renderproto', required: true,
                              version: '>=' + renderext_version)

libXrender_sources = [
  'src/AddTrap.c',
  'src/Color.c',
  'src/Composite.c',
  'src/Cursor.c',
  'src/FillRect.c',
  'src/FillRects.c',
  'src/Filter.c',
  'src/Glyph.c',
  'src/Picture.c',
  'src/Poly.c',
  'src/Trap.c',
  'src/Tri.c',
  'src/Xrender.c',
]

lib = library(
  'Xrender',
  libXrender_sources,
  include_directories: 'include/X11/extensions',
  dependencies: [dep_xproto, dep_renderproto, dep_libx11],
  version: '1.3.0',
  install: true,
)

install_headers(
  'include/X11/extensions/Xrender.h',
  subdir: 'X11/extensions',
)

install_data(
  'doc/libXrender.txt',
  install_dir: get_option('datadir') / 'doc' / meson.project_name()
)

pkg = import('pkgconfig')
pkg.generate(
  lib,
  description: 'X Render Library',
  filebase: 'xrender',
  requires: ['xproto', 'renderproto', 'x11'],
  requires_private: ['x11'],  
  url: 'https://gitlab.freedesktop.org/xorg/lib/libxrender/'
)
