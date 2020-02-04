/*****************************************************************************
**
**  Definition of the toc file preview interface
**
**  Creation date:  2017/04/20
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

#include "tocfilepreview.h"
#include "boxing/platform/memory.h"
#include "boxing/string.h"
#include "xmlutils.h"
#include "boxing/log.h"
#include "mxml.h"

//  DEFINES
//


// PRIVATE INTERFACE
//

static const char * whitespace_cb(mxml_node_t *node, int where);


/*! 
  * \addtogroup file
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_file_preview_s  tocfilepreview.h
 *  \brief      TOC file preview data storage.
 *
 *  \param pages      Vector with afs_toc_file_preview_page instances.
 *  \param dpi        Output resolution for visual files.
 *
 *  Structure for storing TOC file preview pages data.
 */

// PUBLIC AFS TOC FILE PREVIEW PAGE FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC file preview instance.
 *
 *  Allocate memory for the afs_toc_file_preview type
 *  and initializes vector for storing TOC file preview pages data.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_file_preview structure.
 */

afs_toc_file_preview* afs_toc_file_preview_create()
{
    afs_toc_file_preview* toc_file_preview = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_file_preview);
    afs_toc_file_preview_init(toc_file_preview);
    return toc_file_preview;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC file preview instance.
 *
 *  Allocate memory for the afs_toc_file_preview type
 *  and initializes structure vector pointer with specified values.
 *  Return instance of allocated structure.
 *
 *  \param[in]  pages  Pointer to the vector with afs_toc_file_preview_page structure pointers.
 *  \return instance of allocated afs_toc_file_preview structure.
 */

afs_toc_file_preview* afs_toc_file_preview_create2(afs_toc_file_preview_pages* pages)
{
    afs_toc_file_preview* toc_file_preview = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_file_preview);
    afs_toc_file_preview_init2(toc_file_preview, pages);
    return toc_file_preview;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes vector pointer in the structure with NULL value.
 *
 *  Initializes vector pointer in the input structure with NULL value.
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  toc_file_preview  Pointer to the afs_toc_file_preview structure.
 */

void afs_toc_file_preview_init(afs_toc_file_preview * toc_file_preview)
{
    if (toc_file_preview == NULL)
    {
        return;
    }

    toc_file_preview->pages = NULL;
    toc_file_preview->dpi = -1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes vector pointer in the structure with input value.
 *
 *  Initializes vector pointer in the input afs_toc_file_preview structure with specified input value.
 *  If input pointer is NULL then return without initialization.
 *
 *  \param[in]  toc_file_preview  Pointer to the afs_toc_file_preview structure.
 *  \param[in]  pages             Pointer to the vector with afs_toc_file_preview_page structure pointers.
 */

void afs_toc_file_preview_init2(afs_toc_file_preview * toc_file_preview, afs_toc_file_preview_pages* pages)
{
    if (toc_file_preview == NULL)
    {
        return;
    }

    toc_file_preview->pages = pages;
    toc_file_preview->dpi = -1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_file_preview structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_file_preview  Pointer to the afs_toc_file_preview structure.
 */

void afs_toc_file_preview_free(afs_toc_file_preview * toc_file_preview)
{
    if (toc_file_preview == NULL)
    {
        return;
    }

    if (toc_file_preview->pages != NULL)
    {
        for (unsigned int i = 0; i < toc_file_preview->pages->size; i++)
        {
            afs_toc_file_preview_page_free(GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, i));
            GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, i) = NULL;
        }
    }

    gvector_free(toc_file_preview->pages);
    boxing_memory_free(toc_file_preview);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_file_preview structure.
 *
 *  Function create a copy of input afs_toc_file_preview structure and return it.
 *  If TOC file preview pointer is NULL function return NULL.
 *
 *  \param[in]  toc_file_preview  Pointer to the afs_toc_file_preview structure.
 *  \return new copy of afs_toc_file_preview structure or NULL.
 */

afs_toc_file_preview* afs_toc_file_preview_clone(const afs_toc_file_preview * toc_file_preview)
{
    if (toc_file_preview == NULL)
    {
        return NULL;
    }

    if (toc_file_preview->pages == NULL)
    {
        afs_toc_file_preview* return_toc_file_preview = afs_toc_file_preview_create();
        return_toc_file_preview->dpi = toc_file_preview->dpi;

        return return_toc_file_preview;
    }

    afs_toc_file_preview* return_toc_file_preview = afs_toc_file_preview_create2(gvector_create_pointers(toc_file_preview->pages->size));
    
    for (unsigned int i = 0; i < toc_file_preview->pages->size; i++)
    {
        afs_toc_file_preview_page* preview_page = GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, i);
        GVECTORN(return_toc_file_preview->pages, afs_toc_file_preview_page *, i) = afs_toc_file_preview_page_clone(preview_page);
    }
    return_toc_file_preview->dpi = toc_file_preview->dpi;

    return return_toc_file_preview;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_file_preview structures on the identity.
 *
 *  Function checks two instances of the afs_toc_file_preview structures on the identity.
 *
 *  \param[in]   toc_file_preview1  Pointer to the first instance of the afs_toc_file_preview structure.
 *  \param[in]   toc_file_preview2  Pointer to the second instance of the afs_toc_file_preview structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_file_preview_equal(afs_toc_file_preview * toc_file_preview1, afs_toc_file_preview * toc_file_preview2)
{
    if (toc_file_preview1 == NULL && toc_file_preview2 == NULL)
    {
        return DTRUE;
    }

    if (toc_file_preview1 == NULL || toc_file_preview2 == NULL)
    {
        return DFALSE;
    }

    if (toc_file_preview1->pages == NULL && toc_file_preview2->pages == NULL)
    {
        return toc_file_preview1->dpi == toc_file_preview2->dpi ? DTRUE : DFALSE;
    }

    if (toc_file_preview1->pages == NULL || toc_file_preview2->pages == NULL)
    {
        return DFALSE;
    }

    if (toc_file_preview1->pages->size != toc_file_preview2->pages->size)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_file_preview1->pages->size; i++)
    {
        if (afs_toc_file_preview_page_equal(GVECTORN(toc_file_preview1->pages, afs_toc_file_preview_page *, i), GVECTORN(toc_file_preview2->pages, afs_toc_file_preview_page *, i)) == DFALSE)
        {
            return DFALSE;
        }
    }

    return toc_file_preview1->dpi == toc_file_preview2->dpi ? DTRUE : DFALSE;;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_file_preview_page structure to the input afs_toc_file_preview structure.
 *
 *  The function adds a new instance of afs_toc_file_preview_page structure to the input afs_toc_file_preview structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_file_preview_page structure is not add and the function returns DFALSE.
 *  If input afs_toc_file_preview_page instance is not valid then new instance is not add the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_file_preview       Pointer to the instance of the afs_toc_file_preview structure.
 *  \param[in]   toc_file_preview_page  Pointer to the instance of the afs_toc_file_preview_page structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_add_page(afs_toc_file_preview * toc_file_preview, afs_toc_file_preview_page * toc_file_preview_page)
{
    if (toc_file_preview == NULL || toc_file_preview_page == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_file_preview_page_is_valid(toc_file_preview_page) != DTRUE)
    {
        return DFALSE;
    }

    if (toc_file_preview->pages == NULL)
    {
        toc_file_preview->pages = gvector_create_pointers(0);
    }

    size_t old_size = toc_file_preview->pages->size;

    gvector_append_data(toc_file_preview->pages, 1, &toc_file_preview_page);

    if (toc_file_preview->pages->size != old_size + 1)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_file_preview_page structure according to the input index.
 *
 *  The function returns an instance of afs_toc_file_preview_page structure according to the input index.
 *
 *  \param[in]   toc_file_preview  Pointer to the instance of the afs_toc_file_preview structure.
 *  \param[in]   index             Index for specifying the required instance of the afs_toc_file_preview_page structure in the storage vector.
 *  \return an instance of afs_toc_file_preview_page structure or NULL.
 */

afs_toc_file_preview_page * afs_toc_file_preview_get_page(afs_toc_file_preview * toc_file_preview, unsigned int index)
{
    if (toc_file_preview == NULL)
    {
        return NULL;
    }

    if (toc_file_preview->pages == NULL)
    {
        return NULL;
    }

    if (index >= toc_file_preview->pages->size)
    {
        return NULL;
    }

    return GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_file_preview_page structure.
 *
 *  The function returns a count of instances of afs_toc_file_preview_page structure.
 *  If input afs_toc_file_preview pointer is NULL or internal pages vector pointer is NULL then function returns zero.
 *
 *  \param[in]   toc_file_preview  Pointer to the instance of the afs_toc_file_preview structure.
 *  \return a count of instances of afs_toc_file_preview_page structure or zero.
 */

unsigned int afs_toc_file_preview_get_page_count(afs_toc_file_preview * toc_file_preview)
{
    if (toc_file_preview == NULL)
    {
        return 0;
    }

    if (toc_file_preview->pages == NULL)
    {
        return 0;
    }

    return (unsigned int)toc_file_preview->pages->size;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a number of start frame.
 *
 *  The function returns a number of start frame.
 *  If input afs_toc_file_preview pointer is NULL or internal pages vector pointer is NULL or vector has no pointers then function returns -1.
 *
 *  \param[in]   toc_file_preview  Pointer to the instance of the afs_toc_file_preview structure.
 *  \return a number of start frame.
 */

int afs_toc_file_preview_get_start_frame(afs_toc_file_preview * toc_file_preview)
{
    if (toc_file_preview == NULL)
    {
        return -1;
    }

    if (toc_file_preview->pages == NULL)
    {
        return -1;
    }

    if (toc_file_preview->pages->size == 0)
    {
        return -1;
    }

    return GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, 0)->start_frame;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds the offset of the start frame to all afs_toc_file_preview_page instances.
 *
 *  The function adds the offset of the start frame to all afs_toc_file_preview_page instances.
 *  If input afs_toc_file_preview pointer is NULL or internal pages vector pointer is NULL then function returns without adding offset.
 *
 *  \param[in]   toc_file_preview  Pointer to the instance of the afs_toc_file_preview structure.
 *  \param[in]   offset            The offset that we need to add.
 */

void afs_toc_file_preview_add_frame_offset(afs_toc_file_preview * toc_file_preview, unsigned int offset)
{
    if (toc_file_preview == NULL)
    {
        return;
    }

    if (toc_file_preview->pages == NULL)
    {
        return;
    }

    for (unsigned int i = 0; i < toc_file_preview->pages->size; i++)
    {
        GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, i)->start_frame += offset;
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function sets the output resolution for visual files.
 *
 *  The function sets the output resolution for visual files.
 *  If input afs_toc_file_preview pointer is NULL then function returns without sets the output resolution.
 *
 *  \param[in]   toc_file_preview  Pointer to the instance of the afs_toc_file_preview structure.
 *  \param[in]   dpi               The output resolution.
 */

void afs_toc_file_preview_set_dpi(afs_toc_file_preview * toc_file_preview, int dpi)
{
    if (toc_file_preview == NULL)
    {
        return;
    }

    toc_file_preview->dpi = dpi;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the afs_toc_file_preview structure.
 *
 *  The function returns the sign of integrity the afs_toc_file_preview structure.
 *  If TOC file preview pointer is NULL or internal pages vector pointer is NULL or vector has no pointers then function return DFALSE.
 *
 *  \param[in]   toc_file_preview   Pointer to the afs_toc_file_preview structure.
 *  \return the sign of integrity of the afs_toc_file_preview structure.
 */

DBOOL afs_toc_file_preview_is_valid(afs_toc_file_preview * toc_file_preview)
{
    if (toc_file_preview == NULL)
    {
        return DFALSE;
    }

    if (toc_file_preview->pages == NULL)
    {
        return DFALSE;
    }

    if (toc_file_preview->pages->size == 0)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_file_preview->pages->size; i++)
    {
        if (afs_toc_file_preview_page_is_valid(GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, i)) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gets the frames count.
 *
 *  The function gets the frames count according to the input afs_toc_file_preview and afs_toc_preview_layout_definitions structures.
 *  If one of the input pointers is NULL then function return DFALSE and frames count will not change.
 *
 *  \param[in]   toc_file_preview   Pointer to the afs_toc_file_preview structure.
 *  \param[out]  frames_count       Pointer to the frames count integer variable.
 *  \param[in]   definitions        Pointer to the afs_toc_preview_layout_definitions structure.
 *  \return DTRUE if success.
 */

DBOOL afs_toc_file_preview_get_frames_count(afs_toc_file_preview * toc_file_preview, unsigned int * frames_count, const afs_toc_preview_layout_definitions * definitions)
{
    if (toc_file_preview == NULL || frames_count == NULL || definitions == NULL)
    {
        return DFALSE;
    }

    *frames_count = 0;
    unsigned int preview_page_count = afs_toc_file_preview_get_page_count(toc_file_preview);
    for (unsigned int p = 0; p < preview_page_count; p++)
    {
        unsigned int f;
        afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file_preview, p);

        if (toc_file_preview_page == NULL)
        {
            return DFALSE;
        }

        if (afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &f, definitions) == DFALSE)
        {
            return DFALSE;
        }
        *frames_count += f;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_file_preview structure to the XML file.
 *
 *  Function translates the input afs_toc_file_preview structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   file_name         Name of the XML file.
 *  \param[in]   toc_file_preview  Pointer to the afs_toc_file_preview_page structure.
 *  \param[in]   compact           If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_save_file(afs_toc_file_preview * toc_file_preview, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_file_preview == NULL)
    {
        return DFALSE;
    }

    mxml_node_t *tree = mxmlNewXML("1.0");

    if (!afs_toc_file_preview_save_xml(toc_file_preview, tree))
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
 *  \brief Function translates the input afs_toc_file_preview structure to the XML string.
 *
 *  Function translates the input afs_toc_file_preview structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_file_preview  Pointer to the afs_toc_file_preview structure.
 *  \param[in]   compact           If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_file_preview_save_string(afs_toc_file_preview * toc_file_preview, DBOOL compact)
{
    if (toc_file_preview == NULL)
    {
        return DFALSE;
    }

    mxml_node_t* document = mxmlNewXML("1.0");

    if (!afs_toc_file_preview_save_xml(toc_file_preview, document))
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
 *  \brief Function translates the input afs_toc_file_preview structure to the XML nodes.
 *
 *  Function translates the input afs_toc_file_preview structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_file_preview   Pointer to the afs_toc_file_preview structure.
 *  \param[out]  out                Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_save_xml(afs_toc_file_preview * toc_file_preview, mxml_node_t* out)
{
    if (out == NULL || toc_file_preview == NULL)
    {
        return DFALSE;
    }

    if (!afs_toc_file_preview_is_valid(toc_file_preview))
    {
        return DFALSE;
    }

    // Start file tag
    mxml_node_t *preview_node = mxmlNewElement(out, "preview");

    if (toc_file_preview->dpi >= 0)
    {
        afs_xmlutils_add_new_int_node(preview_node, "dpi", toc_file_preview->dpi);
    }

    for (unsigned int i = 0; i < toc_file_preview->pages->size; i++)
    {
        DBOOL result = afs_toc_file_preview_page_save_xml(GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, i), preview_node);

        if (result == DFALSE)
        {
            return DFALSE;
        }
    }
    
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_file_preview structure.
 *
 *  Function translates the input XML nodes to the afs_toc_file_preview structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_file_preview  Pointer to the afs_toc_file_preview structure.
 *  \param[in]   file_name         Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_load_file(afs_toc_file_preview * toc_file_preview, const char * file_name)
{
    if (file_name == NULL || toc_file_preview == NULL)
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

    DBOOL return_value = afs_toc_file_preview_load_xml(toc_file_preview, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_file_preview structure.
 *
 *  Function translates the input XML string to the afs_toc_file_preview structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_file_preview  Pointer to the afs_toc_file_preview structure.
 *  \param[in]   in                Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_load_string(afs_toc_file_preview * toc_file_preview, const char * in)
{
    if (in == NULL || boxing_string_equal(in, "") == DTRUE || toc_file_preview == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    DBOOL return_value = afs_toc_file_preview_load_xml(toc_file_preview, document);

    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC file preview structure.
 *
 *  Function translates the input XML nodes to the TOC file preview structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_file_preview  Pointer to the afs_toc_file_preview structure.
 *  \param[in]   node              Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_load_xml(afs_toc_file_preview * toc_file_preview, mxml_node_t* node)
{
    if (node == NULL || toc_file_preview == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * preview_node = NULL;

    if (boxing_string_equal(mxmlGetElement(node), "preview") == DTRUE)
    {
        preview_node = node;
    }
    else
    {
        preview_node = mxmlFindElement(node, node, "preview", NULL, NULL, MXML_DESCEND);
    }

    if (preview_node == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * page_node = mxmlGetFirstChild(preview_node);

    afs_xmlutils_get_integer_element(&toc_file_preview->dpi, preview_node, "dpi");

    while (page_node != NULL)
    {
        if (mxmlGetType(page_node) == MXML_ELEMENT)
        {
            const char * clave = mxmlGetElement(page_node);
            
            if (boxing_string_equal(clave, "pages") == DTRUE)
            {
                afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();
                
                DBOOL load_page_result = afs_toc_file_preview_page_load_xml(toc_file_preview_page, page_node);

                if (load_page_result == DFALSE)
                {
                    DLOG_INFO("Has no preview.\n");
                    return DFALSE;
                }

                DBOOL add_page_result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

                if (add_page_result == DFALSE)
                {
                    afs_toc_file_preview_page_free(toc_file_preview_page);
                    return DFALSE;
                }
            }
        }

        page_node = mxmlWalkNext(page_node, preview_node, MXML_NO_DESCEND);
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of file group
  */


// PRIVATE AFS TOC FILE PREVIEW PAGE FUNCTIONS
//

static const char * whitespace_cb(mxml_node_t *node, int where)
{
    const char *name;

    /*
    * We can conditionally break to a new line
    * before or after any element. These are
    * just common HTML elements...
    */

    name = mxmlGetElement(node);

    if (boxing_string_equal("preview", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("pages", name) || boxing_string_equal("dpi", name))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n    ");
        }
    }

    return (NULL);
}
