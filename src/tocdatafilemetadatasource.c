/*****************************************************************************
**
**  Definition of the toc data file metadata source interface
**
**  Creation date:  2018/01/25
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2018 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "tocdatafilemetadatasource_c.h"
#include "xmlutils.h"
#include "platform/io.h"
#include "boxing/string.h"
#include "boxing/log.h"

//  DEFINES
//

// PRIVATE INTERFACE
//

static const char * whitespace_cb(mxml_node_t *node, int where);
static char *       get_substring(const char* input_string, size_t start_index, size_t end_index);


/*! 
  * \addtogroup file
  * \{
  */


/*! 
 *  \typedef  gvector afs_toc_data_file_metadata_sources
 *  \brief Array of afs_toc_data_file_metadata_source structure instances.
 *
 *  Array of afs_toc_data_file_metadata_source structure instances with the ability to add new instances.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_data_file_metadata_source_s  tocdatafilemetadatasource_c.h
 *  \brief      TOC data file metadata source storage.
 *
 *  \param data       Metadata source data.
 *  \param format_id  Format id.
 *  \param file_id    File id.
 *  \param source_id  Source id.
 *
 *  Structure for storing toc data file metadata source.
 */

// PUBLIC AFS TOC DATA FILE METADATA SOURCE FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc data file metadata source instance.
 *
 *  Allocate memory for the afs_toc_data_file_metadata_source type
 *  and initializes all structure pointers with NULL values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_data_file_metadata_source structure.
 */

afs_toc_data_file_metadata_source * afs_toc_data_file_metadata_source_create()
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_data_file_metadata_source);
    afs_toc_data_file_metadata_source_init(toc_data_file_metadata_source, NULL, NULL, -1, -1);
    return toc_data_file_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc data file metadata source instance.
 *
 *  Allocate memory for the afs_toc_data_file_metadata_source type
 *  and initializes data, format id, file id and source id with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] data       Metadata source data.
 *  \param[in] format_id  Format id.
 *  \param[in] file_id    File id.
 *  \param[in] source_id  Source id.
 *  \return instance of allocated afs_toc_data_file_metadata_source structure.
 */

afs_toc_data_file_metadata_source * afs_toc_data_file_metadata_source_create2(const char * data, const char * format_id, const int file_id, const int source_id)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();
    afs_toc_data_file_metadata_source_init(toc_data_file_metadata_source, data, format_id, file_id, source_id);
    return toc_data_file_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers and variables with specified input values.
 *
 *  Initializes all input structure pointers and variables with specified input values.
 *  If input toc_data_file_metadata_source pointer is NULL, then return without initialization.
 *
 *  \param[in] toc_data_file_metadata_source  Pointer to the afs_toc_data_file_metadata_source structure.
 *  \param[in] data                           Metadata source data.
 *  \param[in] format_id                      Format id.
 *  \param[in] file_id                        File id.
 *  \param[in] source_id                      Source id.
 */

void afs_toc_data_file_metadata_source_init(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * data, const char * format_id, const int file_id, const int source_id)
{
    if (toc_data_file_metadata_source == NULL)
    {
        return;
    }

    toc_data_file_metadata_source->data = boxing_string_clone(data);
    toc_data_file_metadata_source->format_id = boxing_string_clone(format_id);
    toc_data_file_metadata_source->file_id = file_id;
    toc_data_file_metadata_source->source_id = source_id;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_data_file_metadata_source structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_data_file_metadata_source  Pointer to the afs_toc_data_file_metadata_source structure.
 */

void afs_toc_data_file_metadata_source_free(afs_toc_data_file_metadata_source * toc_data_file_metadata_source)
{
    if (toc_data_file_metadata_source == NULL)
    {
        return;
    }

    boxing_string_free(toc_data_file_metadata_source->data);
    boxing_string_free(toc_data_file_metadata_source->format_id);
    boxing_memory_free(toc_data_file_metadata_source);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_data_file_metadata_source structure.
 *
 *  Function create a copy of input afs_toc_data_file_metadata_source structure and return it.
 *  If TOC data file metadata source pointer is NULL function return NULL.
 *
 *  \param[in]  toc_data_file_metadata_source  Pointer to the afs_toc_data_file_metadata_source structure.
 *  \return new copy of afs_toc_data_file_metadata_source structure or NULL.
 */

afs_toc_data_file_metadata_source * afs_toc_data_file_metadata_source_clone(afs_toc_data_file_metadata_source * toc_data_file_metadata_source)
{
    // If TOC data file metadata source pointer is NULL return NULL.
    if (toc_data_file_metadata_source == NULL)
    {
        return NULL;
    }

    afs_toc_data_file_metadata_source * return_copy = afs_toc_data_file_metadata_source_create();

    return_copy->data = boxing_string_clone(toc_data_file_metadata_source->data);
    return_copy->format_id = boxing_string_clone(toc_data_file_metadata_source->format_id);
    return_copy->file_id = toc_data_file_metadata_source->file_id;
    return_copy->source_id = toc_data_file_metadata_source->source_id;
    
    return return_copy;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_data_file_metadata_source structures on the identity.
 *
 *  Function checks two instances of the afs_toc_data_file_metadata_source structures on the identity.
 *
 *  \param[in]   toc_data_file_metadata_source1  Pointer to the first instance of the afs_toc_data_file_metadata_source structure.
 *  \param[in]   toc_data_file_metadata_source2  Pointer to the second instance of the afs_toc_data_file_metadata_source structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_data_file_metadata_source_equal(afs_toc_data_file_metadata_source * toc_data_file_metadata_source1, afs_toc_data_file_metadata_source * toc_data_file_metadata_source2)
{
    if (toc_data_file_metadata_source1 == NULL && toc_data_file_metadata_source2 == NULL)
    {
        return DTRUE;
    }

    if (toc_data_file_metadata_source1 == NULL || toc_data_file_metadata_source2 == NULL)
    {
        return DFALSE;
    }

    if (boxing_string_equal(toc_data_file_metadata_source1->data, toc_data_file_metadata_source2->data) == DFALSE ||
        boxing_string_equal(toc_data_file_metadata_source1->format_id, toc_data_file_metadata_source2->format_id) == DFALSE ||
        toc_data_file_metadata_source1->file_id != toc_data_file_metadata_source2->file_id ||
        toc_data_file_metadata_source1->source_id != toc_data_file_metadata_source2->source_id)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the afs_toc_data_file_metadata_source structure.
 *
 *  The function returns the sign of integrity the current structure.
 *  If TOC data file metadata source pointer is NULL then function return DFALSE.
 *
 *  \param[in]   toc_data_file_metadata_source   Pointer to the afs_toc_data_file_metadata_source structure.
 *  \return the sign of integrity the afs_toc_data_file_metadata_source structure.
 */

DBOOL afs_toc_data_file_metadata_source_is_valid(afs_toc_data_file_metadata_source * toc_data_file_metadata_source)
{
    // If TOC data file metadata source pointer is NULL return DFALSE
    if (toc_data_file_metadata_source == NULL)
    {
        return DFALSE;
    }

    if ((toc_data_file_metadata_source->data != NULL && boxing_string_length(toc_data_file_metadata_source->data) != 0) ||
        toc_data_file_metadata_source->file_id != -1)
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new data string to the instance of structure afs_toc_data_file_metadata_source.
 *
 *  The function adds a new data string to the instance of structure afs_toc_data_file_metadata_source.
 *  If TOC data file metadata source pointer is NULL or data string pointer is NULL or data string is empty then function return without adding.
 *
 *  \param[in]   toc_data_file_metadata_source   Pointer to the afs_toc_data_file_metadata_source structure.
 *  \param[in]   data                            Metadata source data.
 */

void afs_toc_data_file_metadata_source_set_data(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * data)
{
    if (toc_data_file_metadata_source == NULL || data == NULL || boxing_string_length(data) == 0)
    {
        return;
    }

    if (toc_data_file_metadata_source->data != NULL)
    {
        boxing_string_free(toc_data_file_metadata_source->data);
    }

    toc_data_file_metadata_source->data = boxing_string_clone(data);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new format id string to the instance of structure afs_toc_data_file_metadata_source.
 *
 *  The function adds a new format id string to the instance of structure afs_toc_data_file_metadata_source.
 *  If TOC data file metadata source pointer is NULL or format id string pointer is NULL or format id string is empty then function return without adding.
 *
 *  \param[in]   toc_data_file_metadata_source   Pointer to the afs_toc_data_file_metadata_source structure.
 *  \param[in]   format_id                       Format id.
 */

void afs_toc_data_file_metadata_source_set_format_id(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * format_id)
{
    if (toc_data_file_metadata_source == NULL || format_id == NULL || boxing_string_length(format_id) == 0)
    {
        return;
    }

    if (toc_data_file_metadata_source->format_id != NULL)
    {
        boxing_string_free(toc_data_file_metadata_source->format_id);
    }

    toc_data_file_metadata_source->format_id = boxing_string_clone(format_id);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_data_file_metadata_source structure to the XML file.
 *
 *  Function translates the input afs_toc_data_file_metadata_source structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_data_file_metadata_source  Pointer to the afs_toc_data_file_metadata_source structure.
 *  \param[in]   file_name                      Name of the XML file.
 *  \param[in]   compact                        If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_source_save_file(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_data_file_metadata_source == NULL || afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source) == DFALSE)
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
        mxmlDelete(tree);
        return DFALSE;
    }

    if (!afs_toc_data_file_metadata_source_save_xml(toc_data_file_metadata_source, tree))
    {
        fclose(fp_save);
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
 *  \brief Function translates the input afs_toc_data_file_metadata_source structure to the XML string.
 *
 *  Function translates the input afs_toc_data_file_metadata_source structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_data_file_metadata_source  Pointer to the afs_toc_data_file_metadata_source structure.
 *  \param[in]   compact                        If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_data_file_metadata_source_save_string(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, DBOOL compact)
{
    // If TOC data file metadata source pointer is NULL return DFALSE
    if (toc_data_file_metadata_source == NULL)
    {
        return DFALSE;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_data_file_metadata_source_save_xml(toc_data_file_metadata_source, document))
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
 *  \brief Function translates the input afs_toc_data_file_metadata_source structure to the XML nodes.
 *
 *  Function translates the input afs_toc_data_file_metadata_source structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_data_file_metadata_source   Pointer to the afs_toc_data_file_metadata_source structure.
 *  \param[out]  out                             Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_source_save_xml(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, mxml_node_t* out)
{
    // If output node pointer is NULL or TOC data file metadata source pointer is NULL return DFALSE
    if (out == NULL || toc_data_file_metadata_source == NULL || afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source) == DFALSE)
    {
        return DFALSE;
    }

    mxml_node_t * source_node = mxmlNewElement(out, "source");

    mxmlElementSetAttrf(source_node, "fileId", "%d", toc_data_file_metadata_source->file_id);
    mxmlElementSetAttrf(source_node, "sourceId", "%d", toc_data_file_metadata_source->source_id);

    if (toc_data_file_metadata_source->data != NULL)
    {
        mxml_node_t * data_node = mxmlNewElement(source_node, "data");
        mxmlNewCDATA(data_node, toc_data_file_metadata_source->data);

        if (toc_data_file_metadata_source->format_id != NULL)
        {
            mxmlElementSetAttr(data_node, "formatId", toc_data_file_metadata_source->format_id);
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML file to the afs_toc_data_file_metadata_source structure.
 *
 *  Function translates the input XML file to the afs_toc_data_file_metadata_source structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_file_metadata_source  Pointer to the afs_toc_data_file_metadata_source structure.
 *  \param[in]   file_name                      Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_source_load_file(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * file_name)
{
    // If input file name string pointer is NULL or TOC data file metadata source pointer is NULL return DFALSE
    if (file_name == NULL || toc_data_file_metadata_source == NULL)
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

    DBOOL return_value = afs_toc_data_file_metadata_source_load_xml(toc_data_file_metadata_source, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_data_file_metadata_source structure.
 *
 *  Function translates the input XML string to the afs_toc_data_file_metadata_source structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_file_metadata_source  Pointer to the afs_toc_data_file_metadata_source structure.
 *  \param[in]   in                             Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_source_load_string(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * in)
{
    // If input string pointer is NULL or TOC data file metadata source pointer is NULL return DFALSE
    if (in == NULL || boxing_string_equal(in, "") || toc_data_file_metadata_source == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_data_file_metadata_source_load_xml(toc_data_file_metadata_source, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC data file metadata source structure.
 *
 *  Function translates the input XML nodes to the TOC data file metadata source structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_file_metadata_source  Pointer to the afs_toc_data_file_metadata_source structure.
 *  \param[in]   input_node                     Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_source_load_xml(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, mxml_node_t* input_node)
{
    // If input node pointer is NULL or TOC data file metadata source pointer is NULL return DFALSE
    if (input_node == NULL || toc_data_file_metadata_source == NULL)
    {
        DLOG_ERROR("Setup error.\n");
        return DFALSE;
    }

    mxml_node_t * source_node = NULL;

    if (boxing_string_equal(mxmlGetElement(input_node), "source") == DTRUE)
    {
        source_node = input_node;
    }
    else
    {
        source_node = mxmlFindElement(input_node, input_node, "source", NULL, NULL, MXML_DESCEND);
    }

    if (source_node == NULL)
    {
        DLOG_ERROR("No source node.\n");
        return DFALSE;
    }
    
    if (afs_xmlutils_get_integer_attribute(&toc_data_file_metadata_source->file_id, source_node, "fileId") == DFALSE)
    {
        DLOG_ERROR("No fileId attribute.\n");
        return DFALSE;
    }
    
    if (afs_xmlutils_get_integer_attribute(&toc_data_file_metadata_source->source_id, source_node, "sourceId") == DFALSE)
    {
        toc_data_file_metadata_source->source_id = -1;
    }
    
    mxml_node_t * data_node = mxmlFindElement(source_node, source_node, "data", NULL, NULL, MXML_DESCEND);
    if (data_node != NULL)
    {
        // CDATA is a child node 
        mxml_node_t * cdataNode = mxmlWalkNext(
            data_node,
            data_node,
            MXML_DESCEND);

        while (cdataNode)
        {
            const char* cdata = mxmlGetCDATA(cdataNode);
            if (cdata)
            {
                const size_t cdataLength = strlen(cdata);
                if (cdata[cdataLength - 1] == ']' && cdata[cdataLength - 2] == ']')
                {
                    toc_data_file_metadata_source->data = get_substring(cdata, 0, cdataLength - 2);
                }
                else
                {
                    toc_data_file_metadata_source->data = boxing_string_clone(cdata);
                }
                break;
            }

            cdataNode = mxmlWalkNext(cdataNode, data_node, MXML_NO_DESCEND);
        }

        if (toc_data_file_metadata_source->data == NULL)
        {
            toc_data_file_metadata_source->data = afs_xmlutils_get_node_text(data_node);
        }

        toc_data_file_metadata_source->format_id = boxing_string_clone(mxmlElementGetAttr(data_node, "formatId"));
    }

    return afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source);
}


//----------------------------------------------------------------------------
/*!
  * \} end of file group
  */


// PRIVATE AFS TOC DATA FILE METADATA SOURCE FUNCTIONS
//

static const char * whitespace_cb(mxml_node_t *node, int where)
{
    const char *name, *parent_name;

    /*
    * We can conditionally break to a new line
    * before or after any element. These are
    * just common HTML elements...
    */

    name = mxmlGetElement(node);
    parent_name = mxmlGetElement(node->parent);

    if (boxing_string_equal("source", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("source", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("data", name))
    {
        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n        ");
        }
    }

    return (NULL);
}


static char * get_substring(const char* input_string, size_t start_index, size_t end_index)
{
    if (input_string == NULL || start_index > end_index)
    {
        return NULL;
    }

    if (start_index == end_index)
    {
        return boxing_string_allocate(0);
    }

    size_t string_size = boxing_string_length(input_string);
    if (start_index >= string_size || end_index > string_size)
    {
        return NULL;
    }

    if (start_index == 0 && end_index == string_size)
    {
        return NULL;
    }

    size_t new_string_size = end_index - start_index + 1;
    char* new_string = boxing_string_allocate(new_string_size);

    for (size_t i = 0; i < new_string_size; i++)
    {
        new_string[i] = input_string[i + start_index];
    }

    new_string[new_string_size - 1] = '\0';

    return new_string;
}
