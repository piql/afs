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

// PRIVATE INTERFACE
//

static const char * whitespace_cb(mxml_node_t *node, int where);


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
 *  \param afs_tocs                   List of TOC locations on reel.
 *  \param afs_content_boxing_format  Boxing format used to unbox TOC and data files.
 *  \param afs_applications           List of application locations on reel.
 *
 *  The technical metadata part of the control data contains information that 
 *  can be used to unbox the rest of the reel. Most importantly, is points to the 
 *  location of the table-of-content frames on the reel. It also has a boxing frame 
 *  structure that should be used when unboxing the TOC and the digital data files 
 *  the TOC points to. 
 *
 *  The technical metadata also contains an optional list of locations that points 
 *  to an application that can be used to unbox the TOC and the content. The
 *  purpose for this is to provide a bootstrap mechanism that could be used to
 *  to restore the reel content. An example of this is the iVM virtual machine 
 *  implementation that is a virtual machine execution environment designed to 
 *  embedded on the reel. 
 *
 *  The afs_data_file.format should be used to identify the application format.
 */


//----------------------------------------------------------------------------
/*!
 *  \brief Create a technical metadata instance.
 *
 *  Allocate memory for the afs_technical_metadata type
 *  and initializes all structure pointers with NULL values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_technical_metadata structure.
 */

afs_technical_metadata * afs_technical_metadata_create()
{
    afs_technical_metadata * technical_metadata = BOXING_MEMORY_ALLOCATE_TYPE(afs_technical_metadata);

    technical_metadata->afs_content_boxing_format = NULL;
    technical_metadata->afs_tocs = NULL;
    technical_metadata->afs_applications = NULL;

    return technical_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a technical metadata instance.
 *
 *  Allocate memory for the afs_technical_metadata type
 *  and initialize tocs and boxing format with specified input value.
 *  Return instance of allocated structure.
 *
 *  \param[in]  tocs                   Pointer to the afs_toc_files structure.
 *  \param[in]  content_boxing_format  Pointer to the content_boxing_format structure.
 *  \return instance of allocated afs_technical_metadata structure.
 */

afs_technical_metadata * afs_technical_metadata_create2(afs_toc_files * tocs, afs_boxing_format * content_boxing_format)
{
    afs_technical_metadata * technical_metadata = BOXING_MEMORY_ALLOCATE_TYPE(afs_technical_metadata);

    technical_metadata->afs_tocs = tocs;
    technical_metadata->afs_content_boxing_format = content_boxing_format;
    technical_metadata->afs_applications = NULL;

    return technical_metadata;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a technical metadata instance.
 *
 *  Allocate memory for the afs_technical_metadata type
 *  and initialize tocs and boxing format with specified input value.
 *  Return instance of allocated structure.
 *
 *  \param[in]  tocs                   Pointer to the afs_toc_files structure.
 *  \param[in]  content_boxing_format  Pointer to the content_boxing_format structure.
 *  \param[in]  applications           List of application locations.
 *  \return instance of allocated afs_technical_metadata structure.
 */

afs_technical_metadata * afs_technical_metadata_create3(
    afs_toc_files * tocs,
    afs_boxing_format * content_boxing_format,
    afs_toc_file * applications )
{
    afs_technical_metadata * technical_metadata = BOXING_MEMORY_ALLOCATE_TYPE(afs_technical_metadata);

    technical_metadata->afs_tocs = tocs;
    technical_metadata->afs_content_boxing_format = content_boxing_format;
    technical_metadata->afs_applications = applications;

    return technical_metadata;
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
    afs_toc_files_free(technical_metadata->afs_applications);

    boxing_memory_free(technical_metadata);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_technical_metadata structure.
 *
 *  Function create a copy of input afs_technical_metadata structure and return it.
 *  If technical metadata structure pointer is NULL function return NULL.
 *
 *  \param[in]  technical_metadata  Pointer to the afs_technical_metadata structure.
 *  \return new copy of afs_technical_metadata structure or NULL.
 */

afs_technical_metadata * afs_technical_metadata_clone(const afs_technical_metadata * technical_metadata)
{
    if (technical_metadata == NULL)
    {
        return NULL;
    }

    afs_technical_metadata * clone = afs_technical_metadata_create();
    clone->afs_tocs = afs_toc_files_clone(technical_metadata->afs_tocs);
    clone->afs_content_boxing_format = technical_metadata->afs_content_boxing_format == NULL ? NULL : afs_boxing_format_create2(technical_metadata->afs_content_boxing_format->config);
    clone->afs_applications = afs_toc_files_clone(technical_metadata->afs_applications);

    return clone;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Compare technical metadata
 *
 *  Returns DTRUE if content is equal.
 *
 *  \param[in]   technical_metadata1  Pointer to the first instance of the afs_technical_metadata structure.
 *  \param[in]   technical_metadata2  Pointer to the second instance of the afs_technical_metadata structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_technical_metadata_equal(const afs_technical_metadata * technical_metadata1, const afs_technical_metadata * technical_metadata2)
{
    if (technical_metadata1 == NULL && technical_metadata2 == NULL)
    {
        return DTRUE;
    }

    if (technical_metadata1 == NULL || technical_metadata2 == NULL)
    {
        return DFALSE;
    }

    DBOOL boxing_format_is_equal = DFALSE;
    if (technical_metadata1->afs_content_boxing_format == NULL && technical_metadata2->afs_content_boxing_format == NULL)
    {
        boxing_format_is_equal = DTRUE;
    }
    else if (technical_metadata1->afs_content_boxing_format == NULL || technical_metadata2->afs_content_boxing_format == NULL)
    {
        boxing_format_is_equal = DFALSE;
    }
    else
    {
        boxing_format_is_equal = boxing_config_is_equal(technical_metadata1->afs_content_boxing_format->config, technical_metadata2->afs_content_boxing_format->config);
    }

    if (boxing_format_is_equal == DTRUE &&
        afs_toc_files_equal(technical_metadata1->afs_tocs, technical_metadata2->afs_tocs) &&
        afs_toc_files_equal(technical_metadata1->afs_applications, technical_metadata2->afs_applications) )
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Save as XML file
 *
 *  Function translates the input afs_technical_metadata structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   technical_metadata  Pointer to the afs_technical_metadata structure.
 *  \param[in]   file_name           Name of the XML file.
 *  \param[in]   compact             If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_technical_metadata_save_file(afs_technical_metadata * technical_metadata, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || technical_metadata == NULL)
    {
        return DFALSE;
    }

    if (technical_metadata->afs_tocs == NULL || technical_metadata->afs_content_boxing_format == NULL)
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

    if (!afs_technical_metadata_save_xml(technical_metadata, tree))
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
 *  \brief Save as XML string
 *
 *  Function translates the input afs_technical_metadata structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   technical_metadata  Pointer to the afs_technical_metadata structure.
 *  \param[in]   compact             If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_technical_metadata_save_string(afs_technical_metadata * technical_metadata, DBOOL compact)
{
    if (technical_metadata == NULL)
    {
        return NULL;
    }

    if (technical_metadata->afs_tocs == NULL || technical_metadata->afs_content_boxing_format == NULL)
    {
        return NULL;
    }

    struct mxml_node_s * document = mxmlNewXML("1.0");

    if (!afs_technical_metadata_save_xml(technical_metadata, document))
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
 *  \brief Save as XML nodes
 *
 *  Function translates the input afs_technical_metadata structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   technical_metadata  Pointer to the afs_technical_metadata structure.
 *  \param[out]  out                 Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_technical_metadata_save_xml(afs_technical_metadata * technical_metadata, mxml_node_t * out)
{
    if (out == NULL || technical_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * technical_metadata_node = mxmlNewElement(out, "TechnicalMetadata");

    mxml_node_t * boxer_format_node = mxmlNewElement(technical_metadata_node, "BoxerFormat");
    DBOOL result = afs_boxing_format_save_xml(boxer_format_node, technical_metadata->afs_content_boxing_format);

    if (result == DFALSE)
    {
        return DFALSE;
    }

    if ( technical_metadata->afs_tocs )
    {
        mxml_node_t * tocs_node = mxmlNewElement(technical_metadata_node, "Tocs");
        if (!afs_toc_files_save_xml(technical_metadata->afs_tocs, tocs_node))
        {
            return DFALSE;
        }
    }

    if ( technical_metadata->afs_applications )
    {
        mxml_node_t * tocs_node = mxmlNewElement(technical_metadata_node, "Applications");
        return afs_toc_files_save_xml(technical_metadata->afs_applications, tocs_node);
    }
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Load from file
 *
 *  Function translates the input XML file to the afs_technical_metadata structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  technical_metadata  Pointer to the afs_technical_metadata structure.
 *  \param[in]   file_name           Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_technical_metadata_load_file(afs_technical_metadata * technical_metadata, const char * file_name)
{
    if (file_name == NULL || technical_metadata == NULL)
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

    DBOOL return_value = afs_technical_metadata_load_xml(technical_metadata, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Load from string
 *
 *  Function translates the input XML string to the afs_technical_metadata structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  technical_metadata  Pointer to the afs_technical_metadata structure.
 *  \param[in]   in                  Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_technical_metadata_load_string(afs_technical_metadata * technical_metadata, const char * in)
{
    if (in == NULL || boxing_string_equal(in, "") || technical_metadata == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    DBOOL return_value = afs_technical_metadata_load_xml(technical_metadata, document);

    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Load from XML node tree
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
    
    if (tocs_node != NULL)
    {
        technical_metadata->afs_tocs = afs_toc_files_create();

        DBOOL load_tocs_result = afs_toc_files_load_xml(technical_metadata->afs_tocs, tocs_node);

        if (load_tocs_result == DFALSE)
        {
            afs_boxing_format_free(technical_metadata->afs_content_boxing_format);
            technical_metadata->afs_content_boxing_format = NULL;
            afs_toc_files_free(technical_metadata->afs_tocs);
            technical_metadata->afs_tocs = NULL;
            return DFALSE;
        }
    }

    mxml_node_t * applications_node = mxmlFindElement(technical_metadata_node, technical_metadata_node, "Applications", NULL, NULL, MXML_DESCEND);
    
    if (applications_node == NULL)
    {
        return DTRUE;
    }

    technical_metadata->afs_applications = afs_toc_files_create();

    DBOOL load_applications_result = afs_toc_files_load_xml(technical_metadata->afs_applications, applications_node);

    if (load_applications_result == DFALSE)
    {
        afs_toc_files_free(technical_metadata->afs_applications);
        technical_metadata->afs_applications = NULL;
    }
    
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of cf group
  */


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

    if (boxing_string_equal("TechnicalMetadata", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("work", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("class", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }
    }

    if (boxing_string_equal("class", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                ");
        }
    }

    if (boxing_string_equal("BoxerFormat", name) || boxing_string_equal("Tocs", name) || boxing_string_equal("Applications", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("files", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("file", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }
    }

    if (boxing_string_equal("file", parent_name) && !boxing_string_equal("data", name) && !boxing_string_equal("preview", name) && !boxing_string_equal("metadata", name))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n                ");
        }

        if (where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                ");
        }

        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n                    ");
        }
    }

    if (boxing_string_equal("data", name) && boxing_string_equal("file", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                ");
        }
    }

    if (boxing_string_equal("data", parent_name) && boxing_string_equal("file", mxmlGetElement(node->parent->parent)))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                    ");
        }

        if (where == MXML_WS_AFTER_OPEN && !boxing_string_equal("start", name) && !boxing_string_equal("end", name))
        {
            return ("\n                        ");
        }
    }

    if (boxing_string_equal("preview", name) && boxing_string_equal("file", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                ");
        }
    }

    if (boxing_string_equal("preview", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n                    ");
        }
    }

    if (boxing_string_equal("metadata", name) && boxing_string_equal("file", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                ");
        }
    }

    if (boxing_string_equal("metadata", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                    ");
        }
    }

    if (boxing_string_equal("source", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                        ");
        }
    }

    if (boxing_string_equal("data", parent_name) && boxing_string_equal("source", mxmlGetElement(node->parent->parent)))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n                            ");
        }
    }

    return (NULL);
}
