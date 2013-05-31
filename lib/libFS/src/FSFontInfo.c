/*
 * Copyright 1990 Network Computing Devices;
 * Portions Copyright 1987 by Digital Equipment Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation, and that the names of Network Computing
 * Devices or Digital not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission. Network Computing Devices or Digital make no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * NETWORK COMPUTING DEVICES AND  DIGITAL DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES
 * OR DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/*

Copyright 1987, 1998  The Open Group

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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include	"FSlibint.h"

char      **
FSListFontsWithXInfo(
    FSServer		  *svr,
    const char		  *pattern,
    int			   maxNames,
    int			  *count,
    FSXFontInfoHeader	***info,
    FSPropInfo		***pprops,
    FSPropOffset	***offsets,
    unsigned char	***prop_data)
{
    long        nbytes;
    int         i,
                j;
    size_t      size = 0;
    FSXFontInfoHeader **fhdr = (FSXFontInfoHeader **) NULL;
    FSPropInfo **pi = (FSPropInfo **) NULL;
    FSPropOffset **po = (FSPropOffset **) NULL;
    unsigned char **pd = (unsigned char **) NULL;
    char      **flist = NULL;
    fsListFontsWithXInfoReply reply;
    fsListFontsWithXInfoReq *req;
    fsPropInfo local_pi;
    fsPropOffset local_po;
    Status status;
    Bool eat_data = True;

    GetReq(ListFontsWithXInfo, req);
    req->maxNames = maxNames;
    nbytes = req->nbytes = pattern ? strlen(pattern) : 0;
    req->length += (nbytes + 3) >> 2;
    _FSSend(svr, pattern, nbytes);

    for (i = 0;; i++) {
	if (FSProtocolVersion(svr) > 1)
	{
	    status = _FSReply(svr, (fsReply *) &reply, 0, fsFalse);
	    if (status != 0  &&  reply.nameLength == 0)	/* got last reply */
		break;
	    if (status)
		_FSRead(svr, ((char *) &reply) + SIZEOF(fsGenericReply),
			SIZEOF(fsListFontsWithXInfoReply) -
			SIZEOF(fsGenericReply));
	} else {
	    status = _FSReply(svr, (fsReply *) & reply,
			      ((SIZEOF(fsListFontsWithXInfoReply) -
				SIZEOF(fsGenericReply)) >> 2), fsFalse);
	}
	if (!status) {
	    eat_data = False;
	    goto badmem;
	}
	if (reply.nameLength == 0)	/* got last reply in version 1 */
	    break;
	if ((i + reply.nReplies) >= size) {

	    if (reply.nReplies > SIZE_MAX - i - 1)
		goto badmem;
	    size = i + reply.nReplies + 1;

	    if (size > SIZE_MAX / sizeof(char *))
		goto badmem;

	    if (fhdr) {
#define ResizeArray(var, type) { \
		    type **tmp = FSrealloc(var, sizeof(type *) * size); \
		    if (tmp)						\
			var = tmp;					\
		    else						\
			goto badmem;					\
		}

		ResizeArray(fhdr, FSXFontInfoHeader)
		ResizeArray(flist, char)
		ResizeArray(pi, FSPropInfo)
		ResizeArray(po, FSPropOffset)
		ResizeArray(pd, unsigned char)
	    } else {
#define InitArray(var, type) \
		if ((var = FSmalloc(sizeof(type *) * size)) == NULL) {	\
		    goto badmem;					\
		}

		InitArray(fhdr, FSXFontInfoHeader)
		InitArray(flist, char)
		InitArray(pi, FSPropInfo)
		InitArray(po, FSPropOffset)
		InitArray(pd, unsigned char)
	    }
	}
	fhdr[i] = FSmalloc(sizeof(FSXFontInfoHeader));
	if (!fhdr[i]) {
	    goto badmem;
	}
	FSUnpack_XFontInfoHeader(&reply, fhdr[i], FSProtocolVersion(svr));

	/* alloc space for the name */
	flist[i] = FSmalloc(reply.nameLength + 1);
	if (!flist[i])
	    goto cleanfhdr;
	if (FSProtocolVersion(svr) == 1)
	{
	    /* get the name */
	    _FSReadPad(svr, flist[i], (long) reply.nameLength);
	    flist[i][reply.nameLength] = '\0';
	}

	pi[i] = FSmalloc(sizeof(FSPropInfo));
	if (!pi[i])
	    goto cleanflist;
	_FSReadPad(svr, (char *) &local_pi, SIZEOF(fsPropInfo));
	pi[i]->num_offsets = local_pi.num_offsets;
	pi[i]->data_len = local_pi.data_len;

#if SIZE_MAX <= UINT_MAX
	if (pi[i]->num_offsets > SIZE_MAX / sizeof(FSPropOffset))
	    goto cleanpi;
#endif

	po[i] = FSmalloc(pi[i]->num_offsets * sizeof(FSPropOffset));
	if (!po[i])
	    goto cleanpi;
	pd[i] = FSmalloc(pi[i]->data_len);
	if (!pd[i])
	    goto cleanpo;
	/* get offsets */
	for (j=0; j<pi[i]->num_offsets; j++)
	{
	    _FSReadPad(svr, (char *) &local_po, SIZEOF(fsPropOffset));
	    po[i][j].name.position = local_po.name.position;
	    po[i][j].name.length = local_po.name.length;
	    po[i][j].value.position = local_po.value.position;
	    po[i][j].value.length = local_po.value.length;
	    po[i][j].type = local_po.type;
	}

	/* get prop data */
	if (FSProtocolVersion(svr) == 1)
	    _FSReadPad(svr, (char *) pd[i], pi[i]->data_len);
	else
	    _FSRead(svr, (char *) pd[i], pi[i]->data_len);

	if (FSProtocolVersion(svr) != 1)
	{
	    /* get the name */
	    _FSRead(svr, flist[i], (long) reply.nameLength);
	    flist[i][reply.nameLength] = '\0';

	    nbytes = pi[i]->data_len + reply.nameLength;
	    _FSEatData(svr, (unsigned long) (((nbytes+3)&~3) - nbytes));
	}
	/* avoid integer overflow */
	if (i > INT_MAX - 1) {
	    goto cleanpd;
	}
    }
    *info = fhdr;
    *count = i;
    *pprops = pi;
    *offsets = po;
    *prop_data = pd;
    SyncHandle();
    return flist;

/* Error cleanup for when we're partway through filling in item #i in arrays */
cleanpd:
    FSfree(pd[i]);
cleanpo:
    FSfree(po[i]);
cleanpi:
    FSfree(pi[i]);
cleanflist:
    FSfree(flist[i]);
cleanfhdr:
    FSfree(fhdr[i]);
/* Error cleanup for all previously filled in items in the arrays */
badmem:
    for (j = (i - 1); j >= 0; j--) {
	FSfree(pi[j]);
	FSfree(po[j]);
	FSfree(pd[j]);
	FSfree(flist[j]);
	FSfree(fhdr[j]);
    }
    FSfree(flist);
    FSfree(fhdr);
    FSfree(pi);
    FSfree(po);
    FSfree(pd);

    if (eat_data) {
	do {
	    fsPropInfo  ti;

	    _FSEatData(svr, (reply.nameLength + 3) & ~3);
	    _FSReadPad(svr, (char *) &ti, SIZEOF(fsPropInfo));
	    _FSEatData(svr, (SIZEOF(fsPropOffset) * ti.num_offsets));
	    _FSEatData(svr, ti.data_len);
	} while (_FSReply(svr, (fsReply *) &reply,
			  ((SIZEOF(fsListFontsWithXInfoReply)
			    - SIZEOF(fsGenericReply)) >> 2), fsFalse)
		 && (reply.nameLength != 0));
    }
    SyncHandle();
    return (char **) NULL;
}
