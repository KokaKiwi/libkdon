#include <string.h>
#include <glib.h>
#include <jansson.h>
#include "kdon.h"
#include "kdon_private.h"

static kdon_t *kdon_from_json(json_t *json);

static kdon_t *kdon_object_from_json(json_t *json)
{
    kdon_t *kdon = kdon_object();
    const char *key;
    json_t *value;

    json_object_foreach(json, key, value)
    {
        kdon_object_set_new(kdon, key, kdon_from_json(value));
    }

    return kdon;
}

static kdon_t *kdon_array_from_json(json_t *json)
{
    kdon_t *kdon = kdon_array();
    size_t index;
    json_t *value;

    json_array_foreach(json, index, value)
    {
        kdon_array_append_new(kdon, kdon_from_json(value));
    }

    return kdon;
}

static kdon_t *kdon_string_from_json(json_t *json)
{
    return kdon_string(json_string_value(json));
}

static kdon_t *kdon_integer_from_json(json_t *json)
{
    return kdon_integer(json_integer_value(json));
}

static kdon_t *kdon_real_from_json(json_t *json)
{
    return kdon_real(json_real_value(json));
}

static kdon_t *kdon_boolean_from_json(json_t *json)
{
    return kdon_boolean(json_is_true(json) ? TRUE : FALSE);
}

static kdon_t *kdon_null_from_json(json_t *json)
{
    return kdon_null();
}

static kdon_t *kdon_from_json(json_t *json)
{
    if (json_is_object(json))
    {
        return kdon_object_from_json(json);
    }
    else if (json_is_array(json))
    {
        return kdon_array_from_json(json);
    }
    else if (json_is_string(json))
    {
        return kdon_string_from_json(json);
    }
    else if (json_is_integer(json))
    {
        return kdon_integer_from_json(json);
    }
    else if (json_is_real(json))
    {
        return kdon_real_from_json(json);
    }
    else if (json_is_boolean(json))
    {
        return kdon_boolean_from_json(json);
    }
    else if (json_is_null(json))
    {
        return kdon_null_from_json(json);
    }
    return NULL;
}

kdon_t *kdon_decode_json(const char *data, size_t size, kdon_error_t *error)
{
    json_error_t err;
    json_t *json = json_loads(data, 0, &err);
    kdon_t *kdon = NULL;

    if (json)
    {
        kdon = kdon_from_json(json);
    }
    else
    {
        error->line = err.line;
        error->column = err.column;
        strncpy(error->source, err.source, sizeof(error->source));
        strncpy(error->text, err.text, sizeof(error->text));
    }

    json_decref(json);

    return kdon;
}
