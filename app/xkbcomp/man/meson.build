man_substs = configuration_data()

man_substs.set('xorgversion', f'"xkbcomp @xkbcomp_version@" "X Version 11"')
man_substs.set('xkbconfigroot', XKBCONFIGROOT)
man_substs.set('miscmansuffix', 7)
dir_man1 = join_paths(get_option('prefix'), get_option('mandir'), 'man1')

xkbcomp_man = configure_file(
    input: 'xkbcomp.man',
    output: 'xkbcomp.1',
    configuration: man_substs,
    install: true,
    install_dir: dir_man1
)
