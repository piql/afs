/*****************************************************************************
**
**  Implementation of structure toc_preview_layout_definitions methods C interface
**
**  Creation date:    2017/12/21
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
#include "toc/previewlayoutdefinitions.h"
#include "boxing/string.h"
#include "boxing/log.h"
#include "boxing/math/math.h"
#include "mxml.h"


// PRIVATE INTERFACE
//

typedef struct layout_definitions_lengths_s
{
    unsigned int layout_id_length;
    unsigned int sections_length;
    unsigned int name_length;
    unsigned int section_id_length;
    unsigned int x_length;
    unsigned int y_length;
    unsigned int width_length;
    unsigned int height_length;
    unsigned int rotation_length;
    unsigned int sections_count;
} layout_definitions_lengths;

static const char * whitespace_cb(mxml_node_t *node, int where);
static unsigned int get_digits_count(unsigned int number);
static layout_definitions_lengths get_max_layout_id_lenght(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions);


/*! 
  * \addtogroup preview
  * \{
  */


/*! 
 *  \typedef  gvector afs_toc_preview_layout_definitions_vector
 *  \brief Array of afs_toc_preview_layout_definition structure instances.
 *
 *  Array of afs_toc_preview_layout_definition structure instances with the ability to add new instances.
 */


// PUBLIC AFS TOC PREVIEW LAYOUT DEFINITIONS FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_preview_layout_definitions_s  previewlayoutdefinitions.h
 *  \brief      Array of afs_toc_preview_layout_definition_s.
 *
 *  \param layout_definitions  Vector with afs_toc_preview_layout_definition pointers.
 *
 *  Structure for storing TOC preview layout definitions.
 */


//----------------------------------------------------------------------------
/*!
 *  \brief Create an toc preview layout definitions instance.
 *
 *  Allocate memory for the afs_toc_preview_layout_definitions type
 *  and initializing the vector to store pointers.
 *  Return instance of allocated vector.
 *
 *  \return instance of allocated afs_toc_preview_layout_definitions structure.
 */

afs_toc_preview_layout_definitions * afs_toc_preview_layout_definitions_create()
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = malloc(sizeof(afs_toc_preview_layout_definitions));
    toc_preview_layout_definitions->layout_definitions = NULL;
    toc_preview_layout_definitions->reference_count = 1;
    return toc_preview_layout_definitions;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_preview_layout_definitions structure.
 *
 *  Frees occupied memory of the afs_toc_preview_layout_definitions structure and all toc_preview_layout_definition structures.
 *
 *  \param[in]  toc_preview_layout_definitions  Pointer to the afs_toc_preview_layout_definitions structure.
 */

void afs_toc_preview_layout_definitions_free(afs_toc_preview_layout_definitions * toc_preview_layout_definitions)
{
    if (toc_preview_layout_definitions == NULL)
    {
        return;
    }

    toc_preview_layout_definitions->reference_count--;

    if (toc_preview_layout_definitions->reference_count == 0)
    {
        if (toc_preview_layout_definitions->layout_definitions != NULL)
        {
            for (unsigned int i = 0; i < toc_preview_layout_definitions->layout_definitions->size; i++)
            {
                afs_toc_preview_layout_definition_free(GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i));
                GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i) = NULL;
            }

            gvector_free(toc_preview_layout_definitions->layout_definitions);
            toc_preview_layout_definitions->layout_definitions = NULL;
        }

        free(toc_preview_layout_definitions);
    }
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_preview_layout_definitions structure.
 *
 *  Function create a copy of input afs_toc_preview_layout_definitions structure and return it.
 *  If TOC preview layout definitions pointer is NULL function return NULL.
 *
 *  \param[in]  toc_preview_layout_definitions  Pointer to the afs_toc_preview_layout_definitions structure.
 *  \return new copy of afs_toc_preview_layout_definitions structure or NULL.
 */

afs_toc_preview_layout_definitions * afs_toc_preview_layout_definitions_clone(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions)
{
    // If TOC preview layout definitions pointer is NULL return NULL.
    if (toc_preview_layout_definitions == NULL)
    {
        return NULL;
    }

    if (toc_preview_layout_definitions->layout_definitions == NULL)
    {
        return afs_toc_preview_layout_definitions_create();
    }

    afs_toc_preview_layout_definitions * return_toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    return_toc_preview_layout_definitions->layout_definitions = gvector_create_pointers(toc_preview_layout_definitions->layout_definitions->size);
    
    for (unsigned int i = 0; i < toc_preview_layout_definitions->layout_definitions->size; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i);
        GVECTORN(return_toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i) = afs_toc_preview_layout_definition_clone(toc_preview_layout_definition);
    }

    return return_toc_preview_layout_definitions;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function returns a new reference to the input afs_toc_preview_layout_definitions structure.
 *
 *  Function returns a new reference to the input afs_toc_preview_layout_definitions structure.
 *  The reference count is incremented by 1.
 *  If TOC preview layout definitions pointer is NULL function return NULL.
 *
 *  \param[in]  toc_preview_layout_definitions  Pointer to the afs_toc_preview_layout_definitions structure.
 *  \return new reference of afs_toc_preview_layout_definitions structure or NULL.
 */

afs_toc_preview_layout_definitions * afs_toc_preview_layout_definitions_get_new_reference(afs_toc_preview_layout_definitions * toc_preview_layout_definitions)
{
    // If TOC preview layout definitions pointer is NULL return NULL.
    if (toc_preview_layout_definitions == NULL)
    {
        return NULL;
    }

    toc_preview_layout_definitions->reference_count++;

    return toc_preview_layout_definitions;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_preview_layout_definitions structures on the identity.
 *
 *  Function checks two instances of the afs_toc_preview_layout_definitions structures on the identity.
 *
 *  \param[in]   toc_preview_layout_definitions1  Pointer to the first instance of the afs_toc_preview_layout_definitions structure.
 *  \param[in]   toc_preview_layout_definitions2  Pointer to the second instance of the afs_toc_preview_layout_definitions structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_preview_layout_definitions_equal(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions1, const afs_toc_preview_layout_definitions * toc_preview_layout_definitions2)
{
    if (toc_preview_layout_definitions1 == NULL && toc_preview_layout_definitions2 == NULL)
    {
        return DTRUE;
    }

    if (toc_preview_layout_definitions1 == NULL || toc_preview_layout_definitions2 == NULL)
    {
        return DFALSE;
    }

    if (toc_preview_layout_definitions1->layout_definitions == NULL && toc_preview_layout_definitions2->layout_definitions == NULL)
    {
        return DTRUE;
    }

    if (toc_preview_layout_definitions1->layout_definitions == NULL || toc_preview_layout_definitions2->layout_definitions == NULL)
    {
        return DFALSE;
    }

    if (toc_preview_layout_definitions1->layout_definitions->size != toc_preview_layout_definitions2->layout_definitions->size)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_preview_layout_definitions1->layout_definitions->size; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition1 = GVECTORN(toc_preview_layout_definitions1->layout_definitions, afs_toc_preview_layout_definition *, i);
        afs_toc_preview_layout_definition * toc_preview_layout_definition2 = GVECTORN(toc_preview_layout_definitions2->layout_definitions, afs_toc_preview_layout_definition *, i);

        if (afs_toc_preview_layout_definition_equal(toc_preview_layout_definition1, toc_preview_layout_definition2) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the all afs_toc_preview_layout_definition structures in the input afs_toc_preview_layout_definitions structure.
 *
 *  The function returns the sign of integrity the all afs_toc_preview_layout_definition structures in the input afs_toc_preview_layout_definitions structure.
 *  If TOC preview layout definitions pointer is NULL or internal definitions vector pointer is NULL or vector has no pointers then function return DFALSE.
 *
 *  \param[in]   toc_preview_layout_definitions   Pointer to the afs_toc_preview_layout_definitions structure.
 *  \return the sign of integrity of all afs_toc_preview_layout_definition structures.
 */

DBOOL afs_toc_preview_layout_definitions_is_valid(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions)
{
    if (toc_preview_layout_definitions == NULL)
    {
        return DFALSE;
    }

    if (toc_preview_layout_definitions->layout_definitions == NULL)
    {
        return DTRUE;
    }

    if (toc_preview_layout_definitions->layout_definitions->size == 0)
    {
        return DTRUE;
    }

    for (unsigned int i = 0; i < toc_preview_layout_definitions->layout_definitions->size; i++)
    {
        if (afs_toc_preview_layout_definition_is_valid(GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i)) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_preview_layout_definition structure.
 *
 *  The function returns a count of instances of afs_toc_preview_layout_definition structure.
 *  If input toc_preview_layout_definitions pointer is NULL or internal definitions vector pointer is NULL then function returns zero.
 *
 *  \param[in]   toc_preview_layout_definitions  Pointer to the instance of the afs_toc_preview_layout_definitions structure.
 *  \return a count of instances of afs_toc_preview_layout_definitions structure or zero.
 */

unsigned int afs_toc_preview_layout_definitions_get_count(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions)
{
    if (toc_preview_layout_definitions == NULL)
    {
        return 0;
    }

    if (toc_preview_layout_definitions->layout_definitions == NULL)
    {
        return 0;
    }

    return (unsigned int)toc_preview_layout_definitions->layout_definitions->size;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_preview_layout_definition structure according to the input index.
 *
 *  The function returns an instance of afs_toc_preview_layout_definition structure according to the input index.
 *
 *  \param[in]   toc_preview_layout_definitions  Pointer to the instance of the afs_toc_preview_layout_definitions structure.
 *  \param[in]   index                           Index for specifying the required instance of the afs_toc_preview_layout_definition structure in the storage vector.
 *  \return an instance of afs_toc_preview_layout_definition structure or NULL.
 */

afs_toc_preview_layout_definition * afs_toc_preview_layout_definitions_get_layout_definition_by_index(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const unsigned int index)
{
    if (toc_preview_layout_definitions == NULL)
    {
        return NULL;
    }

    if (toc_preview_layout_definitions->layout_definitions == NULL)
    {
        return NULL;
    }

    if (index >= toc_preview_layout_definitions->layout_definitions->size)
    {
        return NULL;
    }

    return GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_preview_layout_definition structure according to the input id.
 *
 *  The function returns an instance of afs_toc_preview_layout_definition structure according to the input id.
 *
 *  \param[in]   toc_preview_layout_definitions  Pointer to the instance of the afs_toc_preview_layout_definitions structure.
 *  \param[in]   id                              Id for specifying the required instance of the afs_toc_preview_layout_definition structure in the storage vector.
 *  \return an instance of afs_toc_preview_layout_definition structure or NULL.
 */

afs_toc_preview_layout_definition * afs_toc_preview_layout_definitions_get_layout_definition_by_id(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * id)
{
    if (toc_preview_layout_definitions == NULL || id == NULL || boxing_string_length(id) == 0)
    {
        return NULL;
    }

    if (toc_preview_layout_definitions->layout_definitions == NULL)
    {
        return NULL;
    }

    if (toc_preview_layout_definitions->layout_definitions->size == 0)
    {
        return NULL;
    }

    for (unsigned int i = 0; i < toc_preview_layout_definitions->layout_definitions->size; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i);

        if (boxing_string_equal(toc_preview_layout_definition->id, id) == DTRUE)
        {
            return GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i);
        }
    }

    return NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_preview_layout_definition structure according to the input name.
 *
 *  The function returns an instance of afs_toc_preview_layout_definition structure according to the input name.
 *
 *  \param[in]   toc_preview_layout_definitions  Pointer to the instance of the afs_toc_preview_layout_definitions structure.
 *  \param[in]   name                            Named for specifying the required instance of the afs_toc_preview_layout_definition structure in the storage vector.
 *  \return an instance of afs_toc_preview_layout_definition structure or NULL.
 */

afs_toc_preview_layout_definition * afs_toc_preview_layout_definitions_get_layout_definition_by_name(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * name)
{
    if (toc_preview_layout_definitions == NULL || name == NULL || boxing_string_length(name) == 0)
    {
        return NULL;
    }

    if (toc_preview_layout_definitions->layout_definitions == NULL)
    {
        return NULL;
    }

    if (toc_preview_layout_definitions->layout_definitions->size == 0)
    {
        return NULL;
    }

    for (unsigned int i = 0; i < toc_preview_layout_definitions->layout_definitions->size; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i);

        if (boxing_string_equal(toc_preview_layout_definition->name, name) == DTRUE)
        {
            return GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i);
        }
    }

    return NULL;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_preview_layout_definition structure to the input afs_toc_preview_layout_definitions structure.
 *
 *  The function adds a new instance of afs_toc_preview_layout_definition structure to the input afs_toc_preview_layout_definitions structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_preview_layout_definition structure is not add and the function returns DFALSE.
 *  If input afs_toc_preview_layout_definition instance is not valid or new layout definition id already present then new instance is not add, the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_preview_layout_definitions  Pointer to the instance of the afs_toc_preview_layout_definitions structure.
 *  \param[in]   toc_preview_layout_definition   Pointer to the instance of the afs_toc_preview_layout_definition structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definitions_add_layout_definition(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, afs_toc_preview_layout_definition * toc_preview_layout_definition)
{
    if (toc_preview_layout_definitions == NULL || toc_preview_layout_definition == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition) != DTRUE || afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition->id) == DTRUE)
    {
        return DFALSE;
    }

    if (toc_preview_layout_definitions->layout_definitions == NULL)
    {
        toc_preview_layout_definitions->layout_definitions = gvector_create_pointers(0);
    }

    size_t old_size = toc_preview_layout_definitions->layout_definitions->size;

    gvector_append_data(toc_preview_layout_definitions->layout_definitions, 1, &toc_preview_layout_definition);

    if (toc_preview_layout_definitions->layout_definitions->size != old_size + 1)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function checks the presence of the afs_toc_preview_layout_definition instance with the specified id.
 *
 *  The function checks the presence of the afs_toc_preview_layout_definition instance with the specified id.
 *
 *  \param[in]   toc_preview_layout_definitions  Pointer to the instance of the afs_toc_preview_layout_definitions structure.
 *  \param[in]   id                              TOC preview layout definition id.
 *  \return DTRUE if afs_toc_preview_layout_definition instance with specified id is present.
 */

DBOOL afs_toc_preview_layout_definitions_has_layout_definition(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * id)
{
    if (toc_preview_layout_definitions == NULL || id == NULL || boxing_string_length(id) == 0)
    {
        return DFALSE;
    }

    if (toc_preview_layout_definitions->layout_definitions == NULL)
    {
        return DFALSE;
    }

    if (toc_preview_layout_definitions->layout_definitions->size == 0)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_preview_layout_definitions->layout_definitions->size; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i);

        if (boxing_string_equal(toc_preview_layout_definition->id, id) == DTRUE)
        {
            return DTRUE;
        }
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_preview_layout_definitions structure to the XML file.
 *
 *  Function translates the input afs_toc_preview_layout_definitions structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_preview_layout_definitions Pointer to the afs_toc_preview_layout_definitions structure.
 *  \param[in]   file_name                      Name of the XML file.
 *  \param[in]   compact                       If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definitions_save_file(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_preview_layout_definitions == NULL || afs_toc_preview_layout_definitions_is_valid(toc_preview_layout_definitions) == DFALSE)
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

    if (!afs_toc_preview_layout_definitions_save_xml(toc_preview_layout_definitions, tree))
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
 *  \brief Function translates the input afs_toc_preview_layout_definitions structure to the XML string.
 *
 *  Function translates the input afs_toc_preview_layout_definitions structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_preview_layout_definitions Pointer to the afs_toc_preview_layout_definitions structure.
 *  \param[in]   compact                        If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_preview_layout_definitions_save_string(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, DBOOL compact)
{
    // If TOC preview layout definitions pointer is NULL return DFALSE
    if (toc_preview_layout_definitions == NULL)
    {
        return DFALSE;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_preview_layout_definitions_save_xml(toc_preview_layout_definitions, document))
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
 *  \brief Function translates the input afs_toc_preview_layout_definitions structure to the string table.
 *
 *  Function translates the input afs_toc_preview_layout_definitions structure to the string table.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_preview_layout_definitions Pointer to the afs_toc_preview_layout_definitions structure.
 *  \return resulting string or NULL.
 */

char * afs_toc_preview_layout_definitions_save_as_table(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions)
{
    // If TOC preview layout definitions pointer is NULL return DFALSE
    if (toc_preview_layout_definitions == NULL)
    {
        return NULL;
    }

    const char * string_id_pad = "00000000000";
    const char * header =
        "VISUAL LAYOUT DEFINITIONS\n"
        "=========================\n";
    const char * definition_columns = "<layoutId> <sections> <name>\n";
    const char * layout_columns = "\n<layoutId> <sectionId> <x> <y> <width> <height> <rotation>\n";
    
    layout_definitions_lengths lengths = get_max_layout_id_lenght(toc_preview_layout_definitions);
    unsigned int definitions_count = afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions);

    unsigned int header_length = (unsigned int)boxing_string_length(header);
    unsigned int table1_width =  lengths.layout_id_length + lengths.sections_length + lengths.name_length + 3;
    unsigned int table1_length = strlen(definition_columns) + table1_width * definitions_count + 1;
    unsigned int table2_width =  lengths.layout_id_length + lengths.section_id_length + lengths.x_length + lengths.y_length +
                                 lengths.width_length + lengths.height_length + lengths.rotation_length + 7;
    unsigned int table2_length = strlen(layout_columns) + table2_width * lengths.sections_count + 1;

    char * return_string = boxing_string_allocate(header_length + table1_length + table2_length + strlen("\n"));
    char * current_string = return_string;
    current_string += sprintf(current_string, "%s%s", header, definition_columns);

    for (unsigned int i = 0; i < definitions_count; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, i);
        unsigned int sections_count = afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition);

        current_string += sprintf(current_string, "%.*s%s %0*u %s\n", (int)(lengths.layout_id_length - boxing_string_length(toc_preview_layout_definition->id)), string_id_pad, toc_preview_layout_definition->id, lengths.sections_length, sections_count, toc_preview_layout_definition->name);
    }

    current_string += sprintf(current_string, layout_columns);

    for (unsigned int i = 0; i < definitions_count; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, i);
        unsigned int sections_count = afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition);

        for (unsigned int j = 0; j < sections_count; j++)
        {
            afs_toc_preview_section * current_toc_preview_section = afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, j);

            current_string += sprintf(
                current_string, "%.*s%s %0*u %0*u %0*u %0*u %0*u %0*u\n", (int)(lengths.layout_id_length - boxing_string_length(toc_preview_layout_definition->id)), string_id_pad, toc_preview_layout_definition->id, lengths.section_id_length, current_toc_preview_section->id, lengths.x_length, current_toc_preview_section->x, lengths.y_length, current_toc_preview_section->y, lengths.width_length, current_toc_preview_section->width, lengths.height_length, current_toc_preview_section->height, lengths.rotation_length, current_toc_preview_section->rotation);
        }
    }

    if ( definitions_count )
    {
        strcat( return_string, "\n" );
    }
    
    return return_string;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_preview_layout_definitions structure to the XML nodes.
 *
 *  Function translates the input afs_toc_preview_layout_definitions structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_preview_layout_definitions  Pointer to the afs_toc_preview_layout_definitions structure.
 *  \param[out]  out                             Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definitions_save_xml(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions, mxml_node_t* out)
{
    // If output node pointer is NULL or TOC preview layout definitions pointer is NULL return DFALSE
    if (out == NULL || toc_preview_layout_definitions == NULL || afs_toc_preview_layout_definitions_is_valid(toc_preview_layout_definitions) == DFALSE)
    {
        return DFALSE;
    }

    mxml_node_t * definitions_node = mxmlNewElement(out, "layoutDefinitions");

    for (unsigned int i = 0; i < afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions); i++)
    {
        DBOOL result = afs_toc_preview_layout_definition_save_xml(GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i), definitions_node);

        if (result == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the afs_toc_preview_layout_definitions structure.
 *
 *  Function translates the input XML nodes to the afs_toc_preview_layout_definitions structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_layout_definitions  Pointer to the afs_toc_preview_layout_definitions structure.
 *  \param[in]   file_name                       Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definitions_load_file(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * file_name)
{
    // If input file name string pointer is NULL or TOC preview layout definitions pointer is NULL return DFALSE
    if (file_name == NULL || toc_preview_layout_definitions == NULL)
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

    DBOOL return_value = afs_toc_preview_layout_definitions_load_xml(toc_preview_layout_definitions, document);

    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_preview_layout_definitions structure.
 *
 *  Function translates the input XML string to the afs_toc_preview_layout_definitions structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_layout_definitions Pointer to the afs_toc_preview_layout_definitions structure.
 *  \param[in]   in                             Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definitions_load_string(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, const char * in)
{
    // If input string pointer is NULL or TOC preview layout definitions pointer is NULL return DFALSE
    if (in == NULL || boxing_string_equal(in, "") || toc_preview_layout_definitions == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_preview_layout_definitions_load_xml(toc_preview_layout_definitions, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC preview layout definitions structure.
 *
 *  Function translates the input XML nodes to the TOC preview layout definitions structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_preview_layout_definitions Pointer to the afs_toc_preview_layout_definitions structure.
 *  \param[in]   input_node                     Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_preview_layout_definitions_load_xml(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, mxml_node_t* input_node)
{
    // If input node pointer is NULL or TOC preview layout definitions pointer is NULL return DFALSE
    if (input_node == NULL || toc_preview_layout_definitions == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * definitions_node = boxing_string_equal(mxmlGetElement(input_node), "layoutDefinitions") == DTRUE ? input_node : mxmlFindElement(input_node, input_node, "layoutDefinitions", NULL, NULL, MXML_DESCEND);

    if (definitions_node == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * definition_node = mxmlFindElement(definitions_node, definitions_node, "previewLayout", NULL, NULL, MXML_DESCEND);

    while (definition_node != NULL)
    {
        if (mxmlGetType(definition_node) == MXML_ELEMENT)
        {
            afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

            DBOOL load_definition_result = afs_toc_preview_layout_definition_load_xml(toc_preview_layout_definition, definition_node);

            if (load_definition_result == DFALSE)
            {
                DLOG_INFO("Has no TOC preview layout definition.\n");
                return DFALSE;
            }

            DBOOL add_definition_result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

            if (add_definition_result == DFALSE)
            {
                afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
                return DFALSE;
            }
        }

        definition_node = mxmlFindElement(definition_node, definitions_node, "previewLayout", NULL, NULL, MXML_DESCEND);
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of preview group
  */


// PRIVATE AFS TOC PREVIEW LAYOUT DEFINITIONS FUNCTIONS
//

static layout_definitions_lengths get_max_layout_id_lenght(const afs_toc_preview_layout_definitions * toc_preview_layout_definitions)
{
    layout_definitions_lengths lengths;

    lengths.layout_id_length = 0; //(unsigned int)boxing_string_length("<layoutId>");
    lengths.sections_length = 0; //(unsigned int)boxing_string_length("<sections>");
    lengths.name_length = 0; //(unsigned int)boxing_string_length("<name>");
    lengths.section_id_length = 0; //(unsigned int)boxing_string_length("<sectionId>");
    lengths.x_length = 0; //(unsigned int)boxing_string_length("<x>");
    lengths.y_length = 0; //(unsigned int)boxing_string_length("<y>");
    lengths.width_length = 0; //(unsigned int)boxing_string_length("<width>");
    lengths.height_length = 0; //(unsigned int)boxing_string_length("<height>");
    lengths.rotation_length = 3; //(unsigned int)boxing_string_length("<rotation>");
    lengths.sections_count = 0;


    if (toc_preview_layout_definitions == NULL)
    {
        return lengths;
    }

    if (toc_preview_layout_definitions->layout_definitions == NULL)
    {
        return lengths;
    }

    for (unsigned int i = 0; i < toc_preview_layout_definitions->layout_definitions->size; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i);
        unsigned int sections_count = afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition);

        lengths.layout_id_length = BOXING_MATH_MAX((unsigned int)boxing_string_length(toc_preview_layout_definition->id), lengths.layout_id_length);
        lengths.sections_length = BOXING_MATH_MAX(get_digits_count(sections_count), lengths.sections_length);
        lengths.name_length = BOXING_MATH_MAX((unsigned int)boxing_string_length(toc_preview_layout_definition->name), lengths.name_length);

        for (unsigned int j = 0; j < sections_count; j++)
        {
            afs_toc_preview_section * current_toc_preview_section = afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, j);
            lengths.sections_count++;

            lengths.section_id_length = BOXING_MATH_MAX(get_digits_count(current_toc_preview_section->id), lengths.section_id_length);
            lengths.x_length = BOXING_MATH_MAX(get_digits_count(current_toc_preview_section->x), lengths.x_length);
            lengths.y_length = BOXING_MATH_MAX(get_digits_count(current_toc_preview_section->y), lengths.y_length);
            lengths.width_length = BOXING_MATH_MAX(get_digits_count(current_toc_preview_section->width), lengths.width_length);
            lengths.height_length = BOXING_MATH_MAX(get_digits_count(current_toc_preview_section->height), lengths.height_length);
            lengths.rotation_length = BOXING_MATH_MAX(get_digits_count(current_toc_preview_section->rotation), lengths.rotation_length);
        }
    }

    return lengths;
}


static unsigned int get_digits_count(unsigned int number)
{
    return number / 10 != 0 ? 1 + get_digits_count(number / 10) : 1;
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

    if (boxing_string_equal("layoutDefinitions", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("previewLayout", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("id", name) || boxing_string_equal("name", name) || boxing_string_equal("sections", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n        ");
        }
    }

    if (boxing_string_equal("sections", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n            ");
        }
    }

    if (boxing_string_equal("id", name) || boxing_string_equal("name", name))
    {
        if (where == MXML_WS_AFTER_OPEN)
        {
            return ("\n            ");
        }
    }

    return (NULL);
}
