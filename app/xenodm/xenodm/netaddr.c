/*

Copyright 1991, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from The Open Group.

*/

/*
 * xdm - X display manager
 *
 * netaddr.c - Interpretation of XdmcpNetaddr object.
 */

#include "dm.h"
#include "dm_error.h"

#include <X11/X.h>		/* FamilyInternet, etc. */

# include "dm_socket.h"

# ifdef UNIXCONN
#  include <sys/un.h>		/* struct sockaddr_un */
# endif

/* given an XdmcpNetaddr, returns the socket protocol family used,
   e.g., AF_INET */

int NetaddrFamily(XdmcpNetaddr netaddrp)
{
    return ((struct sockaddr *)netaddrp)->sa_family;
}

/* given an XdmcpNetaddr, returns a pointer to the network address
   and sets *lenp to the length of the address */

char * NetaddrAddress(XdmcpNetaddr netaddrp, int *lenp)
{
    switch (NetaddrFamily(netaddrp)) {
# ifdef UNIXCONN
    case AF_UNIX:
	*lenp = strlen(((struct sockaddr_un *)netaddrp)->sun_path);
        return (char *) (((struct sockaddr_un *)netaddrp)->sun_path);
# endif
# ifdef TCPCONN
    case AF_INET:
        *lenp = sizeof (struct in_addr);
        return (char *) &(((struct sockaddr_in *)netaddrp)->sin_addr);
    case AF_INET6:
    {
	struct in6_addr *a = &(((struct sockaddr_in6 *)netaddrp)->sin6_addr);
	if (IN6_IS_ADDR_V4MAPPED(a)) {
	    *lenp = sizeof (struct in_addr);
	    return ((char *) &(a->s6_addr))+12;
	} else {
	    *lenp = sizeof (struct in6_addr);
	    return (char *) &(a->s6_addr);
	}
    }
# endif
    default:
	*lenp = 0;
	return NULL;
    }
}


/* given an XdmcpNetaddr, sets *addr to the network address used and
   sets *len to the number of bytes in addr.
   Returns the X protocol family used, e.g., FamilyInternet */

int ConvertAddr (XdmcpNetaddr saddr, int *len, char **addr)
{
    int retval;

    if ((len == NULL) || (saddr == NULL))
        return -1;
    *addr = NetaddrAddress(saddr, len);
    switch (NetaddrFamily(saddr))
    {
      case AF_UNSPEC:
	retval = FamilyLocal;
	break;
      case AF_UNIX:
        retval = FamilyLocal;
	break;
# ifdef TCPCONN
      case AF_INET:
        retval = FamilyInternet;
	break;
      case AF_INET6:
	if (*len == sizeof(struct in_addr))
	    retval = FamilyInternet;
	else
	    retval = FamilyInternet6;
	break;
# endif
      default:
	retval = -1;
        break;
    }
    Debug ("ConvertAddr returning %d for family %d\n", retval,
	   NetaddrFamily(saddr));
    return retval;
}
