/*****************************************************************************
**
**  Definition of the toc data file interface
**
**  Creation date:  2017/02/22
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

#include "tocfile.h"
#include "xmlutils.h"
#include "platform/io.h"
#include <stdlib.h>

//  DEFINES
//

// PRIVATE INTERFACE
//

static DBOOL        save_data_v1(afs_toc_file * data_file, mxml_node_t* node);
static DBOOL        save_data_v2(afs_toc_file * data_file, mxml_node_t* node);
static const char * whitespace_cb(mxml_node_t *node, int where);


/*! 
  * \addtogroup file
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \enum   afs_toc_file_type tocfile.h
 *  \brief  File type constants.
 *  
 *  \param AFS_TOC_FILE_TYPE_UNDEFINED  (0x0) Undefined file type.
 *  \param AFS_TOC_FILE_TYPE_DIGITAL    (0x1) Digital file type.
 *  \param AFS_TOC_FILE_TYPE_PREVIEW    (0x2) Preview file type.
 *  \param AFS_TOC_FILE_TYPE_ALL        (0x3) Digital and preview file type.
 *
 *  File type contants. A file can be both digital and preview.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  AFS_TOC_FILE_PARENT
 *  \brief     Parent file.
 *  
 *  A parent file has a collection of files inside it, fex a tar archve 
 *  is a parent file with one or more tar items inside.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  AFS_TOC_FILE_NO_PARENT
 *  \brief     File is not a parent.
 *  
 *  The file is not a parent file.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  AFS_TOC_FILE_ID_UNDEFINED
 *  \brief     The file ID is not set.
 *  
 *  File IS is undefined.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_file_s  tocfile.h
 *  \brief      TOC file entry.
 *
 *  \param name         File name.
 *  \param date         File creation date.
 *  \param checksum     File checksum.
 *  \param size         File size in bytes.
 *  \param id           File id.
 *  \param start_frame  Start frame index.
 *  \param start_byte   Start byte relative to the start frame.
 *  \param end_frame    End frame index.
 *  \param end_byte     End byte relative to the enf frame.
 *  \param unique_id    Unique id.
 *  \param parent_id    Parent file id.
 *  \param types        File type.
 *  \param preview      Pointer to afs_toc_file_preview structure.
 *  \param file_format  File format.
 *  \param metadata     Pointer to toc_data_file_metadata instance.
 *
 *  The structure defines a file element in the TOC. The file entry points
 *  to the location (both digital and/or preview) of the file on the reel, 
 *  and it has techincal file metadata, like file name, size on bytes etc.
 */


// PUBLIC AFS TOC DATA FILE FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Return new toc data file instance.
 *
 *  Allocate memory for the afs_toc_file type
 *  and initializes all values to 0 or undefined.
 *
 *  \return instance of allocated afs_toc_file structure.
 */

afs_toc_file* afs_toc_file_create()
{
    afs_toc_file * toc_data_file = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_file);
    afs_toc_file_init(toc_data_file);
    return toc_data_file;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc data file instance.
 *
 *  Allocate memory for the afs_toc_file type and initialize members.
 *
 *  \param[in] name         Name string.
 *  \param[in] date         Date string.
 *  \param[in] checksum     Checksum string.
 *  \param[in] size         Size value.
 *  \param[in] id           Id value.
 *  \param[in] start_frame  Start frame value.
 *  \param[in] start_byte   Start byte value.
 *  \param[in] end_frame    End frame value.
 *  \param[in] end_byte     End byte value.
 *  \param[in] unique_id    Unique id string.
 *  \param[in] parent_id    Parent id value.
 *  \param[in] types        Types value.
 *  \param[in] preview      afs_toc_file_preview structure instance.
 *  \param[in] file_format  File format string.
 *  \param[in] metadata     afs_toc_data_file_metadata structure instance.
 *  \return instance of allocated afs_toc_file structure.
 */

afs_toc_file* afs_toc_file_create2(
    const char *                 name,
    const char *                 date,
    const char *                 checksum,
    boxing_int64                 size,
    int                          id,
    int                          start_frame,
    int                          start_byte,
    int                          end_frame,
    int                          end_byte,
    const char *                 unique_id,
    int                          parent_id,
    int                          types,
    afs_toc_file_preview *       preview,
    const char *                 file_format,
    afs_toc_data_file_metadata * metadata)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    afs_toc_file_init2(
        toc_data_file,
        name,
        date,
        checksum,
        size,
        id,
        start_frame,
        start_byte,
        end_frame,
        end_byte,
        unique_id,
        parent_id,
        types,
        preview,
        file_format,
        metadata);
    return toc_data_file;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all members to 0 / NULL.
 *
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  toc_data_file  Pointer to the afs_toc_file structure.
 */

void afs_toc_file_init(afs_toc_file * toc_data_file)
{
    if (toc_data_file == NULL)
    {
        return;
    }

    toc_data_file->name = NULL;
    toc_data_file->date = NULL;
    toc_data_file->checksum = NULL;
    toc_data_file->unique_id = NULL;
    toc_data_file->preview = NULL;
    toc_data_file->file_format = NULL;
    toc_data_file->metadata = NULL;

    toc_data_file->size = 0;
    toc_data_file->id = AFS_TOC_FILE_ID_UNDEFINED;
    toc_data_file->start_frame = -1;
    toc_data_file->start_byte = -1;
    toc_data_file->end_frame = -1;
    toc_data_file->end_byte = -1;
    toc_data_file->parent_id = AFS_TOC_FILE_NO_PARENT;
    toc_data_file->types = AFS_TOC_FILE_TYPE_UNDEFINED;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers.
 *
 *  Initializes all input structure pointers with specified input values.
 *  If one of the input pointers is NULL, then return without initialization.
 *
 *  \param[in]  toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in] name            Name string.
 *  \param[in] date            Date string.
 *  \param[in] checksum        Checksum string.
 *  \param[in] size            Size value.
 *  \param[in] id              Id value.
 *  \param[in] start_frame     Start frame value.
 *  \param[in] start_byte      Start byte value.
 *  \param[in] end_frame       End frame value.
 *  \param[in] end_byte        End byte value.
 *  \param[in] unique_id       Unique id string.
 *  \param[in] parent_id       Parent id value.
 *  \param[in] types           Types value.
 *  \param[in] preview         afs_toc_file_preview structure instance.
 *  \param[in] file_format     File format string.
 *  \param[in] metadata        afs_toc_data_file_metadata structure instance.
 */

void afs_toc_file_init2(
    afs_toc_file *               toc_data_file,
    const char *                 name,
    const char *                 date,
    const char *                 checksum,
    boxing_int64                 size,
    int                          id,
    int                          start_frame,
    int                          start_byte,
    int                          end_frame,
    int                          end_byte,
    const char *                 unique_id,
    int                          parent_id,
    int                          types,
    afs_toc_file_preview *       preview,
    const char *                 file_format,
    afs_toc_data_file_metadata * metadata)
{
    if (toc_data_file == NULL)
    {
        return;
    }

    toc_data_file->name = boxing_string_clone(name);
    toc_data_file->date = boxing_string_clone(date);
    toc_data_file->checksum = boxing_string_clone(checksum);
    toc_data_file->size = size;
    toc_data_file->id = id;
    toc_data_file->start_frame = start_frame;
    toc_data_file->start_byte = start_byte;
    toc_data_file->end_frame = end_frame;
    toc_data_file->end_byte = end_byte;
    toc_data_file->unique_id = boxing_string_clone(unique_id);
    toc_data_file->parent_id = parent_id;
    toc_data_file->types = types;
    toc_data_file->preview = preview;
    toc_data_file->file_format = boxing_string_clone(file_format);
    toc_data_file->metadata = metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Free afs_toc_file instance.
 *
 *  Frees all memory used by toc file instance and instance itself.
 *
 *  \param[in]  toc_data_file  Pointer to the afs_toc_file structure.
 */

void afs_toc_file_free(afs_toc_file* toc_data_file)
{
    if (toc_data_file == NULL)
    {
        return;
    }

    boxing_string_free(toc_data_file->name);
    boxing_string_free(toc_data_file->date);
    boxing_string_free(toc_data_file->checksum);
    boxing_string_free(toc_data_file->unique_id);
    afs_toc_file_preview_free(toc_data_file->preview);
    boxing_string_free(toc_data_file->file_format);
    afs_toc_data_file_metadata_free(toc_data_file->metadata);
    boxing_memory_free(toc_data_file);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Clone afs_toc_file instance.
 *
 *  Create a copy of input afs_toc_file structure and return it.
 *  If toc_data_file is NULL functions return NULL.
 *
 *  \param[in]  toc_data_file  Pointer to the afs_toc_file structure.
 *  \return new copy of toc_data_file structure or NULL.
 */

afs_toc_file * afs_toc_file_clone(afs_toc_file * toc_data_file)
{
    // If TOC data file pointer is NULL return NULL.
    if (toc_data_file == NULL)
    {
        return NULL;
    }

    return afs_toc_file_create2(
        toc_data_file->name,
        toc_data_file->date,
        toc_data_file->checksum,
        toc_data_file->size,
        toc_data_file->id,
        toc_data_file->start_frame,
        toc_data_file->start_byte,
        toc_data_file->end_frame,
        toc_data_file->end_byte,
        toc_data_file->unique_id,
        toc_data_file->parent_id,
        toc_data_file->types,
        afs_toc_file_preview_clone(toc_data_file->preview),
        toc_data_file->file_format,
        afs_toc_data_file_metadata_clone(toc_data_file->metadata));
}


//----------------------------------------------------------------------------
/*!
 *  \brief Compare toc data file instances
 *
 *  Compare toc data file instances content.
 *
 *  \param[in]   toc_data_file1  Pointer to the first instance
 *  \param[in]   toc_data_file2  Pointer to the second instance
 *  \return DTRUE if instances has equal content.
 */

DBOOL afs_toc_file_equal(afs_toc_file * toc_data_file1, afs_toc_file * toc_data_file2)
{
    if (toc_data_file1 == NULL && toc_data_file2 == NULL)
    {
        return DTRUE;
    }

    if (toc_data_file1 == NULL || toc_data_file2 == NULL)
    {
        return DFALSE;
    }

    if (boxing_string_equal(toc_data_file1->name, toc_data_file2->name) &&
        boxing_string_equal(toc_data_file1->date, toc_data_file2->date) &&
        boxing_string_equal(toc_data_file1->checksum, toc_data_file2->checksum) &&
        toc_data_file1->size == toc_data_file2->size &&
        toc_data_file1->id == toc_data_file2->id &&
        toc_data_file1->start_frame == toc_data_file2->start_frame &&
        toc_data_file1->start_byte == toc_data_file2->start_byte &&
        toc_data_file1->end_frame == toc_data_file2->end_frame &&
        toc_data_file1->end_byte == toc_data_file2->end_byte &&
        boxing_string_equal(toc_data_file1->unique_id, toc_data_file2->unique_id) &&
        toc_data_file1->parent_id == toc_data_file2->parent_id &&
        toc_data_file1->types == toc_data_file2->types &&
        afs_toc_file_preview_equal(toc_data_file1->preview, toc_data_file2->preview) &&
        boxing_string_equal(toc_data_file1->file_format, toc_data_file2->file_format) &&
        afs_toc_data_file_metadata_equal(toc_data_file1->metadata, toc_data_file2->metadata))
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Check for inequality
 *
 *  Compare toc data file instances content.
 *
 *  \param[in]   toc_data_file1  Pointer to the first instance
 *  \param[in]   toc_data_file2  Pointer to the second instance
 *  \return DTRUE if instances has different content.
 */

DBOOL afs_toc_file_not_equal(afs_toc_file * toc_data_file1, afs_toc_file * toc_data_file2)
{
    return afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE ? DFALSE : DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Add frame offset
 *
 *  Update reel position of digital file with a frame offset.
 *
 *  \param[in]   toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in]   offset         Offset in frames.
 */

void afs_toc_file_add_frame_offset(afs_toc_file * toc_data_file, unsigned int offset)
{
    // If TOC data file pointer is NULL return
    if (toc_data_file == NULL)
    {
        return;
    }

    if (toc_data_file->start_frame < 0 || toc_data_file->end_frame < 0)
    {
        return;
    }

    toc_data_file->start_frame += offset;
    toc_data_file->end_frame += offset;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Update file type
 *
 *  Adds a new type to the TOC data file structure.
 *
 *  \param[in]   toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in]   type           Type to be added.
 */

void afs_toc_file_add_type(afs_toc_file * toc_data_file, unsigned int type)
{
    // If TOC data file pointer is NULL return
    if (toc_data_file == NULL)
    {
        return;
    }

    toc_data_file->types |= type;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Set file name
 *
 *  Sets a new file name.
 *
 *  \param[in] toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in] name           Name string.
 */

void afs_toc_file_set_name(afs_toc_file * toc_data_file, const char * name)
{
    // If TOC data file pointer is NULL return
    if (toc_data_file == NULL)
    {
        return;
    }

    if (toc_data_file->name != NULL)
    {
        boxing_string_free(toc_data_file->name);
    }

    toc_data_file->name = boxing_string_clone(name);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Set date.
 *
 *  Sets a new date.
 *
 *  \param[in] toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in] date           Date string.
 */

void afs_toc_file_set_date(afs_toc_file * toc_data_file, const char * date)
{
    // If TOC data file pointer is NULL return
    if (toc_data_file == NULL)
    {
        return;
    }

    if (toc_data_file->date != NULL)
    {
        boxing_string_free(toc_data_file->date);
    }

    toc_data_file->date = boxing_string_clone(date);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Sets checksum
 *
 *  Sets a new checksum.
 *
 *  \param[in] toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in] checksum       Checksum string.
 */

void afs_toc_file_set_checksum(afs_toc_file * toc_data_file, const char * checksum)
{
    // If TOC data file pointer is NULL return
    if (toc_data_file == NULL)
    {
        return;
    }

    if (toc_data_file->checksum != NULL)
    {
        boxing_string_free(toc_data_file->checksum);
    }

    toc_data_file->checksum = boxing_string_clone(checksum);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Set unique ID
 *
 *  Set new unique ID.
 *
 *  \param[in] toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in] unique_id      Unique ID string.
 */

void afs_toc_file_set_unique_id(afs_toc_file * toc_data_file, const char * unique_id)
{
    // If TOC data file pointer is NULL return
    if (toc_data_file == NULL)
    {
        return;
    }

    if (toc_data_file->unique_id != NULL)
    {
        boxing_string_free(toc_data_file->unique_id);
    }

    toc_data_file->unique_id = boxing_string_clone(unique_id);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Set file format
 *
 *  Set new file format.
 *
 *  \param[in] toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in] file_format    File format string.
 */

void afs_toc_file_set_file_format(afs_toc_file * toc_data_file, const char * file_format)
{
    // If TOC data file pointer is NULL return
    if (toc_data_file == NULL)
    {
        return;
    }

    if (toc_data_file->file_format != NULL)
    {
        boxing_string_free(toc_data_file->file_format);
    }

    toc_data_file->file_format = boxing_string_clone(file_format);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Set preview
 *
 *  Sets a new afs_toc_file_preview. Note: Does not take ownership of 
 *  preview pointer.
 *
 *  \param[in] toc_data_file     Pointer to the afs_toc_file structure.
 *  \param[in] toc_file_preview  New afs_toc_file_preview instance.
 */

void afs_toc_file_set_preview(afs_toc_file * toc_data_file, const afs_toc_file_preview * toc_file_preview)
{
    // If TOC data file pointer is NULL return
    if (toc_data_file == NULL)
    {
        return;
    }

    if (toc_data_file->preview != NULL)
    {
        afs_toc_file_preview_free(toc_data_file->preview);
    }

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_PREVIEW);
    toc_data_file->preview = afs_toc_file_preview_clone(toc_file_preview);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Set metadata
 *
 *  Set new metadata. Note: Does not take ownership of metadata pointer.
 *
 *  \param[in] toc_data_file           Pointer to the afs_toc_file structure.
 *  \param[in] toc_data_file_metadata  New afs_toc_data_file_metadata instance.
 */

void afs_toc_file_set_metadata(afs_toc_file * toc_data_file, const afs_toc_data_file_metadata * toc_data_file_metadata)
{
    // If TOC data file pointer is NULL return
    if (toc_data_file == NULL)
    {
        return;
    }

    if (toc_data_file->metadata != NULL)
    {
        afs_toc_data_file_metadata_free(toc_data_file->metadata);
    }

    toc_data_file->metadata = afs_toc_data_file_metadata_clone(toc_data_file_metadata);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Load from XML file.
 *
 *  Load file from XML.
 *
 *  \param[out]  toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in]   file_name      Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_load_file(afs_toc_file * toc_data_file, const char * file_name)
{
    // If input file name string pointer is NULL or TOC data file pointer is NULL return DFALSE
    if (file_name == NULL || toc_data_file == NULL)
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

    DBOOL return_value = afs_toc_file_load_xml(toc_data_file, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Load XML from string
 *
 *  Load from XML string.
 *
 *  \param[out]  toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in]   in             Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_load_string(afs_toc_file * toc_data_file, const char * in)
{
    // If input string pointer is NULL or TOC data file pointer is NULL return DFALSE
    if (in == NULL|| boxing_string_equal(in, "") || toc_data_file == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    DBOOL return_value = afs_toc_file_load_xml(toc_data_file, document);

    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Load XML from tree node.
 *
 *  Load XML from XML parser tree node.
 *
 *  \param[out]  toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in]   node           Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_load_xml(afs_toc_file * toc_data_file, mxml_node_t* node)
{
    // If input node pointer is NULL or TOC data file pointer is NULL return DFALSE
    if (node == NULL || toc_data_file == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * file_node = NULL;
    
    file_node = boxing_string_equal(mxmlGetElement(node), "file") == DTRUE ? node : mxmlFindElement(node, node, "file", NULL, NULL, MXML_DESCEND);

    if (file_node == NULL)
    {
        return DFALSE;
    }
    toc_data_file->file_format = boxing_string_clone(mxmlElementGetAttr(file_node, "formatId"));

    mxml_node_t * name_node = mxmlFindElement(file_node, file_node, "name", NULL, NULL, MXML_DESCEND);
    if (name_node != NULL)
    {
        toc_data_file->name = afs_xmlutils_get_node_text(name_node);
    }

    afs_xmlutils_get_integer_element(&toc_data_file->id, file_node, "id");

    mxml_node_t * unique_id_node = mxmlFindElement(file_node, file_node, "uniqueId", NULL, NULL, MXML_DESCEND);
    if (unique_id_node != NULL)
    {
        toc_data_file->unique_id = afs_xmlutils_get_node_text(unique_id_node);
    }

    mxml_node_t * date_node = mxmlFindElement(file_node, file_node, "date", NULL, NULL, MXML_DESCEND);
    if (date_node != NULL)
    {
        toc_data_file->date = afs_xmlutils_get_node_text(date_node);
    }

    if (!afs_xmlutils_get_integer_element(&toc_data_file->parent_id, file_node, "parentId") && !afs_xmlutils_get_integer_element(&toc_data_file->parent_id, file_node, "parentid"))
    {
        toc_data_file->parent_id = AFS_TOC_FILE_NO_PARENT;
    }

    mxml_node_t * preview_node = mxmlFindElement(file_node, file_node, "preview", NULL, NULL, MXML_DESCEND);
    if (preview_node != NULL)
    {
        afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_PREVIEW);

        afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
        DBOOL result = afs_toc_file_preview_load_xml(toc_file_preview, preview_node);

        if (!result)
        {
            afs_toc_file_preview_free(toc_file_preview);
            return DFALSE;
        }

        toc_data_file->preview = toc_file_preview;
    }

    mxml_node_t * metadata_node = mxmlFindElement(file_node, file_node, "metadata", NULL, NULL, MXML_DESCEND);
    if (metadata_node != NULL)
    {
        afs_toc_data_file_metadata * toc_data_file_metadata = afs_toc_data_file_metadata_create();
        DBOOL result = afs_toc_data_file_metadata_load_xml(toc_data_file_metadata, metadata_node);

        if (!result)
        {
            afs_toc_data_file_metadata_free(toc_data_file_metadata);
            return DFALSE;
        }

        toc_data_file->metadata = toc_data_file_metadata;
    }

    mxml_node_t * data_node = NULL;

    for (data_node = mxmlFindElement(file_node, file_node, "data", NULL, NULL, MXML_DESCEND);
         data_node != NULL;
         data_node = mxmlFindElement(data_node, file_node, "data", NULL, NULL, MXML_DESCEND))
    {
        mxml_node_t * parent_node = mxmlGetParent(data_node);
        const char * parent_name = mxmlGetElement(parent_node);
        if (boxing_string_equal(parent_name, "file"))
        {
            break;
        }
    }

    if (data_node != NULL)
    {
        if (!afs_toc_file_load_data_v2(toc_data_file, data_node))
        {
            return DFALSE;
        }

        afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_DIGITAL);
    }
    else
    {
        if (!afs_toc_file_load_data_v1(toc_data_file, file_node))
        {
            return DFALSE;
        }

        if (mxmlFindElement(file_node, file_node, "size", NULL, NULL, MXML_DESCEND) != NULL ||
            mxmlFindElement(file_node, file_node, "start", NULL, NULL, MXML_DESCEND) != NULL ||
            mxmlFindElement(file_node, file_node, "end", NULL, NULL, MXML_DESCEND) != NULL)
        {
            afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_DIGITAL);
        }

        // As of TOC version 1.0.2 a file can be present without having a type.
        // The reason is that we may want to preserve the file tree structure
        // even when digital data is excluded.
        if (afs_toc_file_is_valid(toc_data_file) == DFALSE)
        {
            toc_data_file->types = AFS_TOC_FILE_TYPE_UNDEFINED;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Load V1 of data section.
 *
 *  Read version one of digital datablock. Early TOCs where 'data' tag is missing and digital attributes direct childs of file tag
 *
 *  \param[out]  toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in]   node           Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_load_data_v1(afs_toc_file * toc_data_file, mxml_node_t* node)
{
    // If input node pointer is NULL or TOC data file pointer is NULL return DFALSE
    if (node == NULL || toc_data_file == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * size_node = mxmlFindElement(node, node, "size", NULL, NULL, MXML_DESCEND);
    if (size_node != NULL)
    {
        char * node_text = afs_xmlutils_get_node_text(size_node);
        toc_data_file->size = STRING_TO_LONG_INT(node_text);
        boxing_string_free(node_text);
    }

    mxml_node_t * checksum_node = mxmlFindElement(node, node, "checksum", NULL, NULL, MXML_DESCEND);
    if (checksum_node != NULL)
    {
        toc_data_file->checksum = afs_xmlutils_get_node_text(checksum_node);
    }
    
    mxml_node_t * start_node = mxmlFindElement(node, node, "start", NULL, NULL, MXML_DESCEND);
    if (start_node != NULL)
    {
        afs_xmlutils_get_integer_element(&toc_data_file->start_frame, start_node, "frame");
        afs_xmlutils_get_integer_element(&toc_data_file->start_byte, start_node, "byte");
    }

    mxml_node_t * end_node = mxmlFindElement(node, node, "end", NULL, NULL, MXML_DESCEND);
    if (end_node != NULL)
    {
        afs_xmlutils_get_integer_element(&toc_data_file->end_frame, end_node, "frame");
        afs_xmlutils_get_integer_element(&toc_data_file->end_byte, end_node, "byte");
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Load V2 of digital data block.
 *
 *  Load version two of digital datablock. Digital attributes childs of data tag.
 *
 *  \param[out]  toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in]   node           Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_load_data_v2(afs_toc_file * toc_data_file, mxml_node_t* node)
{
    // If input node pointer is NULL or TOC data file pointer is NULL return DFALSE
    if (node == NULL || toc_data_file == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * size_node = mxmlFindElement(node, node, "size", NULL, NULL, MXML_DESCEND);
    if (size_node != NULL)
    {
        char * node_text = afs_xmlutils_get_node_text(size_node);
        toc_data_file->size = STRING_TO_LONG_INT(node_text);
        boxing_string_free(node_text);
    }

    mxml_node_t * checksum_node = mxmlFindElement(node, node, "checksum", NULL, NULL, MXML_DESCEND);
    if (checksum_node != NULL)
    {
        toc_data_file->checksum = afs_xmlutils_get_node_text(checksum_node);
    }

    mxml_node_t * start_node = mxmlFindElement(node, node, "start", NULL, NULL, MXML_DESCEND);
    if (start_node != NULL)
    {
        afs_xmlutils_get_integer_attribute(&toc_data_file->start_frame, start_node, "frame");
        afs_xmlutils_get_integer_attribute(&toc_data_file->start_byte, start_node, "byte");
    }

    mxml_node_t * end_node = mxmlFindElement(node, node, "end", NULL, NULL, MXML_DESCEND);
    if (end_node != NULL)
    {
        afs_xmlutils_get_integer_attribute(&toc_data_file->end_frame, end_node, "frame");
        afs_xmlutils_get_integer_attribute(&toc_data_file->end_byte, end_node, "byte");
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Save to XML tree node.
 *
 *  Save to XML tree node.
 *
 *  \param[in]   toc_data_file   Pointer to the afs_toc_file structure.
 *  \param[out]  out             Pointer to the XML node.
 *  \param[in]   data_v1         If DTRUE use V1 of digital data block.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_save_xml(afs_toc_file * toc_data_file, mxml_node_t* out, DBOOL data_v1)
{
    // If output node pointer is NULL or TOC data file pointer is NULL or TOC data file instance is not valid return DFALSE
    if (out == NULL || toc_data_file == NULL || afs_toc_file_is_valid(toc_data_file) == DFALSE)
    {
        return DFALSE;
    }

    mxml_node_t *file_node = mxmlNewElement(out, "file");

    if (toc_data_file->file_format != NULL)
    {
        mxmlElementSetAttr(file_node, "formatId", toc_data_file->file_format);
    }

    afs_xmlutils_add_new_int_node(file_node, "id", toc_data_file->id);
    afs_xmlutils_add_new_text_node(file_node, "uniqueId", toc_data_file->unique_id);
    afs_xmlutils_add_new_text_node(file_node, "name", toc_data_file->name);
    afs_xmlutils_add_new_text_node(file_node, "date", toc_data_file->date);

    if (afs_toc_file_has_parent(toc_data_file) || afs_toc_file_is_parent(toc_data_file))
    {
        afs_xmlutils_add_new_int_node(file_node, "parentId", toc_data_file->parent_id);
    }

    if (afs_toc_file_is_digital(toc_data_file))
    {
        if (data_v1)
        {
            if (!save_data_v1(toc_data_file, file_node))
            {
                return DFALSE;
            }
        }
        else
        {
            if (!save_data_v2(toc_data_file, file_node))
            {
                return DFALSE;
            }
        }
    }

    // Preview section
    if (afs_toc_file_is_preview(toc_data_file))
    {
        afs_toc_file_preview_save_xml(toc_data_file->preview, file_node);
    }

    // Metadata section
    if (afs_toc_file_has_metadata(toc_data_file))
    {
        afs_toc_data_file_metadata_save_xml(toc_data_file->metadata, file_node);
    }
    
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Save XML to string.
 *
 *  Save XML to string.
 *
 *  \param[in]   toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in]   compact        If DTRUE write unformatted XML (no whitespace).
 *  \param[in]   data_v1        If DTRUE use V1 of the digital data block.
 *  \return resulting string or NULL.
 */

char * afs_toc_file_save_string(afs_toc_file * toc_data_file, DBOOL compact, DBOOL data_v1)
{
    // If TOC data file pointer is NULL return DFALSE
    if (toc_data_file == NULL)
    {
        return DFALSE;
    }

    struct mxml_node_s * document = mxmlNewXML("1.0");

    if (!afs_toc_file_save_xml(toc_data_file, document, data_v1))
    {
        mxmlDelete(document);
        return NULL;
    }

    char * xmlString;
    mxmlSetWrapMargin(0);

    if (compact)
    {
        xmlString = mxmlSaveAllocString(document->child, MXML_NO_CALLBACK);
    }
    else
    {
        xmlString = mxmlSaveAllocString(document->child, whitespace_cb);
    }

    mxmlDelete(document);

    return xmlString;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Save XML to file.
 *
 *  Write XML to file.
 *
 *  \param[in]   file_name      Name of the XML file.
 *  \param[in]   toc_data_file  Pointer to the afs_toc_file structure.
 *  \param[in]   compact        If DTRUE write unformatted XML (no whitespace).
 *  \param[in]   data_v1        If DTRUE use V1 of the digital data block.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_save_file(afs_toc_file * toc_data_file, const char * file_name, DBOOL compact, DBOOL data_v1)
{
    if (file_name == NULL || toc_data_file == NULL)
    {
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

    mxml_node_t *tree = mxmlNewXML("1.0");

    if (!afs_toc_file_save_xml(toc_data_file, tree, data_v1))
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
 *  \brief Check if file has a parent
 *
 *  Check if file has a parent.
 *
 *  \param[in]   toc_data_file   afs_toc_file pointer.
 *  \return DTRUE if parent.
 */

DBOOL afs_toc_file_has_parent(afs_toc_file * toc_data_file)
{
    // If TOC data file pointer is NULL return DFALSE
    if (toc_data_file == NULL)
    {
        return DFALSE;
    }

    if (toc_data_file->parent_id != AFS_TOC_FILE_NO_PARENT && !afs_toc_file_is_parent(toc_data_file))
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Check if standalone
 *
 *  A standalone file is not a parent file and not a child of a parent file.
 *
 *  \param[in]   toc_data_file   Pointer to the afs_toc_file structure.
 *  \return DTRUE if file is standalone.
 */

DBOOL afs_toc_file_is_standalone(afs_toc_file * toc_data_file)
{
    // If TOC data file pointer is NULL return DFALSE
    if (toc_data_file == NULL)
    {
        return DFALSE;
    }

    if (!afs_toc_file_is_parent(toc_data_file) && !afs_toc_file_has_parent(toc_data_file))
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Check if file is a parent
 *
 *  Check if file is of type parent.
 *
 *  \param[in]   toc_data_file   Pointer to the afs_toc_file structure.
 *  \return DTRUE if parent.
 */

DBOOL afs_toc_file_is_parent(const afs_toc_file * toc_data_file)
{
    // If TOC data file pointer is NULL return DFALSE
    if (toc_data_file == NULL)
    {
        return DFALSE;
    }

    if (toc_data_file->parent_id == AFS_TOC_FILE_PARENT)
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Check if file is digital
 *
 *  Check if file is of type digital.
 *
 *  \param[in]   toc_data_file   Pointer to the afs_toc_file structure.
 *  \return DTRUE if file is digital.
 */

DBOOL afs_toc_file_is_digital(const afs_toc_file * toc_data_file)
{
    // If TOC data file pointer is NULL return DFALSE
    if (toc_data_file == NULL)
    {
        return DFALSE;
    }

    if ((toc_data_file->types & AFS_TOC_FILE_TYPE_DIGITAL) != 0)
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Check if file has a preview.
 *
 *  Check if file has a preview.
 *
 *  \param[in]   toc_data_file   Pointer to the afs_toc_file structure.
 *  \return DTRUE if file has a preview.
 */

DBOOL afs_toc_file_is_preview(const afs_toc_file * toc_data_file)
{
    // If TOC data file pointer is NULL return DFALSE
    if (toc_data_file == NULL)
    {
        return DFALSE;
    }

    if ((toc_data_file->types & AFS_TOC_FILE_TYPE_PREVIEW) != 0)
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Check if file has metadata
 *
 *  Check if file has metadata.
 *
 *  \param[in]   toc_data_file   Pointer to the afs_toc_file structure.
 *  \return DTRUE if file has metadata
 */

DBOOL afs_toc_file_has_metadata(const afs_toc_file * toc_data_file)
{
    // If TOC data file pointer is NULL return DFALSE
    if (toc_data_file == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_data_file_metadata_get_sources_count(toc_data_file->metadata) != 0)
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Check if file is valid.
 *
 *  Check if the structure represents a valid file: File name is set, has 
 *  digital and/or preview or is a parent file, positiion on reel is valid, 
 *  etc.
 *
 *  \param[in]   toc_data_file   Pointer to the afs_toc_file structure.
 *  \return DTRUE if file is valid.
 */

DBOOL afs_toc_file_is_valid(const afs_toc_file * toc_data_file)
{
    // If TOC data file pointer is NULL return DFALSE
    if (toc_data_file == NULL)
    {
        return DFALSE;
    }

    // Invalid TOC file, is neither digital nor preview
    if (!afs_toc_file_is_digital(toc_data_file) && !afs_toc_file_is_preview(toc_data_file) && !afs_toc_file_is_parent(toc_data_file))
    {
        return DFALSE;
    }

    // General elements

    // Invalid TOC file, illegal id
    if (toc_data_file->id < -2)
    {
        return DFALSE;
    }

    // Invalid TOC file, incorrect general elements
    if (toc_data_file->name == NULL || boxing_string_equal(toc_data_file->name, "") || toc_data_file->date == NULL || boxing_string_equal(toc_data_file->date, ""))
    {
        return DFALSE;
    }

    // Digital elements

    // Invalid TOC digital elements
    if (afs_toc_file_is_digital(toc_data_file))
    {
        if (toc_data_file->start_frame < 0 ||
            toc_data_file->start_byte < 0 ||
            toc_data_file->end_frame < toc_data_file->start_frame ||
            toc_data_file->checksum == NULL || boxing_string_equal(toc_data_file->checksum, "") ||
            (toc_data_file->end_frame == toc_data_file->start_frame && toc_data_file->end_byte < toc_data_file->start_byte))
        {
            return DFALSE;
        }
    }

    // Preview elements

    // Invalid TOC preview elements
    if (afs_toc_file_is_preview(toc_data_file))
    {
        if (toc_data_file->preview == NULL || !afs_toc_file_preview_is_valid(toc_data_file->preview))
        {
            return DFALSE;
        }
    }

    // Invalid TOC metadata
    if (afs_toc_file_has_metadata(toc_data_file))
    {
        if (afs_toc_data_file_metadata_is_valid(toc_data_file->metadata) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Duration of digital frames
 *
 *  Return number of frames the digital version of the file occupies.
 *
 *  \param[in]   toc_file  Pointer to the instance of the afs_toc_file structure.
 *  \return duration frames.
 */

int afs_toc_file_get_duration_frames(afs_toc_file * toc_file)
{
    if (toc_file == NULL)
    {
        return 0;
    }

    return toc_file->end_frame - toc_file->start_frame + 1;
}


//----------------------------------------------------------------------------
/*!
  * \} end of file group
  */


// PRIVATE AFS TOC FILE FUNCTIONS
//

static DBOOL save_data_v1(afs_toc_file * toc_data_file, mxml_node_t* node)
{
    // If TOC data file pointer is NULL or XML node pointer is NULL return DFALSE
    if (toc_data_file == NULL || node == NULL)
    {
        return DFALSE;
    }

    afs_xmlutils_add_new_uint64_node(node, "size", toc_data_file->size);

    mxml_node_t *start_node = mxmlNewElement(node, "start");
    afs_xmlutils_add_new_int_node(start_node, "frame", toc_data_file->start_frame);
    afs_xmlutils_add_new_int_node(start_node, "byte", toc_data_file->start_byte);

    mxml_node_t *end_node = mxmlNewElement(node, "end");
    afs_xmlutils_add_new_int_node(end_node, "frame", toc_data_file->end_frame);
    afs_xmlutils_add_new_int_node(end_node, "byte", toc_data_file->end_byte);

    return DTRUE;
}


static DBOOL save_data_v2(afs_toc_file * toc_data_file, mxml_node_t* node)
{
    // If TOC data file pointer is NULL or XML node pointer is NULL return DFALSE
    if (toc_data_file == NULL || node == NULL)
    {
        return DFALSE;
    }

    mxml_node_t *data_node = mxmlNewElement(node, "data");
      
    afs_xmlutils_add_new_text_node(data_node, "checksum", toc_data_file->checksum);
    afs_xmlutils_add_new_uint64_node(data_node, "size", toc_data_file->size);

    mxml_node_t *start_node = mxmlNewElement(data_node, "start");
    mxmlElementSetAttrf(start_node, "frame", "%d", toc_data_file->start_frame);
    mxmlElementSetAttrf(start_node, "byte", "%d", toc_data_file->start_byte);

    mxml_node_t *end_node = mxmlNewElement(data_node, "end");
    mxmlElementSetAttrf(end_node, "frame", "%d", toc_data_file->end_frame);
    mxmlElementSetAttrf(end_node, "byte", "%d", toc_data_file->end_byte);

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

    if (boxing_string_equal("file", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("file", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("file", parent_name) && !boxing_string_equal("start", name) && !boxing_string_equal("end", name) && !boxing_string_equal("preview", name))
    {
        if (where == MXML_WS_AFTER_OPEN && !boxing_string_equal("data", name) && !boxing_string_equal("metadata", name))
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("start", parent_name) || boxing_string_equal("end", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }

        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n            ");
        }
    }

    if (boxing_string_equal("pages", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("data", parent_name))
    {
        if (boxing_string_equal("checksum", name) || boxing_string_equal("size", name) || boxing_string_equal("start", name) || boxing_string_equal("end", name))
        {
            if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
            {
                return ("\n        ");
            }

            if (where == MXML_WS_AFTER_OPEN && !boxing_string_equal("start", name) && !boxing_string_equal("end", name))
            {
                return ("\n            ");
            }
        }
    }

    if (boxing_string_equal("metadata", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("source", parent_name))
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

    return (NULL);
}
