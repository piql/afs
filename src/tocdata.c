/*****************************************************************************
**
**  Definition of the toc data interface
**
**  Creation date:  2018/05/11
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

#include "tocdata_c.h"
#include "xmlutils.h"
#include "platform/io.h"
#include <stdlib.h>

//  DEFINES
//

static const char * AFS_TOC_INDEX_TYPE_JOB = "job";
static const char * AFS_TOC_INDEX_TYPE_REEL = "reel";

// PRIVATE INTERFACE
//

static const char * whitespace_cb(mxml_node_t *node, int where);

//----------------------------------------------------------------------------
/*! \defgroup  toc  Table of Content.
 *  \brief     Index of content stored on reel.
 *  \ingroup   afs
 *
 *  The TOC is stored in XML format on the reel. This library contains classes
 *  and methods for loading and saveing TOC files in both XML and human readable 
 *  format. It also has functions that allows for TOC creation and manipulation.
 */


//----------------------------------------------------------------------------
/*! \defgroup  metadata  Metadata.
 *  \brief     File metadata.
 *  \ingroup   toc
 *
 *  It contains metadata information for the file. 
 */


//----------------------------------------------------------------------------
/*! \defgroup  preview  Preview.
 *  \brief     Files rendered as analogue images.
 *  \ingroup   toc
 *
 *  Previews are digital files (documents or images) rendered as analogue 
 *  images on the reel. Each digital file are rendered as one or more pages
 *  on the reel.
 *
 *  A frame is split up into one or more sections, where each section is a
 *  rectangular area of the frame. Sections are not allowed to overlap. The
 *  arrangment of sections in a frame is defined by afs_toc_preview_layout_definition_s
 *  which holds an array of afs_toc_preview_section_s sections.
 */


//----------------------------------------------------------------------------
/*! \defgroup  file  File.
 *  \brief     File TOC entry.
 *  \ingroup   toc
 *
 *  The file TOC entry points to the location of the file on the reel, both to
 *  the location of the digital data (if present) and/or the preview location 
 *  (if present).
 */


/*! 
  * \addtogroup toc
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_data_s  tocdata_c.h
 *  \brief      TOC data storage.
 *
 *  \param version                     Version string.
 *  \param index_type                  Index type string.
 *  \param job_id                      Job id string.
 *  \param reel_id                     Reel id string.
 *  \param reels                       Pointer to the afs_toc_data_reels structure.
 *  \param metadata                    Pointer to the afs_toc_metadata structure.
 *  \param preview_layout_definitions  Pointer to the afs_toc_preview_layout_definitions structure.
 *
 *  Structure for storing toc data.
 */


// PUBLIC AFS TOC DATA FUNCTIONS
//


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc data instance.
 *
 *  Allocate memory for the afs_toc_data type
 *  and initializes all structure pointers with NULL values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_data structure.
 */

afs_toc_data * afs_toc_data_create()
{
    afs_toc_data * toc_data = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_data);

    toc_data->version = NULL;
    toc_data->index_type = NULL;
    toc_data->job_id = NULL;
    toc_data->reel_id = NULL;
    toc_data->reels = NULL;
    toc_data->metadata = NULL;
    toc_data->preview_layout_definitions = NULL;

    return toc_data;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a toc data instance.
 *
 *  Allocate memory for the afs_toc_data type
 *  and initializes some structure pointers with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] version     Version string.
 *  \param[in] index_type  Index type string.
 *  \param[in] job_id      Job id string.
 *  \param[in] reel_id     Reel id string.
 *  \return instance of allocated afs_toc_data structure.
 */

afs_toc_data * afs_toc_data_create2(const char * version, const char * index_type, const char * job_id, const char * reel_id)
{
    afs_toc_data * toc_data = afs_toc_data_create();

    toc_data->version = boxing_string_clone(version);
    toc_data->index_type = boxing_string_clone(index_type);
    toc_data->job_id = boxing_string_clone(job_id);
    toc_data->reel_id = boxing_string_clone(reel_id);
    toc_data->reels = NULL;
    toc_data->metadata = NULL;
    toc_data->preview_layout_definitions = NULL;

    return toc_data;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_data structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_data  Pointer to the afs_toc_data structure.
 */

void afs_toc_data_free(afs_toc_data * toc_data)
{
    if (toc_data == NULL)
    {
        return;
    }

    boxing_string_free(toc_data->version);
    boxing_string_free(toc_data->index_type);
    boxing_string_free(toc_data->job_id);
    boxing_string_free(toc_data->reel_id);
    afs_toc_data_reels_free(toc_data->reels);
    afs_toc_metadata_free(toc_data->metadata);
    afs_toc_preview_layout_definitions_free(toc_data->preview_layout_definitions);
    boxing_memory_free(toc_data);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_data structure.
 *
 *  Function create a copy of input afs_toc_data structure and return it.
 *  If TOC data pointer is NULL function return NULL.
 *
 *  \param[in]  toc_data  Pointer to the afs_toc_data structure.
 *  \return new copy of afs_toc_data structure or NULL.
 */

afs_toc_data * afs_toc_data_clone(afs_toc_data * toc_data)
{
    // If TOC data pointer is NULL return NULL.
    if (toc_data == NULL)
    {
        return NULL;
    }

    afs_toc_data * return_toc_data = afs_toc_data_create2(toc_data->version, toc_data->index_type, toc_data->job_id, toc_data->reel_id);
    return_toc_data->reels = afs_toc_data_reels_clone(toc_data->reels);
    return_toc_data->metadata = afs_toc_metadata_clone(toc_data->metadata);
    return_toc_data->preview_layout_definitions = afs_toc_preview_layout_definitions_clone(toc_data->preview_layout_definitions);

    return return_toc_data;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_data structures on the identity.
 *
 *  Function checks two instances of the afs_toc_data structures on the identity.
 *
 *  \param[in]   toc_data1  Pointer to the first instance of the afs_toc_data structure.
 *  \param[in]   toc_data2  Pointer to the second instance of the afs_toc_data structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_data_equal(afs_toc_data * toc_data1, afs_toc_data * toc_data2)
{
    if (toc_data1 == NULL && toc_data2 == NULL)
    {
        return DTRUE;
    }

    if (toc_data1 == NULL || toc_data2 == NULL)
    {
        return DFALSE;
    }

    if (boxing_string_equal(toc_data1->version, toc_data2->version) &&
        boxing_string_equal(toc_data1->index_type, toc_data2->index_type) &&
        boxing_string_equal(toc_data1->job_id, toc_data2->job_id) &&
        boxing_string_equal(toc_data1->reel_id, toc_data2->reel_id) &&
        (afs_toc_data_reels_equal(toc_data1->reels, toc_data2->reels) || (afs_toc_data_reel_count(toc_data1) == 0 && afs_toc_data_reel_count(toc_data2) == 0)) &&
        (afs_toc_metadata_equal(toc_data1->metadata, toc_data2->metadata) || (afs_toc_data_metadata_source_count(toc_data1) == 0 && afs_toc_data_metadata_source_count(toc_data2) == 0)) &&
        (afs_toc_preview_layout_definitions_equal(toc_data1->preview_layout_definitions, toc_data2->preview_layout_definitions) || (afs_toc_data_preview_layout_definition_count(toc_data1) == 0 && afs_toc_data_preview_layout_definition_count(toc_data2) == 0)))
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_data_reel structure according to the input index.
 *
 *  The function returns an instance of afs_toc_data_reel structure according to the input index.
 *
 *  \param[in]   toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \param[in]   index     Index for specifying the required instance of the afs_toc_data_reel structure in the storage structure.
 *  \return an instance of afs_toc_data_reel structure or NULL.
 */

afs_toc_data_reel * afs_toc_data_get_reel_by_index(const afs_toc_data * toc_data, const unsigned int index)
{
    if (toc_data == NULL)
    {
        return NULL;
    }

    return afs_toc_data_reels_get_reel(toc_data->reels, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_data_reel structure according to the input id string.
 *
 *  The function returns an instance of afs_toc_data_reel structure according to the input id string.
 *
 *  \param[in]   toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \param[in]   index     Index for specifying the required instance of the afs_toc_data_reel structure in the storage structure.
 *  \return an instance of afs_toc_data_reel structure or NULL.
 */

afs_toc_data_reel * afs_toc_data_get_reel_by_id(const afs_toc_data * toc_data, const char * id)
{
    if (toc_data == NULL)
    {
        return NULL;
    }

    if (toc_data->reels == NULL)
    {
        return NULL;
    }

    unsigned int reels_count = afs_toc_data_reels_get_reels_count(toc_data->reels);

    if (reels_count == 0)
    {
        return NULL;
    }

    for (unsigned int i = 0; i < reels_count; i++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_reels_get_reel(toc_data->reels, i);

        if (boxing_string_equal(toc_data_reel->id, id) == DTRUE)
        {
            return toc_data_reel;
        }
    }
    return NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_metadata_source structure according to the input index.
 *
 *  The function returns an instance of afs_toc_metadata_source structure according to the input index.
 *
 *  \param[in]   toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \param[in]   index     Index for specifying the required instance of the afs_toc_metadata_source structure in the storage structure.
 *  \return an instance of afs_toc_metadata_source structure or NULL.
 */

afs_toc_metadata_source * afs_toc_data_get_metadata_source(const afs_toc_data * toc_data, const unsigned int index)
{
    if (toc_data == NULL)
    {
        return NULL;
    }

    return afs_toc_metadata_get_source(toc_data->metadata, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_file structure according to the input id.
 *
 *  The function returns an instance of afs_toc_file structure according to the input id.
 *
 *  \param[in]   toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \param[in]   id        Id for specifying the required instance of the afs_toc_file structure in the storage afs_toc_data structure.
 *  \return an instance of afs_toc_file structure or NULL.
 */

afs_toc_file * afs_toc_data_get_file_by_id(const afs_toc_data * toc_data, const int id)
{
    unsigned int reel_count = afs_toc_data_reel_count(toc_data);

    if (reel_count == 0)
    {
        return NULL;
    }

    for (unsigned int i = 0; i < reel_count; i++)
    {
        afs_toc_data_reel  * toc_data_reel = afs_toc_data_reels_get_reel(toc_data->reels, i);
        afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_id(toc_data_reel, id);

        if (toc_file != NULL)
        {
            return toc_file;
        }
    }

    return NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_file structure according to the input unique id.
 *
 *  The function returns an instance of afs_toc_file structure according to the input unique id.
 *
 *  \param[in]   toc_data   Pointer to the instance of the afs_toc_data structure.
 *  \param[in]   unique_id  Unique Id for specifying the required instance of the afs_toc_file structure in the storage afs_toc_data structure.
 *  \return an instance of afs_toc_file structure or NULL.
 */

afs_toc_file * afs_toc_data_get_file_by_unique_id(const afs_toc_data * toc_data, const char * unique_id)
{
    if (unique_id == NULL || boxing_string_length(unique_id) == 0)
    {
        return NULL;
    }

    unsigned int reel_count = afs_toc_data_reel_count(toc_data);

    if (reel_count == 0)
    {
        return NULL;
    }

    for (unsigned int i = 0; i < reel_count; i++)
    {
        afs_toc_data_reel  * toc_data_reel = afs_toc_data_reels_get_reel(toc_data->reels, i);
        afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, unique_id);

        if (toc_file != NULL)
        {
            return toc_file;
        }
    }

    return NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_preview_layout_definition structure according to the input index.
 *
 *  The function returns an instance of afs_toc_preview_layout_definition structure according to the input index.
 *
 *  \param[in]   toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \param[in]   index     Index for specifying the required instance of the afs_toc_preview_layout_definition structure in the storage structure.
 *  \return an instance of afs_toc_preview_layout_definition structure or NULL.
 */

afs_toc_preview_layout_definition * afs_toc_data_get_preview_layout_definition(const afs_toc_data * toc_data, const unsigned int index)
{
    if (toc_data == NULL)
    {
        return NULL;
    }

    return afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_data->preview_layout_definitions, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function sets a new version in the TOC data structure.
 *
 *  Function sets a new version in the TOC data structure.
 *
 *  \param[in] toc_data  Pointer to the afs_toc structure.
 *  \param[in] version   Version string.
 */

void afs_toc_data_set_version(afs_toc_data * toc_data, const char * version)
{
    // If TOC data pointer is NULL return
    if (toc_data == NULL)
    {
        return;
    }

    if (toc_data->version != NULL)
    {
        boxing_string_free(toc_data->version);
    }

    toc_data->version = boxing_string_clone(version);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function sets a new index type in the TOC data structure.
 *
 *  Function sets a new index type in the TOC data structure.
 *
 *  \param[in] toc_data  Pointer to the afs_toc structure.
 *  \param[in] version   Version string.
 */

void afs_toc_data_set_index_type(afs_toc_data * toc_data, const char * index_type)
{
    // If TOC data pointer is NULL return
    if (toc_data == NULL)
    {
        return;
    }

    if (toc_data->index_type != NULL)
    {
        boxing_string_free(toc_data->index_type);
    }

    toc_data->index_type = boxing_string_clone(index_type);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function sets a new job id in the TOC data structure.
 *
 *  Function sets a new job id in the TOC data structure.
 *
 *  \param[in] toc_data  Pointer to the afs_toc structure.
 *  \param[in] job_id    Job id string.
 */

void afs_toc_data_set_job_id(afs_toc_data * toc_data, const char * job_id)
{
    // If TOC data pointer is NULL return
    if (toc_data == NULL)
    {
        return;
    }

    if (toc_data->job_id != NULL)
    {
        boxing_string_free(toc_data->job_id);
    }

    toc_data->job_id = boxing_string_clone(job_id);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function sets a new reel id in the TOC data structure.
 *
 *  Function sets a new reel id in the TOC data structure.
 *
 *  \param[in] toc_data  Pointer to the afs_toc structure.
 *  \param[in] reel_id    Reel id string.
 */

void afs_toc_data_set_reel_id(afs_toc_data * toc_data, const char * reel_id)
{
    // If TOC data pointer is NULL return
    if (toc_data == NULL)
    {
        return;
    }

    if (toc_data->reel_id != NULL)
    {
        boxing_string_free(toc_data->reel_id);
    }

    toc_data->reel_id = boxing_string_clone(reel_id);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_data_reel structure to the input afs_toc_data structure.
 *
 *  The function adds a new instance of afs_toc_data_reel structure to the input afs_toc_data structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_data_reel structure is not add and the function returns DFALSE.
 *  If input afs_toc_data_reel instance is not valid then new instance is not add the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_data       Pointer to the instance of the afs_toc_data structure.
 *  \param[in]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_add_reel(afs_toc_data * toc_data, afs_toc_data_reel * toc_data_reel)
{
    if (toc_data == NULL || toc_data_reel == NULL)
    {
        return DFALSE;
    }

    if (toc_data->reels == NULL)
    {
        toc_data->reels = afs_toc_data_reels_create();
    }

    return afs_toc_data_reels_add_reel(toc_data->reels, toc_data_reel);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_metadata_source structure to the input afs_toc_data structure.
 *
 *  The function adds a new instance of afs_toc_metadata_source structure to the input afs_toc_data structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_metadata_source structure is not add and the function returns DFALSE.
 *  If input afs_toc_metadata_source instance is not valid then new instance is not add the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_data             Pointer to the instance of the afs_toc_data structure.
 *  \param[in]   toc_metadata_source  Pointer to the instance of the afs_toc_metadata_source structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_add_metadata_source(afs_toc_data * toc_data, afs_toc_metadata_source * toc_metadata_source)
{
    if (toc_data == NULL || toc_metadata_source == NULL)
    {
        return DFALSE;
    }

    if (toc_data->metadata == NULL)
    {
        toc_data->metadata = afs_toc_metadata_create();
    }

    return afs_toc_metadata_add_source(toc_data->metadata, toc_metadata_source);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_preview_layout_definition structure to the input afs_toc_data structure.
 *
 *  The function adds a new instance of afs_toc_preview_layout_definition structure to the input afs_toc_data structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_preview_layout_definition structure is not add and the function returns DFALSE.
 *  If input afs_toc_preview_layout_definition instance is not valid or new layout definition id already present then new instance is not add, the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_data                       Pointer to the instance of the afs_toc_data structure.
 *  \param[in]   toc_preview_layout_definition  Pointer to the instance of the afs_toc_preview_layout_definition structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_add_preview_layout_definition(afs_toc_data * toc_data, afs_toc_preview_layout_definition * toc_preview_layout_definition)
{
    if (toc_data == NULL || toc_preview_layout_definition == NULL)
    {
        return DFALSE;
    }

    if (toc_data->preview_layout_definitions == NULL)
    {
        toc_data->preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    }

    return afs_toc_preview_layout_definitions_add_layout_definition(toc_data->preview_layout_definitions, toc_preview_layout_definition);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instances of afs_toc_preview_layout_definition structure from afs_toc_preview_layout_definitions structure to the afs_toc_data structure.
 *
 *  The function adds a new instances of afs_toc_preview_layout_definition structure from afs_toc_preview_layout_definitions structure to the afs_toc_data structure.
 *  If one of the input pointer is NULL the new instances of afs_toc_preview_layout_definition structure is not add and the function returns DFALSE.
 *  If one of the afs_toc_preview_layout_definition instances is not valid or new layout definition id already present then new instance is not add, the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_data                        Pointer to the instance of the afs_toc_data structure.
 *  \param[in]   toc_preview_layout_definitions  Pointer to the instance of the afs_toc_preview_layout_definitions structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_add_preview_layout_definitions(afs_toc_data * toc_data, const afs_toc_preview_layout_definitions * toc_preview_layout_definitions)
{
    if (toc_data == NULL || toc_preview_layout_definitions == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_preview_layout_definitions_is_valid(toc_preview_layout_definitions) == DFALSE)
    {
        return DFALSE;
    }

    unsigned int definitions_count = afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions);

    if (definitions_count == 0)
    {
        return DFALSE;
    }

    if (toc_data->preview_layout_definitions == NULL)
    {
        toc_data->preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    }

    for (unsigned int i = 0; i < definitions_count; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_clone(afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, i));
        
        if (afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition) == DFALSE)
        {
            afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_data_reel structure.
 *
 *  The function returns a count of instances of afs_toc_data_reel structure.
 *  If input afs_toc_data pointer is NULL or internal afs_toc_data_reels structure pointer is NULL then function returns zero.
 *
 *  \param[in]  toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \return a count of instances of afs_toc_data_reel structure or zero.
 */

unsigned int afs_toc_data_reel_count(const afs_toc_data * toc_data)
{
    if (toc_data == NULL)
    {
        return 0;
    }

    return afs_toc_data_reels_get_reels_count(toc_data->reels);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_metadata_source structure.
 *
 *  The function returns a count of instances of afs_toc_metadata_source structure.
 *  If input afs_toc_data pointer is NULL then function returns zero.
 *
 *  \param[in]  toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \return a count of instances of afs_toc_metadata_source structure or zero.
 */

unsigned int afs_toc_data_metadata_source_count(const afs_toc_data * toc_data)
{
    if (toc_data == NULL)
    {
        return 0;
    }

    return afs_toc_metadata_get_sources_count(toc_data->metadata);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_preview_layout_definition structure.
 *
 *  The function returns a count of instances of afs_toc_preview_layout_definition structure.
 *  If input afs_toc_data pointer is NULL then function returns zero.
 *
 *  \param[in]  toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \return a count of instances of afs_toc_preview_layout_definition structure or zero.
 */

unsigned int afs_toc_data_preview_layout_definition_count(const afs_toc_data * toc_data)
{
    if (toc_data == NULL)
    {
        return 0;
    }

    return afs_toc_preview_layout_definitions_get_count(toc_data->preview_layout_definitions);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_file structure.
 *
 *  The function returns a count of instances of afs_toc_file structure.
 *  If input afs_toc_data pointer is NULL or internal afs_toc_data_reels pointer is NULL then function returns zero.
 *
 *  \param[in]  toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \return a count of instances of afs_toc_file structure or zero.
 */

unsigned int afs_toc_data_file_count(const afs_toc_data * toc_data)
{
    if (toc_data == NULL)
    {
        return 0;
    }

    if (toc_data->reels == NULL)
    {
        return 0;
    }

    unsigned int file_count = 0;
    for (unsigned int i = 0; i < afs_toc_data_reels_get_reels_count(toc_data->reels); i++)
    {
        afs_toc_data_reel  * toc_data_reel = afs_toc_data_reels_get_reel(toc_data->reels, i);
        file_count += afs_toc_data_reel_file_count(toc_data_reel);
    }
    return file_count;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of frames in the input afs_toc_data structure instance.
 *
 *  The function returns a count of frames in the input afs_toc_data structure instance.
 *  If input afs_toc_data pointer is NULL or internal afs_toc_data_reels structure pointer is NULL then function returns zero.
 *
 *  \param[in]  toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \return a count of frames or zero.
 */

unsigned int afs_toc_data_frame_count(const afs_toc_data * toc_data)
{
    if (toc_data == NULL)
    {
        return 0;
    }

    if (toc_data->reels == NULL)
    {
        return 0;
    }

    unsigned int frames_count = 0;
    for (unsigned int i = 0; i < afs_toc_data_reels_get_reels_count(toc_data->reels); i++)
    {
        afs_toc_data_reel  * toc_data_reel = afs_toc_data_reels_get_reel(toc_data->reels, i);
        frames_count += afs_toc_data_reel_frame_count(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);
    }
    return frames_count;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a number of first frame.
 *
 *  The function returns a number of first frame.
 *  If input afs_toc_data is NULL or internal afs_toc_data_reels structure pointer is NULL then function returns 0.
 *
 *  \param[in]  toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \return a number of first frame or 0.
 */

int afs_toc_data_first_frame(const afs_toc_data * toc_data)
{
    if (toc_data == NULL)
    {
        return -1;
    }

    if (toc_data->reels == NULL)
    {
        return -1;
    }

    if (afs_toc_data_reels_get_reels_count(toc_data->reels) == 0)
    {
        return 0;
    }

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reels_get_reel(toc_data->reels, 0);
    int first_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    return first_frame;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a number of last frame.
 *
 *  The function returns a number of last frame.
 *  If input afs_toc_data is NULL or internal afs_toc_data_reels structure pointer is NULL then function returns 0.
 *
 *  \param[in]  toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \return a number of last frame or 0.
 */

int afs_toc_data_last_frame(const afs_toc_data * toc_data)
{
    if (toc_data == NULL)
    {
        return -1;
    }

    if (toc_data->reels == NULL)
    {
        return -1;
    }

    unsigned int reels_count = afs_toc_data_reels_get_reels_count(toc_data->reels);
    if (reels_count == 0)
    {
        return 0;
    }

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reels_get_reel(toc_data->reels, reels_count - 1);

    return afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the afs_toc_data structure.
 *
 *  The function returns the sign of integrity the current structure.
 *  If TOC data pointer is NULL then function return DFALSE.
 *
 *  \param[in]  toc_data  Pointer to the afs_toc_data structure.
 *  \return the sign of integrity the afs_toc_data structure.
 */

DBOOL afs_toc_data_is_valid(const afs_toc_data * toc_data)
{
    // If TOC data pointer is NULL return DFALSE
    if (toc_data == NULL)
    {
        return DFALSE;
    }

    if (toc_data->version == NULL || boxing_string_length(toc_data->version) == 0)
    {
        return DFALSE;
    }

    if (toc_data->job_id == NULL || boxing_string_length(toc_data->job_id) == 0)
    {
        return DFALSE;
    }

    if (toc_data->reel_id == NULL || boxing_string_length(toc_data->reel_id) == 0)
    {
        return DFALSE;
    }

    if (boxing_string_equal(toc_data->index_type, AFS_TOC_INDEX_TYPE_JOB) == DFALSE && 
        boxing_string_equal(toc_data->index_type, AFS_TOC_INDEX_TYPE_REEL) == DFALSE)
    {
        return DFALSE;
    }

    if (afs_toc_data_reels_is_valid(toc_data->reels) == DFALSE && toc_data->reels != NULL)
    {
        return DFALSE;
    }

    if (afs_toc_preview_layout_definitions_is_valid(toc_data->preview_layout_definitions) == DFALSE && toc_data->preview_layout_definitions != NULL)
    {
        return DFALSE;
    }

    if (afs_toc_metadata_get_sources_count(toc_data->metadata) != 0)
    {
        if (afs_toc_metadata_is_valid(toc_data->metadata) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds the offset of the start and end frame to all afs_toc_file instances.
 *
 *  The function adds the offset of the start and end frame to all afs_toc_file instances.
 *  If input afs_toc_data pointer is NULL or internal afs_toc_data_reels structure pointer is NULL or
 *  afs_toc_files structure has no afs_toc_data_reel instances then function returns without adding offset.
 *
 *  \param[in,out]   toc_data  Pointer to the instance of the afs_toc_data structure.
 *  \param[in]       offset         The offset that we need to add.
 *  \param[in]       section        Section type.
 */

void afs_toc_data_add_frame_offset(afs_toc_data * toc_data, unsigned int offset, const unsigned int section)
{
    if (toc_data == NULL)
    {
        return;
    }

    if (toc_data->reels == NULL)
    {
        return;
    }

    unsigned int reels_count = afs_toc_data_reels_get_reels_count(toc_data->reels);
    if (reels_count == 0)
    {
        return;
    }

    for (unsigned int i = 0; i < reels_count; i++)
    {
        afs_toc_data_reel  * toc_data_reel = afs_toc_data_reels_get_reel(toc_data->reels, i);
        afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, section);
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_data structure to the XML file.
 *
 *  Function translates the input afs_toc_data structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_data   Pointer to the afs_toc_data structure.
 *  \param[in]   file_name  Name of the XML file.
 *  \param[in]   compact    If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_save_file(afs_toc_data * toc_data, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_data == NULL || afs_toc_data_is_valid(toc_data) == DFALSE)
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

    if (!afs_toc_data_save_xml(toc_data, tree))
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
 *  \brief Function translates the input afs_toc_data structure to the XML string.
 *
 *  Function translates the input afs_toc_data structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_data  Pointer to the afs_toc_data structure.
 *  \param[in]   compact   If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_data_save_string(afs_toc_data * toc_data, DBOOL compact)
{
    // If TOC data reel pointer is NULL return DFALSE
    if (toc_data == NULL)
    {
        return NULL;
    }

    struct mxml_node_s * document = mxmlNewXML("1.0");

    if (!afs_toc_data_save_xml(toc_data, document))
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
 *  \brief Function translates the input afs_toc_data structure to the XML nodes.
 *
 *  Function translates the input afs_toc_data structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_data  Pointer to the afs_toc_data structure.
 *  \param[out]  out       Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_save_xml(afs_toc_data * toc_data, mxml_node_t* out)
{
    // If output node pointer is NULL or TOC data reel pointer is NULL return DFALSE
    if (out == NULL || toc_data == NULL)
    {
        return DFALSE;
    }

    if (!afs_toc_data_is_valid(toc_data))
    {
        return DFALSE;
    }

    mxml_node_t * index_node = mxmlNewElement(out, "Index");

    if (toc_data->version != NULL)
    {
        mxmlElementSetAttr(index_node, "version", toc_data->version);
    }

    afs_xmlutils_add_new_text_node(index_node, "indexType", toc_data->index_type);
    afs_xmlutils_add_new_text_node(index_node, "jobId", toc_data->job_id);
    afs_xmlutils_add_new_text_node(index_node, "reelId", toc_data->reel_id);

    if (afs_toc_metadata_get_sources_count(toc_data->metadata) != 0)
    {
        if (afs_toc_metadata_save_xml(toc_data->metadata, index_node) == DFALSE)
        {
            return DFALSE;
        }
    }

    if (afs_toc_preview_layout_definitions_get_count(toc_data->preview_layout_definitions) != 0)
    {
        if (afs_toc_preview_layout_definitions_save_xml(toc_data->preview_layout_definitions, index_node) == DFALSE)
        {
            return DFALSE;
        }
    }

    if (afs_toc_data_reels_get_reels_count(toc_data->reels) != 0)
    {
        if (afs_toc_data_reels_save_xml(toc_data->reels, index_node) == DFALSE)
        {
            return DFALSE;
        }
    }
    
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_data structure.
 *
 *  Function translates the input XML nodes to the afs_toc_data structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data   Pointer to the afs_toc_data structure.
 *  \param[in]   file_name  Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_load_file(afs_toc_data * toc_data, const char * file_name)
{
    // If input file name string pointer is NULL or TOC data pointer is NULL return DFALSE
    if (file_name == NULL || toc_data == NULL)
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

    DBOOL return_value = afs_toc_data_load_xml(toc_data, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_data structure.
 *
 *  Function translates the input XML string to the afs_toc_data structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data  Pointer to the afs_toc_data structure.
 *  \param[in]   in        Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_load_string(afs_toc_data * toc_data, const char * in)
{
    // If input string pointer is NULL or TOC data pointer is NULL return DFALSE
    if (in == NULL || boxing_string_equal(in, "") || toc_data == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_data_load_xml(toc_data, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC data structure.
 *
 *  Function translates the input XML nodes to the TOC data structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data  Pointer to the afs_toc_data structure.
 *  \param[in]   node      Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_load_xml(afs_toc_data * toc_data, mxml_node_t * node)
{
    // If input node pointer is NULL or TOC data pointer is NULL return DFALSE
    if (node == NULL || toc_data == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * index_node = NULL;

    index_node = boxing_string_equal(mxmlGetElement(node), "Index") == DTRUE ? node : mxmlFindElement(node, node, "Index", NULL, NULL, MXML_DESCEND);

    if (index_node == NULL)
    {
        return DFALSE;
    }

    if (toc_data->version != NULL)
    {
        boxing_string_free(toc_data->version);
    }

    toc_data->version = boxing_string_clone(mxmlElementGetAttr(index_node, "version"));

    mxml_node_t * index_type_node = mxmlFindElement(index_node, index_node, "indexType", NULL, NULL, MXML_DESCEND);
    if (index_type_node != NULL)
    {
        if (toc_data->index_type != NULL)
        {
            boxing_string_free(toc_data->index_type);
        }

        toc_data->index_type = afs_xmlutils_get_node_text(index_type_node);
    }

    mxml_node_t * job_id_node = mxmlFindElement(index_node, index_node, "jobId", NULL, NULL, MXML_DESCEND);
    if (job_id_node != NULL)
    {
        if (toc_data->job_id != NULL)
        {
            boxing_string_free(toc_data->job_id);
        }

        toc_data->job_id = afs_xmlutils_get_node_text(job_id_node);
    }

    mxml_node_t * reel_id_node = mxmlFindElement(index_node, index_node, "reelId", NULL, NULL, MXML_DESCEND);
    if (reel_id_node != NULL)
    {
        if (toc_data->reel_id != NULL)
        {
            boxing_string_free(toc_data->reel_id);
        }

        toc_data->reel_id = afs_xmlutils_get_node_text(reel_id_node);
    }

    DBOOL metadata_loaded = DFALSE;

    mxml_node_t * metadata_node = mxmlFindElement(index_node, index_node, "metadata", NULL, NULL, MXML_DESCEND);

    while (metadata_node != NULL)
    {
        if (boxing_string_equal(metadata_node->parent->value.element.name, "Index") == DTRUE)
        {
            afs_toc_metadata * load_toc_metadata = afs_toc_metadata_create();
            
            DBOOL load_metadata_result = afs_toc_metadata_load_xml(load_toc_metadata, metadata_node);

            if (load_metadata_result == DFALSE)
            {
                afs_toc_metadata_free(load_toc_metadata);
                return DFALSE;
            }

            afs_toc_metadata_free(toc_data->metadata);
            toc_data->metadata = load_toc_metadata;
            metadata_loaded = DTRUE;

            break;
        }

        metadata_node = mxmlFindElement(metadata_node, index_node, "metadata", NULL, NULL, MXML_DESCEND);
    }

    DBOOL definitions_loaded = DFALSE;

    mxml_node_t * layout_definitions_node = mxmlFindElement(index_node, index_node, "layoutDefinitions", NULL, NULL, MXML_DESCEND);

    if (layout_definitions_node != NULL)
    {
        afs_toc_preview_layout_definitions * load_toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

        DBOOL load_layout_definitions_result = afs_toc_preview_layout_definitions_load_xml(load_toc_preview_layout_definitions, layout_definitions_node);

        if (load_layout_definitions_result == DFALSE)
        {
            afs_toc_preview_layout_definitions_free(load_toc_preview_layout_definitions);
            return DFALSE;
        }

        afs_toc_preview_layout_definitions_free(toc_data->preview_layout_definitions);
        toc_data->preview_layout_definitions = load_toc_preview_layout_definitions;
        definitions_loaded = DTRUE;
    }

    DBOOL reels_loaded = DFALSE;

    mxml_node_t * reels_node = mxmlFindElement(index_node, index_node, "reels", NULL, NULL, MXML_DESCEND);

    if (reels_node != NULL)
    {
        afs_toc_data_reels * load_toc_data_reels = afs_toc_data_reels_create();

        DBOOL load_reels_result = afs_toc_data_reels_load_xml(load_toc_data_reels, reels_node);

        if (load_reels_result == DFALSE)
        {
            afs_toc_data_reels_free(load_toc_data_reels);
            return DFALSE;
        }

        afs_toc_data_reels_free(toc_data->reels);
        toc_data->reels = load_toc_data_reels;
        reels_loaded = DTRUE;
    }

    if (metadata_loaded == DFALSE)
    {
        afs_toc_metadata_free(toc_data->metadata);
        toc_data->metadata = NULL;
    }

    if (definitions_loaded == DFALSE)
    {
        afs_toc_preview_layout_definitions_free(toc_data->preview_layout_definitions);
        toc_data->preview_layout_definitions = NULL;
    }

    if (reels_loaded == DFALSE)
    {
        afs_toc_data_reels_free(toc_data->reels);
        toc_data->reels = NULL;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of toc group
  */


// PRIVATE AFS TOC DATA FUNCTIONS
//


static const char * whitespace_cb(mxml_node_t *node, int where)
{
    const char *name, *parent_name, *parent_parent_name, *parent_parent_parent_name;

    /*
    * We can conditionally break to a new line
    * before or after any element. These are
    * just common HTML elements...
    */

    name = mxmlGetElement(node);
    parent_name = mxmlGetElement(node->parent);
    parent_parent_name = (node->parent != NULL) ? mxmlGetElement(node->parent->parent) : NULL;
    parent_parent_parent_name = (parent_parent_name != NULL) ? mxmlGetElement(node->parent->parent->parent) : NULL;

    if (boxing_string_equal("Index", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("Index", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("indexType", name) || boxing_string_equal("jobId", name) || boxing_string_equal("reelId", name))
    {
        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n        ");
        }
    }

    // METADATA formatting

    if (boxing_string_equal("metadata", name) && boxing_string_equal("Index", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("source", name) && boxing_string_equal("metadata", parent_name) && boxing_string_equal("Index", parent_parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("source", parent_name) && boxing_string_equal("metadata", parent_parent_name) && boxing_string_equal("Index", parent_parent_parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }
    }

    if ((boxing_string_equal("data", name) || boxing_string_equal("tags", name)) && boxing_string_equal("metadata", parent_parent_name) && boxing_string_equal("Index", parent_parent_parent_name))
    {
        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n                ");
        }
    }

    // LAYOUT DEFINITIONS formatting

    if (boxing_string_equal("layoutDefinitions", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("previewLayout", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if ((boxing_string_equal("id", name) || boxing_string_equal("name", name) || boxing_string_equal("sections", name)) && boxing_string_equal("previewLayout", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }
    }

    if (boxing_string_equal("sections", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                ");
        }
    }

    if ((boxing_string_equal("id", name) || boxing_string_equal("name", name)) && boxing_string_equal("previewLayout", parent_name))
    {
        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n                ");
        }
    }

    // REELS formatting

    if (boxing_string_equal("reel", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("reel", parent_name) && boxing_string_equal("id", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }

        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n                ");
        }
    }

    if (boxing_string_equal("files", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }
    }

    if (boxing_string_equal("file", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                ");
        }
    }

    if (boxing_string_equal("file", parent_name) && !boxing_string_equal("data", name) && !boxing_string_equal("preview", name) && !boxing_string_equal("metadata", name))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n                    ");
        }

        if (where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                    ");
        }

        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n                        ");
        }
    }

    if (boxing_string_equal("preview", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                    ");
        }
    }

    if (boxing_string_equal("metadata", name) && boxing_string_equal("file", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                    ");
        }
    }

    if (boxing_string_equal("source", name) && boxing_string_equal("metadata", parent_name) && boxing_string_equal("file", parent_parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                        ");
        }
    }

    if (boxing_string_equal("data", name) && boxing_string_equal("source", parent_name) && boxing_string_equal("metadata", parent_parent_name) && boxing_string_equal("file", parent_parent_parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                            ");
        }

        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n                                ");
        }
    }

    if (boxing_string_equal("preview", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n                        ");
        }
    }

    return (NULL);
}
