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
#include "boxing/platform/memory.h"
#include "boxing/graphics/genericframefactory.h"
#include "boxing/codecs/codecdispatcher.h"
#include "boxing/math/math.h"

static void         set_class_from_xml(boxing_config * config, struct mxml_node_s * dom_node);
static DBOOL        save_config_xml(mxml_node_t * out, boxing_config * config);
static const char * whitespace_cb(mxml_node_t *node, int where);


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

afs_boxing_format* afs_boxing_format_create()
{
    afs_boxing_format* boxing_format = BOXING_MEMORY_ALLOCATE_TYPE(afs_boxing_format);
    afs_boxing_format_init(boxing_format);
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

afs_boxing_format* afs_boxing_format_create2(const boxing_config * config)
{
    afs_boxing_format* boxing_format = BOXING_MEMORY_ALLOCATE_TYPE(afs_boxing_format);
    afs_boxing_format_init2(boxing_format, config);
    return boxing_format;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initialize .
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

    boxing_config_free(boxing_format->config);
    boxing_memory_free(boxing_format);
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

DBOOL afs_boxing_format_save_xml(mxml_node_t * out, afs_boxing_format* boxing_format)
{
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

char * afs_boxing_format_save_string(afs_boxing_format* boxing_format, DBOOL compact)
{
    if (boxing_format == NULL)
    {
        return NULL;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

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

DBOOL afs_boxing_format_load_xml(afs_boxing_format* boxing_format, mxml_node_t * in)
{
    if (in == NULL || boxing_format == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * null_node = mxmlFindElement(in, in, "NULL", NULL, NULL, MXML_DESCEND);

    if (null_node != NULL)
    {
        return DTRUE;
    }

    mxml_node_t * work_node = mxmlFindElement(in, in, "work", NULL, NULL, MXML_DESCEND);

    if (work_node == NULL)
    {
        return DFALSE;
    }

    boxing_config_free(boxing_format->config);
    boxing_format->config = boxing_config_create();

    mxml_node_t * class_node = mxmlGetFirstChild(work_node);
    while (class_node != NULL)
    {
        if (mxmlGetType(class_node) == MXML_ELEMENT)
        {
            if (strcmp(mxmlGetElement(class_node), CONFIG_XML_NODE_CLASS) == 0)
            {
                set_class_from_xml(boxing_format->config, class_node);
            }
        }
        class_node = mxmlWalkNext(class_node, work_node, MXML_NO_DESCEND);
    }

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

DBOOL afs_boxing_format_load_config_file(afs_boxing_format* boxing_format, const char * file_name)
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
 *  \brief The function gives the current format name.
 *
 *  Function retrieves the name from given configuration and returns it.
 *  If afs_boxing_format pointer equal to NULL or config pointer equal to NULL or
 *  the name is not found then function return NULL.
 *
 *  \param[in]  boxing_format   Pointer to the afs_boxing_format structure.
 *  \return name of the configuration or NULL.
 */

const char* afs_boxing_format_get_name(afs_boxing_format* boxing_format)
{
    if (boxing_format == NULL)
    {
        return NULL;
    }

    if (boxing_format->config == NULL)
    {
        return NULL;
    }

    return boxing_config_property(boxing_format->config, "FormatInfo", "name");
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gives the current scaling factor.
 *
 *  Function retrieves the current value of scaling factor from given configuration and returns it.
 *  If afs_boxing_format is NULL or boxing_format->config is NULL 0 is returned.
 *
 *  \param[in]  boxing_format   Pointer to the afs_boxing_format structure.
 *  \return the current value of scaling factor.
 */

unsigned int afs_boxing_format_get_scaling_factor(afs_boxing_format* boxing_format)
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


//----------------------------------------------------------------------------
/*!
 *  \brief The function gives the current width of the frame.
 *
 *  Function retrieves the current width of the frame from given configuration and returns it.
 *  If afs_boxing_format pointer equal to NULL
 *  then function return 0.
 *
 *  \param[in]  boxing_format   Pointer to the afs_boxing_format structure.
 *  \return the current width of the frame.
 */

unsigned int afs_boxing_format_get_width(afs_boxing_format* boxing_format)
{
    if (boxing_format == NULL)
    {
        return 0;
    }

    if (boxing_format->config == NULL)
    {
        return 0;
    }

    DBOOL has_point = DFALSE;

    return boxing_config_property_pointi(boxing_format->config, "FrameBuilder", "dimensions", &has_point).x;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gives the current height of the frame.
 *
 *  Function retrieves the current height of the frame from given configuration and returns it.
 *  If afs_boxing_format pointer equal to NULL
 *  then function return 0.
 *
 *  \param[in]  boxing_format   Pointer to the afs_boxing_format structure.
 *  \return the current height of the frame.
 */

unsigned int afs_boxing_format_get_height(afs_boxing_format* boxing_format)
{
    if (boxing_format == NULL)
    {
        return 0;
    }

    if (boxing_format->config == NULL)
    {
        return 0;
    }

    DBOOL has_point = DFALSE;

    return boxing_config_property_pointi(boxing_format->config, "FrameBuilder", "dimensions", &has_point).y;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gives the current data width of the frame.
 *
 *  Function retrieves the current data width of the frame from given configuration and returns it.
 *  If afs_boxing_format pointer equal to NULL
 *  then function return 0.
 *
 *  \param[in]  boxing_format   Pointer to the afs_boxing_format structure.
 *  \return the current data width of the frame.
 */

unsigned int afs_boxing_format_get_data_frame_width(afs_boxing_format * boxing_format)
{
    if (boxing_format == NULL)
    {
        return 0;
    }

    if (boxing_format->config == NULL)
    {
        return 0;
    }

    boxing_frame *  frame = boxing_generic_frame_factory_create(boxing_format->config);

    if (frame == NULL)
    {
        return 0;
    }

    int return_value = frame->container(frame)->dimension(frame->container(frame)).x;
    boxing_generic_frame_factory_free(frame);
    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gives the current data height of the frame.
 *
 *  Function retrieves the current data height of the frame from given configuration and returns it.
 *  If afs_boxing_format pointer equal to NULL
 *  then function return 0.
 *
 *  \param[in]  boxing_format   Pointer to the afs_boxing_format structure.
 *  \return the current data height of the frame.
 */

unsigned int afs_boxing_format_get_data_frame_height(afs_boxing_format* boxing_format)
{
    if (boxing_format == NULL)
    {
        return 0;
    }

    if (boxing_format->config == NULL)
    {
        return 0;
    }

    boxing_frame *  frame = boxing_generic_frame_factory_create(boxing_format->config);

    if (frame == NULL)
    {
        return 0;
    }

    int return_value = frame->container(frame)->dimension(frame->container(frame)).y;
    boxing_generic_frame_factory_free(frame);
    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gives the current bit per pixel value.
 *
 *  Function retrieves the current bit per pixel value from given configuration and returns it.
 *  If afs_boxing_format pointer equal to NULL or configuration pointer equal to NULL
 *  then function return 0.
 *
 *  \param[in]  boxing_format   Pointer to the afs_boxing_format structure.
 *  \return the current bit per pixel value.
 */

unsigned int afs_boxing_format_get_bits_per_pixel(afs_boxing_format* boxing_format)
{
    if (boxing_format == NULL)
    {
        return 0;
    }

    if (boxing_format->config == NULL)
    {
        return 0;
    }

    return boxing_config_property_uint(boxing_format->config, "BaseBuilder", FRAME_BUILDER_COLORDEPTH);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gives the current symbols per pixel value.
 *
 *  Function retrieves the current synmols per pixel value from given configuration and returns it.
 *  If afs_boxing_format pointer equal to NULL or configuration pointer equal to NULL
 *  then function return 0.
 *
 *  \param[in]  boxing_format   Pointer to the afs_boxing_format structure.
 *  \return the current symbols per pixel value.
 */

unsigned int afs_boxing_format_get_symbols_per_pixel(afs_boxing_format* boxing_format)
{
    if (boxing_format == NULL)
    {
        return 0;
    }

    if (boxing_format->config == NULL)
    {
        return 0;
    }

    unsigned int bits_per_pixel = afs_boxing_format_get_bits_per_pixel(boxing_format);

    if (bits_per_pixel == 0)
    {
        return 0;
    }

    return (unsigned int)powf(2.0f, (float)bits_per_pixel);
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
                        if (name_property != NULL)
                        {
                            boxing_config_set_property(config, clave, name_property, property_value);
                        }
                    }
                }
                else
                {
                    if (in_class_name[0] != '!' && strcmp(in_class_name, "#comment") != 0)
                    {
                        DLOG_WARNING1("Unknown XML node type in CLASS Node %s", in_class_name);
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

    if (boxing_string_equal("work", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("class", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("property", name))
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
        if (boxing_string_equal((const char*)value, value_string) == DTRUE)
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
        boxing_memory_free(string_value);
    }
}


static DBOOL save_config_xml(mxml_node_t * out, boxing_config * config)
{
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
                if (boxing_string_equal((const char*)property_name, CONFIG_XML_ATTR_VERSION))
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
