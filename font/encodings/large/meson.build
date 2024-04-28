largeencodingsdir = encodingsdir / 'large'

large_encoding_files = [
  'big5.eten-0.enc',
  'big5hkscs-0.enc',
  'cns11643-1.enc',
  'cns11643-2.enc',
  'cns11643-3.enc',
  'gb18030-0.enc',
  'gb18030.2000-0.enc',
  'gb18030.2000-1.enc',
  'gb2312.1980-0.enc',
  'gbk-0.enc',
  'jisx0201.1976-0.enc',
  'jisx0208.1990-0.enc',
  'jisx0212.1990-0.enc',
  'ksc5601.1987-0.enc',
  'ksc5601.1992-3.enc',
  'sun.unicode.india-0.enc'
]

if get_option('gzip-large-encodings')
  foreach large_encoding_input : large_encoding_files
    large_encoding_output = large_encoding_input + '.gz'
    custom_target(large_encoding_output,
                  input: large_encoding_input,
                  output: large_encoding_output,
                  command: [gzip, '-c', '-n', '@INPUT@'],
                  capture: true, # capture output from stdout
                  install: true,
                  install_dir: largeencodingsdir)
  endforeach
else
  install_data(large_encoding_files, install_dir: largeencodingsdir)
endif

