//
// Copyright 2020 Serge Martin
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// Extract from Serge's printf clover code by airlied.

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include "u_printf.h"
#include "util/macros.h"

/* Some versions of MinGW are missing _vscprintf's declaration, although they
 * still provide the symbol in the import library. */
#ifdef __MINGW32__
_CRTIMP int _vscprintf(const char *format, va_list argptr);
#endif

#ifndef va_copy
#ifdef __va_copy
#define va_copy(dest, src) __va_copy((dest), (src))
#else
#define va_copy(dest, src) (dest) = (src)
#endif
#endif

size_t
u_printf_length(const char *fmt, va_list untouched_args)
{
   int size;
   char junk;

   /* Make a copy of the va_list so the original caller can still use it */
   va_list args;
   va_copy(args, untouched_args);

#ifdef _WIN32
   /* We need to use _vcsprintf to calculate the size as vsnprintf returns -1
    * if the number of characters to write is greater than count.
    */
   size = _vscprintf(fmt, args);
   (void)junk;
#else
   size = vsnprintf(&junk, 1, fmt, args);
#endif
   assert(size >= 0);

   va_end(args);

   return size;
}
