/*
 * Test helper: wraps the static parseEncodingFile() from encparse.c
 * so Python tests can call it via ctypes.
 *
 * We include encparse.c directly to get access to the static function.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <zlib.h>

#include <X11/fonts/fontenc.h>
#include "fontencI.h"
#include "reallocarray.h"

/* Pull in encparse.c to get access to its static functions */
#include "encparse.c"

/*
 * test_parse_encoding_file: open a (possibly gzipped) encoding file
 * and parse it.  Returns 1 on success, 0 on failure (NULL encoding),
 * -1 on file open error.
 *
 * On success, if out_row_size is not NULL, the encoding's row_size
 * is written there.
 */
int test_parse_encoding_file(const char *filename, int *out_row_size)
{
    gzFile f = gzopen(filename, "rb");
    if (!f)
        return -1;

    FontEncPtr enc = parseEncodingFile(f, 0);
    gzclose(f);

    if (!enc)
        return 0;

    if (out_row_size)
        *out_row_size = enc->row_size;

    /* Walk and free all mappings */
    FontMapPtr map = enc->mappings;
    while (map) {
        FontMapPtr next = map->next;
        free(map->client_data);
        free(map);
        map = next;
    }

    /* Free aliases */
    if (enc->aliases) {
        char **a;
        for (a = enc->aliases; *a; a++)
            free(*a);
        free(enc->aliases);
    }

    free(enc->name);
    free(enc);

    return 1;
}
