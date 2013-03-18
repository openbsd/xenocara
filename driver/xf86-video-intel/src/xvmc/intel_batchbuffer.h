#ifndef _INTEL_BATCHBUFFER_H
#define _INTEL_BATCHBUFFER_H

/* #define VERBOSE 0 */
#ifndef VERBOSE
extern int VERBOSE;
#endif

#define BATCH_LOCALS    unsigned char *batch_ptr;

#define BEGIN_BATCH(n)                                                  \
    do {                                                                \
	assert(xvmc_driver->batch.space >= (n) *4);			\
        if (xvmc_driver->batch.space < (n)*4)                           \
            intelFlushBatch(TRUE);                            		\
        batch_ptr = xvmc_driver->batch.ptr;                             \
    } while (0)

#define OUT_BATCH(n)                                                    \
    do {                                                                \
        *(unsigned int *)batch_ptr = (n);                               \
        batch_ptr += 4;                                                 \
    } while (0)

#define OUT_RELOC(bo,read_domains,write_domains,delta)  \
    do { \
        *(unsigned int *)batch_ptr = (delta) + bo->offset; \
        intel_batch_emit_reloc(bo, read_domains, write_domains, delta, batch_ptr); \
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
    } while(0)

extern void intelFlushBatch(Bool);
extern void intelBatchbufferData(const void *, unsigned, unsigned);
extern Bool intelInitBatchBuffer(void);
extern void intelFiniBatchBuffer(void);
extern void intelCmdIoctl(char *, unsigned);
extern void intel_batch_emit_reloc(dri_bo * bo, uint32_t read_domain,
				   uint32_t write_domain, uint32_t delta,
				   unsigned char *);
#endif /* _INTEL_BATCHBUFFER_H */
