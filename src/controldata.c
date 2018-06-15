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
 *  giving a bries description of the reel content and a boxing format. The
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

afs_control_data*  afs_control_data_create()
{
    afs_control_data* control_data = BOXING_MEMORY_ALLOCATE_TYPE(afs_control_data);
    afs_control_data_init(control_data);
    return control_data;
}


//----------------------------------------------------------------------------
/*! 
 *  \brief Initializes all structure pointers with NULL value.
 *
 *  Initializes all input structure pointers with NULL values.
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  control_data  Pointer to the afs_control_data structure.
 */

void afs_control_data_init(afs_control_data* control_data)
{
    if (control_data == NULL)
    {
        return;
    }

    control_data->administrative_metadata = NULL;
    control_data->technical_metadata = NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers.
 *
 *  Initializes all input structure pointers with specified values.
 *  If one of the input pointers is NULL, then return without initialization.
 *
 *  \param[in]  control_data             Pointer to the afs_control_data structure.
 *  \param[in]  administrative_metadata  Pointer to the afs_administrative_metadata structure.
 *  \param[in]  technical_metadata       Pointer to the afs_technical_metadata structure.
 */

void afs_control_data_init2(afs_control_data* control_data, afs_administrative_metadata* administrative_metadata, afs_technical_metadata* technical_metadata)
{
    if (control_data == NULL || administrative_metadata == NULL || technical_metadata == NULL)
    {
        return;
    }

    control_data->administrative_metadata = administrative_metadata;
    control_data->technical_metadata = technical_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of control data structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  control_data  Pointer to the afs_control_data structure.
 */

void afs_control_data_free(afs_control_data* control_data)
{
    if (control_data == NULL)
    {
        return;
    }

    afs_administrative_metadata_free(control_data->administrative_metadata);
    afs_technical_metadata_free(control_data->technical_metadata);
    boxing_memory_free(control_data);
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

DBOOL afs_control_data_save_xml(mxml_node_t * out, afs_control_data* control_data)
{
    if (out == NULL || control_data == NULL)
    {
        return DFALSE;
    }

    afs_administrative_metadata_save_xml(out, control_data->administrative_metadata);
    afs_technical_metadata_save_xml(out, control_data->technical_metadata);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_control_data structure to the XML file.
 *
 *  Function translates the input afs_control_data structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   file_name     Name of the XML file.
 *  \param[in]   control_data  Pointer to the afs_control_data structure.
 *  \param[in]   compact       If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_control_data_save_xml_file(const char * file_name, afs_control_data* control_data, DBOOL compact)
{
    if (file_name == NULL || control_data == NULL)
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

    if (afs_control_data_save_xml(tree, control_data) == DFALSE)
    {
        fclose(fp_save);
        mxmlDelete(tree);
        return DFALSE;
    }

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
        control_data->administrative_metadata = afs_administrative_metadata_create();
        afs_administrative_metadata_load_xml(control_data->administrative_metadata, in);
    }

    if (mxmlFindElement(in, in, "TechnicalMetadata", NULL, NULL, MXML_DESCEND) != NULL)
    {
        control_data->technical_metadata = afs_technical_metadata_create();
        afs_technical_metadata_load_xml(control_data->technical_metadata, in);
    }

    return DTRUE;
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

DBOOL afs_control_data_load_xml_file(afs_control_data* control_data, const char * file_name)
{
    if (control_data == NULL || file_name == NULL)
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

    DBOOL return_value = afs_control_data_load_xml(control_data, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
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
