.\" Copyright 1989  The Open Group
.\"
.\" Permission to use, copy, modify, distribute, and sell this software and its
.\" documentation for any purpose is hereby granted without fee, provided that
.\" the above copyright notice appear in all copies and that both that
.\" copyright notice and this permission notice appear in supporting
.\" documentation.
.\"
.\" The above copyright notice and this permission notice shall be included
.\" in all copies or substantial portions of the Software.
.\"
.\" THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
.\" OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
.\" MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
.\" IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
.\" OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
.\" ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
.\" OTHER DEALINGS IN THE SOFTWARE.
.\"
.\" Except as contained in this notice, the name of The Open Group shall
.\" not be used in advertising or otherwise to promote the sale, use or
.\" other dealings in this Software without prior written authorization
.\" from The Open Group.
.\"
.\" Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
.\"
.\" Permission is hereby granted, free of charge, to any person obtaining a
.\" copy of this software and associated documentation files (the "Software"),
.\" to deal in the Software without restriction, including without limitation
.\" the rights to use, copy, modify, merge, publish, distribute, sublicense,
.\" and/or sell copies of the Software, and to permit persons to whom the
.\" Software is furnished to do so, subject to the following conditions:
.\"
.\" The above copyright notice and this permission notice (including the next
.\" paragraph) shall be included in all copies or substantial portions of the
.\" Software.
.\"
.\" THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
.\" IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
.\" FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
.\" THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
.\" LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
.\" FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
.\" DEALINGS IN THE SOFTWARE.
.\"
.\"
.TH XDMSHELL __appmansuffix__ __xorgversion__
.SH NAME
xdmshell \- shell for starting xdm on login
.SH SYNOPSIS
.B BINDIR/xdmshell
.SH DESCRIPTION
.I Xdmshell
can be used on systems that are configured to start a console in text mode by
default, to allow users to start the X display manager by logging in as a
special user with the shell set to the xdmshell program.
.LP
Many servers set the keyboard to do non-blocking I/O under the assumption that
they are the only programs attempting to read from the keyboard.
Unfortunately, some versions of \fIgetty\fP(__adminmansuffix__) will
immediately see a continuous stream of zero-length reads which they interpret
as end-of-file indicators.  Eventually, \fIinit\fP(__adminmansuffix__) will
disable logins on that line until somebody types the following as root:
.RS
\# kill -HUP 1
.RE
.LP
On some platforms, one alternative is to disable logins on the console
and always run \fIxdm\fP(__appmansuffix__) from \fI/etc/inittab\fP.
.LP
Another approach is to set up an account whose shell is the \fIxdmshell\fP
program found in the xdm distribution.  This program is not installed by
default so that site administrators will examine it to see if it meets their
needs.  The \fIxdmshell\fP utility makes sure that it is being run from the
appropriate type of terminal, starts \fIxdm\fP, waits for it to finish, and
then resets the console if necessary.  If the \fIxdm\fP resources file
(specified by the \fIDisplayManager*resources\fP entry in the
\fIxdm-config\fP file) contains a binding to the \fIabort-display\fP action
similar to the following
.RS
xlogin*login.translations: #override  Ctrl<Key>R: abort-display()
.RE
the console can then by restored by pressing the indicated key
(Control-R in the above example) in the \fIxdm\fP login window.
.LP
.ne 10
The \fIxdmshell\fP program is usually
installed setuid to root but executable only by members of a special group,
of which the only member is the account which has \fIxdmshell\fP as its shell:
.RS
.nf
%  grep xdm /etc/passwd
x::101:51:Account for starting up X:/tmp:BINDIR/xdmshell
%  grep 51 /etc/group
xdmgrp:*:51:
%  ls -lg BINDIR/xdmshell
-rws--x---   1 root     xdmgrp     20338 Nov  1 01:32 BINDIR/xdmshell
.fi
.RE
.LP
If the \fIxdm\fP resources have not been configured to have a key bound to
the \fIabort-display()\fP action, there will be no way for general users to
login to the console directly.  Whether or not this is desirable depends on
the particular site.
.SH "SEE ALSO"
.IR X (__miscmansuffix__),
.IR xdm (__appmansuffix__),
.IR xinit (__appmansuffix__)
