#include <string.h>
#include <glib.h>
#include <jansson.h>
#include "kdon.h"
#include "kdon_private.h"

static json_t *kdon_to_json(kdon_t *kdon);

static json_t *kdon_object_to_json(kdon_t *kdon, kdon_object_t *object)
{
    json_t *json = json_object();
    void *iter;
    const char *key;
    kdon_t *value;

    kdon_object_foreach(kdon, iter, key, value)
    {
        json_object_set_new(json, key, kdon_to_json(value));
    }
    g_free(iter);

    return json;
}

static json_t *kdon_array_to_json(kdon_t *kdon, kdon_array_t *array)
{
    json_t *json = json_array();
    size_t index;
    kdon_t *value;

    kdon_array_foreach(kdon, index, value)
    {
        json_array_append_new(json, kdon_to_json(value));
    }

    return json;
}

static json_t *kdon_string_to_json(kdon_t *kdon, kdon_string_t *string)
{
    json_t *json = json_string(string->value);

    return json;
}

static json_t *kdon_integer_to_json(kdon_t *kdon, kdon_integer_t *integer)
{
    json_t *json = json_integer(integer->value);

    return json;
}

static json_t *kdon_real_to_json(kdon_t *kdon, kdon_real_t *real)
{
    json_t *json = json_real(real->value);

    return json;
}

static json_t *kdon_boolean_to_json(kdon_t *kdon, kdon_boolean_t *boolean)
{
    json_t *json = json_boolean(boolean->value);

    return json;
}

static json_t *kdon_to_json(kdon_t *kdon)
{
    if (kdon_is_object(kdon))
    {
        return kdon_object_to_json(kdon, kdon_to_object(kdon));
    }
    else if (kdon_is_array(kdon))
    {
        return kdon_array_to_json(kdon, kdon_to_array(kdon));
    }
    else if (kdon_is_string(kdon))
    {
        return kdon_string_to_json(kdon, kdon_to_string(kdon));
    }
    else if (kdon_is_integer(kdon))
    {
        return kdon_integer_to_json(kdon, kdon_to_integer(kdon));
    }
    else if (kdon_is_real(kdon))
    {
        return kdon_real_to_json(kdon, kdon_to_real(kdon));
    }
    else if (kdon_is_boolean(kdon))
    {
        return kdon_boolean_to_json(kdon, kdon_to_boolean(kdon));
    }

    return json_null();
}

char *kdon_encode_json(kdon_t *kdon, size_t *size)
{
    json_t *json = kdon_to_json(kdon);
    char *str = json_dumps(json, JSON_INDENT(4));
    json_decref(json);

    if (size)
    {
        if (str)
        {
            *size = strlen(str);
        }
        else
        {
            *size = 0;
        }
    }

    return str;
}
