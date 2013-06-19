#include <glib.h>
#include "kdon.h"
#include "kdon_private.h"

const char *kdon_type_str(kdon_t *kdon)
{
    if (!kdon)
    {
        return NULL;
    }

    switch (kdon->type)
    {
        case KDON_OBJECT:
            return "object";
        case KDON_ARRAY:
            return "array";
        case KDON_STRING:
            return "string";
        case KDON_INTEGER:
            return "integer";
        case KDON_REAL:
            return "real";
        case KDON_BOOLEAN:
            return "boolean";
        case KDON_NULL:
        default:
            return "null";
    }
}
