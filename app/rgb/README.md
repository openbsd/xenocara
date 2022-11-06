X color name database
---------------------

This package includes both the list mapping X color names to RGB values
(rgb.txt) and programs to read and write the color database.

The "others" subdirectory contains some alternate color databases.

The configure script accepts --with-rgb-db-type=(text|dbm|ndbm) to specify
the type of database to use for the color name data. The default is "text".

The configure script accepts --with-rgb-db-library=<library-name> to specify
the dbm or ndbm compatible library to use for the color name data. The default
is "auto" to search for known implementations.  Library names should be
specified as would be provided for the "-l" flag for the linker - for
instance, "c" for libc, "dbm" for libdbm, etc.

Distributors who do choose to enable the use of a database library are
responsible for understanding the requirements of the license terms of
the library they link with, which may impose additional restrictions
beyond the MIT/X11 license used for this package.

If the dbm or ndbm format are chosen, this package also builds the
rgb command to build the database in that format from rgb.txt.

Regardless of format, this package builds the showrgb command to show
the contents of the database, from whichever of dbm, ndbm, or text file
was chosen by the --with-rgb-db-type option when building.

All questions regarding this software should be directed at the
Xorg mailing list:

  https://lists.x.org/mailman/listinfo/xorg

The master development code repository can be found at:

  https://gitlab.freedesktop.org/xorg/app/rgb

Please submit bug reports and requests to merge patches there.

For patch submission instructions, see:

  https://www.x.org/wiki/Development/Documentation/SubmittingPatches

