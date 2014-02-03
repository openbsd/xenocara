/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"

#include <unistd.h>
#include <pthread.h>
#include <signal.h>

static int max_threads = -1;

static struct thread {
    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    void (*func)(void *arg);
    void *arg;
} *threads;

static void *__run__(void *arg)
{
	struct thread *t = arg;
	sigset_t signals;

	/* Disable all signals in the slave threads as X uses them for IO */
	sigfillset(&signals);
	pthread_sigmask(SIG_BLOCK, &signals, NULL);

	pthread_mutex_lock(&t->mutex);
	while (1) {
		while (t->func == NULL)
			pthread_cond_wait(&t->cond, &t->mutex);
		pthread_mutex_unlock(&t->mutex);

		assert(t->func);
		t->func(t->arg);

		pthread_mutex_lock(&t->mutex);
		t->func = NULL;
		pthread_cond_signal(&t->cond);
	}
	pthread_mutex_unlock(&t->mutex);

	return NULL;
}

#if defined(__GNUC__)
#define popcount(x) __builtin_popcount(x)
#else
static int popcount(unsigned int x)
{
	int count = 0;

	while (x) {
		count += x&1;
		x >>= 1;
	}

	return count;
}
#endif

static int
num_cores(void)
{
	FILE *file = fopen("/proc/cpuinfo", "r");
	int count = 0;
	if (file) {
		size_t len = 0;
		char *line = NULL;
		uint32_t processors = 0, cores = 0;
		while (getline(&line, &len, file) != -1) {
			int id;
			if (sscanf(line, "physical id : %d", &id) == 1) {
				if (id >= 32)
					continue;
				processors |= 1 << id;
			} else if (sscanf(line, "core id : %d", &id) == 1) {
				if (id >= 32)
					continue;
				cores |= 1 << id;
			}
		}
		free(line);
		fclose(file);

		DBG(("%s: processors=0x%08x, cores=0x%08x\n",
		     __FUNCTION__, processors, cores));

		count = popcount(processors) * popcount(cores);
	}
	return count;
}

void sna_threads_init(void)
{
	int n;

	if (max_threads != -1)
		return;

	max_threads = num_cores();
	if (max_threads == 0)
		max_threads = sysconf(_SC_NPROCESSORS_ONLN) / 2;
	if (max_threads <= 1)
		goto bail;

	DBG(("%s: creating a thread pool of %d threads\n",
	     __func__, max_threads));

	threads = malloc (sizeof(threads[0])*max_threads);
	if (threads == NULL)
		goto bail;

	for (n = 0; n < max_threads; n++) {
		pthread_mutex_init(&threads[n].mutex, NULL);
		pthread_cond_init(&threads[n].cond, NULL);

		threads[n].func = NULL;
		if (pthread_create(&threads[n].thread, NULL,
				   __run__, &threads[n]))
			goto bail;
	}

	return;

bail:
	max_threads = 0;
}

void sna_threads_run(void (*func)(void *arg), void *arg)
{
	int n;

	assert(max_threads > 0);

	for (n = 0; n < max_threads; n++) {
		if (threads[n].func)
			continue;

		pthread_mutex_lock(&threads[n].mutex);
		if (threads[n].func) {
			pthread_mutex_unlock(&threads[n].mutex);
			continue;
		}

		goto execute;
	}

	n = rand() % max_threads;
	pthread_mutex_lock(&threads[n].mutex);
	while (threads[n].func)
		pthread_cond_wait(&threads[n].cond, &threads[n].mutex);

execute:
	threads[n].func = func;
	threads[n].arg = arg;
	pthread_cond_signal(&threads[n].cond);
	pthread_mutex_unlock(&threads[n].mutex);
}

void sna_threads_wait(void)
{
	int n;

	assert(max_threads > 0);

	for (n = 0; n < max_threads; n++) {
		if (threads[n].func == NULL)
			continue;

		pthread_mutex_lock(&threads[n].mutex);
		while (threads[n].func)
			pthread_cond_wait(&threads[n].cond, &threads[n].mutex);
		pthread_mutex_unlock(&threads[n].mutex);
	}
}

int sna_use_threads(int width, int height, int threshold)
{
	int num_threads;

	if (max_threads <= 0)
		return 1;

	if (width < 128)
		height /= 128/width;

	num_threads = height * max_threads / threshold - 1;
	if (num_threads <= 0)
		return 1;

	if (num_threads > max_threads)
		num_threads = max_threads;
	return num_threads;
}

struct thread_composite {
	pixman_image_t *src, *mask, *dst;
	pixman_op_t op;
	int16_t src_x, src_y;
	int16_t mask_x, mask_y;
	int16_t dst_x, dst_y;
	uint16_t width, height;
};

static void thread_composite(void *arg)
{
	struct thread_composite *t = arg;
	pixman_image_composite(t->op, t->src, t->mask, t->dst,
			       t->src_x, t->src_y,
			       t->mask_x, t->mask_y,
			       t->dst_x, t->dst_y,
			       t->width, t->height);
}

void sna_image_composite(pixman_op_t        op,
			 pixman_image_t    *src,
			 pixman_image_t    *mask,
			 pixman_image_t    *dst,
			 int16_t            src_x,
			 int16_t            src_y,
			 int16_t            mask_x,
			 int16_t            mask_y,
			 int16_t            dst_x,
			 int16_t            dst_y,
			 uint16_t           width,
			 uint16_t           height)
{
	int num_threads;

	num_threads = sna_use_threads(width, height, 32);
	if (num_threads <= 1) {
		pixman_image_composite(op, src, mask, dst,
				       src_x, src_y,
				       mask_x, mask_y,
				       dst_x, dst_y,
				       width, height);
	} else {
		struct thread_composite data[num_threads];
		int y, dy, n;

		DBG(("%s: using %d threads for compositing %dx%d\n",
		     __FUNCTION__, num_threads, width, height));

		y = dst_y;
		dy = (height + num_threads - 1) / num_threads;
		num_threads -= (num_threads-1) * dy >= height;

		data[0].op = op;
		data[0].src = src;
		data[0].mask = mask;
		data[0].dst = dst;
		data[0].src_x = src_x;
		data[0].src_y = src_y;
		data[0].mask_x = mask_x;
		data[0].mask_y = mask_y;
		data[0].dst_x = dst_x;
		data[0].dst_y = y;
		data[0].width = width;
		data[0].height = dy;

		for (n = 1; n < num_threads; n++) {
			data[n] = data[0];
			data[n].src_y += y - dst_y;
			data[n].mask_y += y - dst_y;
			data[n].dst_y = y;
			y += dy;

			sna_threads_run(thread_composite, &data[n]);
		}

		assert(y < dst_y + height);
		if (y + dy > dst_y + height)
			dy = dst_y + height - y;

		data[0].src_y += y - dst_y;
		data[0].mask_y += y - dst_y;
		data[0].dst_y = y;
		data[0].height = dy;

		thread_composite(&data[0]);

		sna_threads_wait();
	}
}
