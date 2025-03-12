/*****************************************************************************
**
**  Implementation of structure afs_toc_preview_section methods C interface
**
**  Creation date:    2017/12/01
**  Created by:       Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "toc/previewsections.h"
#include "boxing/string.h"
#include "boxing/log.h"

// PRIVATE INTERFACE
//
static const char * whitespace_cb(mxml_node_t *node, int where);


/*! 
  * \addtogroup preview
  * \{
  */


/*! 
 *  \typedef  gvector afs_toc_preview_sections_vector
 *  \brief Array of afs_toc_preview_section structure instances.
 *
 *  Array of afs_toc_preview_section structure instances with the ability to add new instances.
 */


// PUBLIC AFS TOC PEVIEW SECTIONS FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_preview_sections_s  previewsections.h
 *  \brief      TOC preview sections storage.
 *
 *  \param sections  Vector with afs_toc_preview_section pointers.
 *
 *  Structure for storing TOC preview sections.
 */


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc preview sections instance.
 *
 *  Allocate memory for the afs_toc_preview_sections type
 *  and initializing the vector to store pointers.
 *  Return instance of allocated vector.
 *
 *  \return instance of allocated afs_toc_preview_sections structure.
 */

afs_toc_preview_sections * afs_toc_preview_sections_create()
{
    afs_toc_preview_sections * toc_preview_sections = malloc(sizeof(afs_toc_preview_sections));
    toc_preview_sections->sections = NULL;
    toc_preview_sections->reference_count = 1;
    return toc_preview_sections;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc preview sections instance from XML nodes.
 *
 *  Allocate memory for the afs_toc_preview_sections type
 *  and translates the input XML nodes to the toc preview sections vector.
 *  Return instance of allocated vector.
 *
 *  \return instance of allocated afs_toc_preview_sections vector.
 */

afs_toc_preview_sections * afs_toc_preview_sections_create2(mxml_node_t * sections_node)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    afs_toc_preview_sections_load_xml(toc_preview_sections, sections_node);
    return toc_preview_sections;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_preview_sections structure.
 *
 *  Frees occupied memory of the afs_toc_preview_sections structure.
 *
 *  \param[in]  toc_preview_sections  Pointer to the afs_toc_preview_sections structure.
 */

void afs_toc_preview_sections_free(afs_toc_preview_sections * toc_preview_sections)
{
    if (toc_preview_sections == NULL)
    {
        return;
    }

    toc_preview_sections->reference_count--;

    if (toc_preview_sections->reference_count == 0)
    {
        if (toc_preview_sections->sections != NULL)
        {
            for (unsigned int i = 0; i < toc_preview_sections->sections->size; i++)
            {
                afs_toc_preview_section_free(GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, i));
                GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, i) = NULL;
            }

            gvector_free(toc_preview_sections->sections);
            toc_preview_sections->sections = NULL;
        }

        free(toc_preview_sections);
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_preview_sections structure.
 *
 *  Function create a copy of input afs_toc_preview_sections structure and return it.
 *  If TOC preview sections pointer is NULL function return NULL.
 *
 *  \param[in]  toc_preview_sections  Pointer to the afs_toc_preview_sections structure.
 *  \return new copy of afs_toc_preview_sections structure or NULL.
 */

afs_toc_preview_sections * afs_toc_preview_sections_clone(const afs_toc_preview_sections * toc_preview_sections)
{
    // If TOC preview sections pointer is NULL return NULL.
    if (toc_preview_sections == NULL)
    {
        return NULL;
    }

    if (toc_preview_sections->sections == NULL)
    {
        return afs_toc_preview_sections_create();
    }

    afs_toc_preview_sections * return_toc_preview_sections = afs_toc_preview_sections_create();
    return_toc_preview_sections->sections = gvector_create_pointers(toc_preview_sections->sections->size);
    
    for (unsigned int i = 0; i < toc_preview_sections->sections->size; i++)
    {
        afs_toc_preview_section * toc_preview_section = GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, i);
        GVECTORN(return_toc_preview_sections->sections, afs_toc_preview_section *, i) = afs_toc_preview_section_clone(toc_preview_section);
    }

    return return_toc_preview_sections;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function returns a new reference to the input afs_toc_preview_sections structure.
 *
 *  Function returns a new reference to the input afs_toc_preview_sections structure.
 *  The reference count is incremented by 1.
 *  If TOC preview sections pointer is NULL function return NULL.
 *
 *  \param[in]  toc_preview_sections  Pointer to the afs_toc_preview_sections structure.
 *  \return new reference of afs_toc_preview_sections structure or NULL.
 */

afs_toc_preview_sections * afs_toc_preview_sections_get_new_reference(afs_toc_preview_sections * toc_preview_sections)
{
    // If TOC preview sections pointer is NULL return NULL.
    if (toc_preview_sections == NULL)
    {
        return NULL;
    }

    toc_preview_sections->reference_count++;
    return toc_preview_sections;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_preview_sections structures on the identity.
 *
 *  Function checks two instances of the afs_toc_preview_sections structures on the identity.
 *
 *  \param[in]   toc_preview_sections1  Pointer to the first instance of the afs_toc_preview_sections structure.
 *  \param[in]   toc_preview_sections2  Pointer to the second instance of the afs_toc_preview_sections structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_preview_sections_equal(const afs_toc_preview_sections * toc_preview_sections1, const afs_toc_preview_sections * toc_preview_sections2)
{
    if (toc_preview_sections1 == NULL && toc_preview_sections2 == NULL)
    {
        return DTRUE;
    }

    if (toc_preview_sections1 == NULL || toc_preview_sections2 == NULL)
    {
        return DFALSE;
    }

    if (toc_preview_sections1->sections == NULL && toc_preview_sections2->sections == NULL)
    {
        return DTRUE;
    }

    if (toc_preview_sections1->sections == NULL || toc_preview_sections2->sections == NULL)
    {
        return DFALSE;
    }

    if (toc_preview_sections1->sections->size != toc_preview_sections2->sections->size)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_preview_sections1->sections->size; i++)
    {
        afs_toc_preview_section * toc_preview_section1 = GVECTORN(toc_preview_sections1->sections, afs_toc_preview_section *, i);
        afs_toc_preview_section * toc_preview_section2 = GVECTORN(toc_preview_sections2->sections, afs_toc_preview_section *, i);

        if (afs_toc_preview_section_equal(toc_preview_section1, toc_preview_section2) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the all afs_toc_preview_section structures in the input afs_toc_preview_sections structure.
 *
 *  The function returns the sign of integrity the all afs_toc_preview_section structures in the input afs_toc_preview_sections structure.
 *  If TOC preview sections pointer is NULL or internal sections vector pointer is NULL or vector has no pointers then function return DFALSE.
 *
 *  \param[in]   toc_preview_sections   Pointer to the afs_toc_preview_sections structure.
 *  \return the sign of integrity of all afs_toc_preview_section structures.
 */

DBOOL afs_toc_preview_sections_is_valid(const afs_toc_preview_sections * toc_preview_sections)
{
    if (toc_preview_sections == NULL)
    {
        return DFALSE;
    }

    if (toc_preview_sections->sections == NULL)
    {
        return DFALSE;
    }

    if (toc_preview_sections->sections->size == 0)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_preview_sections->sections->size; i++)
    {
        afs_toc_preview_section * toc_preview_section1 = GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, i);

        if (afs_toc_preview_section_is_valid(toc_preview_section1) == DFALSE)
        {
            return DFALSE;
        }

        for (unsigned int j = i + 1; j < toc_preview_sections->sections->size; j++)
        {
            afs_toc_preview_section * toc_preview_section2 = GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, j);

            if (afs_toc_preview_section_is_overlap(toc_preview_section1, toc_preview_section2))
            {
                return DFALSE;
            }
        }

    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_preview_section structure.
 *
 *  The function returns a count of instances of afs_toc_preview_section structure.
 *  If input afs_toc_preview_sections pointer is NULL or internal sections vector pointer is NULL then function returns zero.
 *
 *  \param[in]   toc_preview_sections  Pointer to the instance of the afs_toc_preview_sections structure.
 *  \return a count of instances of afs_toc_preview_section structure or zero.
 */

unsigned int afs_toc_preview_sections_size(const afs_toc_preview_sections * toc_preview_sections)
{
    if (toc_preview_sections == NULL)
    {
        return 0;
    }

    if (toc_preview_sections->sections == NULL)
    {
        return 0;
    }

    return (unsigned int)toc_preview_sections->sections->size;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_preview_section structure to the input afs_toc_preview_sections structure.
 *
 *  The function adds a new instance of afs_toc_preview_section structure to the input afs_toc_preview_sections structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_preview_section structure is not add and the function returns DFALSE.
 *  If input afs_toc_preview_section instance is not valid then new instance is not add, the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_preview_sections  Pointer to the instance of the afs_toc_preview_sections structure.
 *  \param[in]   toc_preview_section   Pointer to the instance of the afs_toc_preview_section structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_sections_append(afs_toc_preview_sections * toc_preview_sections, afs_toc_preview_section * toc_preview_section)
{
    if (toc_preview_sections == NULL || toc_preview_section == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_preview_section_is_valid(toc_preview_section) != DTRUE)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < afs_toc_preview_sections_size(toc_preview_sections); i++)
    {
        // Duplicate id not allowed
        afs_toc_preview_section * current_toc_preview_section = afs_toc_preview_sections_get(toc_preview_sections, i);

        if (current_toc_preview_section->id == toc_preview_section->id)
        {
            return DFALSE;
        }

        // Overlapping region not allowed
        // A.LO<=B.HI && A.HI>=B.LO
        if (afs_toc_preview_section_is_overlap(current_toc_preview_section, toc_preview_section) == DTRUE)
        {
            return DFALSE;
        }
    }

    if (toc_preview_sections->sections == NULL)
    {
        toc_preview_sections->sections = gvector_create_pointers(0);
    }

    size_t old_size = toc_preview_sections->sections->size;

    gvector_append_data(toc_preview_sections->sections, 1, &toc_preview_section);

    if (toc_preview_sections->sections->size != old_size + 1)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_preview_section structure according to the input index.
 *
 *  The function returns an instance of afs_toc_preview_section structure according to the input index.
 *
 *  \param[in]   toc_preview_sections  Pointer to the instance of the afs_toc_preview_sections structure.
 *  \param[in]   index                 Index for specifying the required instance of the afs_toc_preview_section structure in the storage vector.
 *  \return an instance of afs_toc_preview_section structure or NULL.
 */

afs_toc_preview_section * afs_toc_preview_sections_get(const afs_toc_preview_sections * toc_preview_sections, unsigned int index)
{
    if (toc_preview_sections == NULL)
    {
        return NULL;
    }

    if (toc_preview_sections->sections == NULL)
    {
        return NULL;
    }

    if (index >= toc_preview_sections->sections->size)
    {
        return NULL;
    }

    return GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_preview_sections structure to the XML file.
 *
 *  Function translates the input afs_toc_preview_sections structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_preview_sections Pointer to the afs_toc_preview_sections structure.
 *  \param[in]   file_name            Name of the XML file.
 *  \param[in]   compact              If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_sections_save_file(const afs_toc_preview_sections * toc_preview_sections, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_preview_sections == NULL || afs_toc_preview_sections_is_valid(toc_preview_sections) == DFALSE)
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

    if (!afs_toc_preview_sections_save_xml(toc_preview_sections, tree))
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
 *  \brief Function translates the input afs_toc_preview_sections structure to the XML string.
 *
 *  Function translates the input afs_toc_preview_sections structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_preview_sections Pointer to the afs_toc_preview_sections structure.
 *  \param[in]   compact              If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_preview_sections_save_string(const afs_toc_preview_sections * toc_preview_sections, DBOOL compact)
{
    // If TOC preview sections pointer is NULL return DFALSE
    if (toc_preview_sections == NULL)
    {
        return DFALSE;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_preview_sections_save_xml(toc_preview_sections, document))
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
 *  \brief Function translates the input afs_toc_preview_sections structure to the XML nodes.
 *
 *  Function translates the input afs_toc_preview_sections structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_preview_sections  Pointer to the afs_toc_preview_sections structure.
 *  \param[out]  out                   Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_sections_save_xml(const afs_toc_preview_sections * toc_preview_sections, mxml_node_t* out)
{
    // If output node pointer is NULL or TOC preview sections pointer is NULL return DFALSE
    if (out == NULL || toc_preview_sections == NULL || afs_toc_preview_sections_is_valid(toc_preview_sections) == DFALSE)
    {
        return DFALSE;
    }

    mxml_node_t * sections_node = mxmlNewElement(out, "sections");

    for (unsigned int i = 0; i < toc_preview_sections->sections->size; i++)
    {
        DBOOL result = afs_toc_preview_section_save_xml(GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, i), sections_node);

        if (result == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_preview_sections structure.
 *
 *  Function translates the input XML nodes to the afs_toc_preview_sections structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_sections Pointer to the afs_toc_preview_sections structure.
 *  \param[in]   file_name            Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_sections_load_file(afs_toc_preview_sections * toc_preview_sections, const char * file_name)
{
    // If input file name string pointer is NULL or TOC preview sections pointer is NULL return DFALSE
    if (file_name == NULL || toc_preview_sections == NULL)
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

    DBOOL return_value = afs_toc_preview_sections_load_xml(toc_preview_sections, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_preview_sections structure.
 *
 *  Function translates the input XML string to the afs_toc_preview_sections structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_sections Pointer to the afs_toc_preview_sections structure.
 *  \param[in]   in                   Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_sections_load_string(afs_toc_preview_sections * toc_preview_sections, const char * in)
{
    // If input string pointer is NULL or TOC preview sections pointer is NULL return DFALSE
    if (in == NULL || boxing_string_equal(in, "") || toc_preview_sections == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_preview_sections_load_xml(toc_preview_sections, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC preview sections structure.
 *
 *  Function translates the input XML nodes to the TOC preview sections structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_sections Pointer to the afs_toc_preview_sections structure.
 *  \param[in]   input_node           Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_sections_load_xml(afs_toc_preview_sections * toc_preview_sections, mxml_node_t * input_node)
{
    // If input node pointer is NULL or TOC preview sections pointer is NULL return DFALSE
    if (input_node == NULL || toc_preview_sections == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * sections_node = boxing_string_equal(mxmlGetElement(input_node), "sections") == DTRUE ? input_node : mxmlFindElement(input_node, input_node, "sections", NULL, NULL, MXML_DESCEND);

    if (sections_node == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * section_node = mxmlFindElement(sections_node, sections_node, "section", NULL, NULL, MXML_DESCEND);

    while (section_node != NULL)
    {
        if (mxmlGetType(section_node) == MXML_ELEMENT)
        {
            afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

            DBOOL load_section_result = afs_toc_preview_section_load_xml(toc_preview_section, section_node);

            if (load_section_result == DFALSE)
            {
                DLOG_INFO("Has no TOC preview section.\n");
                return DFALSE;
            }

            DBOOL add_section_result = afs_toc_preview_sections_append(toc_preview_sections, toc_preview_section);

            if (add_section_result == DFALSE)
            {
                afs_toc_preview_section_free(toc_preview_section);
                return DFALSE;
            }
        }

        section_node = mxmlFindElement(section_node, sections_node, "section", NULL, NULL, MXML_DESCEND);
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of preview group
  */


// PRIVATE AFS TOC PREVIEW SECTIONS FUNCTIONS
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
