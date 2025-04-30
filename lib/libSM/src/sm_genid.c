/*

Copyright (c) 2002, Oracle and/or its affiliates.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice (including the next
paragraph) shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/
/*

Copyright 1993, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/

/*
 * Author: Ralph Mor, X Consortium
 */

#ifdef WIN32
# define _WILLWINSOCK_
#endif
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include <X11/SM/SMlib.h>
#include "SMlibint.h"
#ifdef XTHREADS
# include <X11/Xthreads.h>
#endif
#include <stdio.h>
#include <unistd.h>

#include <time.h>
#define Time_t time_t

#if defined(HAVE_UUID_CREATE)
# include <uuid.h>
#elif defined(HAVE_LIBUUID)
# include <uuid/uuid.h>
#else

# ifndef WIN32

#  ifdef TCPCONN
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#   define XOS_USE_NO_LOCKING
#   define X_INCLUDE_NETDB_H
#   include <X11/Xos_r.h>
#  endif

# else /* WIN32 */

#  include <X11/Xwinsock.h>
#  include <X11/Xw32defs.h>
#  define X_INCLUDE_NETDB_H
#  define XOS_USE_MTSAFE_NETDBAPI
#  include <X11/Xos_r.h>

# endif /* WIN32 */

# if defined(IPv6) && !defined(AF_INET6)
#  error "Cannot build IPv6 support without AF_INET6"
# endif

#endif /* !(HAVE_UUID_CREATE || HAVE_LIBUUID) */


char *
SmsGenerateClientID(SmsConn smsConn)
{
#if defined(HAVE_UUID_CREATE)
    char *id;
    char *temp;
    uuid_t uuid;
    uint32_t status;
    size_t len;

    uuid_create(&uuid, &status);

    uuid_to_string(&uuid, &temp, &status);

    len = strlen(temp) + 2;
    if ((id = malloc(len)) != NULL)
        snprintf(id, len, "2%s", temp);

    free(temp);

    return id;
#elif defined(HAVE_LIBUUID)
    char *id;
    char temp[256];
    uuid_t uuid;

    uuid_generate(uuid);

    temp[0] = '2';
    temp[1] = '\0';
    uuid_unparse_lower(uuid, &temp[1]);

    id = strdup (temp);

    return id;
#elif defined(TCPCONN)
    static const char hex[] = "0123456789abcdef";
    char hostname[256];
    char address[64], *addr_ptr = address;
    char temp[256];
    char *id;
    static int sequence = 0;
    char* inet_addr;
    char *ptr1;
    unsigned char decimal[4];
    int i;
    struct in_addr *haddr = NULL;
# ifdef HAVE_GETADDRINFO
    struct addrinfo *ai, *first_ai;
# endif

    if (gethostname (hostname, sizeof (hostname)))
	return (NULL);

# ifdef HAVE_GETADDRINFO
    if (getaddrinfo(hostname,NULL,NULL,&ai) != 0)
	return NULL;

    for (first_ai = ai; ai != NULL; ai = ai->ai_next) {
	if (ai->ai_family == AF_INET)
	    break;
#  ifdef IPv6
	if (ai->ai_family == AF_INET6)
	    break;
#  endif
    }
    if (ai == NULL) {
	freeaddrinfo(first_ai);
	return NULL;
    }

#  ifdef IPv6
    if (ai->ai_family == AF_INET6) {
	unsigned char *cp = (unsigned char *) &((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr.s6_addr;

	*addr_ptr++ = '6';	/* IPv6 address code */

	for (i = 0 ; i < 16 ; i++) {
            *addr_ptr++ = hex[cp[i] >> 4];
            *addr_ptr++ = hex[cp[i] & 0x0f];
	}

        *addr_ptr++ = '\0';
    } else  /* Fall through to IPv4 address handling */
#  endif /* IPv6 */
    {
	haddr = &((struct sockaddr_in *)ai->ai_addr)->sin_addr;
# else /* !HAVE_GETADDRINFO */
#  ifdef XTHREADS_NEEDS_BYNAMEPARAMS
	_Xgethostbynameparams hparams;
#  endif
	struct hostent *hostp;

	if ((hostp = _XGethostbyname (hostname,hparams)) != NULL)
	    haddr = (struct in_addr *)(hostp->h_addr);
	else
	    return NULL;
# endif /* !HAVE_GETADDRINFO */

	*addr_ptr++ = '1';

	{
	    unsigned char *cp = (unsigned char *) &(haddr->s_addr);

	    for (i = 0; i < 4; i++) {
		*addr_ptr++ = hex[cp[i] >> 4];
		*addr_ptr++ = hex[cp[i] & 0x0f];
	    }
	}

	*addr_ptr++ = '\0';

# ifdef HAVE_GETADDRINFO
    }
    freeaddrinfo(first_ai);
# endif

    sprintf (temp, "1%s%.13ld%.10ld%.4d", address, (long)time((Time_t*)0),
	     (long)getpid(), sequence);

    if (++sequence > 9999)
	sequence = 0;

    id = strdup (temp);

    return (id);
#else /* no supported id generation method, neither UUID nor TCPCONN */
    return (NULL);
#endif
}
