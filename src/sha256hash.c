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

//  PROJECT INCLUDES
//

#include "sha256hash.h"
#include "boxing/config.h"
#include <string.h>
#include <stdio.h>


// PRIVATE INTERFACE
//

static int          compress(afs_hash256_state * md, unsigned char *buf);
static unsigned int rotr(const unsigned int value, int shift);

// CONSTANTS
//

const struct afs_ltc_hash256_descriptor sha256_desc =
{
    "sha256",
    0,
    32,
    64,

    /* OID */
    { 2, 16, 840, 1, 101, 3, 4, 2, 1 },
    9
};

//  DEFINES
//

#define AFS_ROR(x,n) rotr(x,n)
#define AFS_RORc(x,n) AFS_ROR(x,n)

#define AFS_Ch(x,y,z)       (z ^ (x & (y ^ z)))
#define AFS_Maj(x,y,z)      (((x | y) & z) | (x & y)) 
#define AFS_S(x, n)         AFS_RORc((x),(n))
#define AFS_R(x, n)         (((x)&0xFFFFFFFFUL)>>(n))
#define AFS_Sigma0(x)       (AFS_S(x, 2) ^ AFS_S(x, 13) ^ AFS_S(x, 22))
#define AFS_Sigma1(x)       (AFS_S(x, 6) ^ AFS_S(x, 11) ^ AFS_S(x, 25))
#define AFS_Gamma0(x)       (AFS_S(x, 7) ^ AFS_S(x, 18) ^ AFS_R(x, 3))
#define AFS_Gamma1(x)       (AFS_S(x, 17) ^ AFS_S(x, 19) ^ AFS_R(x, 10))

#define LOAD32H(x, y)                                  \
    do                                                 \
        {                                              \
        x = ((unsigned long)((y)[0] & 255)<<24) |      \
            ((unsigned long)((y)[1] & 255)<<16) |      \
            ((unsigned long)((y)[2] & 255)<<8)  |      \
            ((unsigned long)((y)[3] & 255));           \
        } while(0)

#define AFS_STORE32H(x, y)                                                                  \
        { (y)[0] = (unsigned char)(((x)>>24)&255); (y)[1] = (unsigned char)(((x)>>16)&255); \
          (y)[2] = (unsigned char)(((x)>>8)&255); (y)[3] = (unsigned char)((x)&255); }

#define AFS_STORE64H(x, y)                                                                  \
        { (y)[0] = (unsigned char)(((x)>>56)&255); (y)[1] = (unsigned char)(((x)>>48)&255); \
          (y)[2] = (unsigned char)(((x)>>40)&255); (y)[3] = (unsigned char)(((x)>>32)&255); \
          (y)[4] = (unsigned char)(((x)>>24)&255); (y)[5] = (unsigned char)(((x)>>16)&255); \
          (y)[6] = (unsigned char)(((x)>>8)&255);  (y)[7] = (unsigned char)((x)&255); }

// A simple macro for making sha-3 hash "process" functions
#define AFS_SHA3_HASH_PROCESS(func_name, compress_name, state_var, block_size)              \
int func_name (afs_hash256_state * md, const unsigned char *in, unsigned long inlen)        \
{                                                                                           \
    unsigned long n;                                                                        \
    int           err;                                                                      \
                                                                                            \
    if(md == NULL || in == NULL)                                                            \
    {                                                                                       \
        return CRYPT_ERROR;                                                                 \
    }                                                                                       \
                                                                                            \
    if (md->state_var.curlen > sizeof(md->state_var.buf))                                   \
    {                                                                                       \
        return CRYPT_INVALID_ARG;                                                           \
    }                                                                                       \
                                                                                            \
    if ((md->state_var.length + inlen) < md->state_var.length)                              \
    {	                                                                                    \
        return CRYPT_HASH_OVERFLOW;                                                         \
    }                                                                                       \
                                                                                            \
    while (inlen > 0)                                                                       \
    {                                                                                       \
        if (md->state_var.curlen == 0 && inlen >= block_size)                               \
        {                                                                                   \
            if ((err = compress_name (md, (unsigned char *)in)) != CRYPT_OK)                \
            {                                                                               \
                return err;                                                                 \
            }                                                                               \
                                                                                            \
            md->state_var.length += block_size * 8;                                         \
            in += block_size;                                                               \
            inlen -= block_size;                                                            \
        } else                                                                              \
        {                                                                                   \
            n = MIN(inlen, (block_size - md->state_var.curlen));                            \
            memcpy(md->state_var.buf + md->state_var.curlen, in, (size_t)n);                \
            md->state_var.curlen += n;                                                      \
            in += n;                                                                        \
            inlen -= n;                                                                     \
            if (md->state_var.curlen == block_size)                                         \
            {                                                                               \
                if ((err = compress_name(md, md->state_var.buf)) != CRYPT_OK)               \
                {                                                                           \
                    return err;                                                             \
                }                                                                           \
                                                                                            \
                md->state_var.length += 8*block_size;                                       \
                md->state_var.curlen = 0;                                                   \
            }                                                                               \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    return CRYPT_OK;                                                                        \
}


/*! 
  * \addtogroup hash
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_OK
 *  \brief     Result OK.
 *  
 *  Hash calculation result OK.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_ERROR
 *  \brief     Generic Error.
 *  
 *  Hash calculation result generic error.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_NOP
 *  \brief     No operation.
 *  
 *  Not a failure but no operation was performed.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_INVALID_KEYSIZE
 *  \brief     Invalid key size given.
 *  
 *  Hash calculation result - Invalid key size given.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_INVALID_ROUNDS
 *  \brief     Invalid number of rounds.
 *  
 *  Hash calculation result - Invalid number of rounds.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_FAIL_TESTVECTOR
 *  \brief     Algorithm failed test vectors.
 *  
 *  Hash calculation result - Algorithm failed test vectors.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_BUFFER_OVERFLOW
 *  \brief     Not enough space for output.
 *  
 *  Hash calculation result - Not enough space for output.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_INVALID_PACKET
 *  \brief     Invalid input packet given.
 *  
 *  Hash calculation result - Invalid input packet given.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_INVALID_PRNGSIZE
 *  \brief     Invalid number of bits for a PRNG.
 *  
 *  Hash calculation result - Invalid number of bits for a PRNG.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_ERROR_READPRNG
 *  \brief     Could not read enough from PRNG.
 *  
 *  Hash calculation result - Could not read enough from PRNG.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_INVALID_CIPHER
 *  \brief     Invalid cipher specified.
 *  
 *  Hash calculation result - Invalid cipher specified.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_INVALID_HASH
 *  \brief     Invalid hash specified.
 *  
 *  Hash calculation result - Invalid hash specified.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_INVALID_PRNG
 *  \brief     Invalid PRNG specified.
 *  
 *  Hash calculation result - Invalid PRNG specified.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_MEM
 *  \brief     Out of memory.
 *  
 *  Hash calculation result - Out of memory.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_PK_TYPE_MISMATCH
 *  \brief     Not equivalent types of PK keys.
 *  
 *  Hash calculation result - Not equivalent types of PK keys.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_PK_NOT_PRIVATE
 *  \brief     Requires a private PK key.
 *  
 *  Hash calculation result - Requires a private PK key.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_INVALID_ARG
 *  \brief     Generic invalid argument.
 *  
 *  Hash calculation result - Generic invalid argument.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_FILE_NOTFOUND
 *  \brief     File Not Found.
 *  
 *  Hash calculation result - File not found.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_PK_INVALID_TYPE
 *  \brief     Invalid type of PK key.
 *  
 *  Hash calculation result - Invalid type of PK key.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_PK_INVALID_SYSTEM
 *  \brief     Invalid PK system specified.
 *  
 *  Hash calculation result - Invalid PK system specified.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_PK_DUP
 *  \brief     Duplicate key already in key ring.
 *  
 *  Hash calculation result - Duplicate key already in key ring.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_PK_NOT_FOUND
 *  \brief     Key not found in keyring.
 *  
 *  Hash calculation result - Key not found in keyring.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_PK_INVALID_SIZE
 *  \brief     Invalid size input for PK parameters.
 *  
 *  Hash calculation result - Invalid size input for PK parameters.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_INVALID_PRIME_SIZE
 *  \brief     Invalid size of prime requested.
 *  
 *  Hash calculation result - Invalid size of prime requested.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_PK_INVALID_PADDING
 *  \brief     Invalid padding on input.
 *  
 *  Hash calculation result - Invalid padding on input.
 */


//----------------------------------------------------------------------------
/*!
 *  \var enum  CRYPT_HASH_OVERFLOW
 *  \brief     Hash applied to too many bits.
 *  
 *  Hash calculation result - Hash applied to too many bits.
 */


//----------------------------------------------------------------------------
/*!
 *  \var    sha256_desc
 *  \brief  Constant for the SHA256 hash descriptor.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_sha256_state  sha256hash.h
 *  \brief      SHA256 calculation state.
 *
 *  \param length  Length.
 *  \param state   State.
 *  \param curlen  Current length.
 *  \param buf     Buffer.
 *
 *  Structure for storing SHA256 calculation state information.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_hash256_state_s  sha256hash.h
 *  \brief      SHA256 hash calculation state.
 *
 *  \param dummy  Dummy.
 *  \param sha1   SHA256 calculation state.
 *  \param data   Data.
 *
 *  Structure for storing SHA256 hash calculation state information.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_ltc_hash256_descriptor  sha256hash.h
 *  \brief      Hash descriptor.
 *
 *  \param name       Name of hash.
 *  \param ID         Internal ID.
 *  \param hashsize   Size of digest in octets.
 *  \param blocksize  Input block size in octets.
 *  \param OID        ASN.1 OID.
 *  \param OIDlen     Length of DER encoding.
 *
 *  Structure for storing SHA256 hash descriptor data.
 */


// PUBLIC SHA256 HASH FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Initialize the hash state.
 *
 *  Initialize the SHA256 hash state.
 *
 *  \param[in]  md   The hash state you wish to initialize.
 *  \return CRYPT_OK if successful.
 */

int afs_sha256_init(afs_hash256_state * md)
{
    if (md == NULL)
    {
        return CRYPT_NOP;
    }

    md->afs_sha256.state[0] = 0x6A09E667UL;
    md->afs_sha256.state[1] = 0xBB67AE85UL;
    md->afs_sha256.state[2] = 0x3C6EF372UL;
    md->afs_sha256.state[3] = 0xA54FF53AUL;
    md->afs_sha256.state[4] = 0x510E527FUL;
    md->afs_sha256.state[5] = 0x9B05688CUL;
    md->afs_sha256.state[6] = 0x1F83D9ABUL;
    md->afs_sha256.state[7] = 0x5BE0CD19UL;
    md->afs_sha256.curlen = 0;
    md->afs_sha256.length = 0;

    return CRYPT_OK;
}


//----------------------------------------------------------------------------
/*!
 *  \def AFS_SHA3_HASH_PROCESS(func_name, compress_name, state_var, block_size) sha256hash.h
 *  \brief A simple macro for making sha-3 hash "process" functions.
 *
 *  A simple macro for making sha-3 hash "process" functions.
 *
 *  \param[in]  func_name      The name of the function.
 *  \param[in]  compress_name  The name of the compress function.
 *  \param[in]  state_var      State variable.
 *  \param[in]  block_size     Size of the block.
 */

AFS_SHA3_HASH_PROCESS(afs_sha256_process, compress, afs_sha256, 64)


//----------------------------------------------------------------------------
/*!
 *  \brief Terminate the hash to get the digest.
 *
 *  Terminate the SHA256 hash to get the digest.
 *
 *  \param[in]  md      The hash state.
 *  \param[out] out     The destination of the hash (32 bytes).
 *  \return CRYPT_OK if successful.
 */

 int afs_sha256_done(afs_hash256_state * md, unsigned char *out)
{
    int i;

    if (md == NULL || out == NULL)
    {
        return CRYPT_ERROR;
    }

    if (md->afs_sha256.curlen >= sizeof(md->afs_sha256.buf)) 
    {
        return CRYPT_INVALID_ARG;
    }

    /* increase the length of the message */
    md->afs_sha256.length += md->afs_sha256.curlen * 8;

    /* append the '1' bit */
    md->afs_sha256.buf[md->afs_sha256.curlen++] = (unsigned char)0x80;

    /* if the length is currently above 56 bytes we append zeros
    * then compress.  Then we can fall back to padding zeros and length
    * encoding like normal.
    */
    if (md->afs_sha256.curlen > 56) 
    {
        while (md->afs_sha256.curlen < 64) 
        {
            md->afs_sha256.buf[md->afs_sha256.curlen++] = (unsigned char)0;
        }

        compress(md, md->afs_sha256.buf);
        md->afs_sha256.curlen = 0;
    }

    /* pad upto 56 bytes of zeroes */
    while (md->afs_sha256.curlen < 56) 
    {
        md->afs_sha256.buf[md->afs_sha256.curlen++] = (unsigned char)0;
    }

    /* store length */
    AFS_STORE64H(md->afs_sha256.length, md->afs_sha256.buf + 56);
    compress(md, md->afs_sha256.buf);

    /* copy output */
    for (i = 0; i < 8; i++) 
    {
        AFS_STORE32H(md->afs_sha256.state[i], out + (4 * i));
    }

    return CRYPT_OK;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Self-test the hash.
 *
 *  Self-test calculation of the SHA256 hash.
 *
 *  \return CRYPT_OK if successful.
 */

int  afs_sha256hash_test(void)
{
    static const struct 
    {
        char *msg;
        unsigned char hash[32];
    } tests[] = 
    {
        { "abc",
        { 0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea,
        0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
        0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
        0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad }
        },
        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
        { 0x24, 0x8d, 0x6a, 0x61, 0xd2, 0x06, 0x38, 0xb8,
        0xe5, 0xc0, 0x26, 0x93, 0x0c, 0x3e, 0x60, 0x39,
        0xa3, 0x3c, 0xe4, 0x59, 0x64, 0xff, 0x21, 0x67,
        0xf6, 0xec, 0xed, 0xd4, 0x19, 0xdb, 0x06, 0xc1 }
        },
    };

    int i;
    unsigned char tmp[32];
    afs_hash256_state md;

    for (i = 0; i < (int)(sizeof(tests) / sizeof(tests[0])); i++) 
    {
        int hash_result;

        hash_result = afs_sha256_init(&md);
        hash_result = afs_sha256_process(&md, (unsigned char*)tests[i].msg, (unsigned long)strlen(tests[i].msg));
        hash_result = afs_sha256_done(&md, tmp);

        /// \todo warning: variable ‘hash_result’ set but not used
        (void)hash_result;

        if (memcmp(tmp, tests[i].hash, 32) != 0)
        {
            return CRYPT_FAIL_TESTVECTOR;
        }
    }

    return CRYPT_OK;
}


//----------------------------------------------------------------------------
/*!
  * \} end of hash group
  */


// PRIVATE SHA256 HASH FUNCTIONS
//

static int  compress(afs_hash256_state * md, unsigned char *buf)
{
    unsigned long S[8], W[64], t0, t1;
    int i;

    /* copy state into S */
    for (i = 0; i < 8; i++)
    {
        S[i] = md->afs_sha256.state[i];
    }

    /* copy the state into 512-bits into W[0..15] */
    for (i = 0; i < 16; i++)
    {
        LOAD32H(W[i], buf + (4 * i));
    }

    /* fill W[16..63] */
    for (i = 16; i < 64; i++)
    {
        W[i] = AFS_Gamma1(W[i - 2]) + W[i - 7] + AFS_Gamma0(W[i - 15]) + W[i - 16];
    }

    /* Compress */
    #define RND(a,b,c,d,e,f,g,h,i,ki)                        \
     t0 = h + AFS_Sigma1(e) + AFS_Ch(e, f, g) + ki + W[i];   \
     t1 = AFS_Sigma0(a) + AFS_Maj(a, b, c);                  \
     d += t0;                                                \
     h  = t0 + t1;

    RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 0, 0x428a2f98);
    RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 1, 0x71374491);
    RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 2, 0xb5c0fbcf);
    RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 3, 0xe9b5dba5);
    RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 4, 0x3956c25b);
    RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 5, 0x59f111f1);
    RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 6, 0x923f82a4);
    RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 7, 0xab1c5ed5);
    RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 8, 0xd807aa98);
    RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 9, 0x12835b01);
    RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 10, 0x243185be);
    RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 11, 0x550c7dc3);
    RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 12, 0x72be5d74);
    RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 13, 0x80deb1fe);
    RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 14, 0x9bdc06a7);
    RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 15, 0xc19bf174);
    RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 16, 0xe49b69c1);
    RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 17, 0xefbe4786);
    RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 18, 0x0fc19dc6);
    RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 19, 0x240ca1cc);
    RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 20, 0x2de92c6f);
    RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 21, 0x4a7484aa);
    RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 22, 0x5cb0a9dc);
    RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 23, 0x76f988da);
    RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 24, 0x983e5152);
    RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 25, 0xa831c66d);
    RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 26, 0xb00327c8);
    RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 27, 0xbf597fc7);
    RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 28, 0xc6e00bf3);
    RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 29, 0xd5a79147);
    RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 30, 0x06ca6351);
    RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 31, 0x14292967);
    RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 32, 0x27b70a85);
    RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 33, 0x2e1b2138);
    RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 34, 0x4d2c6dfc);
    RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 35, 0x53380d13);
    RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 36, 0x650a7354);
    RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 37, 0x766a0abb);
    RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 38, 0x81c2c92e);
    RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 39, 0x92722c85);
    RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 40, 0xa2bfe8a1);
    RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 41, 0xa81a664b);
    RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 42, 0xc24b8b70);
    RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 43, 0xc76c51a3);
    RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 44, 0xd192e819);
    RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 45, 0xd6990624);
    RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 46, 0xf40e3585);
    RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 47, 0x106aa070);
    RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 48, 0x19a4c116);
    RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 49, 0x1e376c08);
    RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 50, 0x2748774c);
    RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 51, 0x34b0bcb5);
    RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 52, 0x391c0cb3);
    RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 53, 0x4ed8aa4a);
    RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 54, 0x5b9cca4f);
    RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 55, 0x682e6ff3);
    RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 56, 0x748f82ee);
    RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 57, 0x78a5636f);
    RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 58, 0x84c87814);
    RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 59, 0x8cc70208);
    RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 60, 0x90befffa);
    RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 61, 0xa4506ceb);
    RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 62, 0xbef9a3f7);
    RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 63, 0xc67178f2);

    #undef RND     

    /* feedback */
    for (i = 0; i < 8; i++) {
        md->afs_sha256.state[i] = md->afs_sha256.state[i] + S[i];
    }

    return CRYPT_OK;
}


unsigned int rotr(const unsigned int value, int shift) 
{
    if ((shift &= sizeof(value) * 8 - 1) == 0)
    {
        printf("!\n");
        return value;
    }

    return (value >> shift) | (value << (sizeof(value) * 8 - shift));
}
