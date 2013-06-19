#include <glib.h>
#include <string.h>
#include "kdon.h"
#include "kdon_private.h"

kdon_t *kdon_decode(const char *data, const char *format, size_t size, kdon_error_t *error)
{
#ifdef HAVE_JANSSON
    if (strcmp(format, "json") == 0)
    {
        return kdon_decode_json(data, size, error);
    }
#endif
    return NULL;
}
