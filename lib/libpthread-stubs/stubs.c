/* Copyright (C) 2006 Diego Pettenò
 * Inspired by libX11 code copyright (c) 1995 David E. Wexelblat.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the names of the authors or their
 * institutions shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization from the authors.
 */

#include <pthread.h>
#include <stdlib.h>
#include "config.h"

#ifndef HAVE_PTHREAD_SELF
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
pthread_t pthread_self() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_self = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_MUTEX_INIT
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_mutex_init() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_mutex_init = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_MUTEX_DESTROY
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_mutex_destroy() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_mutex_destroy = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_MUTEX_LOCK
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_mutex_lock() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_mutex_lock = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_MUTEX_UNLOCK
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_mutex_unlock() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_mutex_unlock = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_COND_INIT
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_cond_init() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_cond_init = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_COND_DESTROY
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_cond_destroy() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_cond_destroy = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_CONDATTR_INIT
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_condattr_init() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_condattr_init = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_CONDATTR_DESTROY
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_condattr_destroy() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_condattr_destroy = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_COND_WAIT
#define NEED_ABORT_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_cond_wait() __attribute__ ((weak, alias ("__pthread_abort_stub")));
# else
#  pragma weak pthread_cond_wait = __pthread_abort_stub
# endif
#endif

#ifndef HAVE_PTHREAD_COND_TIMEDWAIT
#define NEED_ABORT_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_cond_timedwait() __attribute__ ((weak, alias ("__pthread_abort_stub")));
# else
#  pragma weak pthread_cond_timedwait = __pthread_abort_stub
# endif
#endif

#ifndef HAVE_PTHREAD_COND_SIGNAL
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_cond_signal() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_cond_signal = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_COND_BROADCAST
#define NEED_ZERO_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_cond_broadcast() __attribute__ ((weak, alias ("__pthread_zero_stub")));
# else
#  pragma weak pthread_cond_broadcast = __pthread_zero_stub
# endif
#endif

#ifndef HAVE_PTHREAD_EQUAL
#define NEED_EQUAL_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_equal() __attribute__ ((weak, alias ("__pthread_equal_stub")));
# else
#  pragma weak pthread_equal = __pthread_equal_stub
# endif
#endif

#ifndef HAVE_PTHREAD_EXIT
#define NEED_EXIT_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
void pthread_exit() __attribute__ ((weak, alias ("__pthread_exit_stub")));
# else
#  pragma weak pthread_exit = __pthread_exit_stub
# endif
#endif

#ifndef HAVE_PTHREAD_ONCE
#define NEED_ONCE_STUB
#ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_once() __attribute__ ((weak, alias ("__pthread_once_stub")));
# else
#  pragma wek pthread_once = __pthread_once_stub
# endif
#endif

#ifndef HAVE_PTHREAD_KEY_CREATE
#define NEED_PTHREAD_KEY_CREATE_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_key_create() __attribute ((weak, alias ("__pthread_key_create_stub")));
# else
#   pragma weak pthread_key_create = __pthread_key_create_stub
# endif
#endif

#ifndef HAVE_THREAD_SETSPECIFIC
#define NEED_PTHREAD_SETSPECIFIC_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
int pthread_setspecific() __attribute ((weak, alias ("__pthread_setspecific_stub")));
# else
#  pragma weak pthread_setspecific = __pthread_setspecific_stub
# endif
#endif

#ifndef HAVE_THREAD_GETSPECIFIC
#define NEED_PTHREAD_GETSPECIFIC_STUB
# ifdef SUPPORT_ATTRIBUTE_ALIAS
void * pthread_getspecific() __attribute ((weak, alias ("__pthread_getspecific_stub")));
# else
#  pragma weak pthread_getspecific = __pthread_getspecific_stub
# endif
#endif

#ifdef NEED_ZERO_STUB
static int __pthread_zero_stub()
{
    return 0;
}
#endif

#ifdef NEED_ABORT_STUB
static int __pthread_abort_stub()
{
    abort();
}
#endif

#ifdef NEED_EQUAL_STUB
static int __pthread_equal_stub(pthread_t t1, pthread_t t2)
{
    return (t1 == t2);
}
#endif

#ifdef NEED_EXIT_STUB
static void __pthread_exit_stub(void *ret)
{
    exit(EXIT_SUCCESS);
}
#endif

#ifdef NEED_ONCE_STUB

#include <errno.h>

#define PTHREAD_ONCE_KEYS_CHUNK 100
static void**__pthread_once_keys = NULL;
static unsigned int __pthread_once_last_key = 0;

static int 
__pthread_once_stub(void *id, void (*routine)(void))
{
    void **tmp;
    unsigned int i;

    /* look for the id */
    for (i = 0; i < __pthread_once_last_key; i++)
	if (__pthread_once_keys[i] == id) 
	    return 0;
    /* allocate more room if needed */
    if ((__pthread_once_last_key % PTHREAD_ONCE_KEYS_CHUNK) == 0) {
	tmp = realloc(__pthread_once_keys,
	      (__pthread_once_last_key 
	       + PTHREAD_ONCE_KEYS_CHUNK)*sizeof(void *));
	if (tmp == NULL)
	    return ENOMEM;
	for (i = 0; i < PTHREAD_ONCE_KEYS_CHUNK; i++)
	    tmp[__pthread_once_last_key + i] = NULL;
	__pthread_once_keys = tmp;
    }
    /* call the routine */
    routine();
    /* Mark it */
    __pthread_once_keys[__pthread_once_last_key++] = id;
    return 0;
}
#endif

#ifdef NEED_PTHREAD_KEY_CREATE_STUB
#include <errno.h>

#define _PTHREAD_KEYS_CHUNK 100

static void **__pthread_keys = NULL;
static unsigned int __pthread_last_key = 0;

static int
__pthread_key_create_stub(unsigned int *key, void (*destructor)(void *))
{
    void **tmp;
    unsigned int i;

    if ((__pthread_last_key % _PTHREAD_KEYS_CHUNK) == 0) {
	tmp = realloc(__pthread_keys,
	    (__pthread_last_key + _PTHREAD_KEYS_CHUNK)*sizeof(void *));
	if (tmp == NULL)
	    return ENOMEM;
	for (i = 0; i < _PTHREAD_KEYS_CHUNK; i++)
	    tmp[__pthread_last_key + i] = NULL;
	__pthread_keys = tmp;
    }
    *key = __pthread_last_key++;
    return 0;
}
#endif

#ifdef NEED_PTHREAD_SETSPECIFIC_STUB
static int
__pthread_setspecific_stub(unsigned int key, const void *value)
{
    if (__pthread_last_key == 0 || key >= __pthread_last_key)
	return EINVAL;
    __pthread_keys[key] = (void *)value;
    return 0;
}
#endif

#ifdef NEED_PTHREAD_GETSPECIFIC_STUB
static void *
__pthread_getspecific_stub(unsigned int key)
{
    if (__pthread_last_key == 0 || key >= __pthread_last_key)
	return NULL;
    return(__pthread_keys[key]);
}
#endif
