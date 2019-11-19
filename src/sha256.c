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

//  PROJECT INCLUDES
//

#include "sha256.h"
#include "sha256hash.h"
#include "platform/io.h"
#include "boxing/log.h"
#include <stdio.h>
#include <errno.h>

//  DEFINES
//

// PRIVATE INTERFACE
//

static char* sum2_private(FILE *in, boxing_int64 start, boxing_int64 size);
static void close_file(FILE *file, const char* file_name);


/*! 
  * \addtogroup hash
  * \{
  */


// PUBLIC SHA256 FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief Calculates SHA256 hash for the file with data location parameters.
 *
 *  Calculate SHA256 hash by the given path to the file, start data position and data size in the input file.
 *
 *  \param[in] file_name   Path to the file for which we need to calculate the hash.
 *  \param[in] start       Start position in the file from which we begin to calculate the hash.
 *  \param[in] size        Size of the data in the file for which we will calculate hash.
 *  \return calculated hash for the file (NULL if error).
 */

char* afs_sha256_sum2(const char* file_name, boxing_int64 start, boxing_int64 size)
{
    FILE *file;
    char *check_sum = NULL;

    if (file_name == NULL)
    {
        return check_sum;
    }

    file = fopen(file_name, "rb");

    if (file == NULL)
    {
        return check_sum;
    }

    check_sum = sum2_private(file, start, size);

    close_file(file, file_name);
    return check_sum;
}


//----------------------------------------------------------------------------
/*!
 *  \brief Calculates SHA256 hash for the file.
 *
 *  Calculate SHA256 hash by the given path to the file.
 *
 *  \param[in] file_name   Path to the file for which we need to calculate the hash.
 *  \return calculated hash for the file ("" if error).
 */

char* afs_sha256_sum1(const char* file_name)
{
    return afs_sha256_sum2(file_name, 0, 0);
}

//----------------------------------------------------------------------------
/*!
 *  \brief Self-test the hash.
 *
 *  Self-test the calculates sha256 hash.
 *
 *  \return return CRYPT_OK if successful.
 */

int afs_sha256_test(void)
{
    return afs_sha256hash_test();
}


//----------------------------------------------------------------------------
/*!
  * \} end of hash group
  */


// PRIVATE SHA256 FUNCTIONS
//

// The function handle the input file stream and calculates the SHA256 hash for this file.

static int hash_filehandle(FILE *in, unsigned char *out, boxing_int64 size)
{
    if (in == NULL || out == NULL)
    {
        return CRYPT_NOP;
    }

    afs_hash256_state md;
    #define BUFFER_SIZE 512L
    unsigned char buf[BUFFER_SIZE];
    size_t bytes_read;
    int err;

    if ((err = afs_sha256_init(&md)) != CRYPT_OK) 
    {
        return err;
    }

    while (size)
    {
        size_t max_size = (size_t)(size > BUFFER_SIZE ? BUFFER_SIZE : size);
        bytes_read = (size_t)fread(buf, 1, max_size, in);
        if (!bytes_read)
        {
            if (errno != 0)
            {
                return err;
            }
            break;
        }
        if ((err = afs_sha256_process(&md, buf, (unsigned long)bytes_read)) != CRYPT_OK)
        {
            return err;
        }
        size -= bytes_read;
    };

    err = afs_sha256_done(&md, out);

    return err;
}


static char* sum2_private(FILE *in, boxing_int64 start, boxing_int64 size)
{
    int err;

    // Define the filesize
    if (DFSEEK(in, 0L, SEEK_END) != 0)
    {
        return NULL;
    }

    boxing_int64 file_size = DFTELL(in);
    if (file_size < 0)
    {
        return NULL;
    }


    // If the target data position is outside of the file, then exit with an error
    if (start + size > file_size)
    {
        return NULL;
    }

    // If the data size is set incorrectly, then define the size of the data in the file
    if (size == 0)
    {
        size = file_size - start;
    }

    // Go to the starting data position
    if (DFSEEK(in, start, SEEK_SET) != 0)
    {
        return NULL;
    }

    // Allocate memory to store the hash
    unsigned char* hashResult = (unsigned char*)malloc(sha256_desc.hashsize);

    // Handle file to calculate SHA1 hash
    err = hash_filehandle(in, hashResult, size);

    /// \todo Here you should check 'err' variable,
    /// for now, it's not used.
    (void)err;

    char* return_string = malloc(sha256_desc.hashsize * 2 + 1);
    for (unsigned int i = 0; i < sha256_desc.hashsize; i++)
    {
        sprintf(return_string, "%02x", hashResult[i]);
        return_string += 2;
    }

    return_string -= sha256_desc.hashsize * 2;

    free(hashResult);

    return return_string;
}


static void close_file(FILE *file, const char* file_name)
{
    if (fclose(file) != 0)
    {
        DLOG_INFO1("Error closing file %s.\n", file_name);
    }
}
