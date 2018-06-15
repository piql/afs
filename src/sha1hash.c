/*****************************************************************************
**
**  Definition of the SHA1 checksum interface
**
**  Creation date:  2016/11/09
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

#include "sha1hash.h"
#include <stdio.h>

// PRIVATE INTERFACE
//

static int          compress(afs_hash1_state *md, unsigned char *buf);
static unsigned int rotl(const unsigned int value, int shift);

// CONSTANTS
//

const struct afs_ltc_hash_descriptor sha1_desc =
{
    "sha1",
    2,
    20,
    64,

    /* OID */
    { 1, 3, 14, 3, 2, 26, },
    6
};


//  DEFINES
//

#define AFS_LOAD32H(x, y)                 \
     { x = ((unsigned long)((y)[0] & 255)<<24) | \
           ((unsigned long)((y)[1] & 255)<<16) | \
           ((unsigned long)((y)[2] & 255)<<8)  | \
           ((unsigned long)((y)[3] & 255)); } 

#ifndef AFS_MIN
#define AFS_MIN(x, y) ( ((x)<(y))?(x):(y) )
#endif

#define AFS_STORE32H(x, y)                                                                  \
            { (y)[0] = (unsigned char)(((x)>>24)&255); (y)[1] = (unsigned char)(((x)>>16)&255); \
          (y)[2] = (unsigned char)(((x)>>8)&255); (y)[3] = (unsigned char)((x)&255); }

#define AFS_STORE64H(x, y)                                                                 \
           { (y)[0] = (unsigned char)(((x)>>56)&255); (y)[1] = (unsigned char)(((x)>>48)&255); \
         (y)[2] = (unsigned char)(((x)>>40)&255); (y)[3] = (unsigned char)(((x)>>32)&255); \
         (y)[4] = (unsigned char)(((x)>>24)&255); (y)[5] = (unsigned char)(((x)>>16)&255); \
         (y)[6] = (unsigned char)(((x)>>8)&255);  (y)[7] = (unsigned char)((x)&255); }

#define AFS_F0(x,y,z)  (z ^ (x & (y ^ z)))
#define AFS_F1(x,y,z)  (x ^ y ^ z)
#define AFS_F2(x,y,z)  ((x & y) | (z & (x | y)))
#define AFS_F3(x,y,z)  (x ^ y ^ z)


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
 *  \var    sha1_desc
 *  \brief  Constant for the SHA1 hash descriptor.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_sha1_state  sha1hash.h
 *  \brief      SHA1 calculation state.
 *
 *  \param length  Length.
 *  \param state   State.
 *  \param curlen  Current length.
 *  \param buf     Buffer.
 *
 *  Structure for storing SHA1 calculation state information.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_hash1_state_s  sha1hash.h
 *  \brief      SHA1 hash calculation state.
 *
 *  \param dummy  Dummy.
 *  \param sha1   SHA1 calculation state.
 *  \param data   Data.
 *
 *  Structure for storing SHA1 hash calculation state information.
 */


//----------------------------------------------------------------------------
/*!
 *  \struct     afs_ltc_hash_descriptor  sha1hash.h
 *  \brief      Hash descriptor.
 *
 *  \param name       Name of hash.
 *  \param ID         Internal ID.
 *  \param hashsize   Size of digest in octets.
 *  \param blocksize  Input block size in octets.
 *  \param OID        ASN.1 OID.
 *  \param OIDlen     Length of DER encoding.
 *
 *  Structure for storing hash descriptor data.
 */


// PUBLIC SHA1HASH FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Initialize the hash state.
 *
 *  Initialize the hash state.
 *
 *  \param[in] md   The hash state you wish to initialize.
 *  \return CRYPT_OK if successful.
 */

int afs_sha1_init(afs_hash1_state * md)
{
    if (md == NULL)
    {
        return CRYPT_NOP;
    }

    md->sha1.state[0] = 0x67452301UL;
    md->sha1.state[1] = 0xefcdab89UL;
    md->sha1.state[2] = 0x98badcfeUL;
    md->sha1.state[3] = 0x10325476UL;
    md->sha1.state[4] = 0xc3d2e1f0UL;
    md->sha1.curlen = 0;
    md->sha1.length = 0;
    return CRYPT_OK;
}

//----------------------------------------------------------------------------
/*!
 *  \brief Process the text.
 *
 *  Process the text, remember you can call process() multiple times.
 *
 *  \param[in] md    The hash state.
 *  \param[in] in    Input string.
 *  \param[in] inlen Length of the input string.
 *  \return CRYPT_OK if successful.
 */

int afs_sha1_process(afs_hash1_state * md, const unsigned char *in, unsigned long inlen)
{
    unsigned long n;
    int           err;

    if (md == NULL || in == NULL)
    {
        return CRYPT_NOP;
    }

    if (md->sha1.curlen > sizeof(md->sha1.buf))
    {
        return CRYPT_INVALID_ARG;
    }

    while (inlen > 0)
    {
        if (md->sha1.curlen == 0 && inlen >= 64)
        {
            if ((err = compress(md, (unsigned char *)in)) != CRYPT_OK)
            {
                return err;
            }
            md->sha1.length += 64 * 8;
            in += 64;
            inlen -= 64;
        }
        else
        {
            n = AFS_MIN(inlen, (64 - md->sha1.curlen));
            memcpy(md->sha1.buf + md->sha1.curlen, in, (size_t)n);
            md->sha1.curlen += n;
            in += n;
            inlen -= n;
            if (md->sha1.curlen == 64)
            {
                if ((err = compress(md, md->sha1.buf)) != CRYPT_OK)
                {
                    return err;
                }
                md->sha1.length += 8 * 64;
                md->sha1.curlen = 0;
            }
        }
    }

    return CRYPT_OK;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Terminate the hash to get the digest.
 *
 *  Terminate the hash to get the digest.
 *
 *  \param[in]  md    The hash state.
 *  \param[out] out   The destination of the hash (20 bytes).
 *  \return CRYPT_OK if successful.
 */

int afs_sha1_done(afs_hash1_state * md, unsigned char *out)
{
    int i;

    if (md == NULL || out == NULL)
    {
        return CRYPT_NOP;
    }

    if (md->sha1.curlen >= sizeof(md->sha1.buf))
    {
        return CRYPT_INVALID_ARG;
    }

    /* increase the length of the message */
    md->sha1.length += md->sha1.curlen * 8;

    /* append the '1' bit */
    md->sha1.buf[md->sha1.curlen++] = (unsigned char)0x80;

    /* if the length is currently above 56 bytes we append zeros
    * then compress.  Then we can fall back to padding zeros and length
    * encoding like normal.
    */
    if (md->sha1.curlen > 56)
    {
        while (md->sha1.curlen < 64)
        {
            md->sha1.buf[md->sha1.curlen++] = (unsigned char)0;
        }
        compress(md, md->sha1.buf);
        md->sha1.curlen = 0;
    }

    /* pad upto 56 bytes of zeroes */
    while (md->sha1.curlen < 56)
    {
        md->sha1.buf[md->sha1.curlen++] = (unsigned char)0;
    }

    /* store length */
    AFS_STORE64H(md->sha1.length, md->sha1.buf + 56);
    compress(md, md->sha1.buf);

    /* copy output */
    for (i = 0; i < 5; i++)
    {
        AFS_STORE32H(md->sha1.state[i], out + (4 * i));
    }

    return CRYPT_OK;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Convert hash to hexademical hash string.
 *
 *  Convert hash to hexademical hash string.
 *
 *  \param[in]  in    The input hash (20 bytes).
 *  \param[out] out   The destination of the hash (40 + 1 symbols).
 *  \return CRYPT_OK if successful.
 */

int afs_sha1_hash_to_hex_string(const unsigned char * in, char * out)
{
    if (NULL == in ||
        NULL == out)
    {
        return CRYPT_NOP;
    }

    char * return_string = out;
    for (unsigned int i = 0; i < sha1_desc.hashsize; i++)
    {
        sprintf(return_string, "%02x", in[i]);
        return_string += 2;
    }

    return CRYPT_OK;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Self-test the hash.
 *
 *  Self-test the hash.
 *
 *  \return CRYPT_OK if successful, CRYPT_NOP if self-tests have been disabled.
 */

int afs_sha1hash_test(void)
{
    static const struct {
        char *msg;
        unsigned char hash[20];
    } tests[] = {
        { "abc",
        { 0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81, 0x6a,
        0xba, 0x3e, 0x25, 0x71, 0x78, 0x50, 0xc2, 0x6c,
        0x9c, 0xd0, 0xd8, 0x9d }
        },
        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
        { 0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E,
        0xBA, 0xAE, 0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5,
        0xE5, 0x46, 0x70, 0xF1 }
        }
    };

    int i;
    unsigned char tmp[20];
    afs_hash1_state md;

    for (i = 0; i < (int)(sizeof(tests) / sizeof(tests[0])); i++) {
        afs_sha1_init(&md);
        afs_sha1_process(&md, (unsigned char*)tests[i].msg, (unsigned long)strlen(tests[i].msg));
        afs_sha1_done(&md, tmp);
        if (memcmp(tmp, tests[i].hash, 20) != 0) {
            return CRYPT_FAIL_TESTVECTOR;
        }
    }
    return CRYPT_OK;
}


//----------------------------------------------------------------------------
/*!
  * \} end of hash group
  */


// PRIVATE SHA1HASH FUNCTIONS
//

static unsigned int rotl(const unsigned int value, int shift)
{
    if ((shift &= sizeof(value) * 8 - 1) == 0)
    {
        return value;
    }

    return (value << shift) | (value >> (sizeof(value) * 8 - shift));
}

static int compress(afs_hash1_state *md, unsigned char *buf)
{
    unsigned long a, b, c, d, e, W[80], i;

    /* copy the state into 512-bits into W[0..15] */
    for (i = 0; i < 16; i++)
    {
        AFS_LOAD32H(W[i], buf + (4 * i));
    }

    /* copy state */
    a = md->sha1.state[0];
    b = md->sha1.state[1];
    c = md->sha1.state[2];
    d = md->sha1.state[3];
    e = md->sha1.state[4];

    /* expand it */
    for (i = 16; i < 80; i++)
    {
        W[i] = AFS_ROL(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
    }

    /* compress */
    /* round one */
#define FF0(a,b,c,d,e,i) e = (AFS_ROLc(a, 5) + AFS_F0(b,c,d) + e + W[i] + 0x5a827999UL); b = AFS_ROLc(b, 30);
#define FF1(a,b,c,d,e,i) e = (AFS_ROLc(a, 5) + AFS_F1(b,c,d) + e + W[i] + 0x6ed9eba1UL); b = AFS_ROLc(b, 30);
#define FF2(a,b,c,d,e,i) e = (AFS_ROLc(a, 5) + AFS_F2(b,c,d) + e + W[i] + 0x8f1bbcdcUL); b = AFS_ROLc(b, 30);
#define FF3(a,b,c,d,e,i) e = (AFS_ROLc(a, 5) + AFS_F3(b,c,d) + e + W[i] + 0xca62c1d6UL); b = AFS_ROLc(b, 30);

    /* round one */
    for (i = 0; i < 20;) {
        FF0(a, b, c, d, e, i++);
        FF0(e, a, b, c, d, i++);
        FF0(d, e, a, b, c, i++);
        FF0(c, d, e, a, b, i++);
        FF0(b, c, d, e, a, i++);
    }

    /* round two */
    for (; i < 40;)  {
        FF1(a, b, c, d, e, i++);
        FF1(e, a, b, c, d, i++);
        FF1(d, e, a, b, c, i++);
        FF1(c, d, e, a, b, i++);
        FF1(b, c, d, e, a, i++);
    }

    /* round three */
    for (; i < 60;)  {
        FF2(a, b, c, d, e, i++);
        FF2(e, a, b, c, d, i++);
        FF2(d, e, a, b, c, i++);
        FF2(c, d, e, a, b, i++);
        FF2(b, c, d, e, a, i++);
    }

    /* round four */
    for (; i < 80;)  {
        FF3(a, b, c, d, e, i++);
        FF3(e, a, b, c, d, i++);
        FF3(d, e, a, b, c, i++);
        FF3(c, d, e, a, b, i++);
        FF3(b, c, d, e, a, i++);
    }

#undef FF0
#undef FF1
#undef FF2
#undef FF3

    /* store */
    md->sha1.state[0] = md->sha1.state[0] + a;
    md->sha1.state[1] = md->sha1.state[1] + b;
    md->sha1.state[2] = md->sha1.state[2] + c;
    md->sha1.state[3] = md->sha1.state[3] + d;
    md->sha1.state[4] = md->sha1.state[4] + e;

    return CRYPT_OK;
}
