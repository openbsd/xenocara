
#ifndef _EyesP_h
#define _EyesP_h

#include "Eyes.h"
#include <X11/CoreP.h>
#ifdef XRENDER
#include <X11/extensions/Xrender.h>
#endif
#include "transform.h"
#ifdef PRESENT
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>
#include <xcb/present.h>
#include <xcb/xfixes.h>
#include <xcb/damage.h>
#endif

#define SEG_BUFF_SIZE		128

typedef struct {
	// X offset
	double x;
	// Y offset
	double y;
} EyeLayout;

typedef struct {
	EyeLayout *eyes;
	int count;

	double w_min_x;
	double w_max_x;
	double w_min_y;
double w_max_y;
} EyeConfiguration;

/* New fields for the eyes widget instance record */
typedef struct {
	 Pixel		pixel[PART_SHAPE];
	 GC		gc[PART_MAX];
/* start of graph stuff */
	 int		backing_store;	/* backing store variety */
	 Boolean	reverse_video;	/* swap fg and bg pixels */
	 Boolean	shape_window;	/* use SetWindowShapeMask */
	 int		update;		/* current timeout index */
	 TPoint		mouse;		/* old mouse position */
	 Boolean	biblically_accurate;
	 EyeConfiguration *configuration;
	 TPoint		*pupils;
	 Transform	t;
	 Transform	maskt;
	 XtIntervalId	interval_id;
	 Pixmap		shape_mask;	/* window shape */
         Boolean        has_xi2;
#ifdef XRENDER
	 Boolean    	render;
	 Picture	picture;
	 Picture	fill[PART_SHAPE];
#endif
#ifdef PRESENT
         Pixmap         back_buffer;
         xcb_damage_damage_t back_damage;
         xcb_xfixes_region_t back_region;
         Boolean        present;
#endif
	 Boolean    	distance;
   } EyesPart;

#define xt_xcb(w)       (XGetXCBConnection(XtDisplay(w)))

/* Full instance record declaration */
typedef struct _EyesRec {
   CorePart core;
   EyesPart eyes;
   } EyesRec;

/* New fields for the Eyes widget class record */
typedef struct {int dummy;} EyesClassPart;

/* Full class record declaration. */
typedef struct _EyesClassRec {
   CoreClassPart core_class;
   EyesClassPart eyes_class;
   } EyesClassRec;

/* Class pointer. */
extern EyesClassRec eyesClassRec;

#endif /* _EyesP_h */
