/* Copyright 2019 Intel Corporation */
/* SPDX-License-Identifier: MIT */

#include "no_extern_c.h"

#ifndef _C11_COMPAT_H_
#define _C11_COMPAT_H_

#if defined(__cplusplus)
   /* This is C++ code, not C */
#elif (__STDC_VERSION__ >= 201112L)
   /* Already C11 */
#else

#ifndef __has_extension
#define __has_extension(x) 0
#endif

/*
 * C11 static_assert() macro
 * assert.h only defines that name for C11 and above
 */
#ifndef static_assert
#if defined(__clang__) && __has_extension(c_static_assert)
#define static_assert _Static_assert
#elif defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || (__GNUC__ > 4))
#define static_assert _Static_assert
#else
#define static_assert(cond, mesg) struct _C11_COMPAT_H_static_assert_unused
#endif
#endif


#endif /* !C++ && !C11 */

#endif /* _C11_COMPAT_H_ */
