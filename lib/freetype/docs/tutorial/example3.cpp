// example3.cpp

// This file demonstrates how to use FreeType's stand-alone renderer,
// both in B/W and 5-levels gray mode.
//
// Copy the files ftraster.c, ftimage.h, and ftmisc.h into the same
// directory as this file, then say
//
//   g++ -D_STANDALONE_ \
//       -DFT_RASTER_OPTION_ANTI_ALIASING \
//       -o example3 example3.cpp
//
// You need FreeType version 2.3.10 or newer.
//
// Written Sep. 2009 by Werner Lemberg,
// based on code contributed by Erik Möller.
//
// Public domain.

#include "ftraster.c"
#include <fstream>

// Define an acorn-like shape to test with.
struct Vec2
{
  Vec2(float a, float b) : x(a), y(b) { }

  float x, y;
};

static Vec2 k_shape[] =
{ Vec2(-3, -18), Vec2(0, -12), Vec2(6, -10), Vec2(12, -6), Vec2(12, -4),
  Vec2(11, -4), Vec2(10, -5), Vec2(10, 1), Vec2(9, 6), Vec2(7, 10),
  Vec2(5, 12), Vec2(4, 15), Vec2(3, 14), Vec2(1, 13), Vec2(-1, 13),
  Vec2(-5, 11), Vec2(-8, 8), Vec2(-11, 2), Vec2(-11, -2), Vec2(-14, 0),
  Vec2(-14, -2), Vec2(-11, -7), Vec2(-9, -9), Vec2(-8, -9), Vec2(-5, -12),
  Vec2(-5, -14), Vec2(-7, -15), Vec2(-8, -14), Vec2(-9, -15), Vec2(-9, -17),
  Vec2(-7, -17), Vec2(-6, -18)
};

void*
MY_Alloc_Func(FT_Memory memory,
              long size)
{
  return malloc(size);
}

void
MY_Free_Func(FT_Memory memory,
             void *block)
{
  free(block);
}

void*
MY_Realloc_Func(FT_Memory memory,
                long cur_size,
                long new_size,
                void* block)
{
  return realloc(block, new_size);
}


FT_Memory mem;


// Render a shape and dump it out as out-mono.pbm (b/w) and
// out-gray.pgm (grayscale).
int
main()
{
  // Set up the memory management.
  mem = new FT_MemoryRec;
  mem->alloc = MY_Alloc_Func;
  mem->free = MY_Free_Func;
  mem->realloc = MY_Realloc_Func;


  // Build an outline manually.
  FT_Outline_ outline;
  outline.n_contours = 1;
  outline.n_points = sizeof (k_shape) / sizeof (Vec2);
  outline.points = new FT_Vector[outline.n_points];
  for (int i = 0; i < outline.n_points; ++i)
  {
    FT_Vector v;
    // Offset it to fit in the image and scale it up 10 times.
    v.x = (20 + k_shape[i].x) * 10 * 64;
    v.y = (20 + k_shape[i].y) * 10 * 64;
    outline.points[i] = v;
  }
  outline.tags = new char[outline.n_points];
  for (int i = 0; i < outline.n_points; ++i)
    outline.tags[i] = 1;
  outline.contours = new short[outline.n_contours];
  outline.contours[0] = outline.n_points - 1;
  outline.flags = 0;


  const int width = 500;
  const int rows = 400;

  // 1 bit per pixel.
  const int pitch_mono = (width + 7) >> 3;
  // 8 bits per pixel; must be a multiple of four.
  const int pitch_gray = (width + 3) & -4;


  FT_Bitmap bmp;
  FT_Raster raster;
  FT_Raster_Params params;


  // Allocate a chunk of memory for the render pool.
  const int kRenderPoolSize = 1024 * 1024;
  unsigned char *renderPool = new unsigned char[kRenderPoolSize];


  // Set up a bitmap.
  bmp.buffer = new unsigned char[rows * pitch_mono];
  memset(bmp.buffer, 0, rows * pitch_mono);
  bmp.width = width;
  bmp.rows = rows;
  bmp.pitch = pitch_mono;
  bmp.pixel_mode = FT_PIXEL_MODE_MONO;

  // Set up the necessary raster parameters.
  memset(&params, 0, sizeof (params));
  params.source = &outline;
  params.target = &bmp;

  // Initialize the rasterer and get it to render into the bitmap.
  ft_standard_raster.raster_new(mem, &raster);
  ft_standard_raster.raster_reset(raster, renderPool, kRenderPoolSize);
  ft_standard_raster.raster_render(raster, &params);

  // Dump out the raw image data (in PBM format).
  std::ofstream out_mono("out-mono.pbm", std::ios::binary);
  out_mono << "P4 " << width << " " << rows << "\n";
  out_mono.write((const char *)bmp.buffer, rows * pitch_mono);

  delete[] bmp.buffer;


  // Set up a pixmap.
  bmp.buffer = new unsigned char[rows * pitch_gray];
  memset(bmp.buffer, 0, rows * pitch_gray);
  bmp.width = width;
  bmp.rows = rows;
  bmp.pitch = pitch_gray;
  bmp.pixel_mode = FT_PIXEL_MODE_GRAY;
  bmp.num_grays = 16;

  // Set up the necessary raster parameters.
  memset(&params, 0, sizeof (params));
  params.source = &outline;
  params.target = &bmp;
  params.flags = FT_RASTER_FLAG_AA;

  // Initialize the rasterer and get it to render into the pixmap.
  ft_standard_raster.raster_new(mem, &raster);
  ft_standard_raster.raster_reset(raster, renderPool, kRenderPoolSize);
  ft_standard_raster.raster_render(raster, &params);

  // Dump out the raw image data (in PBM format).
  std::ofstream out_gray("out-gray.pgm", std::ios::binary);
  out_gray << "P5 " << width << " " << rows << " 255\n";
  out_gray.write((const char *)bmp.buffer, rows * pitch_gray);


  // Cleanup.
  delete[] renderPool;
  delete[] bmp.buffer;
  delete[] outline.points;
  delete[] outline.tags;
  delete[] outline.contours;
  delete mem;

  return 0;
}
