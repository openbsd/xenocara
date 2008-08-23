xf86Fpit.c
(and associated files).

Last updated Jan 2007
This information applies to version 1.1.0 of this driver.

Supported Hardware:


	Fujitsu Stylistic LT	(Reported to work, but I have reason to belive that it does NOT.)
	Fujistu Stylistic 500   (Should Work)
	Fujistu Stylistic 1000  (Should Work)
	Fujistu Stylistic 1200  (Should Work)
	Fujistu Stylistic 2300  (Should Work)
	Fujitsu Stylistic 3400  (and possibly other passive-pen systems)
	FinePoint MP800

History and Contributors:

-  Steven Lang <tiger@tyger.org> wrote a Xinput extension for the AceCad Drawing Tablet.
-  This was modified originaly by Rob Tsuk and John Apfelbaum (http://linuxslate.org/) to
   produce a working version for the Stylistic 500 and 1000 using XFree86 3.3.6.
   (This can be obtained from http://linuxslate.com)
-  Richard Miller-Smith <Richard.Miller-Smith@philips.com> Merged the code from the above
   project into the XFree86 4.0.2 Elographics driver by Patrick Lecoanet.
-  John Apfelbaum continuted the work to produce a working XFree86 4.0.x driver for the
   Stylistic 1200.  
-  David Clay added support for Stylistic 3400 passive pen, and possibly
   others. (Also fixed processing of all packets, and enabled right mouse button.)

Please visit http://webcvs.freedesktop.org/xorg/driver/xf86-input-fpit/ for the
latest version of this driver.

License:

Please visit http://XFree86.org for license information. 

 * Copyright 1995, 1999 by Patrick Lecoanet, France. <lecoanet@cena.dgac.fr>
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Patrick Lecoanet not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  Patrick Lecoanet makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.

Installation:

1.  Install and configure Linux w/o consideration of the digitizer tablet.
2.  Get X working the way you want it. 
3.  Set up a serial device to be initialized at startup.
    Modify /etc/serial.conf if your distribution uses it. Otherwise
    add or modify your appropriate startup scripts to include:

setserial /dev/ttyS3 autoconfig
setserial /dev/ttyS3 port ??? IRQ ?? baud_base 115200 [uart 16450]

The uart setting may be necessary for the device to work.
See below for appropriate port/IRQ values.

4.  Copy fpit_drv.o to /usr/X11R6/lib/modules/input

5.  Add the following to your X config file (XF86Config/XF86Config-4/xorg.conf)

Section "InputDevice"
	Identifier	"mouse0"
	Driver		"fpit"
	Option		"Device"	"/dev/ttyS3"
	# These may need tweaking; see below.
	Option		"BaudRate"	"19200"
	Option		"MaximumXPosition"	"6250"
	Option		"MaximumYPosition"	"4950"
	Option		"MinimumXPosition"	"130"
	Option		"MinimumYPosition"	"0"
	Option		"InvertY"
	# For a passive pen, e.g. Stylistic 3400
	Option		"Passive"
	# To make the touchscreen respond automatically to
	# resolution changes and screen rotation:
	Option		"TrackRandR"
EndSection

6. Remember to add this Input Device to the server description (Near the end of the file.)

7. Start or restart X.

8. If required adjust the baud rate and Min/Max X/Y positions so that the pointer
   tracks the pen correctly.

For Fujitsu Stylistic xx00 models, try IRQ 15, with port either unspecified
 or set to 0xfce8.

For Fujitsu Stylistic 3400 models, try IRQ 4 and port 0xfd68.
 Recommended X config settings are BaudRate 9600, Min X/Y 0, MaxX 4070, MaxY 4020.

In general you may wish to consult /proc/ioports or /sys/devices/pnp0/*
(under Linux 2.6) for serial information. BaudRate should generally be 9600,
19200, or 38400.


Hints if you are having problems (Thanks to Aron Hsiao):

Problem 1:  Side switch being reported as wild button numbers
	   (like 249 instead of 3): 

Solution:  Add the following to your xinitrc: 

	xsetpointer TOUCHSCREEN
	xmodmap -e 'pointer = 1 2 3'

This should be re-stating the defaults, but Aron Hsiao agrees that it appears
to be an XFree86 4.x bug. 

Problem 2:  X Server crash during GUI startup (Particularly Gnome). 
            Or: programs using high-resolution pen tracking via XInput
            don't work.

Solution:   You must have a regular mouse defined as the default pointer
	    even if no mouse is used.  During startup, Gnome attempts to
	    set mouse acceleration for the default pointer.  Since the
	    pendrivers are absolute pointers, and acceleration is meaningless,
	    they do not take well to attempts to set it :-)

	    Additionally, setting the tablet as a core pointer prevents X
            from sending XInput events. This lowers the pen-tracking
            resolution available to programs by 1 or 2 orders of magnitude!

Problem 3:  Jittery cursor and undesired mouse clicks (both buttons),
            particuarly on the Stylistic 1200, and particuarly after the
	    system has warmed up.
	    
Solution:   (Not really a solution) This is a hardware problem.  Some
	    people have reported good results by modifying the CPU heatsink
	    on the Stylistic 1200.  Putting the system to sleep when not
	    actively using it extends battery life, and keeps the system from
	    getting too hot.

            If cursor movement seems completely random and mouse clicks seem
            inexplicable, your baud rate may be set incorrectly.

Bugs and Needed Work:

(See above)

Adjusting the constants in the XF86Config(-4) is teedious and requires
multiple restarts of the X Window system.  -- Somebody PLEASE write a
calibration program !





/* $XFree86: xc/programs/Xserver/hw/xfree86/input/fpit/readme.txt,v 1.2 2002/11/22 03:37:37 dawes Exp $ */
