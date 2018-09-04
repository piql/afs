/*****************************************************************************
**
**  Definition of the TOC data reels interface
**
**  Creation date:  2018/05/10
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2018 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

//  PROJECT INCLUDES
//

#include "tocdatareels.h"
#include "boxing/platform/memory.h"
#include "boxing/log.h"
#include "boxing/utils.h"

// DEFINES
//

// PRIVATE INTERFACE
//

static const char *               whitespace_cb(mxml_node_t *node, int where);

/*! 
  * \addtogroup file
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_toc_data_reels_s  tocdatareels.h
 *  \brief      TOC data reels data storage.
 *
 *  \param reels  Vector with afs_toc_data_reel instances.
 *
 *  Structure for storing TOC data reels data.
 */

// PUBLIC AFS TOC DATA REELS FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC data reels instance.
 *
 *  Allocate memory for the afs_toc_data_reels type
 *  and initializes vector for storing TOC data reels data.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_data_reels structure.
 */

afs_toc_data_reels * afs_toc_data_reels_create()
{
    afs_toc_data_reels * toc_data_reels = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_data_reels);
    toc_data_reels->reels = NULL;
    return toc_data_reels;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a TOC data reels instance.
 *
 *  Allocate memory for the afs_toc_data_reels type
 *  and initializes structure vector pointer with specified values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_toc_data_reels structure.
 */

afs_toc_data_reels * afs_toc_data_reels_create2(afs_toc_data_reels_vector * reels)
{
    afs_toc_data_reels * toc_data_reels = BOXING_MEMORY_ALLOCATE_TYPE(afs_toc_data_reels);
    toc_data_reels->reels = reels;
    return toc_data_reels;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_toc_data_reels structure.
 *
 *  Frees occupied memory of all internal structure pointers and structure pointer.
 *
 *  \param[in]  toc_data_reels  Pointer to the afs_toc_data_reels structure.
 */

void afs_toc_data_reels_free(afs_toc_data_reels * toc_data_reels)
{
    if (toc_data_reels == NULL)
    {
        return;
    }

    if (toc_data_reels->reels != NULL)
    {
        for (unsigned int i = 0; i < toc_data_reels->reels->size; i++)
        {
            afs_toc_data_reel_free(GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, i));
            GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, i) = NULL;
        }
    }

    gvector_free(toc_data_reels->reels);
    boxing_memory_free(toc_data_reels);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_toc_data_reels structure.
 *
 *  Function create a copy of input afs_toc_data_reels structure and return it.
 *  If TOC data reels pointer is NULL function return NULL.
 *
 *  \param[in]  toc_data_reels  Pointer to the afs_toc_data_reels structure.
 *  \return new copy of afs_toc_data_reels structure or NULL.
 */

afs_toc_data_reels * afs_toc_data_reels_clone(afs_toc_data_reels * toc_data_reels)
{
    // If TOC data reels pointer is NULL return NULL.
    if (toc_data_reels == NULL)
    {
        return NULL;
    }

    if (toc_data_reels->reels == NULL)
    {
        return afs_toc_data_reels_create();
    }

    afs_toc_data_reels * return_toc_data_reels = afs_toc_data_reels_create2(gvector_create_pointers(toc_data_reels->reels->size));
    
    for (unsigned int i = 0; i < toc_data_reels->reels->size; i++)
    {
        afs_toc_data_reel * toc_data_reel = GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, i);
        GVECTORN(return_toc_data_reels->reels, afs_toc_data_reel *, i) = afs_toc_data_reel_clone(toc_data_reel);
    }

    return return_toc_data_reels;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_toc_data_reels structures on the identity.
 *
 *  Function checks two instances of the afs_toc_data_reels structures on the identity.
 *
 *  \param[in]   toc_data_reels1  Pointer to the first instance of the afs_toc_data_reels structure.
 *  \param[in]   toc_data_reels2  Pointer to the second instance of the afs_toc_data_reels structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_toc_data_reels_equal(afs_toc_data_reels * toc_data_reels1, afs_toc_data_reels * toc_data_reels2)
{
    if (toc_data_reels1 == NULL && toc_data_reels2 == NULL)
    {
        return DTRUE;
    }

    if (toc_data_reels1 == NULL || toc_data_reels2 == NULL)
    {
        return DFALSE;
    }

    if (toc_data_reels1->reels == NULL && toc_data_reels2->reels == NULL)
    {
        return DTRUE;
    }

    if (toc_data_reels1->reels == NULL || toc_data_reels2->reels == NULL)
    {
        return DFALSE;
    }

    if (toc_data_reels1->reels->size != toc_data_reels2->reels->size)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < toc_data_reels1->reels->size; i++)
    {
        if (afs_toc_data_reel_equal(GVECTORN(toc_data_reels1->reels, afs_toc_data_reel *, i), GVECTORN(toc_data_reels2->reels, afs_toc_data_reel *, i)) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_toc_data_reel structure to the input afs_toc_data_reels structure.
 *
 *  The function adds a new instance of afs_toc_data_reel structure to the input afs_toc_data_reels structure.
 *  If one of the input pointer is NULL the new instance of afs_toc_data_reel structure is not add and the function returns DFALSE.
 *  If input afs_toc_data_reel instance is not valid then new instance is not add the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   toc_data_reels  Pointer to the instance of the afs_toc_data_reels structure.
 *  \param[in]   toc_data_reel   Pointer to the instance of the afs_toc_data_reel structure.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reels_add_reel(afs_toc_data_reels * toc_data_reels, afs_toc_data_reel * toc_data_reel)
{
    if (toc_data_reels == NULL || toc_data_reel == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_data_reel_is_valid(toc_data_reel) != DTRUE)
    {
        return DFALSE;
    }

    if (toc_data_reels->reels == NULL)
    {
        toc_data_reels->reels = gvector_create_pointers(0);
    }

    size_t old_size = toc_data_reels->reels->size;

    gvector_append_data(toc_data_reels->reels, 1, &toc_data_reel);

    if (toc_data_reels->reels->size != old_size + 1)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_toc_data_reel structure according to the input index.
 *
 *  The function returns an instance of afs_toc_data_reel structure according to the input index.
 *
 *  \param[in]   toc_data_reels  Pointer to the instance of the afs_toc_data_reels structure.
 *  \param[in]   index           Index for specifying the required instance of the afs_toc_data_reel structure in the storage vector.
 *  \return an instance of afs_toc_data_reel structure or NULL.
 */

afs_toc_data_reel * afs_toc_data_reels_get_reel(const afs_toc_data_reels * toc_data_reels, const unsigned int index)
{
    if (toc_data_reels == NULL)
    {
        return NULL;
    }

    if (toc_data_reels->reels == NULL)
    {
        return NULL;
    }

    if (index >= toc_data_reels->reels->size)
    {
        return NULL;
    }

    return GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_toc_data_reel structure.
 *
 *  The function returns a count of instances of afs_toc_data_reel structure.
 *  If input afs_toc_data_reels pointer is NULL or internal reels vector pointer is NULL then function returns zero.
 *
 *  \param[in]  toc_data_reels  Pointer to the instance of the afs_toc_data_reels structure.
 *  \return a count of instances of afs_toc_data_reel structure or zero.
 */

unsigned int afs_toc_data_reels_get_reels_count(const afs_toc_data_reels * toc_data_reels)
{
    if (toc_data_reels == NULL)
    {
        return 0;
    }

    if (toc_data_reels->reels == NULL)
    {
        return 0;
    }

    return (unsigned int)toc_data_reels->reels->size;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns the sign of integrity the all afs_toc_data_reel structures in the input afs_toc_data_reels structure.
 *
 *  The function returns the sign of integrity the all afs_toc_data_reel structures in the input afs_toc_data_reels structure.
 *  If TOC data reels pointer is NULL or internal reels vector pointer is NULL or vector has no pointers then function return DFALSE.
 *
 *  \param[in]   toc_data_reels   Pointer to the afs_toc_data_reels structure.
 *  \return the sign of integrity the all afs_toc_data_reels structures.
 */

DBOOL afs_toc_data_reels_is_valid(afs_toc_data_reels * toc_data_reels)
{
    if (toc_data_reels == NULL)
    {
        return DFALSE;
    }

    if (afs_toc_data_reels_get_reels_count(toc_data_reels) == 0)
    {
        return DTRUE;
    }

    for (unsigned int i = 0; i < toc_data_reels->reels->size; i++)
    {
        if (afs_toc_data_reel_is_valid(GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, i)) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input afs_toc_data_reels structure to the XML file.
 *
 *  Function translates the input afs_toc_data_reels structure to the XML file.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_data_reels  Pointer to the afs_toc_data_reels structure.
 *  \param[in]   file_name       Name of the XML file.
 *  \param[in]   compact         If compact is DFALSE then in the resulting XML file needs to add formatting (new lines and tabs).
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reels_save_file(afs_toc_data_reels * toc_data_reels, const char * file_name, DBOOL compact)
{
    if (file_name == NULL || toc_data_reels == NULL)
    {
        return DFALSE;
    }

    mxml_node_t *tree = mxmlNewXML("1.0");

    if (!afs_toc_data_reels_save_xml(toc_data_reels, tree))
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
 *  \brief Function translates the input afs_toc_data_reels structure to the XML string.
 *
 *  Function translates the input afs_toc_data_reels structure to the XML string.
 *  If translates is successful, then function return resulting string, else function return NULL.
 *
 *  \param[in]   toc_data_reels  Pointer to the afs_toc_data_reels structure.
 *  \param[in]   compact         If compact is DFALSE then in the resulting XML string needs to add formatting (new lines and tabs).
 *  \return resulting string or NULL.
 */

char * afs_toc_data_reels_save_string(afs_toc_data_reels * toc_data_reels, DBOOL compact)
{
    // If TOC data reels pointer is NULL return DFALSE
    if (toc_data_reels == NULL)
    {
        return NULL;
    }

    struct mxml_node_s* document = mxmlNewXML("1.0");

    if (!afs_toc_data_reels_save_xml(toc_data_reels, document))
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
 *  \brief Function translates the input afs_toc_data_reels structure to the XML nodes.
 *
 *  Function translates the input afs_toc_data_reels structure to the XML nodes.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[in]   toc_data_reels  Pointer to the afs_toc_data_reels structure.
 *  \param[out]  out             Pointer to the XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reels_save_xml(afs_toc_data_reels * toc_data_reels, mxml_node_t * out)
{
    // If output node pointer is NULL or TOC data reels pointer is NULL return DFALSE
    if (out == NULL || toc_data_reels == NULL)
    {
        return DFALSE;
    }

    if (!afs_toc_data_reels_is_valid(toc_data_reels))
    {
        return DFALSE;
    }

    mxml_node_t * reels_node = mxmlNewElement(out, "reels");

    for (unsigned int i = 0; i < afs_toc_data_reels_get_reels_count(toc_data_reels); i++)
    {
        DBOOL result = afs_toc_data_reel_save_xml(GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, i), reels_node);

        if (result == DFALSE)
        {
            return DFALSE;
        }
    }
    
    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML file to the afs_toc_data_reels structure.
 *
 *  Function translates the input XML file to the afs_toc_data_reels structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_reels  Pointer to the afs_toc_data_reels structure.
 *  \param[in]   file_name       Name of the XML file.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reels_load_file(afs_toc_data_reels * toc_data_reels, const char * file_name)
{
    // If input file name string pointer is NULL or TOC data reels pointer is NULL return DFALSE
    if (file_name == NULL || toc_data_reels == NULL)
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
    
    DBOOL return_value = afs_toc_data_reels_load_xml(toc_data_reels, document);
    
    fclose(fp_load);
    mxmlDelete(document);

    return return_value;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML string to the afs_toc_data_reels structure.
 *
 *  Function translates the input XML string to the afs_toc_data_reels structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_reels  Pointer to the afs_toc_data_reels structure.
 *  \param[in]   in              Pointer to the input XML string.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reels_load_string(afs_toc_data_reels * toc_data_reels, const char * in)
{
    // If input string pointer is NULL or TOC data reels pointer is NULL return DFALSE
    if (in == NULL || boxing_string_equal(in, "") || toc_data_reels == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * document = mxmlLoadString(NULL, in, MXML_OPAQUE_CALLBACK);

    if (!afs_toc_data_reels_load_xml(toc_data_reels, document))
    {
        return DFALSE;
    }

    mxmlDelete(document);

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function translates the input XML nodes to the TOC data reels structure.
 *
 *  Function translates the input XML nodes to the TOC data reels structure.
 *  If translates is successful, then function return DTRUE, else function return DFALSE.
 *
 *  \param[out]  toc_data_reels  Pointer to the afs_toc_data_reels structure.
 *  \param[in]   node            Pointer to the input XML node.
 *  \return DTRUE on success.
 */

DBOOL afs_toc_data_reels_load_xml(afs_toc_data_reels * toc_data_reels, mxml_node_t * node)
{
    // If input node pointer is NULL or TOC data reels pointer is NULL return DFALSE
    if (node == NULL || toc_data_reels == NULL)
    {
        return DFALSE;
    }

    mxml_node_t * reels_node = boxing_string_equal(mxmlGetElement(node), "reels") == DTRUE ? node : mxmlFindElement(node, node, "reels", NULL, NULL, MXML_DESCEND);

    mxml_node_t * reel_node = mxmlFindElement(reels_node, reels_node, "reel", NULL, NULL, MXML_DESCEND);

    while (reel_node != NULL)
    {
        if (mxmlGetType(reel_node) == MXML_ELEMENT)
        {
            afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

            DBOOL load_reel_result = afs_toc_data_reel_load_xml(toc_data_reel, reel_node);

            if (load_reel_result == DFALSE)
            {
                DLOG_INFO("Has no REEL.\n");
                return DFALSE;
            }

            DBOOL add_reel_result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

            if (add_reel_result == DFALSE)
            {
                afs_toc_data_reel_free(toc_data_reel);
                return DFALSE;
            }
        }

        reel_node = mxmlFindElement(reel_node, node, "reel", NULL, NULL, MXML_DESCEND);
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
  * \} end of file group
  */


// PRIVATE AFS TOC DATA REELS FUNCTIONS
//


static const char * whitespace_cb(mxml_node_t *node, int where)
{
    const char *name, *parent_name, *parent_parent_name;

    /*
    * We can conditionally break to a new line
    * before or after any element. These are
    * just common HTML elements...
    */

    name = mxmlGetElement(node);
    parent_name = mxmlGetElement(node->parent);
    parent_parent_name = (node->parent != NULL) ? mxmlGetElement(node->parent->parent) : NULL;

    if (boxing_string_equal("reels", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n");
        }
    }

    if (boxing_string_equal("reel", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n    ");
        }
    }

    if (boxing_string_equal("id", name) && boxing_string_equal("reel", parent_name))
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

    if ((boxing_string_equal("data", name) && boxing_string_equal("file", parent_name)) || boxing_string_equal("preview", name) || boxing_string_equal("metadata", name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                ");
        }
    }

    if (boxing_string_equal("metadata", parent_name) && boxing_string_equal("file", parent_parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                    ");
        }
    }

    if (boxing_string_equal("source", parent_name) && boxing_string_equal("metadata", parent_parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                        ");
        }
    }

    if (boxing_string_equal("data", parent_name) && boxing_string_equal("source", parent_parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE)
        {
            return ("\n                            ");
        }
    }

    if (boxing_string_equal("data", parent_name) && boxing_string_equal("file", parent_parent_name))
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

    if (boxing_string_equal("preview", parent_name))
    {
        if (where == MXML_WS_BEFORE_OPEN)
        {
            return ("\n                    ");
        }
    }

    return (NULL);
}
