/*
 * file v1krisc.h
 *
 * low level function to communicate with the on-board RISC
 */

#ifndef __V1KRISC_H__
#define __V1KRISC_H__



/*
 * includes
 */

#include "v1kregs.h"
#include "vtypes.h"



/*
 * function prototypes
 */

void v1k_start(ScrnInfoPtr pScreenInfo, vu32 pc);
void v1k_continue(ScrnInfoPtr pScreenInfo);
void v1k_stop(ScrnInfoPtr pScreenInfo);
void v1k_flushicache(ScrnInfoPtr pScreenInfo);
void v1k_softreset(ScrnInfoPtr pScreenInfo);



#endif /* #ifndef __V1KRISC_H__ */
