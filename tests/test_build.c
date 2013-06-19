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

static kdon_t *create_config()
{
    kdon_t *config = kdon_object();

    kdon_object_set_new(config, "name", kdon_string("kdon"));
    kdon_object_set_new(config, "version", kdon_string("0.1.0"));
    kdon_object_set_new(config, "rev", kdon_integer(1));

    return config;
}

static kdon_t *create_inventory()
{
    kdon_t *inventory = kdon_array();

    kdon_array_append_new(inventory, kdon_string("Candle"));

    return inventory;
}

static kdon_t *create_player(const char *name, int level)
{
    kdon_t *player = kdon_object();

    kdon_object_set_new(player, "name", kdon_string(name));
    kdon_object_set_new(player, "level", kdon_integer(level));
    kdon_object_set_new(player, "inventory", create_inventory());
    kdon_object_set_new(player, "overhigh", kdon_boolean(level > 100));

    return player;
}

static kdon_t *create_players()
{
    kdon_t *players = kdon_array();

    kdon_array_append_new(players, create_player("Kevin", 20));
    kdon_array_append_new(players, create_player("Big Bowser", 123));
    kdon_array_append_new(players, create_player("Space", 42));

    return players;
}

static kdon_t *create_object()
{
    kdon_t *obj = kdon_object();

    kdon_object_set_new(obj, "config", create_config());
    kdon_object_set_new(obj, "players", create_players());

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
