/*
 * Copyright © 2007 Intel Corporation
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <drm.h>
#include <i915_drm.h>
#include "intel_bufmgr.h"
#include "intel_bufmgr_priv.h"

/** @file intel_bufmgr.c
 *
 * Convenience functions for buffer management methods.
 */

drm_intel_bo *
drm_intel_bo_alloc(drm_intel_bufmgr *bufmgr, const char *name,
		   unsigned long size, unsigned int alignment)
{
   return bufmgr->bo_alloc(bufmgr, name, size, alignment);
}

void
drm_intel_bo_reference(drm_intel_bo *bo)
{
   bo->bufmgr->bo_reference(bo);
}

void
drm_intel_bo_unreference(drm_intel_bo *bo)
{
   if (bo == NULL)
      return;

   bo->bufmgr->bo_unreference(bo);
}

int
drm_intel_bo_map(drm_intel_bo *buf, int write_enable)
{
   return buf->bufmgr->bo_map(buf, write_enable);
}

int
drm_intel_bo_unmap(drm_intel_bo *buf)
{
   return buf->bufmgr->bo_unmap(buf);
}

int
drm_intel_bo_subdata(drm_intel_bo *bo, unsigned long offset,
		     unsigned long size, const void *data)
{
   int ret;

   if (bo->bufmgr->bo_subdata)
      return bo->bufmgr->bo_subdata(bo, offset, size, data);
   if (size == 0 || data == NULL)
      return 0;

   ret = drm_intel_bo_map(bo, 1);
   if (ret)
       return ret;
   memcpy((unsigned char *)bo->virtual + offset, data, size);
   drm_intel_bo_unmap(bo);
   return 0;
}

int
drm_intel_bo_get_subdata(drm_intel_bo *bo, unsigned long offset,
			 unsigned long size, void *data)
{
   int ret;
   if (bo->bufmgr->bo_subdata)
      return bo->bufmgr->bo_get_subdata(bo, offset, size, data);

   if (size == 0 || data == NULL)
      return 0;

   ret = drm_intel_bo_map(bo, 0);
   if (ret)
       return ret;
   memcpy(data, (unsigned char *)bo->virtual + offset, size);
   drm_intel_bo_unmap(bo);
   return 0;
}

void
drm_intel_bo_wait_rendering(drm_intel_bo *bo)
{
   bo->bufmgr->bo_wait_rendering(bo);
}

void
drm_intel_bufmgr_destroy(drm_intel_bufmgr *bufmgr)
{
   bufmgr->destroy(bufmgr);
}

int
drm_intel_bo_exec(drm_intel_bo *bo, int used,
		  drm_clip_rect_t *cliprects, int num_cliprects,
		  int DR4)
{
   return bo->bufmgr->bo_exec(bo, used, cliprects, num_cliprects, DR4);
}

void
drm_intel_bufmgr_set_debug(drm_intel_bufmgr *bufmgr, int enable_debug)
{
   bufmgr->debug = enable_debug;
}

int
drm_intel_bufmgr_check_aperture_space(drm_intel_bo **bo_array, int count)
{
	return bo_array[0]->bufmgr->check_aperture_space(bo_array, count);
}

int
drm_intel_bo_flink(drm_intel_bo *bo, uint32_t *name)
{
    if (bo->bufmgr->bo_flink)
	return bo->bufmgr->bo_flink(bo, name);

    return -ENODEV;
}

int
drm_intel_bo_emit_reloc(drm_intel_bo *bo, uint32_t offset,
			drm_intel_bo *target_bo, uint32_t target_offset,
			uint32_t read_domains, uint32_t write_domain)
{
	return bo->bufmgr->bo_emit_reloc(bo, offset,
					 target_bo, target_offset,
					 read_domains, write_domain);
}

int
drm_intel_bo_pin(drm_intel_bo *bo, uint32_t alignment)
{
    if (bo->bufmgr->bo_pin)
	return bo->bufmgr->bo_pin(bo, alignment);

    return -ENODEV;
}

int
drm_intel_bo_unpin(drm_intel_bo *bo)
{
    if (bo->bufmgr->bo_unpin)
	return bo->bufmgr->bo_unpin(bo);

    return -ENODEV;
}

int drm_intel_bo_set_tiling(drm_intel_bo *bo, uint32_t *tiling_mode,
			    uint32_t stride)
{
    if (bo->bufmgr->bo_set_tiling)
	return bo->bufmgr->bo_set_tiling(bo, tiling_mode, stride);

    *tiling_mode = I915_TILING_NONE;
    return 0;
}

int drm_intel_bo_get_tiling(drm_intel_bo *bo, uint32_t *tiling_mode,
			    uint32_t *swizzle_mode)
{
    if (bo->bufmgr->bo_get_tiling)
	return bo->bufmgr->bo_get_tiling(bo, tiling_mode, swizzle_mode);

    *tiling_mode = I915_TILING_NONE;
    *swizzle_mode = I915_BIT_6_SWIZZLE_NONE;
    return 0;
}

#if 0
/*
 * $XFree86: xc/lib/XThrStub/UIThrStubs.c,v 3.3 2001/11/18 21:13:26 herrb Exp $
 *
 * Copyright (c) 1995 David E. Wexelblat.  All rights reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL DAVID E. WEXELBLAT BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of David E. Wexelblat shall
 * not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization
 * from David E. Wexelblat.
 * 
 */

/*
 * Stubs for thread functions needed by the X library.  Supports 
 * UnixWare 2.x threads; may support Solaris 2 threads as well, but not
 * tested.  Defining things this way removes the dependency of the X
 * library on the threads library, but still supports threads if the user
 * specificies the thread library on the link line.
 */

/*
 * Modifications by Carlos A M dos Santos, XFree86 Project, November 1999.
 *
 * Explanation from <X11/Xos_r.h>:
 * The structure below is complicated, mostly because P1003.1c (the
 * IEEE POSIX Threads spec) went through lots of drafts, and some
 * vendors shipped systems based on draft API that were changed later.
 * Unfortunately POSIX did not provide a feature-test macro for
 * distinguishing each of the drafts.
 */

#include <stdlib.h>

static int _Xthr_once_stub_(void *, void (*)(void));
static int _Xthr_key_create_stub_(unsigned int *, void (*)(void *));
static int _Xthr_setspecific_stub_(unsigned int, const void *);
static void *_Xthr_getspecific_stub_(unsigned int);

#ifdef CTHREADS
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cthreads.h>
typedef cthread_t xthread_t;
#define xthread_self cthread_self
#pragma weak cthread_self = _Xthr_self_stub_
#define xmutex_init mutex_init
#pragma weak mutex_init = _Xthr_zero_stub_
#pragma weak mutex_clear = _Xthr_zero_stub_
#pragma weak mutex_lock = _Xthr_zero_stub_
#pragma weak mutex_unlock = _Xthr_zero_stub_
#pragma weak condition_init = _Xthr_zero_stub_
#pragma weak condition_clear = _Xthr_zero_stub_
#pragma weak condition_wait = _Xthr_zero_stub_
#pragma weak condition_signal = _Xthr_zero_stub_
#pragma weak condition_broadcast = _Xthr_zero_stub_
#else /* !CTHREADS */
#if defined(SVR4) && !defined(__sgi)
#include <thread.h>
typedef thread_t xthread_t;
#pragma weak thr_self = _Xthr_self_stub_
#pragma weak mutex_init = _Xthr_zero_stub_
#pragma weak mutex_destroy = _Xthr_zero_stub_
#pragma weak mutex_lock = _Xthr_zero_stub_
#pragma weak mutex_unlock = _Xthr_zero_stub_
#pragma weak cond_init = _Xthr_zero_stub_
#pragma weak cond_destroy = _Xthr_zero_stub_
#pragma weak cond_wait = _Xthr_zero_stub_
#pragma weak cond_signal = _Xthr_zero_stub_
#pragma weak cond_broadcast = _Xthr_zero_stub_
#else /* !SVR4 */
#ifdef WIN32
    /*
     * Don't know what to do here. Is there something do be done at all?
     */
#else /* !WIN32 */
#ifdef USE_TIS_SUPPORT
#include <tis.h>
typedef pthread_t xthread_t;
#pragma weak tis_self = _Xthr_self_stub_
#pragma weak tis_mutex_init = _Xthr_zero_stub_
#pragma weak tis_mutex_destroy = _Xthr_zero_stub_
#pragma weak tis_mutex_lock = _Xthr_zero_stub_
#pragma weak tis_mutex_unlock = _Xthr_zero_stub_
#pragma weak tis_cond_init = _Xthr_zero_stub_
#pragma weak tis_cond_destroy = _Xthr_zero_stub_
#pragma weak tis_cond_wait = _Xthr_zero_stub_
#pragma weak tis_cond_signal = _Xthr_zero_stub_
#pragma weak tis_cond_broadcast = _Xthr_zero_stub_
#else
#include <pthread.h>
typedef pthread_t xthread_t;
#if __GNUC__ >= 3 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
xthread_t pthread_self()    __attribute__ ((weak, alias ("_Xthr_self_stub_")));
int pthread_mutex_init()    __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int pthread_mutex_destroy() __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int pthread_mutex_lock()    __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int pthread_mutex_unlock()  __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int pthread_cond_init()     __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int pthread_cond_destroy()  __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int pthread_cond_wait()     __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int pthread_cond_signal()   __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int pthread_cond_broadcast() __attribute__ ((weak, alias ("_Xthr_zero_stub_")));
int pthread_key_create()    __attribute__ ((weak, alias ("_Xthr_key_create_stub_")));
void *pthread_getspecific()  __attribute__ ((weak, alias ("_Xthr_getspecific_stub_")));
int pthread_setspecific()   __attribute__ ((weak, alias ("_Xthr_setspecific_stub_")));
int pthread_once() __attribute__ ((weak, alias ("_Xthr_once_stub_")));
#else	/* __GNUC__ */
#pragma weak pthread_self = _Xthr_self_stub_
#pragma weak pthread_mutex_init = _Xthr_zero_stub_
#pragma weak pthread_mutex_destroy = _Xthr_zero_stub_
#pragma weak pthread_mutex_lock = _Xthr_zero_stub_
#pragma weak pthread_mutex_unlock = _Xthr_zero_stub_
#pragma weak pthread_cond_init = _Xthr_zero_stub_
#pragma weak pthread_cond_destroy = _Xthr_zero_stub_
#pragma weak pthread_cond_wait = _Xthr_zero_stub_
#pragma weak pthread_cond_signal = _Xthr_zero_stub_
#pragma weak pthread_cond_broadcast = _Xthr_zero_stub_
/* These are added for libGL */
#pragma weak pthread_key_create = _Xthr_key_create_stub_
#pragma weak pthread_getspecific = _Xthr_getspecific_stub_
#pragma weak pthread_setspecific = _Xthr_setspecific_stub_
#pragam weak pthread_once = _Xthr_once_stub_
#endif	/* __GNUC__ */
#if defined(_DECTHREADS_) || defined(linux)
#pragma weak pthread_equal = _Xthr_equal_stub_	/* See Xthreads.h! */
int
_Xthr_equal_stub_()
{
    return(1);
}
#endif /* _DECTHREADS_ || linux */
#endif /* USE_TIS_SUPPORT */
#endif /* WIN32 */
#endif /* SVR4 */
#endif /* CTHREADS */

static xthread_t 
_Xthr_self_stub_()
{
    static xthread_t _X_no_thread_id;

    return(_X_no_thread_id);	/* defined by <X11/Xthreads.h> */
}

static int 
_Xthr_zero_stub_()
{
    return(0);
}

static int 
_Xthr_once_stub_(void *id, void (*routine)(void))
{
    static int done = 0;
    
    if (!done) {
	routine();
	done++;
    }
    return 0;
}

#include <errno.h>

#define XTHR_KEYS_CHUNK 100

static void **_Xthr_keys_ = NULL;
static unsigned int _Xthr_last_key_ = 0;

static int
_Xthr_key_create_stub_(unsigned int *key, void (*destructor)(void *))
{
    void **tmp;
    unsigned int i;

    if ((_Xthr_last_key_ % XTHR_KEYS_CHUNK) == 0) {
	tmp = realloc(_Xthr_keys_,
	    (_Xthr_last_key_ + XTHR_KEYS_CHUNK)*sizeof(void *));
	if (tmp == NULL) {
	    free(_Xthr_keys_);
	    return ENOMEM;
	}
	for (i = 0; i < XTHR_KEYS_CHUNK; i++)
	    tmp[_Xthr_last_key_ + i] = 0;
	_Xthr_keys_ = tmp;
    }
    *key = _Xthr_last_key_++;
    return 0;
}

static int
_Xthr_setspecific_stub_(unsigned int key, const void *value)
{
    if (_Xthr_last_key_ == 0 || key >= _Xthr_last_key_)
	return EINVAL;
    _Xthr_keys_[key] = value;
    return 0;
}

static void *
_Xthr_getspecific_stub_(unsigned int key)
{
    if (_Xthr_last_key_ == 0 || key >= _Xthr_last_key_)
	return NULL;
    return(_Xthr_keys_[key]);
}
#endif
