#include <glib.h>
#include <msgpack.h>
#include "kdon.h"
#include "kdon_private.h"

static void kdon_to_msgpack(kdon_t *kdon, msgpack_packer *packer);

static void kdon_object_to_msgpack(kdon_t *kdon, msgpack_packer *packer)
{
    size_t size = kdon_object_size(kdon);
    void *iter;
    const char *key;
    kdon_t *value;

    msgpack_pack_map(packer, size);

    kdon_object_foreach(kdon, iter, key, value)
    {
        msgpack_pack_raw(packer, strlen(key));
        msgpack_pack_raw_body(packer, key, strlen(key));
        kdon_to_msgpack(value, packer);
    }
    g_free(iter);
}

static void kdon_array_to_msgpack(kdon_t *kdon, msgpack_packer *packer)
{
    size_t size = kdon_array_size(kdon);
    size_t index;
    kdon_t *value;

    msgpack_pack_array(packer, size);

    kdon_array_foreach(kdon, index, value)
    {
        kdon_to_msgpack(value, packer);
    }
}

static void kdon_string_to_msgpack(kdon_t *kdon, kdon_string_t *string, msgpack_packer *packer)
{
    msgpack_pack_raw(packer, strlen(string->value));
    msgpack_pack_raw_body(packer, string->value, strlen(string->value));
}

static void kdon_integer_to_msgpack(kdon_t *kdon, kdon_integer_t *integer, msgpack_packer *packer)
{
    msgpack_pack_uint64(packer, integer->value);
}

static void kdon_real_to_msgpack(kdon_t *kdon, kdon_real_t *real, msgpack_packer *packer)
{
    msgpack_pack_double(packer, real->value);
}

static void kdon_boolean_to_msgpack(kdon_t *kdon, kdon_boolean_t *boolean, msgpack_packer *packer)
{
    if (boolean->value)
    {
        msgpack_pack_true(packer);
    }
    else
    {
        msgpack_pack_false(packer);
    }
}

static void kdon_null_to_msgpack(kdon_t *kdon, msgpack_packer *packer)
{
    msgpack_pack_nil(packer);
}

static void kdon_to_msgpack(kdon_t *kdon, msgpack_packer *packer)
{
    if (kdon_is_object(kdon))
    {
        kdon_object_to_msgpack(kdon, packer);
    }
    else if (kdon_is_array(kdon))
    {
        kdon_array_to_msgpack(kdon, packer);
    }
    else if (kdon_is_string(kdon))
    {
        kdon_string_to_msgpack(kdon, kdon_to_string(kdon), packer);
    }
    else if (kdon_is_integer(kdon))
    {
        kdon_integer_to_msgpack(kdon, kdon_to_integer(kdon), packer);
    }
    else if (kdon_is_real(kdon))
    {
        kdon_real_to_msgpack(kdon, kdon_to_real(kdon), packer);
    }
    else if (kdon_is_boolean(kdon))
    {
        kdon_boolean_to_msgpack(kdon, kdon_to_boolean(kdon), packer);
    }
    else if (kdon_is_null(kdon))
    {
        kdon_null_to_msgpack(kdon, packer);
    }
}

char *kdon_encode_msgpack(kdon_t *kdon, size_t *size)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    msgpack_packer packer;
    msgpack_packer_init(&packer, &sbuf, msgpack_sbuffer_write);

    kdon_to_msgpack(kdon, &packer);

    if (size)
    {
        *size = sbuf.size;
    }

    return sbuf.data;
}
