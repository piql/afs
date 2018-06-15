/*****************************************************************************
**
**  Definition of the Technical Metadata interface
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
#include "technicalmetadata.h"
#include "boxing/platform/memory.h"


// PUBLIC AFS TECHNICAL METADATA FUNCTIONS
//


/*! 
  * \addtogroup cf
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_technical_metadata_s  technicalmetadata.h
 *  \brief      Technical metadata storage.
 *
 *  \param afs_tocs                   Pointer to the afs_toc_files structure.
 *  \param afs_content_boxing_format  Pointer to the afs_boxing_format structure.
 *
 *  Structure for storing technical metadata.
 */


//----------------------------------------------------------------------------
/*!
 *  \brief Create an technical metadata instance.
 *
 *  Allocate memory for the afs_technical_metadata type
 *  and initializes all structure pointers with NULL values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_technical_metadata structure.
 */

afs_technical_metadata* afs_technical_metadata_create()
{
    afs_technical_metadata* technical_metadata = BOXING_MEMORY_ALLOCATE_TYPE(afs_technical_metadata);
    afs_technical_metadata_init(technical_metadata);
    return technical_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers with NULL value.
 *
 *  Initializes all input structure pointers with NULL values.
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  technical_metadata  Pointer to the afs_technical_metadata structure.
 */

void afs_technical_metadata_init(afs_technical_metadata* technical_metadata)
{
    if (technical_metadata == NULL)
    {
        return;
    }

    technical_metadata->afs_content_boxing_format = NULL;
    technical_metadata->afs_tocs = NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers.
 * 
 *  Initializes all input structure pointers with specified values.
 *  If one of the input pointers is NULL, then return without initialization.
 *
 *  \param[in]  technical_metadata     Pointer to the afs_technical_metadata structure.
 *  \param[in]  tocs                   Pointer to the afs_toc_files structure.
 *  \param[in]  content_boxing_format  Pointer to the content_boxing_format structure.
 */

void afs_technical_metadata_init2(afs_technical_metadata * technical_metadata, afs_toc_files * tocs, afs_boxing_format * content_boxing_format)
{
    if (technical_metadata == NULL || tocs == NULL || content_boxing_format == NULL)
    {
        return;
    }

    technical_metadata->afs_tocs = tocs;
    technical_metadata->afs_content_boxing_format = content_boxing_format;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of technical metadata structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  technical_metadata  Pointer to the afs_technical_metadata structure.
 */

void afs_technical_metadata_free(afs_technical_metadata* technical_metadata)
{
    if (technical_metadata == NULL)
    {
        return;
    }

    afs_boxing_format_free(technical_metadata->afs_content_boxing_format);
    afs_toc_files_free(technical_metadata->afs_tocs);

    boxing_memory_free(technical_metadata);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_technical_metadata structure to the XML nodes.
 *
 *  Function translates the input afs_technical_metadata structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  out                 Pointer to the XML node.
 *  \param[in]   technical_metadata  Pointer to the afs_technical_metadata structure.
 *  \return DTRUE on success.
 */

DBOOL afs_technical_metadata_save_xml(mxml_node_t * out, afs_technical_metadata* technical_metadata)
{
    if (out == NULL || technical_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t *technical_metadata_node = mxmlNewElement(out, "TechnicalMetadata");

    mxml_node_t *boxer_format_node = mxmlNewElement(technical_metadata_node, "BoxerFormat");
    DBOOL result = afs_boxing_format_save_xml(boxer_format_node, technical_metadata->afs_content_boxing_format);

    if (result == DFALSE)
    {
        return DFALSE;
    }

    mxml_node_t *tocs_node = mxmlNewElement(technical_metadata_node, "Tocs");
    result = afs_toc_files_save_xml(technical_metadata->afs_tocs, tocs_node);

    return result;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_technical_metadata structure.
 *
 *  Function translates the input XML nodes to the afs_technical_metadata structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  technical_metadata  Pointer to the afs_technical_metadata structure.
 *  \param[in]   in                  Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_technical_metadata_load_xml(afs_technical_metadata* technical_metadata, mxml_node_t * in)
{
    // If input data pointer is NULL or technical metadata pointer is NULL return DFALSE
    if (in == NULL || technical_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * technical_metadata_node = mxmlFindElement(in, in, "TechnicalMetadata", NULL, NULL, MXML_DESCEND);

    if (technical_metadata_node == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * boxer_format_node = mxmlFindElement(technical_metadata_node, technical_metadata_node, "BoxerFormat", NULL, NULL, MXML_DESCEND);

    if (boxer_format_node == NULL)
    {
        return DFALSE;
    }

    technical_metadata->afs_content_boxing_format = afs_boxing_format_create();

    DBOOL load_boxing_format_resuls = afs_boxing_format_load_xml(technical_metadata->afs_content_boxing_format, boxer_format_node);

    if (load_boxing_format_resuls == DFALSE)
    {
        afs_boxing_format_free(technical_metadata->afs_content_boxing_format);
        technical_metadata->afs_content_boxing_format = NULL;
    }

    mxml_node_t * tocs_node = mxmlFindElement(technical_metadata_node, technical_metadata_node, "Tocs", NULL, NULL, MXML_DESCEND);
    
    if (tocs_node == NULL)
    {
        return DTRUE;
    }

    technical_metadata->afs_tocs = afs_toc_files_create();

    DBOOL load_tocs_result = afs_toc_files_load_xml(technical_metadata->afs_tocs, tocs_node);

    if (load_tocs_result == DFALSE)
    {
        afs_boxing_format_free(technical_metadata->afs_content_boxing_format);
        technical_metadata->afs_content_boxing_format = NULL;
        afs_toc_files_free(technical_metadata->afs_tocs);
        technical_metadata->afs_tocs = NULL;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of cf group
  */
