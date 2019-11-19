/*****************************************************************************
**
**  Definition of the toc data reel interface
**
**  Creation date:  2018/05/07
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

#include "tocdatareel_c.h"
#include "xmlutils.h"
#include "platform/io.h"
#include <stdlib.h>

//  DEFINES
//

// PRIVATE INTERFACE
//

static void merge(afs_frame_ranges * ranges);
static const char * whitespace_cb(mxml_node_t *node, int where);

/*! 
  * \addtogroup file
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \enum   afs_toc_data_reel_section tocdatareel_c.h
 *  \brief  The enumeration for specifying the section type.
 *  
 *  \param AFS_TOC_DATA_REEL_SECTION_DIGITAL                   (0x1) Digital section type.
 *  \param AFS_TOC_DATA_REEL_SECTION_PREVIEW                   (0x2) Preview section type.
 *  \param AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS  (0x4) File format descriptions section type.
 *  \param AFS_TOC_DATA_REEL_SECTION_ALL                       Any of the above section types.
 *
 *  The enumeration for specifying the section type.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_data_reel_s  tocdatareel_c.h
 *  \brief      TOC data reel data storage.
 *
 *  \param id     Reel Id string.
 *  \param files  Pointer to toc_data_files structure instance.
 *
 *  Structure for storing toc data reel data.
 */


// PUBLIC AFS TOC DATA REEL FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create a toc data reel instance.
 *
 *  Allocate memory for the afs_toc_data_reel type
 *  and initializes all structure pointers with NULL values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_data_reel structure.
 */

afs_toc_data_reel * afs_toc_data_reel_create()
{
    afs_toc_data_reel * toc_data_reel = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_data_reel);
    toc_data_reel->id = NULL;
    toc_data_reel->files = NULL;
    return toc_data_reel;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a toc data reel instance.
 *
 *  Allocate memory for the afs_toc_data_reel type
 *  and initialize id string with specified input value, initialize files pointer with NULL value.
 *  Return instance of allocated structure.
 *
 *  \param[in] id  Reel Id string.
 *  \return instance of allocated afs_toc_data_reel structure.
 */

afs_toc_data_reel * afs_toc_data_reel_create2(const char * id)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    toc_data_reel->id = boxing_string_clone(id);
    toc_data_reel->files = NULL;
    return toc_data_reel;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_data_reel structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_data_reel  Pointer to the afs_toc_data_reel structure.
 */

void afs_toc_data_reel_free(afs_toc_data_reel * toc_data_reel)
{
    if (toc_data_reel == NULL)
    {
        return;
    }

    boxing_string_free(toc_data_reel->id);
    afs_toc_files_free(toc_data_reel->files);
    boxing_memory_free(toc_data_reel);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_data_reel structure.
 *
 *  Function create a copy of input afs_toc_data_reel structure and return it.
 *  If TOC data reel structure pointer is NULL function return NULL.
 *
 *  \param[in]  toc_data_reel  Pointer to the afs_toc_data_reel structure.
 *  \return new copy of afs_toc_data_reel structure or NULL.
 */

afs_toc_data_reel * afs_toc_data_reel_clone(const afs_toc_data_reel * toc_data_reel)
{
    // If TOC data reel pointer is NULL return NULL.
    if (toc_data_reel == NULL)
    {
        return NULL;
    }

    afs_toc_data_reel * return_toc_data_reel = afs_toc_data_reel_create();
    return_toc_data_reel->id = boxing_string_clone(toc_data_reel->id);
    return_toc_data_reel->files = afs_toc_files_clone(toc_data_reel->files);

    return return_toc_data_reel;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_data_reel structures on the identity.
 *
 *  Function checks two instances of the afs_toc_data_reel structures on the identity.
 *
 *  \param[in]   toc_data_reel1  Pointer to the first instance of the afs_toc_data_reel structure.
 *  \param[in]   toc_data_reel2  Pointer to the second instance of the afs_toc_data_reel structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_data_reel_equal(const afs_toc_data_reel * toc_data_reel1, const afs_toc_data_reel * toc_data_reel2)
{
    if (toc_data_reel1 == NULL && toc_data_reel2 == NULL)
    {
        return DTRUE;
    }

    if (toc_data_reel1 == NULL || toc_data_reel2 == NULL)
    {
        return DFALSE;
    }

    if (boxing_string_equal(toc_data_reel1->id, toc_data_reel2->id) &&
        afs_toc_files_equal(toc_data_reel1->files, toc_data_reel2->files))
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function sets a new id in the TOC data reel structure.
 *
 *  Function sets a new id in the TOC data reel structure.
 *
 *  \param[in] toc_data_reel  Pointer to the afs_toc_data_reel structure.
 *  \param[in] id             Id string.
 */

void afs_toc_data_reel_set_id(afs_toc_data_reel * toc_data_reel, const char * id)
{
    // If TOC data reel pointer is NULL return
    if (toc_data_reel == NULL)
    {
        return;
    }

    if (toc_data_reel->id != NULL)
    {
        boxing_string_free(toc_data_reel->id);
    }

    toc_data_reel->id = boxing_string_clone(id);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_file structure to the input afs_toc_data_reel structure.
 *
 *  The function adds a new instance of afs_toc_file structure to the input afs_toc_data_reel structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_file structure is not add and the function returns DFALSE.
 *  If input afs_toc_file instance is not valid then new instance is not add the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]   toc_file       Pointer to the instance of the afs_toc_file structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reel_add_file(afs_toc_data_reel * toc_data_reel, afs_toc_file * toc_file)
{
    if (toc_data_reel == NULL || toc_file == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_file_is_valid(toc_file) != DTRUE)
    {
        return DFALSE;
    }

    if (toc_data_reel->files == NULL)
    {
        toc_data_reel->files = afs_toc_files_create();
    }

    if (toc_data_reel->files->tocs == NULL)
    {
        toc_data_reel->files->tocs = gvector_create_pointers(0);
    }

    size_t old_size = toc_data_reel->files->tocs->size;

    gvector_append_data(toc_data_reel->files->tocs, 1, &toc_file);

    if (toc_data_reel->files->tocs->size != old_size + 1)
    {
        return DFALSE;
    }

    return DTRUE;
}

//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_file structure according to the input index.
 *
 *  The function returns an instance of afs_toc_file structure according to the input index.
 *
 *  \param[in]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]   index          Index for specifying the required instance of the afs_toc_file structure in the afs_toc_data_reel structure.
 *  \return an instance of afs_toc_file structure or NULL.
 */

afs_toc_file * afs_toc_data_reel_get_file_by_index(const afs_toc_data_reel * toc_data_reel, const unsigned int index)
{
    if (toc_data_reel == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs == NULL)
    {
        return NULL;
    }

    if (index >= toc_data_reel->files->tocs->size)
    {
        return NULL;
    }

    return GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_file structure according to the input id.
 *
 *  The function returns an instance of afs_toc_file structure according to the input id.
 *
 *  \param[in]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]   id             Id for specifying the required instance of the afs_toc_file structure in the storage afs_toc_data_reel structure.
 *  \return an instance of afs_toc_file structure or NULL.
 */

afs_toc_file * afs_toc_data_reel_get_file_by_id(const afs_toc_data_reel * toc_data_reel, const int id)
{
    if (toc_data_reel == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs->size == 0)
    {
        return NULL;
    }

    for (unsigned int i = 0; i < toc_data_reel->files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i);

        if (toc_file->id == id)
        {
            return GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i);
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
 *  \param[in]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]   unique_id      Unique Id for specifying the required instance of the afs_toc_file structure in the storage afs_toc_data_reel structure.
 *  \return an instance of afs_toc_file structure or NULL.
 */

afs_toc_file * afs_toc_data_reel_get_file_by_unique_id(const afs_toc_data_reel * toc_data_reel, const char * unique_id)
{
    if (toc_data_reel == NULL || unique_id == NULL || boxing_string_length(unique_id) == 0)
    {
        return NULL;
    }

    if (toc_data_reel->files == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs->size == 0)
    {
        return NULL;
    }

    for (unsigned int i = 0; i < toc_data_reel->files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i);

        if (boxing_string_equal(toc_file->unique_id, unique_id) == DTRUE)
        {
            return GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i);
        }
    }

    return NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gets all files on reel of type 'parent'.
 *
 *  The function gets all files on reel of type 'parent'.
 *
 *  \param[in]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]   types          Types value.
 *  \return an instance of afs_toc_files structure or NULL.
 */

afs_toc_files * afs_toc_data_reel_get_parent_files(const afs_toc_data_reel * toc_data_reel, const int types)
{
    if (toc_data_reel == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs->size == 0)
    {
        return NULL;
    }

    afs_toc_files * toc_files = NULL;

    int type = 0;

    if (types & AFS_TOC_DATA_REEL_SECTION_DIGITAL)
    {
        type |= AFS_TOC_FILE_TYPE_DIGITAL;
    }

    if (types & AFS_TOC_DATA_REEL_SECTION_PREVIEW)
    {
        type |= AFS_TOC_FILE_TYPE_PREVIEW;
    }

    for (unsigned int i = 0; i < toc_data_reel->files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i);

        if (afs_toc_file_is_parent(toc_file) == DTRUE && (toc_file->types & type) != 0)
        {
            if (toc_files == NULL)
            {
                toc_files = afs_toc_files_create();
            }

            afs_toc_files_add_toc(toc_files, afs_toc_file_clone(toc_file));
        }
    }

    return toc_files;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gets all files on reel of type 'child'.
 *
 *  The function gets all files on reel of type 'child'.
 *
 *  \param[in]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]   types          Types value.
 *  \return an instance of afs_toc_files structure or NULL.
 */

afs_toc_files * afs_toc_data_reel_get_child_files(const afs_toc_data_reel * toc_data_reel, const int types)
{
    if (toc_data_reel == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs->size == 0)
    {
        return NULL;
    }

    afs_toc_files * toc_files = NULL;

    int type = 0;

    if (types & AFS_TOC_DATA_REEL_SECTION_DIGITAL)
    {
        type |= AFS_TOC_FILE_TYPE_DIGITAL;
    }

    if (types & AFS_TOC_DATA_REEL_SECTION_PREVIEW)
    {
        type |= AFS_TOC_FILE_TYPE_PREVIEW;
    }

    for (unsigned int i = 0; i < toc_data_reel->files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i);

        if (afs_toc_file_is_parent(toc_file) == DFALSE && (toc_file->types & type) != 0)
        {
            if (toc_files == NULL)
            {
                toc_files = afs_toc_files_create();
            }

            afs_toc_file * toc_file_add = afs_toc_file_clone(toc_file);
            DBOOL result = afs_toc_files_add_toc(toc_files, toc_file_add);

            if (result == DFALSE)
            {
                afs_toc_file_free(toc_file_add);
            }
        }
    }

    return toc_files;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gets all files without children and parent.
 *
 *  The function gets all files without children and parent.
 *
 *  \param[in]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]   types          Types value.
 *  \return an instance of afs_toc_files structure or NULL.
 */

afs_toc_files * afs_toc_data_reel_get_standalone_files(const afs_toc_data_reel * toc_data_reel, const int types)
{
    if (toc_data_reel == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs == NULL)
    {
        return NULL;
    }

    if (toc_data_reel->files->tocs->size == 0)
    {
        return NULL;
    }

    afs_toc_files * toc_files = NULL;

    int type = 0;

    if (types & AFS_TOC_DATA_REEL_SECTION_DIGITAL)
    {
        type |= AFS_TOC_FILE_TYPE_DIGITAL;
    }

    if (types & AFS_TOC_DATA_REEL_SECTION_PREVIEW)
    {
        type |= AFS_TOC_FILE_TYPE_PREVIEW;
    }

    for (unsigned int i = 0; i < toc_data_reel->files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i);

        if (afs_toc_file_is_standalone(toc_file) == DTRUE && (toc_file->types & type) != 0)
        {
            if (toc_files == NULL)
            {
                toc_files = afs_toc_files_create();
            }

            afs_toc_files_add_toc(toc_files, afs_toc_file_clone(toc_file));
        }
    }

    return toc_files;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_file structure.
 *
 *  The function returns a count of instances of afs_toc_file structure.
 *  If input afs_toc_data_reel is NULL or internal afs_toc_files structure pointer is NULL then function returns zero.
 *
 *  \param[in]  toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \return a count of instances of afs_toc_file structure or zero.
 */

unsigned int afs_toc_data_reel_file_count(const afs_toc_data_reel * toc_data_reel)
{
    if (toc_data_reel == NULL)
    {
        return 0;
    }

    if (toc_data_reel->files == NULL)
    {
        return 0;
    }

    if (toc_data_reel->files->tocs == NULL)
    {
        return 0;
    }

    return (unsigned int)toc_data_reel->files->tocs->size;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of frames in the input afs_toc_data_reel structure instance.
 *
 *  The function returns a count of frames in the input afs_toc_data_reel structure instance.
 *  If input afs_toc_data_reel is NULL or internal afs_toc_files structure pointer is NULL then function returns zero.
 *
 *  \param[in]  toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]  section        Section type.
 *  \return a count of frames or zero.
 */

unsigned int afs_toc_data_reel_frame_count(const afs_toc_data_reel * toc_data_reel, const unsigned int section)
{
    if (toc_data_reel == NULL)
    {
        return 0;
    }

    int first = afs_toc_data_reel_first_frame(toc_data_reel, section);
    int last = afs_toc_data_reel_last_frame(toc_data_reel, section);

    if (first < 0 || last < 0 || first > last)
    {
        return 0;
    }

    return last - first + 1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a number of first frame.
 *
 *  The function returns a number of first frame.
 *  If input afs_toc_data_reel is NULL or internal afs_toc_files structure pointer is NULL or 
 *  afs_toc_files structure has no afs_toc_file instances then function returns -1.
 *
 *  \param[in]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]   section        Section type.
 *  \return a number of first frame or -1.
 */

int afs_toc_data_reel_first_frame(const afs_toc_data_reel * toc_data_reel, const unsigned int section)
{
    if (toc_data_reel == NULL)
    {
        return -1;
    }

    if (toc_data_reel->files == NULL)
    {
        return -1;
    }

    int type = 0;

    if (section & AFS_TOC_DATA_REEL_SECTION_DIGITAL)
    {
        type |= AFS_TOC_FILE_TYPE_DIGITAL;
    }

    if (section & AFS_TOC_DATA_REEL_SECTION_PREVIEW)
    {
        type |= AFS_TOC_FILE_TYPE_PREVIEW;
    }

    int first = -1;
    if (type)
    {
        first = afs_toc_files_get_first_frame(toc_data_reel->files, type);
    }

    return first;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a number of last frame.
 *
 *  The function returns a number of last frame.
 *  If input afs_toc_data_reel is NULL or internal afs_toc_files structure pointer is NULL or 
 *  afs_toc_files structure has no afs_toc_file instances then function returns -1.
 *
 *  \param[in]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]   section        Section type.
 *  \return a number of last frame or -1.
 */

int afs_toc_data_reel_last_frame(const afs_toc_data_reel * toc_data_reel, const unsigned int section)
{
    if (toc_data_reel == NULL)
    {
        return -1;
    }

    if (toc_data_reel->files == NULL)
    {
        return -1;
    }

    int type = 0;

    if (section & AFS_TOC_DATA_REEL_SECTION_DIGITAL)
    {
        type |= AFS_TOC_FILE_TYPE_DIGITAL;
    }

    if (section & AFS_TOC_DATA_REEL_SECTION_PREVIEW)
    {
        type |= AFS_TOC_FILE_TYPE_PREVIEW;
    }

    int last = -1;
    if (type)
    {
        last = afs_toc_files_get_last_frame(toc_data_reel->files, type, NULL);
    }

    return last;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds the offset of the start frame to all afs_toc_file instances.
 *
 *  The function adds the offset of the start frame to all afs_toc_file instances.
 *  If input afs_toc_data_reel pointer is NULL or internal afs_toc_files structure pointer is NULL or
 *  afs_toc_files structure has no afs_toc_file instances then function returns without adding offset.
 *
 *  \param[in,out]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]       offset         The offset that we need to add.
 *  \param[in]       section        Section type.
 */

void afs_toc_data_reel_add_frame_offset(afs_toc_data_reel * toc_data_reel, const unsigned int offset, const unsigned int section)
{
    if (toc_data_reel == NULL)
    {
        return;
    }

    if (toc_data_reel->files == NULL)
    {
        return;
    }

    if (toc_data_reel->files->tocs == NULL)
    {
        return;
    }

    int type = 0;

    if (section & AFS_TOC_DATA_REEL_SECTION_DIGITAL)
    {
        type |= AFS_TOC_FILE_TYPE_DIGITAL;
    }

    if (section & AFS_TOC_DATA_REEL_SECTION_PREVIEW)
    {
        type |= AFS_TOC_FILE_TYPE_PREVIEW;
    }

    if (type)
    {
        afs_toc_files_add_frame_offset(toc_data_reel->files, offset, type);
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the size in bytes.
 *
 *  The function returns the size in bytes.
 *  If input afs_toc_data_reel pointer is NULL or internal afs_toc_files structure pointer is NULL or
 *  afs_toc_files structure has no afs_toc_file instances then function returns without adding offset.
 *
 *  \param[in,out]   toc_data_reel  Pointer to the instance of the afs_toc_data_reel structure.
 *  \param[in]       offset         The offset that we need to add.
 *  \param[in]       section        Section type.
 */

boxing_int64 afs_toc_data_reel_size_bytes(const afs_toc_data_reel * toc_data_reel)
{
    if (toc_data_reel == NULL)
    {
        return 0;
    }

    if (toc_data_reel->files == NULL)
    {
        return 0;
    }

    boxing_int64 size = 0;

    for (size_t i = 0; i < afs_toc_files_get_tocs_count(toc_data_reel->files); i++)
    {
        afs_toc_file * toc_file = afs_toc_files_get_toc(toc_data_reel->files, i);
        if (toc_file->start_byte > toc_file->end_byte)
        {
            return 0;
        }
        size += (boxing_int64)(toc_file->end_byte - toc_file->start_byte + 1);
    }
    
    return size;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity of the input afs_toc_data_reel structure.
 *
 *  The function returns the sign of integrity of the input afs_toc_data_reel structure.
 *  If TOC data reel pointer is NULL then function return DFALSE.
 *
 *  \param[in]  toc_data_reel  Pointer to the afs_toc_data_reel structure.
 *  \return the sign of integrity of the afs_toc_data_reel structures.
 */

DBOOL afs_toc_data_reel_is_valid(const afs_toc_data_reel * toc_data_reel)
{
    if (toc_data_reel == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_data_reel_file_count(toc_data_reel) != 0 && afs_toc_files_is_valid(toc_data_reel->files) == DFALSE)
    {
        return DFALSE;
    }

    if (toc_data_reel->id == NULL || boxing_string_length(toc_data_reel->id) == 0)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gets frame ranges from the input afs_toc_files structure.
 *
 *  The function gets frame ranges from the input afs_toc_data_reel structure.
 *  If TOC data reel pointer is NULL then function return NULL.
 *
 *  \param[in]  toc_files  Pointer to the afs_toc_files structure.
 *  \return the frame ranges structure instance or NULL.
 */

afs_frame_ranges * afs_toc_data_reel_get_frame_ranges(const afs_toc_files * toc_files)
{
    size_t files_count = afs_toc_files_get_tocs_count(toc_files);

    if (files_count == 0)
    {
        return NULL;
    }

    afs_frame_ranges * frame_ranges = afs_frame_ranges_create();

    for (size_t i = 0; i < files_count; i++)
    {
        afs_toc_file * current_toc_file = afs_toc_files_get_toc(toc_files, i);
        afs_frame_range * current_frame_range = afs_frame_range_create2(current_toc_file->start_frame, current_toc_file->end_frame - current_toc_file->start_frame + 1);
        DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, current_frame_range);

        if (result == DFALSE)
        {
            afs_frame_range_free(current_frame_range);
        }
    }
    merge(frame_ranges);

    return frame_ranges;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_data_reel structure to the XML file.
 *
 *  Function translates the input afs_toc_data_reel structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_data_reel  Pointer to the afs_toc_data_reel structure.
 *  \param[in]   file_name      Name of the XML file.
 *  \param[in]   compact        If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reel_save_file(afs_toc_data_reel * toc_data_reel, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_data_reel == NULL || afs_toc_data_reel_is_valid(toc_data_reel) == DFALSE)
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

    if (!afs_toc_data_reel_save_xml(toc_data_reel, tree))
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
 *  \brief Function translates the input afs_toc_data_reel structure to the XML string.
 *
 *  Function translates the input afs_toc_data_reel structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_data_reel     Pointer to the afs_toc_data_reel structure.
 *  \param[in]   compact           If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_data_reel_save_string(afs_toc_data_reel * toc_data_reel, DBOOL compact)
{
    // If TOC data reel pointer is NULL return DFALSE
    if (toc_data_reel == NULL)
    {
        return NULL;
    }

    struct mxml_node_s * document = mxmlNewXML("1.0");

    if (!afs_toc_data_reel_save_xml(toc_data_reel, document))
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
 *  \brief Function translates the input afs_toc_data_reel structure to the XML nodes.
 *
 *  Function translates the input afs_toc_data_reel structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_data_reel  Pointer to the afs_toc_data_reel structure.
 *  \param[out]  out            Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reel_save_xml(afs_toc_data_reel * toc_data_reel, mxml_node_t* out)
{
    // If output node pointer is NULL or TOC data reel pointer is NULL return DFALSE
    if (out == NULL || toc_data_reel == NULL)
    {
        return DFALSE;
    }

    if (!afs_toc_data_reel_is_valid(toc_data_reel))
    {
        return DFALSE;
    }

    mxml_node_t * reel_node = mxmlNewElement(out, "reel");
    afs_xmlutils_add_new_text_node(reel_node, "id", toc_data_reel->id);

    if (toc_data_reel->files != NULL && afs_toc_files_save_xml(toc_data_reel->files, reel_node) == DFALSE)
    {
        return DFALSE;
    }
    
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML file to the afs_toc_data_reel structure.
 *
 *  Function translates the input XML file to the afs_toc_data_reel structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_reel  Pointer to the afs_toc_data_reel structure.
 *  \param[in]   file_name      Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reel_load_file(afs_toc_data_reel * toc_data_reel, const char * file_name)
{
    // If input file name string pointer is NULL or TOC data reel pointer is NULL return DFALSE
    if (file_name == NULL || toc_data_reel == NULL)
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

    DBOOL return_value = afs_toc_data_reel_load_xml(toc_data_reel, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_data_reel structure.
 *
 *  Function translates the input XML string to the afs_toc_data_reel structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_reel  Pointer to the afs_toc_data_reel structure.
 *  \param[in]   in             Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reel_load_string(afs_toc_data_reel * toc_data_reel, const char * in)
{
    // If input string pointer is NULL or TOC data reel pointer is NULL return DFALSE
    if (in == NULL || boxing_string_equal(in, "") || toc_data_reel == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_data_reel_load_xml(toc_data_reel, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC data reel structure.
 *
 *  Function translates the input XML nodes to the TOC data reel structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_reel  Pointer to the afs_toc_data_reel structure.
 *  \param[in]   node           Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reel_load_xml(afs_toc_data_reel * toc_data_reel, mxml_node_t* node)
{
    // If input node pointer is NULL or TOC data reel pointer is NULL return DFALSE
    if (node == NULL || toc_data_reel == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * reel_node = NULL;

    reel_node = boxing_string_equal(mxmlGetElement(node), "reel") == DTRUE ? node : mxmlFindElement(node, node, "reel", NULL, NULL, MXML_DESCEND);

    if (reel_node == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * id_node = mxmlFindElement(reel_node, reel_node, "id", NULL, NULL, MXML_DESCEND);

    if (id_node == NULL)
    {
        return DFALSE;
    }

    toc_data_reel->id = afs_xmlutils_get_node_text(id_node);

    mxml_node_t * files_node = mxmlFindElement(reel_node, reel_node, "files", NULL, NULL, MXML_DESCEND);

    if (files_node != NULL)
    {
        toc_data_reel->files = afs_toc_files_create();

        DBOOL load_files_result = afs_toc_files_load_xml(toc_data_reel->files, files_node);

        if (load_files_result == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of file group
  */




static DBOOL merge_frame_ranges(afs_frame_range * a, const afs_frame_range * b)
{
    if ((a->start > (afs_frame_range_get_end(b) + 1)) || ((afs_frame_range_get_end(a) + 1) < b->start))
    {
        return DFALSE;
    }

    a->start = BOXING_MATH_MIN(a->start, b->start);
    a->count = BOXING_MATH_MAX(afs_frame_range_get_end(a), afs_frame_range_get_end(b)) - a->start + 1;

    return DTRUE;
}


static void sort_frame_ranges(afs_frame_ranges * ranges) // Insertion sorting
{
    afs_frame_range * temp_range; // Temporary variable for storing the value of the element of the vector being sorted
    int item; // Previous item index
    unsigned int ranges_count = afs_frame_ranges_get_count(ranges);

    for (unsigned int counter = 1; counter < ranges_count; counter++)
    {
        temp_range = GVECTORN(ranges->frame_ranges, afs_frame_range *, counter); // Initialize the temporary variable with the current value of the vector element
        item = counter - 1; // Remember the index of the previous element of the vector
        while (item >= 0 && GVECTORN(ranges->frame_ranges, afs_frame_range *, item)->start > temp_range->start) // While the index is not 0 and the previous element of the vector is larger than the current one
        {
            GVECTORN(ranges->frame_ranges, afs_frame_range *, item + 1) = GVECTORN(ranges->frame_ranges, afs_frame_range *, item); // Permutation of array elements
            GVECTORN(ranges->frame_ranges, afs_frame_range *, item) = temp_range;
            item--;
        }
    }
}


static void merge(afs_frame_ranges * ranges)
{
    if (afs_frame_ranges_get_count(ranges) < 2)
    {
        return;
    }

    sort_frame_ranges(ranges);

    afs_frame_ranges * merged_ranges = afs_frame_ranges_create();

    afs_frame_range * range = GVECTORN(ranges->frame_ranges, afs_frame_range *, 0);
    afs_frame_ranges_add_frame_range(merged_ranges, afs_frame_range_clone(range));

    for (unsigned int i = 1; i < afs_frame_ranges_get_count(ranges); i++)
    {
        int current_merged_size = afs_frame_ranges_get_count(merged_ranges);
        afs_frame_range * last_merged_range = GVECTORN(merged_ranges->frame_ranges, afs_frame_range *, current_merged_size - 1);
        afs_frame_range * current_range = GVECTORN(ranges->frame_ranges, afs_frame_range *, i);
        
        if (merge_frame_ranges(last_merged_range, current_range) == DFALSE)
        {
            afs_frame_ranges_add_frame_range(merged_ranges, afs_frame_range_clone(current_range));
        }
    }

    gvector_swap(ranges->frame_ranges, merged_ranges->frame_ranges);
    
    afs_frame_ranges_free(merged_ranges);
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

    if (boxing_string_equal("reel", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("reel", parent_name) && boxing_string_equal("id", name))
    {
        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("reel", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("file", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("file", parent_name) && !boxing_string_equal("data", name) && !boxing_string_equal("preview", name) && !boxing_string_equal("metadata", name))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n            ");
        }

        if (where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }

        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n                ");
        }
    }

    if (boxing_string_equal("data", name) && boxing_string_equal("file", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }
    }

    if (boxing_string_equal("data", parent_name) && boxing_string_equal("file", mxmlGetElement(node->parent->parent)))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                ");
        }

        if (where == MXML_WS_AFTER_OPEN && !boxing_string_equal("start", name) && !boxing_string_equal("end", name))
        {
            return ("\n                    ");
        }
    }

    if (boxing_string_equal("preview", name) && boxing_string_equal("file", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }
    }

    if (boxing_string_equal("preview", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n                ");
        }
    }

    if (boxing_string_equal("metadata", name) && boxing_string_equal("file", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }
    }

    if (boxing_string_equal("metadata", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                ");
        }
    }

    if (boxing_string_equal("source", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                    ");
        }
    }

    if (boxing_string_equal("data", parent_name) && boxing_string_equal("source", mxmlGetElement(node->parent->parent)))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n                        ");
        }
    }

    return (NULL);
}
