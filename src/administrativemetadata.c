/*****************************************************************************
**
**  Definition of the Administrative Metadata interface
**
**  Creation date:  2016/12/28
**  Created by:     Piql AS
**
**
**  Copyright (c) 2016 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "administrativemetadata.h"
#include "boxing/platform/memory.h"
#include "boxing/string.h"
#include "xmlutils.h"

//  DEFINES
//
#define AFS_ADMINISTRATIVE_METADATA_TAG_COUNT 6

// PRIVATE INTERFACE
//
typedef struct afs_administrative_metadata_tag_s
{
    const char* tag;
    char** content;
} afs_administrative_metadata_tag;

static afs_administrative_metadata_tag* get_tags_list(afs_administrative_metadata* control_data);


/*! 
  * \addtogroup cf
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_administrative_metadata_s  controldata.h
 *  \brief      Administrative metadata storage.
 *
 *  \param reel_id        Reel id string.
 *  \param print_reel_id  Print reel id string.
 *  \param title          Title string.
 *  \param description    Description string.
 *  \param creator        Creator string.
 *  \param creation_date  Creation date string.
 *
 *  Structure for storing administrative metadata.
 */


// PUBLIC AFS ADMINISTRATIVE METADATA FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create an administrative metadata instance.
 *
 *  Allocate memory for the afs_administrative_metadata type
 *  and initializes all structure pointers with NULL values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_administrative_metadata structure.
 */

afs_administrative_metadata* afs_administrative_metadata_create()
{
    afs_administrative_metadata* administrative_metadata = BOXING_MEMORY_ALLOCATE_TYPE(afs_administrative_metadata);
    afs_administrative_metadata_init(administrative_metadata);
    return administrative_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers with NULL value.
 *
 *  Initializes all input structure pointers with NULL values.
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  administrative_metadata  Pointer to the afs_administrative_metadata structure.
 */

void afs_administrative_metadata_init(afs_administrative_metadata* administrative_metadata)
{
    if (administrative_metadata == NULL)
    {
        return;
    }

    administrative_metadata->reel_id = NULL;
    administrative_metadata->print_reel_id = NULL;
    administrative_metadata->title = NULL;
    administrative_metadata->description = NULL;
    administrative_metadata->creator = NULL;
    administrative_metadata->creation_date = NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers.
 *
 *  Initializes all input structure pointers with specified values.
 *  If one of the input pointers is NULL, then return without initialization.
 *
 *  \param[in]  administrative_metadata  Pointer to the afs_administrative_metadata structure.
 *  \param[in]  reel_id                  Pointer to the reel_id string.
 *  \param[in]  print_reel_id            Pointer to the print_reel_id string.
 *  \param[in]  title                    Pointer to the title string.
 *  \param[in]  description              Pointer to the description string.
 *  \param[in]  creator                  Pointer to the creator string.
 *  \param[in]  creation_date            Pointer to the creation_date string.
 */

void afs_administrative_metadata_init2(
    afs_administrative_metadata* administrative_metadata,
    const char* reel_id,
    const char* print_reel_id,
    const char* title,
    const char* description,
    const char* creator,
    const char* creation_date)
{
    if (administrative_metadata == NULL || reel_id == NULL || print_reel_id == NULL || title == NULL || description == NULL || creator == NULL || creation_date == NULL)
    {
        return;
    }

    administrative_metadata->reel_id = boxing_string_clone(reel_id);
    administrative_metadata->print_reel_id = boxing_string_clone(print_reel_id);
    administrative_metadata->title = boxing_string_clone(title);
    administrative_metadata->description = boxing_string_clone(description);
    administrative_metadata->creator = boxing_string_clone(creator);
    administrative_metadata->creation_date = boxing_string_clone(creation_date);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of administrative metadata structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  administrative_metadata  Pointer to the afs_administrative_metadata structure.
 */

void afs_administrative_metadata_free(afs_administrative_metadata* administrative_metadata)
{
    if (administrative_metadata == NULL)
    {
        return;
    }

    boxing_memory_free(administrative_metadata->reel_id);
    boxing_memory_free(administrative_metadata->print_reel_id);
    boxing_memory_free(administrative_metadata->title);
    boxing_memory_free(administrative_metadata->description);
    boxing_memory_free(administrative_metadata->creator);
    boxing_memory_free(administrative_metadata->creation_date);
    boxing_memory_free(administrative_metadata);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_administrative_metadata structure to the XML nodes.
 *
 *  Function translates the input afs_administrative_metadata structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  out                      Pointer to the XML node.
 *  \param[in]   administrative_metadata  Pointer to the afs_administrative_metadata structure.
 *  \return DTRUE on success.
 */

DBOOL afs_administrative_metadata_save_xml(mxml_node_t * out, afs_administrative_metadata* administrative_metadata)
{
    // If output node pointer is NULL or administrative metadata pointer is NULL return DFALSE
    if (out == NULL || administrative_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t *administrative_metadata_node = mxmlNewElement(out, "AdministrativeMetadata");

    afs_administrative_metadata_tag* tags = get_tags_list(administrative_metadata);

    for (unsigned int i = 0; i < AFS_ADMINISTRATIVE_METADATA_TAG_COUNT; i++)
    {
        mxml_node_t *current_tag = mxmlNewElement(administrative_metadata_node, tags[i].tag);
        afs_xmlutils_set_node_text(*tags[i].content, current_tag);
    }

    boxing_memory_free(tags);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_administrative_metadata structure.
 *
 *  Function translates the input XML nodes to the afs_administrative_metadata structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  administrative_metadata  Pointer to the afs_administrative_metadata structure.
 *  \param[in]   in                       Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_administrative_metadata_load_xml(afs_administrative_metadata* administrative_metadata, mxml_node_t * in)
{
    // If input data pointer is NULL or administrative metadata pointer is NULL return DFALSE
    if (in == NULL || administrative_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * administrative_metadata_node = mxmlFindElement(in, in, "AdministrativeMetadata", NULL, NULL, MXML_DESCEND);

    if (administrative_metadata_node == NULL)
    {
        return DFALSE;
    }

    afs_administrative_metadata_tag* tags = get_tags_list(administrative_metadata);

    for (unsigned int i = 0; i < AFS_ADMINISTRATIVE_METADATA_TAG_COUNT; i++)
    {
        mxml_node_t * current_tag_node = mxmlFindElement(administrative_metadata_node, administrative_metadata_node, tags[i].tag, NULL, NULL, MXML_DESCEND);

        if (current_tag_node == NULL)
        {
            boxing_memory_free(tags);
            return DFALSE;
        }

        *tags[i].content = afs_xmlutils_get_node_text(current_tag_node);
    }

    boxing_memory_free(tags);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of cf group
  */


// PRIVATE AFS ADMINISTRATIVE METADATA FUNCTIONS
//

static afs_administrative_metadata_tag* get_tags_list(afs_administrative_metadata* control_data)
{
    afs_administrative_metadata_tag* tags = BOXING_MEMORY_ALLOCATE_TYPE_ARRAY(afs_administrative_metadata_tag, AFS_ADMINISTRATIVE_METADATA_TAG_COUNT);
    tags[0].tag = "ReelId";
    tags[0].content = &control_data->reel_id;

    tags[1].tag = "PrintReelId";
    tags[1].content = &control_data->print_reel_id;

    tags[2].tag = "Title";
    tags[2].content = &control_data->title;

    tags[3].tag = "Description";
    tags[3].content = &control_data->description;

    tags[4].tag = "Creator";
    tags[4].content = &control_data->creator;

    tags[5].tag = "CreationDate";
    tags[5].content = &control_data->creation_date;

    return tags;
}
