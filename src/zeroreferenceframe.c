/*****************************************************************************
**
**  Definition of the toc zero reference mark interface
**
**  Creation date:  2017/03/15
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

#include "zeroreferenceframe.h"
#include "boxing/platform/memory.h"

//  DEFINES
//

#define FRAME_HEIGHT_MM 13.87f
#define PITCH_HEIGHT_MM 1.98f
#define PERF_PITCH_HEIGHT_MM 4.72f


//----------------------------------------------------------------------------
/*! 
 *  \defgroup zrf Zero reference frame
 *  \brief        First frame on the AFS reel.
 *  \ingroup afs
 *
 *  The zero reference frame shall always be the first frame of the AFS reel.
 *  It's purpose is to provide a machine readable start of reel indication,
 *  giving the physical position of frame 0. The frame following the ZRF shall
 *  always be the control frame.
 */


/*! 
  * \addtogroup zrf
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_zero_reference_mark_s  zeroreferenceframe.h
 *  \brief      Zero reference mark storage.
 *
 *  \param height      Height of the white area in pixels.
 *  \param width       Width of the white area in pixels.
 *  \param top_left_x  Top left X coordinate of the white area.
 *  \param top_left_y  Top left Y coordinate of the white area.
 *
 *  Structure for storing zero reference mark data.
 *  The Zero Reference Mark (ZRM) is a small black patch in the zero reference frame.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_zero_reference_frame_s  zeroreferenceframe.h
 *  \brief      Zero reference frame storage.
 *
 *  \param afs_zero_reference_mark  Pointer to the afs_zero_reference_mark structure.
 *  \param height                   Height of the frame in pixels.
 *  \param width                    Width of the frame in pixels.
 *
 *  Structure for storing zero reference frame data.
 *  The Zero Reference Frame (ZRF) is the first frame in the Storage Area,
 *  used to mark the start of the Storage Area.
 *  The ZRF consist of an empty frame with a small black patch - the Zero Reference Mark (ZRM).
 */



// PUBLIC AFS ZERO REFERENCE MARK FUNCTIONS
//


//----------------------------------------------------------------------------
/*!
 *  \brief Create a zero reference mark instance.
 *
 *  Allocate memory for the afs_zero_reference_mark type
 *  and initializes all structure values according to specified input values.
 *  Return instance of allocated structure.
 *  If width or height of the frame equal to zero, then function return NULL.
 *
 *  \param[in] frame_width   Width of the frame in pixel.
 *  \param[in] frame_height  Height of the frame in pixel.
 *  \return instance of allocated afs_zero_reference_mark structure.
 */

afs_zero_reference_mark* afs_zero_reference_mark_create(unsigned int frame_width, unsigned int frame_height)
{
    if (frame_width == 0 || frame_height == 0)
    {
        return NULL;
    }

    afs_zero_reference_mark* zero_reference_mark = BOXING_MEMORY_ALLOCATE_TYPE(afs_zero_reference_mark);

    if (!afs_zero_reference_mark_init(zero_reference_mark, frame_width, frame_height))
    {
        afs_zero_reference_mark_free(zero_reference_mark);
        return NULL;
    }

    return zero_reference_mark;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure values.
 *
 *  Initializes all input structure values according to specified input values.
 *  If zero_reference_mark input pointers is NULL or 
 *  one of the frame sizes equal to zero, then return without initialization.
 *
 *  \param[in]  zero_reference_mark  Pointer to the afs_zero_reference_mark structure.
 *  \param[in]  frame_width          Width of the frame in pixel.
 *  \param[in]  frame_height         Height of the frame in pixel.
 *  \return sign of success of the initialization.
 */

DBOOL afs_zero_reference_mark_init(afs_zero_reference_mark * zero_reference_mark, unsigned int frame_width, unsigned int frame_height)
{
    if (zero_reference_mark == NULL || frame_width == 0 || frame_height == 0)
    {
        return DFALSE;
    }

    float pixel_pitch = FRAME_HEIGHT_MM / (float)frame_height;

    unsigned int width = frame_width / 2;
    unsigned int height = (unsigned int)(PITCH_HEIGHT_MM / pixel_pitch);
    unsigned int top_left_x = width;
    unsigned int top_left_y = (unsigned int)((FRAME_HEIGHT_MM - (2.0f * PERF_PITCH_HEIGHT_MM + PITCH_HEIGHT_MM)) / (2.0f * pixel_pitch));

    if (width == 0 || height == 0 || top_left_x == 0 || top_left_y == 0)
    {
        return DFALSE;
    }

    zero_reference_mark->width = width;
    zero_reference_mark->height = height;
    zero_reference_mark->top_left_x = top_left_x;
    zero_reference_mark->top_left_y = top_left_y;

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_zero_reference_mark structure.
 *
 *  Frees occupied memory of structure pointer.
 *
 *  \param[in]  zero_reference_mark  Pointer to the afs_zero_reference_mark structure.
 */

void afs_zero_reference_mark_free(afs_zero_reference_mark* zero_reference_mark)
{
    if (zero_reference_mark == NULL)
    {
        return;
    }

    boxing_memory_free(zero_reference_mark);
}


// PUBLIC AFS ZERO REFERENCE FRAME FUNCTIONS
//


//----------------------------------------------------------------------------
/*!
 *  \brief Create a zero reference frame instance.
 *
 *  Allocate memory for the afs_zero_reference_frame type
 *  and initializes all structure values according to specified input values.
 *  Return instance of allocated structure.
 *  If width or height of the frame equal to zero, then function return NULL.
 *
 *  \param[in] frame_width   Width of the frame in pixel.
 *  \param[in] frame_height  Height of the frame in pixel.
 *  \return instance of allocated afs_zero_reference_frame structure.
 */

afs_zero_reference_frame* afs_zero_reference_frame_create(unsigned int frame_width, unsigned int frame_height)
{
    if (frame_width == 0 || frame_height == 0)
    {
        return NULL;
    }

    afs_zero_reference_frame* zero_reference_frame = BOXING_MEMORY_ALLOCATE_TYPE(afs_zero_reference_frame);
    zero_reference_frame->zero_reference_mark = NULL;

    if (afs_zero_reference_frame_init(zero_reference_frame, frame_width, frame_height) == DFALSE)
    {
        afs_zero_reference_frame_free(zero_reference_frame);
        return NULL;
    }

    return zero_reference_frame;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Initializes all structure values.
 *
 *  Initializes all input structure values according to specified input values.
 *  If zero_reference_frame input pointers is NULL or 
 *  one of the frame sizes equal to zero, then return without initialization.
 *
 *  \param[in]  zero_reference_frame  Pointer to the afs_zero_reference_frame structure.
 *  \param[in]  frame_width           Width of the frame in pixel.
 *  \param[in]  frame_height          Height of the frame in pixel.
 *  \return sign of success of the initialization.
 */

DBOOL afs_zero_reference_frame_init(afs_zero_reference_frame * zero_reference_frame, unsigned int frame_width, unsigned int frame_height)
{
    if (zero_reference_frame == NULL || frame_width == 0 || frame_height == 0)
    {
        return DFALSE;
    }
    
    afs_zero_reference_mark * zero_reference_mark = afs_zero_reference_mark_create(frame_width, frame_height);
    
    if (zero_reference_mark == NULL)
    {
        //zero_reference_frame->zero_reference_mark = NULL;
        return DFALSE;
    }

    zero_reference_frame->zero_reference_mark = zero_reference_mark;
    zero_reference_frame->width = frame_width;
    zero_reference_frame->height = frame_height;

    return DTRUE;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Frees occupied memory of afs_zero_reference_frame structure.
 *
 *  Frees occupied memory of structure pointer.
 *
 *  \param[in]  zero_reference_frame  Pointer to the afs_zero_reference_frame structure.
 */

void afs_zero_reference_frame_free(afs_zero_reference_frame* zero_reference_frame)
{
    if (zero_reference_frame == NULL)
    {
        return;
    }

    if (zero_reference_frame->zero_reference_mark != NULL)
    {
        afs_zero_reference_mark_free(zero_reference_frame->zero_reference_mark);
    }
    boxing_memory_free(zero_reference_frame);
}


//----------------------------------------------------------------------------
/*!
 *  \brief Create an image8 instance for zero reference frame.
 *
 *  Allocate memory for the image8 type according to specified zero reference frame instance
 *  and initialize all points.
 *  Return created an image8 instance.
 *  If input zero reference frame instance equal to NULL, then function return NULL.
 *
 *  \param[in] zero_reference_frame   Zero reference frame instance.
 *  \return created an image8 instance or NULL.
 */

boxing_image8 * afs_zero_reference_frame_create_image(afs_zero_reference_frame* zero_reference_frame)
{
    if (zero_reference_frame == NULL)
    {
        return NULL;
    }

    boxing_image8 * image = boxing_image8_create(zero_reference_frame->width, zero_reference_frame->height);

    if (image == NULL)
    {
        return NULL;
    }

    afs_zero_reference_mark * zero_reference_mark = zero_reference_frame->zero_reference_mark;

    for (unsigned int i = 0; i < zero_reference_frame->width; i++)
    {
        for (unsigned int j = 0; j < zero_reference_frame->height; j++)
        {
            if (i >= zero_reference_mark->top_left_x &&
                j >= zero_reference_mark->top_left_y &&
                j < (zero_reference_mark->top_left_y + zero_reference_mark->height))
            {
                IMAGE8_PIXEL(image, i, j) = BOXING_PIXEL_MIN;
            }
            else
            {
                IMAGE8_PIXEL(image, i, j) = BOXING_PIXEL_MAX;
            }
        }
    }

    return image;
}


//----------------------------------------------------------------------------
/*!
  * \} end of zrf group
  */
