#ifndef _CLIENT
#define _CLIENT

#include "binding.h"

#if defined(__cplusplus)
    extern "C" {
#endif

typedef void* LPBIOSREGS;
#define ASSERT_HANDLER(pBoard)

typedef struct TAGCLIENTDATA
{
    ULONG          ulFrameBufferBase;
    ULONG          ulRegisterBase;
    pointer        pMga;
} CLIENTDATA, *LPCLIENTDATA;

#if defined(__cplusplus)
    }
#endif

#define ESC_CUSTOM_SET_FUNCTION_PTR     0x80000200
#define ESC_CUSTOM_GET_FUNCTION_PTR     0x80000201
#define ESC_CUSTOM_PATCH_HSL            0x80000202

#endif
