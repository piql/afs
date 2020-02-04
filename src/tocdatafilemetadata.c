/*****************************************************************************
**
**  Definition of the toc data file metadata source interface
**
**  Creation date:  2018/02/01
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

#include "tocdatafilemetadata_c.h"
#include "boxing/platform/memory.h"
#include "boxing/log.h"

// PRIVATE INTERFACE
//

typedef struct toc_data_file_metadata_lengths_s
{
    unsigned int id_length;
    unsigned int file_id_length;
    unsigned int source_id_length;
    unsigned int format_id_length;
    unsigned int data_length;
} toc_data_file_metadata_lengths;

static toc_data_file_metadata_lengths get_max_toc_data_file_metadata_lenght(const afs_toc_data_file_metadata * toc_data_file_metadata);
static unsigned int get_digits_count(unsigned int number);
static const char * whitespace_cb(mxml_node_t *node, int where);


/*! 
  * \addtogroup file
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_data_file_metadata_s  tocdatafilemetadata_c.h
 *  \brief      TOC data file metadata storage.
 *
 *  \param sources  Vector with afs_toc_data_file_metadata instances.
 *
 *  Structure for storing TOC data file metadata.
 */

// PUBLIC AFS TOC DATA FILE METADATA FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create an empty TOC data file metadata instance.
 *
 *  Allocate memory for the afs_toc_data_file_metadata type
 *  and initializes vector for storing TOC data file metadata sources.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_data_file_metadata structure.
 */

afs_toc_data_file_metadata * afs_toc_data_file_metadata_create()
{
    afs_toc_data_file_metadata * toc_data_file_metadata = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_data_file_metadata);
    afs_toc_data_file_metadata_init(toc_data_file_metadata);
    return toc_data_file_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC data file metadata instance from vector of afs_toc_data_file_metadata_sources.
 *
 *  Allocate memory for the afs_toc_data_file_metadata type
 *  and initializes structure vector pointer with specified vector of afs_toc_data_file_metadata_sources.
 *  Return instance of allocated structure.
 *
 *  \param[in]  sources  Pointer to the vector with afs_toc_data_file_metadata_source structure pointers.
 *  \return instance of allocated afs_toc_data_file_metadata structure.
 */

afs_toc_data_file_metadata * afs_toc_data_file_metadata_create2(afs_toc_data_file_metadata_sources * sources)
{
    afs_toc_data_file_metadata * toc_data_file_metadata = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_data_file_metadata);
    afs_toc_data_file_metadata_init2(toc_data_file_metadata, sources);
    return toc_data_file_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes vector pointer in the structure with NULL value.
 *
 *  Initializes vector pointer in the input structure with NULL value.
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  toc_data_file_metadata  Pointer to the afs_toc_data_file_metadata structure.
 */

void afs_toc_data_file_metadata_init(afs_toc_data_file_metadata * toc_data_file_metadata)
{
    if (toc_data_file_metadata == NULL)
    {
        return;
    }

    toc_data_file_metadata->sources = NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes vector pointer in the structure with input value.
 *
 *  Initializes vector pointer in the input afs_toc_data_file_metadata structure with specified input value.
 *  If input afs_toc_data_file_metadata pointers is NULL then return without initialization.
 *
 *  \param[in]  toc_data_file_metadata  Pointer to the afs_toc_data_file_metadata structure.
 *  \param[in]  sources                 Pointer to the vector with afs_toc_data_file_metadata_sources structure pointers.
 */

void afs_toc_data_file_metadata_init2(afs_toc_data_file_metadata * toc_data_file_metadata, afs_toc_data_file_metadata_sources * sources)
{
    if (toc_data_file_metadata == NULL)
    {
        return;
    }

    toc_data_file_metadata->sources = sources;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_data_file_metadata structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_data_file_metadata  Pointer to the afs_toc_data_file_metadata structure.
 */

void afs_toc_data_file_metadata_free(afs_toc_data_file_metadata * toc_data_file_metadata)
{
    if (toc_data_file_metadata == NULL)
    {
        return;
    }

    if (toc_data_file_metadata->sources != NULL)
    {
        for (unsigned int i = 0; i < toc_data_file_metadata->sources->size; i++)
        {
            afs_toc_data_file_metadata_source_free(GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, i));
            GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, i) = NULL;
        }
    }

    gvector_free(toc_data_file_metadata->sources);
    boxing_memory_free(toc_data_file_metadata);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_data_file_metadata structure.
 *
 *  Function create a copy of input afs_toc_data_file_metadata structure and return it.
 *  If TOC data file metadata pointer is NULL function return NULL.
 *
 *  \param[in]  toc_data_file_metadata  Pointer to the afs_toc_data_file_metadata structure.
 *  \return new copy of afs_toc_data_file_metadata structure or NULL.
 */

afs_toc_data_file_metadata * afs_toc_data_file_metadata_clone(const afs_toc_data_file_metadata * toc_data_file_metadata)
{
    if (toc_data_file_metadata == NULL)
    {
        return NULL;
    }

    if (toc_data_file_metadata->sources == NULL)
    {
        return afs_toc_data_file_metadata_create();
    }

    afs_toc_data_file_metadata * return_toc_data_file_metadata = afs_toc_data_file_metadata_create2(gvector_create_pointers(toc_data_file_metadata->sources->size));
    
    for (unsigned int i = 0; i < toc_data_file_metadata->sources->size; i++)
    {
        afs_toc_data_file_metadata_source * toc_data_file_metadata_source = GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, i);
        GVECTORN(return_toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, i) = afs_toc_data_file_metadata_source_clone(toc_data_file_metadata_source);
    }

    return return_toc_data_file_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_data_file_metadata structures on the identity.
 *
 *  Function checks two instances of the afs_toc_data_file_metadata structures on the identity.
 *
 *  \param[in]   toc_data_file_metadata1  Pointer to the first instance of the afs_toc_data_file_metadata structure.
 *  \param[in]   toc_data_file_metadata2  Pointer to the second instance of the afs_toc_data_file_metadata structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_data_file_metadata_equal(const afs_toc_data_file_metadata * toc_data_file_metadata1, const afs_toc_data_file_metadata * toc_data_file_metadata2)
{
    if (toc_data_file_metadata1 == NULL && toc_data_file_metadata2 == NULL)
    {
        return DTRUE;
    }

    if (toc_data_file_metadata1 == NULL || toc_data_file_metadata2 == NULL)
    {
        return DFALSE;
    }

    if (toc_data_file_metadata1->sources == NULL && toc_data_file_metadata2->sources == NULL)
    {
        return DTRUE;
    }

    if (toc_data_file_metadata1->sources == NULL || toc_data_file_metadata2->sources == NULL)
    {
        return DFALSE;
    }

    if (toc_data_file_metadata1->sources->size != toc_data_file_metadata2->sources->size)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_data_file_metadata1->sources->size; i++)
    {
        afs_toc_data_file_metadata_source * source1 = GVECTORN(toc_data_file_metadata1->sources, afs_toc_data_file_metadata_source *, i);
        afs_toc_data_file_metadata_source * source2 = GVECTORN(toc_data_file_metadata2->sources, afs_toc_data_file_metadata_source *, i);

        if (afs_toc_data_file_metadata_source_equal(source1, source2) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_data_file_metadata_source structure to the input afs_toc_data_file_metadata structure.
 *
 *  The function adds a new instance of afs_toc_data_file_metadata_source structure to the input afs_toc_data_file_metadata structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_data_file_metadata_source structure is not add and the function returns DFALSE.
 *  If input afs_toc_data_file_metadata_source instance is not valid then new instance is not add the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_data_file_metadata         Pointer to the instance of the afs_toc_data_file_metadata structure.
 *  \param[in]   toc_data_file_metadata_source  Pointer to the instance of the afs_toc_data_file_metadata_source structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_add_source(afs_toc_data_file_metadata * toc_data_file_metadata, afs_toc_data_file_metadata_source * toc_data_file_metadata_source)
{
    if (toc_data_file_metadata == NULL || toc_data_file_metadata_source == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source) != DTRUE)
    {
        return DFALSE;
    }

    if (toc_data_file_metadata->sources == NULL)
    {
        toc_data_file_metadata->sources = gvector_create_pointers(0);
    }

    size_t old_size = toc_data_file_metadata->sources->size;

    gvector_append_data(toc_data_file_metadata->sources, 1, &toc_data_file_metadata_source);

    if (toc_data_file_metadata->sources->size != old_size + 1)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_data_file_metadata_source structure according to the input index.
 *
 *  The function returns an instance of afs_toc_data_file_metadata_source structure according to the input index.
 *
 *  \param[in]   toc_data_file_metadata  Pointer to the instance of the afs_toc_data_file_metadata structure.
 *  \param[in]   index                   Index for specifying the required instance of the afs_toc_data_file_metadata_source structure in the storage vector.
 *  \return an instance of afs_toc_data_file_metadata_source structure or NULL.
 */

afs_toc_data_file_metadata_source * afs_toc_data_file_metadata_get_source(const afs_toc_data_file_metadata * toc_data_file_metadata, unsigned int index)
{
    if (toc_data_file_metadata == NULL)
    {
        return NULL;
    }

    if (toc_data_file_metadata->sources == NULL)
    {
        return NULL;
    }

    if (index >= toc_data_file_metadata->sources->size)
    {
        return NULL;
    }

    return GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_data_file_metadata_source structure.
 *
 *  The function returns a count of instances of afs_toc_data_file_metadata_source structure.
 *  If input afs_toc_data_file_metadata pointer is NULL or internal sources vector pointer is NULL then function returns zero.
 *
 *  \param[in]   toc_data_file_metadata  Pointer to the instance of the afs_toc_data_file_metadata structure.
 *  \return a count of instances of afs_toc_data_file_metadata_source structure or zero.
 */

unsigned int afs_toc_data_file_metadata_get_sources_count(const afs_toc_data_file_metadata * toc_data_file_metadata)
{
    if (toc_data_file_metadata == NULL)
    {
        return 0;
    }

    if (toc_data_file_metadata->sources == NULL)
    {
        return 0;
    }

    return (unsigned int)toc_data_file_metadata->sources->size;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the all afs_toc_data_file_metadata_source structures in the input afs_toc_data_file_metadata structure.
 *
 *  The function returns the sign of integrity the all afs_toc_data_file_metadata_source structures in the input afs_toc_data_file_metadata structure.
 *  If TOC data file metadata pointer is NULL or internal sources vector pointer is NULL or vector has no pointers then function return DFALSE.
 *
 *  \param[in]   toc_data_file_metadata   Pointer to the afs_toc_data_file_metadata structure.
 *  \return the sign of integrity of all afs_toc_data_file_metadata_source structures.
 */

DBOOL afs_toc_data_file_metadata_is_valid(const afs_toc_data_file_metadata * toc_data_file_metadata)
{
    if (toc_data_file_metadata == NULL)
    {
        return DFALSE;
    }

    if (toc_data_file_metadata->sources == NULL)
    {
        return DTRUE;
    }

    if (toc_data_file_metadata->sources->size == 0)
    {
        return DTRUE;
    }

    for (unsigned int i = 0; i < toc_data_file_metadata->sources->size; i++)
    {
        if (afs_toc_data_file_metadata_source_is_valid(GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, i)) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_data_file_metadata structure to the XML file.
 *
 *  Function translates the input afs_toc_data_file_metadata structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_data_file_metadata  Pointer to the afs_toc_data_file_metadata structure.
 *  \param[in]   file_name               Name of the XML file.
 *  \param[in]   compact                 If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_save_file(afs_toc_data_file_metadata * toc_data_file_metadata, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_data_file_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t *tree = mxmlNewXML("1.0");

    if (!afs_toc_data_file_metadata_save_xml(toc_data_file_metadata, tree))
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
 *  \brief Function translates the input afs_toc_data_file_metadata structure to the XML string.
 *
 *  Function translates the input afs_toc_data_file_metadata structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_data_file_metadata  Pointer to the afs_toc_data_file_metadata structure.
 *  \param[in]   compact                 If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_data_file_metadata_save_string(afs_toc_data_file_metadata * toc_data_file_metadata, DBOOL compact)
{
    if (toc_data_file_metadata == NULL)
    {
        return DFALSE;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_data_file_metadata_save_xml(toc_data_file_metadata, document))
    {
        mxmlDelete(document);
        return NULL;
    }

    char * xmlString;
    mxmlSetWrapMargin(0);

    struct mxml_node_s* metadata = mxmlFindElement( document, document, "metadata", NULL, NULL, MXML_DESCEND );

    xmlString = compact == DTRUE ? mxmlSaveAllocString(metadata, MXML_NO_CALLBACK) : mxmlSaveAllocString(metadata, whitespace_cb);

    mxmlDelete(document);

    return xmlString;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Write metadata as table
 *
 *  Write metadata in ASCII table format. This function will allocate and return 
 *  the output string, it must be freed by the caller.
 *
 *  \param[in]   toc_data_file_metadata Instance to be printed
 *  \return resulting string or NULL.
 */

char * afs_toc_data_file_metadata_save_as_table(const afs_toc_data_file_metadata * toc_data_file_metadata)
{
    if (toc_data_file_metadata == NULL)
    {
        return NULL;
    }

    const char * header =
        "METADATA SOURCES\n"
        "================\n"
        "<id> <fileId> <sourceId> <formatId> <data>\n";

    toc_data_file_metadata_lengths lengths = get_max_toc_data_file_metadata_lenght(toc_data_file_metadata);
    unsigned int sources_count = afs_toc_data_file_metadata_get_sources_count(toc_data_file_metadata);

    unsigned int header_length = (unsigned int)boxing_string_length(header);
    unsigned int table_width = lengths.id_length + lengths.file_id_length + lengths.source_id_length + lengths.format_id_length + lengths.data_length + 4 + strlen("\n");
    unsigned int table_length = table_width * sources_count + 1;

    char * return_string = boxing_string_allocate(header_length + table_length);
    char * current_string = return_string;
    current_string += sprintf(current_string, "%s", header);

    for (unsigned int i = 0; i < sources_count; i++)
    {
        afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_get_source(toc_data_file_metadata, i);
        current_string += sprintf(current_string, "%0*u %0*u %0*u %-*s %s\n", lengths.id_length, i, lengths.file_id_length, toc_data_file_metadata_source->file_id, lengths.source_id_length, toc_data_file_metadata_source->source_id, lengths.format_id_length, toc_data_file_metadata_source->format_id, toc_data_file_metadata_source->data);
    }

    return return_string;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_data_file_metadata structure to the XML nodes.
 *
 *  Function translates the input afs_toc_data_file_metadata structure to the XML nodes.
 *  If input afs_toc_data_file_metadata structure is not valid then function return DFALSE without translation.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_data_file_metadata  Pointer to the afs_toc_data_file_metadata structure.
 *  \param[out]  out                     Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_save_xml(afs_toc_data_file_metadata * toc_data_file_metadata, mxml_node_t* out)
{
    if (out == NULL || toc_data_file_metadata == NULL)
    {
        return DFALSE;
    }

    if (!afs_toc_data_file_metadata_is_valid(toc_data_file_metadata))
    {
        return DFALSE;
    }

    mxml_node_t * metadata_node = mxmlNewElement(out, "metadata");

    if (toc_data_file_metadata->sources == NULL || afs_toc_data_file_metadata_get_sources_count(toc_data_file_metadata) == 0)
    {
        return DTRUE;
    }

    for (unsigned int i = 0; i < toc_data_file_metadata->sources->size; i++)
    {
        DBOOL result = afs_toc_data_file_metadata_source_save_xml(GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, i), metadata_node);

        if (result == DFALSE)
        {
            return DFALSE;
        }
    }
    
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_data_file_metadata structure.
 *
 *  Function translates the input XML nodes to the afs_toc_data_file_metadata structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_file_metadata  Pointer to the afs_toc_data_file_metadata structure.
 *  \param[in]   file_name               Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_load_file(afs_toc_data_file_metadata * toc_data_file_metadata, const char * file_name)
{
    if (file_name == NULL || toc_data_file_metadata == NULL)
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
    
    DBOOL return_value = afs_toc_data_file_metadata_load_xml(toc_data_file_metadata, document);
    
    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_data_file_metadata structure.
 *
 *  Function translates the input XML string to the afs_toc_data_file_metadata structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_file_metadata  Pointer to the afs_toc_data_file_metadata structure.
 *  \param[in]   in                      Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_load_string(afs_toc_data_file_metadata * toc_data_file_metadata, const char * in)
{
    if (in == NULL || boxing_string_equal(in, "") || toc_data_file_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_data_file_metadata_load_xml(toc_data_file_metadata, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC data file metadata structure.
 *
 *  Function translates the input XML nodes to the TOC data file metadata structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_file_metadata  Pointer to the afs_toc_data_file_metadata structure.
 *  \param[in]   node                    Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_file_metadata_load_xml(afs_toc_data_file_metadata * toc_data_file_metadata, mxml_node_t* node)
{
    if (node == NULL || toc_data_file_metadata == NULL)
    {
        DLOG_ERROR("setup error.\n");
        return DFALSE;
    }

    mxml_node_t * metadata_node = boxing_string_equal(mxmlGetElement(node), "metadata") == DTRUE ? node : mxmlFindElement(node, node, "metadata", NULL, NULL, MXML_DESCEND);

    if (metadata_node == NULL)
    {
        DLOG_ERROR("Not a metadata node.\n");
        return DFALSE;
    }

    mxml_node_t * source_node = mxmlFindElement(metadata_node, metadata_node, "source", NULL, NULL, MXML_DESCEND);

    while (source_node != NULL)
    {
        if (mxmlGetType(source_node) == MXML_ELEMENT)
        {
            afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

            DBOOL load_toc_result = afs_toc_data_file_metadata_source_load_xml(toc_data_file_metadata_source, source_node);

            if (load_toc_result == DFALSE)
            {
                DLOG_ERROR("Has no TOC data file metadata source.\n");
                afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
                return DFALSE;
            }

            DBOOL add_toc_result = afs_toc_data_file_metadata_add_source(toc_data_file_metadata, toc_data_file_metadata_source);

            if (add_toc_result == DFALSE)
            {
                DLOG_ERROR("Failed to add file metadata source.\n");
                afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
                return DFALSE;
            }
        }

        source_node = mxmlFindElement(source_node, metadata_node, "source", NULL, NULL, MXML_DESCEND);
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of file group
  */


// PRIVATE AFS TOC DATA FILE METADATA FUNCTIONS
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

    if (boxing_string_equal("metadata", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("source", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("source", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("data", name))
    {
        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n            ");
        }
    }

    return (NULL);
}


static toc_data_file_metadata_lengths get_max_toc_data_file_metadata_lenght(const afs_toc_data_file_metadata * toc_data_file_metadata)
{
    toc_data_file_metadata_lengths lengths;

    lengths.id_length = 1; //(unsigned int)boxing_string_length("<id>");
    lengths.file_id_length = 0;//(unsigned int)boxing_string_length("<fileId>");
    lengths.source_id_length = 0;//(unsigned int)boxing_string_length("<sourceId>");
    lengths.format_id_length = 0;//(unsigned int)boxing_string_length("<formatId>");
    lengths.data_length = 0;//(unsigned int)boxing_string_length("<data>");

    if (toc_data_file_metadata == NULL)
    {
        return lengths;
    }

    if (toc_data_file_metadata->sources == NULL)
    {
        return lengths;
    }

    for (unsigned int i = 0; i < toc_data_file_metadata->sources->size; i++)
    {
        afs_toc_data_file_metadata_source * toc_data_file_metadata_source = GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, i);

        lengths.id_length = get_digits_count(i) > lengths.id_length ? get_digits_count(i) : lengths.id_length;
        lengths.file_id_length = get_digits_count(toc_data_file_metadata_source->file_id) > lengths.file_id_length ? get_digits_count(toc_data_file_metadata_source->file_id) : lengths.file_id_length;
        lengths.source_id_length = get_digits_count(toc_data_file_metadata_source->source_id) > lengths.source_id_length ? get_digits_count(toc_data_file_metadata_source->source_id) : lengths.source_id_length;
        lengths.format_id_length = (unsigned int)boxing_string_length(toc_data_file_metadata_source->format_id) > lengths.format_id_length ? (unsigned int)boxing_string_length(toc_data_file_metadata_source->format_id) : lengths.format_id_length;
        lengths.data_length = (unsigned int)boxing_string_length(toc_data_file_metadata_source->data) > lengths.data_length ? (unsigned int)boxing_string_length(toc_data_file_metadata_source->data) : lengths.data_length;
    }

    return lengths;
}


static unsigned int get_digits_count(unsigned int number)
{
    return number / 10 != 0 ? 1 + get_digits_count(number / 10) : 1;
}
