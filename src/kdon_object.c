#include <glib.h>
#include "kdon.h"
#include "kdon_private.h"

static void kdon_object_delete_item(gpointer data)
{
    kdon_decref((kdon_t *) data);
}

kdon_t *kdon_object(void)
{
    kdon_object_t *object = g_new(kdon_object_t, 1);

    kdon_init(&(object->kdon), KDON_OBJECT);
    object->table = g_hash_table_new_full(g_str_hash, g_str_equal,
                                          g_free, kdon_object_delete_item);

    return &(object->kdon);
}

size_t kdon_object_size(const kdon_t *kdon)
{
    kdon_object_t *object;

    if (!kdon || !kdon_is_object(kdon))
    {
        return 0;
    }
    object = kdon_to_object(kdon);
    return g_hash_table_size(object->table);
}

kdon_t *kdon_object_get(const kdon_t *kdon, const char *key)
{
    kdon_object_t *object;

    if (!kdon || !kdon_is_object(kdon))
    {
        return NULL;
    }
    object = kdon_to_object(kdon);
    return (kdon_t *) g_hash_table_lookup(object->table, key);
}

gboolean kdon_object_set_new(kdon_t *kdon, const char *key, kdon_t *value)
{
    kdon_object_t *object;

    if (!kdon || !kdon_is_object(kdon))
    {
        return FALSE;
    }
    object = kdon_to_object(kdon);

    g_hash_table_replace(object->table, g_strdup(key), value);
    return TRUE;
}

gboolean kdon_object_del(kdon_t *kdon, const char *key)
{
    kdon_object_t *object;

    if (!kdon || !kdon_is_object(kdon))
    {
        return FALSE;
    }
    object = kdon_to_object(kdon);

    g_hash_table_remove(object->table, key);
    return TRUE;
}

gboolean kdon_object_clear(kdon_t *kdon)
{
    kdon_object_t *object;

    if (!kdon || !kdon_is_object(kdon))
    {
        return FALSE;
    }
    object = kdon_to_object(kdon);

    g_hash_table_remove_all(object->table);
    return TRUE;
}

gboolean kdon_object_update(kdon_t *kdon, kdon_t *other)
{
    void *iter;
    const char *key;
    kdon_t *value;

    if (!kdon || !kdon_is_object(kdon)
        || !other || !kdon_is_object(other))
    {
        return FALSE;
    }

    kdon_object_foreach(other, iter, key, value)
    {
        if (!kdon_object_set(kdon, key, value))
        {
            return FALSE;
        }
    }

    return TRUE;
}

void *kdon_object_iter_init(kdon_t *kdon)
{
    kdon_object_t *object;
    GHashTableIter *iter;

    if (!kdon || !kdon_is_object(kdon))
    {
        return NULL;
    }
    object = kdon_to_object(kdon);

    iter = g_new(GHashTableIter, 1);
    g_hash_table_iter_init(iter, object->table);

    return iter;
}

gboolean kdon_object_iter_next(kdon_t *kdon, void *iter,
                               const char **key, kdon_t **value)
{
    kdon_object_t *object;
    GHashTableIter *i = (GHashTableIter *) iter;

    if (!kdon || !i || !kdon_is_object(kdon))
    {
        return FALSE;
    }
    object = kdon_to_object(kdon);

    return g_hash_table_iter_next(i, (gpointer *) key,
                                  (gpointer *) value);
}

const char *kdon_object_iter_key(kdon_t *kdon, void *iter)
{
    const char *key = NULL;

    if (!kdon_object_iter_next(kdon, iter, &key, NULL))
    {
        return NULL;
    }

    return key;
}

kdon_t *kdon_object_iter_value(kdon_t *kdon, void *iter)
{
    kdon_t *value = NULL;

    if (!kdon_object_iter_next(kdon, iter, NULL, &value))
    {
        return NULL;
    }

    return value;
}

void kdon_object_delete(kdon_object_t *object)
{
    g_hash_table_unref(object->table);
    g_free(object);
}
