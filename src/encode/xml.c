#include <glib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "kdon.h"
#include "kdon_private.h"

static xmlNodePtr kdon_to_xml(kdon_t *kdon);

static xmlNodePtr kdon_object_to_xml(kdon_t *kdon)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "object");
    void *iter;
    const char *key;
    kdon_t *value;

    kdon_object_foreach(kdon, iter, key, value)
    {
        xmlNodePtr item = xmlNewNode(NULL, BAD_CAST key);

        xmlAddChild(item, kdon_to_xml(value));

        xmlAddChild(node, item);
    }
    g_free(iter);

    return node;
}

static xmlNodePtr kdon_array_to_xml(kdon_t *kdon)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "array");
    size_t index;
    kdon_t *value;

    kdon_array_foreach(kdon, index, value)
    {
        xmlAddChild(node, kdon_to_xml(value));
    }

    return node;
}

static xmlNodePtr kdon_string_to_xml(kdon_t *kdon, kdon_string_t *string)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "string");

    xmlNewProp(node, BAD_CAST "value", BAD_CAST string->value);

    return node;
}

static xmlNodePtr kdon_integer_to_xml(kdon_t *kdon, kdon_integer_t *integer)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "integer");
    char value[32];

    sprintf(value, "%ld", integer->value);
    xmlNewProp(node, BAD_CAST "value", BAD_CAST value);

    return node;
}

static xmlNodePtr kdon_real_to_xml(kdon_t *kdon, kdon_real_t *real)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "real");
    char value[32];

    sprintf(value, "%f", real->value);
    xmlNewProp(node, BAD_CAST "value", BAD_CAST value);

    return node;
}

static xmlNodePtr kdon_boolean_to_xml(kdon_t *kdon, kdon_boolean_t *boolean)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "boolean");

    xmlNewProp(node, BAD_CAST "value", BAD_CAST (boolean->value ? "true" : "false"));

    return node;
}

static xmlNodePtr kdon_null_to_xml(kdon_t *kdon)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "null");

    return node;
}

static xmlNodePtr kdon_to_xml(kdon_t *kdon)
{
    if (kdon_is_object(kdon))
    {
        return kdon_object_to_xml(kdon);
    }
    else if (kdon_is_array(kdon))
    {
        return kdon_array_to_xml(kdon);
    }
    else if (kdon_is_string(kdon))
    {
        return kdon_string_to_xml(kdon, kdon_to_string(kdon));
    }
    else if (kdon_is_integer(kdon))
    {
        return kdon_integer_to_xml(kdon, kdon_to_integer(kdon));
    }
    else if (kdon_is_real(kdon))
    {
        return kdon_real_to_xml(kdon, kdon_to_real(kdon));
    }
    else if (kdon_is_boolean(kdon))
    {
        return kdon_boolean_to_xml(kdon, kdon_to_boolean(kdon));
    }
    else if (kdon_is_null(kdon))
    {
        return kdon_null_to_xml(kdon);
    }
    return NULL;
}

char *kdon_encode_xml(kdon_t *kdon, size_t *size)
{
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
    char *str;
    int isize;

    xmlDocSetRootElement(doc, kdon_to_xml(kdon));

    xmlDocDumpFormatMemory(doc, (xmlChar **) &str, &isize, 1);
    xmlFreeDoc(doc);

    if (size)
    {
        *size = isize;
    }

    return str;
}
