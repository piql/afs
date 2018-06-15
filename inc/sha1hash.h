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

#ifdef __cplusplus
extern "C" {
#endif

#include "platform/io.h"
#include <string.h>
#include <stdlib.h>


/*!
 *  \ingroup hash
 *
 *  SHA1 hash calculation resut types.
 */

enum {
    CRYPT_OK = 0,
    CRYPT_ERROR,
    CRYPT_NOP,

    CRYPT_INVALID_KEYSIZE,
    CRYPT_INVALID_ROUNDS,
    CRYPT_FAIL_TESTVECTOR,

    CRYPT_BUFFER_OVERFLOW,
    CRYPT_INVALID_PACKET,

    CRYPT_INVALID_PRNGSIZE,
    CRYPT_ERROR_READPRNG,

    CRYPT_INVALID_CIPHER,
    CRYPT_INVALID_HASH,
    CRYPT_INVALID_PRNG,

    CRYPT_MEM,

    CRYPT_PK_TYPE_MISMATCH,
    CRYPT_PK_NOT_PRIVATE,

    CRYPT_INVALID_ARG,
    CRYPT_FILE_NOTFOUND,

    CRYPT_PK_INVALID_TYPE,
    CRYPT_PK_INVALID_SYSTEM,
    CRYPT_PK_DUP,
    CRYPT_PK_NOT_FOUND,
    CRYPT_PK_INVALID_SIZE,

    CRYPT_INVALID_PRIME_SIZE,
    CRYPT_PK_INVALID_PADDING
};


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
