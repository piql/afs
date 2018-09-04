/*****************************************************************************
**
**  Definition of the toc metadata source interface
**
**  Creation date:  2017/08/23
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
#include "tocmetadatasource_c.h"
#include "xmlutils.h"
#include "platform/io.h"
#include "boxing/string.h"

//  DEFINES
//

// PRIVATE INTERFACE
//
static DBOOL        toc_metadata_source_tags_equal(afs_toc_metadata_source_tags * metadata_source_tags1, afs_toc_metadata_source_tags * metadata_source_tags2);
static const char * whitespace_cb(mxml_node_t *node, int where);
static char *       get_substring(const char* input_string, size_t start_index, size_t end_index);


/*! 
  * \addtogroup metadata
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \var enum  ID_UNDEFINED
 *  \brief     Uninitialized source ID
 *  
 *  Source ID uninitialized constant.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  FILE_ID_UNDEFINED
 *  \brief     Uninitialized file ID
 *
 *  File ID uninitialized constant.
 */


/*! 
 *  \typedef  gvector afs_toc_metadata_sources
 *  \brief Array of afs_toc_metadata_source pointers.
 *
 *  Dynamic array of afs_toc_metadata_source pointers.
 */


/*! 
 *  \typedef  gvector afs_toc_metadata_source_tags
 *  \brief Array of metadata tag strings.
 *
 *  Dynamic array of metadata tag strings.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_metadata_source_s  tocmetadatasource_c.h
 *  \brief      Describes a metadata source.
 *
 *  \param format     Data format.
 *  \param id         Source id.
 *  \param file_id    File id.
 *  \param data       Data.
 *  \param tags       Tags.
 *
 *  A metadata source is an element in the TOC describing where more metadata 
 *  for the job can be found, it can also be used to define the tags for the 
 *  file metadata entries.
 *  
 *  If file_id is not FILE_ID_UNDEFINED, the source point to a file in the TOC 
 *  that has metadata for the job. In this case data can contain data extracted 
 *  from the file, and format defines the format of that data. If tags is not 
 *  NULL, the data should be a list of semicolon separated values (;) and 
 *  tags should contain a list of semicolon separated keys corresponding to those
 *  values.
 *
 *  If file_id equals FILE_ID_UNDEFINED and data is not NULL, the source can 
 *  contains manually entered metadata (metadata added during reel creation) in 
 *  data. If tags is not NULL it has the same function as described above.
 *
 *  If file_id equals FILE_ID_UNDEFINED and data is NULL, the tags should be a
 *  list of semicolon separated keys. File metadata in the toc will point to 
 *  this source. This ways multiple file entries can use the same set of keys, 
 *  this avoids duplication and saves space.
 */

// PUBLIC AFS TOC METADATA SOURCE FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc metadata source instance.
 *
 *  Allocate memory for the afs_toc_metadata_source type
 *  and initializes all structure pointers with NULL values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_metadata_source structure.
 */

afs_toc_metadata_source * afs_toc_metadata_source_create()
{
    afs_toc_metadata_source * toc_metadata_source = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_metadata_source);
    afs_toc_metadata_source_init(toc_metadata_source);
    return toc_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc metadata source instance.
 *
 *  Allocate memory for the afs_toc_metadata_source type
 *  and initializes format name and file id with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] format     Format name.
 *  \param[in] file_id    File id.
 *  \return instance of allocated afs_toc_metadata_source structure.
 */

afs_toc_metadata_source * afs_toc_metadata_source_create2(const char * format, int file_id)
{
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();
    afs_toc_metadata_source_init2(toc_metadata_source, format, file_id, NULL, NULL, ID_UNDEFINED);
    return toc_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc metadata source instance.
 *
 *  Allocate memory for the afs_toc_metadata_source type
 *  and initializes format name, file id and id with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] format     Format name.
 *  \param[in] file_id    File id.
 *  \param[in] id         Metadata id.
 *  \return instance of allocated afs_toc_metadata_source structure.
 */

afs_toc_metadata_source * afs_toc_metadata_source_create3(const char * format, int file_id, int id)
{
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();
    afs_toc_metadata_source_init2(toc_metadata_source, format, file_id, NULL, NULL, id);
    return toc_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc metadata source instance.
 *
 *  Allocate memory for the afs_toc_metadata_source type
 *  and initializes format name, file id and XML data with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] format     Format name.
 *  \param[in] file_id    File id.
 *  \param[in] data       XML data.
 *  \return instance of allocated afs_toc_metadata_source structure.
 */

afs_toc_metadata_source * afs_toc_metadata_source_create4(const char * format, int file_id, const char * data)
{
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();
    afs_toc_metadata_source_init2(toc_metadata_source, format, file_id, data, NULL, ID_UNDEFINED);
    return toc_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc metadata source instance.
 *
 *  Allocate memory for the afs_toc_metadata_source type
 *  and initializes format name, file id, XML data and id with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] format     Format name.
 *  \param[in] file_id    File id.
 *  \param[in] data       XML data.
 *  \param[in] id         Metadata id.
 *  \return instance of allocated afs_toc_metadata_source structure.
 */

afs_toc_metadata_source * afs_toc_metadata_source_create5(const char * format, int file_id, const char * data, int id)
{
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();
    afs_toc_metadata_source_init2(toc_metadata_source, format, file_id, data, NULL, id);
    return toc_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc metadata source instance.
 *
 *  Allocate memory for the afs_toc_metadata_source type
 *  and initializes format name, file id, XML data and XML tags with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] format     Format name.
 *  \param[in] file_id    File id.
 *  \param[in] data       XML data.
 *  \param[in] tags       XML tags. (string format)
 *  \return instance of allocated afs_toc_metadata_source structure.
 */

afs_toc_metadata_source * afs_toc_metadata_source_create6(const char * format, int file_id, const char * data, const char * tags)
{
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();
    afs_toc_metadata_source_init2(toc_metadata_source, format, file_id, data, tags, ID_UNDEFINED);
    return toc_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc metadata source instance.
 *
 *  Allocate memory for the afs_toc_metadata_source type
 *  and initializes format name, file id, XML data, XML tags ans id with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] format     Format name.
 *  \param[in] file_id    File id.
 *  \param[in] data       XML data.
 *  \param[in] tags       XML tags. (string format)
 *  \param[in] id         Metadata id.
 *  \return instance of allocated afs_toc_metadata_source structure.
 */

afs_toc_metadata_source * afs_toc_metadata_source_create7(const char * format, int file_id, const char * data, const char * tags, int id)
{
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();
    afs_toc_metadata_source_init2(toc_metadata_source, format, file_id, data, tags, id);
    return toc_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc metadata source instance.
 *
 *  Allocate memory for the afs_toc_metadata_source type
 *  and initializes format name, file id and XML tags with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] format     Format name.
 *  \param[in] file_id    File id.
 *  \param[in] tags       XML tags. (vector format)
 *  \return instance of allocated afs_toc_metadata_source structure.
 */

afs_toc_metadata_source * afs_toc_metadata_source_create8(const char * format, int file_id, afs_toc_metadata_source_tags * tags)
{
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();
    afs_toc_metadata_source_init3(toc_metadata_source, format, file_id, tags, ID_UNDEFINED);
    return toc_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc metadata source instance.
 *
 *  Allocate memory for the afs_toc_metadata_source type
 *  and initializes format name, file id, XML tags and id with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] format     Format name.
 *  \param[in] file_id    File id.
 *  \param[in] tags       XML tags. (vector format)
 *  \param[in] id         Metadata id.
 *  \return instance of allocated afs_toc_metadata_source structure.
 */

afs_toc_metadata_source * afs_toc_metadata_source_create9(const char * format, int file_id, afs_toc_metadata_source_tags * tags, int id)
{
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();
    afs_toc_metadata_source_init3(toc_metadata_source, format, file_id, tags, id);
    return toc_metadata_source;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers with NULL value.
 *
 *  Initializes all input structure pointers with NULL values.
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  toc_metadata_source  Pointer to the afs_toc_metadata_source structure.
 */

void afs_toc_metadata_source_init(afs_toc_metadata_source * toc_metadata_source)
{
    if (toc_metadata_source == NULL)
    {
        return;
    }

    toc_metadata_source->format = NULL;
    toc_metadata_source->id = ID_UNDEFINED;
    toc_metadata_source->file_id = FILE_ID_UNDEFINED;
    toc_metadata_source->data = NULL;
    toc_metadata_source->tags = NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers with specified input values.
 *
 *  Initializes all input structure pointers with specified input values.
 *  If input toc_metadata_source pointer is NULL, then return without initialization.
 *
 *  \param[in] toc_metadata_source  Pointer to the afs_toc_metadata_source structure.
 *  \param[in] format               Format name.
 *  \param[in] file_id              File id.
 *  \param[in] data                 XML data.
 *  \param[in] tags                 XML tags. (string format)
 *  \param[in] id                   Metadata id.
 */

void afs_toc_metadata_source_init2(afs_toc_metadata_source * toc_metadata_source, const char * format, int file_id, const char * data, const char * tags, int id)
{
    if (toc_metadata_source == NULL)
    {
        return;
    }

    toc_metadata_source->format = boxing_string_clone(format);
    toc_metadata_source->id = id;
    toc_metadata_source->file_id = file_id;
    toc_metadata_source->data = boxing_string_clone(data);
    toc_metadata_source->tags = boxing_string_split(tags, ";");
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers with specified input values.
 *
 *  Initializes all input structure pointers with specified input values.
 *  If input toc_metadata_source pointer is NULL, then return without initialization.
 *
 *  \param[in] toc_metadata_source  Pointer to the afs_toc_metadata_source structure.
 *  \param[in] format               Format name.
 *  \param[in] file_id              File id.
 *  \param[in] tags                 XML tags. (string format)
 *  \param[in] id                   Metadata id.
 */

void afs_toc_metadata_source_init3(afs_toc_metadata_source * toc_metadata_source, const char * format, int file_id, afs_toc_metadata_source_tags * tags, int id)
{
    if (toc_metadata_source == NULL)
    {
        return;
    }

    toc_metadata_source->format = boxing_string_clone(format);
    toc_metadata_source->id = id;
    toc_metadata_source->file_id = file_id;
    toc_metadata_source->data = NULL;
    toc_metadata_source->tags = tags;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_metadata_source structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_metadata_source  Pointer to the afs_toc_metadata_source structure.
 */

void afs_toc_metadata_source_free(afs_toc_metadata_source * toc_metadata_source)
{
    if (toc_metadata_source == NULL)
    {
        return;
    }

    if (toc_metadata_source->tags != NULL)
    {
        for (unsigned int i = 0; i < toc_metadata_source->tags->size; i++)
        {
            boxing_string_free(GVECTORN(toc_metadata_source->tags, char *, i));
            GVECTORN(toc_metadata_source->tags, char *, i) = NULL;
        }
    }

    boxing_string_free(toc_metadata_source->format);
    boxing_string_free(toc_metadata_source->data);
    gvector_free(toc_metadata_source->tags);
    boxing_memory_free(toc_metadata_source);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_metadata_source structure.
 *
 *  Function create a copy of input afs_toc_metadata_source structure and return it.
 *  If TOC metadata source pointer is NULL function return NULL.
 *
 *  \param[in]  toc_metadata_source  Pointer to the afs_toc_metadata_source structure.
 *  \return new copy of toc_metadata_source structure or NULL.
 */

afs_toc_metadata_source * afs_toc_metadata_source_clone(afs_toc_metadata_source * toc_metadata_source)
{
    // If TOC metadata source pointer is NULL return NULL.
    if (toc_metadata_source == NULL)
    {
        return NULL;
    }

    afs_toc_metadata_source * return_copy = afs_toc_metadata_source_create();

    return_copy->format = boxing_string_clone(toc_metadata_source->format);
    return_copy->id = toc_metadata_source->id;
    return_copy->file_id = toc_metadata_source->file_id;
    return_copy->data = boxing_string_clone(toc_metadata_source->data);
    
    if (toc_metadata_source->tags != NULL)
    {
        return_copy->tags = gvector_create(toc_metadata_source->tags->item_size, toc_metadata_source->tags->size);
        for (unsigned int i = 0; i < toc_metadata_source->tags->size; i++)
        {
            GVECTORN(return_copy->tags, char *, i) = boxing_string_clone(GVECTORN(toc_metadata_source->tags, char *, i));
        }
    }
    else
    {
        return_copy->tags = NULL;
    }

    return return_copy;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_metadata_source structures on the identity.
 *
 *  Function checks two instances of the afs_toc_metadata_source structures on the identity.
 *
 *  \param[in]   toc_metadata_source1  Pointer to the first instance of the afs_toc_metadata_source structure.
 *  \param[in]   toc_metadata_source2  Pointer to the second instance of the afs_toc_metadata_source structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_metadata_source_equal(afs_toc_metadata_source * toc_metadata_source1, afs_toc_metadata_source * toc_metadata_source2)
{
    if (toc_metadata_source1 == NULL && toc_metadata_source2 == NULL)
    {
        return DTRUE;
    }

    if (toc_metadata_source1 == NULL || toc_metadata_source2 == NULL)
    {
        return DFALSE;
    }

    DBOOL tags_identity = toc_metadata_source_tags_equal(toc_metadata_source1->tags, toc_metadata_source2->tags);

    if (tags_identity == DFALSE ||
        boxing_string_equal(toc_metadata_source1->format, toc_metadata_source2->format) == DFALSE ||
        toc_metadata_source1->id != toc_metadata_source2->id ||
        toc_metadata_source1->file_id != toc_metadata_source2->file_id ||
        boxing_string_equal(toc_metadata_source1->data, toc_metadata_source2->data) == DFALSE)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the afs_toc_metadata_source structure.
 *
 *  The function returns the sign of integrity the current structure.
 *  If TOC metadata source pointer is NULL then function return DFALSE.
 *
 *  \param[in]   toc_metadata_source   Pointer to the afs_toc_metadata_source structure.
 *  \return the sign of integrity the afs_toc_metadata_source structure.
 */

DBOOL afs_toc_metadata_source_is_valid(afs_toc_metadata_source * toc_metadata_source)
{
    // If TOC metadata source pointer is NULL return DFALSE
    if (toc_metadata_source == NULL)
    {
        return DFALSE;
    }

    if (toc_metadata_source->format == NULL ||
        boxing_string_equal(toc_metadata_source->format, "") == DTRUE ||
        (toc_metadata_source->file_id != FILE_ID_UNDEFINED && toc_metadata_source->file_id < 0) ||
        (toc_metadata_source->file_id == FILE_ID_UNDEFINED && (toc_metadata_source->data == NULL || boxing_string_equal(toc_metadata_source->data, ""))))
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function sets the list of tags.
 *
 *  The function sets the list of tags according to a string with tags which separated by a character ';'.
 *  Before setting tags, the vector with tags in the structure is cleared.
 *  If TOC metadata source pointer or tags pointer is NULL or tags string is empty then function return without setting.
 *
 *  \param[in]   toc_metadata_source   Pointer to the afs_toc_metadata_source structure.
 *  \param[in]   tags                  A string with a list of tags separated by ';'.
 */

void afs_toc_metadata_source_set_tags(afs_toc_metadata_source * toc_metadata_source, const char * tags)
{
    if (toc_metadata_source == NULL || tags == NULL || boxing_string_equal(tags, "") == DTRUE)
    {
        return;
    }

    if (toc_metadata_source->tags != NULL)
    {
        for (unsigned int i = 0; i < toc_metadata_source->tags->size; i++)
        {
            boxing_string_free(GVECTORN(toc_metadata_source->tags, char *, i));
            GVECTORN(toc_metadata_source->tags, char *, i) = NULL;
        }
        gvector_free(toc_metadata_source->tags);
    }

    toc_metadata_source->tags = boxing_string_split(tags, ";");
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gets the string of tags.
 *
 *  The function gets the string of tags which separated by a character ';' 
 *  according to a tags vector in the input afs_toc_metadata_source instance.
 *  If TOC metadata source pointer is NULL then function return NULL.
 *
 *  \param[in]   toc_metadata_source   Pointer to the afs_toc_metadata_source structure.
 *  \return string of tags or NULL.
 */

char * afs_toc_metadata_source_get_tags(afs_toc_metadata_source * toc_metadata_source)
{
    // If input afs_toc_metadata_source pointer is NULL return NULL
    if (toc_metadata_source == NULL)
    {
        return NULL;
    }

    // If tags pointer is NULL return NULL
    if (toc_metadata_source->tags == NULL)
    {
        return NULL;
    }

    size_t string_size = 0;

    for (unsigned int i = 0; i < toc_metadata_source->tags->size; i++)
    {
        string_size += boxing_string_length(GVECTORN(toc_metadata_source->tags, char *, i));
        if (i < toc_metadata_source->tags->size - 1)
        {
            string_size++;
        }
    }

    char * return_string = boxing_string_allocate(string_size);

    for (unsigned int i = 0; i < toc_metadata_source->tags->size; i++)
    {
        strcat(return_string, GVECTORN(toc_metadata_source->tags, char *, i));
        if (i < toc_metadata_source->tags->size - 1)
        {
            strcat(return_string, ";");
        }
    }

    return return_string;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_metadata_source structure to the XML nodes.
 *
 *  Function translates the input afs_toc_metadata_source structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_metadata_source   Pointer to the afs_toc_metadata_source structure.
 *  \param[out]  out                   Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_source_save_xml(afs_toc_metadata_source * toc_metadata_source, mxml_node_t* out)
{
    // If output node pointer is NULL or TOC metadata source pointer is NULL return DFALSE
    if (out == NULL || toc_metadata_source == NULL || afs_toc_metadata_source_is_valid(toc_metadata_source) == DFALSE)
    {
        return DFALSE;
    }

    mxml_node_t * source_node = mxmlNewElement(out, "source");

    mxmlElementSetAttr(source_node, "formatId", toc_metadata_source->format);
    mxmlElementSetAttrf(source_node, "id", "%d", toc_metadata_source->id);
    mxmlElementSetAttrf(source_node, "fileId", "%d", toc_metadata_source->file_id);

    if (toc_metadata_source->data != NULL) //  && boxing_string_length(toc_metadata_source->data)
    {
        mxml_node_t * data_node = mxmlNewElement(source_node, "data");
        mxmlNewCDATA(data_node, toc_metadata_source->data);
    }

    if (toc_metadata_source->tags != NULL)
    {
        char * source_tags = afs_toc_metadata_source_get_tags(toc_metadata_source);
        afs_xmlutils_add_new_text_node(source_node, "tags", source_tags);
        boxing_string_free(source_tags);
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_metadata_source structure to the XML string.
 *
 *  Function translates the input afs_toc_metadata_source structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_metadata_source  Pointer to the afs_toc_metadata_source structure.
 *  \param[in]   compact              If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_metadata_source_save_string(afs_toc_metadata_source * toc_metadata_source, DBOOL compact)
{
    // If TOC metadata source pointer is NULL return DFALSE
    if (toc_metadata_source == NULL)
    {
        return DFALSE;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_metadata_source_save_xml(toc_metadata_source, document))
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
 *  \brief Function translates the input afs_toc_metadata_source structure to the XML file.
 *
 *  Function translates the input afs_toc_metadata_source structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_metadata_source  Pointer to the afs_toc_metadata_source structure.
 *  \param[in]   file_name            Name of the XML file.
 *  \param[in]   compact              If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_source_save_file(afs_toc_metadata_source * toc_metadata_source, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_metadata_source == NULL || afs_toc_metadata_source_is_valid(toc_metadata_source) == DFALSE)
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

    if (!afs_toc_metadata_source_save_xml(toc_metadata_source, tree))
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
 *  \brief Function translates the input XML nodes to the TOC metadata source structure.
 *
 *  Function translates the input XML nodes to the TOC metadata source structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_metadata_source  Pointer to the afs_toc_metadata_source structure.
 *  \param[in]   input_node           Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_source_load_xml(afs_toc_metadata_source * toc_metadata_source, mxml_node_t* input_node)
{
    // If input node pointer is NULL or TOC metadata source pointer is NULL return DFALSE
    if (input_node == NULL || toc_metadata_source == NULL)
    {
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
        return DFALSE;
    }

    toc_metadata_source->format = boxing_string_clone(mxmlElementGetAttr(source_node, "formatId"));
    if (afs_xmlutils_get_integer_attribute(&toc_metadata_source->id, source_node, "id") == DFALSE)
    {
        return DFALSE;
    }
    if (afs_xmlutils_get_integer_attribute(&toc_metadata_source->file_id, source_node, "fileId") == DFALSE)
    {
        return DFALSE;
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
                    toc_metadata_source->data = get_substring(cdata, 0, cdataLength - 2);
                }
                else
                {
                    toc_metadata_source->data = boxing_string_clone(cdata);
                }
                break;
            }

            cdataNode = mxmlWalkNext(cdataNode, data_node, MXML_NO_DESCEND);
        }

        if (toc_metadata_source->data == NULL)
        {
            toc_metadata_source->data = afs_xmlutils_get_node_text(data_node);
        }
    }

    mxml_node_t * tags_node = mxmlFindElement(source_node, source_node, "tags", NULL, NULL, MXML_DESCEND);
    if (tags_node != NULL)
    {
        char * node_text = afs_xmlutils_get_node_text(tags_node);
        afs_toc_metadata_source_set_tags(toc_metadata_source, node_text);
        boxing_string_free(node_text);
    }

    return afs_toc_metadata_source_is_valid(toc_metadata_source);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_metadata_source structure.
 *
 *  Function translates the input XML string to the afs_toc_metadata_source structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_metadata_source  Pointer to the afs_toc_metadata_source structure.
 *  \param[in]   in                   Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_source_load_string(afs_toc_metadata_source * toc_metadata_source, const char * in)
{
    // If input string pointer is NULL or TOC metadata source pointer is NULL return DFALSE
    if (in == NULL || boxing_string_equal(in, "") || toc_metadata_source == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_metadata_source_load_xml(toc_metadata_source, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_metadata_source structure.
 *
 *  Function translates the input XML nodes to the afs_toc_metadata_source structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_metadata_source  Pointer to the afs_toc_metadata_source structure.
 *  \param[in]   file_name            Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_source_load_file(afs_toc_metadata_source * toc_metadata_source, const char * file_name)
{
    // If input file name string pointer is NULL or TOC metadata source pointer is NULL return DFALSE
    if (file_name == NULL || toc_metadata_source == NULL)
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

    DBOOL return_value = afs_toc_metadata_source_load_xml(toc_metadata_source, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
  * \} end of metadata group
  */


// PRIVATE AFS TOC METADATA SOURCE FUNCTIONS
//

static DBOOL toc_metadata_source_tags_equal(afs_toc_metadata_source_tags * metadata_source_tags1, afs_toc_metadata_source_tags * metadata_source_tags2)
{
    if (metadata_source_tags1 == NULL && metadata_source_tags2 == NULL)
    {
        return DTRUE;
    }

    if (metadata_source_tags1 == NULL || metadata_source_tags2 == NULL)
    {
        return DFALSE;
    }

    if (metadata_source_tags1->size != metadata_source_tags2->size)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < metadata_source_tags1->size; i++)
    {
        if (boxing_string_equal(GVECTORN(metadata_source_tags1, const char *, i), GVECTORN(metadata_source_tags2, const char *, i)) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


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

    if (boxing_string_equal("data", name) || boxing_string_equal("tags", name))
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
