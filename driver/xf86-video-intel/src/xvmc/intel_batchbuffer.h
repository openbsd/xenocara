#ifndef _INTEL_BATCHBUFFER_H
#define _INTEL_BATCHBUFFER_H

/* #define VERBOSE 0 */
#ifndef VERBOSE
extern int VERBOSE;
#endif

#define BATCH_LOCALS    unsigned char *batch_ptr;

#define BEGIN_BATCH(n)                                                  \
    do {                                                                \
        if (xvmc_driver->batch.space < (n)*4)                           \
            intelFlushBatch(TRUE);                            		\
        batch_ptr = xvmc_driver->batch.ptr;                             \
    } while (0)

#define OUT_BATCH(n)                                                    \
    do {                                                                \
        *(unsigned int *)batch_ptr = (n);                               \
        batch_ptr += 4;                                                 \
    } while (0)

#define OUT_BATCH_SHORT(n)                                              \
    do {                                                                \
        *(short *)batch_ptr = (n);                                      \
        batch_ptr += 2;                                                 \
    } while (0)

#define OUT_BATCH_CHAR(n)                                               \
	do {                                                                \
		*(char *)batch_ptr = (n);                                       \
		batch_ptr ++;                                                   \
	} while (0)
#define ADVANCE_BATCH()                                                  \
    do {                                                                 \
        xvmc_driver->batch.space -= (batch_ptr - xvmc_driver->batch.ptr);\
        xvmc_driver->batch.ptr = batch_ptr;                              \
        assert(xvmc_driver->batch.space >= 0);                           \
    } while(0)

extern void intelFlushBatch(Bool);
extern void intelBatchbufferData(const void *, unsigned, unsigned);
extern Bool intelInitBatchBuffer(void);
extern void intelFiniBatchBuffer(void);
extern void intelCmdIoctl(char *, unsigned);
#endif /* _INTEL_BATCHBUFFER_H */
