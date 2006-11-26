#if !defined( lint ) && !defined( SABER )
static const char sccsid[] = "@(#)magick.c     5.00 00/10/26 xlockmore";

#endif

/*-
 * Utilities for Reading images using ImageMagick
 *
 * Copyright (c) 2000 by J.Jansen (joukj@hrem.nano.tudelft.nl)
 *
 * See xlock.c for copying information.
 *
 * Revision History:
 * 26-Oct-00: Created
 * 15-Mar-06: Corrected for other QuantumDepth than 8
 * 04-Apr-06: Corrected for TrueColor visuals
 */

#include "xlock.h"

#ifdef USE_MAGICK
#undef inline
#include "magick.h"

XLockImage xlockimage;

int
MagickFileToImage(ModeInfo * mi, char *filename, XImage ** image ,
		  Colormap m_cmap )
{
   Image* imageData = (Image*) NULL;
   ImageInfo imageInfo;
   ExceptionInfo exception;
   PixelPacket* pixdata;
   IndexPacket* tmpdata0;
   int i, fuzz, depth, format;
   unsigned long black, white, fgpix, bgpix;
   XColor      fgcol, bgcol;
   unsigned long* pixel_num;

   if ( MI_IS_VERBOSE( mi ) )
     {
	(void) printf ( "Reading image %s\n", filename );
     }

   /* Read image data from file */
/* PURIFY reports a memory leak on the next line */
   GetImageInfo( &imageInfo );
   imageInfo.dither = 0;
   (void) strcpy( imageInfo.filename , filename );
   GetExceptionInfo( &exception );
   imageData = ReadImage ( &imageInfo, &exception );
   if ( imageData == (Image*) NULL )
     {
	(void) fprintf ( stderr , "Error reading image %s\n", imageInfo.filename );
	return MagickFileInvalid;
     }

   /* setup X-image */
   xlockimage.width = imageData->columns;
   xlockimage.height = imageData->rows;
   xlockimage.depth = imageData->depth;
   if ( MI_IS_VERBOSE( mi ) )
     {
	(void) printf ( "Image dimensions %d x %d, depth = %d\n",
		       xlockimage.width, xlockimage.height, xlockimage.depth );
     }

   depth = MI_DEPTH(mi);
   if ( depth < 9 )
     xlockimage.data = (unsigned char *) malloc((int) ( xlockimage.width *
							xlockimage.height));
   else
     xlockimage.data = (unsigned char *) malloc((int) ( xlockimage.width *
							xlockimage.height *
							4 ));
   if (!xlockimage.data)
     {
	(void) fprintf(stderr, "out of memory for Image file\n");
	return MagickNoMemory;
     }

   format=(depth == 1) ? XYBitmap : ZPixmap;
   if ( MI_IS_VERBOSE( mi ) )
	(void) printf ( "XCreateImage depth = %d\n", depth );
   if ( depth < 9 )
     *image = XCreateImage( MI_DISPLAY(mi), MI_VISUAL(mi), depth, format, 0,
			 (char *) xlockimage.data, (int) xlockimage.width,
			 (int) xlockimage.height, 16, (int) xlockimage.width );
   else
     *image = XCreateImage( MI_DISPLAY(mi), MI_VISUAL(mi), depth, format, 0,
			 (char *) xlockimage.data, (int) xlockimage.width,
			 (int) xlockimage.height, 16, (int) xlockimage.width*4 );
     
   if (!*image)
     {
	(void) fprintf(stderr, "could not create image from file\n");
	return MagickColorError;
     }

   if ( imageData->colors == 0 )
     {
	PixelPacket* pixtmp;

/* PURIFY reports a memory leak on the next line */
	AllocateImageColormap ( imageData , COLORMAP_SIZE );
	pixdata = GetImagePixels( imageData , 0 , 0 , imageData->columns ,
					     imageData->rows );
	imageData->colors = imageData->columns*imageData->rows;
	tmpdata0 = (IndexPacket *) malloc((int) ( xlockimage.width *
						  xlockimage.height *
						  sizeof(IndexPacket)));

#if (QuantumDepth == 8)
	for ( fuzz=0; imageData->colors > COLORMAP_SIZE; fuzz++ )
#elif (QuantumDepth == 16)
	for ( fuzz=0; imageData->colors > COLORMAP_SIZE; fuzz+=256 )
#elif (QuantumDepth == 32)
	for ( fuzz=0; imageData->colors > COLORMAP_SIZE; fuzz+=65536 )
#elif (QuantumDepth == 64)
	for ( fuzz=0; imageData->colors > COLORMAP_SIZE; fuzz+=4294967296 )
#else
#error "ImageMagick configuration error"
#endif
	  {
	     int num_col , j , x , y;

	     pixtmp = pixdata;
	     num_col = 0;
	     for ( i=0; i<imageData->columns*imageData->rows; i++)
	       {
		  PixelPacket* pixtmp2;

		  pixtmp2 = imageData->colormap;
		  for ( j=0; j<num_col; j++ )
		    {
		       if ( ABS( pixtmp->red - pixtmp2->red ) +
			 ABS( pixtmp->green - pixtmp2->green ) +
			 ABS( pixtmp->blue - pixtmp2->blue ) < fuzz )
			 break;
		       pixtmp2++;
		    }
		  if ( j == num_col )
		    {
		       pixtmp2->red = pixtmp->red;
		       pixtmp2->green = pixtmp->green;
		       pixtmp2->blue = pixtmp->blue;
		       num_col++;
		       if ( num_col == COLORMAP_SIZE )
			 {
			    num_col++;
			    break;
			 }
		    }
		  tmpdata0[ i ] = j;
		  pixtmp++;
	       }
	     imageData->colors = num_col;
	  }
     }
   else
     {
	if ( imageData->colors > COLORMAP_SIZE )
	  {
	     imageData->colors = COLORMAP_SIZE;
	     (void) fprintf( stderr ,
			    "wrong colour reducing algorithm used\n" );
	     (void) fprintf( stderr ,
	     "Please notify the maintainer that this statement is reached\n" );
	  }
   /* get pixel information */
   if ( !( pixdata = GetImagePixels( imageData , 0 , 0 , imageData->columns ,
				    imageData->rows ) ) )
     {
	(void) fprintf( stderr , "Error getting pixels\n" );
	return MagickFileInvalid;
     }
	tmpdata0 = GetIndexes ( imageData );
     }

   /*set up colourmap */
	black = MI_BLACK_PIXEL(mi);
	white = MI_WHITE_PIXEL(mi);
	fgpix = MI_FG_PIXEL(mi);
	bgpix = MI_BG_PIXEL(mi);
	fgcol.pixel = fgpix;
	bgcol.pixel = bgpix;
	XQueryColor(MI_DISPLAY(mi), MI_COLORMAP(mi), &fgcol);
	XQueryColor(MI_DISPLAY(mi), MI_COLORMAP(mi), &bgcol);

	/* Set these, if Image does not overwrite some, so much the better. */
	if (fgpix < COLORMAP_SIZE) {
		xlockimage.red[fgpix] = fgcol.red >> 8;
		xlockimage.green[fgpix] = fgcol.green >> 8;
		xlockimage.blue[fgpix] = fgcol.blue >> 8;
	}
	if (bgpix < COLORMAP_SIZE) {
		xlockimage.red[bgpix] = bgcol.red >> 8;
		xlockimage.green[bgpix] = bgcol.green >> 8;
		xlockimage.blue[bgpix] = bgcol.blue >> 8;
	}
	if (white < COLORMAP_SIZE) {
		xlockimage.red[white] = 0xFF;
		xlockimage.green[white] = 0xFF;
		xlockimage.blue[white] = 0xFF;
	}
	if (black < COLORMAP_SIZE) {
		xlockimage.red[black] = 0;
		xlockimage.green[black] = 0;
		xlockimage.blue[black] = 0;
	}
   /* supply data and colours*/
	xlockimage.colors = imageData->colors;
	pixel_num = malloc( imageData->colors * sizeof( unsigned long ) );
	for ( i=0 ; i<xlockimage.colors ; i++ )
	  {
	     XColor Xcol;
	     
#if (QuantumDepth == 8)
	     xlockimage.red[ i ] =  imageData->colormap->red * 257;
	     xlockimage.green[ i ] = imageData->colormap->green *257;
	     xlockimage.blue[ i ] = imageData->colormap->blue * 257;
#elif (QuantumDepth == 16)
	     xlockimage.red[ i ] =  imageData->colormap->red;
	     xlockimage.green[ i ] = imageData->colormap->green;
	     xlockimage.blue[ i ] = imageData->colormap->blue;
#elif (QuantumDepth == 32)
	     xlockimage.red[ i ] =  imageData->colormap->red / 256;
	     xlockimage.green[ i ] = imageData->colormap->green / 256;
	     xlockimage.blue[ i ] = imageData->colormap->blue/ 256;
#elif (QuantumDepth == 64)
	     xlockimage.red[ i ] =  imageData->colormap->red / 65537;
	     xlockimage.green[ i ] = imageData->colormap->green / 65537;
	     xlockimage.blue[ i ] = imageData->colormap->blue/ 65537;
#else
#error "ImageMagick configuration error"
#endif
	     Xcol.red = xlockimage.red[ i ];
	     Xcol.green = xlockimage.green[ i ];
	     Xcol.blue = xlockimage.blue[ i ];
	     XAllocColor( MI_DISPLAY(mi) , m_cmap , &Xcol);
	     pixel_num[i] = Xcol.pixel;
	     imageData->colormap++;
	  }

   if ( MI_IS_VERBOSE( mi ) )
     {
	(void) printf ( "xlockimage.colors %d\n", xlockimage.colors );
     }
   /* Make sure there is a black ... */
   if (xlockimage.colors <= 0xff)
     xlockimage.red[0xff] = xlockimage.green[0xff] = xlockimage.blue[0xff] = 0;

	  {
	     IndexPacket* tmpdata;
	     int x , y;

	     tmpdata = tmpdata0;
	     for ( y=0; y<imageData->rows; y++ )
	       {
		  for ( x=0; x<imageData->columns; x++)
	       {
		  XPutPixel( *image , x , y , pixel_num[ tmpdata[ 0 ] ] );
		  tmpdata++;
	       }
	       }
	     free( tmpdata0 );
	     free( pixel_num );
	  }

   /* clean up */
   DestroyImage( imageData );

   return MagickSuccess;
}

#endif /* USE_MAGICK */
