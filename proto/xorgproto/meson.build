# Copyright © 2018 Intel Corporation

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

project('xorgproto', 'c', license : 'MIT', version : '2019.2')
pkg = import('pkgconfig')
cc = meson.get_compiler('c')
sed = find_program('sed')

pcs = [
        ['applewmproto',        '1.4.2'],
        ['bigreqsproto',        '1.1.2'],
        ['compositeproto',      '0.4.2'],
        ['damageproto',         '1.2.1'],
        ['dmxproto',            '2.3.1'],
        ['dri2proto',           '2.8'],
        ['dri3proto',           '1.2'],
        ['fixesproto',          '5.0'],
        ['fontsproto',          '2.1.3'],
        ['glproto',             '1.4.17'],
        ['inputproto',          '2.3.2'],
        ['kbproto',             '1.0.7'],
        ['presentproto',        '1.2'],
        ['randrproto',          '1.6.0'],
        ['recordproto',         '1.14.2'],
        ['renderproto',         '0.11.1'],
        ['resourceproto',       '1.2.0'],
        ['scrnsaverproto',      '1.2.2'],
        ['trapproto',           '3.4.3'],
        ['videoproto',          '2.3.3'],
        ['xcmiscproto',         '1.2.2'],
        ['xextproto',           '7.3.0'],
        ['xf86bigfontproto',    '1.2.0'],
        ['xf86dgaproto',        '2.1'],
        ['xf86driproto',        '2.1.1'],
        ['xf86miscproto',       '0.9.3'],
        ['xf86vidmodeproto',    '2.3.1'],
        ['xineramaproto',       '1.2.1'],
        ['xproto',              '7.0.32'],
        ['xproxymngproto',      '1.0.3'],
]

foreach pc : pcs
    if pc[0] == 'xf86driproto'
        subdir = 'X11/dri'
    else
        subdir = ''
    endif
    pkg.generate(
        name : pc[0],
        filebase : pc[0],
        description : pc[0] + ' headers',
        version : pc[1],
        install_dir : get_option('datadir') + '/pkgconfig',
        subdirs : subdir,
    )
endforeach

if get_option('legacy') == true
    legacy_pcs = [
        ['evieproto', '1.1.1'],
        ['fontcacheproto', '0.1.3'],
        ['lg3dproto', '5.0'],
        ['printproto', '1.0.5'],
        ['windowswmproto', '1.0.4'],
        ['xcalibrateproto', '0.1.0'],
        ['xf86rushproto', '1.2.2'],
    ]
    foreach pc : legacy_pcs
        pkg.generate(
            name : pc[0],
            filebase : pc[0],
            description : pc[0] + ' headers',
            version : pc[1],
            install_dir : get_option('datadir') + '/pkgconfig'
        )
    endforeach
endif

ext_xorgproto = declare_dependency(
    include_directories : include_directories('include')
)

subdir('include')
