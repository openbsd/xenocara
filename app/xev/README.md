xev creates a window and then asks the X server to send it X11 events
whenever anything happens to the window (such as it being moved,
resized, typed in, clicked in, etc.).  You can also attach it to an
existing window.  It is useful for seeing what causes events to occur
and to display the information that they contain; it is essentially a
debugging and development tool, and should not be needed in normal
usage.

All questions regarding this software should be directed at the
Xorg mailing list:

  https://lists.x.org/mailman/listinfo/xorg

The master development code repository can be found at:

  https://gitlab.freedesktop.org/xorg/app/xev

Please submit bug reports and requests to merge patches there.

For patch submission instructions, see:

  https://www.x.org/wiki/Development/Documentation/SubmittingPatches

