#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "kdon.h"

static char *json_source = "[{\"name\":\"toto\"}, \"toto\", 45, true, false, null]";

int main(int argc, char **argv)
{
    kdon_t *kdon = kdon_decode(json_source, "json", strlen(json_source), NULL);
    char *str = NULL;

    if (kdon)
    {
        str = kdon_encode(kdon, "xml", NULL);
    }

    printf("%s", str);

    g_free(str);
    kdon_decref(kdon);

    return EXIT_SUCCESS;
}
