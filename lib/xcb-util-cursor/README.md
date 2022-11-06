About XCB util modules
======================

The XCB util modules provides a number of libraries which sit on top
of libxcb, the core X protocol library, and some of the extension
libraries. These experimental libraries provide convenience functions
and interfaces which make the raw X protocol more usable. Some of the
libraries also provide client-side code which is not strictly part of
the X protocol but which have traditionally been provided by Xlib.

If you find any of these libraries useful, please let us know what
you're using and why you aren't in a mental hospital yet. We'd welcome
patches/suggestions for enhancement and new libraries; Please report any
issues you find to the freedesktop.org bug tracker, at:

  https://gitlab.freedesktop.org/xorg/lib/libxcb-cursor/issues

Discussion about XCB occurs on the XCB mailing list:

  https://lists.freedesktop.org/mailman/listinfo/xcb

About XCB util-cursor module
============================

XCB util-cursor module provides the following libraries:

  - cursor: port of libxcursor

You can obtain the latest development versions of XCB util-cursor using
GIT from https://gitlab.freedesktop.org/xorg/lib/libxcb-cursor

  For anonymous checkouts, use:

    git clone --recursive https://gitlab.freedesktop.org/xorg/lib/libxcb-cursor.git

  For developers, use:

    git clone --recursive git@gitlab.freedesktop.org:xorg/lib/libxcb-cursor.git
