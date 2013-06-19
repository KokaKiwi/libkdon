#include <glib.h>
#include "kdon.h"
#include "kdon_private.h"

kdon_t *kdon_string(const char *value)
{
    kdon_string_t *string = g_new(kdon_string_t, 1);

    kdon_init(&(string->kdon), KDON_STRING);
    string->value = g_strdup(value);

    return &(string->kdon);
}

kdon_t *kdon_integer(kdon_int_t value)
{
    kdon_integer_t *integer = g_new(kdon_integer_t, 1);

    kdon_init(&(integer->kdon), KDON_INTEGER);
    integer->value = value;

    return &(integer->kdon);
}

kdon_t *kdon_real(double value)
{
    kdon_real_t *real = g_new(kdon_real_t, 1);

    kdon_init(&(real->kdon), KDON_REAL);
    real->value = value;

    return &(real->kdon);
}

kdon_t *kdon_boolean(gboolean value)
{
    kdon_boolean_t *boolean = g_new(kdon_boolean_t, 1);

    kdon_init(&(boolean->kdon), KDON_BOOLEAN);
    boolean->value = value;

    return &(boolean->kdon);
}

kdon_t *kdon_null(void)
{
    kdon_t *kdon = g_new(kdon_t, 1);

    kdon_init(kdon, KDON_NULL);

    return kdon;
}

const char *kdon_string_value(kdon_t *kdon)
{
    kdon_string_t *string;

    if (!kdon || !kdon_is_string(kdon))
    {
        return NULL;
    }
    string = kdon_to_string(kdon);

    return string->value;
}

kdon_int_t kdon_integer_value(kdon_t *kdon)
{
    kdon_integer_t *integer;

    if (!kdon || !kdon_is_integer(kdon))
    {
        return 0;
    }
    integer = kdon_to_integer(kdon);

    return integer->value;
}

double kdon_real_value(kdon_t *kdon)
{
    kdon_real_t *real;

    if (!kdon || !kdon_is_real(kdon))
    {
        return 0;
    }
    real = kdon_to_real(kdon);

    return real->value;
}

double kdon_number_value(kdon_t *kdon)
{
    if (kdon_is_integer(kdon))
    {
        return (double) kdon_integer_value(kdon);
    }
    else if (kdon_is_real(kdon))
    {
        return kdon_real_value(kdon);
    }

    return 0;
}

gboolean kdon_boolean_value(kdon_t *kdon)
{
    kdon_boolean_t *boolean;

    if (!kdon || !kdon_is_boolean(kdon))
    {
        return FALSE;
    }
    boolean = kdon_to_boolean(kdon);

    return boolean->value;
}

gboolean kdon_string_set(kdon_t *kdon, const char *value)
{
    kdon_string_t *string;

    if (!kdon || !kdon_is_string(kdon))
    {
        return FALSE;
    }
    string = kdon_to_string(kdon);

    g_free(string->value);
    string->value = g_strdup(value);
    return TRUE;
}

gboolean kdon_integer_set(kdon_t *kdon, kdon_int_t value)
{
    kdon_integer_t *integer;

    if (!kdon || !kdon_is_integer(kdon))
    {
        return FALSE;
    }
    integer = kdon_to_integer(kdon);

    integer->value = value;
    return TRUE;
}

gboolean kdon_real_set(kdon_t *kdon, double value)
{
    kdon_real_t *real;

    if (!kdon || !kdon_is_real(kdon))
    {
        return FALSE;
    }
    real = kdon_to_real(kdon);

    real->value = value;
    return TRUE;
}

gboolean kdon_boolean_set(kdon_t *kdon, gboolean value)
{
    kdon_boolean_t *boolean;

    if (!kdon || !kdon_is_boolean(kdon))
    {
        return FALSE;
    }
    boolean = kdon_to_boolean(kdon);

    boolean->value = value;
    return TRUE;
}

void kdon_string_delete(kdon_string_t *string)
{
    g_free(string->value);
    g_free(string);
}

void kdon_integer_delete(kdon_integer_t *integer)
{
    g_free(integer);
}

void kdon_real_delete(kdon_real_t *real)
{
    g_free(real);
}

void kdon_boolean_delete(kdon_boolean_t *boolean)
{
    g_free(boolean);
}
