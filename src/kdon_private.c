#include <glib.h>
#include "kdon.h"
#include "kdon_private.h"

void kdon_init(kdon_t *kdon, kdon_type_t type)
{
    kdon->type = type;
    kdon->refcount = 1;
}

void kdon_delete(kdon_t *kdon)
{
    if (kdon_is_object(kdon))
    {
        kdon_object_delete(kdon_to_object(kdon));
    }
    else if (kdon_is_array(kdon))
    {
        kdon_array_delete(kdon_to_array(kdon));
    }
    else if (kdon_is_string(kdon))
    {
        kdon_string_delete(kdon_to_string(kdon));
    }
    else if (kdon_is_integer(kdon))
    {
        kdon_integer_delete(kdon_to_integer(kdon));
    }
    else if (kdon_is_real(kdon))
    {
        kdon_real_delete(kdon_to_real(kdon));
    }
    else if (kdon_is_boolean(kdon))
    {
        kdon_boolean_delete(kdon_to_boolean(kdon));
    }
    else
    {
        g_free(kdon);
    }
}
