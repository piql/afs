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

/*!
 *  \ingroup hash
 *
 *  SHA256 hash calculation resut types.
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
    CRYPT_PK_INVALID_PADDING,

    CRYPT_HASH_OVERFLOW
};


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

#endif // AFS_SHA256_HASH_H
