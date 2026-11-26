#ifndef AFS_SHA256_HASH_H
#define AFS_SHA256_HASH_H

/*****************************************************************************
**
**  Definition of the SHA256 checksum interface
**
**  Creation date:  2017/03/20
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include <stdlib.h>
#include "afs_hash_result.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \ingroup hash
 *
 *  SHA256 hash calculation.
 *
 *  The result codes shared with the SHA-1 interface are in afs_hash_result.h.
 */

struct afs_sha256_state
{
    unsigned long long length;
    unsigned long      state[8];
    unsigned long      curlen;
    unsigned char      buf[64];
};


typedef union afs_hash256_state_s
{
    char                    dummy[1];
    struct afs_sha256_state afs_sha256;
    void *                  data;
} afs_hash256_state;


struct afs_ltc_hash256_descriptor
{
    char *        name;
    unsigned char ID;
    unsigned long hashsize;
    unsigned long blocksize;
    unsigned long OID[16];
    unsigned long OIDlen;
};


int afs_sha256_init(afs_hash256_state * md);
int afs_sha256_process(afs_hash256_state * md, const unsigned char *in, unsigned long inlen);
int afs_sha256_done(afs_hash256_state * md, unsigned char *hash);
int afs_sha256hash_test(void);

extern const struct afs_ltc_hash256_descriptor sha256_desc;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_SHA256_HASH_H
