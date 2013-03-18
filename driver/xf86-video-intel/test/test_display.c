#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "test.h"

static Window get_root(struct test_display *t)
{
	XSetWindowAttributes attr;
	Window w;

	/* Be nasty and install a fullscreen window on top so that we
	 * can guarantee we do not get clipped by children.
	 */
	attr.override_redirect = 1;
	w= XCreateWindow(t->dpy, DefaultRootWindow(t->dpy),
			 0, 0, t->width, t->height, 0,
			 DefaultDepth(t->dpy, DefaultScreen(t->dpy)),
			 InputOutput,
			 DefaultVisual(t->dpy, DefaultScreen(t->dpy)),
			 CWOverrideRedirect, &attr);
	XMapWindow(t->dpy, w);

	return w;
}

static Display *real_display(int argc, char **argv)
{
	Display *dpy;
	const char *name = NULL;
	int i;

	for (i = 0; i < argc; i++) {
		if (strncmp(argv[i], "-d", 2) == 0) {
			if (argv[i][2] == '\0') {
				if (i+1 < argc) {
					name = argv[i+1];
					i++;
				}
			} else
				name = argv[i] + 2;
		}
	}

	if (name == NULL)
		name = getenv("DISPLAY");
	if (name == NULL)
		name = ":0"; /* useful default */

	dpy = XOpenDisplay(name);
	if (dpy == NULL)
		die("unable to open real display %s\n", name);

	printf("Opened connection to %s for testing.\n", name);
	return dpy;
}

static Display *ref_display(int width, int height, int depth)
{
	Display *dpy;
	char buf[160];
	const char *name;
	int try;

	name = getenv("REF_DISPLAY");
	if (name) {
		dpy = XOpenDisplay(name);
		if (dpy == NULL)
			die("unable to open reference display %s\n", name);

		printf("Opened connection to %s for reference.\n", name);
		return dpy;
	}

	snprintf(buf, sizeof(buf),
		 "Xvfb -ac -terminate -screen 0 %dx%dx%d :99 >/dev/null 2>&1 &",
		 width, height, depth);
	if (system(buf))
		die("unable to spawn '%s' for reference display\n", buf);

	try = 0;
	while (try++ < 1000) {
		dpy = XOpenDisplay(":99");
		if (dpy)
			break;
		usleep(1000);
	}

	if (dpy == NULL)
		die("unable to open reference display\n");

	return dpy;
}

static void shm_setup(struct test_display *d)
{
	int major, minor, has_pixmaps;
	int size;

	XShmQueryVersion(d->dpy, &major, &minor, &has_pixmaps);
	if (major == 0 && minor == 0)
		die("XSHM not supported\n");

	size = d->width * d->height * 4;
	d->max_shm_size = size;

	d->shm.shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0600);
	if (d->shm.shmid == -1)
		die("failed to allocated %d bytes for a shm segment\n", size);

	d->shm.shmaddr = shmat(d->shm.shmid, NULL, 0);
	d->shm.readOnly = 0;
	XShmAttach(d->dpy, &d->shm);
	XSync(d->dpy, 1);
}

static void default_setup(struct test_display *dpy)
{
	dpy->width = WidthOfScreen(DefaultScreenOfDisplay(dpy->dpy));
	dpy->height = HeightOfScreen(DefaultScreenOfDisplay(dpy->dpy));
	dpy->format =
		XRenderFindVisualFormat(dpy->dpy,
					DefaultVisual(dpy->dpy,
						      DefaultScreen(dpy->dpy)));
}

static void test_get_displays(int argc, char **argv,
			      struct test_display *real,
			      struct test_display *ref)
{
	real->dpy = real_display(argc, argv);
	default_setup(real);
	shm_setup(real);
	real->root = get_root(real);

	ref->dpy = ref_display(real->width, real->height,
			       DefaultDepth(real->dpy, DefaultScreen(real->dpy)));
	default_setup(ref);
	shm_setup(ref);
	ref->root = get_root(ref);
}

void test_init(struct test *test, int argc, char **argv)
{
	memset(test, 0, sizeof(*test));
	test_get_displays(argc, argv, &test->real, &test->ref);
}
