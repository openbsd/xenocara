#ifndef __GENERIC_BUS_H__
#define __GENERIC_BUS_H__

/* this is meant to be used for proprietary buses where abstraction is needed
   but they don't occur often enough to warrant a separate helper library */


#define GB_IOCTL_GET_NAME	1
          /* third argument is size of the buffer, fourth argument is pointer
	     to the buffer. Returns the name of the bus */
#define GB_IOCTL_GET_TYPE	2
          /* third argument is size of the buffer, fourth argument is pointer
	     to the buffer. Returns the type of the bus, driver should check
	     this at initialization time to find out whether they are compatible
	      */


typedef struct _GENERIC_BUS_Rec *GENERIC_BUS_Ptr;

typedef struct _GENERIC_BUS_Rec{
        int scrnIndex;
        DevUnion  DriverPrivate;
	Bool (*ioctl)(GENERIC_BUS_Ptr, long, long, char *);
	Bool (*read)(GENERIC_BUS_Ptr, CARD32,  CARD32, CARD8 *);
	Bool (*write)(GENERIC_BUS_Ptr, CARD32,  CARD32, CARD8 *);
	Bool (*fifo_read)(GENERIC_BUS_Ptr, CARD32,  CARD32, CARD8 *);
	Bool (*fifo_write)(GENERIC_BUS_Ptr, CARD32,  CARD32, CARD8 *);

	} GENERIC_BUS_Rec;





#endif
