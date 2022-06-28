#ifndef _COMMON_COMPAT_H_
#define _COMMOM_COMPAT_H_

#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 18)
#define CONST_ABI_18_0 const
#else
#define CONST_ABI_18_0
#endif

#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 16)
#define CONST_ABI_16_0 const
#else
#define CONST_ABI_16_0
#endif

#if ((GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 16) && \
     (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 19))
#define CONST_ABI_16_TO_19 const
#else
#define CONST_ABI_16_TO_19
#endif

#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 13)
static inline void
xf86SaveModeContents(DisplayModePtr intern, const DisplayModeRec *mode)
{
    *intern = *mode;
    intern->prev = intern->next = NULL;
    intern->name = NULL;
    intern->PrivSize = 0;
    intern->PrivFlags = 0;
    intern->Private = NULL;
}
#endif

#ifndef fbGetRotatedPixmap
#define fbGetRotatedPixmap(_pGC) NULL
#endif

#endif

