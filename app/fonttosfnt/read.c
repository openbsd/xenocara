/* $XdotOrg: xc/programs/fonttosfnt/read.c,v 1.2 2004/04/23 19:54:32 eich Exp $ */
/*
Copyright (c) 2002 by Juliusz Chroboczek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/* $XdotOrg: xc/programs/fonttosfnt/read.c,v 1.2 2004/04/23 19:54:32 eich Exp $ */
/* $XFree86: xc/programs/fonttosfnt/read.c,v 1.5 2003/12/19 02:05:39 dawes Exp $ */

#include <stdio.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MODULE_H
#include FT_BDF_H
#include FT_FONT_FORMATS_H
#include "X11/Xos.h"
#include "fonttosfnt.h"
#include "X11/fonts/fontenc.h"

#define FLOOR2(x, y) ((x) >= 0 ? (x) / (y) : -(((-(x)) + (y) - 1) / (y)))
#define CEIL2(x, y) (FLOOR2((x) + (y) - 1, (y)))
#define FT_Pos_DOWN(x) (FLOOR2((x),64))
#define FT_Pos_UP(x) (CEIL2((x), 64))
#define MIN(x, y) (((x) <= (y)) ? (x) : (y))
#define STREAM_FILE(stream) ((FILE*)stream->descriptor.pointer)

static int ft_inited = 0;
static FT_Library ft_library;

static int
FT_Ensure_Inited(void)
{
    int rc;
    if(ft_inited)
        return 0;

    rc = FT_Init_FreeType(&ft_library);
    if(rc != 0)
        return rc;

    ft_inited = 1;
    return 0;
}

static unsigned long
forwardRead(FT_Stream stream, unsigned long offset, unsigned char *buffer,
            unsigned long count) {
    unsigned char skip_buffer[BUFSIZ];
    unsigned long skip_count;
    FILE *file = STREAM_FILE(stream);

    /* We may be asked to skip forward, but by not doing so we increase our
       chance of survival. */
    if(count == 0)
        return ferror(file) == 0 ? 0 : 1;

    if(offset < stream->pos) {
        fprintf(stderr, "Cannot move backward in input stream.\n");
        return 0;
    }
    while((skip_count = MIN(BUFSIZ, offset - stream->pos))) {
        if(fread(skip_buffer, sizeof(*skip_buffer), skip_count, file) <
           skip_count)
            return 0;
        stream->pos += sizeof(*skip_buffer) * skip_count;
    }

    return (unsigned long)fread(buffer, sizeof(*buffer), count, file);
}

static void
streamClose(FT_Stream stream) {
    fclose(STREAM_FILE(stream));
    stream->descriptor.pointer = NULL;
    stream->size = 0;
}

int
readFile(char *filename, FontPtr font)
{
    int j, k, index;
    int rc;
    FT_Open_Args input = { 0 };
    FT_Face face;
    StrikePtr strike;
    BitmapPtr bitmap;
    int symbol = 0;
    int force_unicode = 1;
    const char *family_name, *encoding_name, *file_format;
    FontMapPtr mapping = NULL;
    FontMapReversePtr reverse = NULL;
    

    rc = FT_Ensure_Inited();
    if(rc != 0)
        return rc;

    if(filename != NULL) {
        input.pathname = filename;
        input.flags = FT_OPEN_PATHNAME;
    } else {
        input.flags = FT_OPEN_STREAM | FT_OPEN_DRIVER;
        input.driver = FT_Get_Module(ft_library, "bdf");
        input.stream = calloc(1, sizeof(FT_StreamRec));
        if(input.stream == NULL)
            return -1;

        input.stream->size = 0x7FFFFFFF;
        input.stream->descriptor.pointer = stdin;
        input.stream->read = forwardRead;
        input.stream->close = streamClose;
    }
    rc = FT_Open_Face(ft_library, &input, 0, &face);
    if(rc != 0) {
        fprintf(stderr, "Couldn't open face %s.\n",
                filename ? filename : "<stdin>");
        return -1;
    }

    file_format = FT_Get_Font_Format(face);
    if(strcmp(file_format, "BDF") != 0)
	fprintf(stderr,
		"font file %s is of format %s.\n"
		"It's recommended to convert directly from a BDF font.\n"
		"Some font properties may get lost when converting via a PCF font.\n",
		filename ? filename : "<stdin>",
		file_format);

    /* FreeType will insist on encodings which are simple subsets of unicode
     * to be read as unicode regardless of what we call them. */
    for(j = 0; j < face->num_charmaps; ++j) {
        if((face->charmaps[j]->encoding == ft_encoding_none) ||
           (face->charmaps[j]->encoding == ft_encoding_adobe_standard)) {
            force_unicode = 0;
            break;
        }
    }

    encoding_name = faceEncoding(face);
    if(encoding_name == NULL) {
        symbol = 1;
    } else if(strcasecmp(encoding_name, "iso10646-1") != 0) {
        if(reencode_flag)
            mapping = FontEncMapFind(encoding_name,
                                     FONT_ENCODING_UNICODE, 0, 0, NULL);
        if(mapping == NULL) {
            symbol = 1;
        } else {
            reverse = FontMapReverse(mapping);
            if(reverse == NULL) {
                fprintf(stderr, "Couldn't reverse mapping.\n");
                return -1;
            }
        }
    }

    if(face->family_name)
        family_name = face->family_name;
    else
	family_name = faceStringProp(face, "FONT");

    if(verbose_flag) {
        fprintf(stderr, "%s %s %s: %d sizes%s\n",
                filename ? filename : "<stdin>",
                face->family_name, face->style_name, face->num_fixed_sizes,
                symbol ? " (symbol)" : "");
    }

    if(font->numNames == 0 && face->style_name && family_name) {
        char *full_name, *unique_name, *buf;
        int i;
        if(strcmp(face->style_name, "Regular") == 0)
            full_name = sprintf_alloc("%s", family_name);
        else
            full_name = sprintf_alloc("%s %s", 
                                      family_name, face->style_name);

        /* The unique name doesn't actually need to be globally
           unique; it only needs to be unique among all installed fonts on a
           Windows system.  We don't bother getting it quite right. */
        if(face->num_fixed_sizes <= 0)
            unique_name = sprintf_alloc("%s "XVENDORNAMESHORT" bitmap",
                                        full_name);
        else if(face->available_sizes[0].width == 
                face->available_sizes[0].height)
            unique_name = sprintf_alloc("%s "XVENDORNAMESHORT
					   " bitmap size %d",
                                           full_name, 
                                           face->available_sizes[0].height);
        else
            unique_name = sprintf_alloc("%s "XVENDORNAMESHORT
                                        " bitmap size %dx%d",
                                        full_name, 
                                        face->available_sizes[0].width,
                                        face->available_sizes[0].height);

        font->names = malloc(10 * sizeof(FontNameEntryRec));
        if(font->names == NULL) {
            fprintf(stderr, "Couldn't allocate names.\n");
            return -1;
        }
        i = 0;

	buf = faceStringProp(face, "COPYRIGHT");
	if(buf) {
            font->names[i].nid = 0;
            font->names[i].size = 2 * strlen(buf);
            font->names[i].value = makeUTF16(buf);
	    free(buf);
            i++;
	}

        font->names[i].nid = 1;
        font->names[i].size = 2 * strlen(family_name);
        font->names[i].value = makeUTF16(family_name);
        i++;

        font->names[i].nid = 2;
        font->names[i].size = 2 * strlen(face->style_name);
        font->names[i].value = makeUTF16(face->style_name);
        i++;

        font->names[i].nid = 3;
        font->names[i].size = 2 * strlen(unique_name);
        font->names[i].value = makeUTF16(unique_name);
        i++;

        font->names[i].nid = 4;
        font->names[i].size = 2 * strlen(full_name);
        font->names[i].value = makeUTF16(full_name);
        i++;

        font->names[i].nid = 5;
        font->names[i].size = 2 * strlen("Version 0.0");
        font->names[i].value = makeUTF16("Version 0.0");
        i++;

	buf = faceStringProp(face, "FOUNDRY");
	if(buf) {
            font->names[i].nid = 8;
	    font->names[i].size = 2 * strlen(buf);
	    font->names[i].value = makeUTF16(buf);
	    free(buf);
            i++;
	}

        font->names[i].nid = 10;
        font->names[i].size = 2 * strlen(XVENDORNAMESHORT
					 " converted bitmap font");
        font->names[i].value = makeUTF16(XVENDORNAMESHORT
					 " converted bitmap font");
        i++;
#ifdef __VENDORWEBSUPPORT__
        font->names[i].nid = 11;
        font->names[i].size = 2 * strlen(__VENDORWEBSUPPORT__);
        font->names[i].value = makeUTF16(__VENDORWEBSUPPORT__);
        i++;
#endif
        font->numNames = i;
    }

    if(face->num_fixed_sizes == 0) {
        fprintf(stderr, "No bitmaps in face.\n");
        return -1;
    }

    if((!symbol && !mapping) || force_unicode) {
        rc = FT_Select_Charmap(face, ft_encoding_unicode);
    } else {
        rc = FT_Select_Charmap(face, ft_encoding_none);
        if(rc != 0) {
            /* BDF will default to Adobe Standard even for nonstandard
             * encodings, so try that as a last resort. */
            rc = FT_Select_Charmap(face, ft_encoding_adobe_standard);
        }
    }
    if(rc != 0) {
        fprintf(stderr, "Couldn't select character map: %x.\n", rc);
        return -1;
    }

    font->flags = faceFlags(face) | (symbol ? FACE_SYMBOL : 0);
    font->weight = faceWeight(face);
    font->width = faceWidth(face);
    font->foundry = faceFoundry(face);
    font->italicAngle = faceItalicAngle(face);
    font->pxMetrics.height = face->available_sizes[0].height;
    font->pxMetrics.size = faceIntProp(face, "PIXEL_SIZE");
    font->pxMetrics.xHeight = faceIntProp(face, "X_HEIGHT");
    font->pxMetrics.capHeight = faceIntProp(face, "CAP_HEIGHT");
    font->pxMetrics.ascent = faceIntProp(face, "FONT_ASCENT");
    font->pxMetrics.descent = faceIntProp(face, "FONT_DESCENT");
    font->pxMetrics.underlinePosition = faceIntProp(face, "UNDERLINE_POSITION");
    font->pxMetrics.underlineThickness = faceIntProp(face, "UNDERLINE_THICKNESS");

    for(int i = 0; i < face->num_fixed_sizes; i++) {
        if(verbose_flag)
            fprintf(stderr, "size %d: %dx%d\n",
                    i, 
                    (int)((face->available_sizes[i].x_ppem + 32) >> 6),
                    (int)((face->available_sizes[i].y_ppem + 32) >> 6));

        rc = FT_Set_Pixel_Sizes(face,
                                (face->available_sizes[i].x_ppem + 32) >> 6,
                                (face->available_sizes[i].y_ppem + 32) >> 6);
        if(rc != 0) {
            fprintf(stderr, "Couldn't set size.\n");
            return -1;
        }

        strike = makeStrike(font, 
                            (face->available_sizes[i].x_ppem + 32) >> 6,
                            (face->available_sizes[i].y_ppem + 32) >> 6);
        if(strike == NULL) {
            fprintf(stderr, "Couldn't allocate strike.\n");
            return -1;
        }

        for(j = 0; j < FONT_CODES; j++) {
            if(mapping)
                k = reverse->reverse(j, reverse->data);
            else
                k = j;
            if(k <= 0 && j != 0)
                continue;
            index = FT_Get_Char_Index(face, k);
            if(j != 0 && index == 0)
                continue;
            rc = FT_Load_Glyph(face, index,
                               FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
            if(rc != 0) {
                fprintf(stderr, "Couldn't load glyph for U+%04X (%d)\n",
                        j, index);
                continue;
            }
            bitmap = makeBitmap(strike, j,
                                FT_Pos_UP(face->glyph->metrics.horiAdvance),
                                FT_Pos_DOWN(face->glyph->metrics.horiBearingX),
                                FT_Pos_DOWN(face->glyph->metrics.horiBearingY),
                                face->glyph->bitmap.width,
                                face->glyph->bitmap.rows,
                                face->glyph->bitmap.pitch, 
                                face->glyph->bitmap.buffer,
                                crop_flag);

            if(bitmap == NULL) {
                fprintf(stderr, "Couldn't create bitmap.\n");
                return -1;
            }
        }
    }

    FT_Done_Face(face);
    free(input.stream);

    j = 0;
    for(int i = 0; i < FONT_CODES; i++) {
        int found = 0;
        strike = font->strikes;
        while(strike) {
            bitmap = STRIKE_BITMAP(strike, i);
            if(bitmap) {
                bitmap->index = j;
                found = 1;
            } else {
                if(i == 0) {
                    fprintf(stderr, 
                            "Warning: no bitmap for the undefined glyph.\n");
                    found = 1;
                }
            }
            strike = strike->next;
        }
        if(found)
            j++;
    }
    return 0;
}
