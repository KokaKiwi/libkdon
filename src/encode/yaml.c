#include <glib.h>
#include <yaml.h>
#include "kdon.h"
#include "kdon_private.h"

static void kdon_to_yaml(kdon_t *kdon, yaml_emitter_t *emitter);

static void kdon_emit(yaml_emitter_t *emitter, yaml_event_t *event)
{
    if (!yaml_emitter_emit(emitter, event))
    {
        g_error("YAML encoding error.");
    }
}

static void kdon_object_to_yaml(kdon_t *kdon, yaml_emitter_t *emitter)
{
    yaml_event_t event;
    void *iter;
    const char *key;
    kdon_t *value;

    yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_ANY_MAPPING_STYLE);
    kdon_emit(emitter, &event);

    kdon_object_foreach(kdon, iter, key, value)
    {
        yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *) key, strlen(key), 1, 1, YAML_PLAIN_SCALAR_STYLE);
        kdon_emit(emitter, &event);
        kdon_to_yaml(value, emitter);
    }
    g_free(iter);

    yaml_mapping_end_event_initialize(&event);
    kdon_emit(emitter, &event);
}

static void kdon_array_to_yaml(kdon_t *kdon, yaml_emitter_t *emitter)
{
    yaml_event_t event;
    size_t index;
    kdon_t *value;

    yaml_sequence_start_event_initialize(&event, NULL, NULL, 1, YAML_ANY_SEQUENCE_STYLE);
    kdon_emit(emitter, &event);

    kdon_array_foreach(kdon, index, value)
    {
        kdon_to_yaml(value, emitter);
    }

    yaml_sequence_end_event_initialize(&event);
    kdon_emit(emitter, &event);
}

static void kdon_string_to_yaml(kdon_t *kdon, kdon_string_t *string, yaml_emitter_t *emitter)
{
    yaml_event_t event;

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *) string->value, strlen(string->value), 1, 1, YAML_DOUBLE_QUOTED_SCALAR_STYLE);
    kdon_emit(emitter, &event);
}

static void kdon_integer_to_yaml(kdon_t *kdon, kdon_integer_t *integer, yaml_emitter_t *emitter)
{
    yaml_event_t event;
    char value[32];

    sprintf(value, "%ld", integer->value);
    yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *) "int", (yaml_char_t *) value, strlen(value), 0, 0, YAML_ANY_SCALAR_STYLE);
    kdon_emit(emitter, &event);
}

static void kdon_real_to_yaml(kdon_t *kdon, kdon_real_t *real, yaml_emitter_t *emitter)
{
    yaml_event_t event;
    char value[32];

    sprintf(value, "%f", real->value);
    yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *) "real", (yaml_char_t *) value, strlen(value), 0, 0, YAML_ANY_SCALAR_STYLE);
    kdon_emit(emitter, &event);
}

static void kdon_boolean_to_yaml(kdon_t *kdon, kdon_boolean_t *boolean, yaml_emitter_t *emitter)
{
    yaml_event_t event;
    char *value = (boolean->value ? "true" : "false");

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *) value, strlen(value), 1, 1, YAML_ANY_SCALAR_STYLE);
    kdon_emit(emitter, &event);
}

static void kdon_null_to_yaml(kdon_t *kdon, yaml_emitter_t *emitter)
{
    yaml_event_t event;
    char *value = "null";

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *) value, strlen(value), 1, 1, YAML_ANY_SCALAR_STYLE);
    kdon_emit(emitter, &event);
}

static void kdon_to_yaml(kdon_t *kdon, yaml_emitter_t *emitter)
{
    if (kdon_is_object(kdon))
    {
        kdon_object_to_yaml(kdon, emitter);
    }
    else if (kdon_is_array(kdon))
    {
        kdon_array_to_yaml(kdon, emitter);
    }
    else if (kdon_is_string(kdon))
    {
        kdon_string_to_yaml(kdon, kdon_to_string(kdon), emitter);
    }
    else if (kdon_is_integer(kdon))
    {
        kdon_integer_to_yaml(kdon, kdon_to_integer(kdon), emitter);
    }
    else if (kdon_is_real(kdon))
    {
        kdon_real_to_yaml(kdon, kdon_to_real(kdon), emitter);
    }
    else if (kdon_is_boolean(kdon))
    {
        kdon_boolean_to_yaml(kdon, kdon_to_boolean(kdon), emitter);
    }
    else if (kdon_is_null(kdon))
    {
        kdon_null_to_yaml(kdon, emitter);
    }
}

static int kdon_yaml_write(void *ext, unsigned char *buffer, size_t size)
{
    GString *string = (GString *) ext;

    g_string_append_len(string, (const gchar *) buffer, size);

    return 1;
}

char *kdon_encode_yaml(kdon_t *kdon, size_t *size)
{
    GString *string = g_string_new("");
    yaml_emitter_t emitter;
    yaml_event_t event;

    yaml_emitter_initialize(&emitter);
    yaml_emitter_set_output(&emitter, kdon_yaml_write, string);

    yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
    kdon_emit(&emitter, &event);
    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 1);
    kdon_emit(&emitter, &event);

    kdon_to_yaml(kdon, &emitter);

    yaml_document_end_event_initialize(&event, 1);
    kdon_emit(&emitter, &event);
    yaml_stream_end_event_initialize(&event);
    kdon_emit(&emitter, &event);

    yaml_emitter_delete(&emitter);
    if (size)
    {
        *size = string->len;
    }

    return g_string_free(string, FALSE);
}
