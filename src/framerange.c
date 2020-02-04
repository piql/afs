/*****************************************************************************
**
**  Definition of the frame range interface
**
**  Creation date:  2018/05/02
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

#include "framerange.h"
#include "boxing/platform/memory.h"
#include "boxing/log.h"

//  DEFINES
//


// PRIVATE INTERFACE
//

static void afs_frame_range_init(afs_frame_range * frame_range);
static void afs_frame_range_init2(afs_frame_range * frame_range, afs_frame_range_type start, afs_frame_range_type count);


/*! 
  * \addtogroup file
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_frame_range  framerange.h
 *  \brief      Frame range data storage.
 *
 *  \param start  Frame range start position.
 *  \param count  Frame range count.
 *
 *  Structure for storing frame range data.
 */


// PUBLIC AFS FRAME RANGE FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Create a frame range instance.
 *
 *  Allocate memory for the afs_frame_range type
 *  and initializes all structure data with zero values.
 *  Return instance of allocated structure.
 *
 *  \return instance of allocated afs_frame_range structure.
 */

afs_frame_range * afs_frame_range_create()
{
    afs_frame_range * frame_range = BOXING_MEMORY_ALLOCATE_TYPE(afs_frame_range);
    afs_frame_range_init(frame_range);
    return frame_range;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create a frame range instance.
 *
 *  Allocate memory for the afs_frame_range type
 *  and initializes all structure data with specified values.
 *  Return instance of allocated structure.
 *
 *  \param start[in]  Frame range start position.
 *  \param count[in]  Frame range count.
 *  \return instance of allocated afs_frame_range structure.
 */

afs_frame_range * afs_frame_range_create2(afs_frame_range_type start, afs_frame_range_type count)
{
    afs_frame_range * frame_range = BOXING_MEMORY_ALLOCATE_TYPE(afs_frame_range);
    afs_frame_range_init2(frame_range, start, count);
    return frame_range;
}





//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_frame_range structure.
 *
 *  Frees occupied memory of structure pointer.
 *
 *  \param[in]  frame_range  Pointer to the afs_frame_range structure.
 */

void afs_frame_range_free(afs_frame_range * frame_range)
{
    if (frame_range == NULL)
    {
        return;
    }

    boxing_memory_free(frame_range);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function create a copy of input afs_frame_range structure.
 *
 *  Function create a copy of input afs_frame_range structure and return it.
 *  If frame range pointer is NULL function return NULL.
 *
 *  \param[in]  frame_range  Pointer to the afs_frame_range structure.
 *  \return new copy of afs_frame_range structure or NULL.
 */

afs_frame_range * afs_frame_range_clone(afs_frame_range * frame_range)
{
    if (frame_range == NULL)
    {
        return NULL;
    }

    return afs_frame_range_create2(frame_range->start, frame_range->count);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Function checks two instances of the afs_frame_range structures on the identity.
 *
 *  Function checks two instances of the afs_frame_range structures on the identity.
 *
 *  \param[in]   frame_range1  Pointer to the first instance of the afs_frame_range structure.
 *  \param[in]   frame_range2  Pointer to the second instance of the afs_frame_range structure.
 *  \return sign of identity of the input structures.
 */

DBOOL afs_frame_range_equal(afs_frame_range * frame_range1, afs_frame_range * frame_range2)
{
    if (frame_range1 == NULL && frame_range2 == NULL)
    {
        return DTRUE;
    }

    if (frame_range1 == NULL || frame_range2 == NULL)
    {
        return DFALSE;
    }

    if (frame_range1->start == frame_range2->start &&
        frame_range1->count == frame_range2->count)
    {
        return DTRUE;
    }

    return DFALSE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function returns end of the frame range.
 *
 *  The function returns end of the frame range.
 *  If frame range pointer is NULL or frame range count equal to zero then function return zero.
 *
 *  \param[in]   toc_file_preview_page   Pointer to the afs_toc_file_preview_page structure.
 *  \return the sign of integrity the afs_toc_file_preview_page structure.
 */

afs_frame_range_type afs_frame_range_get_end(const afs_frame_range * frame_range)
{
    if (frame_range == NULL)
    {
        return 0;
    }

    if (frame_range->count == 0)
    {
        return 0;
    }

    return frame_range->start + frame_range->count - 1;
}


//----------------------------------------------------------------------------
/*!
  * \} end of file group
  */


// PRIVATE AFS FRAME RANGE FUNCTIONS
//


static void afs_frame_range_init(afs_frame_range * frame_range)
{
    if (frame_range == NULL)
    {
        return;
    }

    frame_range->start = 0;
    frame_range->count = 0;
}


static void afs_frame_range_init2(afs_frame_range * frame_range, afs_frame_range_type start, afs_frame_range_type count)
{
    if (frame_range == NULL)
    {
        return;
    }

    frame_range->start = start;
    frame_range->count = count;
}
