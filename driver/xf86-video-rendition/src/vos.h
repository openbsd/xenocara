/*
 * file vos.h
 *
 * layer to map operating system dependent system calls
 */

#ifndef __VOS_H__
#define __VOS_H__


/* Define IODEBUG if you need to debug out/in functions.  <DI> */
/* IODEBUG define is used in compiler.h. DON'T MOVE DOWN!      */

/* #define IODEBUG */

/*
 * includes
 */

#include "compiler.h"
#include "vtypes.h"

/*
 * function prototypes
 */
#define /*void*/ verite_out8(/*unsigned long*/ port, /*vu8*/ data) \
                   outb(port, data)

#define /*void*/ verite_out16(/*unsigned long*/ port, /*vu16*/ data) \
                   outw(port, data)

#define /*void*/ verite_out32(/*unsigned long*/ port, /*vu32*/ data) \
                   outl(port, data)

#define /*vu8*/  verite_in8(/*unsigned long*/ io_base)  ((vu8)inb(io_base))
#define /*vu16*/ verite_in16(/*unsigned long*/ io_base) ((vu16)inw(io_base))
#define /*vu32*/ verite_in32(/*unsigned long*/ io_base) ((vu32)inl(io_base))

/* memory accesses */
#define verite_read_memory32(base, offset) MMIO_IN32(base, offset)
#define verite_read_memory16(base, offset) MMIO_IN16(base, offset)
#define verite_read_memory8(base, offset)  MMIO_IN8(base, offset)
#define verite_write_memory32(base, offset, data) MMIO_OUT32(base, offset, data)
#define verite_write_memory16(base, offset, data) MMIO_OUT16(base, offset, data)
#define verite_write_memory8(base, offset, data)  MMIO_OUT8(base, offset, data)

/* the rest of it */
void verite_enableio(void);
void verite_disableio(void);
vu8 *verite_mapmemory(vu8 *membase, vu32 size);
void verite_unmapmemory(vu8 *vmembase, vu32 size);

#endif /* #ifndef _VOS_H_ */

/*
 * end of file vos.h
 */
