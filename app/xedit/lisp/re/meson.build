re_test = executable(
  'tests',
  'tests.c',
  c_args: ['-D_BSD_SOURCE'],
  include_directories: include_directories('.'),
  install: false,
  link_with: [libre],
)

test(
  'lisp/re',
  re_test,
  workdir: meson.current_source_dir(),
)
