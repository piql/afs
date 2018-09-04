#ifndef AFS_ZERO_REFERENCE_FRAME_H
#define AFS_ZERO_REFERENCE_FRAME_H

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

#include "boxing/image8.h"

#ifdef __cplusplus
extern "C" {
#endif

// Definition structure afs_zero_reference_mark
typedef struct afs_zero_reference_mark_s
{
    unsigned int height;     // Height of the black area in pixels
    unsigned int width;      // Width of the black area in pixels
    unsigned int top_left_x; // Top left X coordinate of the white area
    unsigned int top_left_y; // Top left Y coordinate of the white area
} afs_zero_reference_mark;

// Definition structure afs_zero_reference_frame
typedef struct afs_zero_reference_frame_s
{
    afs_zero_reference_mark * zero_reference_mark;
    unsigned int height;     // Height of the frame in pixels
    unsigned int width;      // Width of the frame in pixels
} afs_zero_reference_frame;

afs_zero_reference_mark* afs_zero_reference_mark_create(unsigned int frame_width, unsigned int frame_height);
DBOOL                    afs_zero_reference_mark_init(afs_zero_reference_mark * zero_reference_mark, unsigned int frame_width, unsigned int frame_height);
void                     afs_zero_reference_mark_free(afs_zero_reference_mark* zero_reference_mark);

afs_zero_reference_frame* afs_zero_reference_frame_create(unsigned int frame_width, unsigned int frame_height);
DBOOL                     afs_zero_reference_frame_init(afs_zero_reference_frame * zero_reference_frame, unsigned int frame_width, unsigned int frame_height);
void                      afs_zero_reference_frame_free(afs_zero_reference_frame* zero_reference_frame);

boxing_image8 *          afs_zero_reference_frame_create_image(afs_zero_reference_frame* zero_reference_frame);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_ZERO_REFERENCE_FRAME_H
