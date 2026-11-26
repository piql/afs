#ifndef AFS_HASH_RESULT_H
#define AFS_HASH_RESULT_H

/*****************************************************************************
**
**  Result codes shared by the SHA-1 and SHA-256 hash interfaces
**
**  Copyright (c) 2026 Piql AS. All rights reserved.
**
**  This file is part of the boxing library
**
*****************************************************************************/

//  SYSTEM INCLUDES
//

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \ingroup hash
 *
 *  Hash calculation result types.
 *
 *  These names and values were previously duplicated in sha1hash.h and
 *  sha256hash.h, which made the two headers impossible to include in the same
 *  translation unit. The values are unchanged; CRYPT_HASH_OVERFLOW was only
 *  present in the SHA-256 header and is kept here for all users.
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

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_HASH_RESULT_H
