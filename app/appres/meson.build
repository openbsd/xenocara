project('appres', 'c',
  version : '1.0.7',
  license : 'MIT',
  default_options: ['warning_level=3'],
)

cc = meson.get_compiler('c')

prefix = get_option('prefix')

package_string = '@0@ @1@'.format(meson.project_name(), meson.project_version())

conf_data = configuration_data()
conf_data.set('PACKAGE_STRING', '"@0@"'.format(package_string))
configure_file(output : 'config.h', configuration : conf_data)
add_global_arguments('-DHAVE_CONFIG_H', language : 'c')

x11_dep = dependency('x11', required: true)
xmuu_dep = dependency('xt', required: true)
xproto_dep = dependency('xproto', required: true, version: '>= 7.0.17')

sources = ['appres.c']
executable('appres', sources,
  dependencies: [x11_dep, xmuu_dep, xproto_dep],
  install: true)

man = join_paths(prefix, get_option('mandir'))

man_conf = configuration_data()
man_conf.set('PACKAGE_STRING', package_string)
man_conf.set('APP_MAN_SUFFIX', '1')
man_conf.set('MISC_MAN_SUFFIX', '7')
man_conf.set('XORG_MAN_PAGE', 'X Version 11')

install_man(configure_file(
  input: 'man/appres.man',
  output: 'appres.1',
  install_dir: join_paths(man, 'man1'),
  configuration: man_conf
))
