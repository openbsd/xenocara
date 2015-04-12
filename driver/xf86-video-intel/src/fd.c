/***************************************************************************

 Copyright 2013 Intel Corporation.  All Rights Reserved.

 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sub license, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice (including the
 next paragraph) shall be included in all copies or substantial portions
 of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 IN NO EVENT SHALL INTEL, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fcntl.h>
#include <unistd.h>

#include <misc.h> /* MAXCLIENTS */

#include "fd.h"

int fd_move_cloexec(int fd)
{
	int newfd;

	newfd = fcntl(fd,
#ifdef F_DUPFD_CLOEXEC
		      F_DUPFD_CLOEXEC,
#else
		      F_DUPFD,
#endif
		      MAXCLIENTS);
	if (newfd < 0)
		return fd;

#ifndef F_DUPFD_CLOEXEC
	newfd = fd_set_cloexec(newfd);
#endif

	close(fd);
	return newfd;
}

int fd_set_cloexec(int fd)
{
	int flags;

	if (fd == -1)
		return fd;

#ifdef FD_CLOEXEC
	flags = fcntl(fd, F_GETFD);
	if (flags != -1) {
		flags |= FD_CLOEXEC;
		fcntl(fd, F_SETFD, flags);
	}
#endif

	return fd;
}

int fd_set_nonblock(int fd)
{
	int flags;

	if (fd == -1)
		return fd;

	flags = fcntl(fd, F_GETFL);
	if (flags != -1) {
		flags |= O_NONBLOCK;
		fcntl(fd, F_SETFL, flags);
	}

	return fd;
}

