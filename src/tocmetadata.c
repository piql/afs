/*****************************************************************************
**
**  Definition of the toc metadata source interface
**
**  Creation date:  2017/08/22
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

#include "tocmetadata_c.h"
#include "boxing/log.h"
#include "mxml.h"

// PRIVATE INTERFACE
//

static const char * whitespace_cb(mxml_node_t *node, int where);


/*! 
  * \addtogroup metadata
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_metadata_s  tocmetadata_c.h
 *  \brief      TOC metadata storage.
 *
 *  \param sources  Vector with afs_toc_metadata_source instances.
 *
 *  Structure for storing TOC metadata.
 */

// PUBLIC AFS TOC METADATA FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create an empty TOC metadata instance.
 *
 *  Allocate memory for the afs_toc_metadata type
 *  and initializes vector for storing TOC metadata sources.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_metadata structure.
 */

afs_toc_metadata* afs_toc_metadata_create(void)
{
    afs_toc_metadata* toc_metadata = malloc(sizeof(afs_toc_metadata));
    afs_toc_metadata_init(toc_metadata);
    return toc_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC metadata instance from vector of afs_toc_metadata_sources.
 *
 *  Allocate memory for the afs_toc_metadata type
 *  and initializes structure vector pointer with specified vector of afs_toc_metadata_sources.
 *  Return instance of allocated structure.
 *
 *  \param[in]  sources  Pointer to the vector with afs_toc_metadata_source structure pointers.
 *  \return instance of allocated afs_toc_metadata structure.
 */

afs_toc_metadata * afs_toc_metadata_create2(afs_toc_metadata_sources * sources)
{
    afs_toc_metadata* toc_metadata = malloc(sizeof(afs_toc_metadata));
    afs_toc_metadata_init2(toc_metadata, sources);
    return toc_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC metadata instance from XML nodes.
 *
 *  Allocate memory for the afs_toc_metadata type
 *  and translates the input XML nodes to the TOC metadata structure.
 *  Return instance of allocated structure.
 *
 *  \param[in]  sources_node  Pointer to the source XML node.
 *  \return instance of allocated afs_toc_metadata structure.
 */

afs_toc_metadata * afs_toc_metadata_create3(mxml_node_t* sources_node)
{
    afs_toc_metadata* toc_metadata = afs_toc_metadata_create();
    afs_toc_metadata_load_xml(toc_metadata, sources_node);
    return toc_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes vector pointer in the structure with NULL value.
 *
 *  Initializes vector pointer in the input structure with NULL value.
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  toc_metadata  Pointer to the afs_toc_metadata structure.
 */

void afs_toc_metadata_init(afs_toc_metadata * toc_metadata)
{
    if (toc_metadata == NULL)
    {
        return;
    }

    toc_metadata->sources = NULL;
    toc_metadata->reference_count = 1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes vector pointer in the structure with input value.
 *
 *  Initializes vector pointer in the input afs_toc_metadata structure with specified input value.
 *  If input afs_toc_metadata pointers is NULL then return without initialization.
 *
 *  \param[in]  toc_metadata  Pointer to the afs_toc_metadata structure.
 *  \param[in]  sources       Pointer to the vector with afs_toc_metadata_sources structure pointers.
 */

void afs_toc_metadata_init2(afs_toc_metadata * toc_metadata, afs_toc_metadata_sources * sources)
{
    if (toc_metadata == NULL)
    {
        return;
    }

    toc_metadata->sources = sources;
    toc_metadata->reference_count = 1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_metadata structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_metadata  Pointer to the afs_toc_metadata structure.
 */

void afs_toc_metadata_free(afs_toc_metadata * toc_metadata)
{
    if (toc_metadata == NULL)
    {
        return;
    }

    toc_metadata->reference_count--;

    if (toc_metadata->reference_count <= 0)
    {
        if (toc_metadata->sources != NULL)
        {
            for (unsigned int i = 0; i < toc_metadata->sources->size; i++)
            {
                afs_toc_metadata_source_free(GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, i));
                GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, i) = NULL;
            }
        }

        gvector_free(toc_metadata->sources);
        free(toc_metadata);
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_metadata structure.
 *
 *  Function create a copy of input afs_toc_metadata structure and return it.
 *  If TOC metadata pointer is NULL function return NULL.
 *
 *  \param[in]  toc_metadata  Pointer to the afs_toc_metadata structure.
 *  \return new copy of afs_toc_metadata structure or NULL.
 */

afs_toc_metadata * afs_toc_metadata_clone(afs_toc_metadata * toc_metadata)
{
    // If TOC metadata pointer is NULL return NULL.
    if (toc_metadata == NULL)
    {
        return NULL;
    }

    if (toc_metadata->sources == NULL)
    {
        return afs_toc_metadata_create();
    }

    afs_toc_metadata * return_toc_metadata = afs_toc_metadata_create2(gvector_create_pointers(toc_metadata->sources->size));
    
    for (unsigned int i = 0; i < toc_metadata->sources->size; i++)
    {
        afs_toc_metadata_source * toc_metadata_source = GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, i);
        GVECTORN(return_toc_metadata->sources, afs_toc_metadata_source *, i) = afs_toc_metadata_source_clone(toc_metadata_source);
    }

    return return_toc_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function returns a new reference to the input afs_toc_metadata structure.
 *
 *  Function returns a new reference to the input afs_toc_metadata structure.
 *  The reference count is incremented by 1.
 *  If TOC metadata pointer is NULL function return NULL.
 *
 *  \param[in]  toc_metadata  Pointer to the afs_toc_metadata structure.
 *  \return new reference of afs_toc_metadata structure or NULL.
 */

afs_toc_metadata * afs_toc_metadata_get_new_reference(afs_toc_metadata * toc_metadata)
{
    // If TOC metadata pointer is NULL return NULL.
    if (toc_metadata == NULL)
    {
        return NULL;
    }

    toc_metadata->reference_count++;
    return toc_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_metadata structures on the identity.
 *
 *  Function checks two instances of the afs_toc_metadata structures on the identity.
 *
 *  \param[in]   toc_metadata1  Pointer to the first instance of the afs_toc_metadata structure.
 *  \param[in]   toc_metadata2  Pointer to the second instance of the afs_toc_metadata structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_metadata_equal(afs_toc_metadata * toc_metadata1, afs_toc_metadata * toc_metadata2)
{
    if (toc_metadata1 == NULL && toc_metadata2 == NULL)
    {
        return DTRUE;
    }

    if (toc_metadata1 == NULL || toc_metadata2 == NULL)
    {
        return DFALSE;
    }

    if (toc_metadata1->sources == NULL && toc_metadata2->sources == NULL)
    {
        return DTRUE;
    }

    if (toc_metadata1->sources == NULL || toc_metadata2->sources == NULL)
    {
        return DFALSE;
    }

    if (toc_metadata1->sources->size != toc_metadata2->sources->size)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_metadata1->sources->size; i++)
    {
        afs_toc_metadata_source * source1 = GVECTORN(toc_metadata1->sources, afs_toc_metadata_source *, i);
        afs_toc_metadata_source * source2 = GVECTORN(toc_metadata2->sources, afs_toc_metadata_source *, i);

        if (afs_toc_metadata_source_equal(source1, source2) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_metadata_source structure to the input afs_toc_metadata structure.
 *
 *  The function adds a new instance of afs_toc_metadata_source structure to the input afs_toc_metadata structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_metadata_source structure is not add and the function returns DFALSE.
 *  If input afs_toc_metadata_source instance is not valid then new instance is not add the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_metadata         Pointer to the instance of the afs_toc_metadata structure.
 *  \param[in]   toc_metadata_source  Pointer to the instance of the afs_toc_metadata_source structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_add_source(afs_toc_metadata * toc_metadata, afs_toc_metadata_source * toc_metadata_source)
{
    if (toc_metadata == NULL || toc_metadata_source == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_metadata_source_is_valid(toc_metadata_source) != DTRUE)
    {
        return DFALSE;
    }

    if (toc_metadata->sources == NULL)
    {
        toc_metadata->sources = gvector_create_pointers(0);
    }

    size_t old_size = toc_metadata->sources->size;

    gvector_append_data(toc_metadata->sources, 1, &toc_metadata_source);

    if (toc_metadata->sources->size != old_size + 1)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_metadata_source structure according to the input index.
 *
 *  The function returns an instance of afs_toc_metadata_source structure according to the input index.
 *
 *  \param[in]   toc_metadata  Pointer to the instance of the afs_toc_metadata structure.
 *  \param[in]   index         Index for specifying the required instance of the afs_toc_metadata_source structure in the storage vector.
 *  \return an instance of afs_toc_metadata_source structure or NULL.
 */

afs_toc_metadata_source * afs_toc_metadata_get_source(afs_toc_metadata * toc_metadata, unsigned int index)
{
    if (toc_metadata == NULL)
    {
        return NULL;
    }

    if (toc_metadata->sources == NULL)
    {
        return NULL;
    }

    if (index >= toc_metadata->sources->size)
    {
        return NULL;
    }

    return GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_metadata_source structure.
 *
 *  The function returns a count of instances of afs_toc_metadata_source structure.
 *  If input afs_toc_metadata pointer is NULL or internal sources vector pointer is NULL then function returns zero.
 *
 *  \param[in]   toc_metadata  Pointer to the instance of the afs_toc_metadata structure.
 *  \return a count of instances of afs_toc_metadata_source structure or zero.
 */

unsigned int afs_toc_metadata_get_sources_count(afs_toc_metadata * toc_metadata)
{
    if (toc_metadata == NULL)
    {
        return 0;
    }

    if (toc_metadata->sources == NULL)
    {
        return 0;
    }

    return (unsigned int)toc_metadata->sources->size;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the all afs_toc_metadata_source structures in the input afs_toc_metadata structure.
 *
 *  The function returns the sign of integrity the all afs_toc_metadata_source structures in the input afs_toc_metadata structure.
 *  If TOC metadata pointer is NULL or internal sources vector pointer is NULL or vector has no pointers then function return DFALSE.
 *
 *  \param[in]   toc_metadata   Pointer to the afs_toc_metadata structure.
 *  \return the sign of integrity of all afs_toc_metadata_source structures.
 */

DBOOL afs_toc_metadata_is_valid(afs_toc_metadata * toc_metadata)
{
    if (toc_metadata == NULL)
    {
        return DFALSE;
    }

    if (toc_metadata->sources == NULL)
    {
        return DFALSE;
    }

    if (toc_metadata->sources->size == 0)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_metadata->sources->size; i++)
    {
        if (afs_toc_metadata_source_is_valid(GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, i)) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_metadata structure to the XML file.
 *
 *  Function translates the input afs_toc_metadata structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_metadata  Pointer to the afs_toc_metadata structure.
 *  \param[in]   file_name     Name of the XML file.
 *  \param[in]   compact       If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_save_file(afs_toc_metadata * toc_metadata, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t *tree = mxmlNewXML("1.0");

    if (!afs_toc_metadata_save_xml(toc_metadata, tree))
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
 *  \brief Function translates the input afs_toc_metadata structure to the XML string.
 *
 *  Function translates the input afs_toc_metadata structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_metadata  Pointer to the afs_toc_metadata structure.
 *  \param[in]   compact       If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_metadata_save_string(afs_toc_metadata * toc_metadata, DBOOL compact)
{
    // If TOC metadata pointer is NULL return DFALSE
    if (toc_metadata == NULL)
    {
        return DFALSE;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_metadata_save_xml(toc_metadata, document))
    {
        mxmlDelete(document);
        return NULL;
    }

    char * xmlString;
    mxmlSetWrapMargin(0);

    struct mxml_node_s* metadata = mxmlFindElement( document, document, "metadata", NULL, NULL, MXML_DESCEND );

    if (compact)
    {
        xmlString = mxmlSaveAllocString(metadata, MXML_NO_CALLBACK);
    }
    else
    {
        xmlString = mxmlSaveAllocString(metadata, whitespace_cb);
    }

    mxmlDelete(document);

    return xmlString;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_metadata structure to the XML nodes.
 *
 *  Function translates the input afs_toc_metadata structure to the XML nodes.
 *  If input afs_toc_metadata structure is not valid then function return DFALSE without translation.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_metadata  Pointer to the afs_toc_metadata structure.
 *  \param[out]  out           Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_save_xml(afs_toc_metadata * toc_metadata, mxml_node_t* out)
{
    // If output node pointer is NULL or TOC metadata pointer is NULL return DFALSE
    if (out == NULL || toc_metadata == NULL)
    {
        return DFALSE;
    }

    if (!afs_toc_metadata_is_valid(toc_metadata))
    {
        return DFALSE;
    }

    mxml_node_t * metadata_node = mxmlNewElement(out, "metadata");

    for (unsigned int i = 0; i < toc_metadata->sources->size; i++)
    {
        DBOOL result = afs_toc_metadata_source_save_xml(GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, i), metadata_node);

        if (result == DFALSE)
        {
            return DFALSE;
        }
    }
    
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_metadata structure.
 *
 *  Function translates the input XML nodes to the afs_toc_metadata structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_metadata  Pointer to the afs_toc_metadata structure.
 *  \param[in]   file_name     Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_load_file(afs_toc_metadata * toc_metadata, const char * file_name)
{
    // If input file name string pointer is NULL or TOC metadata pointer is NULL return DFALSE
    if (file_name == NULL || toc_metadata == NULL)
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
    
    DBOOL return_value = afs_toc_metadata_load_xml(toc_metadata, document);
    
    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_metadata structure.
 *
 *  Function translates the input XML string to the afs_toc_metadata structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_metadata  Pointer to the afs_toc_metadata structure.
 *  \param[in]   in            Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_load_string(afs_toc_metadata * toc_metadata, const char * in)
{
    // If input string pointer is NULL or TOC metadata pointer is NULL return DFALSE
    if (in == NULL || strlen(in) == 0 || toc_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_metadata_load_xml(toc_metadata, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC metadata structure.
 *
 *  Function translates the input XML nodes to the TOC metadata structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_metadata  Pointer to the afs_toc_metadata structure.
 *  \param[in]   node          Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_metadata_load_xml(afs_toc_metadata * toc_metadata, mxml_node_t* node)
{
    // If input node pointer is NULL or TOC metadata pointer is NULL return DFALSE
    if (node == NULL || toc_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * metadata_node;

    if (strcmp(mxmlGetElement(node), "metadata") == 0)
    {
        metadata_node = node;
    }
    else
    {
        metadata_node = mxmlFindElement(node, node, "metadata", NULL, NULL, MXML_DESCEND);
    }

    if (metadata_node == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * source_node = mxmlFindElement(metadata_node, metadata_node, "source", NULL, NULL, MXML_DESCEND);

    while (source_node != NULL)
    {
        if (mxmlGetType(source_node) == MXML_ELEMENT)
        {
            afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();

            DBOOL load_toc_result = afs_toc_metadata_source_load_xml(toc_metadata_source, source_node);

            if (load_toc_result == DFALSE)
            {
                DLOG_INFO("Has no TOC metadata source.\n");
                afs_toc_metadata_source_free(toc_metadata_source);
                return DFALSE;
            }

            DBOOL add_toc_result = afs_toc_metadata_add_source(toc_metadata, toc_metadata_source);

            if (add_toc_result == DFALSE)
            {
                afs_toc_metadata_source_free(toc_metadata_source);
                return DFALSE;
            }
        }

        source_node = mxmlFindElement(source_node, metadata_node, "source", NULL, NULL, MXML_DESCEND);
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of metadata group
  */


// PRIVATE AFS TOC METADATA FUNCTIONS
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

    if (strcmp("metadata", name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (strcmp("source", name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (strcmp("source", parent_name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (strcmp("data", name) == 0 || strcmp("tags", name) == 0)
    {
        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n            ");
        }
    }

    return (NULL);
}
