#ifndef AFS_FRAME_RANGE_H
#define AFS_FRAME_RANGE_H

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

#include "boxing/bool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int afs_frame_range_type;

// Definition structure afs_frame_range
typedef struct afs_frame_range_s
{
    afs_frame_range_type  start;
    afs_frame_range_type  count;
} afs_frame_range;


afs_frame_range * afs_frame_range_create();
afs_frame_range * afs_frame_range_create2(afs_frame_range_type start, afs_frame_range_type count);
void              afs_frame_range_free(afs_frame_range * frame_range);

afs_frame_range *    afs_frame_range_clone(afs_frame_range * frame_range);
DBOOL                afs_frame_range_equal(afs_frame_range * frame_range1, afs_frame_range * frame_range2);
afs_frame_range_type afs_frame_range_get_end(const afs_frame_range * frame_range);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_FRAME_RANGE_H
