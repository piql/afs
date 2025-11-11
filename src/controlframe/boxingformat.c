/*****************************************************************************
**
**  Implementation of the boxing format interface
**
**  Creation date:  2017/02/08
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "controlframe/boxingformat.h"
#include "boxing/globals.h"
#include "boxing/log.h"
#include "boxing/graphics/genericframefactory.h"
#include "boxing/codecs/codecdispatcher.h"
#include "boxing/math/math.h"
#include "mxml.h"

static void         set_class_from_xml(boxing_config * config, struct mxml_node_s * dom_node);
static DBOOL        save_config_xml(mxml_node_t * out, boxing_config * config);
static const char * whitespace_cb(mxml_node_t *node, int where);
static void         initialize_instance(afs_boxing_format * boxing_format);
static const char * get_name(const afs_boxing_format * format);
static unsigned int get_bytes_per_frame(const afs_boxing_format * format, boxing_frame * frame);
static unsigned int get_data_bytes_per_frame(const afs_boxing_format * format, boxing_frame * frame);
static unsigned int get_data_stripe_size(const afs_boxing_format * format);
static unsigned int get_scaling_factor(const afs_boxing_format * boxing_format);
static unsigned int get_width(boxing_frame * frame);
static unsigned int get_height(boxing_frame * frame);
static unsigned int get_data_frame_width(boxing_frame * frame);
static unsigned int get_data_frame_height(boxing_frame * frame);
static unsigned int get_bits_per_pixel(afs_boxing_format * boxing_format, boxing_frame * frame);
static unsigned int get_symbol_per_pixel(afs_boxing_format * boxing_format);
static unsigned int get_ceil(const size_t dividend, const unsigned int divisor);


//----------------------------------------------------------------------------
/*!
 *  \defgroup   boxingformat Boxing format
 *  \brief      XML based description of frame coding and decoding.
 *  \ingroup    afs
 *
 *  The boxing format controls the layout and properties of a frame in the 
 *  AFS file system. It is therefore the configuration object when boxing and unboxing 
 *  frames. The AFS library has a set of predefined formats, used when decoding 
 *  the control frame. The control frame itself contains another boxing format,
 *  used when unboxing the rest of the reel.
 */


/*! 
  * \addtogroup boxingformat
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_boxing_format_s  boxingformat.h
 *  \brief      Boxing format parameters
 *
 *  \param config  boxing_config containing key/value pairs with all boxing parameters
 *
 */

//----------------------------------------------------------------------------
/*!
 *  \brief Create an boxing format instance.
 *
 *  Allocate memory for the afs_boxing_format structure
 *  and internal boxing_config structure.
 *  Return instance of allocated afs_boxing_format structure.
 *
 *  \return instance of allocated afs_boxing_format structure.
 */

afs_boxing_format * afs_boxing_format_create(void)
{
    afs_boxing_format * boxing_format = malloc(sizeof(afs_boxing_format));

    boxing_format->config = NULL;
    initialize_instance(boxing_format);

    boxing_format->reference_count = 1;

    return boxing_format;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an boxing format instance from specified boxing_config structure.
 *
 *  Allocate memory for the afs_boxing_format structure
 *  and internal boxing_config structure and initialize it by cloning the given config.
 *  Return instance of allocated afs_boxing_format structure.
 *
 *  \param[in]  config  Pointer to the boxing_config structure.
 *  \return instance of allocated afs_boxing_format structure.
 */

afs_boxing_format * afs_boxing_format_create2(const boxing_config * config)
{
    afs_boxing_format * boxing_format = malloc(sizeof(afs_boxing_format));
    afs_boxing_format_init2(boxing_format, config);
    return boxing_format;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initialize.
 *
 *  Initialize input structure pointer with NULL values.
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  boxing_format  Pointer to the afs_boxing_format structure.
 */

void afs_boxing_format_init(afs_boxing_format* boxing_format)
{
    if (boxing_format == NULL)
    {
        return;
    }

    boxing_format->config = NULL;

    boxing_format->reference_count = 1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initialize boxing_config structure pointer.
 *
 *  Initialize input boxing_config structure pointer with specified boxing_config structure.
 *  If one of the input pointers is NULL, then return without initialization.
 *
 *  \param[in]  boxing_format  Pointer to the afs_boxing_format structure.
 *  \param[in]  config         Pointer to the boxing_config structure.
 */

void afs_boxing_format_init2(afs_boxing_format* boxing_format, const boxing_config * config)
{
    if (boxing_format == NULL)
    {
        return;
    }

    boxing_format->config = boxing_config_clone(config);
    initialize_instance(boxing_format);

    boxing_format->reference_count = 1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of boxing format structure.
 *
 *  Frees occupied memory of all internal structure pointer and structure pointer.
 *
 *  \param[in]  boxing_format  Pointer to the afs_boxing_format structure.
 */

void afs_boxing_format_free(afs_boxing_format* boxing_format)
{
    if (boxing_format == NULL)
    {
        return;
    }

    boxing_format->reference_count--;

    if (boxing_format->reference_count <= 0)
    {
        boxing_config_free(boxing_format->config);
        free(boxing_format);
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_boxing_format structure.
 *
 *  Function create a copy of input afs_boxing_format structure and return it.
 *  If boxing format pointer is NULL function return NULL.
 *
 *  \param[in]  boxing_format  Pointer to the afs_boxing_format structure.
 *  \return new copy of afs_boxing_format structure or NULL.
 */

afs_boxing_format * afs_boxing_format_clone(const afs_boxing_format * boxing_format)
{
    // If boxing format pointer is NULL return NULL.
    if (boxing_format == NULL)
    {
        return NULL;
    }

    afs_boxing_format * return_boxing_format = afs_boxing_format_create();
    return_boxing_format->config = boxing_config_clone(boxing_format->config);
    return_boxing_format->name = get_name(return_boxing_format);
    return_boxing_format->bytes_per_frame = boxing_format->bytes_per_frame;
    return_boxing_format->data_bytes_per_frame = boxing_format->data_bytes_per_frame;
    return_boxing_format->data_stripe_size = boxing_format->data_stripe_size;
    return_boxing_format->scaling_factor = boxing_format->scaling_factor;
    return_boxing_format->width = boxing_format->width;
    return_boxing_format->height = boxing_format->height;
    return_boxing_format->data_frame_width = boxing_format->data_frame_width;
    return_boxing_format->data_frame_height = boxing_format->data_frame_height;
    return_boxing_format->bits_per_pixel = boxing_format->bits_per_pixel;
    return_boxing_format->symbol_per_pixel = boxing_format->symbol_per_pixel;

    return return_boxing_format;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function returns a new reference to the input afs_boxing_format structure.
 *
 *  Function returns a new reference to the input afs_boxing_format structure.
 *  The reference count is incremented by 1.
 *  If boxing format pointer is NULL function return NULL.
 *
 *  \param[in]  boxing_format  Pointer to the afs_boxing_format structure.
 *  \return new reference of afs_boxing_format structure or NULL.
 */

afs_boxing_format * afs_boxing_format_get_new_reference(afs_boxing_format * boxing_format)
{
    // If boxing format pointer is NULL return NULL.
    if (boxing_format == NULL)
    {
        return NULL;
    }

    boxing_format->reference_count++;
    return boxing_format;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_boxing_format structures on the identity.
 *
 *  Function checks two instances of the afs_boxing_format structures on the identity.
 *
 *  \param[in]   boxing_format1  Pointer to the first instance of the afs_boxing_format structure.
 *  \param[in]   boxing_format2  Pointer to the second instance of the afs_boxing_format structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_boxing_format_equal(const afs_boxing_format * boxing_format1, const afs_boxing_format * boxing_format2)
{
    if (boxing_format1 == NULL && boxing_format2 == NULL)
    {
        return DTRUE;
    }

    if (boxing_format1 == NULL || boxing_format2 == NULL)
    {
        return DFALSE;
    }

    if (boxing_config_is_equal(boxing_format1->config, boxing_format2->config) == DTRUE &&
        (boxing_format1->name == boxing_format2->name || (boxing_format1->name && boxing_format2->name && strcmp(boxing_format1->name, boxing_format2->name) == 0)) &&
        boxing_format1->bytes_per_frame == boxing_format2->bytes_per_frame &&
        boxing_format1->data_bytes_per_frame == boxing_format2->data_bytes_per_frame &&
        boxing_format1->data_stripe_size == boxing_format2->data_stripe_size &&
        boxing_format1->scaling_factor == boxing_format2->scaling_factor &&
        boxing_format1->width == boxing_format2->width &&
        boxing_format1->height == boxing_format2->height &&
        boxing_format1->data_frame_width == boxing_format2->data_frame_width &&
        boxing_format1->data_frame_height == boxing_format2->data_frame_height &&
        boxing_format1->bits_per_pixel == boxing_format2->bits_per_pixel &&
        boxing_format1->symbol_per_pixel == boxing_format2->symbol_per_pixel)
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gives the data frames count.
 *
 *  Function calculates the data frames count and returns it.
 *  If afs_boxing_format pointer equal to NULL then function return zero.
 *
 *  \param[in]  boxing_format   Pointer to the afs_boxing_format structure.
 *  \param[in]  file_size       File size.
 *  \return data frames count or zero.
 */

unsigned int afs_boxing_format_get_data_frames(const afs_boxing_format * boxing_format, const size_t file_size)
{
    if (boxing_format == NULL)
    {
        return 0;
    }

    return get_ceil(file_size, boxing_format->bytes_per_frame);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function sets a new instance of the configuration.
 *
 *  The function sets a new configuration instance and initializes the structure data based on the new configuration.
 *  If the pointer to structure afs_boxing_format is NULL, then the function exits without specifying a new configuration.
 *
 *  \param[in]  boxing_format   Pointer to the afs_boxing_format structure.
 *  \param[in]  config          Pointer to the boxing_config structure.
 */

void afs_boxing_format_set_config(afs_boxing_format * boxing_format, const boxing_config * config)
{
    if (boxing_format == NULL)
    {
        return;
    }

    if (boxing_format->config != NULL)
    {
        boxing_config_free(boxing_format->config);
    }

    boxing_format->config = boxing_config_clone(config);
    initialize_instance(boxing_format);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_boxing_format structure to the XML nodes.
 *
 *  Function translates the input afs_boxing_format structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  out            Pointer to the XML node.
 *  \param[in]   boxing_format  Pointer to the afs_boxing_format structure.
 *  \return DTRUE on success.
 */

DBOOL afs_boxing_format_save_xml(struct mxml_node_s * out, afs_boxing_format* boxing_format)
{
    // If output node pointer is NULL or boxing format pointer is NULL return DFALSE
    if (out == NULL || boxing_format == NULL)
    {
        return DFALSE;
    }

    if (boxing_format->config == NULL)
    {
        mxmlNewElement(out, "NULL");
        return DTRUE;
    }

    return save_config_xml(out, boxing_format->config);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_boxing_format structure to the XML string.
 *
 *  Function translates the input afs_boxing_format structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   boxing_format  Pointer to the afs_boxing_format structure.
 *  \param[in]   compact        If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_boxing_format_save_string(afs_boxing_format * boxing_format, DBOOL compact)
{
    if (boxing_format == NULL)
    {
        return NULL;
    }

    struct mxml_node_s * document = mxmlNewXML("1.0");

    if (!afs_boxing_format_save_xml(document, boxing_format))
    {
        return NULL;
    }

    char * xmlString;
    mxmlSetWrapMargin(0);

    if (compact)
    {
        xmlString = mxmlSaveAllocString(document, MXML_NO_CALLBACK);
    }
    else
    {
        xmlString = mxmlSaveAllocString(document, whitespace_cb);
    }

    mxmlDelete(document);

    return xmlString;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_boxing_format structure.
 *
 *  Function translates the input XML nodes to the afs_boxing_format structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  boxing_format      Pointer to the afs_boxing_format structure.
 *  \param[in]   in                 Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_boxing_format_load_xml(afs_boxing_format * boxing_format, mxml_node_t * in)
{
    // If input XML node pointer pointer is NULL or boxing format pointer is NULL return DFALSE
    if (in == NULL || boxing_format == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * null_node = mxmlFindElement(in, in, "NULL", NULL, NULL, MXML_DESCEND);

    if (null_node != NULL)
    {
        afs_boxing_format_set_config(boxing_format, NULL);
        return DTRUE;
    }

    mxml_node_t * work_node = mxmlFindElement(in, in, "work", NULL, NULL, MXML_DESCEND);

    if (work_node == NULL)
    {
        return DFALSE;
    }

    boxing_config * config = boxing_config_create();

    mxml_node_t * class_node = mxmlGetFirstChild(work_node);
    while (class_node != NULL)
    {
        if (mxmlGetType(class_node) == MXML_ELEMENT)
        {
            if (strcmp(mxmlGetElement(class_node), CONFIG_XML_NODE_CLASS) == 0)
            {
                set_class_from_xml(config, class_node);
            }
        }
        class_node = mxmlWalkNext(class_node, work_node, MXML_NO_DESCEND);
    }

    afs_boxing_format_set_config(boxing_format, config);

    boxing_config_free(config);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_boxing_format structure.
 *
 *  Function translates the input XML string to the afs_boxing_format structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  boxing_format  Pointer to the afs_boxing_format structure.
 *  \param[in]   in             Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_boxing_format_load_string(afs_boxing_format* boxing_format, const char * in)
{
    // If input string pointer is NULL or boxing format pointer is NULL return DFALSE
    if (in == NULL || boxing_format == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_boxing_format_load_xml(boxing_format, document))
    {
        mxmlDelete(document);
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_boxing_format structure to the XML file.
 *
 *  Function translates the input afs_boxing_format structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   file_name      Name of the XML file.
 *  \param[in]   boxing_format  Pointer to the afs_boxing_format structure.
 *  \param[in]   compact        If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_boxing_format_save_config_file(const char * file_name, afs_boxing_format* boxing_format, DBOOL compact)
{
    if (file_name == NULL || boxing_format == NULL)
    {
        return DFALSE;
    }

    mxml_node_t *tree = mxmlNewXML("1.0");

#ifndef WIN32
    FILE * fp_save = fopen(file_name, "w+");
#else
    FILE * fp_save = fopen(file_name, "w+b");
#endif

    if (fp_save == NULL)
    {
        return DFALSE;
    }

    if (boxing_format->config != NULL)
    {
        if (save_config_xml(tree, boxing_format->config) == DFALSE)
        {
            fclose(fp_save);
            mxmlDelete(tree);
            return DFALSE;
        }
    }
    else
    {
        mxmlNewElement(tree, "NULL");
    }

    mxmlSetWrapMargin(0);

    if (compact == DTRUE)
    {
        mxmlSaveFile(tree, fp_save, MXML_NO_CALLBACK);
    }
    else
    {
        mxmlSaveFile(tree, fp_save, whitespace_cb);
    }


    fclose(fp_save);
    mxmlDelete(tree);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_boxing_format structure.
 *
 *  Function translates the input XML nodes to the afs_boxing_format structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  boxing_format  Pointer to the afs_boxing_format structure.
 *  \param[in]   file_name      Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_boxing_format_load_config_file(afs_boxing_format * boxing_format, const char * file_name)
{
    if (file_name == NULL || boxing_format == NULL)
    {
        return DFALSE;
    }

#ifndef WIN32
    FILE * fp_load = fopen(file_name, "r");
#else
    FILE * fp_load = fopen(file_name, "rb");
#endif

    if (fp_load == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadFile(NULL, fp_load, MXML_OPAQUE_CALLBACK);

    if (document == NULL)
    {
        fclose(fp_load);
        mxmlDelete(document);

        return DFALSE;
    }

    DBOOL return_value = afs_boxing_format_load_xml(boxing_format, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
  * \} end of boxingformat group
  */


// PRIVATE AFS BOXING FORMAT FUNCTIONS
//

static void set_class_from_xml(boxing_config * config, mxml_node_t * dom_node)
{
    const char * clave = mxmlElementGetAttr(dom_node, CONFIG_XML_ATTR_NAME);

    if (clave != NULL && clave[0] != '\0')
    {
        const char * version_value = mxmlElementGetAttr(dom_node, CONFIG_XML_ATTR_VERSION);
        if (version_value != NULL)
        {
            boxing_config_set_property(config, clave, CONFIG_XML_ATTR_VERSION, version_value);
        }

        mxml_node_t * node_in_class = mxmlGetFirstChild(dom_node);
        while (node_in_class != NULL)
        {
            if (mxmlGetType(node_in_class) == MXML_ELEMENT)
            {
                const char * in_class_name = mxmlGetElement(node_in_class);
                if (strcmp(in_class_name, CONFIG_XML_NODE_PROPERTY) == 0)
                {
                    const char * name_property = mxmlElementGetAttr(node_in_class, CONFIG_XML_ATTR_NAME);
                    if (name_property != NULL && name_property[0] != '\0')
                    {
                        const char * property_value = mxmlElementGetAttr(node_in_class, CONFIG_XML_ATTR_VALUE);
                        if (property_value != NULL && property_value[0] != '\0')
                        {
                            boxing_config_set_property(config, clave, name_property, property_value);
                        }
                    }
                }
                else
                {
                    if (in_class_name[0] != '!' && strcmp(in_class_name, "#comment") != 0)
                    {
                        DLOG_WARNING1("(set_class_from_xml) Unknown XML node type in CLASS Node %s", in_class_name);
                    }
                }
            }
            node_in_class = mxmlWalkNext(node_in_class, dom_node, MXML_NO_DESCEND);
        }
    }
}


static const char * whitespace_cb(mxml_node_t *node, int where)
{
    const char *name;

    /*
    * We can conditionally break to a new line
    * before or after any element. These are
    * just common HTML elements...
    */

    name = mxmlGetElement(node);

    if (strcmp("work", name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (strcmp("class", name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (strcmp("property", name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    return (NULL);
}


static const char * get_key_by_value(GHashTable * input_table, const char* value_string)
{
    if (input_table == NULL || value_string == NULL)
    {
        return NULL;
    }

    GHashTableIter i;
    g_hash_table_iter_init(&i, input_table);

    gpointer key;
    gpointer value;

    while (g_hash_table_iter_next(&i, &key, &value))
    {
        if (strcmp((const char*)value, value_string) == 0)
        {
            return (const char*)key;
        }
    }

    return NULL;
}


static void set_version(struct mxml_node_s* node, GHashTable * class_hash)
{
    g_variant * version_property = g_hash_table_lookup(class_hash, CONFIG_XML_ATTR_VERSION);
    if (version_property != NULL)
    {
        char * string_value = g_variant_to_string(version_property);
        mxmlElementSetAttr(node, CONFIG_XML_ATTR_VERSION, string_value);
        free(string_value);
    }
}


static DBOOL save_config_xml(mxml_node_t * out, boxing_config * config)
{
    // If output XML node pointer is NULL or boxing format pointer is NULL return DFALSE
    if (out == NULL || config == NULL)
    {
        return DFALSE;
    }

    if (config != NULL)
    {
        struct mxml_node_s *data = mxmlNewElement(out, "work");
        mxmlElementSetAttr(data, "version", "1.0.0.0");

        // Iterate all elements in hashmap
        GHashTableIter i;
        g_hash_table_iter_init(&i, config->groups);

        gpointer class_name;
        gpointer value;

        while (g_hash_table_iter_next(&i, &class_name, &value))
        {
            struct mxml_node_s *group = mxmlNewElement(data, "class");
            mxmlElementSetAttr(group, "key", (const char*)class_name);

            set_version(group, value);

            const char * alias = get_key_by_value(config->aliases, (const char*)class_name);

            if (alias != NULL)
            {
                struct mxml_node_s *property_alias = mxmlNewElement(group, "property");

                mxmlElementSetAttr(property_alias, "key", CONFIG_XML_KEY_ALIAS);
                mxmlElementSetAttr(property_alias, "value", alias);

                continue;
            }

            GHashTable * class_hash = value;
            GHashTableIter j;
            g_hash_table_iter_init(&j, class_hash);
            gpointer property_name;
            while (g_hash_table_iter_next(&j, &property_name, &value))
            {
                if (strcmp((const char*)property_name, CONFIG_XML_ATTR_VERSION) == 0)
                {
                    continue;
                }

                struct mxml_node_s *property = mxmlNewElement(group, "property");
                mxmlElementSetAttr(property, "key", (const char*)property_name);

                g_variant* v = (g_variant*)value;
                char * vs = g_variant_to_string(v);
                mxmlElementSetAttr(property, "value", vs);
                free(vs);
            }
        }
    }

    return DTRUE;
}


static void initialize_empty_instance(afs_boxing_format * boxing_format)
{
    if (boxing_format == NULL)
    {
        return;
    }

    boxing_format->name = NULL;
    boxing_format->bytes_per_frame = 0;
    boxing_format->data_bytes_per_frame = 0;
    boxing_format->data_stripe_size = 0;
    boxing_format->scaling_factor = 0;
    boxing_format->width = 0;
    boxing_format->height = 0;
    boxing_format->data_frame_width = 0;
    boxing_format->data_frame_height = 0;
    boxing_format->bits_per_pixel = 0;
    boxing_format->symbol_per_pixel = 0;

    return;
}


static void initialize_instance(afs_boxing_format * boxing_format)
{
    if (boxing_format == NULL)
    {
        return;
    }

    if (boxing_format->config == NULL)
    {
        initialize_empty_instance(boxing_format);
        return;
    }

    boxing_frame * frame = boxing_generic_frame_factory_create(boxing_format->config);

    if (frame == NULL)
    {
        initialize_empty_instance(boxing_format);
        return;
    }
    
    boxing_format->name = get_name(boxing_format);
    boxing_format->data_stripe_size = get_data_stripe_size(boxing_format);
    boxing_format->scaling_factor = get_scaling_factor(boxing_format);
    boxing_format->width = get_width(frame);
    boxing_format->height = get_height(frame);
    boxing_format->data_frame_width = get_data_frame_width(frame);
    boxing_format->data_frame_height = get_data_frame_height(frame);
    boxing_format->bits_per_pixel = get_bits_per_pixel(boxing_format, frame);
    boxing_format->symbol_per_pixel = get_symbol_per_pixel(boxing_format);
    boxing_format->bytes_per_frame = get_bytes_per_frame(boxing_format, frame);
    boxing_format->data_bytes_per_frame = get_data_bytes_per_frame(boxing_format, frame);

    boxing_generic_frame_factory_free(frame);
}


static const char * get_name(const afs_boxing_format * format)
{
    if (format == NULL)
    {
        return NULL;
    }

    if (format->config == NULL)
    {
        return NULL;
    }

    return boxing_config_property(format->config, "FormatInfo", "name");
}


static unsigned int get_bytes_per_frame(const afs_boxing_format * format, boxing_frame * frame)
{
    if (format == NULL || frame == NULL)
    {
        return 0;
    }
           
    return frame->container(frame)->capasity(frame->container(frame)) * format->bits_per_pixel / CHAR_BIT;
}


static unsigned int get_data_bytes_per_frame(const afs_boxing_format * format, boxing_frame * frame)
{
    if (format == NULL || frame == NULL)
    {
        return 0;
    }

    int capasity = frame->container(frame)->capasity(frame->container(frame));

    boxing_codecdispatcher * codec_dispatcher = boxing_codecdispatcher_create(capasity, 1 << format->bits_per_pixel, format->config, "DataCodingScheme");
    unsigned int packetSize = boxing_codecdispatcher_get_decoded_packet_size(codec_dispatcher);
    boxing_codecdispatcher_free(codec_dispatcher);

    return packetSize;
}


static unsigned int get_data_stripe_size(const afs_boxing_format * format)
{
    if (format == NULL)
    {
        return 0;
    }

    return (unsigned int)boxing_codecdispatcher_get_stripe_size(format->config);
}


static unsigned int get_scaling_factor(const afs_boxing_format * boxing_format)
{
    if (boxing_format == NULL)
    {
        return 0;
    }

    if (boxing_format->config == NULL)
    {
        return 0;
    }

    if (boxing_config_is_set(boxing_format->config, "FrameRaster", "scalingFactor"))
    {
        return boxing_config_property_int(boxing_format->config, "FrameRaster", "scalingFactor");
    }
    else if (!boxing_config_is_set(boxing_format->config, "BaseBuilder", FRAME_BUILDER_SCALINGFACTOR))
    {
        return 1;
    }
    else
    {
        return boxing_config_property_uint(boxing_format->config, "BaseBuilder", FRAME_BUILDER_SCALINGFACTOR);
    }
}


static unsigned int get_width(boxing_frame * frame)
{
    if (frame == NULL)
    {
        return 0;
    }

    return frame->size(frame).x;
}


static unsigned int get_height(boxing_frame * frame)
{
    if (frame == NULL)
    {
        return 0;
    }

    return frame->size(frame).y;
}


static unsigned int get_data_frame_width(boxing_frame * frame)
{
    if (frame == NULL)
    {
        return 0;
    }

    return frame->container(frame)->dimension(frame->container(frame)).x;
}


static unsigned int get_data_frame_height(boxing_frame * frame)
{
    if (frame == NULL)
    {
        return 0;
    }

    return frame->container(frame)->dimension(frame->container(frame)).y;
}

static unsigned int get_bits_per_pixel(afs_boxing_format * boxing_format, boxing_frame * frame)
{
    if (boxing_format == NULL)
    {
        return 0;
    }

    if (boxing_format->config == NULL)
    {
        return 0;
    }

    if (frame)
    {
        int levelsPerSymbol = frame->levels_per_symbol(frame) - 1;
        unsigned int color_depth = 0;
        while (levelsPerSymbol)
        {
            color_depth++;
            levelsPerSymbol >>= 1;
        }

        return color_depth;
    }
    else if (boxing_config_is_set(boxing_format->config, "BaseBuilder", FRAME_BUILDER_COLORDEPTH))
    {
        return boxing_config_property_uint(boxing_format->config, "BaseBuilder", FRAME_BUILDER_COLORDEPTH);
    }
    else
    {
        return 1;
    }
}


static unsigned int get_symbol_per_pixel(afs_boxing_format * boxing_format)
{
    if (boxing_format == NULL)
    {
        return 0;
    }

    if (boxing_format->bits_per_pixel == 0)
    {
        return 0;
    }

    return (unsigned int)powf(2.0f, (float)boxing_format->bits_per_pixel);
}


static unsigned int get_ceil(const size_t dividend, const unsigned int divisor)
{
    if (divisor == 0)
    {
        return 0;
    }

    unsigned int result = (unsigned)dividend / divisor;
    
    if (dividend % divisor)
    {
        result++;
    }
    return result;
}
