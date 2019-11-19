/*****************************************************************************
**
**  Implementation of structure afs_frame_ranges methods C interface
**
**  Creation date:    2018/05/03
**  Created by:       Oleksandr Ivanov
**
**
**  Copyright (c) 2018 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "frameranges.h"
#include "boxing/platform/memory.h"
#include "boxing/log.h"


// PRIVATE INTERFACE
//


/*!
 * \addtogroup file
 * \{
 */


/*!
 *  \typedef  gvector afs_frame_ranges_vector
 *  \brief Array of frame_range structure instances.
 *
 *  Array of afs_frame_range structure instances with the ability to add new instances.
 */


// PUBLIC AFS FRAME RANGES FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \struct     afs_frame_ranges_s  frameranges.h
 *  \brief      Frame ranges data storage.
 *
 *  \param frame_ranges  Vector with afs_frame_range pointers.
 *
 *  Structure for storing frames ranges.
 */


//----------------------------------------------------------------------------
/*!
 *  \brief Create a frame ranges instance.
 *
 *  Allocate memory for the afs_frame_ranges type
 *  and initializing the vector pointer with NULL value.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_frame_ranges structure or NULL.
 */

afs_frame_ranges * afs_frame_ranges_create()
{
    afs_frame_ranges * frame_ranges = boxing_memory_allocate(sizeof(afs_frame_ranges));
    frame_ranges->frame_ranges = NULL;
    return frame_ranges;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_frame_ranges structure.
 *
 *  Frees occupied memory of the afs_frame_ranges structure and all afs_frame_range structures.
 *
 *  \param[in]  frame_ranges  Pointer to the afs_frame_ranges structure.
 */

void afs_frame_ranges_free(afs_frame_ranges * frame_ranges)
{
    if (frame_ranges == NULL)
    {
        return;
    }

    if (frame_ranges->frame_ranges != NULL)
    {
        for (unsigned int i = 0; i < frame_ranges->frame_ranges->size; i++)
        {
            afs_frame_range_free(GVECTORN(frame_ranges->frame_ranges, afs_frame_range *, i));
            GVECTORN(frame_ranges->frame_ranges, afs_frame_range *, i) = NULL;
        }

        gvector_free(frame_ranges->frame_ranges);
        frame_ranges->frame_ranges = NULL;
    }

    boxing_memory_free(frame_ranges);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_frame_ranges structure.
 *
 *  Function create a copy of input afs_frame_ranges structure and return it.
 *  If frame ranges pointer is NULL function return NULL.
 *
 *  \param[in]  frame_ranges  Pointer to the afs_frame_ranges structure.
 *  \return new copy of afs_frame_ranges structure or NULL.
 */

afs_frame_ranges * afs_frame_ranges_clone(const afs_frame_ranges * frame_ranges)
{
    if (frame_ranges == NULL)
    {
        return NULL;
    }

    if (frame_ranges->frame_ranges == NULL)
    {
        return afs_frame_ranges_create();
    }

    afs_frame_ranges * return_frame_ranges = afs_frame_ranges_create();
    return_frame_ranges->frame_ranges = gvector_create_pointers(frame_ranges->frame_ranges->size);

    for (unsigned int i = 0; i < frame_ranges->frame_ranges->size; i++)
    {
        afs_frame_range * frame_range = GVECTORN(frame_ranges->frame_ranges, afs_frame_range *, i);
        GVECTORN(return_frame_ranges->frame_ranges, afs_frame_range *, i) = afs_frame_range_clone(frame_range);
    }

    return return_frame_ranges;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_frame_ranges structures on the identity.
 *
 *  Function checks two instances of the afs_frame_ranges structures on the identity.
 *
 *  \param[in]   frame_ranges1  Pointer to the first instance of the afs_frame_ranges structure.
 *  \param[in]   frame_ranges2  Pointer to the second instance of the afs_frame_ranges structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_frame_ranges_equal(const afs_frame_ranges * frame_ranges1, const afs_frame_ranges * frame_ranges2)
{
    if (frame_ranges1 == NULL && frame_ranges2 == NULL)
    {
        return DTRUE;
    }

    if (frame_ranges1 == NULL || frame_ranges2 == NULL)
    {
        return DFALSE;
    }

    if (frame_ranges1->frame_ranges == NULL && frame_ranges2->frame_ranges == NULL)
    {
        return DTRUE;
    }

    if (frame_ranges1->frame_ranges == NULL || frame_ranges2->frame_ranges == NULL)
    {
        return DFALSE;
    }

    if (frame_ranges1->frame_ranges->size != frame_ranges2->frame_ranges->size)
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < frame_ranges1->frame_ranges->size; i++)
    {
        afs_frame_range * frame_range1 = GVECTORN(frame_ranges1->frame_ranges, afs_frame_range *, i);
        afs_frame_range * frame_range2 = GVECTORN(frame_ranges2->frame_ranges, afs_frame_range *, i);

        if (afs_frame_range_equal(frame_range1, frame_range2) == DFALSE)
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns a count of instances of afs_frame_range structure.
 *
 *  The function returns a count of instances of afs_frame_range structure.
 *  If input afs_frame_ranges pointer is NULL or internal ranges vector pointer is NULL then function returns zero.
 *
 *  \param[in]   frame_ranges  Pointer to the instance of the afs_frame_ranges structure.
 *  \return a count of instances of afs_frame_range structure or zero.
 */

unsigned int afs_frame_ranges_get_count(const afs_frame_ranges * frame_ranges)
{
    if (frame_ranges == NULL)
    {
        return 0;
    }

    if (frame_ranges->frame_ranges == NULL)
    {
        return 0;
    }

    return (unsigned int)frame_ranges->frame_ranges->size;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns an instance of afs_frame_range structure according to the input index.
 *
 *  The function returns an instance of afs_frame_range structure according to the input index.
 *
 *  \param[in]   frame_ranges  Pointer to the instance of the afs_frame_ranges structure.
 *  \param[in]   index         Index for specifying the required instance of the afs_frame_range structure in the storage vector.
 *  \return an instance of afs_frame_range structure or NULL.
 */

afs_frame_range * afs_frame_ranges_get_frame_range(const afs_frame_ranges * frame_ranges, const unsigned int index)
{
    if (frame_ranges == NULL)
    {
        return NULL;
    }

    if (frame_ranges->frame_ranges == NULL)
    {
        return NULL;
    }

    if (index >= frame_ranges->frame_ranges->size)
    {
        return NULL;
    }

    return GVECTORN(frame_ranges->frame_ranges, afs_frame_range *, index);
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function adds a new instance of afs_frame_range structure to the input afs_frame_ranges structure.
 *
 *  The function adds a new instance of afs_frame_range structure to the input afs_frame_ranges structure.
 *  If one of the input pointer is NULL the new instance of afs_frame_range structure is not add and the function returns DFALSE.
 *  If addition is successful, then function return DTRUE.
 *
 *  \param[in]   frame_ranges  Pointer to the instance of the afs_frame_ranges structure.
 *  \param[in]   frame_range   Pointer to the instance of the afs_frame_range structure.
 *  \return DTRUE on success.
 */

DBOOL afs_frame_ranges_add_frame_range(afs_frame_ranges * frame_ranges, afs_frame_range * frame_range)
{
    if (frame_ranges == NULL || frame_range == NULL)
    {
        return DFALSE;
    }

    if (frame_ranges->frame_ranges == NULL)
    {
        frame_ranges->frame_ranges = gvector_create_pointers(0);
    }

    size_t old_size = frame_ranges->frame_ranges->size;

    gvector_append_data(frame_ranges->frame_ranges, 1, &frame_range);

    if (frame_ranges->frame_ranges->size != old_size + 1)
    {
        return DFALSE;
    }

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 * \} end of file group
 */


// PRIVATE AFS FRAME RANGES FUNCTIONS
//
