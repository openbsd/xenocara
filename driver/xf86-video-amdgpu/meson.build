project(
  'xf86-video-amdgpu',
  'c',
  version: '25.0.0',
  license: 'MIT',
  meson_version: '>=0.59.0',
  default_options: ['warning_level=1']
)

# Dependencies
xorg_dep = dependency('xorg-server', version: '>=1.18', required: true)
xproto_dep = dependency('xproto', required: true)
fontsproto_dep = dependency('fontsproto', required: true)
xf86driproto_dep = dependency('xf86driproto', required: true)
randrproto_dep = dependency('randrproto', required: true)
renderproto_dep = dependency('renderproto', required: true)
videoproto_dep = dependency('videoproto', required: true)
xextproto_dep = dependency('xextproto', version: '>=7.0.99.1', required: true)
libdrm_dep = dependency('libdrm', version: '>=2.4.121', required: true)
libdrm_amdgpu_dep = dependency('libdrm_amdgpu', version: '>=2.4.121', required: true)
gbm_dep = dependency('gbm', required: true)

# Optional dependencies
libudev_dep = dependency('libudev', required: get_option('udev'))
libgl_dep = dependency('gl', required: get_option('glamor'))

# Configurable directories
moduledir = join_paths(get_option('libdir'), get_option('moduledir'))
configdir = join_paths(get_option('prefix'), get_option('configdir'))

# Preprocessor definitions
add_project_arguments('-DHAVE_CONFIG_H', language: 'c')

cc = meson.get_compiler('c')
have_gbm_bo_use_linear = cc.has_header_symbol('gbm.h', 'GBM_BO_USE_LINEAR')
have_gbm_bo_use_front_rendering = cc.has_header_symbol('gbm.h', 'GBM_BO_USE_FRONT_RENDERING')
have_byteswap = cc.has_header('byteswap.h')
xorg_include = xorg_dep.get_variable(pkgconfig: 'sdkdir')
have_fbGlyphs = cc.has_header('fbpict.h', args: ['-I' + xorg_include])
have_dri3_h = cc.has_header('dri3.h', args: ['-I' + xorg_include])
have_present_h = cc.has_header('present.h', args: ['-I' + xorg_include])
have_regionduplicate = cc.has_header('regionstr.h', args: ['-I' + xorg_include])
have_xf86CursorResetCursor = cc.has_header('xf86Cursor.h', args: ['-I' + xorg_include])
have_misyncshm = cc.has_header('misyncshm.h', args: ['-I' + xorg_include])
have_glamor = cc.has_header('glamor.h', args: ['-I' + xorg_include])

# Config header
version = meson.project_version()
varr = version.split('.')
major_version = varr[0]
minor_version = varr[1]
micro_version = varr[2]
config_h = configuration_data()
config_h.set('HAVE_DRI3_H', have_dri3_h ? 1 : 0)
config_h.set('HAVE_PRESENT_H', have_present_h ? 1 : 0)
config_h.set('HAVE_MISYNCSHM_H', have_misyncshm ? 1 : 0)
config_h.set('HAVE_FBGLYPHS', have_fbGlyphs ? 1 : 0)
config_h.set('HAVE_GBM_BO_USE_FRONT_RENDERING', have_gbm_bo_use_front_rendering ? 1 : 0)
config_h.set('HAVE_GBM_BO_USE_LINEAR', have_gbm_bo_use_linear ? 1 : 0)
config_h.set('HAVE_BYTESWAP_H', have_byteswap ? 1 : 0)
config_h.set('HAVE_GLAMOR_H', have_glamor ? 1 : 0)
config_h.set('HAVE_GLAMOR_FINISH', have_glamor ? 1 : 0)
config_h.set('USE_GLAMOR', get_option('glamor').allowed() ? 1 : 0)
config_h.set('HAVE_LIBUDEV', libudev_dep.found() ? 1 : 0)
config_h.set('HAVE_REGIONDUPLICATE', have_regionduplicate ? 1 : 0)
config_h.set('HAVE_XF86_CURSOR_RESET_CURSOR', have_xf86CursorResetCursor ? 1 : 0)
config_h.set('HAVE_XEXTPROTO_71', xextproto_dep.found() ? 1 : 0)
config_h.set('PACKAGE_NAME', meson.project_name())
config_h.set('PACKAGE_VERSION', version)
config_h.set('PACKAGE_VERSION_MAJOR', major_version)
config_h.set('PACKAGE_VERSION_MINOR', minor_version)
config_h.set('PACKAGE_VERSION_PATCHLEVEL', micro_version)
configure_file(
  output: 'config.h',
  configuration: config_h
)

subdir('src')
subdir('man')
subdir('conf')

summary({
  'prefix': get_option('prefix'),
  'exec_prefix': get_option('prefix'),
  'libdir': get_option('libdir'),
  'includedir': get_option('includedir'),
  'configdir': configdir,
  'moduledir': moduledir,
  'CFLAGS': meson.get_compiler('c').get_supported_arguments(['-Wall']),
  'GLAMOR': get_option('glamor'),
  'LIBUDEV': libudev_dep,
}, section: 'Configuration')
