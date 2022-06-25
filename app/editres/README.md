Editres is a tool that allows users and application developers to view
the full widget hierarchy of any Xt Toolkit application that speaks the
Editres protocol.  In addition, editres will help the user construct
resource specifications, allow the user to apply the resource to
the application and view the results dynamically.  Once the user is
happy with a resource specification editres will append the resource
string to the user's X Resources file.

For more information on editres usage, see:

  http://www.rahul.net/kenton/editres.html

Editres depends on the _EresInsert<something> and
_EresRetrieve<something> routines that are are implemented and used by
"EditresCom.c".  Since this module is intended to be included in your
widget library or toolkit, editres should automatically link in the
module, but if you have not included that module into your library you
will need to edit the makefile to make sure that editres links in this
file.

  ------------------------------------------------------------

All questions regarding this software should be directed at the
Xorg mailing list:

  https://lists.x.org/mailman/listinfo/xorg

The primary development code repository can be found at:

  https://gitlab.freedesktop.org/xorg/app/editres

Please submit bug reports and requests to merge patches there.

For patch submission instructions, see:

  https://www.x.org/wiki/Development/Documentation/SubmittingPatches

