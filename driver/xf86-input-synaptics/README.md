Synaptics touchpad driver for X.Org
-----------------------------------

FAQ
---

* Is this free software?

    Yes, the source code is released under the MIT license.

* When will the driver be included in the XOrg distribution?

    It is already.

* How do I use this driver with Linux kernel 2.6.x?

    You need to have the "evdev" driver loaded or compiled into the
    kernel (CONFIG_INPUT_EVDEV). Set the "Protocol" parameter in the X
    configuration file to "auto-dev". Also, if you set the "Device"
    parameter to "/dev/psaux", the same X configuration file should
    work for a 2.4.x kernel.

    When configuring the kernel, enable PS/2 mouse support
    (CONFIG_MOUSE_PS2).

* It still doesn't work with a 2.6 kernel.

    Some distributions come with an incomplete /dev directory. The
    driver needs the /dev/input/eventX device nodes. Try to create
    them manually if they don't exist already. (Look at
    /proc/bus/input/devices to figure out how many nodes you need.)

	     # mknod /dev/input/event0 c 13 64
	     # mknod /dev/input/event1 c 13 65
	     # mknod /dev/input/event2 c 13 66
	     ...

* How can I configure tap-to-click behavior?

    If you set MaxTapTime=0 in the X config file then the touchpad
    will not use tapping at all, i.e. touching/tapping will not be
    taken as a mouse click.

    If, instead, you set MaxTapMove=0 in the X config file, then the
    touchpad will not use tapping for a single finger tap (left mouse
    button click) but will for the two and three finger tap (middle
    and right button click).

* Why did tap-to-click stop working after I upgraded from an old version?

    Time is now measured in milliseconds instead of "number of
    packets". In practice, this means that if you are upgrading from
    an old version, you need to change MaxTapTime and
    EmulateMidButtonTime to make "tap to click" work. Good values are
    180 and 75 respectively.

* Gnome scrollbars scroll too much when using tap-to-click. Why?

    The ClickTime parameter is probably too big. Try setting it to
    100. Gnome scrollbars use auto repeat, ie if you press the left
    mouse button and keep it pressed, the scroll bar will move until
    you release the button. This will lead to problems if the tap time
    is longer than the delay before auto repeat starts.

* Vertical and horizontal scrolling events are mixed up. How come?

    Probably because some X startup/login script uses xmodmap to remap
    the mouse buttons. Correct settings for the touchpad are:

	xmodmap -e 'pointer = 1 2 3 4 5 6 7'

    You can check the current settings by running:

	xmodmap -pp

* Horizontal scrolling doesn't work in some programs. Is it a driver
  bug?

    No, probably not. Support for horizontal scroll events must be
    handled by the application programs. Not all programs do that
    yet. Ask the authors of the application in question to implement
    support for horizontal scroll events.

    You can use the "xev" program to check if the synaptics driver
    generates the horizontal scroll events.

    If you are having problems with Mozilla, try this link:

  https://lists.debian.org/debian-laptop/2004/08/msg00167.html

* Can the driver be used together with gpm?

    No, not reliably, if you are using a 2.4.x kernel. The gpm driver
    and the X driver both try to read data from the touchpad, and if
    they try to read at the same time, both drivers see incomplete
    data and don't know how to interpret it.

    If you are running a 2.6.x kernel though, there should be no
    conflict, because the kernel driver will make sure both user space
    drivers receive all events from the touchpad.

* Can I use this driver with an ALPS Glidepoint device?

    Yes, see the README.alps file for more information.

* The driver says "reset failed" and the touchpad doesn't work. What
  can I do?

    This problem has been reported for some Compaq models. It's
    currently not known why it happens, but removing the reset command
    from the driver appears to make it work. If you use a 2.4 linux
    kernel, replace the contents of the ps2_synaptics_reset() function
    in ps2comm.c with a "return TRUE;" statement. If you use a 2.6
    linux kernel, remove the while loop in synaptics_query_hardware()
    in the file drivers/input/mouse/synaptics.c in the linux kernel
    source code.


Authors
-------

Many people have contributed to this driver. Look at the top of
synaptics.c and ps2comm.c for details.

The current maintainer is X.org development team <xorg-devel@lists.x.org>.


Contacts
--------
All questions regarding this software should be directed at the
Xorg mailing list:

  https://lists.x.org/mailman/listinfo/xorg

The primary development code repository can be found at:

  https://gitlab.freedesktop.org/xorg/driver/xf86-input-synaptics

Please submit bug reports and requests to merge patches there.

For patch submission instructions, see:

  https://www.x.org/wiki/Development/Documentation/SubmittingPatches

