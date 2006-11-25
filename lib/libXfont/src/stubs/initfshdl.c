#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "stubs.h"

weak int
init_fs_handlers(FontPathElementPtr fpe,
                 BlockHandlerProcPtr block_handler)
{
  return Successful;
}
