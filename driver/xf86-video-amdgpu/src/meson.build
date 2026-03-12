srcs = [
  'amdgpu_bo_helper.c',
  'amdgpu_dri2.c',
  'amdgpu_dri3.c',
  'amdgpu_drm_queue.c',
  'amdgpu_glamor.c',
  'amdgpu_glamor_wrappers.c',
  'amdgpu_kms.c',
  'amdgpu_misc.c',
  'amdgpu_pixmap.c',
  'amdgpu_probe.c',
  'amdgpu_present.c',
  'amdgpu_sync.c',
  'amdgpu_video.c',
  'drmmode_display.c',
]

amdgpu_drv_libs = [
  fontsproto_dep,
  gbm_dep,
  libdrm_amdgpu_dep,
  libdrm_dep,
  randrproto_dep,
  renderproto_dep,
  videoproto_dep,
  xextproto_dep,
  xf86driproto_dep,
  xorg_dep,
  xproto_dep
]
if libudev_dep.found()
  amdgpu_drv_libs += libudev_dep
endif

shared_module(
  'amdgpu_drv',
  srcs,
  include_directories: include_directories('..'),
  dependencies: amdgpu_drv_libs,
  install: true,
  install_dir: join_paths(moduledir, 'drivers'),
  name_prefix: '',
)
