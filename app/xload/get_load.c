/*

Copyright (c) 1989  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

/*
 * get_load - get system load
 *
 * Authors:  Many and varied...
 *
 * Call InitLoadPoint() to initialize.
 * GetLoadPoint() is a callback for the StripChart widget.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/Xlocale.h>
#include <stdio.h>
#include <stdlib.h>
#include "xload.h"

#if defined(__CYGWIN__) || defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# include <X11/Xwindows.h>
typedef struct {
  DWORD stat;
  union {
    LONG vLong;
    double vDouble;
    LONGLONG vLongLong;
    void *string;
  } u;
} COUNTER;
static HANDLE query;
static HANDLE counter;
static HINSTANCE hdll;
static long (__stdcall *pdhopen)(LPCSTR, DWORD, HANDLE);
static long (__stdcall *pdhaddcounter)(HANDLE, LPCSTR, DWORD, HANDLE*);
static long (__stdcall *pdhcollectquerydata)(HANDLE);
static long (__stdcall *pdhgetformattedcountervalue)(HANDLE, DWORD, LPDWORD, COUNTER*);
# define CYGWIN_PERF
void InitLoadPoint(void)
{
  long ret;
  hdll=LoadLibrary("pdh.dll");
  if (!hdll) exit(-1);
  pdhopen=(void*)GetProcAddress(hdll, "PdhOpenQueryA");
  if (!pdhopen) exit(-1);
  pdhaddcounter=(void*)GetProcAddress(hdll, "PdhAddCounterA");
  if (!pdhaddcounter) exit(-1);
  pdhcollectquerydata=(void*)GetProcAddress(hdll, "PdhCollectQueryData");
  if (!pdhcollectquerydata) exit(-1);
  pdhgetformattedcountervalue=(void*)GetProcAddress(hdll, "PdhGetFormattedCounterValue");
  if (!pdhgetformattedcountervalue) exit(-1);
  ret = pdhopen( NULL , 0, &query );
  if (ret!=0) exit(-1);
  ret = pdhaddcounter(query, "\\Processor(_Total)\\% Processor Time", 0, &counter);
  if (ret!=0) exit(-1);
}
void GetLoadPoint(
     Widget  w,              /* unused */
     XtPointer    closure,        /* unused */
     XtPointer    call_data)      /* pointer to (double) return value */
{
  double *loadavg = (double *)call_data;
  COUNTER fmtvalue;
  long ret;
  *loadavg = 0.0;
  ret = pdhcollectquerydata(query);
  if (ret!=0) return;
  ret = pdhgetformattedcountervalue(counter, 0x200, NULL, &fmtvalue);
  if (ret!=0) return;
  *loadavg = (fmtvalue.u.vDouble-0.01)/100.0;
}
#else /* not CYGWIN */

static void xload_error(const char *, const char *) _X_NORETURN;

# ifdef HAVE_GETLOADAVG
#  include <stdlib.h>
#  ifdef HAVE_SYS_LOADAVG_H
#   include <sys/loadavg.h>	/* Solaris definition of getloadavg */
#  endif

void InitLoadPoint(void)
{
}

void GetLoadPoint(
    Widget w,            /* unused */
    XtPointer closure,   /* unused */
    XtPointer call_data) /* ptr to (double) return value */
{
    double *loadavg = (double *)call_data;

    if (getloadavg(loadavg, 1) < 0)
        xload_error("couldn't obtain load average", "");
}

# else /* not HAVE_GETLOADAVG */


#  if !defined(linux) && !defined(__GLIBC__)
#   include <nlist.h>
#  endif /* !linux && ... */

#  ifdef CSRG_BASED
#   include <sys/param.h>
#  endif

#  if defined(linux) || (defined(__FreeBSD_kernel__) && defined(__GLIBC__))

void InitLoadPoint(void)
{
      return;
}

void GetLoadPoint(
    Widget	w,		/* unused */
    XtPointer	closure,	/* unused */
    XtPointer	call_data)      /* pointer to (double) return value */
{
      static int fd = -1;
      int n;
      char buf[10] = {0, };
#   ifndef X_LOCALE
      char *dp;
      static char ldp = 0;
#   endif


      if (fd < 0)
      {
              if (fd == -2 ||
                  (fd = open("/proc/loadavg", O_RDONLY)) < 0)
              {
                      fd = -2;
                      *(double *)call_data = 0.0;
                      return;
              }
#   ifndef X_LOCALE
	      ldp = *localeconv()->decimal_point;
#   endif
      }
      else
              lseek(fd, 0, 0);

      if ((n = read(fd, buf, sizeof(buf)-1)) > 0) {
#   ifndef X_LOCALE
	  if (ldp != '.')
	      while ((dp = memchr(buf,'.',sizeof(buf)-1)) != NULL) {
		  *(char *)dp = ldp;
	      }

#   endif
	  if (sscanf(buf, "%lf", (double *)call_data) == 1)
	      return;
      }


      *(double *)call_data = 0.0;     /* temporary hiccup */

      return;
}

#  else /* linux */

#   ifdef __GNU__

#    include <mach.h>

static processor_set_t default_set;

void InitLoadPoint(void)
{
  if (processor_set_default (mach_host_self (), &default_set) != KERN_SUCCESS)
    xload_error("cannot get processor_set_default", "");
}

/* ARGSUSED */
void GetLoadPoint(
    Widget	w,		/* unused */
    XtPointer	closure,	/* unused */
    XtPointer	call_data)	/* pointer to (double) return value */
{
  host_t host;
  struct processor_set_basic_info info;
  unsigned info_count;

  info_count = PROCESSOR_SET_BASIC_INFO_COUNT;
  if (processor_set_info (default_set, PROCESSOR_SET_BASIC_INFO, &host,
			  (processor_set_info_t) &info, &info_count)
      != KERN_SUCCESS)
    {
      InitLoadPoint();
      info.load_average = 0;
    }

  *(double *)call_data = info.load_average * 1000 / LOAD_SCALE;

  return;
}

#   else /* __GNU__ */

#    ifdef __APPLE__

#     include <mach/mach.h>

static mach_port_t host_priv_port;

void InitLoadPoint(void)
{
    host_priv_port = mach_host_self();
}

/* ARGSUSED */
void GetLoadPoint(
    Widget	w,		/* unused */
    XtPointer	closure,	/* unused */
    XtPointer	call_data)	/* pointer to (double) return value */
{
    double *loadavg = (double *)call_data;

    struct host_load_info load_data;
    int host_count;
    kern_return_t kr;

    host_count = sizeof(load_data)/sizeof(integer_t);
    kr = host_statistics(host_priv_port, HOST_LOAD_INFO,
                        (host_info_t)&load_data, &host_count);
    if (kr != KERN_SUCCESS)
        xload_error("cannot get host statistics", "");
    *loadavg = (double)load_data.avenrun[0]/LOAD_SCALE;
    return;
}

#    else /* __APPLE__ */



#     ifdef __QNXNTO__
#      include <time.h>
#      include <sys/neutrino.h>
static _Uint64t          nto_idle = 0, nto_idle_last = 0;
static  int       nto_idle_id;
static  struct timespec nto_now, nto_last;

void
InitLoadPoint(void)
{
  nto_idle_id = ClockId(1, 1); /* Idle thread */
  ClockTime(nto_idle_id, NULL, &nto_idle_last);
  clock_gettime( CLOCK_REALTIME, &nto_last);
}

/* ARGSUSED */
void
GetLoadPoint(			/* QNX NTO version */
    Widget	w,		/* unused */
    XtPointer	closure,	/* unused */
    XtPointer	call_data)	/* pointer to (double) return value */
{
    double *loadavg = (double *)call_data;
    double timediff;
    double temp = 0.0;

    ClockTime(nto_idle_id, NULL, &nto_idle);
    clock_gettime( CLOCK_REALTIME, &nto_now);
    timediff = 1000000000.0 * (nto_now.tv_sec - nto_last.tv_sec)
               + (nto_now.tv_nsec - nto_last.tv_nsec);
    temp = 1.0 - (nto_idle-nto_idle_last)/timediff;
    *loadavg = temp >= 0 ? temp : 0;
    nto_idle_last = nto_idle;
    nto_last = nto_now;
}
#     else /* not __QNXNTO__ */

#      ifndef KMEM_FILE
#       define KMEM_FILE "/dev/kmem"
#      endif

#      ifndef KERNEL_FILE
/*
 * provide default for everyone else
 */
/* If <paths.h> exists, check in it */
#       ifdef HAVE_PATHS_H
#        include <paths.h>
#        ifdef _PATH_UNIX
#         define KERNEL_FILE _PATH_UNIX
#        else
#         ifdef _PATH_KERNEL
#          define KERNEL_FILE _PATH_KERNEL
#         else
#          define KERNEL_FILE "/vmunix"
#         endif
#        endif
#       else /* HAVE_PATHS_H */
#        define KERNEL_FILE "/vmunix"
#       endif /* HAVE_PATHS_H */
#      endif /* KERNEL_FILE */


#      ifndef KERNEL_LOAD_VARIABLE
#       if defined(BSD) && (BSD >= 199103)
#        define KERNEL_LOAD_VARIABLE "_averunnable"
#       else /* BSD >= 199103 */
#        define KERNEL_LOAD_VARIABLE "_avenrun"
#       endif /* BSD >= 199103 */
#      endif /* KERNEL_LOAD_VARIABLE */

static struct nlist namelist[] = {	    /* namelist for vmunix grubbing */
#      define LOADAV 0
    {KERNEL_LOAD_VARIABLE},
    {0}
};

static int kmem;
static long loadavg_seek;

void InitLoadPoint()
{
#      if !defined(AIXV5) && !(BSD >= 199103) && !defined(__APPLE__)
    extern void nlist();
#      endif

    nlist( KERNEL_FILE, namelist);
    /*
     * Some systems appear to set only one of these to Zero if the entry could
     * not be found, I hope no_one returns Zero as a good value, or bad things
     * will happen to you.  (I have a hard time believing the value will
     * ever really be zero anyway).   CDP 5/17/89.
     */
    if (namelist[LOADAV].n_type == 0 ||
	namelist[LOADAV].n_value == 0) {
	xload_error("cannot get name list from", KERNEL_FILE);
	exit(-1);
    }
    loadavg_seek = namelist[LOADAV].n_value;
    kmem = open(KMEM_FILE, O_RDONLY);
    if (kmem < 0) xload_error("cannot open", KMEM_FILE);
}

/* ARGSUSED */
void GetLoadPoint( w, closure, call_data )
     Widget	w;		/* unused */
     XtPointer	closure;	/* unused */
     XtPointer	call_data;	/* pointer to (double) return value */
{
  	double *loadavg = (double *)call_data;

	(void) lseek(kmem, loadavg_seek, 0);

#      if (BSD >= 199103)
	{
		long temp;
		(void) read(kmem, (char *)&temp, sizeof(long));
		*loadavg = (double)temp/FSCALE;
	}
#      else /* else not BSD */
	(void) read(kmem, (char *)loadavg, sizeof(double));
#      endif /* or ... else */
	return;
}
#     endif /* __QNXNTO__ else */
#    endif /* __APPLE__ else */
#   endif /* __GNU__ else */
#  endif /* linux else */
# endif /* HAVE_GETLOADAVG else */

static void xload_error(const char *str1, const char *str2)
{
    (void) fprintf(stderr,"xload: %s %s\n", str1, str2);
    exit(-1);
}

#endif /* END of __CYGWIN__ */
