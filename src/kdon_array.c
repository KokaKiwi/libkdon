#include <glib.h>
#include "kdon.h"
#include "kdon_private.h"

static void kdon_array_delete_item(gpointer data)
{
    kdon_decref((kdon_t *) data);
}

kdon_t *kdon_array(void)
{
    kdon_array_t *array = g_new(kdon_array_t, 1);

    kdon_init(&(array->kdon), KDON_ARRAY);
    array->array = g_ptr_array_new_with_free_func(kdon_array_delete_item);

    return &(array->kdon);
}

size_t kdon_array_size(const kdon_t *kdon)
{
    kdon_array_t *array;

    if (!kdon || !kdon_is_array(kdon))
    {
        return 0;
    }
    array = kdon_to_array(kdon);

    return array->array->len;
}

kdon_t *kdon_array_get(const kdon_t *kdon, size_t index)
{
    kdon_array_t *array;

    if (!kdon || !kdon_is_array(kdon))
    {
        return NULL;
    }
    array = kdon_to_array(kdon);

    return (kdon_t *) g_ptr_array_index(array->array, index);
}

gboolean kdon_array_append_new(kdon_t *kdon, kdon_t *value)
{
    kdon_array_t *array;

    if (!kdon || !kdon_is_array(kdon) || kdon == value)
    {
        kdon_decref(value);
        return FALSE;
    }
    array = kdon_to_array(kdon);

    g_ptr_array_add(array->array, value);

    return TRUE;
}

gboolean kdon_array_remove(kdon_t *kdon, size_t index)
{
    kdon_array_t *array;

    if (!kdon || !kdon_is_array(kdon))
    {
        return FALSE;
    }
    array = kdon_to_array(kdon);

    g_ptr_array_remove_index(array->array, index);
    return TRUE;
}

gboolean kdon_array_clear(kdon_t *kdon)
{
    kdon_array_t *array;

    if (!kdon || !kdon_is_array(kdon))
    {
        return FALSE;
    }
    array = kdon_to_array(kdon);

    g_ptr_array_remove_range(array->array, 0, array->array->len);
    return TRUE;
}

gboolean kdon_array_extend(kdon_t *kdon, kdon_t *other)
{
    kdon_array_t *array;
    size_t index;
    kdon_t *value;

    if (!kdon || !kdon_is_array(kdon))
    {
        return FALSE;
    }
    array = kdon_to_array(kdon);

    kdon_array_foreach(other, index, value)
    {
        g_ptr_array_add(array->array, value);
    }
    return TRUE;
}

void kdon_array_delete(kdon_array_t *array)
{
    g_ptr_array_free(array->array, TRUE);
    g_free(array);
}
