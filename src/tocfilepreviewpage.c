/*****************************************************************************
**
**  Definition of the toc file preview page interface
**
**  Creation date:  2017/04/10
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

#include "tocfilepreviewpage.h"
#include "xmlutils.h"
#include "boxing/math/math.h"
#include "boxing/string.h"
#include "boxing/log.h"
#include "mxml.h"

// SYSTEM INCLUDES
#include <string.h>

//  DEFINES
//



// PRIVATE INTERFACE
//

static DBOOL        extract_xy_from_string(unsigned int* x, unsigned int* y, const char* text);
static const char * whitespace_cb(mxml_node_t *node, int where);


/*! 
  * \addtogroup file
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_file_preview_page_s  tocfilepreviewpage.h
 *  \brief      TOC file preview page data storage.
 *
 *  \param layout_id      Layout id string.
 *  \param start_frame    Start frame number.
 *  \param start_section  Start section number.
 *  \param section_count  Number of sections.
 *  \param dimension_x    X-axis dimension.
 *  \param dimension_y    Y-axis dimension.
 *  \param overlap_x      X-axis overlap.
 *  \param overlap_y      Y-axis overlap.
 *
 *  Structure for storing TOC file preview page data.
 */


// PUBLIC AFS TOC FILE PREVIEW PAGE FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC file preview page instance.
 *
 *  Allocate memory for the afs_toc_file_preview_page type
 *  and initializes all structure pointers with NULL values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_file_preview_page structure.
 */

afs_toc_file_preview_page* afs_toc_file_preview_page_create(void)
{
    afs_toc_file_preview_page* toc_file_preview_page = malloc(sizeof(afs_toc_file_preview_page));
    afs_toc_file_preview_page_init(toc_file_preview_page);
    return toc_file_preview_page;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC file preview page instance.
 *
 *  Allocate memory for the afs_toc_file_preview_page type
 *  and initializes all structure pointers with specified values.
 *  Return instance of allocated structure.
 *
 *  \param[in]  layout_id              Layout id string.
 *  \param[in]  start_frame            Start frame number.
 *  \param[in]  start_section          Start section number.
 *  \param[in]  section_count          Number of sections.
 *  \param[in]  dimension_x            X-axis dimension.
 *  \param[in]  dimension_y            Y-axis dimension.
 *  \param[in]  overlap_x              X-axis overlap.
 *  \param[in]  overlap_y              Y-axis overlap.
 *  \return instance of allocated afs_toc_file_preview_page structure.
 */

afs_toc_file_preview_page* afs_toc_file_preview_page_create2(
    const char *  layout_id,
    unsigned int  start_frame,
    unsigned int  start_section,
    unsigned int  section_count,
    unsigned int  dimension_x,
    unsigned int  dimension_y,
    unsigned int  overlap_x,
    unsigned int  overlap_y)
{
    afs_toc_file_preview_page* toc_file_preview_page = malloc(sizeof(afs_toc_file_preview_page));
    afs_toc_file_preview_page_init2(toc_file_preview_page, layout_id, start_frame, start_section, section_count, dimension_x, dimension_y, overlap_x, overlap_y);
    return toc_file_preview_page;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers with default values.
 *
 *  Initializes all input structure pointers with NULL values and all unsigned int members with zero value.
 *  If input pointer is NULL, then return without initialization.
 *
 *  \param[in]  toc_file_preview_page  Pointer to the afs_toc_file_preview_page structure.
 */

void afs_toc_file_preview_page_init(afs_toc_file_preview_page * toc_file_preview_page)
{
    if (toc_file_preview_page == NULL)
    {
        return;
    }

    toc_file_preview_page->layout_id = NULL;
    toc_file_preview_page->start_frame = 0;
    toc_file_preview_page->start_section = 0;
    toc_file_preview_page->section_count = 0;
    toc_file_preview_page->dimension_x = 0;
    toc_file_preview_page->dimension_y = 0;
    toc_file_preview_page->overlap_x = 0;
    toc_file_preview_page->overlap_y = 0;

    toc_file_preview_page->reference_count = 1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers.
 *
 *  Initializes all input structure pointers with specified values.
 *  If one of the input pointers is NULL, then return without initialization.
 *
 *  \param[in]  toc_file_preview_page  Pointer to the afs_toc_file_preview_page structure.
 *  \param[in]  layout_id              Layout id string.
 *  \param[in]  start_frame            Start frame number.
 *  \param[in]  start_section          Start section number.
 *  \param[in]  section_count          Number of sections.
 *  \param[in]  dimension_x            X-axis dimension.
 *  \param[in]  dimension_y            Y-axis dimension.
 *  \param[in]  overlap_x              X-axis overlap.
 *  \param[in]  overlap_y              Y-axis overlap.
 */

void afs_toc_file_preview_page_init2(
    afs_toc_file_preview_page * toc_file_preview_page,
    const char *                layout_id,
    unsigned int                start_frame,
    unsigned int                start_section,
    unsigned int                section_count,
    unsigned int                dimension_x,
    unsigned int                dimension_y,
    unsigned int                overlap_x,
    unsigned int                overlap_y)
{
    if (toc_file_preview_page == NULL)
    {
        return;
    }

    toc_file_preview_page->layout_id = boxing_string_clone(layout_id);
    toc_file_preview_page->start_frame = start_frame;
    toc_file_preview_page->start_section = start_section;
    toc_file_preview_page->section_count = section_count;
    toc_file_preview_page->dimension_x = dimension_x;
    toc_file_preview_page->dimension_y = dimension_y;
    toc_file_preview_page->overlap_x = overlap_x;
    toc_file_preview_page->overlap_y = overlap_y;

    toc_file_preview_page->reference_count = 1;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_file_preview_page structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_file_preview_page  Pointer to the afs_toc_file_preview_page structure.
 */

void afs_toc_file_preview_page_free(afs_toc_file_preview_page * toc_file_preview_page)
{
    if (toc_file_preview_page == NULL)
    {
        return;
    }

    toc_file_preview_page->reference_count--;

    if (toc_file_preview_page->reference_count <= 0)
    {
        free(toc_file_preview_page->layout_id);
        free(toc_file_preview_page);
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_file_preview_page structure.
 *
 *  Function create a copy of input afs_toc_file_preview_page structure and return it.
 *  If TOC file preview page pointer is NULL function return NULL.
 *
 *  \param[in]  toc_file_preview_page  Pointer to the afs_toc_file_preview_page structure.
 *  \return new copy of afs_toc_file_preview_page structure or NULL.
 */

afs_toc_file_preview_page * afs_toc_file_preview_page_clone(afs_toc_file_preview_page * toc_file_preview_page)
{
    // If TOC file preview page pointer is NULL return NULL.
    if (toc_file_preview_page == NULL)
    {
        return NULL;
    }

    return afs_toc_file_preview_page_create2(
        toc_file_preview_page->layout_id, 
        toc_file_preview_page->start_frame,
        toc_file_preview_page->start_section,
        toc_file_preview_page->section_count,
        toc_file_preview_page->dimension_x,
        toc_file_preview_page->dimension_y,
        toc_file_preview_page->overlap_x,
        toc_file_preview_page->overlap_y);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function returns a new reference to the input afs_toc_file_preview_page structure.
 *
 *  Function returns a new reference to the input afs_toc_file_preview_page structure.
 *  The reference count is incremented by 1.
 *  If TOC file preview page pointer is NULL function return NULL.
 *
 *  \param[in]  toc_file_preview_page  Pointer to the afs_toc_file_preview_page structure.
 *  \return new reference of afs_toc_file_preview_page structure or NULL.
 */

afs_toc_file_preview_page * afs_toc_file_preview_page_get_new_reference(afs_toc_file_preview_page * toc_file_preview_page)
{
    // If TOC file preview page pointer is NULL return NULL.
    if (toc_file_preview_page == NULL)
    {
        return NULL;
    }

    toc_file_preview_page->reference_count++;
    return toc_file_preview_page;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_file_preview_page structures on the identity.
 *
 *  Function checks two instances of the afs_toc_file_preview_page structures on the identity.
 *
 *  \param[in]   toc_file_preview_page1  Pointer to the first instance of the afs_toc_file_preview_page structure.
 *  \param[in]   toc_file_preview_page2  Pointer to the second instance of the afs_toc_file_preview_page structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_file_preview_page_equal(afs_toc_file_preview_page * toc_file_preview_page1, afs_toc_file_preview_page * toc_file_preview_page2)
{
    if (toc_file_preview_page1 == NULL && toc_file_preview_page2 == NULL)
    {
        return DTRUE;
    }

    if (toc_file_preview_page1 == NULL || toc_file_preview_page2 == NULL)
    {
        return DFALSE;
    }

    if (strcmp(toc_file_preview_page1->layout_id ? toc_file_preview_page1->layout_id : "", toc_file_preview_page2->layout_id ? toc_file_preview_page2->layout_id : "") == 0 &&
        toc_file_preview_page1->start_frame == toc_file_preview_page2->start_frame &&
        toc_file_preview_page1->start_section == toc_file_preview_page2->start_section &&
        toc_file_preview_page1->section_count == toc_file_preview_page2->section_count &&
        toc_file_preview_page1->dimension_x == toc_file_preview_page2->dimension_x &&
        toc_file_preview_page1->dimension_y == toc_file_preview_page2->dimension_y &&
        toc_file_preview_page1->overlap_x == toc_file_preview_page2->overlap_x &&
        toc_file_preview_page1->overlap_y == toc_file_preview_page2->overlap_y)
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the afs_toc_file_preview_page structure.
 *
 *  The function returns the sign of integrity the current structure.
 *  If TOC file preview page pointer is NULL then function return DFALSE.
 *
 *  \param[in]   toc_file_preview_page   Pointer to the afs_toc_file_preview_page structure.
 *  \return the sign of integrity the afs_toc_file_preview_page structure.
 */

DBOOL afs_toc_file_preview_page_is_valid(afs_toc_file_preview_page * toc_file_preview_page)
{
    if (toc_file_preview_page == NULL)
    {
        return DFALSE;
    }

    if (toc_file_preview_page->layout_id != NULL &&
        strlen(toc_file_preview_page->layout_id) != 0 &&
        toc_file_preview_page->section_count != 0 &&
        toc_file_preview_page->overlap_x <= toc_file_preview_page->dimension_x &&
        toc_file_preview_page->overlap_y <= toc_file_preview_page->dimension_y)
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gets the frames count.
 *
 *  The function gets the frames count according to the input afs_toc_file_preview_page and afs_toc_preview_layout_definitions structures.
 *  If one of the input pointers is NULL then function return DFALSE and frames count will not change.
 *
 *  \param[in]   toc_file_preview_page   Pointer to the afs_toc_file_preview_page structure.
 *  \param[out]  frames_count            Pointer to the frames count integer variable.
 *  \param[in]   definitions             Pointer to the afs_toc_preview_layout_definitions structure.
 *  \return DTRUE if success.
 */

DBOOL afs_toc_file_preview_page_get_frames_count(afs_toc_file_preview_page * toc_file_preview_page, unsigned int * frames_count, const afs_toc_preview_layout_definitions * definitions)
{
    if ( frames_count == NULL )
    {
        return DFALSE;
    }

    *frames_count = 0;

    if ( toc_file_preview_page == NULL || definitions == NULL )
    {
        return DFALSE;
    }

    if (toc_file_preview_page->layout_id == NULL)
    {
        return DFALSE;
    }

    // Search for layout
    afs_toc_preview_layout_definition * definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(definitions, toc_file_preview_page->layout_id);
    if (definition == NULL)
    {
        return DFALSE;
    }

    int frame_section_capacity = afs_toc_preview_layout_definition_get_section_count(definition);

    if (frame_section_capacity == 0)
    {
        return DFALSE;
    }

    if ( toc_file_preview_page->section_count == 0 )
    {
        return DTRUE;
    }

    *frames_count = (toc_file_preview_page->start_section + toc_file_preview_page->section_count) / frame_section_capacity;

    if ((toc_file_preview_page->start_section + toc_file_preview_page->section_count) % frame_section_capacity)
    {
        (*frames_count)++;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function gets the sections on frame value.
 *
 *  The function gets the sections on frame value according to the input afs_toc_file_preview_page and afs_toc_preview_layout_definitions structures.
 *  If one of the input pointers is NULL then function return DFALSE and sections on frame varable will not change.
 *
 *  \param[in]   toc_file_preview_page   Pointer to the afs_toc_file_preview_page structure.
 *  \param[out]  sections                Pointer to the sections on frame integer variable.
 *  \param[in]   frame                   Frame number.
 *  \param[in]   definitions             Pointer to the afs_toc_preview_layout_definitions structure.
 *  \return DTRUE if success.
 */

DBOOL afs_toc_file_preview_page_get_sections_on_frame(afs_toc_file_preview_page * toc_file_preview_page, unsigned int * sections, unsigned int frame, const afs_toc_preview_layout_definitions * definitions)
{
    if (toc_file_preview_page == NULL || sections == NULL || definitions == NULL)
    {
        return DFALSE;
    }

    if (toc_file_preview_page->layout_id == NULL)
    {
        return DFALSE;
    }

    // Search for layout
    afs_toc_preview_layout_definition * definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(definitions, toc_file_preview_page->layout_id);
    if (definition == NULL)
    {
        return DFALSE;
    }

    unsigned int frame_section_capacity = afs_toc_preview_layout_definition_get_section_count(definition);
    *sections = (unsigned)BOXING_MATH_MAX(BOXING_MATH_MIN(((int)toc_file_preview_page->section_count + (int)toc_file_preview_page->start_section) -
                (int)frame_section_capacity * BOXING_MATH_MAX(0, (int)frame), (int)frame_section_capacity), 0) -
                BOXING_MATH_MAX(BOXING_MATH_MIN((int)toc_file_preview_page->start_section -
                (int)frame_section_capacity * BOXING_MATH_MAX(0, (int)frame), (int)frame_section_capacity), 0);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_file_preview_page structure to the XML file.
 *
 *  Function translates the input afs_toc_file_preview_page structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   file_name              Name of the XML file.
 *  \param[in]   toc_file_preview_page  Pointer to the afs_toc_file_preview_page structure.
 *  \param[in]   compact                If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_page_save_file(afs_toc_file_preview_page * toc_file_preview_page, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_file_preview_page == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * tree = mxmlNewXML("1.0");

#ifndef WIN32
    FILE * fp_save = fopen(file_name, "w+");
#else
    FILE * fp_save = fopen(file_name, "w+b");
#endif

    if (fp_save == NULL)
    {
        return DFALSE;
    }

    if (!afs_toc_file_preview_page_save_xml(toc_file_preview_page, tree))
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
 *  \brief Function translates the input afs_toc_file_preview_page structure to the XML string.
 *
 *  Function translates the input afs_toc_file_preview_page structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_file_preview_page  Pointer to the afs_toc_file_preview_page structure.
 *  \param[in]   compact                If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_file_preview_page_save_string(afs_toc_file_preview_page * toc_file_preview_page, DBOOL compact)
{
    // If TOC file preview page pointer is NULL return DFALSE
    if (toc_file_preview_page == NULL)
    {
        return DFALSE;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_file_preview_page_save_xml(toc_file_preview_page, document))
    {
        DLOG_INFO("Load XML failed!\n");
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
 *  \brief Function translates the input afs_toc_file_preview_page structure to the XML nodes.
 *
 *  Function translates the input afs_toc_file_preview_page structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_file_preview_page   Pointer to the afs_toc_file_preview_page structure.
 *  \param[out]  out                     Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_page_save_xml(afs_toc_file_preview_page * toc_file_preview_page, mxml_node_t* out)
{
    // If output node pointer is NULL or TOC file preview page pointer is NULL return DFALSE
    if (out == NULL || toc_file_preview_page == NULL)
    {
        return DFALSE;
    }

    // Start file tag
    mxml_node_t * file_node = mxmlNewElement(out, "pages");

    if (toc_file_preview_page->layout_id != NULL)
    {
        mxmlElementSetAttr(file_node, "layoutId", toc_file_preview_page->layout_id);
    }

    mxmlElementSetAttrf(file_node, "frame", "%u", toc_file_preview_page->start_frame);
    mxmlElementSetAttrf(file_node, "section", "%u", toc_file_preview_page->start_section);
    mxmlElementSetAttrf(file_node, "count", "%u", toc_file_preview_page->section_count);

    if (toc_file_preview_page->dimension_x && toc_file_preview_page->dimension_y)
    {
        mxmlElementSetAttrf(file_node, "dimension", "%ux%u", toc_file_preview_page->dimension_x, toc_file_preview_page->dimension_y);
    }

    if (toc_file_preview_page->overlap_x && toc_file_preview_page->overlap_y)
    {
        mxmlElementSetAttrf(file_node, "overlap", "%ux%u", toc_file_preview_page->overlap_x, toc_file_preview_page->overlap_y);
    }
    
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_file_preview_page structure.
 *
 *  Function translates the input XML nodes to the afs_toc_file_preview_page structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_file_preview_page  Pointer to the afs_toc_file_preview_page structure.
 *  \param[in]   file_name              Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_page_load_file(afs_toc_file_preview_page * toc_file_preview_page, const char * file_name)
{
    // If input file name string pointer is NULL or TOC file preview page pointer is NULL return DFALSE
    if (file_name == NULL || toc_file_preview_page == NULL)
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

    DBOOL return_value = afs_toc_file_preview_page_load_xml(toc_file_preview_page, document);
    
    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_file_preview_page structure.
 *
 *  Function translates the input XML string to the afs_toc_file_preview_page structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_file_preview_page  Pointer to the afs_toc_file_preview_page structure.
 *  \param[in]   in                     Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_page_load_string(afs_toc_file_preview_page * toc_file_preview_page, const char * in)
{
    // If input string pointer is NULL or TOC file preview page pointer is NULL return DFALSE
    if (in == NULL || strlen(in) == 0 || toc_file_preview_page == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_file_preview_page_load_xml(toc_file_preview_page, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC file preview page structure.
 *
 *  Function translates the input XML nodes to the TOC file preview page structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_file_preview_page  Pointer to the afs_toc_file_preview_page structure.
 *  \param[in]   node                   Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_file_preview_page_load_xml(afs_toc_file_preview_page * toc_file_preview_page, mxml_node_t* node)
{
    // If input node pointer is NULL or TOC file preview page pointer is NULL return DFALSE
    if (node == NULL || toc_file_preview_page == NULL)
    {
        return DFALSE;
    }
    
    mxml_node_t * file_node = NULL;

    if (strcmp(mxmlGetElement(node), "pages") == 0)
    {
        file_node = node;
    }
    else
    {
        file_node = mxmlFindElement(node, node, "pages", NULL, NULL, MXML_DESCEND);
    }

    if (file_node == NULL)
    {
        return DFALSE;
    }

    toc_file_preview_page->layout_id = boxing_string_clone(mxmlElementGetAttr(file_node, "layoutId"));

    afs_xmlutils_get_integer_attribute((int*)&toc_file_preview_page->start_frame, file_node, "frame");
    afs_xmlutils_get_integer_attribute((int*)&toc_file_preview_page->start_section, file_node, "section");
    afs_xmlutils_get_integer_attribute((int*)&toc_file_preview_page->section_count, file_node, "count");

    if (!extract_xy_from_string(&toc_file_preview_page->dimension_x, &toc_file_preview_page->dimension_y, mxmlElementGetAttr(file_node, "dimension")))
    {
        DLOG_INFO1("Filed to extract dimension from string - %s.\n", mxmlElementGetAttr(file_node, "dimension"));
        return DFALSE;
    }

    if (!extract_xy_from_string(&toc_file_preview_page->overlap_x, &toc_file_preview_page->overlap_y, mxmlElementGetAttr(file_node, "overlap")))
    {
        DLOG_INFO1("Filed to extract overlap from string - %s.\n", mxmlElementGetAttr(file_node, "overlap"));
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of file group
  */


// PRIVATE AFS TOC FILE PREVIEW PAGE FUNCTIONS
//


static DBOOL extract_xy_from_string(unsigned int * x, unsigned int * y, const char * text)
{
    if (x == NULL || y == NULL)
    {
        return DFALSE;
    }

    if (text == NULL || strlen(text) == 0)
    {
        *x = 0;
        *y = 0;

        return DTRUE;
    }

    int match_count = sscanf(text, "%ux%u", x, y);

    if (match_count != 2)
    {
        return DFALSE;
    }

    return DTRUE;
}


static const char * whitespace_cb(mxml_node_t * node, int where)
{
    const char *name, *parent_name;

    /*
    * We can conditionally break to a new line
    * before or after any element. These are
    * just common HTML elements...
    */

    name = mxmlGetElement(node);
    parent_name = mxmlGetElement(node->parent);

    /// \todo warning: variable ‘parent_name’ set but not used [-Wunused-but-set-variable]
    (void) parent_name;

    if (strcmp("pages", name) == 0)
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n    ");
        }
    }

    return (NULL);
}
