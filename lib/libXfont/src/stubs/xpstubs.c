#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "stubs.h"

weak Bool
XpClientIsBitmapClient(ClientPtr client)
{
    return True;
}

weak Bool
XpClientIsPrintClient(ClientPtr client, FontPathElementPtr fpe)
{
    return False;
}
