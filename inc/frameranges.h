#ifndef AFS_FRAME_RANGES_H
#define AFS_FRAME_RANGES_H

/*****************************************************************************
**
**  Definition of frame ranges C interface
**
**  Creation date:  2018/05/03
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2018 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "framerange.h"
#include "gvector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef gvector afs_frame_ranges_vector;

typedef struct afs_frame_ranges_s
{
    afs_frame_ranges_vector *  frame_ranges;
} afs_frame_ranges;

afs_frame_ranges * afs_frame_ranges_create();
void               afs_frame_ranges_free(afs_frame_ranges * frame_ranges);

afs_frame_ranges * afs_frame_ranges_clone(const afs_frame_ranges * frame_ranges);
DBOOL              afs_frame_ranges_equal(const afs_frame_ranges * frame_ranges1, const afs_frame_ranges * frame_ranges2);

unsigned int       afs_frame_ranges_get_count(const afs_frame_ranges * frame_ranges);
afs_frame_range *  afs_frame_ranges_get_frame_range(const afs_frame_ranges * frame_ranges, const unsigned int index);
DBOOL              afs_frame_ranges_add_frame_range(afs_frame_ranges * frame_ranges, afs_frame_range * frame_range);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_FRAME_RANGES_H
