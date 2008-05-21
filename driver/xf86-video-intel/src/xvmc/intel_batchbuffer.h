#ifndef _INTEL_BATCHBUFFER_H
#define _INTEL_BATCHBUFFER_H

/* #define VERBOSE 0 */
#ifndef VERBOSE
extern int VERBOSE;
#endif

#define BATCH_LOCALS    char *batch_ptr;

#define BEGIN_BATCH(n)                                                  \
    do {                                                                \
        if (VERBOSE) fprintf(stderr,                                    \
                             "BEGIN_BATCH(%ld) in %s, %d dwords free\n", \
                             ((unsigned long)n), __FUNCTION__,          \
                             xvmc_driver->batch.space/4);                     \
        if (xvmc_driver->batch.space < (n)*4)                                 \
            intelFlushBatch(TRUE);                            \
        batch_ptr = xvmc_driver->batch.ptr;                                   \
    } while (0)

#define OUT_BATCH(n)                                                    \
    do {                                                                \
        *(GLuint *)batch_ptr = (n);                                     \
        if (VERBOSE) fprintf(stderr, " -- %08x at %s/%d\n", (n), __FILE__, __LINE__); \
        batch_ptr += 4;                                                 \
    } while (0)

#define ADVANCE_BATCH()                                        \
    do {                                                       \
        if (VERBOSE) fprintf(stderr, "ADVANCE_BATCH()\n");     \
        xvmc_driver->batch.space -= (batch_ptr - xvmc_driver->batch.ptr);  \
        xvmc_driver->batch.ptr = batch_ptr;                          \
        assert(xvmc_driver->batch.space >= 0);                       \
    } while(0)

extern void intelFlushBatch(Bool);
extern void intelBatchbufferData(const void *, unsigned, unsigned);
extern Bool intelInitBatchBuffer(void);
extern void intelFiniBatchBuffer(void);
extern void intelCmdIoctl(char *, unsigned);
#endif /* _INTEL_BATCHBUFFER_H */
