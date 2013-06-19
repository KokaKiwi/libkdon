#include <glib.h>
#include <string.h>
#include "kdon.h"
#include "kdon_private.h"

char *kdon_encode(kdon_t *kdon, const char *format, size_t *size)
{
#ifdef HAVE_JANSSON
    if (strcmp(format, "json") == 0)
    {
        return kdon_encode_json(kdon, size);
    }
#endif
#ifdef HAVE_MSGPACK
    if (strcmp(format, "msgpack") == 0)
    {
        return kdon_encode_msgpack(kdon, size);
    }
#endif
#ifdef HAVE_LIBXML2
    if (strcmp(format, "xml") == 0)
    {
        return kdon_encode_xml(kdon, size);
    }
#endif
#ifdef HAVE_LIBXML2
    if (strcmp(format, "yaml") == 0)
    {
        return kdon_encode_yaml(kdon, size);
    }
#endif
    return NULL;
}
