/*****************************************************************************
**
**  Implementation of structure toc_preview_layout_definition methods C interface
**
**  Creation date:  2016/08/18
**  Created by:     Alexander Silokhin
**
**
**  Copyright (c) 2016 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "toc/previewlayoutdefinition.h"
#include "boxing/platform/memory.h"
#include "boxing/string.h"
#include "xmlutils.h"

// PRIVATE INTERFACE
//

static const char * whitespace_cb(mxml_node_t *node, int where);


/*! 
  * \addtogroup preview
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_preview_layout_definition_s  previewlayoutdefinition.h
 *  \brief      Frame preview layout.
 *
 *  \param id        Preview layout definition ID.
 *  \param name      Preview layout definition name.
 *  \param sections  Preview layout definition sections storage vector.
 *
 *  The preview layout definition spilt a frame into sections, where each section 
 *  can store a page/image from a file. The preview block of a toc_data_file_s 
 *  refer to the layout definition, multiple file instances can refer to the same
 *  definition.
 */

// PUBLIC AFS TOC PREVIEW LAYOUT DEFINITION FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create an empty TOC preview layout definition instance.
 *
 *  Allocate memory for the afs_toc_preview_layout_definition type
 *  and initializes vector for storing TOC afs_toc_preview_layout_definition sections.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_preview_layout_definition structure.
 */

afs_toc_preview_layout_definition * afs_toc_preview_layout_definition_create()
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = malloc(sizeof(afs_toc_preview_layout_definition));
    afs_toc_preview_layout_definition_init2(toc_preview_layout_definition, NULL, NULL, NULL);

    return toc_preview_layout_definition;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc preview layout definition instance.
 *
 *  Allocate memory for the afs_toc_preview_layout_definition type
 *  and initializes id and name with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in]  id   Preview layout definition id.
 *  \param[in]  name Preview layout definition name.
 *  \return instance of allocated afs_toc_preview_layout_definition structure.
 */

afs_toc_preview_layout_definition * afs_toc_preview_layout_definition_create2( const char * id, const char * name )
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = malloc(sizeof(afs_toc_preview_layout_definition));
    toc_preview_layout_definition->sections = NULL;
    afs_toc_preview_layout_definition_init(toc_preview_layout_definition, id, name);
    
    return toc_preview_layout_definition;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc preview layout definition instance.
 *
 *  Allocate memory for the afs_toc_preview_layout_definition type
 *  and initializes id, name and sections vector with specified input values.
 *  Return instance of allocated structure.
 *
 *  \param[in]  id       Preview layout definition id.
 *  \param[in]  name     Preview layout definition name.
 *  \param[in]  sections TOC preview sections vector.
 *  \return instance of allocated afs_toc_preview_layout_definition structure.
 */

afs_toc_preview_layout_definition * afs_toc_preview_layout_definition_create3(const char * id, const char * name, afs_toc_preview_sections* sections)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = malloc(sizeof(afs_toc_preview_layout_definition));
    afs_toc_preview_layout_definition_init2(toc_preview_layout_definition, id, name, sections);
    
    return toc_preview_layout_definition;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC preview layout definition instance from XML nodes.
 *
 *  Allocate memory for the afs_toc_preview_layout_definition type
 *  and translates the input XML nodes to the TOC preview layout definition structure.
 *  Return instance of allocated structure.
 *
 *  \param[in]   definition_node  Pointer to the definition XML node.
 *  \return instance of allocated afs_toc_preview_layout_definition structure.
 */

afs_toc_preview_layout_definition * afs_toc_preview_layout_definition_create4(mxml_node_t* definition_node)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_layout_definition_load_xml(toc_preview_layout_definition, definition_node);
    return toc_preview_layout_definition;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers with specified input values.
 *
 *  Initializes all input structure pointers with specified input values.
 *  If input afs_toc_preview_layout_definition pointer is NULL, then return without initialization.
 *
 *  \param[in]  toc_preview_layout_definition  Pointer to the afs_toc_preview_layout_definition structure.
 *  \param[in]  id                             Preview layout definition id.
 *  \param[in]  name                           Preview layout definition name.
 */

void afs_toc_preview_layout_definition_init(afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * id, const char * name)
{
    if (toc_preview_layout_definition == NULL)
    {
        return;
    }
    
    toc_preview_layout_definition->id = boxing_string_clone(id);
    toc_preview_layout_definition->name = boxing_string_clone(name);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure pointers with specified input values.
 *
 *  Initializes all input structure pointers with specified input values.
 *  If input afs_toc_preview_layout_definition pointer is NULL, then return without initialization.
 *
 *  \param[in]  toc_preview_layout_definition  Pointer to the afs_toc_preview_layout_definition structure.
 *  \param[in]  id                             Preview layout definition id.
 *  \param[in]  name                           Preview layout definition name.
 *  \param[in]  sections                       TOC preview sections vector.
 */

void afs_toc_preview_layout_definition_init2(afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * id, const char * name, afs_toc_preview_sections * sections)
{
    if (toc_preview_layout_definition == NULL)
    {
        return;
    }
    
    toc_preview_layout_definition->id = boxing_string_clone(id);
    toc_preview_layout_definition->name = boxing_string_clone(name);
    toc_preview_layout_definition->sections = sections;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_preview_layout_definition structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_preview_layout_definition  Pointer to the afs_toc_preview_layout_definition structure.
 */

void afs_toc_preview_layout_definition_free(afs_toc_preview_layout_definition * toc_preview_layout_definition)
{
    if (toc_preview_layout_definition != NULL)
    {
        boxing_string_free(toc_preview_layout_definition->id);
        boxing_string_free(toc_preview_layout_definition->name);
        afs_toc_preview_sections_free(toc_preview_layout_definition->sections);
        boxing_memory_free(toc_preview_layout_definition);
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_preview_layout_definition structure.
 *
 *  Function create a copy of input afs_toc_preview_layout_definition structure and return it.
 *  If TOC preview layout definition pointer is NULL function return NULL.
 *
 *  \param[in]  toc_preview_layout_definition  Pointer to the afs_toc_preview_layout_definition structure.
 *  \return new copy of afs_toc_preview_layout_definition structure or NULL.
 */

afs_toc_preview_layout_definition * afs_toc_preview_layout_definition_clone(const afs_toc_preview_layout_definition * toc_preview_layout_definition)
{
    if (toc_preview_layout_definition == NULL)
    {
        return NULL;
    }

    afs_toc_preview_layout_definition * return_copy = afs_toc_preview_layout_definition_create2(toc_preview_layout_definition->id, toc_preview_layout_definition->name);

    return_copy->sections = afs_toc_preview_sections_clone(toc_preview_layout_definition->sections);

    return return_copy;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_preview_layout_definition structures on the identity.
 *
 *  Function checks two instances of the afs_toc_preview_layout_definition structures on the identity.
 *
 *  \param[in]   toc_preview_layout_definition1  Pointer to the first instance of the afs_toc_preview_layout_definition structure.
 *  \param[in]   toc_preview_layout_definition2  Pointer to the second instance of the afs_toc_preview_layout_definition structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_preview_layout_definition_equal(const afs_toc_preview_layout_definition * toc_preview_layout_definition1, const afs_toc_preview_layout_definition * toc_preview_layout_definition2)
{
    if (toc_preview_layout_definition1 == NULL && toc_preview_layout_definition2 == NULL)
    {
        return DTRUE;
    }

    if (toc_preview_layout_definition1 == NULL || toc_preview_layout_definition2 == NULL)
    {
        return DFALSE;
    }

    DBOOL sections_identity = afs_toc_preview_sections_equal(toc_preview_layout_definition1->sections, toc_preview_layout_definition2->sections);

    if (sections_identity == DFALSE ||
        boxing_string_equal(toc_preview_layout_definition1->id, toc_preview_layout_definition2->id) == DFALSE ||
        boxing_string_equal(toc_preview_layout_definition1->name, toc_preview_layout_definition2->name) == DFALSE)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the afs_toc_preview_layout_definition structure.
 *
 *  The function returns the sign of integrity the current structure.
 *  If TOC preview layout definition pointer is NULL then function return DFALSE.
 *
 *  \param[in]   toc_preview_layout_definition   Pointer to the afs_toc_preview_layout_definition structure.
 *  \return the sign of integrity the afs_toc_preview_layout_definition structure.
 */

DBOOL afs_toc_preview_layout_definition_is_valid(const afs_toc_preview_layout_definition * toc_preview_layout_definition)
{
    if (toc_preview_layout_definition == NULL)
    {
        return DFALSE;
    }

    if (toc_preview_layout_definition->id == NULL || boxing_string_equal(toc_preview_layout_definition->id, "") == DTRUE ||
        (toc_preview_layout_definition->name == NULL || boxing_string_equal(toc_preview_layout_definition->name, "") == DTRUE) ||
        afs_toc_preview_sections_is_valid(toc_preview_layout_definition->sections) == DFALSE)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_preview_section structure.
 *
 *  The function returns a count of instances of afs_toc_preview_section structure.
 *  If input afs_toc_preview_layout_definition pointer is NULL or internal afs_toc_preview_sections pointer is NULL then function returns zero.
 *
 *  \param[in]   toc_preview_layout_definition  Pointer to the instance of the afs_toc_preview_layout_definition.
 *  \return a count of instances of afs_toc_preview_section structure or zero.
 */

unsigned int afs_toc_preview_layout_definition_get_section_count(const afs_toc_preview_layout_definition * toc_preview_layout_definition)
{
    return (NULL != toc_preview_layout_definition && NULL != toc_preview_layout_definition->sections) ?
            afs_toc_preview_sections_size(toc_preview_layout_definition->sections) :
            0;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_preview_section structure according to the input index.
 *
 *  The function returns an instance of afs_toc_preview_section structure according to the input index.
 *
 *  \param[in]   toc_preview_layout_definition  Pointer to the instance of the afs_toc_preview_layout_definition structure.
 *  \param[in]   index                          Index for specifying the required instance of the afs_toc_preview_section structure in the storage structure.
 *  \return an instance of afs_toc_preview_section structure or NULL.
 */

afs_toc_preview_section * afs_toc_preview_layout_definition_get_section(const afs_toc_preview_layout_definition * toc_preview_layout_definition, unsigned int index)
{
    return (NULL != toc_preview_layout_definition && NULL != toc_preview_layout_definition->sections) ?
            afs_toc_preview_sections_get(toc_preview_layout_definition->sections, index) :
            NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_preview_section structure to the input afs_toc_preview_layout_definition structure.
 *
 *  The function adds a new instance of afs_toc_preview_section structure to the input afs_toc_preview_layout_definition structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_preview_section structure is not add and the function returns DFALSE.
 *  If input afs_toc_preview_section instance is not valid then new instance is not add the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_preview_layout_definition  Pointer to the instance of the afs_toc_preview_layout_definition structure.
 *  \param[in]   toc_preview_section            Pointer to the instance of the afs_toc_preview_section structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definition_add_section(afs_toc_preview_layout_definition * toc_preview_layout_definition, afs_toc_preview_section * toc_preview_section)
{
    if (toc_preview_layout_definition == NULL || afs_toc_preview_section_is_valid(toc_preview_section) == DFALSE)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition); i++)
    {
        // Duplicate id not allowed
        afs_toc_preview_section * current_toc_preview_section = afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, i);

        if (current_toc_preview_section->id == toc_preview_section->id)
        {
            return DFALSE;
        }

        // Overlapping region not allowed
        // A.LO<=B.HI && A.HI>=B.LO
        if (afs_toc_preview_section_is_overlap(current_toc_preview_section, toc_preview_section)  == DTRUE)
        {
            return DFALSE;
        }
    }

    if (toc_preview_layout_definition->sections == NULL)
    {
        toc_preview_layout_definition->sections = afs_toc_preview_sections_create();
    }

    return afs_toc_preview_sections_append(toc_preview_layout_definition->sections, toc_preview_section);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function checks the size of all sections for identity.
 *
 *  The function checks the size of all sections for identity.
 *  If input afs_toc_preview_layout_definition pointer is NULL or afs_toc_preview_layout_definition instance has no sections
 *  afs_toc_preview_layout_definition instance has only one section then function returns DTRUE.
 *  If the sizes of all sections are the same the function returns DTRUE else function return DFALSE.
 *
 *  \param[in]   toc_preview_layout_definition  Pointer to the instance of the afs_toc_preview_layout_definition structure.
 *  \return DTRUE if sizes of all sections are the same.
 */

DBOOL afs_toc_preview_layout_definition_is_section_size_equal(const afs_toc_preview_layout_definition * toc_preview_layout_definition)
{
    if (afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition) <= 1)
    {
        return DTRUE;
    }

    for (unsigned int i = 1; i < afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition); i++)
    {
        afs_toc_preview_section * first_toc_preview_section = afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, 0);
        afs_toc_preview_section * current_toc_preview_section = afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, i);

        if (first_toc_preview_section->width != current_toc_preview_section->width ||
            first_toc_preview_section->height != current_toc_preview_section->height)
        {
            return DFALSE;
        }
    }
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_preview_layout_definition structure to the XML file.
 *
 *  Function translates the input afs_toc_preview_layout_definition structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_preview_layout_definition  Pointer to the afs_toc_preview_layout_definition structure.
 *  \param[in]   file_name                      Name of the XML file.
 *  \param[in]   compact                        If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definition_save_file(const afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_preview_layout_definition == NULL)
    {
        return DFALSE;
    }

    mxml_node_t *tree = mxmlNewXML("1.0");

    if (!afs_toc_preview_layout_definition_save_xml(toc_preview_layout_definition, tree))
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
 *  \brief Function translates the input afs_toc_preview_layout_definition structure to the XML string.
 *
 *  Function translates the input afs_toc_preview_layout_definition structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_preview_layout_definition  Pointer to the afs_toc_preview_layout_definition structure.
 *  \param[in]   compact                        If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_preview_layout_definition_save_string(const afs_toc_preview_layout_definition * toc_preview_layout_definition, DBOOL compact)
{
    if (toc_preview_layout_definition == NULL)
    {
        return DFALSE;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_preview_layout_definition_save_xml(toc_preview_layout_definition, document))
    {
        mxmlDelete(document);
        return NULL;
    }

    char * xmlString;
    mxmlSetWrapMargin(0);

    if (compact)
    {
        xmlString = mxmlSaveAllocString( document, MXML_NO_CALLBACK );
    }
    else
    {
        xmlString = mxmlSaveAllocString( document, whitespace_cb );
    }

    mxmlDelete(document);

    return xmlString;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_preview_layout_definition structure to the XML nodes.
 *
 *  Function translates the input afs_toc_preview_layout_definition structure to the XML nodes.
 *  If input afs_toc_preview_layout_definition structure is not valid then function return DFALSE without translation.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_preview_layout_definition  Pointer to the afs_toc_preview_layout_definition structure.
 *  \param[out]  out                            Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definition_save_xml(const afs_toc_preview_layout_definition * toc_preview_layout_definition, mxml_node_t* out)
{
    if (out == NULL || toc_preview_layout_definition == NULL)
    {
        return DFALSE;
    }

    if (!afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition))
    {
        return DFALSE;
    }

    mxml_node_t * preview_node = mxmlNewElement(out, "previewLayout");
    afs_xmlutils_add_new_text_node(preview_node, "id", toc_preview_layout_definition->id);
    afs_xmlutils_add_new_text_node(preview_node, "name", toc_preview_layout_definition->name);

    return afs_toc_preview_sections_save_xml(toc_preview_layout_definition->sections, preview_node);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_preview_layout_definition structure.
 *
 *  Function translates the input XML nodes to the afs_toc_preview_layout_definition structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_layout_definition  Pointer to the afs_toc_preview_layout_definition structure.
 *  \param[in]   file_name                      Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definition_load_file(afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * file_name)
{
    if (file_name == NULL || toc_preview_layout_definition == NULL)
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

    DBOOL return_value = afs_toc_preview_layout_definition_load_xml(toc_preview_layout_definition, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_preview_layout_definition structure.
 *
 *  Function translates the input XML string to the afs_toc_preview_layout_definition structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_layout_definition  Pointer to the afs_toc_preview_layout_definition structure.
 *  \param[in]   in                             Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definition_load_string(afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * in)
{
    if (in == NULL || boxing_string_equal(in, "") || toc_preview_layout_definition == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_preview_layout_definition_load_xml(toc_preview_layout_definition, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC preview layout definition structure.
 *
 *  Function translates the input XML nodes to the TOC preview layout definition structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_layout_definition  Pointer to the afs_toc_preview_layout_definition structure.
 *  \param[in]   node                           Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definition_load_xml(afs_toc_preview_layout_definition * toc_preview_layout_definition, mxml_node_t* node)
{
    if (node == NULL || toc_preview_layout_definition == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * preview_node;

    if (boxing_string_equal(mxmlGetElement(node), "previewLayout") == DTRUE)
    {
        preview_node = node;
    }
    else
    {
        preview_node = mxmlFindElement(node, node, "previewLayout", NULL, NULL, MXML_DESCEND);
    }

    if (preview_node == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * id_node = mxmlFindElement(preview_node, preview_node, "id", NULL, NULL, MXML_DESCEND);
    toc_preview_layout_definition->id = id_node != NULL ? afs_xmlutils_get_node_text(id_node) : NULL;

    mxml_node_t * name_node = mxmlFindElement(preview_node, preview_node, "name", NULL, NULL, MXML_DESCEND);
    toc_preview_layout_definition->name = name_node != NULL ? afs_xmlutils_get_node_text(name_node) : NULL;

    toc_preview_layout_definition->sections = afs_toc_preview_sections_create();
    DBOOL result = afs_toc_preview_sections_load_xml(toc_preview_layout_definition->sections, preview_node);

    return result;
}


//----------------------------------------------------------------------------
/*!
  * \} end of preview group
  */


// PRIVATE AFS TOC PREVIEW LAYOUT DEFINITION FUNCTIONS
//

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

    if (boxing_string_equal("previewLayout", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("id", name) || boxing_string_equal("name", name) || boxing_string_equal("sections", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("sections", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("id", name) || boxing_string_equal("name", name))
    {
        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n        ");
        }
    }

    return (NULL);
}
