#ifndef KDON_PRIVATE_H_
#define KDON_PRIVATE_H_

#include <glib.h>
#include "kdon.h"
#include "kdon_config.h"

G_BEGIN_DECLS

#define container_of(ptr_, type_, member_)  \
    ((type_ *)((char *)ptr_ - offsetof(type_, member_)))

#define kdon_typedef(type)\
    typedef struct _kdon_##type##_t kdon_##type##_t;\
    struct _kdon_##type##_t

kdon_typedef(object)
{
    kdon_t kdon;
    GHashTable *table;
};

kdon_typedef(array)
{
    kdon_t kdon;
    GPtrArray *array;
};

kdon_typedef(string)
{
    kdon_t kdon;
    char *value;
};

kdon_typedef(integer)
{
    kdon_t kdon;
    kdon_int_t value;
};

kdon_typedef(real)
{
    kdon_t kdon;
    double value;
};

kdon_typedef(boolean)
{
    kdon_t kdon;
    gboolean value;
};

/* type conversion macros */
#define kdon_to_type(_kdon, type)   container_of(_kdon, type, kdon)
#define kdon_to_object(kdon)        kdon_to_type(kdon, kdon_object_t)
#define kdon_to_array(kdon)         kdon_to_type(kdon, kdon_array_t)
#define kdon_to_string(kdon)        kdon_to_type(kdon, kdon_string_t)
#define kdon_to_integer(kdon)       kdon_to_type(kdon, kdon_integer_t)
#define kdon_to_real(kdon)          kdon_to_type(kdon, kdon_real_t)
#define kdon_to_boolean(kdon)       kdon_to_type(kdon, kdon_boolean_t)

/* KDON private functions */
void kdon_init(kdon_t *kdon, kdon_type_t type);
void kdon_delete(kdon_t *kdon);

/* object private functions */
void kdon_object_delete(kdon_object_t *object);

/* array private functions */
void kdon_array_delete(kdon_array_t *array);

/* literal private functions */
void kdon_string_delete(kdon_string_t *string);
void kdon_integer_delete(kdon_integer_t *integer);
void kdon_real_delete(kdon_real_t *real);
void kdon_boolean_delete(kdon_boolean_t *boolean);

/* encoding private functions */
#ifdef HAVE_JANSSON
char *kdon_encode_json(kdon_t *kdon, size_t *size);
#endif
#ifdef HAVE_MSGPACK
char *kdon_encode_msgpack(kdon_t *kdon, size_t *size);
#endif
#ifdef HAVE_LIBXML2
char *kdon_encode_xml(kdon_t *kdon, size_t *size);
#endif
#ifdef HAVE_LIBYAML
char *kdon_encode_yaml(kdon_t *kdon, size_t *size);
#endif

G_END_DECLS

#endif /* KDON_PRIVATE_H_ */
