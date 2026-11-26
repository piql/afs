#ifndef AFS_SHA1_HASH_H
#define AFS_SHA1_HASH_H

/*****************************************************************************
**
**  Definition of the SHA1 checksum interface
**
**  Creation date:  2016/11/09
**  Created by:     Piql AS
**
**
**  Copyright (c) 2014 Piql AS. All rights reserved.
**
**  This file is part of the boxing library
**
*****************************************************************************/

#include "platform/io.h"
#include "afs_hash_result.h"
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


struct afs_sha1_state
{
    unsigned long long length;
    unsigned long      state[5];
    unsigned long      curlen;
    unsigned char      buf[64];
};


typedef union afs_hash1_state_s
{
    char                  dummy[1];
    struct afs_sha1_state sha1;
    void *                data;
} afs_hash1_state;


struct afs_ltc_hash_descriptor
{
    char *        name;
    unsigned char ID;
    unsigned long hashsize;
    unsigned long blocksize;
    unsigned long OID[16];
    unsigned long OIDlen;
};

int afs_sha1_init(afs_hash1_state * md);
int afs_sha1_process(afs_hash1_state * md, const unsigned char *in, unsigned long inlen);
int afs_sha1_done(afs_hash1_state * md, unsigned char *out);
int afs_sha1_hash_to_hex_string(const unsigned char * in, char * out);
int afs_sha1hash_test(void);
extern const struct afs_ltc_hash_descriptor sha1_desc;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_SHA1_HASH_H
