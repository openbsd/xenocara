project('encodings',
        version: '1.1.0',
        meson_version: '>= 0.58.0',
        license: 'MIT'
)

mkfontscale = find_program('mkfontscale', required: true, native: true)

if get_option('gzip-small-encodings') or get_option('gzip-large-encodings')
    gzip = find_program('gzip', required: true, native: true)
endif

encodingsdir = get_option('encodingsdir')
if encodingsdir == ''
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
  encodingsdir = fontrootdir / 'encodings'
endif

small_encoding_files = [
  'adobe-dingbats.enc',
  'adobe-standard.enc',
  'adobe-symbol.enc',
  'armscii-8.enc',
  'ascii-0.enc',
  'dec-special.enc',
  'ibm-cp437.enc',
  'ibm-cp850.enc',
  'ibm-cp852.enc',
  'ibm-cp866.enc',
  'iso8859-11.enc',
  'iso8859-13.enc',
  'iso8859-14.enc',
  'iso8859-16.enc',
  'iso8859-6.16.enc',
  'iso8859-6.8x.enc',
  'microsoft-cp1250.enc',
  'microsoft-cp1251.enc',
  'microsoft-cp1252.enc',
  'microsoft-cp1253.enc',
  'microsoft-cp1254.enc',
  'microsoft-cp1255.enc',
  'microsoft-cp1256.enc',
  'microsoft-cp1257.enc',
  'microsoft-cp1258.enc',
  'microsoft-win3.1.enc',
  'mulearabic-0.enc',
  'mulearabic-1.enc',
  'mulearabic-2.enc',
  'mulelao-1.enc',
  'suneu-greek.enc',
  'tcvn-0.enc',
  'tis620-2.enc',
  'viscii1.1-1.enc'
]

if get_option('gzip-small-encodings')
  foreach small_encoding_input : small_encoding_files
    small_encoding_output = small_encoding_input + '.gz'
    custom_target(small_encoding_output,
                  input: small_encoding_input,
                  output: small_encoding_output,
                  command: [gzip, '-c', '-n', '@INPUT@'],
                  capture: true, # capture output from stdout
                  install: true,
                  install_dir: encodingsdir)
  endforeach
else
  install_data(small_encoding_files, install_dir: encodingsdir)
endif

subdir('large')

conf_data = configuration_data({
  'mkfontscale': mkfontscale.full_path(),
  'encodingsdir': encodingsdir
})
mkencodingsdir = configure_file(input : 'mkencodingsdir.in',
                                output : 'mkencodingsdir',
                                configuration : conf_data)
meson.add_install_script(mkencodingsdir)

summary({'encodingsdir': encodingsdir,
         'gzip-small-encodings': get_option('gzip-small-encodings'),
         'gzip-large-encodings':  get_option('gzip-large-encodings'),
         'mkfontscale': mkfontscale})
