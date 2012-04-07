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

#if defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
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
#define CYGWIN_PERF 
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
#else


#if defined(att) || defined(QNX4)
#define LOADSTUB
#endif

#ifndef LOADSTUB
#if !defined(linux) && !defined(__UNIXOS2__) && !defined(__GLIBC__)
#include <nlist.h>
#endif /* !linux && ... */
#endif /* LOADSTUB */

#if defined(MOTOROLA) && defined(SYSV)
#include <sys/sysinfo.h>
#endif

#ifdef CSRG_BASED
#include <sys/param.h>
#endif

#if defined(umips) || (defined(ultrix) && defined(mips))
#include <sys/fixpoint.h>
#endif

#if  defined(CRAY) || defined(AIXV3)
#include <sys/param.h>
#define word word_t
#include <sys/sysinfo.h>
#undef word
#undef n_type
#define n_type n_value
#endif	/* CRAY */

#ifdef sequent
#include <sys/vm.h>
#endif /* sequent */


#ifdef hcx
#include <sys/param.h>
#endif /* hcx */

#if defined(UTEK) || defined(alliant) || (defined(MOTOROLA) && defined(SVR4))
#define FSCALE	100.0
#endif

#ifdef sequent
#define FSCALE	1000.0
#endif

#ifdef sgi
#define FSCALE	1024.0
#endif

#if defined(sony) && OSMAJORVERSION == 4
#ifdef mips
#include <sys/fixpoint.h>
#else
#include <sys/param.h>
#endif
#endif

#ifdef __osf__
/*
 * Use the table(2) interface; it doesn't require setuid root.
 *
 * Select 0, 1, or 2 for 5, 30, or 60 second load averages.
 */
#ifndef WHICH_AVG
#define WHICH_AVG 1
#endif
#include <sys/table.h>
#endif

#ifdef SVR4
#ifndef FSCALE
#define FSCALE	(1 << 8)
#endif
#endif

#ifdef X_NOT_POSIX
extern long lseek();
#endif

static void xload_error(const char *, const char *) _X_NORETURN;


#if defined(SYSV) && defined(i386)
/*
 * inspired by 'avgload' by John F. Haugh II
 */
#include <sys/param.h>
#include <sys/buf.h>
#include <sys/immu.h>
#include <sys/region.h>
#include <sys/var.h>
#include <sys/proc.h>
#define KERNEL_FILE "/unix"
#define KMEM_FILE "/dev/kmem"
#define VAR_NAME "v"
#define PROC_NAME "proc"
#define BUF_NAME "buf"
#define DECAY 0.8
struct nlist namelist[] = {
  {VAR_NAME},
  {PROC_NAME},
  {BUF_NAME},
  {0},
};

static int kmem;
static struct var v;
static struct proc *p;
static XtPointer first_buf, last_buf;

void InitLoadPoint()				/* SYSV386 version */
{
    int i;

    nlist( KERNEL_FILE, namelist);

    for (i=0; namelist[i].n_name; i++) 
	if (namelist[i].n_value == 0)
	    xload_error("cannot get name list from", KERNEL_FILE);

    if ((kmem = open(KMEM_FILE, O_RDONLY)) < 0)
	xload_error("cannot open", KMEM_FILE);

    if (lseek(kmem, namelist[0].n_value, 0) == -1)
	xload_error("cannot seek", VAR_NAME);

    if (read(kmem, &v, sizeof(v)) != sizeof(v))
	xload_error("cannot read", VAR_NAME);

    if ((p=(struct proc *)malloc(v.v_proc*sizeof(*p))) == NULL)
	xload_error("cannot allocat space for", PROC_NAME);
	  
    first_buf = (XtPointer) namelist[2].n_value;
    last_buf  = (char *)first_buf + v.v_buf * sizeof(struct buf);
}
	
/* ARGSUSED */
void GetLoadPoint( w, closure, call_data )	/* SYSV386 version */
Widget	w;		/* unused */
XtPointer	closure;	/* unused */
XtPointer	call_data;	/* pointer to (double) return value */
{
    double *loadavg = (double *)call_data;
    static double avenrun = 0.0;
    int i, nproc, size;
	
    (void) lseek(kmem, namelist[0].n_value, 0);
    (void) read(kmem, &v, sizeof(v));

    size = (struct proc *)v.ve_proc - (struct proc *)namelist[1].n_value;

    (void) lseek(kmem, namelist[1].n_value, 0);
    (void) read(kmem, p, size * sizeof(struct proc));

    for (nproc = 0, i=0; i<size; i++) 
	  if ((p[i].p_stat == SRUN) ||
	      (p[i].p_stat == SIDL) ||
	      (p[i].p_stat == SXBRK) ||
	      (p[i].p_stat == SSLEEP && (p[i].p_pri < PZERO) &&
	       (p[i].p_wchan >= (char *)first_buf) && (p[i].p_wchan < (char *)last_buf)))
	    nproc++;

    /* update the load average using a decay filter */
    avenrun = DECAY * avenrun + nproc * (1.0 - DECAY);
    *loadavg = avenrun;

    return;
}
#else /* not (SYSV && i386) */

#if defined(linux) || (defined(__FreeBSD_kernel__) && defined(__GLIBC__))

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
#ifndef X_LOCALE
      char *dp;
      static char ldp = 0;
#endif


      if (fd < 0)
      {
              if (fd == -2 ||
                  (fd = open("/proc/loadavg", O_RDONLY)) < 0)
              {
                      fd = -2;
                      *(double *)call_data = 0.0;
                      return;
              }
#ifndef X_LOCALE
	      ldp = *localeconv()->decimal_point;
#endif
      }
      else
              lseek(fd, 0, 0);

      if ((n = read(fd, buf, sizeof(buf)-1)) > 0) {
#ifndef X_LOCALE
	  if (ldp != '.')
	      while ((dp = memchr(buf,'.',sizeof(buf)-1)) != NULL) {
		  *(char *)dp = ldp;
	      }
	  
#endif
	  if (sscanf(buf, "%lf", (double *)call_data) == 1)
	      return;
      }
      

      *(double *)call_data = 0.0;     /* temporary hiccup */

      return;
}

#else /* linux */

#ifdef __GNU__

#include <mach.h>

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

#else /* __GNU__ */

#ifdef __APPLE__

#include <mach/mach.h>

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

#else /* __APPLE__ */

#ifdef LOADSTUB

void InitLoadPoint()
{
}

/* ARGSUSED */
void GetLoadPoint( w, closure, call_data )
     Widget	w;		/* unused */
     XtPointer	closure;	/* unused */
     XtPointer	call_data;	/* pointer to (double) return value */
{
	*(double *)call_data = 1.0;
}

#else /* not LOADSTUB */

#ifdef __osf__

void InitLoadPoint()
{
}

/*ARGSUSED*/
void GetLoadPoint( w, closure, call_data )
     Widget   w;              /* unused */
     XtPointer  closure;        /* unused */
     XtPointer  call_data;      /* pointer to (double) return value */
{
    double *loadavg = (double *)call_data;
    struct tbl_loadavg load_data;

    if (table(TBL_LOADAVG, 0, (char *)&load_data, 1, sizeof(load_data)) < 0)
	xload_error("error reading load average", "");
    *loadavg = (load_data.tl_lscale == 0) ?
	load_data.tl_avenrun.d[WHICH_AVG] :
	load_data.tl_avenrun.l[WHICH_AVG] / (double)load_data.tl_lscale;
}

#else /* not __osf__ */

#ifdef __QNXNTO__
#include <time.h>
#include <sys/neutrino.h>
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
#else /* not __QNXNTO__ */

#ifdef __bsdi__
#include <kvm.h>

static struct nlist nl[] = {
  { "_averunnable" },
#define X_AVERUNNABLE 0
  { "_fscale" },
#define X_FSCALE      1
  { "" },
};
static kvm_t *kd;
static int fscale;

void InitLoadPoint(void)
{
  fixpt_t averunnable[3];  /* unused really */

  if ((kd = kvm_openfiles(NULL, NULL, NULL, O_RDONLY, NULL)) == NULL)
    xload_error("can't open kvm files", "");

  if (kvm_nlist(kd, nl) != 0)
    xload_error("can't read name list", "");

  if (kvm_read(kd, (off_t)nl[X_AVERUNNABLE].n_value, (char *)averunnable,
	       sizeof(averunnable)) != sizeof(averunnable))
    xload_error("couldn't obtain _averunnable variable", "");

  if (kvm_read(kd, (off_t)nl[X_FSCALE].n_value, (char *)&fscale,
	       sizeof(fscale)) != sizeof(fscale))
    xload_error("couldn't obtain _fscale variable", "");

  return;
}

void GetLoadPoint(
     Widget	w,		/* unused */
     XtPointer	closure,	/* unused */
    XtPointer	call_data)	/* ptr to (double) return value */
{
  double *loadavg = (double *)call_data;
  fixpt_t t;

  if (kvm_read(kd, (off_t)nl[X_AVERUNNABLE].n_value, (char *)&t,
	       sizeof(t)) != sizeof(t))
    xload_error("couldn't obtain load average", "");

  *loadavg = (double)t/fscale;

  return;
}

#else /* not __bsdi__ */
#if defined(HAVE_GETLOADAVG)
#include <stdlib.h>
#ifdef HAVE_SYS_LOADAVG_H
#include <sys/loadavg.h>	/* Solaris definition of getloadavg */
#endif

void InitLoadPoint()
{
}

void GetLoadPoint(w, closure, call_data)
     Widget w;          /* unused */
     XtPointer closure;   /* unused */
     XtPointer call_data; /* ptr to (double) return value */
{
  double *loadavg = (double *)call_data;

  if (getloadavg(loadavg, 1) < 0) 
    xload_error("couldn't obtain load average", "");
}

#else /* not HAVE_GETLOADAVG */

#ifndef KMEM_FILE
#define KMEM_FILE "/dev/kmem"
#endif

#ifndef KERNEL_FILE

#ifdef alliant
#define KERNEL_FILE "/vmunix"
#endif /* alliant */

#ifdef CRAY
#define KERNEL_FILE "/unicos"
#endif /* CRAY */

#ifdef hpux
#define KERNEL_FILE "/hp-ux"
#endif /* hpux */


#ifdef umips
# ifdef SYSTYPE_SYSV
# define KERNEL_FILE "/unix"
# else
# define KERNEL_FILE "/vmunix"
# endif /* SYSTYPE_SYSV */
#endif /* umips */

#ifdef sequent
#define KERNEL_FILE "/dynix"
#endif /* sequent */

#ifdef hcx
#define KERNEL_FILE "/unix"
#endif /* hcx */

#ifdef MOTOROLA
#if defined(SYSV) && defined(m68k)
#define KERNEL_FILE "/sysV68"
#endif
#if defined(SYSV) && defined(m88k)
#define KERNEL_FILE "/unix"
#endif
#ifdef SVR4
#define KERNEL_FILE "/unix"
#endif
#endif /* MOTOROLA */

#ifdef sgi
#if (OSMAJORVERSION > 4)
#define KERNEL_FILE "/unix"
#endif
#endif

/*
 * provide default for everyone else
 */
#ifndef KERNEL_FILE
#ifdef SVR4
#define KERNEL_FILE "/stand/unix"
#else
#ifdef SYSV
#define KERNEL_FILE "/unix"
#else
/* If a BSD system, check in <paths.h> */
#   ifdef BSD
#    include <paths.h>
#    ifdef _PATH_UNIX
#     define KERNEL_FILE _PATH_UNIX
#    else
#     ifdef _PATH_KERNEL
#      define KERNEL_FILE _PATH_KERNEL
#     else
#      define KERNEL_FILE "/vmunix"
#     endif
#    endif
#   else /* BSD */
#    define KERNEL_FILE "/vmunix"
#   endif /* BSD */
#endif /* SYSV */
#endif /* SVR4 */
#endif /* KERNEL_FILE */
#endif /* KERNEL_FILE */


#ifndef KERNEL_LOAD_VARIABLE
#    if defined(BSD) && (BSD >= 199103)
#        define KERNEL_LOAD_VARIABLE "_averunnable"
#    endif /* BSD >= 199103 */

#    ifdef alliant
#        define KERNEL_LOAD_VARIABLE "_Loadavg"
#    endif /* alliant */

#    ifdef CRAY
#        if defined(CRAY2) && OSMAJORVERSION == 4
#            define KERNEL_LOAD_VARIABLE "avenrun"
#        else
#            define KERNEL_LOAD_VARIABLE "sysinfo"
#            define SYSINFO
#        endif /* defined(CRAY2) && OSMAJORVERSION == 4 */
#    endif /* CRAY */

#    ifdef hpux
#        ifdef __hp9000s800
#            define KERNEL_LOAD_VARIABLE "avenrun"
#        endif /* hp9000s800 */
#    endif /* hpux */

#    ifdef umips
#        ifdef SYSTYPE_SYSV
#            define KERNEL_LOAD_VARIABLE "avenrun"
#        else
#            define KERNEL_LOAD_VARIABLE "_avenrun"
#        endif /* SYSTYPE_SYSV */
#    endif /* umips */

#    ifdef sgi
#	 define KERNEL_LOAD_VARIABLE "avenrun"
#    endif /* sgi */

#    ifdef AIXV3
#        define KERNEL_LOAD_VARIABLE "sysinfo"
#    endif /* AIXV3 */

#    ifdef MOTOROLA
#        if defined(SYSV) && defined(m68k)
#            define KERNEL_LOAD_VARIABLE "sysinfo"
#        endif
#        if defined(SYSV) && defined(m88k)
#            define KERNEL_LOAD_VARIABLE "_sysinfo"
#        endif
#        ifdef SVR4
#            define KERNEL_LOAD_VARIABLE "avenrun"
#        endif
#    endif /* MOTOROLA */

#endif /* KERNEL_LOAD_VARIABLE */

/*
 * provide default for everyone else
 */

#ifndef KERNEL_LOAD_VARIABLE
#    ifdef USG
#        define KERNEL_LOAD_VARIABLE "sysinfo"
#        define SYSINFO
#    else
#    ifdef SVR4
#        define KERNEL_LOAD_VARIABLE "avenrun"
#    else
#        define KERNEL_LOAD_VARIABLE "_avenrun"
#    endif
#    endif
#endif /* KERNEL_LOAD_VARIABLE */

static struct nlist namelist[] = {	    /* namelist for vmunix grubbing */
#define LOADAV 0
    {KERNEL_LOAD_VARIABLE},
    {0}
};

static int kmem;
static long loadavg_seek;

void InitLoadPoint()
{
#if !defined(SVR4) && !defined(sgi) && !defined(MOTOROLA) && !defined(AIXV5) && !(BSD >= 199103) && !defined(__APPLE__)
    extern void nlist();
#endif

#ifdef AIXV3
    knlist( namelist, 1, sizeof(struct nlist));
#else	
    nlist( KERNEL_FILE, namelist);
#endif
    /*
     * Some systems appear to set only one of these to Zero if the entry could
     * not be found, I hope no_one returns Zero as a good value, or bad things
     * will happen to you.  (I have a hard time believing the value will
     * ever really be zero anyway).   CDP 5/17/89.
     */
#ifdef hcx
    if (namelist[LOADAV].n_type == 0 &&
#else
    if (namelist[LOADAV].n_type == 0 ||
#endif /* hcx */
	namelist[LOADAV].n_value == 0) {
	xload_error("cannot get name list from", KERNEL_FILE);
	exit(-1);
    }
    loadavg_seek = namelist[LOADAV].n_value;
#if defined(umips) && defined(SYSTYPE_SYSV)
    loadavg_seek &= 0x7fffffff;
#endif /* umips && SYSTYPE_SYSV */
#if (defined(CRAY) && defined(SYSINFO))
    loadavg_seek += ((char *) (((struct sysinfo *)NULL)->avenrun)) -
	((char *) NULL);
#endif /* CRAY && SYSINFO */
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

#if defined (UTEK) || defined(sequent) || defined(alliant) || defined(SVR4) || defined(sgi) || defined(hcx) || (BSD >= 199103)
	{
		long temp;
		(void) read(kmem, (char *)&temp, sizeof(long));
		*loadavg = (double)temp/FSCALE;
	}
#else /* else not UTEK or sequent or alliant or SVR4 or sgi or hcx */
#  if defined(umips) || (defined(ultrix) && defined(mips))
	{
		fix temp;
		(void) read(kmem, (char *)&temp, sizeof(fix));
		*loadavg = FIX_TO_DBL(temp);
	}
#  else /* not umips or ultrix risc */
#    ifdef AIXV3
        {
          struct sysinfo sysinfo_now;
          struct sysinfo sysinfo_last;
          static firsttime = TRUE;
          static double runavg = 0.0, swpavg = 0.0;

          (void) lseek(kmem, loadavg_seek, 0);
          (void) read(kmem, (char *)&sysinfo_last, sizeof(struct sysinfo));
          if (firsttime)
            {
              *loadavg = 0.0;
              firsttime = FALSE;
            }
          else
            {
              sleep(1);
              (void) lseek(kmem, loadavg_seek, 0);
              (void) read(kmem, (char *)&sysinfo_now, sizeof(struct sysinfo));
              runavg *= 0.8; swpavg *= 0.8;
              if (sysinfo_now.runocc != sysinfo_last.runocc)
                runavg += 0.2*((sysinfo_now.runque - sysinfo_last.runque - 1)
                          /(double)(sysinfo_now.runocc - sysinfo_last.runocc));
              if (sysinfo_now.swpocc != sysinfo_last.swpocc)
                swpavg += 0.2*((sysinfo_now.swpque - sysinfo_last.swpque)
                          /(double)(sysinfo_now.swpocc - sysinfo_last.swpocc));
              *loadavg = runavg + swpavg;
              sysinfo_last = sysinfo_now;
            }
          /* otherwise we leave load alone. */
        }
#    else /* not AIXV3 */
#      if defined(MOTOROLA) && defined(SYSV)
	{
        static int init = 0;
        static kmem;
        static long loadavg_seek;

#define CEXP    0.25            /* Constant used for load averaging */

        struct sysinfo sysinfod;
        static double oldloadavg;
        static double cexp = CEXP;
        static long sv_rq, sv_oc;   /* save old values */
        double rq, oc;              /* amount values have changed */

        if (!init)
        {
            if (nlist(KERNEL_FILE,namelist) == -1)
            {
                perror("xload: nlist()");
                xload_error("cannot get name list from", KERNEL_FILE);
            }
            loadavg_seek = namelist[0].n_value;

            kmem = open(KMEM_FILE, O_RDONLY);
            if (kmem < 0)
            {
                perror("xload: open()");
                xload_error("cannot open", KMEM_FILE);
            }
        }

        lseek(kmem, loadavg_seek, 0);
        if (read(kmem, &sysinfod, (int) sizeof (struct sysinfo)) == -1)
        {
             perror("xload: read() SYSINFONL");
             xload_error("read failed from", KMEM_FILE);
        }

        if (!init)
        {
            init = 1;
            sv_rq = sysinfod.runque;
            sv_oc = sysinfod.runocc;
            oldloadavg = *loadavg = 0.0;
            return;
        }
        /*
         * calculate the amount the values have
         * changed since last update
         */
        rq = (double) sysinfod.runque - sv_rq;
        oc = (double) sysinfod.runocc - sv_oc;

        /*
         * save old values for next time
         */
        sv_rq = sysinfod.runque;
        sv_oc = sysinfod.runocc;

        if (oc == 0.0)          /* avoid divide by zero  */
        {
                *loadavg = (1.0 - cexp) * oldloadavg;

        }
        else
        {
                *loadavg = ((1.0 - cexp) * oldloadavg) + ((rq / oc) * cexp);
        }
        oldloadavg = *loadavg;
	}
#      else /* not MOTOROLA */
#     if defined(sony) && OSMAJORVERSION == 4
#      ifdef mips
	{
		fix temp;
		(void) read(kmem, (char *)&temp, sizeof(fix));
		*loadavg = FIX_TO_DBL(temp);
	}
#      else /* not mips */
	{
		long temp;
		(void) read(kmem, (char *)&temp, sizeof(long));
		*loadavg = (double)temp/FSCALE;
	}
#      endif /* mips */
#     else /* not sony NEWSOS4 */
	(void) read(kmem, (char *)loadavg, sizeof(double));
#      endif /* sony NEWOS4 */
#     endif /* MOTOROLA else */
#    endif /* AIXV3 else */
#  endif /* umips else */
#endif /* SVR4 or ... else */
	return;
}
#endif /* HAVE_GETLOADAVG else */
#endif /* __bsdi__ else */
#endif /* __QNXNTO__ else */
#endif /* __osf__ else */
#endif /* LOADSTUB else */
#endif /* __APPLE__ else */
#endif /* __GNU__ else */
#endif /* linux else */
#endif /* SYSV && i386 else */

static void xload_error(const char *str1, const char *str2)
{
    (void) fprintf(stderr,"xload: %s %s\n", str1, str2);
#ifdef __bsdi__
    if (kd)
	kvm_close(kd);
#endif
    exit(-1);
}

#endif /* END of __CYGWIN__ */
