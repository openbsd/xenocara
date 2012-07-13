/**
 ** Hack for OpenBSD atexit
 ** Author:  Matthew Dempsky <matthew@dempsky.org>
 **/
extern int __cxa_atexit(void (*)(void *), void *, void *);
extern void *__dso_handle;

int __attribute__((visibility("hidden")))
atexit(void (*fn)(void))
{
   return (__cxa_atexit((void (*)(void *))fn, 0, &__dso_handle));
}
