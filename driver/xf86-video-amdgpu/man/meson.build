driver_name = 'amdgpu'
driver_man_suffix = '4'
input_name = '@0@.man'.format(driver_name)
output_name = '@0@.@1@'.format(driver_name, driver_man_suffix)
driverman_pre = '@0@.man'.format(driver_name)

man_data = configuration_data()
man_data.set('drivermansuffix', driver_man_suffix)
man_data.set('vendorversion', '@0@ @1@'.format(meson.project_name(), meson.project_version()))

configure_file(
  input: input_name,
  output: output_name,
  configuration: man_data,
  install: true,
  install_dir: join_paths(get_option('mandir'), 'man' + driver_man_suffix),
)
