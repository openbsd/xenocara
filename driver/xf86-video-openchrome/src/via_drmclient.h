/*
 * Copyright (C) 2005 The Unichrome Project, All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _VIA_DRMCLIENT_H
#define _VIA_DRMCLIENT_H

#include "stdint.h"
#include "drm.h"
#include "xf86drm.h"

#ifdef X_NEED_DRMLOCK
#define drm_hw_lock_t drmLock
#endif

#define UNICHROME_LOCK(fd, lockNo, saPriv, context, lastcontext, ret)	\
    do {								\
	volatile drm_hw_lock_t *lockPtr = XVMCLOCKPTR((saPriv), (lockNo));	\
	unsigned lockVal;						\
        DRM_CAS_RESULT(__ret);						\
									\
	ret = 0;							\
	lockVal = lockPtr->lock & ~(DRM_LOCK_HELD | DRM_LOCK_CONT);	\
	DRM_CAS(lockPtr, lockVal, (context) | DRM_LOCK_HELD, __ret);	\
	if (__ret) {							\
	    drm_via_futex_t fx;						\
									\
            lockVal = lockPtr->lock;					\
	    if (! (lockVal & DRM_LOCK_HELD)) continue;			\
	    if ((lockVal & ~(DRM_LOCK_HELD | DRM_LOCK_CONT) )		\
		== (context)) {						\
	      lastcontext = lockVal & ~(DRM_LOCK_HELD | DRM_LOCK_CONT);	\
		break;							\
	    }								\
	    fx.val = lockVal | DRM_LOCK_CONT;				\
	    DRM_CAS( lockPtr, lockVal, fx.val, __ret);			\
	    lockVal = lockPtr->lock;					\
	    if (__ret) continue;					\
	    fx.func = VIA_FUTEX_WAIT;					\
	    fx.lock = (lockNo);						\
	    fx.ms = 10;							\
	    ret = drmCommandWrite((fd), DRM_VIA_DEC_FUTEX,		\
				  &fx,sizeof(fx));			\
	    lastcontext = lockVal;					\
	    if (ret) break;						\
	    continue;							\
	} else {							\
	    lastcontext = lockVal;					\
	    break;							\
	}								\
    } while (1)								\
    
#define UNICHROME_UNLOCK(fd, lockNo, saPriv, context)			\
    do {								\
      volatile drm_hw_lock_t *lockPtr = XVMCLOCKPTR((saPriv), (lockNo)); 	\
									\
	if ((lockPtr->lock & ~DRM_LOCK_CONT) ==				\
	       ((context) | DRM_LOCK_HELD)) {				\
	  DRM_CAS_RESULT(__ret);					\
	    DRM_CAS(lockPtr,(context) | DRM_LOCK_HELD, context, __ret); \
	    if (__ret) {						\
		drm_via_futex_t fx;					\
		fx.func = VIA_FUTEX_WAKE;				\
		fx.lock = lockNo;					\
		DRM_CAS(lockPtr, (context) | DRM_LOCK_HELD |		\
			DRM_LOCK_CONT,					\
			context, __ret);				\
		drmCommandWrite((fd), DRM_VIA_DEC_FUTEX, &fx,		\
				sizeof(fx));				\
	    }								\
	}								\
    } while (0)								\

#define UNICHROME_LOCK_DECODER1 0
#define UNICHROME_LOCK_DECODER2 1
#define UNICHROME_LOCK_HQV      4
#define __user
      
#endif
