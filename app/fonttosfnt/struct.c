/*
Copyright (c) 2002-2003 by Juliusz Chroboczek

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
/* $XFree86: xc/programs/fonttosfnt/struct.c,v 1.3 2003/10/24 20:38:11 tsi Exp $ */

#include <stdlib.h>
#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "fonttosfnt.h"

FontPtr
makeFont(void)
{
    FontPtr font;

    font = malloc(sizeof(FontRec));
    if(font == NULL)
        return NULL;

    *font = (FontRec) {
        .numNames = 0,
        .names = NULL,
        .flags = 0,
        .weight = 500,
        .width = 5,
        .italicAngle = 0,
        .pxMetrics.height = UNDEF,
        .pxMetrics.maxX = UNDEF,
        .pxMetrics.minX = UNDEF,
        .pxMetrics.maxY = UNDEF,
        .pxMetrics.minY = UNDEF,
        .pxMetrics.xHeight = UNDEF,
        .pxMetrics.capHeight = UNDEF,
        .pxMetrics.maxAwidth = UNDEF,
        .pxMetrics.awidth = UNDEF,
        .pxMetrics.ascent = UNDEF,
        .pxMetrics.descent = UNDEF,
        .pxMetrics.underlinePosition = UNDEF,
        .pxMetrics.underlineThickness = UNDEF,
        .metrics.height = UNDEF,
        .metrics.maxX = UNDEF,
        .metrics.minX = UNDEF,
        .metrics.maxY = UNDEF,
        .metrics.minY = UNDEF,
        .metrics.xHeight = UNDEF,
        .metrics.capHeight = UNDEF,
        .metrics.maxAwidth = UNDEF,
        .metrics.awidth = UNDEF,
        .metrics.ascent = UNDEF,
        .metrics.descent = UNDEF,
        .metrics.underlinePosition = UNDEF,
        .metrics.underlineThickness = UNDEF,
        .foundry = makeName("UNKN"),
        .strikes = NULL,
    };
    return font;
}

StrikePtr
makeStrike(FontPtr font, int sizeX, int sizeY)
{
    StrikePtr strike, last_strike;

    strike = font->strikes;
    last_strike = NULL;
    while(strike) {
        if(strike->sizeX == sizeX && strike->sizeY == sizeY)
            return strike;
        last_strike = strike;
        strike = strike->next;
    }

    strike = malloc(sizeof(StrikeRec));
    if(strike == NULL)
        return NULL;
    else {
        BitmapPtr **bitmaps =
            calloc(FONT_CODES / FONT_SEGMENT_SIZE, sizeof(BitmapPtr*));
        if (bitmaps == NULL) {
            free(strike);
            return NULL;
        }
        *strike = (StrikeRec) {
            .sizeX = sizeX,
            .sizeY = sizeY,
            .bitmaps = bitmaps,
            .numSbits = 0,
            .next = NULL,
            .bitmapSizeTableLocation = 0xDEADFACE,
            .indexSubTables = NULL,
        };
        if (last_strike)
            last_strike->next = strike;
        else
            font->strikes = strike;
    }
    return strike;
}

BitmapPtr
makeBitmap(StrikePtr strike, int code,
           int advanceWidth, int horiBearingX, int horiBearingY,
           int width, int height, int stride,
           const unsigned char *raster, int crop)
{
    BitmapPtr bitmap;
    int i, j, x, y;
    int dx, dy, new_width, new_height;

    bitmap = malloc(sizeof(BitmapRec));
    if(bitmap == NULL) 
        return NULL;

    *bitmap = (BitmapRec) {
        .index = -1,
        .width = 0,
        .height = 0,
        .stride = 0,
        .raster = NULL,
        .location = 0xDEADFACE,
    };

    i = code / FONT_SEGMENT_SIZE;
    j = code % FONT_SEGMENT_SIZE;

    if(strike->bitmaps[i] == NULL) {
        strike->bitmaps[i] = calloc(FONT_SEGMENT_SIZE, sizeof(BitmapPtr));
    }
    if(strike->bitmaps[i] == NULL) {
        free(bitmap);
        return NULL;
    }
    if(strike->bitmaps[i][j] != NULL) {
        if(verbose_flag)
            fprintf(stderr, "Duplicate bitmap %d.\n", code);
        free(bitmap);
        return strike->bitmaps[i][j];
    }

    dx = 0;
    dy = 0;
    new_width = width;
    new_height = height;

    if(crop) {
        int empty;
        while(new_width > 0) {
            empty = 1;
            x = new_width - 1;
            for(y = 0; y < new_height; y++) {
                if(BITREF(raster, stride, x + dx, y + dy)) {
                    empty = 0;
                    break;
                }
            }
            if(empty)
                new_width--;
            else
                break;
        }
        while(new_height > 0) {
            empty = 1;
            y = new_height - 1;
            for(x = 0; x < new_width; x++) {
                if(BITREF(raster, stride, x + dx, y + dy)) {
                    empty = 0;
                    break;
                }
            }
            if(empty)
                new_height--;
            else
                break;
        }
        while(new_width > 0) {
            empty = 1;
            x = 0;
            for(y = 0; y < new_height; y++) {
                if(BITREF(raster, stride, x + dx, y + dy)) {
                    empty = 0;
                    break;
                }
            }
            if(empty) {
                dx++;
                new_width--;
            } else
                break;
        }
        while(new_height > 0) {
            empty = 1;
            y = 0;
            for(x = 0; x < new_width; x++) {
                if(BITREF(raster, stride, x + dx, y + dy)) {
                    empty = 0;
                    break;
                }
            }
            if(empty) {
                dy++;
                new_height--;
            } else
                break;
        }
    }


    bitmap->advanceWidth = advanceWidth;
    bitmap->horiBearingX = horiBearingX + dx;
    bitmap->horiBearingY = horiBearingY - dy;
    bitmap->width = new_width;
    bitmap->height = new_height;
    bitmap->stride = (new_width + 7) / 8;

    bitmap->raster = malloc(bitmap->height * bitmap->stride);
    if(bitmap->raster == NULL) {
        free(bitmap);
        return NULL;
    }
    memset(bitmap->raster, 0, bitmap->height * bitmap->stride);
    for(y = 0; y < new_height; y++) {
        for(x = 0; x < new_width; x++) {
            if(BITREF(raster, stride, x + dx, y + dy))
                bitmap->raster[y * bitmap->stride + x / 8] |=
                    1 << (7 - (x % 8));
        }
    }
    strike->bitmaps[i][j] = bitmap;
    strike->numSbits++;

    return bitmap;
}

IndexSubTablePtr
makeIndexSubTables(StrikePtr strike, CmapPtr cmap)
{
    IndexSubTablePtr first, last;
    int index;

    first = NULL;
    last = NULL;

    /* Assuming that we're writing bit-aligned data, small metrics
       and short offsets, a constant metrics segment saves 5 bytes 
       per glyph in the EBDT table, and 2 bytes per glyph in the EBLC
       table.  On the other hand, the overhead for a supplementary
       type 2 indexSubTable is 8 bytes for the indexSubTableArray
       entry and 20 bytes for the subtable itself.  It's worth
       splitting at 5 glyphs.  There's no analogue of a type 2
       indexSubTable with byte-aligned data, so we don't bother
       splitting when byte-aligning. */
    index = 0;
    while(index < 0xFFFF) {
        int constantMetrics = 1;
        int n;
        IndexSubTablePtr table;
        BitmapPtr bitmap0, bitmap;

        bitmap0 = strikeBitmapIndex(strike, cmap, index);
        if(bitmap0 == NULL) {
            index++;
            continue;
        }
        n = 1;
        while((bitmap = strikeBitmapIndex(strike, cmap, index + n)) != NULL) {
            if(constantMetrics) {
                if(!SAME_METRICS(bitmap0, bitmap)) {
                    if(bit_aligned_flag && n >= 4)
                        break;
                    else
                        constantMetrics = 0;
                }
            } else if(bit_aligned_flag) {
                BitmapPtr b1 = strikeBitmapIndex(strike, cmap, index + n + 1);
                BitmapPtr b2 = strikeBitmapIndex(strike, cmap, index + n + 2);
                BitmapPtr b3 = strikeBitmapIndex(strike, cmap, index + n + 3);
                BitmapPtr b4 = strikeBitmapIndex(strike, cmap, index + n + 4);
                if(b1 && b2 && b3 && b4 &&
                   SAME_METRICS(bitmap, b1) && 
                   SAME_METRICS(bitmap, b2) && 
                   SAME_METRICS(bitmap, b3) && 
                   SAME_METRICS(bitmap, b4)) {
                    break;
                }
            }
            n++;
        }
        if(n <= 1)
            constantMetrics = 0;

        table = malloc(sizeof(IndexSubTableRec));
        *table = (IndexSubTableRec) {
            .firstGlyphIndex = index,
            .lastGlyphIndex = index + n - 1,
            .constantMetrics = constantMetrics,
            .location = 0xDEADFACE,
            .lastLocation = 0xDEADFACE,
            .next = NULL,
        };

        if(first == NULL) {
            first = table;
            last = table;
        } else {
            last->next = table;
            last = table;
        }
        index += n;
    }
    return first;
}

int
fontIndex(FontPtr font, int code)
{
    StrikePtr strike;

    if(code == 0)
        return 0;
    strike = font->strikes;
    while(strike) {
        BitmapPtr bitmap = STRIKE_BITMAP(strike, code);
        if(bitmap)
            return bitmap->index;
        strike = strike->next;
    }
    return -1;
}

CmapPtr
makeCmap(FontPtr font)
{
    CmapPtr cmap_head = NULL;
    CmapPtr cmap_last = NULL;
    CmapPtr cmap;
    int code, maxindex = 0;

    code = 0;
    while(code < FONT_CODES) {
        int i;
        int index = fontIndex(font, code);
        if(index < 0) {
            code++;
            continue;
        }
        i = 1;
        while(code + i < FONT_CODES && 
              fontIndex(font, code + i) == index + i) {
            i++;
        }
        cmap = malloc(sizeof(CmapRec));
        if(cmap == NULL)
            return NULL;
        *cmap = (CmapRec) {
            .startCode = code,
            .endCode = code + i - 1,
            .index = index,
            .next = NULL,
            .maxindex = 0,
        };
        if(maxindex < index + i - 1)
            maxindex = index + i - 1;
        if(cmap_head == NULL)
            cmap_head = cmap;
        else
            cmap_last->next = cmap;
        cmap_last = cmap;

        code += i;
    }
    cmap_head->maxindex = maxindex;
    cmap_head->inverse = calloc(maxindex + 1, sizeof(int));
    cmap = cmap_head;
    while(cmap) {
        for(int i = cmap->index;
            i <= cmap->endCode - cmap->startCode + cmap->index; i++) {
            cmap_head->inverse[i] =
                i - cmap->index + cmap->startCode;
        }
        cmap = cmap->next;
    }

    return cmap_head;
}

int
findCode(CmapPtr cmap_head, int index)
{
    if(index < 0 || index > cmap_head->maxindex)
        return -1;
    return cmap_head->inverse[index];

}

int
maxIndex(CmapPtr cmap_head)
{
    return cmap_head->maxindex;
}

BitmapPtr
strikeBitmapIndex(StrikePtr strike, CmapPtr cmap, int index)
{
    int code = findCode(cmap, index);
    if(code < 0)
        return NULL;

    return STRIKE_BITMAP(strike, code);
}

int
strikeMaxWidth(StrikePtr strike)
{
    int width_max = 0;

    for(int i = 0; i < FONT_CODES; i++) {
        BitmapPtr bitmap = STRIKE_BITMAP(strike, i);
        if(!bitmap)
            continue;
        if(bitmap->advanceWidth > width_max)
            width_max = bitmap->advanceWidth;
    }

    return width_max;
}

int
glyphMetrics(FontPtr font, int code,
             int *width_return,
             int *x_min_return, int *y_min_return,
             int *x_max_return, int *y_max_return)
{
    StrikePtr strike;

    strike = font->strikes;
    while(strike) {
        BitmapPtr bitmap = STRIKE_BITMAP(strike, code);
        if(bitmap) {
            if(width_return)
                *width_return = 
                    (((float)bitmap->advanceWidth) / strike->sizeX) *
                    TWO_SIXTEENTH;
            if(x_min_return)
                *x_min_return =
                    ((float)bitmap->horiBearingX / strike->sizeX) * 
                    TWO_SIXTEENTH;
            if(y_min_return)
                *y_min_return =
                    (((float)bitmap->horiBearingY - bitmap->height) 
                     / strike->sizeY) * TWO_SIXTEENTH;
            if(x_max_return)
                *x_max_return =
                    (((float)bitmap->horiBearingX + bitmap->width)
                     / strike->sizeX) * TWO_SIXTEENTH;
            if(y_max_return)
                *y_max_return =
                    (((float)bitmap->horiBearingY) / strike->sizeY) *
                    TWO_SIXTEENTH;
            return 1;
        }
        strike = strike->next;
    }

    return -1;
}
