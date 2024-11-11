/*
 * Copyright © 2024 Thomas E. Dickey
 * Copyright © 2002 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "xcursorint.h"
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG_XCURSOR
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
void _XcursorTrace(const char *fmt, ...)
{
    FILE *fp = fopen("/tmp/xcursor.log", "a");
    if (fp != NULL) {
	unsigned save = umask(0);
	va_list ap;
	va_start(ap, fmt);
	vfprintf(fp, fmt, ap);
	va_end(ap);
	fclose(fp);
	umask(save);
    }
}

void *_XcursorReturnAddr(void *addr)
{
    _XcursorTrace(T_RETURN(p), addr);
    return addr;
}

int _XcursorReturnCode(int code)
{
    _XcursorTrace(T_RETURN(d), code);
    return code;
}

unsigned long _XcursorReturnLong(unsigned long code)
{
    _XcursorTrace(T_RETURN(lu), code);
    return code;
}

unsigned _XcursorReturnUint(unsigned code)
{
    _XcursorTrace(T_RETURN(u), code);
    return code;
}

void _XcursorReturnVoid(void)
{
    _XcursorTrace(T_RETURN(s), "");
    return;
}
#endif /* DEBUG_XCURSOR */

XcursorImage *
XcursorImageCreate (int width, int height)
{
    XcursorImage    *image = NULL;

    enterFunc((T_CALLED(XcursorImageCreate) "(%d, %d)\n", width, height));

    if (width < 0 || height < 0) {
       /* EMPTY */;
    } else if (width > XCURSOR_IMAGE_MAX_SIZE
    	    || height > XCURSOR_IMAGE_MAX_SIZE) {
       /* EMPTY */;
    } else {
	image = malloc (sizeof (XcursorImage) +
			(size_t) (width * height) * sizeof (XcursorPixel));
	if (image) {
	    image->version = XCURSOR_IMAGE_VERSION;
	    image->pixels  = (XcursorPixel *) (image + 1);
	    image->size	   = (XcursorDim) (width > height ? width : height);
	    image->width   = (XcursorDim) width;
	    image->height  = (XcursorDim) height;
	    image->delay   = 0;
	}
    }
    returnAddr(image);
}

void
XcursorImageDestroy (XcursorImage *image)
{
    enterFunc((T_CALLED(XcursorImageDestroy ) "(%p)\n", (void*)image));

    free (image);

    returnVoid();
}

XcursorImages *
XcursorImagesCreate (int size)
{
    XcursorImages   *images;

    enterFunc((T_CALLED(XcursorImagesCreate) "(%d)\n", size));

    images = malloc (sizeof (XcursorImages) +
		     (size_t) size * sizeof (XcursorImage *));
    if (images) {
	images->nimage = 0;
	images->images = (XcursorImage **) (images + 1);
	images->name = NULL;
    }
    returnAddr(images);
}

void
XcursorImagesDestroy (XcursorImages *images)
{
    enterFunc((T_CALLED(XcursorImagesDestroy) "(%p)\n", (void*)images));

    if (images) {
	int	n;

	for (n = 0; n < images->nimage; n++)
	    XcursorImageDestroy (images->images[n]);
	if (images->name)
	    free (images->name);
	free (images);
    }

    returnVoid();
}

void
XcursorImagesSetName (XcursorImages *images, const char *name)
{
    enterFunc((T_CALLED(XcursorImagesSetName) "(%p, \"%s\")\n",
	      (void*)images,
	      NonNull(name)));

    if (images && name) {
	char *new = strdup (name);

	if (new) {
	    if (images->name)
		free (images->name);
	    images->name = new;
	}
    }

    returnVoid();
}

XcursorComment *
XcursorCommentCreate (XcursorUInt comment_type, int length)
{
    XcursorComment  *comment;

    if (length < 0 || length > XCURSOR_COMMENT_MAX_LEN)
	return NULL;

    comment = malloc (sizeof (XcursorComment) + (size_t) length + 1);
    if (!comment)
	return NULL;
    comment->version = XCURSOR_COMMENT_VERSION;
    comment->comment_type = comment_type;
    comment->comment = (char *) (comment + 1);
    comment->comment[0] = '\0';
    return comment;
}

void
XcursorCommentDestroy (XcursorComment *comment)
{
    free (comment);
}

XcursorComments *
XcursorCommentsCreate (int size)
{
    XcursorComments *comments;

    comments = malloc (sizeof (XcursorComments) +
		       (size_t) size * sizeof (XcursorComment *));
    if (!comments)
	return NULL;
    comments->ncomment = 0;
    comments->comments = (XcursorComment **) (comments + 1);
    return comments;
}

void
XcursorCommentsDestroy (XcursorComments *comments)
{
    int	n;

    if (!comments)
	return;

    for (n = 0; n < comments->ncomment; n++)
	XcursorCommentDestroy (comments->comments[n]);
    free (comments);
}

static XcursorBool
_XcursorReadUInt (XcursorFile *file, XcursorUInt *u)
{
    unsigned char   bytes[4];

    if (!file || !u)
	return XcursorFalse;

    if ((*file->read) (file, bytes, 4) != 4)
	return XcursorFalse;

    *u = ((XcursorUInt)(bytes[0]) << 0) |
	 ((XcursorUInt)(bytes[1]) << 8) |
         ((XcursorUInt)(bytes[2]) << 16) |
         ((XcursorUInt)(bytes[3]) << 24);
    return XcursorTrue;
}

static XcursorBool
_XcursorReadBytes (XcursorFile *file, char *bytes, int length)
{
    if (!file || !bytes || (*file->read) (file, (unsigned char *) bytes, length) != length)
	return XcursorFalse;
    return XcursorTrue;
}

static XcursorBool
_XcursorWriteUInt (XcursorFile *file, XcursorUInt u)
{
    unsigned char   bytes[4];

    if (!file)
        return XcursorFalse;

    bytes[0] = (unsigned char)(u);
    bytes[1] = (unsigned char)(u >>  8);
    bytes[2] = (unsigned char)(u >> 16);
    bytes[3] = (unsigned char)(u >> 24);
    if ((*file->write) (file, bytes, 4) != 4)
	return XcursorFalse;
    return XcursorTrue;
}

static XcursorBool
_XcursorWriteBytes (XcursorFile *file, char *bytes, int length)
{
    if (!file || !bytes || (*file->write) (file, (unsigned char *) bytes, length) != length)
	return XcursorFalse;
    return XcursorTrue;
}

static void
_XcursorFileHeaderDestroy (XcursorFileHeader *fileHeader)
{
    free (fileHeader);
}

static XcursorFileHeader *
_XcursorFileHeaderCreate (XcursorUInt ntoc)
{
    XcursorFileHeader	*fileHeader;

    if (ntoc > 0x10000)
	return NULL;
    fileHeader = malloc (sizeof (XcursorFileHeader) +
			 ntoc * sizeof (XcursorFileToc));
    if (!fileHeader)
	return NULL;
    fileHeader->magic = XCURSOR_MAGIC;
    fileHeader->header = XCURSOR_FILE_HEADER_LEN;
    fileHeader->version = XCURSOR_FILE_VERSION;
    fileHeader->ntoc = ntoc;
    fileHeader->tocs = (XcursorFileToc *) (fileHeader + 1);
    return fileHeader;
}

static XcursorFileHeader *
_XcursorReadFileHeader (XcursorFile *file)
{
    XcursorFileHeader	head, *fileHeader;
    XcursorUInt		skip;
    XcursorUInt		n;

    if (!file)
        return NULL;

    if (!_XcursorReadUInt (file, &head.magic))
	return NULL;
    if (head.magic != XCURSOR_MAGIC)
	return NULL;
    if (!_XcursorReadUInt (file, &head.header))
	return NULL;
    if (head.header < XCURSOR_FILE_HEADER_LEN)
	return NULL;
    if (!_XcursorReadUInt (file, &head.version))
	return NULL;
    if (!_XcursorReadUInt (file, &head.ntoc))
	return NULL;
    skip = head.header - XCURSOR_FILE_HEADER_LEN;
    if (skip)
	if ((*file->seek) (file, skip, SEEK_CUR) == EOF)
	    return NULL;
    fileHeader = _XcursorFileHeaderCreate (head.ntoc);
    if (!fileHeader)
	return NULL;
    fileHeader->magic = head.magic;
    fileHeader->header = head.header;
    fileHeader->version = head.version;
    fileHeader->ntoc = head.ntoc;
    for (n = 0; n < fileHeader->ntoc; n++)
    {
	if (!_XcursorReadUInt (file, &fileHeader->tocs[n].type))
	    break;
	if (!_XcursorReadUInt (file, &fileHeader->tocs[n].subtype))
	    break;
	if (!_XcursorReadUInt (file, &fileHeader->tocs[n].position))
	    break;
    }
    if (n != fileHeader->ntoc)
    {
	_XcursorFileHeaderDestroy (fileHeader);
	return NULL;
    }
    return fileHeader;
}

static XcursorUInt
_XcursorFileHeaderLength (XcursorFileHeader *fileHeader)
{
    return (XCURSOR_FILE_HEADER_LEN +
	    fileHeader->ntoc * XCURSOR_FILE_TOC_LEN);
}

static XcursorBool
_XcursorWriteFileHeader (XcursorFile *file, XcursorFileHeader *fileHeader)
{
    XcursorUInt toc;

    if (!file || !fileHeader)
        return XcursorFalse;

    if (!_XcursorWriteUInt (file, fileHeader->magic))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, fileHeader->header))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, fileHeader->version))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, fileHeader->ntoc))
	return XcursorFalse;
    for (toc = 0; toc < fileHeader->ntoc; toc++)
    {
	if (!_XcursorWriteUInt (file, fileHeader->tocs[toc].type))
	    return XcursorFalse;
	if (!_XcursorWriteUInt (file, fileHeader->tocs[toc].subtype))
	    return XcursorFalse;
	if (!_XcursorWriteUInt (file, fileHeader->tocs[toc].position))
	    return XcursorFalse;
    }
    return XcursorTrue;
}

static XcursorBool
_XcursorSeekToToc (XcursorFile		*file,
		   XcursorFileHeader	*fileHeader,
		   int			toc)
{
    if (!file || !fileHeader || \
        (*file->seek) (file, fileHeader->tocs[toc].position, SEEK_SET) == EOF)
	return XcursorFalse;
    return XcursorTrue;
}

static XcursorBool
_XcursorFileReadChunkHeader (XcursorFile	*file,
			     XcursorFileHeader	*fileHeader,
			     int		toc,
			     XcursorChunkHeader	*chunkHeader)
{
    if (!file || !fileHeader || !chunkHeader)
        return XcursorFalse;
    if (!_XcursorSeekToToc (file, fileHeader, toc))
	return XcursorFalse;
    if (!_XcursorReadUInt (file, &chunkHeader->header))
	return XcursorFalse;
    if (!_XcursorReadUInt (file, &chunkHeader->type))
	return XcursorFalse;
    if (!_XcursorReadUInt (file, &chunkHeader->subtype))
	return XcursorFalse;
    if (!_XcursorReadUInt (file, &chunkHeader->version))
	return XcursorFalse;
    /* sanity check */
    if (chunkHeader->type != fileHeader->tocs[toc].type ||
	chunkHeader->subtype != fileHeader->tocs[toc].subtype)
	return XcursorFalse;
    return XcursorTrue;
}

static XcursorBool
_XcursorFileWriteChunkHeader (XcursorFile	    *file,
			      XcursorFileHeader	    *fileHeader,
			      int		    toc,
			      XcursorChunkHeader    *chunkHeader)
{
    if (!file || !fileHeader || !chunkHeader)
        return XcursorFalse;
    if (!_XcursorSeekToToc (file, fileHeader, toc))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, chunkHeader->header))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, chunkHeader->type))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, chunkHeader->subtype))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, chunkHeader->version))
	return XcursorFalse;
    return XcursorTrue;
}

#define dist(a,b)   ((a) > (b) ? (a) - (b) : (b) - (a))

static XcursorDim
_XcursorFindBestSize (XcursorFileHeader *fileHeader,
		      XcursorDim	size,
		      int		*nsizesp)
{
    XcursorUInt	n;
    int		nsizes = 0;
    XcursorDim	bestSize = 0;
    XcursorDim	thisSize;

    enterFunc((T_CALLED(_XcursorFindBestSize) "(%p, %u, %p)\n",
              (void*)fileHeader, size, (void*)nsizesp));

    if (!fileHeader || !nsizesp)
        returnUint(0);

    for (n = 0; n < fileHeader->ntoc; n++)
    {
	if (fileHeader->tocs[n].type != XCURSOR_IMAGE_TYPE)
	    continue;
	thisSize = fileHeader->tocs[n].subtype;
	if (!bestSize || dist (thisSize, size) < dist (bestSize, size))
	{
	    bestSize = thisSize;
	    nsizes = 1;
	}
	else if (thisSize == bestSize)
	    nsizes++;
    }
    *nsizesp = nsizes;
    returnUint(bestSize);
}

static int
_XcursorFindImageToc (XcursorFileHeader	*fileHeader,
		      XcursorDim	size,
		      int		count)
{
    XcursorUInt		toc;
    XcursorDim		thisSize;

    enterFunc((T_CALLED(_XcursorFindImageToc) "(%p, %u, %d)\n",
              (void*)fileHeader, size, count));

    if (!fileHeader)
        returnCode(0);

    for (toc = 0; toc < fileHeader->ntoc; toc++)
    {
	if (fileHeader->tocs[toc].type != XCURSOR_IMAGE_TYPE)
	    continue;
	thisSize = fileHeader->tocs[toc].subtype;
	if (thisSize != size)
	    continue;
	if (!count)
	    break;
	count--;
    }
    if (toc == fileHeader->ntoc)
	returnCode(-1);
    returnCode((int) toc);
}

static XcursorImage *
_XcursorReadImage (XcursorFile		*file,
		   XcursorFileHeader	*fileHeader,
		   int			toc)
{
    XcursorChunkHeader	chunkHeader;
    XcursorImage	head;
    XcursorImage	*image;
    int			n;
    XcursorPixel	*p;

    enterFunc((T_CALLED(_XcursorReadImage) "(%p, %p, %d)\n",
              (void*)file, (void*)fileHeader, toc));

    if (!file || !fileHeader)
        returnAddr(NULL);

    if (!_XcursorFileReadChunkHeader (file, fileHeader, toc, &chunkHeader))
	returnAddr(NULL);
    if (!_XcursorReadUInt (file, &head.width))
	returnAddr(NULL);
    if (!_XcursorReadUInt (file, &head.height))
	returnAddr(NULL);
    if (!_XcursorReadUInt (file, &head.xhot))
	returnAddr(NULL);
    if (!_XcursorReadUInt (file, &head.yhot))
	returnAddr(NULL);
    if (!_XcursorReadUInt (file, &head.delay))
	returnAddr(NULL);
    /* sanity check data */
    if (head.width > XCURSOR_IMAGE_MAX_SIZE  ||
	head.height > XCURSOR_IMAGE_MAX_SIZE)
	returnAddr(NULL);
    if (head.width == 0 || head.height == 0)
	returnAddr(NULL);
    if (head.xhot > head.width || head.yhot > head.height)
	returnAddr(NULL);

    /* Create the image and initialize it */
    image = XcursorImageCreate ((int) head.width, (int) head.height);
    if (image == NULL)
	returnAddr(NULL);
    if (chunkHeader.version < image->version)
	image->version = chunkHeader.version;
    image->size = chunkHeader.subtype;
    image->xhot = head.xhot;
    image->yhot = head.yhot;
    image->delay = head.delay;
    n = (int) (image->width * image->height);
    p = image->pixels;
    while (n--)
    {
	if (!_XcursorReadUInt (file, p))
	{
	    XcursorImageDestroy (image);
	    returnAddr(NULL);
	}
	p++;
    }
    returnAddr(image);
}

static XcursorUInt
_XcursorImageLength (XcursorImage   *image)
{
    enterFunc((T_CALLED(_XcursorImageLength) "(%p)\n", (void*)image));

    if (!image)
        returnUint(0);

    returnUint(XCURSOR_IMAGE_HEADER_LEN + (image->width * image->height) * 4);
}

static XcursorBool
_XcursorWriteImage (XcursorFile		*file,
		    XcursorFileHeader	*fileHeader,
		    int			toc,
		    XcursorImage	*image)
{
    XcursorChunkHeader	chunkHeader;
    int			n;
    XcursorPixel	*p;

    enterFunc((T_CALLED(_XcursorWriteImage) "(%p, %p, %d, %p)\n",
              (void*)file, (void*)fileHeader, toc, (void*)image));

    if (!file || !fileHeader || !image)
        returnCode(XcursorFalse);

    /* sanity check data */
    if (image->width > XCURSOR_IMAGE_MAX_SIZE  ||
	image->height > XCURSOR_IMAGE_MAX_SIZE)
	returnCode(XcursorFalse);
    if (image->width == 0 || image->height == 0)
	returnCode(XcursorFalse);
    if (image->xhot > image->width || image->yhot > image->height)
	returnCode(XcursorFalse);

    /* write chunk header */
    chunkHeader.header = XCURSOR_IMAGE_HEADER_LEN;
    chunkHeader.type = XCURSOR_IMAGE_TYPE;
    chunkHeader.subtype = image->size;
    chunkHeader.version = XCURSOR_IMAGE_VERSION;

    if (!_XcursorFileWriteChunkHeader (file, fileHeader, toc, &chunkHeader))
	returnCode(XcursorFalse);

    /* write extra image header fields */
    if (!_XcursorWriteUInt (file, image->width))
	returnCode(XcursorFalse);
    if (!_XcursorWriteUInt (file, image->height))
	returnCode(XcursorFalse);
    if (!_XcursorWriteUInt (file, image->xhot))
	returnCode(XcursorFalse);
    if (!_XcursorWriteUInt (file, image->yhot))
	returnCode(XcursorFalse);
    if (!_XcursorWriteUInt (file, image->delay))
	returnCode(XcursorFalse);

    /* write the image */
    n = (int) (image->width * image->height);
    p = image->pixels;
    while (n--)
    {
	if (!_XcursorWriteUInt (file, *p))
	    returnCode(XcursorFalse);
	p++;
    }
    returnCode(XcursorTrue);
}

static XcursorComment *
_XcursorReadComment (XcursorFile	    *file,
		     XcursorFileHeader	    *fileHeader,
		     int		    toc)
{
    XcursorChunkHeader	chunkHeader;
    XcursorUInt		length;
    XcursorComment	*comment;

    enterFunc((T_CALLED(_XcursorReadComment) "(%p, %p, %d)\n",
              (void*)file, (void*)fileHeader, toc));

    if (!file || !fileHeader)
        returnAddr(NULL);

    /* read chunk header */
    if (!_XcursorFileReadChunkHeader (file, fileHeader, toc, &chunkHeader))
	returnAddr(NULL);
    /* read extra comment header fields */
    if (!_XcursorReadUInt (file, &length))
	returnAddr(NULL);
    comment = XcursorCommentCreate (chunkHeader.subtype, (int) length);
    if (!comment)
	returnAddr(NULL);
    if (!_XcursorReadBytes (file, comment->comment, (int) length))
    {
	XcursorCommentDestroy (comment);
	returnAddr(NULL);
    }
    comment->comment[length] = '\0';
    returnAddr(comment);
}

static XcursorUInt
_XcursorCommentLength (XcursorComment	    *comment)
{
    return XCURSOR_COMMENT_HEADER_LEN + (XcursorUInt) strlen (comment->comment);
}

static XcursorBool
_XcursorWriteComment (XcursorFile	    *file,
		      XcursorFileHeader	    *fileHeader,
		      int		    toc,
		      XcursorComment	    *comment)
{
    XcursorChunkHeader	chunkHeader;
    XcursorUInt		length;

    if (!file || !fileHeader || !comment || !comment->comment)
        return XcursorFalse;

    length = (XcursorUInt) strlen (comment->comment);

    /* sanity check data */
    if (length > XCURSOR_COMMENT_MAX_LEN)
	return XcursorFalse;

    /* read chunk header */
    chunkHeader.header = XCURSOR_COMMENT_HEADER_LEN;
    chunkHeader.type = XCURSOR_COMMENT_TYPE;
    chunkHeader.subtype = comment->comment_type;
    chunkHeader.version = XCURSOR_COMMENT_VERSION;

    if (!_XcursorFileWriteChunkHeader (file, fileHeader, toc, &chunkHeader))
	return XcursorFalse;

    /* write extra comment header fields */
    if (!_XcursorWriteUInt (file, length))
	return XcursorFalse;

    if (!_XcursorWriteBytes (file, comment->comment, (int) length))
	return XcursorFalse;
    return XcursorTrue;
}

static XcursorImage *
_XcursorResizeImage (XcursorImage *src, int size)
{
    XcursorDim dest_y, dest_x;
    double scale = (double) size / src->size;
    XcursorImage *dest;

    enterFunc((T_CALLED(_XcursorResizeImage) "(%p, %d)\n", (void*)src, size));

    dest = XcursorImageCreate ((int) (src->width * scale),
			       (int) (src->height * scale));
    if (!dest)
	returnAddr(NULL);

    dest->size = (XcursorDim) size;
    dest->xhot = (XcursorDim) (src->xhot * scale);
    dest->yhot = (XcursorDim) (src->yhot * scale);
    dest->delay = src->delay;

    for (dest_y = 0; dest_y < dest->height; dest_y++)
    {
	XcursorDim src_y = (XcursorDim) (dest_y / scale);
	XcursorPixel *src_row = src->pixels + (src_y * src->width);
	XcursorPixel *dest_row = dest->pixels + (dest_y * dest->width);
	for (dest_x = 0; dest_x < dest->width; dest_x++)
	{
	    XcursorDim src_x = (XcursorDim) (dest_x / scale);
	    dest_row[dest_x] = src_row[src_x];
	}
    }

    returnAddr(dest);
}

static XcursorImage *
_XcursorXcFileLoadImage (XcursorFile *file, int size, XcursorBool resize)
{
    XcursorFileHeader	*fileHeader;
    XcursorDim		bestSize;
    int			nsize;
    int			toc;
    XcursorImage	*image;

    enterFunc((T_CALLED(_XcursorXcFileLoadImage) "(%p, %d, %d)\n",
	      (void*)file, size, resize));

    if (size < 0)
	returnAddr(NULL);
    fileHeader = _XcursorReadFileHeader (file);
    if (!fileHeader)
	returnAddr(NULL);
    bestSize = _XcursorFindBestSize (fileHeader, (XcursorDim) size, &nsize);
    if (!bestSize)
	returnAddr(NULL);
    toc = _XcursorFindImageToc (fileHeader, bestSize, 0);
    if (toc < 0)
	returnAddr(NULL);
    image = _XcursorReadImage (file, fileHeader, toc);
    _XcursorFileHeaderDestroy (fileHeader);

    if (resize && image != NULL && (image->size != (XcursorDim) size))
    {
	XcursorImage *resized_image = _XcursorResizeImage (image, size);
	XcursorImageDestroy (image);
	image = resized_image;
    }

    returnAddr(image);
}

XcursorImage *
XcursorXcFileLoadImage (XcursorFile *file, int size)
{
    enterFunc((T_CALLED(XcursorXcFileLoadImage) "(%p, %d)\n", (void*)file, size));

    returnAddr(_XcursorXcFileLoadImage (file, size, XcursorFalse));
}

XcursorImages *
_XcursorXcFileLoadImages (XcursorFile *file, int size, XcursorBool resize)
{
    XcursorFileHeader	*fileHeader;
    XcursorDim		bestSize;
    int			nsize;
    XcursorImages	*images;
    int			n;
    XcursorImage        *image;

    enterFunc((T_CALLED(_XcursorXcFileLoadImages) "(%p, %d, %d)\n",
	      (void*)file, size, resize));

    if (!file || size < 0)
	returnAddr(NULL);
    fileHeader = _XcursorReadFileHeader (file);
    if (!fileHeader)
	returnAddr(NULL);
    bestSize = _XcursorFindBestSize (fileHeader, (XcursorDim) size, &nsize);
    if (!bestSize)
    {
        _XcursorFileHeaderDestroy (fileHeader);
	returnAddr(NULL);
    }
    images = XcursorImagesCreate (nsize);
    if (!images)
    {
        _XcursorFileHeaderDestroy (fileHeader);
	returnAddr(NULL);
    }
    for (n = 0; n < nsize; n++)
    {
	int toc = _XcursorFindImageToc (fileHeader, bestSize, n);
	if (toc < 0)
	    break;
	image = _XcursorReadImage (file, fileHeader, toc);
	if (!image)
	    break;
	if (resize && (image->size != (XcursorDim) size))
	{
	    XcursorImage *resized_image = _XcursorResizeImage (image, size);
	    XcursorImageDestroy (image);
	    image = resized_image;
	    if (image == NULL)
		break;
	}
	images->images[images->nimage] = image;
	images->nimage++;
    }
    _XcursorFileHeaderDestroy (fileHeader);
    if (images != NULL && images->nimage != nsize)
    {
	XcursorImagesDestroy (images);
	images = NULL;
    }
    returnAddr(images);
}

XcursorImages *
XcursorXcFileLoadImages (XcursorFile *file, int size)
{
    enterFunc((T_CALLED(XcursorXcFileLoadImages) "(%p, %d)\n", (void*)file, size));

    returnAddr(_XcursorXcFileLoadImages (file, size, XcursorFalse));
}

XcursorImages *
XcursorXcFileLoadAllImages (XcursorFile *file)
{
    XcursorFileHeader	*fileHeader;
    XcursorImage	*image;
    XcursorImages	*images;
    int			nimage;
    XcursorUInt		n;
    XcursorUInt		toc;

    enterFunc((T_CALLED(XcursorXcFileLoadAllImages) "(%p)\n", (void*)file));

    if (!file)
        returnAddr(NULL);

    fileHeader = _XcursorReadFileHeader (file);
    if (!fileHeader)
	returnAddr(NULL);
    nimage = 0;
    for (n = 0; n < fileHeader->ntoc; n++)
    {
	switch (fileHeader->tocs[n].type) {
	case XCURSOR_IMAGE_TYPE:
	    nimage++;
	    break;
	}
    }
    images = XcursorImagesCreate (nimage);
    if (!images)
    {
	_XcursorFileHeaderDestroy (fileHeader);
	returnAddr(NULL);
    }
    for (toc = 0; toc < fileHeader->ntoc; toc++)
    {
	switch (fileHeader->tocs[toc].type) {
	case XCURSOR_IMAGE_TYPE:
	    image = _XcursorReadImage (file, fileHeader, (int) toc);
	    if (image)
	    {
		images->images[images->nimage] = image;
		images->nimage++;
	    }
	    break;
	}
    }
    _XcursorFileHeaderDestroy (fileHeader);
    if (images->nimage != nimage)
    {
	XcursorImagesDestroy (images);
	images = NULL;
    }
    returnAddr(images);
}

XcursorBool
XcursorXcFileLoad (XcursorFile	    *file,
		   XcursorComments  **commentsp,
		   XcursorImages    **imagesp)
{
    XcursorFileHeader	*fileHeader;
    int			nimage;
    int			ncomment;
    XcursorImages	*images;
    XcursorImage	*image;
    XcursorComment	*comment;
    XcursorComments	*comments;
    XcursorUInt		toc;

    enterFunc((T_CALLED(XcursorXcFileLoad) "(%p, %p, %p)\n",
	      (void*)file, (void*)commentsp, (void*)imagesp));

    if (!file)
        returnCode(0);
    fileHeader = _XcursorReadFileHeader (file);
    if (!fileHeader)
	returnCode(0);
    nimage = 0;
    ncomment = 0;
    for (toc = 0; toc < fileHeader->ntoc; toc++)
    {
	switch (fileHeader->tocs[toc].type) {
	case XCURSOR_COMMENT_TYPE:
	    ncomment++;
	    break;
	case XCURSOR_IMAGE_TYPE:
	    nimage++;
	    break;
	}
    }
    images = XcursorImagesCreate (nimage);
    if (!images)
    {
	_XcursorFileHeaderDestroy (fileHeader);
	returnCode(0);
    }
    comments = XcursorCommentsCreate (ncomment);
    if (!comments)
    {
	_XcursorFileHeaderDestroy (fileHeader);
	XcursorImagesDestroy (images);
	returnCode(0);
    }
    for (toc = 0; toc < fileHeader->ntoc; toc++)
    {
	switch (fileHeader->tocs[toc].type) {
	case XCURSOR_COMMENT_TYPE:
	    comment = _XcursorReadComment (file, fileHeader, (int) toc);
	    if (comment)
	    {
		comments->comments[comments->ncomment] = comment;
		comments->ncomment++;
	    }
	    break;
	case XCURSOR_IMAGE_TYPE:
	    image = _XcursorReadImage (file, fileHeader, (int) toc);
	    if (image)
	    {
		images->images[images->nimage] = image;
		images->nimage++;
	    }
	    break;
	}
    }
    _XcursorFileHeaderDestroy (fileHeader);
    if (images->nimage != nimage || comments->ncomment != ncomment)
    {
	XcursorImagesDestroy (images);
	XcursorCommentsDestroy (comments);
	images = NULL;
	comments = NULL;
	returnCode(XcursorFalse);
    }
    *imagesp = images;
    *commentsp = comments;
    returnCode(XcursorTrue);
}

XcursorBool
XcursorXcFileSave (XcursorFile		    *file,
		   const XcursorComments    *comments,
		   const XcursorImages	    *images)
{
    XcursorFileHeader	*fileHeader;
    XcursorUInt		position;
    XcursorUInt		n;
    int			toc;
    XcursorUInt		ncomment;
    XcursorUInt		nimage;

    enterFunc((T_CALLED(XcursorXcFileSave) "(%p, %p, %p)\n",
	      (void*)file, (const void*)comments, (const void*)images));

    if (!file || !comments || !images)
	returnCode(XcursorFalse);

    /*
     * Caller may have tainted the counts.
     */
    ncomment = (XcursorUInt)(comments->ncomment > 0 ? comments->ncomment : 0);
    nimage   = (XcursorUInt)(images->nimage     > 0 ? images->nimage : 0);

    fileHeader = _XcursorFileHeaderCreate (ncomment + nimage);
    if (!fileHeader)
	returnCode(XcursorFalse);

    position = _XcursorFileHeaderLength (fileHeader);

    /*
     * Compute the toc.  Place the images before the comments
     * as they're more often read
     */

    toc = 0;
    for (n = 0; n < nimage; n++)
    {
	fileHeader->tocs[toc].type = XCURSOR_IMAGE_TYPE;
	fileHeader->tocs[toc].subtype = images->images[n]->size;
	fileHeader->tocs[toc].position = position;
	position += _XcursorImageLength (images->images[n]);
	toc++;
    }

    for (n = 0; n < ncomment; n++)
    {
	fileHeader->tocs[toc].type = XCURSOR_COMMENT_TYPE;
	fileHeader->tocs[toc].subtype = comments->comments[n]->comment_type;
	fileHeader->tocs[toc].position = position;
	position += _XcursorCommentLength (comments->comments[n]);
	toc++;
    }

    /*
     * Write the header and the toc
     */
    if (!_XcursorWriteFileHeader (file, fileHeader))
	goto bail;

    /*
     * Write the images
     */
    toc = 0;
    for (n = 0; n < nimage; n++)
    {
	if (!_XcursorWriteImage (file, fileHeader, toc, images->images[n]))
	    goto bail;
	toc++;
    }

    /*
     * Write the comments
     */
    for (n = 0; n < ncomment; n++)
    {
	if (!_XcursorWriteComment (file, fileHeader, toc, comments->comments[n]))
	    goto bail;
	toc++;
    }

    _XcursorFileHeaderDestroy (fileHeader);
    returnCode(XcursorTrue);

bail:
    _XcursorFileHeaderDestroy (fileHeader);
    returnCode(XcursorFalse);
}

static int
_XcursorStdioFileRead (XcursorFile *file, unsigned char *buf, int len)
{
    FILE    *f = file->closure;
    return (int) fread (buf, 1, (size_t) len, f);
}

static int
_XcursorStdioFileWrite (XcursorFile *file, unsigned char *buf, int len)
{
    FILE    *f = file->closure;
    return (int) fwrite (buf, 1, (size_t) len, f);
}

static int
_XcursorStdioFileSeek (XcursorFile *file, long offset, int whence)
{
    FILE    *f = file->closure;
    return fseek (f, offset, whence);
}

static void
_XcursorStdioFileInitialize (FILE *stdfile, XcursorFile *file)
{
    file->closure = stdfile;
    file->read = _XcursorStdioFileRead;
    file->write = _XcursorStdioFileWrite;
    file->seek = _XcursorStdioFileSeek;
}

XcursorImage *
_XcursorFileLoadImage (FILE *file, int size, XcursorBool resize)
{
    XcursorFile	f;

    enterFunc((T_CALLED(_XcursorFileLoadImage) "(%p, %d, %d)\n", (void*)file, size, resize));

    if (!file)
	returnAddr(NULL);

    _XcursorStdioFileInitialize (file, &f);
    returnAddr(_XcursorXcFileLoadImage (&f, size, resize));
}

XcursorImages *
_XcursorFileLoadImages (FILE *file, int size, XcursorBool resize)
{
    XcursorFile	f;

    enterFunc((T_CALLED(_XcursorFileLoadImages) "(%p, %d, %d)\n", (void*)file, size, resize));

    if (!file)
	returnAddr(NULL);

    _XcursorStdioFileInitialize (file, &f);
    returnAddr(_XcursorXcFileLoadImages (&f, size, resize));
}

XcursorImage *
XcursorFileLoadImage (FILE *file, int size)
{
    XcursorFile	f;

    enterFunc((T_CALLED(XcursorFileLoadImage) "(%p, %d)\n", (void*)file, size));

    if (!file)
	returnAddr(NULL);

    _XcursorStdioFileInitialize (file, &f);
    returnAddr(XcursorXcFileLoadImage (&f, size));
}

XcursorImages *
XcursorFileLoadImages (FILE *file, int size)
{
    XcursorFile	f;

    enterFunc((T_CALLED(XcursorFileLoadImages) "(%p, %d)\n", (void*)file, size));

    if (!file)
	returnAddr(NULL);

    _XcursorStdioFileInitialize (file, &f);
    returnAddr(XcursorXcFileLoadImages (&f, size));
}

XcursorImages *
XcursorFileLoadAllImages (FILE *file)
{
    XcursorFile	f;

    enterFunc((T_CALLED(XcursorFileLoadAllImages) "(%p)\n", (void*)file));

    if (!file)
	returnAddr(NULL);

    _XcursorStdioFileInitialize (file, &f);
    returnAddr(XcursorXcFileLoadAllImages (&f));
}

XcursorBool
XcursorFileLoad (FILE		    *file,
		 XcursorComments    **commentsp,
		 XcursorImages	    **imagesp)
{
    XcursorFile	f;

    enterFunc((T_CALLED(XcursorFileLoad) "(%p, %p, %p)\n",
	       (void*)file, (void*)commentsp, (void*)imagesp));

    if (!file || !commentsp || !imagesp)
	returnCode(XcursorFalse);

    _XcursorStdioFileInitialize (file, &f);
    returnCode(XcursorXcFileLoad (&f, commentsp, imagesp));
}

XcursorBool
XcursorFileSaveImages (FILE *file, const XcursorImages *images)
{
    XcursorComments *comments;
    XcursorFile	    f;
    XcursorBool	    ret;

    enterFunc((T_CALLED(XcursorFileSaveImages) "(%p, %p)\n",
	       (void*)file, (const void*)images));

    if (!file || !images)
	returnCode(0);
    if ((comments = XcursorCommentsCreate (0)) == NULL)
	returnCode(0);
    _XcursorStdioFileInitialize (file, &f);
    ret = XcursorXcFileSave (&f, comments, images) && fflush (file) != EOF;
    XcursorCommentsDestroy (comments);
    returnCode(ret);
}

XcursorBool
XcursorFileSave (FILE *			file,
		 const XcursorComments	*comments,
		 const XcursorImages	*images)
{
    XcursorFile	    f;

    enterFunc((T_CALLED(_XcursorFileSave) "(%p, %p, %p)\n",
	       (void*)file, (const void*)comments, (const void*)images));

    if (!file || !comments || !images)
	returnCode(XcursorFalse);

    _XcursorStdioFileInitialize (file, &f);
    returnCode(XcursorXcFileSave (&f, comments, images) && fflush (file) != EOF);
}

XcursorImage *
XcursorFilenameLoadImage (const char *file, int size)
{
    FILE	    *f;
    XcursorImage    *image;

    enterFunc((T_CALLED(XcursorFilenameLoadImage) "(\"%s\", %d)\n",
	      NonNull(file), size));

    if (!file || size < 0)
	returnAddr(NULL);

    f = fopen (file, "r" FOPEN_CLOEXEC);
    if (!f)
	returnAddr(NULL);
    image = XcursorFileLoadImage (f, size);
    fclose (f);
    returnAddr(image);
}

XcursorImages *
_XcursorFilenameLoadImages (const char *file, int size, XcursorBool resize)
{
    FILE	    *f;
    XcursorImages   *images;

    enterFunc((T_CALLED(_XcursorFilenameLoadImages) "(\"%s\", %d, %d)\n",
	      NonNull(file), size, resize));

    if (!file || size < 0)
	returnAddr(NULL);

    f = fopen (file, "r" FOPEN_CLOEXEC);
    if (!f)
	returnAddr(NULL);
    images = _XcursorFileLoadImages (f, size, resize);
    fclose (f);
    returnAddr(images);
}

XcursorImages *
XcursorFilenameLoadImages (const char *file, int size)
{
    FILE	    *f;
    XcursorImages   *images;

    enterFunc((T_CALLED(XcursorFilenameLoadImages) "(\"%s\", %d)\n",
	      NonNull(file), size));

    if (!file || size < 0)
	returnAddr(NULL);

    f = fopen (file, "r" FOPEN_CLOEXEC);
    if (!f)
	returnAddr(NULL);
    images = XcursorFileLoadImages (f, size);
    fclose (f);
    returnAddr(images);
}

XcursorImages *
XcursorFilenameLoadAllImages (const char *file)
{
    FILE	    *f;
    XcursorImages   *images;

    enterFunc((T_CALLED(XcursorFilenameLoadAllImages) "(\"%s\")\n",
	      NonNull(file)));

    if (!file)
	returnAddr(NULL);

    f = fopen (file, "r" FOPEN_CLOEXEC);
    if (!f)
	returnAddr(NULL);
    images = XcursorFileLoadAllImages (f);
    fclose (f);
    returnAddr(images);
}

XcursorBool
XcursorFilenameLoad (const char		*file,
		     XcursorComments	**commentsp,
		     XcursorImages	**imagesp)
{
    FILE	    *f;
    XcursorBool	    ret;

    enterFunc((T_CALLED() "(\"%s\", %p, %p)\n",
	      NonNull(file), (void*)commentsp, (void*)imagesp));

    if (!file)
	returnCode(XcursorFalse);

    f = fopen (file, "r" FOPEN_CLOEXEC);
    if (!f)
	returnCode(0);
    ret = XcursorFileLoad (f, commentsp, imagesp);
    fclose (f);
    returnCode(ret);
}

XcursorBool
XcursorFilenameSaveImages (const char *file, const XcursorImages *images)
{
    FILE	    *f;
    XcursorBool	    ret;

    enterFunc((T_CALLED(XcursorFilenameSaveImages) "(\"%s\", %p)\n",
	      NonNull(file), (const void*)images));

    if (!file || !images)
	returnCode(XcursorFalse);

    f = fopen (file, "w" FOPEN_CLOEXEC);
    if (!f)
	returnCode(0);
    ret = XcursorFileSaveImages (f, images);
    returnCode(fclose (f) != EOF && ret);
}

XcursorBool
XcursorFilenameSave (const char		    *file,
		     const XcursorComments  *comments,
		     const XcursorImages    *images)
{
    FILE	    *f;
    XcursorBool	    ret;

    enterFunc((T_CALLED(XcursorFilenameSave ) "(\"%s\", %p, %p)\n",
    		NonNull(file),
		(const void *) comments,
		(const void *) images));

    if (!file || !comments || !images) {
	ret = XcursorFalse;
    } else {
	f = fopen (file, "w" FOPEN_CLOEXEC);
	if (!f) {
	    ret = 0;
	} else {
	    ret = XcursorFileSave (f, comments, images);
	    ret = fclose (f) != EOF && ret;
	}
    }
    returnCode(ret);
}
