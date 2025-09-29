/*****************************************************************************
**
**  Definition of the TOC files interface
**
**  Creation date:  2017/05/26
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

#include "tocfiles.h"
#include "boxing/log.h"
#include "boxing/utils.h"
#include "mxml.h"

//  SYSTEM INCLUDES
//
#include <inttypes.h>

// DEFINES
//

#define SET_MAX_LENGTH(saved_length,new_length) { unsigned int nl = new_length; saved_length = nl > saved_length ? nl : saved_length; }

typedef struct toc_files_lengths_s
{
    unsigned int id_length;
    unsigned int unique_id_length;
    unsigned int parent_id_length;
    unsigned int format_id_length;
    unsigned int file_name_length;
} toc_files_lengths;


typedef struct toc_files_location_lengths_s
{
    unsigned int digital_id_length;
    unsigned int digital_start_frame_length;
    unsigned int digital_start_byte_length;
    unsigned int digital_end_frame_length;
    unsigned int digital_end_byte_length;
    unsigned int digital_size_length;
    unsigned int digital_sha1_checksum_length;
    unsigned int visual_id_length;
    unsigned int visual_layout_id_length;
    unsigned int visual_start_frame_length;
    unsigned int visual_start_section_length;
    unsigned int visual_section_count_length;
    unsigned int visual_dimension_length;
    unsigned int visual_overlap_length;
    unsigned int visual_pages_count;
} toc_files_location_lengths;


typedef struct toc_files_metadata_lengths_s
{
    unsigned int file_id_length;
    unsigned int id_length;
    unsigned int source_file_id_length;
    unsigned int source_id_length;
    unsigned int source_format_id_length;
    unsigned int source_data_length;
    unsigned int metadata_sources_count;
} toc_files_metadata_lengths;

// PRIVATE INTERFACE
//
static toc_files_lengths          get_max_toc_files_lenght(const afs_toc_files * toc_files);
static toc_files_location_lengths get_max_toc_files_location_lenght(const afs_toc_files * toc_files);
static toc_files_metadata_lengths get_max_toc_files_metadata_lenght(const afs_toc_files * toc_files);
static unsigned int               get_digits_count(int number);
static unsigned int               get_unsigned_digits_count(unsigned int number);
static unsigned int               get_digits_count_64(boxing_int64 number);
static const char *               whitespace_cb(mxml_node_t *node, int where);
void                              string_copy_strip_newline(char* destination, const char* source, unsigned int destination_length);

/*! 
  * \addtogroup file
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_files_s  tocfiles.h
 *  \brief      TOC files data storage.
 *
 *  \param tocs      Vector with afs_toc_file instances.
 *
 *  Structure for storing TOC files data.
 */

// PUBLIC AFS TOC FILES FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC files instance.
 *
 *  Allocate memory for the afs_toc_files type
 *  and initializes vector for storing TOC files data.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_files structure.
 */

afs_toc_files* afs_toc_files_create(void)
{
    afs_toc_files* toc_files = malloc(sizeof(afs_toc_files));
    afs_toc_files_init(toc_files);
    return toc_files;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC files instance.
 *
 *  Allocate memory for the afs_toc_files type
 *  and initializes structure vector pointer with specified values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_files structure.
 */

afs_toc_files* afs_toc_files_create2(afs_toc_indices * tocs)
{
    afs_toc_files* toc_files = malloc(sizeof(afs_toc_files));
    afs_toc_files_init2(toc_files, tocs);
    return toc_files;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes vector pointer in the structure with NULL value.
 *
 *  Initializes vector pointer in the input structure with NULL value.
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  toc_files  Pointer to the afs_toc_files structure.
 */

void afs_toc_files_init(afs_toc_files * toc_files)
{
    if (toc_files == NULL)
    {
        return;
    }

    toc_files->tocs = NULL;

    toc_files->reference_count = 1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes vector pointer in the structure with input value.
 *
 *  Initializes vector pointer in the input afs_toc_files structure with specified input value.
 *  If input pointer is NULL then return without initialization.
 *
 *  \param[in]  toc_files  Pointer to the afs_toc_files structure.
 *  \param[in]  tocs       Pointer to the vector with afs_toc_file_preview_page structure pointers.
 */

void afs_toc_files_init2(afs_toc_files * toc_files, afs_toc_indices* tocs)
{
    if (toc_files == NULL)
    {
        return;
    }

    toc_files->tocs = tocs;

    toc_files->reference_count = 1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_files structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_files  Pointer to the afs_toc_files structure.
 */

void afs_toc_files_free(afs_toc_files * toc_files)
{
    if (toc_files == NULL)
    {
        return;
    }

    toc_files->reference_count--;

    if (toc_files->reference_count <= 0)
    {
        if (toc_files->tocs != NULL)
        {
            for (unsigned int i = 0; i < toc_files->tocs->size; i++)
            {
                afs_toc_file_free(GVECTORN(toc_files->tocs, afs_toc_file *, i));
                GVECTORN(toc_files->tocs, afs_toc_file *, i) = NULL;
            }
        }

        gvector_free(toc_files->tocs);
        free(toc_files);
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_files structure.
 *
 *  Function create a copy of input afs_toc_files structure and return it.
 *  If TOC files pointer is NULL function return NULL.
 *
 *  \param[in]  toc_files  Pointer to the afs_toc_files structure.
 *  \return new copy of afs_toc_files structure or NULL.
 */

afs_toc_files* afs_toc_files_clone(afs_toc_files * toc_files)
{
    // If TOC files pointer is NULL return NULL.
    if (toc_files == NULL)
    {
        return NULL;
    }

    if (toc_files->tocs == NULL)
    {
        return afs_toc_files_create();
    }

    afs_toc_files* return_toc_files = afs_toc_files_create2(gvector_create_pointers(toc_files->tocs->size));
    
    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file* toc_file = GVECTORN(toc_files->tocs, afs_toc_file *, i);
        GVECTORN(return_toc_files->tocs, afs_toc_file *, i) = afs_toc_file_clone(toc_file);
    }

    return return_toc_files;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function returns a new reference to the input afs_toc_files structure.
 *
 *  Function returns a new reference to the input afs_toc_files structure.
 *  The reference count is incremented by 1.
 *  If TOC files pointer is NULL function return NULL.
 *
 *  \param[in]  toc_files  Pointer to the afs_toc_files structure.
 *  \return new reference of afs_toc_files structure or NULL.
 */

afs_toc_files * afs_toc_files_get_new_reference(afs_toc_files * toc_files)
{
    // If TOC files pointer is NULL return NULL.
    if (toc_files == NULL)
    {
        return NULL;
    }

    toc_files->reference_count++;
    return toc_files;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_files structures on the identity.
 *
 *  Function checks two instances of the afs_toc_files structures on the identity.
 *
 *  \param[in]   toc_files1  Pointer to the first instance of the afs_toc_files structure.
 *  \param[in]   toc_files2  Pointer to the second instance of the afs_toc_files structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_files_equal(afs_toc_files * toc_files1, afs_toc_files * toc_files2)
{
    if (toc_files1 == NULL && toc_files2 == NULL)
    {
        return DTRUE;
    }

    if (toc_files1 == NULL || toc_files2 == NULL)
    {
        return DFALSE;
    }

    if (toc_files1->tocs == NULL && toc_files2->tocs == NULL)
    {
        return DTRUE;
    }

    if (toc_files1->tocs == NULL || toc_files2->tocs == NULL)
    {
        return DFALSE;
    }

    if (toc_files1->tocs->size != toc_files2->tocs->size)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_files1->tocs->size; i++)
    {
        if (afs_toc_file_equal(GVECTORN(toc_files1->tocs, afs_toc_file *, i), GVECTORN(toc_files2->tocs, afs_toc_file *, i)) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_file structure to the input afs_toc_files structure.
 *
 *  The function adds a new instance of afs_toc_file structure to the input afs_toc_files structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_file structure is not add and the function returns DFALSE.
 *  If input afs_toc_file instance is not valid then new instance is not add the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_files  Pointer to the instance of the afs_toc_files structure.
 *  \param[in]   toc_file   Pointer to the instance of the afs_toc_file structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_files_add_toc(afs_toc_files * toc_files, afs_toc_file * toc_file)
{
    if (toc_files == NULL || toc_file == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_file_is_valid(toc_file) != DTRUE)
    {
        return DFALSE;
    }

    if (toc_files->tocs == NULL)
    {
        toc_files->tocs = gvector_create_pointers(0);
    }

    size_t old_size = toc_files->tocs->size;

    gvector_append_data(toc_files->tocs, 1, &toc_file);

    if (toc_files->tocs->size != old_size + 1)
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
 *  \param[in]   toc_files  Pointer to the instance of the afs_toc_files structure.
 *  \param[in]   index      Index for specifying the required instance of the afs_toc_file structure in the storage vector.
 *  \return an instance of afs_toc_file structure or NULL.
 */

afs_toc_file * afs_toc_files_get_toc(const afs_toc_files * toc_files, const size_t index)
{
    if (toc_files == NULL)
    {
        return NULL;
    }

    if (toc_files->tocs == NULL)
    {
        return NULL;
    }

    if (index >= toc_files->tocs->size)
    {
        return NULL;
    }

    return GVECTORN(toc_files->tocs, afs_toc_file *, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_file structure.
 *
 *  The function returns a count of instances of afs_toc_file structure.
 *  If input afs_toc_files pointer is NULL or internal tocs vector pointer is NULL then function returns zero.
 *
 *  \param[in]   toc_files  Pointer to the instance of the afs_toc_files structure.
 *  \return a count of instances of afs_toc_file structure or zero.
 */

size_t afs_toc_files_get_tocs_count( const afs_toc_files * toc_files)
{
    if (toc_files == NULL)
    {
        return 0;
    }

    if (toc_files->tocs == NULL)
    {
        return 0;
    }

    return toc_files->tocs->size;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns frame duration.
 *
 *  The function returns frame duration according to the given afs_toc_files structure.
 *  If input afs_toc_files pointer is NULL or first or last frame is undefined then function returns zero.
 *
 *  \param[in]   toc_files  Pointer to the instance of the afs_toc_files structure.
 *  \param[in]   type       File type.
 *  \return frame duration.
 */

int afs_toc_files_get_duration_frames(afs_toc_files * toc_files, unsigned int type, afs_toc_preview_layout_definitions * definitions)
{
    int first = afs_toc_files_get_first_frame(toc_files, type);
    int last = afs_toc_files_get_last_frame(toc_files, type, definitions);

    if (first < 0 || last < 0)
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
 *  If input afs_toc_files pointer is NULL or internal tocs vector pointer is NULL or vector has no pointers then function returns -1.
 *
 *  \param[in]   toc_files  Pointer to the instance of the afs_toc_files structure.
 *  \param[in]   type       File type.
 *  \return a number of first frame.
 */

int afs_toc_files_get_first_frame(afs_toc_files * toc_files, unsigned int type)
{
    if (toc_files == NULL)
    {
        return -1;
    }

    if (toc_files->tocs == NULL)
    {
        return -1;
    }

    if (toc_files->tocs->size == 0)
    {
        return -1;
    }

    if ( type == AFS_TOC_FILE_TYPE_UNDEFINED )
    {
        return -1;
    }

    if ( type == AFS_TOC_FILE_TYPE_ALL )
    {
        const int firstDigitalFrame = afs_toc_files_get_first_frame( toc_files, AFS_TOC_FILE_TYPE_DIGITAL );
        const int firstVisualFrame = afs_toc_files_get_first_frame( toc_files, AFS_TOC_FILE_TYPE_PREVIEW );
        if ( firstDigitalFrame != -1 && firstVisualFrame != -1 )
        {
            return BOXING_MATH_MIN( firstVisualFrame, firstDigitalFrame );
        }
        else if ( firstDigitalFrame != -1 )
        {
            return firstDigitalFrame;
        }
        else if ( firstVisualFrame != -1 )
        {
            return firstVisualFrame;
        }
    }
    else
    {
        for ( size_t i = 0; i < toc_files->tocs->size; i++ )
        {
            afs_toc_file * current_toc = GVECTORN( toc_files->tocs, afs_toc_file *, i );

            if ( current_toc != NULL )
            {
                if ( (type & AFS_TOC_FILE_TYPE_DIGITAL) && afs_toc_file_is_digital( current_toc ) )
                {
                    return current_toc->start_frame;
                }

                if ( (type & AFS_TOC_FILE_TYPE_PREVIEW) && afs_toc_file_is_preview( current_toc ) )
                {
                    return afs_toc_file_preview_get_start_frame( current_toc->preview );
                }
            }
        }
    }

    return -1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a number of last frame.
 *
 *  The function returns a number of last frame.
 *  If input afs_toc_files pointer is NULL or internal tocs vector pointer is NULL or vector has no pointers then function returns -1.
 *
 *  \param[in]   toc_files    Pointer to the instance of the afs_toc_files structure.
 *  \param[in]   type         File type.
 *  \param[in]   definitions  Pointer to the instance of the afs_toc_preview_layout_definitions structure.
 *  \return a number of last frame.
 */

int afs_toc_files_get_last_frame(afs_toc_files * toc_files, unsigned int type, afs_toc_preview_layout_definitions * definitions)
{
    if (toc_files == NULL)
    {
        return -1;
    }

    if (toc_files->tocs == NULL)
    {
        return -1;
    }

    if (toc_files->tocs->size == 0)
    {
        return -1;
    }

    if ( type == AFS_TOC_FILE_TYPE_PREVIEW && definitions == NULL )
    {
        return -1;
    }

    if ( type == AFS_TOC_FILE_TYPE_UNDEFINED )
    {
        return -1;
    }

    if ( type == AFS_TOC_FILE_TYPE_ALL )
    {
        return BOXING_MATH_MAX(
            afs_toc_files_get_last_frame( toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL ), 
            afs_toc_files_get_last_frame( toc_files, AFS_TOC_FILE_TYPE_PREVIEW, definitions ) );
    }
    else
    {
        for ( size_t i = toc_files->tocs->size; i > 0; i-- )
        {
            afs_toc_file * current_toc = GVECTORN( toc_files->tocs, afs_toc_file *, i - 1 );

            if ( current_toc != NULL )
            {
                if ( (type & AFS_TOC_FILE_TYPE_DIGITAL) && afs_toc_file_is_digital( current_toc ) )
                {
                    return current_toc->end_frame;
                }

                if ( (type & AFS_TOC_FILE_TYPE_PREVIEW) && afs_toc_file_is_preview( current_toc ) )
                {
                    const int startFrame = afs_toc_file_preview_get_start_frame( current_toc->preview );
                    unsigned int frameCount;
                    afs_toc_file_preview_get_frames_count( current_toc->preview, &frameCount, definitions );
                    return startFrame + frameCount - 1;
                }
            }
        }
    }

    return -1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds the offset of the start frame to all afs_toc_file instances.
 *
 *  The function adds the offset of the start frame to all afs_toc_file instances.
 *  If input afs_toc_files pointer is NULL or internal pages vector pointer is NULL then function returns without adding offset.
 *
 *  \param[in,out]   toc_files  Pointer to the instance of the afs_toc_files structure.
 *  \param[in]       offset     The offset that we need to add.
 *  \param[in]       type       File type.
 */

void afs_toc_files_add_frame_offset(afs_toc_files * toc_files, unsigned int offset, unsigned int type)
{
    if (toc_files == NULL)
    {
        return;
    }

    if (toc_files->tocs == NULL)
    {
        return;
    }

    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        if (type & AFS_TOC_FILE_TYPE_DIGITAL)
        {
            if (GVECTORN(toc_files->tocs, afs_toc_file *, i)->start_frame >= 0)
            {
                GVECTORN(toc_files->tocs, afs_toc_file *, i)->start_frame += offset;
            }
            if (GVECTORN(toc_files->tocs, afs_toc_file *, i)->end_frame >= 0)
            {
                GVECTORN(toc_files->tocs, afs_toc_file *, i)->end_frame += offset;
            }
        }

        if (type & AFS_TOC_FILE_TYPE_PREVIEW)
        {
            afs_toc_file_preview_add_frame_offset(GVECTORN(toc_files->tocs, afs_toc_file *, i)->preview, offset);
        }
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the all afs_toc_file structures in the input afs_toc_files structure.
 *
 *  The function returns the sign of integrity the all afs_toc_file structures in the input afs_toc_files structure.
 *  If TOC files pointer is NULL or internal tocs vector pointer is NULL or vector has no pointers then function return DFALSE.
 *
 *  \param[in]   toc_files   Pointer to the afs_toc_files structure.
 *  \return the sign of integrity the all afs_toc_file structures.
 */

DBOOL afs_toc_files_is_valid(afs_toc_files * toc_files)
{
    if (toc_files == NULL)
    {
        return DFALSE;
    }

    if (toc_files->tocs == NULL)
    {
        return DTRUE;
    }

    if (toc_files->tocs->size == 0)
    {
        return DTRUE;
    }

    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        if (afs_toc_file_is_valid(GVECTORN(toc_files->tocs, afs_toc_file *, i)) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_files structure to the XML file.
 *
 *  Function translates the input afs_toc_files structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_files         Pointer to the afs_toc_files structure.
 *  \param[in]   file_name         Name of the XML file.
 *  \param[in]   compact           If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_files_save_file(afs_toc_files * toc_files, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_files == NULL)
    {
        return DFALSE;
    }

    mxml_node_t *tree = mxmlNewXML("1.0");

    if (!afs_toc_files_save_xml(toc_files, tree))
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
 *  \brief Function translates the input afs_toc_files structure to the XML string.
 *
 *  Function translates the input afs_toc_files structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_files         Pointer to the afs_toc_files structure.
 *  \param[in]   compact           If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_files_save_string(afs_toc_files * toc_files, DBOOL compact)
{
    // If TOC files pointer is NULL return DFALSE
    if (toc_files == NULL)
    {
        return NULL;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_files_save_xml(toc_files, document))
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
 *  \brief Function translates the input afs_toc_files structure to the XML nodes.
 *
 *  Function translates the input afs_toc_files structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_files          Pointer to the afs_toc_files structure.
 *  \param[out]  out                Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_files_save_xml(afs_toc_files * toc_files, mxml_node_t* out)
{
    // If output node pointer is NULL or TOC files pointer is NULL return DFALSE
    if (out == NULL || toc_files == NULL)
    {
        return DFALSE;
    }

    if (!afs_toc_files_is_valid(toc_files))
    {
        return DFALSE;
    }

    mxml_node_t * files_node = mxmlNewElement(out, "files");

    for (size_t i = 0; i < afs_toc_files_get_tocs_count(toc_files); i++)
    {
        DBOOL result = afs_toc_file_save_xml(GVECTORN(toc_files->tocs, afs_toc_file *, i), files_node, DFALSE);

        if (result == DFALSE)
        {
            return DFALSE;
        }
    }
    
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_files structure.
 *
 *  Function translates the input XML nodes to the afs_toc_files structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_files         Pointer to the afs_toc_files structure.
 *  \param[in]   file_name         Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_files_load_file(afs_toc_files * toc_files, const char * file_name)
{
    // If input file name string pointer is NULL or TOC files pointer is NULL return DFALSE
    if (file_name == NULL || toc_files == NULL)
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
    
    DBOOL return_value = afs_toc_files_load_xml(toc_files, document);
    
    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_files structure.
 *
 *  Function translates the input XML string to the afs_toc_files structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_files         Pointer to the afs_toc_files structure.
 *  \param[in]   in                Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_files_load_string(afs_toc_files * toc_files, const char * in)
{
    // If input string pointer is NULL or TOC files pointer is NULL return DFALSE
    if (in == NULL || strlen(in) == 0 || toc_files == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_files_load_xml(toc_files, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC files structure.
 *
 *  Function translates the input XML nodes to the TOC files structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_files         Pointer to the afs_toc_files structure.
 *  \param[in]   node              Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_files_load_xml(afs_toc_files * toc_files, mxml_node_t* node)
{
    // If input node pointer is NULL or TOC file preview pointer is NULL return DFALSE
    if (node == NULL || toc_files == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * toc_node = mxmlFindElement(node, node, "file", NULL, NULL, MXML_DESCEND);

    while (toc_node != NULL)
    {
        if (mxmlGetType(toc_node) == MXML_ELEMENT)
        {
            afs_toc_file * toc_file = afs_toc_file_create();

            DBOOL load_toc_result = afs_toc_file_load_xml(toc_file, toc_node);

            if (load_toc_result == DFALSE)
            {
                DLOG_INFO("Has no TOC.\n");
                return DFALSE;
            }

            DBOOL add_toc_result = afs_toc_files_add_toc(toc_files, toc_file);

            if (add_toc_result == DFALSE)
            {
                afs_toc_file_free(toc_file);
                return DFALSE;
            }
        }

        toc_node = mxmlFindElement(toc_node, node, "file", NULL, NULL, MXML_DESCEND); // mxmlWalkNext(toc_node, node, MXML_NO_DESCEND);
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_files structure to the string table.
 *
 *  Function translates the input afs_toc_files structure to the string table.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_files Pointer to the afs_toc_files structure.
 *  \return resulting string or NULL.
 */

char * afs_toc_files_save_as_table(afs_toc_files * toc_files)
{
    size_t tocs_count = afs_toc_files_get_tocs_count(toc_files);

    const char * header = "<fileId> <uniqueId> <parentId> <formatId> <fileName>";
    size_t header_length = strlen(header);

    if (tocs_count == 0)
    {
        char * return_string = malloc(header_length + 2 + 1);
        sprintf(return_string, "%s\n\n", header);
        return return_string;
    }

    toc_files_lengths lengths;
    
    lengths = get_max_toc_files_lenght(toc_files);
    
    size_t table1_width = lengths.id_length + lengths.unique_id_length + lengths.parent_id_length +
        lengths.format_id_length + lengths.file_name_length + 5;
    size_t table1_length = header_length + table1_width * tocs_count + 1;

    char * return_string = malloc(table1_length + 1);
    char * current_string = return_string;
    
    current_string += sprintf(current_string, "%s\n", header);

    for (size_t i = 0; i < tocs_count; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_files->tocs, afs_toc_file *, i);

        char parent_id_string[255]; // Temporary string
        if (toc_file->parent_id == AFS_TOC_FILE_NO_PARENT)
        {
            lengths.parent_id_length > 1 ? sprintf(parent_id_string, "%0*d%s", lengths.parent_id_length - 1, 0, "N") : sprintf(parent_id_string, "%s", "N");
        }
        else if (toc_file->parent_id == AFS_TOC_FILE_PARENT)
        {
            lengths.parent_id_length > 1 ? sprintf(parent_id_string, "%0*d%s", lengths.parent_id_length - 1, 0, "P") : sprintf(parent_id_string, "%s", "P");
        }
        else
        {
            sprintf(parent_id_string, "%0*d", lengths.parent_id_length, toc_file->parent_id);
        }

        char file_format_string[255]; // Temporary string
        if (strlen(toc_file->file_format) != 0)
        {
            sprintf(file_format_string, "%s", toc_file->file_format);
        }
        else
        {
            sprintf(file_format_string, "%s", "N");
        }

        current_string += sprintf(current_string, "%0*d %-*s %s %-*s %s\n", lengths.id_length, toc_file->id, 36, toc_file->unique_id,
            parent_id_string, lengths.format_id_length, file_format_string, toc_file->name);
    }

    return return_string;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Write TOC in human readable table format to string 
 *
 *  \param[in]  toc_files Pointer to the afs_toc_files structure.
 *  \return resulting string or NULL.
 */

char * afs_toc_files_save_as_location_table(afs_toc_files * toc_files)
{
    // If TOC files pointer is NULL return NULL
    if (toc_files == NULL)
    {
        return NULL;
    }

    if (toc_files->tocs == NULL)
    {
        char* empty = boxing_string_clone("");
        return empty;
    }

    const char * digital_header =
        "DIGITAL DATA LOCATIONS\n"
        "======================\n";
    const char * digital_header2 = "<fileId> <startFrame> <startByte> <endFrame> <endByte> <size> <sha1Checksum>\n";
    const char * visual_header =
        "VISUAL DATA LOCATIONS\n"
        "=====================\n"
        "<fileId> <layoutId> <startFrame> <startSection> <sectionCount> <dimensionWxH> <overlapWxH>\n";

    toc_files_location_lengths lengths = get_max_toc_files_location_lenght(toc_files);
    size_t tocs_count = afs_toc_files_get_tocs_count(toc_files);

    size_t newline_length = strlen("\n");
    size_t digital_header_length = strlen(digital_header);
    size_t digital_header2_length = strlen(digital_header2);
    size_t visual_header_length = strlen(visual_header);

    size_t digital_table_width = lengths.digital_id_length + lengths.digital_start_frame_length + lengths.digital_start_byte_length +
        lengths.digital_end_frame_length + lengths.digital_end_byte_length + lengths.digital_size_length + lengths.digital_sha1_checksum_length + 6 + newline_length;
    size_t digital_table_length = digital_header_length + digital_header2_length + digital_table_width * tocs_count + newline_length;

    size_t visual_table_width = lengths.visual_id_length + lengths.visual_layout_id_length + lengths.visual_start_frame_length +
        lengths.visual_start_section_length + lengths.visual_section_count_length + lengths.visual_dimension_length + lengths.visual_overlap_length + 6 + newline_length;
    size_t visual_table_length = visual_header_length + visual_table_width * lengths.visual_pages_count;

    char * return_string = malloc(digital_table_length + visual_table_length + 1);
    char * current_string = return_string;

    DBOOL digital_header_done = DFALSE;
    for (size_t i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_files->tocs, afs_toc_file *, i);

        if (toc_file == NULL)
        {
            continue;
        }

        if (afs_toc_file_is_digital(toc_file) == DTRUE)
        {
            if (digital_header_done == DFALSE)
            {
                current_string += sprintf(current_string, "%s%s", digital_header, digital_header2);
                digital_header_done = DTRUE;
            }

            current_string += sprintf(current_string, "%0*d %0*d %0*d %0*d %0*d %0*"PRId64" %s\n", lengths.digital_id_length, toc_file->id,
                lengths.digital_start_frame_length, toc_file->start_frame, lengths.digital_start_byte_length, toc_file->start_byte, lengths.digital_end_frame_length, toc_file->end_frame,
                lengths.digital_end_byte_length, toc_file->end_byte, lengths.digital_size_length, toc_file->size, toc_file->checksum);
        }
    }

    if (digital_header_done == DTRUE)
    {
        current_string += sprintf(current_string, "\n");
    }

    DBOOL visual_header_done = DFALSE;

    for (size_t i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_files->tocs, afs_toc_file *, i);

        if (toc_file == NULL)
        {
            continue;
        }

        if (afs_toc_file_is_preview(toc_file) == DTRUE && afs_toc_file_is_parent(toc_file) == DFALSE)
        {
            afs_toc_file_preview * toc_file_preview = toc_file->preview;

            if (toc_file_preview == NULL)
            {
                continue;
            }

            if (toc_file_preview->pages == NULL)
            {
                continue;
            }

            for (unsigned int j = 0; j < toc_file_preview->pages->size; j++)
            {
                afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file_preview, j);

                if (toc_file_preview_page == NULL)
                {
                    continue;
                }

                if (visual_header_done == DFALSE)
                {
                    current_string += sprintf(current_string, "%s", visual_header);
                    visual_header_done = DTRUE;
                }

                char layout_id_string[255]; // Temporary string
                unsigned int layout_id_length = (unsigned int)strlen(toc_file_preview_page->layout_id);
                if (lengths.visual_layout_id_length > layout_id_length)
                {
                    sprintf(layout_id_string, "%0*d%s", lengths.visual_layout_id_length - layout_id_length, 0, toc_file_preview_page->layout_id);
                }
                else
                {
                    sprintf(layout_id_string, "%s", toc_file_preview_page->layout_id);
                }

                char dimension_string[255]; // Temporary string
                sprintf(dimension_string, "%0*ux%0*u", 5, toc_file_preview_page->dimension_x, 5, toc_file_preview_page->dimension_y);
                char overlap_string[255]; // Temporary string
                sprintf(overlap_string, "%0*ux%0*u", 2, toc_file_preview_page->overlap_x, 2, toc_file_preview_page->overlap_y);

                current_string += sprintf(current_string, "%0*d %s %0*u %0*u %0*u %s %s\n", lengths.visual_id_length, toc_file->id,
                    layout_id_string, lengths.visual_start_frame_length, toc_file_preview_page->start_frame, lengths.visual_start_section_length, toc_file_preview_page->start_section,
                    lengths.visual_section_count_length, toc_file_preview_page->section_count, dimension_string, overlap_string);
            }
        }
    }

    return return_string;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_files structure to the string of metadata table.
 *
 *  Function translates the input afs_toc_files structure to the string of metadata table.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]  toc_files Pointer to the afs_toc_files structure.
 *  \return resulting string or NULL.
 */

char * afs_toc_files_save_as_metadata_table(afs_toc_files * toc_files)
{
    // If TOC files pointer is NULL return NULL
    if (toc_files == NULL)
    {
        return NULL;
    }

    if (toc_files->tocs == NULL)
    {
        return NULL;
    }

    const char * metadata_header =
        "\n"
        "USER DATA FILES METADATA\n"
        "========================\n";
    const char * metadata_columns =
        "<fileId> <sourceFileId> <sourceId> <formatId> <data>\n";
    toc_files_metadata_lengths lengths = get_max_toc_files_metadata_lenght(toc_files);

    unsigned int metadata_header_length = (unsigned int)(strlen(metadata_header) + strlen(metadata_columns));
    unsigned int metadata_table_width = lengths.file_id_length + lengths.id_length + lengths.source_file_id_length +
        lengths.source_id_length + lengths.source_format_id_length + lengths.source_data_length + (unsigned int)5 + (unsigned int)strlen("\n");
    unsigned int metadata_table_length = metadata_header_length + metadata_table_width * lengths.metadata_sources_count + 1;

    size_t current_string_remaining_size = metadata_table_length + 1;
    char * return_string = malloc(current_string_remaining_size);
    char * current_string = return_string;

    DBOOL metadata_header_done = DFALSE;

    int n;

    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_files->tocs, afs_toc_file *, i);

        if (toc_file == NULL)
        {
            continue;
        }

        if (afs_toc_file_has_metadata(toc_file) == DTRUE)
        {
            afs_toc_data_file_metadata * toc_data_file_metadata = toc_file->metadata;

            unsigned int sources_count = afs_toc_data_file_metadata_get_sources_count(toc_data_file_metadata);
            for (unsigned int j = 0; j < sources_count; j++)
            {
                afs_toc_data_file_metadata_source * metadata_source = GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, j);

                if (metadata_source == NULL)
                {
                    continue;
                }

                if (metadata_header_done == DFALSE)
                {
                    n = snprintf(current_string, current_string_remaining_size, "%s", metadata_header);
                    assert(n >= 0 && n < current_string_remaining_size);
                    current_string_remaining_size -= n;
                    current_string += n;
                    n = snprintf(current_string, current_string_remaining_size, "%s", metadata_columns);
                    assert(n >= 0 && n < current_string_remaining_size);
                    current_string_remaining_size -= n;
                    current_string += n;
                    metadata_header_done = DTRUE;
                }

                #define MAX_DATA_LENGTH 55

                char data[MAX_DATA_LENGTH];
                unsigned int data_length = (unsigned int)strlen(metadata_source->data);
                
                if (data_length >= MAX_DATA_LENGTH)
                {
                    string_copy_strip_newline(data, metadata_source->data, MAX_DATA_LENGTH);
                    strcpy(data+(MAX_DATA_LENGTH-4), "...");
                }
                else
                {
                    string_copy_strip_newline(data, metadata_source->data, data_length + 1);
                }

                char source_id[32];
                if ( lengths.source_id_length > sizeof(source_id))
                {
                    free(return_string);
                    return NULL;
                }
                
                if ( metadata_source->source_id != -1 )
                {
                    sprintf(source_id, "%0*d", lengths.source_id_length, metadata_source->source_id);
                }
                else
                {
                    if (lengths.source_id_length > 1)
                    {
                        sprintf(source_id, "%0*d%s", lengths.source_id_length, 0, "N");
                    }
                    else
                    {
                        sprintf(source_id, "%s", "N");
                    }
                }

                n = snprintf(current_string, "%0*d %0*d %s %-*s %s\n",
                    current_string_remaining_size,
                    lengths.file_id_length, toc_file->id,
                    lengths.source_file_id_length, metadata_source->file_id,
                    source_id,
                    lengths.source_format_id_length, metadata_source->format_id,
                    data);
                assert(n >= 0 && n < current_string_remaining_size);
                current_string_remaining_size -= n;
                current_string += n;

            }
        }
    }

    return return_string;
}


//----------------------------------------------------------------------------
/*!
  * \} end of file group
  */


// PRIVATE AFS TOC FILES FUNCTIONS
//


static toc_files_lengths get_max_toc_files_lenght(const afs_toc_files * toc_files)
{
    toc_files_lengths lengths;

    lengths.id_length = 1;
    lengths.unique_id_length = 36;
    lengths.parent_id_length = 1;
    lengths.format_id_length = 0;
    lengths.file_name_length = 0;

    if (toc_files == NULL)
    {
        return lengths;
    }

    if (toc_files->tocs == NULL)
    {
        return lengths;
    }

    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_files->tocs, afs_toc_file *, i);
        
        SET_MAX_LENGTH(lengths.id_length, get_digits_count(toc_file->id));
        SET_MAX_LENGTH(lengths.unique_id_length, (unsigned int)strlen(toc_file->unique_id));

        unsigned int current_parent_id_length = toc_file->parent_id != AFS_TOC_FILE_PARENT && toc_file->parent_id != AFS_TOC_FILE_NO_PARENT ? get_digits_count(toc_file->parent_id) : 1;
        SET_MAX_LENGTH(lengths.parent_id_length, current_parent_id_length);

        unsigned int current_format_id_length = strlen(toc_file->file_format) == 0 ? 1 : (unsigned int)strlen(toc_file->file_format);
        SET_MAX_LENGTH(lengths.format_id_length, current_format_id_length);
        SET_MAX_LENGTH(lengths.file_name_length, (unsigned int)strlen(toc_file->name));
    }

    return lengths;
}


static toc_files_location_lengths get_max_toc_files_location_lenght(const afs_toc_files * toc_files)
{
    toc_files_location_lengths lengths;

    lengths.digital_id_length = 1;
    lengths.digital_start_frame_length = 3;
    lengths.digital_start_byte_length = 7;
    lengths.digital_end_frame_length = 3;
    lengths.digital_end_byte_length = 7;
    lengths.digital_size_length = 12;
    lengths.digital_sha1_checksum_length = 40;

    lengths.visual_id_length = 1;
    lengths.visual_layout_id_length = 1;
    lengths.visual_start_frame_length = 3;
    lengths.visual_start_section_length = 1;
    lengths.visual_section_count_length = 3;
    lengths.visual_dimension_length = 11;
    lengths.visual_overlap_length = 5;

    lengths.visual_pages_count = 0;

    if (toc_files == NULL)
    {
        return lengths;
    }

    if (toc_files->tocs == NULL)
    {
        return lengths;
    }

    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_files->tocs, afs_toc_file *, i);

        if (toc_file == NULL)
        {
            continue;
        }

        SET_MAX_LENGTH(lengths.digital_id_length, get_digits_count(toc_file->id));
        SET_MAX_LENGTH(lengths.digital_start_frame_length, get_digits_count(toc_file->start_frame));
        SET_MAX_LENGTH(lengths.digital_start_byte_length, get_digits_count(toc_file->start_byte));
        SET_MAX_LENGTH(lengths.digital_end_frame_length, get_digits_count(toc_file->end_frame));
        SET_MAX_LENGTH(lengths.digital_end_byte_length, get_digits_count(toc_file->end_byte));
        SET_MAX_LENGTH(lengths.digital_size_length, get_digits_count_64(toc_file->size));
        SET_MAX_LENGTH(lengths.digital_sha1_checksum_length, (unsigned int)strlen(toc_file->checksum));

        afs_toc_file_preview * toc_file_preview = toc_file->preview;

        if (toc_file_preview == NULL)
        {
            continue;
        }

        if (toc_file_preview->pages == NULL)
        {
            continue;
        }

        for (unsigned int j = 0; j < toc_file_preview->pages->size; j++)
        {
            afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file_preview, j);

            if (toc_file_preview_page == NULL)
            {
                continue;
            }

            lengths.visual_pages_count++;

            SET_MAX_LENGTH(lengths.visual_id_length, get_digits_count(toc_file->id));
            SET_MAX_LENGTH(lengths.visual_layout_id_length, (unsigned int)strlen(toc_file_preview_page->layout_id));
            SET_MAX_LENGTH(lengths.visual_start_frame_length, get_unsigned_digits_count(toc_file_preview_page->start_frame));
            SET_MAX_LENGTH(lengths.visual_start_section_length, get_unsigned_digits_count(toc_file_preview_page->start_section));
            SET_MAX_LENGTH(lengths.visual_section_count_length, get_unsigned_digits_count(toc_file_preview_page->section_count));
            unsigned int current_visual_dimension_length = get_unsigned_digits_count(toc_file_preview_page->dimension_x) + get_unsigned_digits_count(toc_file_preview_page->dimension_y) + 1;
            SET_MAX_LENGTH(lengths.visual_dimension_length, current_visual_dimension_length);
            unsigned int current_visual_overlap_length = get_unsigned_digits_count(toc_file_preview_page->overlap_x) + get_unsigned_digits_count(toc_file_preview_page->overlap_y) + 1;
            SET_MAX_LENGTH(lengths.visual_overlap_length, current_visual_overlap_length);
        }
    }

    return lengths;
}


static toc_files_metadata_lengths get_max_toc_files_metadata_lenght(const afs_toc_files * toc_files)
{
    toc_files_metadata_lengths lengths;

    lengths.file_id_length = 0;
    lengths.id_length = 0;
    lengths.source_file_id_length = 0;
    lengths.source_id_length = 0;
    lengths.source_format_id_length = 0;
    lengths.source_data_length = 0;

    lengths.metadata_sources_count = 0;

    if (toc_files == NULL)
    {
        return lengths;
    }

    if (toc_files->tocs == NULL)
    {
        return lengths;
    }

    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(toc_files->tocs, afs_toc_file *, i);

        if (toc_file == NULL)
        {
            continue;
        }

        if (afs_toc_file_has_metadata(toc_file) == DTRUE)
        {
            afs_toc_data_file_metadata * toc_data_file_metadata = toc_file->metadata;

            unsigned int sources_count = afs_toc_data_file_metadata_get_sources_count(toc_data_file_metadata);
            for (unsigned int j = 0; j < sources_count; j++)
            {
                afs_toc_data_file_metadata_source * metadata_source = GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, j);

                if (metadata_source == NULL)
                {
                    continue;
                }

                lengths.metadata_sources_count++;

                unsigned int current_file_id_length = get_digits_count(toc_file->id);
                SET_MAX_LENGTH(lengths.file_id_length, current_file_id_length);

                unsigned int current_id_length = get_digits_count(j);
                SET_MAX_LENGTH(lengths.id_length, current_id_length);

                unsigned int current_source_file_id_length = get_digits_count(metadata_source->file_id);
                SET_MAX_LENGTH(lengths.source_file_id_length, current_source_file_id_length);

                unsigned int current_source_id_length = get_digits_count(metadata_source->source_id);
                SET_MAX_LENGTH(lengths.source_id_length, current_source_id_length);

                unsigned int current_source_format_id_length = (unsigned int)strlen(metadata_source->format_id);
                SET_MAX_LENGTH(lengths.source_format_id_length, current_source_format_id_length);

                unsigned int current_source_data_length = (unsigned int)strlen(metadata_source->data);
                SET_MAX_LENGTH(lengths.source_data_length, current_source_data_length);
            }
        }
    }

    return lengths;
}


static unsigned int get_digits_count(int number)
{
    unsigned int digits_count = number / 10 != 0 ? 1 + get_digits_count(number / 10) : 1;
    if (number < 0 && digits_count == 1)
    {
        digits_count++;
    }
    return digits_count;
}


static unsigned int get_unsigned_digits_count(unsigned int number)
{
    unsigned int digits_count = number / 10 != 0 ? 1 + get_unsigned_digits_count(number / 10) : 1;
    return digits_count;
}


static unsigned int get_digits_count_64(boxing_int64 number)
{
    unsigned int digits_count = number / 10 != 0 ? 1 + get_digits_count_64(number / 10) : 1;
    if (number < 0)
    {
        digits_count++;
    }
    return digits_count;
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

    if (strcmp("files", name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (strcmp("file", name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (strcmp("file", parent_name) == 0 && strcmp("data", name) != 0 && strcmp("preview", name) != 0)
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n        ");
        }

        if (where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }

        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n            ");
        }
    }

    if (strcmp("data", name) == 0 || strcmp("preview", name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (strcmp("data", parent_name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }
        if (where == MXML_WS_AFTER_OPEN && strcmp("start", name) != 0 && strcmp("end", name) != 0)
        {
            return ("\n                ");
        }
    }

    if (strcmp("preview", parent_name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n            ");
        }
    }

    return (NULL);
}

void string_copy_strip_newline( char* destination, const char* source, unsigned int destination_length )
{
    unsigned int count = 0;
    while ( *source && count < (destination_length-1) )
    {
        if ( *source == '\n' || *source == '\r' || *source == '\t' )
        {
            source++;
        }
        else
        {
            *destination++ = *source++;
            count++;
        }
    }
    *destination = '\0';
}
