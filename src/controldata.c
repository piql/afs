/*****************************************************************************
**
**  Definition of the Control Data interface
**
**  Creation date:  2016/12/28
**  Created by:     Piql AS
**
**
**  Copyright (c) 2014 Piql AS. All rights reserved.
**
**  This file is part of the boxing library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "controldata.h"
#include "boxing/string.h"
#include "boxing/platform/memory.h"
#include "mxml.h"

// PRIVATE INTERFACE
//
static const char * whitespace_cb(mxml_node_t *node, int where);


//----------------------------------------------------------------------------
/*! 
 *  \defgroup cf Control frame
 *  \brief  The control frame describes how to decode the rest of the reel.
 *  \ingroup afs
 *
 *  The control frame is always frame number 1 in the reel. It has metadata
 *  giving a brief description of the reel content and a boxing format. The
 *  boxing format contained in the control frame should be used when decoding 
 *  the rest of the reel. The control frame also points to the locations of 
 *  the table of content.
 */


/*! 
  * \addtogroup cf
  * \{
  */


// PUBLIC AFS CONTROL_DATA FUNCTIONS
//


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_control_data_s  controldata.h
 *  \brief      Control frame.
 *
 *  \param administrative_metadata  Pointer to the afs_administrative_metadata structure.
 *  \param technical_metadata       Pointer to the afs_technical_metadata structure.
 *
 *  The control frame data is grouped into two types: Administrative and technical 
 *  metadata.
 */


//----------------------------------------------------------------------------
/*!
 *  \brief Create an control_data instance.
 *
 *  Allocate memory for the afs_control_data type
 *  and initializes all structure pointers with NULL values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_control_data structure.
 */

afs_control_data *  afs_control_data_create()
{
    afs_control_data* control_data = BOXING_MEMORY_ALLOCATE_TYPE(afs_control_data);

    control_data->administrative_metadata = NULL;
    control_data->technical_metadata = NULL;

    return control_data;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a control data instance.
 *
 *  Allocate memory for the afs_control_data type
 *  and initialize afs_administrative_metadata and afs_technical_metadata structures with specified input value.
 *  Return instance of allocated structure.
 *
 *  \param[in]  administrative_metadata  Pointer to the afs_administrative_metadata structure.
 *  \param[in]  technical_metadata       Pointer to the afs_technical_metadata structure.
 *  \return instance of allocated afs_control_data structure.
 */

afs_control_data * afs_control_data_create2(afs_administrative_metadata * administrative_metadata, afs_technical_metadata * technical_metadata)
{
    afs_control_data * control_data = BOXING_MEMORY_ALLOCATE_TYPE(afs_control_data);

    control_data->administrative_metadata = administrative_metadata;
    control_data->technical_metadata = technical_metadata;

    return control_data;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of control data structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  control_data  Pointer to the afs_control_data structure.
 */

void afs_control_data_free(afs_control_data * control_data)
{
    if (control_data == NULL)
    {
        return;
    }

    afs_administrative_metadata_free(control_data->administrative_metadata);
    control_data->administrative_metadata = NULL;
    afs_technical_metadata_free(control_data->technical_metadata);
    control_data->technical_metadata = NULL;
    boxing_memory_free(control_data);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_control_data structure.
 *
 *  Function create a copy of input afs_control_data structure and return it.
 *  If control data structure pointer is NULL function return NULL.
 *
 *  \param[in]  control_data  Pointer to the afs_control_data structure.
 *  \return new copy of afs_control_data structure or NULL.
 */

afs_control_data * afs_control_data_clone(const afs_control_data * control_data)
{
    if (control_data == NULL)
    {
        return NULL;
    }

    afs_control_data * return_control_data = afs_control_data_create();
    return_control_data->administrative_metadata = afs_administrative_metadata_clone(control_data->administrative_metadata);
    return_control_data->technical_metadata = afs_technical_metadata_clone(control_data->technical_metadata);

    return return_control_data;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_control_data structures on the identity.
 *
 *  Function checks two instances of the afs_control_data structures on the identity.
 *
 *  \param[in]   control_data1  Pointer to the first instance of the afs_control_data structure.
 *  \param[in]   control_data2  Pointer to the second instance of the afs_control_data structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_control_data_equal(const afs_control_data * control_data1, const afs_control_data * control_data2)
{
    if (control_data1 == NULL && control_data2 == NULL)
    {
        return DTRUE;
    }

    if (control_data1 == NULL || control_data2 == NULL)
    {
        return DFALSE;
    }

    if (afs_administrative_metadata_equal(control_data1->administrative_metadata, control_data2->administrative_metadata) == DTRUE &&
        afs_technical_metadata_equal(control_data1->technical_metadata, control_data2->technical_metadata) == DTRUE)
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_control_data structure to the XML file.
 *
 *  Function translates the input afs_control_data structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   control_data  Pointer to the afs_control_data structure.
 *  \param[in]   file_name     Name of the XML file.
 *  \param[in]   compact       If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_control_data_save_file(afs_control_data * control_data, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || control_data == NULL)
    {
        return DFALSE;
    }

    if (control_data->administrative_metadata == NULL || control_data->technical_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * tree = mxmlNewXML("1.0");

    if (!afs_control_data_save_xml(control_data, tree))
    {
        mxmlDelete(tree);
        return DFALSE;
    }

#ifndef WIN32
    FILE * fp_save = fopen(file_name, "w+");
#else
    FILE * fp_save = fopen(file_name, "w+b");
#endif

    if (fp_save == NULL)
    {
        mxmlDelete(tree);
        return DFALSE;
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
 *  \brief Function translates the input afs_control_data structure to the XML string.
 *
 *  Function translates the input afs_control_data structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   control_data  Pointer to the afs_control_data structure.
 *  \param[in]   compact       If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_control_data_save_string(afs_control_data * control_data, DBOOL compact)
{
    if (control_data == NULL)
    {
        return NULL;
    }

    if (control_data->administrative_metadata == NULL || control_data->technical_metadata == NULL)
    {
        return NULL;
    }

    struct mxml_node_s * document = mxmlNewXML("1.0");

    if (!afs_control_data_save_xml(control_data, document))
    {
        mxmlDelete(document);
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
 *  \brief Function translates the input afs_control_data structure to the XML nodes.
 *
 *  Function translates the input afs_control_data structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  out           Pointer to the XML node.
 *  \param[in]   control_data  Pointer to the afs_control_data structure.
 *  \return DTRUE on success.
 */

DBOOL afs_control_data_save_xml(afs_control_data * control_data, mxml_node_t * out)
{
    if (out == NULL || control_data == NULL)
    {
        return DFALSE;
    }

    DBOOL result = afs_administrative_metadata_save_xml(control_data->administrative_metadata, out);

    if (result == DFALSE)
    {
        return DFALSE;
    }

    return afs_technical_metadata_save_xml(control_data->technical_metadata, out);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML file to the afs_control_data structure.
 *
 *  Function translates the input XML file to the afs_control_data structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  control_data  Pointer to the afs_control_data structure.
 *  \param[in]   file_name     Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_control_data_load_file(afs_control_data * control_data, const char * file_name)
{
    if (file_name == NULL || control_data == NULL)
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

    DBOOL return_value = afs_control_data_load_xml(control_data, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_control_data structure.
 *
 *  Function translates the input XML string to the afs_control_data structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  control_data  Pointer to the afs_control_data structure.
 *  \param[in]   in            Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_control_data_load_string(afs_control_data * control_data, const char * in)
{
    if (in == NULL || boxing_string_equal(in, "") || control_data == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_control_data_load_xml(control_data, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_control_data structure.
 *
 *  Function translates the input XML nodes to the afs_control_data structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  control_data  Pointer to the afs_control_data structure.
 *  \param[in]   in            Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_control_data_load_xml(afs_control_data* control_data, mxml_node_t * in)
{
    if (control_data == NULL || in == NULL)
    {
        return DFALSE;
    }

    if (mxmlFindElement(in, in, "AdministrativeMetadata", NULL, NULL, MXML_DESCEND) != NULL)
    {
        if (control_data->administrative_metadata != NULL)
        {
            afs_administrative_metadata_free(control_data->administrative_metadata);
        }
        control_data->administrative_metadata = afs_administrative_metadata_create();
        afs_administrative_metadata_load_xml(control_data->administrative_metadata, in);
    }
    else
    {
        return DFALSE;
    }

    if (mxmlFindElement(in, in, "TechnicalMetadata", NULL, NULL, MXML_DESCEND) != NULL)
    {
        if (control_data->technical_metadata != NULL)
        {
            afs_technical_metadata_free(control_data->technical_metadata);
        }
        control_data->technical_metadata = afs_technical_metadata_create();
        afs_technical_metadata_load_xml(control_data->technical_metadata, in);
    }
    else
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of cf group
  */


// PRIVATE AFS CONTROL DATA FUNCTIONS
//

static const char * whitespace_cb(mxml_node_t *node, int where)
{
    const char *name = node->value.element.name;
    const char *parent = mxmlGetElement(node->parent);

    switch (where)
    {
    case MXML_WS_BEFORE_OPEN:
        if (name[0] == '?')
        {
            return NULL;
        }
        if (boxing_string_equal(node->parent->value.element.name, "AdministrativeMetadata") == DTRUE || boxing_string_equal(node->parent->value.element.name, "TechnicalMetadata") == DTRUE)
        {
            return "\n    ";
        }
        if (boxing_string_equal(parent, "Tocs") == DTRUE || boxing_string_equal(name, "work") == DTRUE)
        {
            return "\n        ";
        }
        if (boxing_string_equal(name, "class") == DTRUE || boxing_string_equal(parent, "file"))
        {
            return "\n            ";
        }
        if (boxing_string_equal(name, "property") == DTRUE || boxing_string_equal(parent, "data") || boxing_string_equal(parent, "preview"))
        {
            return "\n                ";
        }
        return "\n";
    case MXML_WS_AFTER_OPEN:
        if (mxmlGetOpaque(node) == NULL && mxmlGetText(node, 0) == NULL)
        {
            return NULL;
        }
        if (boxing_string_equal(node->parent->value.element.name, "AdministrativeMetadata") == DTRUE || boxing_string_equal(node->parent->value.element.name, "TechnicalMetadata") == DTRUE)
        {
            return "\n        ";
        }
        if (boxing_string_equal(parent, "file"))
        {
            return "\n                ";
        }
        if (boxing_string_equal(parent, "data"))
        {
            return "\n                    ";
        }
        return NULL;
    case MXML_WS_BEFORE_CLOSE:
        if (boxing_string_equal(node->parent->value.element.name, "AdministrativeMetadata") == DTRUE || boxing_string_equal(node->parent->value.element.name, "TechnicalMetadata") == DTRUE)
        {
            return "\n    ";
        }
        if (boxing_string_equal(parent, "Tocs") == DTRUE || boxing_string_equal(name, "work") == DTRUE)
        {
            return "\n        ";
        }
        if (boxing_string_equal(name, "class") == DTRUE || boxing_string_equal(parent, "file"))
        {
            return "\n            ";
        }
        if (boxing_string_equal(parent, "data"))
        {
            return "\n                ";
        }
        return "\n";
    case MXML_WS_AFTER_CLOSE:
        return NULL;
    default:
        return NULL;
    }
}
