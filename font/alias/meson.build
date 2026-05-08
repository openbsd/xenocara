project('font-alias',
        version: '1.0.6',
        meson_version: '>= 0.64.0',
        license: 'MIT'
)

fontrootdir = get_option('fontrootdir')
if fontrootdir == ''
  fontutil = dependency('fontutil', required: false, version: '>= 1.1.0')
  if fontutil.found()
    fontrootdir = fontutil.get_variable('fontrootdir')
  endif
  if fontrootdir == ''
    fontrootdir = get_option('datadir') / 'fonts' / 'X11'
  endif
endif

alias_files = [
  '100dpi/fonts.alias',
  '75dpi/fonts.alias',
  'cyrillic/fonts.alias',
  'misc/fonts.alias'
]
install_data(alias_files, install_dir: fontrootdir, preserve_path: true)

summary({'fontrootdir': fontrootdir})
