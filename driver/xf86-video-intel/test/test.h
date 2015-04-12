#ifndef TEST_H
#define TEST_H

#include <stdint.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xrender.h>

#define DEFAULT_ITERATIONS 20

enum target {
	ROOT,
	CHILD,
	PIXMAP,
};
#define TARGET_FIRST ROOT
#define TARGET_LAST PIXMAP

enum mask {
	MASK_NONE,
	MASK_NONE_AA,
	MASK_A1,
	MASK_A8,
};

struct test {
	struct test_display {
		Display *dpy;
		Window root;
		XShmSegmentInfo shm;
		int max_shm_size;
		int has_shm_pixmaps;
		int width, height, depth;
		XRenderPictFormat *format;
		enum { REF, OUT } target;
	} out, ref;
};

void die(const char *fmt, ...);

#define die_unless(expr) do{ if (!(expr)) die("verification failed: %s\n", #expr); } while(0)

void test_init(struct test *test, int argc, char **argv);

void test_compare(struct test *out,
		  Drawable out_draw, XRenderPictFormat *out_format,
		  Drawable ref_draw, XRenderPictFormat *ref_format,
		  int x, int y, int w, int h, const char *info);

#define MAX_DELTA 3
int pixel_difference(uint32_t a, uint32_t b);

static inline int pixel_equal(int depth, uint32_t a, uint32_t b)
{
	if (depth != 32) {
		uint32_t mask = (1 << depth) - 1;
		a &= mask;
		b &= mask;
	}

	if (a == b)
		return 1;

	return pixel_difference(a, b) < MAX_DELTA;
}

void
test_init_image(XImage *ximage,
		XShmSegmentInfo *shm,
		XRenderPictFormat *format,
		int width, int height);

const char *test_target_name(enum target target);

struct test_target {
	struct test_display *dpy;
	Drawable draw;
	GC gc;
	XRenderPictFormat *format;
	Picture picture;
	int width, height, depth;
	enum target target;
};

void test_target_create_render(struct test_display *dpy,
			       enum target target,
			       struct test_target *tt);
void test_target_destroy_render(struct test_display *dpy,
				struct test_target *tt);

static inline uint32_t depth_mask(int depth)
{
	if (depth == 32)
		return 0xffffffff;
	else
		return (1 << depth) - 1;
}

static inline uint32_t color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	uint16_t ra = red * alpha;
	uint16_t ga = green * alpha;
	uint16_t ba = blue * alpha;

	return alpha << 24 | ra >> 8 << 16 | ga >> 8 << 8 | ba >> 8;
}

void test_timer_start(struct test_display *t, struct timespec *tv);
double test_timer_stop(struct test_display *t, struct timespec *tv);

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

#define SETS(I) ((I) >= 12 ? 1 : 1 << (12 - (I)))
#define REPS(I) (1 << (I))

#endif
