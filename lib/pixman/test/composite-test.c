#include <stdlib.h>
#include <stdio.h>
#include "pixman.h"
#include "utils.h"

#define WIDTH	100
#define HEIGHT	100

static uint32_t
reader (const void *src, int size)
{
    switch (size)
    {
    case 1:
	return *(uint8_t *)src;
    case 2:
	return *(uint16_t *)src;
    case 4:
	return *(uint32_t *)src;
    default:
	g_assert_not_reached();
    }
}

static void
writer (void *src, uint32_t value, int size)
{
    switch (size)
    {
    case 1:
	*(uint8_t *)src = value;
	break;

    case 2:
	*(uint16_t *)src = value;
	break;

    case 4:
	*(uint32_t *)src = value;
	break;
    }
}

int
main (int argc, char **argv)
{
    uint32_t *src = malloc (WIDTH * HEIGHT * 4);
    uint32_t *dest = malloc (WIDTH * HEIGHT * 4);
    pixman_image_t *src_img;
    pixman_image_t *dest_img;
    int i;

    for (i = 0; i < WIDTH * HEIGHT; ++i)
	src[i] = 0x7f7f0000; /* red */

    for (i = 0; i < WIDTH * HEIGHT; ++i)
	dest[i] = 0x7f00007f; /* blue */
    
    src_img = pixman_image_create_bits (PIXMAN_a8r8g8b8,
					WIDTH, HEIGHT,
					src,
					WIDTH * 4);

    dest_img = pixman_image_create_bits (PIXMAN_a8r8g8b8,
					 WIDTH, HEIGHT,
					 dest,
					 WIDTH * 4);

    pixman_image_set_accessors (src_img, reader, writer);
    pixman_image_set_accessors (dest_img, reader, writer);
    
    pixman_image_composite (PIXMAN_OP_OVER, src_img, NULL, dest_img,
			    0, 0, 0, 0, 0, 0, WIDTH, HEIGHT);

    show_image (dest_img);
    
    pixman_image_unref (src_img);
    pixman_image_unref (dest_img);
    free (src);
    free (dest);
    
    return 0;
}
