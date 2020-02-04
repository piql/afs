/*****************************************************************************
**
**  Implementation of structure afs_toc_preview_section methods C interface
**
**  Creation date:    2016/08/18
**  Created by:       Alexander Silokhin
**  Correction date:  2017/11/27
**  Corrected by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2016 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "toc/previewsection.h"
#include "xmlutils.h"
#include "boxing/string.h"

// PRIVATE INTERFACE
//
static const char * whitespace_cb(mxml_node_t *node, int where);


/*! 
  * \addtogroup preview
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_preview_section_s  previewsection.h
 *  \brief      Preview section data storage.
 *
 *  \param id         Preview section id.
 *  \param x          X coordinate of the beginning of the section.
 *  \param y          Y coordinate of the beginning of the section.
 *  \param width      Width of the section.
 *  \param height     Height of the section.
 *  \param rotation   Section rotation.
 *
 *  Structure for storing preview section data.
 */


// PUBLIC AFS TOC PEVIEW SECTION FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc preview section instance.
 *
 *  Allocate memory for the afs_toc_preview_section type
 *  and initializes all structure data with zero values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_preview_section structure.
 */

afs_toc_preview_section * afs_toc_preview_section_create()
{
    afs_toc_preview_section * toc_preview_section = malloc(sizeof(afs_toc_preview_section));
    afs_toc_preview_section_init(toc_preview_section, 0, 0, 0, 0, 0, 0);

    return toc_preview_section;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc preview section instance.
 *
 *  Allocate memory for the afs_toc_preview_section type
 *  and initializes all structure data with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in] id         Preview section id.
 *  \param[in] x          X coordinate of the beginning of the section.
 *  \param[in] y          Y coordinate of the beginning of the section.
 *  \param[in] width      Width of the section.
 *  \param[in] height     Height of the section.
 *  \param[in] rotation   Section rotation.
 *  \return instance of allocated afs_toc_metadata_source structure.
 */

afs_toc_preview_section * afs_toc_preview_section_create2(unsigned int id, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int rotation)
{
    afs_toc_preview_section * toc_preview_section = malloc(sizeof(afs_toc_preview_section));
    afs_toc_preview_section_init(toc_preview_section, id, x, y, width, height, rotation);
    
    return toc_preview_section;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers with specified input values.
 *
 *  Initializes all input structure pointers with specified input values.
 *  If input afs_toc_preview_section pointer is NULL, then return without initialization.
 *
 *  \param[in] toc_preview_section  Pointer to the afs_toc_preview_section structure.
 *  \param[in] id                   Preview section id.
 *  \param[in] x                    X coordinate of the beginning of the section.
 *  \param[in] y                    Y coordinate of the beginning of the section.
 *  \param[in] width                Width of the section.
 *  \param[in] height               Height of the section.
 *  \param[in] rotation             Section rotation.
 */

void afs_toc_preview_section_init(afs_toc_preview_section * toc_preview_section, unsigned int id, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int rotation)
{
    if (NULL == toc_preview_section)
    {
        return;
    }
    
    toc_preview_section->id = id;
    toc_preview_section->x = x;
    toc_preview_section->y = y;
    toc_preview_section->width = width;
    toc_preview_section->height = height;
    toc_preview_section->rotation = rotation;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_preview_section structure.
 *
 *  Frees occupied memory of the afs_toc_preview_section structure.
 *
 *  \param[in]  toc_preview_section  Pointer to the afs_toc_preview_section structure.
 */

void afs_toc_preview_section_free(afs_toc_preview_section * toc_preview_section)
{
    free(toc_preview_section);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_preview_section structure.
 *
 *  Function create a copy of input afs_toc_preview_section structure and return it.
 *  If TOC preview section pointer is NULL function return NULL.
 *
 *  \param[in]  toc_preview_section  Pointer to the afs_toc_preview_section structure.
 *  \return new copy of afs_toc_preview_section structure or NULL.
 */

afs_toc_preview_section * afs_toc_preview_section_clone(const afs_toc_preview_section * toc_preview_section)
{
    if (toc_preview_section == NULL)
    {
        return NULL;
    }

    afs_toc_preview_section * return_copy = afs_toc_preview_section_create2(
        toc_preview_section->id, 
        toc_preview_section->x, 
        toc_preview_section->y,
        toc_preview_section->width,
        toc_preview_section->height,
        toc_preview_section->rotation);

    return return_copy;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_preview_section structures on the identity.
 *
 *  Function checks two instances of the afs_toc_preview_section structures on the identity.
 *
 *  \param[in]   toc_preview_section1  Pointer to the first instance of the afs_toc_preview_section structure.
 *  \param[in]   toc_preview_section2  Pointer to the second instance of the afs_toc_preview_section structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_preview_section_equal(const afs_toc_preview_section * toc_preview_section1, const afs_toc_preview_section * toc_preview_section2)
{
    if (toc_preview_section1 == NULL && toc_preview_section2 == NULL)
    {
        return DTRUE;
    }

    if (toc_preview_section1 == NULL || toc_preview_section2 == NULL)
    {
        return DFALSE;
    }

    return (toc_preview_section1->id == toc_preview_section2->id &&
            toc_preview_section1->x == toc_preview_section2->x &&
            toc_preview_section1->y == toc_preview_section2->y &&
            toc_preview_section1->width == toc_preview_section2->width &&
            toc_preview_section1->height == toc_preview_section2->height &&
            toc_preview_section1->rotation == toc_preview_section2->rotation) ? DTRUE : DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the afs_toc_preview_section structure.
 *
 *  The function returns the sign of integrity the current structure.
 *  If TOC preview section pointer is NULL then function return DFALSE.
 *
 *  \param[in]   toc_preview_section   Pointer to the afs_toc_preview_section structure.
 *  \return the sign of integrity the afs_toc_preview_section structure.
 */

DBOOL afs_toc_preview_section_is_valid(const afs_toc_preview_section * toc_preview_section)
{
    if (toc_preview_section == NULL)
    {
        return DFALSE;
    }

    if (toc_preview_section->rotation != 0 && 
        toc_preview_section->rotation != 90 && 
        toc_preview_section->rotation != 180 && 
        toc_preview_section->rotation != 270)
    {
        return DFALSE;
    }

    if (toc_preview_section->width == 0 || toc_preview_section->height == 0)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the width of the content section.
 *
 *  The function returns the width of the content section.
 *  If TOC preview section pointer is NULL then function return zero.
 *
 *  \param[in]   toc_preview_section   Pointer to the afs_toc_preview_section structure.
 *  \return width of the content section or zero.
 */

unsigned int afs_toc_preview_section_content_width(const afs_toc_preview_section * toc_preview_section)
{
    if (toc_preview_section == NULL || toc_preview_section->rotation % 90 != 0)
    {
        return 0;
    }

    return toc_preview_section->rotation % 180 == 0 ? toc_preview_section->width : toc_preview_section->height;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the height of the content section.
 *
 *  The function returns the height of the content section.
 *  If TOC preview section pointer is NULL then function return zero.
 *
 *  \param[in]   toc_preview_section   Pointer to the afs_toc_preview_section structure.
 *  \return height of the content section or zero.
 */

unsigned int afs_toc_preview_section_content_height(const afs_toc_preview_section * toc_preview_section)
{
    if (toc_preview_section == NULL || toc_preview_section->rotation % 90 != 0)
    {
        return 0;
    }

    return toc_preview_section->rotation % 180 == 0 ? toc_preview_section->height : toc_preview_section->width;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_preview_section structures for overlap.
 *
 *  Function checks two instances of the afs_toc_preview_section structures for overlap.
 *
 *  \param[in]   toc_preview_section1  Pointer to the first instance of the afs_toc_preview_section structure.
 *  \param[in]   toc_preview_section2  Pointer to the second instance of the afs_toc_preview_section structure.
 *  \return sign of overlap of the input structures.
 */

DBOOL afs_toc_preview_section_is_overlap(const afs_toc_preview_section * toc_preview_section1, const afs_toc_preview_section * toc_preview_section2)
{
    if (afs_toc_preview_section_is_valid(toc_preview_section1) == DFALSE || afs_toc_preview_section_is_valid(toc_preview_section2) == DFALSE)
    {
        return DFALSE;
    }

    return toc_preview_section1->x <= (toc_preview_section2->x + toc_preview_section2->width - 1) &&
           (toc_preview_section1->x + toc_preview_section1->width - 1) >= toc_preview_section2->x &&
           toc_preview_section1->y <= (toc_preview_section2->y + toc_preview_section2->height - 1) &&
           (toc_preview_section1->y + toc_preview_section1->height - 1) >= toc_preview_section2->y ? DTRUE : DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_preview_section structure to the XML file.
 *
 *  Function translates the input afs_toc_preview_section structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_preview_section  Pointer to the afs_toc_preview_section structure.
 *  \param[in]   file_name            Name of the XML file.
 *  \param[in]   compact              If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_section_save_file(const afs_toc_preview_section * toc_preview_section, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_preview_section == NULL || afs_toc_preview_section_is_valid(toc_preview_section) == DFALSE)
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

    if (!afs_toc_preview_section_save_xml(toc_preview_section, tree))
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
 *  \brief Function translates the input afs_toc_preview_section structure to the XML string.
 *
 *  Function translates the input afs_toc_preview_section structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_preview_section  Pointer to the afs_toc_preview_section structure.
 *  \param[in]   compact              If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_preview_section_save_string(const afs_toc_preview_section * toc_preview_section, DBOOL compact)
{
    if (toc_preview_section == NULL)
    {
        return DFALSE;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_preview_section_save_xml(toc_preview_section, document))
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
 *  \brief Function translates the input afs_toc_preview_section structure to the XML nodes.
 *
 *  Function translates the input afs_toc_preview_section structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_preview_section   Pointer to the afs_toc_preview_section structure.
 *  \param[out]  out                   Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_section_save_xml(const afs_toc_preview_section * toc_preview_section, mxml_node_t* out)
{
    if (out == NULL || toc_preview_section == NULL || afs_toc_preview_section_is_valid(toc_preview_section) == DFALSE)
    {
        return DFALSE;
    }

    mxml_node_t * section_node = mxmlNewElement(out, "section");

    mxmlElementSetAttrf(section_node, "id", "%u", toc_preview_section->id);
    mxmlElementSetAttrf(section_node, "x", "%u", toc_preview_section->x);
    mxmlElementSetAttrf(section_node, "y", "%u", toc_preview_section->y);
    mxmlElementSetAttrf(section_node, "w", "%u", toc_preview_section->width);
    mxmlElementSetAttrf(section_node, "h", "%u", toc_preview_section->height);
    mxmlElementSetAttrf(section_node, "rotation", "%u", toc_preview_section->rotation);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_preview_section structure.
 *
 *  Function translates the input XML nodes to the afs_toc_preview_section structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_section  Pointer to the afs_toc_preview_section structure.
 *  \param[in]   file_name            Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_section_load_file(afs_toc_preview_section * toc_preview_section, const char * file_name)
{
    if (file_name == NULL || toc_preview_section == NULL)
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

    DBOOL return_value = afs_toc_preview_section_load_xml(toc_preview_section, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_preview_section structure.
 *
 *  Function translates the input XML string to the afs_toc_preview_section structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_section  Pointer to the afs_toc_preview_section structure.
 *  \param[in]   in                   Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_section_load_string(afs_toc_preview_section * toc_preview_section, const char * in)
{
    if (in == NULL || boxing_string_equal(in, "") || toc_preview_section == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_preview_section_load_xml(toc_preview_section, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC preview section structure.
 *
 *  Function translates the input XML nodes to the TOC preview section structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_section  Pointer to the afs_toc_preview_section structure.
 *  \param[in]   input_node           Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_section_load_xml(afs_toc_preview_section * toc_preview_section, mxml_node_t* input_node)
{
    if (input_node == NULL || toc_preview_section == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * section_node = NULL;

    if (boxing_string_equal(mxmlGetElement(input_node), "section") == DTRUE)
    {
        section_node = input_node;
    }
    else
    {
        section_node = mxmlFindElement(input_node, input_node, "section", NULL, NULL, MXML_DESCEND);
    }

    if (section_node == NULL)
    {
        return DFALSE;
    }

    if (afs_xmlutils_get_unsigned_integer_attribute(&toc_preview_section->id, section_node, "id") == DFALSE)
    {
        return DFALSE;
    }
    if (afs_xmlutils_get_unsigned_integer_attribute(&toc_preview_section->x, section_node, "x") == DFALSE)
    {
        return DFALSE;
    }
    if (afs_xmlutils_get_unsigned_integer_attribute(&toc_preview_section->y, section_node, "y") == DFALSE)
    {
        return DFALSE;
    }
    if (afs_xmlutils_get_unsigned_integer_attribute(&toc_preview_section->width, section_node, "w") == DFALSE)
    {
        return DFALSE;
    }
    if (afs_xmlutils_get_unsigned_integer_attribute(&toc_preview_section->height, section_node, "h") == DFALSE)
    {
        return DFALSE;
    }
    if (afs_xmlutils_get_unsigned_integer_attribute(&toc_preview_section->rotation, section_node, "rotation") == DFALSE)
    {
        return DFALSE;
    }

    return afs_toc_preview_section_is_valid(toc_preview_section);
}


//----------------------------------------------------------------------------
/*!
  * \} end of preview group
  */


// PRIVATE AFS TOC PREVIEW SECTION FUNCTIONS
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
    
    if (boxing_string_equal("section", name))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n");
        }
    }

    return (NULL);
}
