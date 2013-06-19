#include <glib.h>
#include "kdon.h"
#include "kdon_private.h"

kdon_t *kdon_incref(kdon_t *kdon)
{
    if (kdon)
    {
        kdon->refcount++;
    }

    return kdon;
}

kdon_t *kdon_decref(kdon_t *kdon)
{
    if (kdon && kdon->refcount != (size_t) -1)
    {
        kdon->refcount--;
        if (kdon->refcount == 0)
        {
            kdon_delete(kdon);
        }
    }

    return kdon;
}
