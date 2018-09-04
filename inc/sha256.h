#ifndef AFS_SHA256_H
#define AFS_SHA256_H

/*****************************************************************************
**
**  Definition of the SHA256 files checksum interface
**
**  Creation date:  2017/22/03
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the boxing library
**
*****************************************************************************/

#include "boxing/platform/types.h"

#ifdef __cplusplus
extern "C" {
#endif

    int   afs_sha256_test(void);
    char* afs_sha256_sum1(const char* file_name);
    char* afs_sha256_sum2(const char* file_name, boxing_int64 start, boxing_int64 size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_SHA256_H
