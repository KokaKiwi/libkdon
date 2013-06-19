#ifndef KDON_H_
#define KDON_H_

#include <glib.h>
#include <stdlib.h>
#include <stdint.h>

G_BEGIN_DECLS

#define KDON_VERSION "0.1.0"

typedef enum _kdon_type_t kdon_type_t;
enum _kdon_type_t
{
    KDON_OBJECT,
    KDON_ARRAY,
    KDON_STRING,
    KDON_INTEGER,
    KDON_REAL,
    KDON_BOOLEAN,
    KDON_NULL
};

typedef struct _kdon_t kdon_t;
struct _kdon_t
{
    kdon_type_t type;
    size_t refcount;
};

typedef uint64_t kdon_int_t;

#define kdon_typeof(kdon)       ((kdon)->type)
#define kdon_is_object(kdon)    ((kdon) && kdon_typeof(kdon) == KDON_OBJECT)
#define kdon_is_array(kdon)     ((kdon) && kdon_typeof(kdon) == KDON_ARRAY)
#define kdon_is_string(kdon)    ((kdon) && kdon_typeof(kdon) == KDON_STRING)
#define kdon_is_integer(kdon)   ((kdon) && kdon_typeof(kdon) == KDON_INTEGER)
#define kdon_is_real(kdon)      ((kdon) && kdon_typeof(kdon) == KDON_REAL)
#define kdon_is_number(kdon)    (kdon_is_integer(kdon) || kdon_is_real(kdon))
#define kdon_is_boolean(kdon)   ((kdon) && kdon_typeof(kdon) == KDON_BOOLEAN)
#define kdon_is_null(kdon)      ((kdon) && kdon_typeof(kdon) == KDON_NULL)

/* memory functions */
kdon_t *kdon_incref(kdon_t *kdon);
kdon_t *kdon_decref(kdon_t *kdon);

/* object functions */
kdon_t *kdon_object(void);
size_t kdon_object_size(const kdon_t *kdon);
kdon_t *kdon_object_get(const kdon_t *kdon, const char *key);
gboolean kdon_object_set_new(kdon_t *kdon, const char *key, kdon_t *value);
gboolean kdon_object_del(kdon_t *kdon, const char *key);
gboolean kdon_object_clear(kdon_t *kdon);
gboolean kdon_object_update(kdon_t *kdon, kdon_t *other);
void *kdon_object_iter_init(kdon_t *kdon);
gboolean kdon_object_iter_next(kdon_t *kdon, void *iter,
                               const char **key, kdon_t **value);
const char *kdon_object_iter_key(kdon_t *kdon, void *iter);
kdon_t *kdon_object_iter_value(kdon_t *kdon, void *iter);

#define kdon_object_set(kdon, key, value) \
    kdon_object_set_new(kdon, key, kdon_incref(value))

#define kdon_object_foreach(kdon, iter, key, value) \
    iter = kdon_object_iter_init(kdon); \
    while (kdon_object_iter_next(kdon, iter, &(key), &(value)))

/* array functions */
kdon_t *kdon_array(void);
size_t kdon_array_size(const kdon_t *kdon);
kdon_t *kdon_array_get(const kdon_t *kdon, size_t index);
gboolean kdon_array_append_new(kdon_t *kdon, kdon_t *value);
gboolean kdon_array_remove(kdon_t *kdon, size_t index);
gboolean kdon_array_clear(kdon_t *kdon);
gboolean kdon_array_extend(kdon_t *kdon, kdon_t *other);

#define kdon_array_append(kdon, value) \
    kdon_array_append_new(kdon, kdon_incref(value))

#define kdon_array_foreach(kdon, index, value) \
    for (index = 0; \
         index < kdon_array_size(kdon) && (value = kdon_array_get(kdon, index)); \
         index++)

/* literal functions */
kdon_t *kdon_string(const char *value);
kdon_t *kdon_integer(kdon_int_t value);
kdon_t *kdon_real(double value);
kdon_t *kdon_boolean(gboolean value);
kdon_t *kdon_null(void);

const char *kdon_string_value(kdon_t *kdon);
kdon_int_t kdon_integer_value(kdon_t *kdon);
double kdon_real_value(kdon_t *kdon);
double kdon_number_value(kdon_t *kdon);
gboolean kdon_boolean_value(kdon_t *kdon);

gboolean kdon_string_set(kdon_t *kdon, const char *value);
gboolean kdon_integer_set(kdon_t *kdon, kdon_int_t value);
gboolean kdon_real_set(kdon_t *kdon, double value);
gboolean kdon_boolean_set(kdon_t *kdon, gboolean value);

#define kdon_true()     (kdon_boolean(TRUE))
#define kdon_false()    (kdon_boolean(FALSE))

/* misc functions */
const char *kdon_type_str(kdon_t *kdon);

/* encode functions */
char *kdon_encode(kdon_t *kdon, const char *format, size_t *size);

G_END_DECLS

#endif /* KDON_H_ */
