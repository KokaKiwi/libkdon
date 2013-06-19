#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <msgpack.h>
#include "kdon.h"

void print_hex(const char * buffer, size_t size)
{
    size_t i;
    for (i = 0; i < size; i++)
    {
        printf("%02x", buffer[i] & 0xff);
    }
}

void kdon_print_msgpack(char *data, size_t size)
{
    printf("Data: ");
    print_hex(data, size);
    printf("\n");
    printf("Size: %zd\n", size);

    msgpack_unpacked msg;
    msgpack_unpacked_init(&msg);
    msgpack_unpack_next(&msg, data, size, NULL);
    msgpack_object_print(stdout, msg.data);
    msgpack_unpacked_destroy(&msg);
    printf("\n");
}

static kdon_t *create_object()
{
    kdon_t *obj = kdon_array();

    kdon_t *o = kdon_object();

    kdon_object_set_new(o, "name", kdon_string("toto"));

    kdon_array_append_new(obj, o);

    kdon_array_append_new(obj, kdon_string("toto"));
    kdon_array_append_new(obj, kdon_integer(45));
    kdon_array_append_new(obj, kdon_true());
    kdon_array_append_new(obj, kdon_false());
    kdon_array_append_new(obj, kdon_null());

    return obj;
}

int main(int argc, char **argv)
{
    kdon_t *obj = create_object();

    size_t size;
    char *str;

    printf("=== JSON ===\n");
    str = kdon_encode(obj, "json", &size);
    printf("%s\n", str);
    printf("Size: %zd\n", size);
    free(str);

    printf("\n");
    printf("=== MsgPack ===\n");
    str = kdon_encode(obj, "msgpack", &size);
    kdon_print_msgpack(str, size);
    free(str);

    printf("\n");
    printf("=== XML ===\n");
    str = kdon_encode(obj, "xml", NULL);
    printf("%s", str);
    free(str);

    printf("\n");
    printf("=== YAML ===\n");
    str = kdon_encode(obj, "yaml", NULL);
    printf("%s\n", str);
    free(str);

    kdon_decref(obj);

    return EXIT_SUCCESS;
}
