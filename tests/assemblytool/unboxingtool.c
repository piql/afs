/*****************************************************************************
**
**  Implementation of the unboxing tool test application
**
**  Creation date:  2017/10/05
**  Created by:     Aleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the boxing library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "platform/io.h"
#include "unboxingtool.h"
#include <stdio.h>
#include <string.h>
#include "boxing_config.h"
#include "boxing/utils.h"
#include "boxing/unboxer_utility.h"

static unsigned int metadata_content_type = BOXING_METADATA_CONTENT_TYPES_UNKNOWN;

static const char * result_names[] = { "OK",
                                       "METADATA ERROR",
                                       "BORDER TRACKING ERROR",
                                       "DATA DECODE ERROR",
                                       "CRC MISMATCH ERROR",
                                       "CONFIG ERROR",
                                       "PROCESS CALLBACK ABORT" };

// PRIVATE INTERFACE
//

static boxing_image8 * read_input_file(const char* input_file_name, const unsigned int width, const unsigned int height);
static int             unboxing_complete_callback(void * user, int* res, boxing_stats_decode * stats);
static int             unboxing_metadata_complete_callback(void * user, int* res, boxing_metadata_list * meta_data);
static const char *    get_process_result_name(enum boxing_unboxer_result result);
static DBOOL           check_frame_size(const boxing_int64 file_size, const unsigned int width, const unsigned int height, int* color_depth, DBOOL* hasHeader);
static DBOOL           normalize_image_data(boxing_image8* input_image, unsigned int color_depth);
static unsigned int    get_max_decimal_value(unsigned int bit_rate);
static void            change_symbols(char* input_file_name);
static char *          get_input_file_name(const char * file_name_template, unsigned int frame_number);
static char *          prepare_file_name(afs_toc_file * current_toc);
static char *          get_file_name_with_extension_from_path(const char * path);
static char *          get_file_name_from_path(const char * path);
static DBOOL           check_tar_format(char * input_string);
static DBOOL           save_output_data(gvector* output_data, const char * output_file_name, DBOOL is_first_frame, DBOOL is_last_frame, unsigned int start_byte_number, unsigned int end_byte_number);

// PUBLIC UNBOXING TOOL FUNCTIONS
//

//----------------------------------------------------------------------------
/*!
 *  \brief The function unboxing a file from a set of frames.
 *
 *  The function unboxing a file from a set of frames based on the specified TOC file.
 *  If the input current_toc instance is NULL or input template for the files is NULL or input format name is NULL or width or height is zero then the function returns an error.
 *
 *  \ingroup unbox
 *  \param[in]   current_toc  Input afs_toc_file instance.
 *  \param[in]   input_files  Template to detrmine the nemes of the input frame files.
 *  \param[in]   format       Name of the boxing format.
 *  \param[in]   width        Width of the frame in pixel.
 *  \param[in]   height       Height of the frame in pixel.
 *  \return process result.
 */

int boxing_unbox_data_frame_file(afs_toc_file * current_toc, unboxing_parameters parameters)
{
    if (current_toc == NULL || parameters.input_files == NULL || strcmp(parameters.input_files, "") == 0 || parameters.format == NULL || parameters.x_size == 0 || parameters.y_size == 0)
    {
        fprintf(stderr, "Invalid input data for unboxing file...\n");
        return -1;
    }

    // Set the output file name
    char * output_file_name = prepare_file_name(current_toc);
    // Set the start and endbytes
    unsigned int start_byte_number = current_toc->start_byte;
    unsigned int end_byte_number = current_toc->end_byte;

    // Reset the file if it exists and is not empty
#ifndef WIN32
    FILE * out_file = fopen(output_file_name, "w+");
#else
    FILE * out_file = fopen(output_file_name, "w+b");
#endif

    if (out_file == NULL)
    {
        fprintf(stderr, "Error creating ouput file with path %s!\n", output_file_name);
        return -1;
    }

    fclose(out_file);

    // Create utility to unbox file
#ifdef BOXINGLIB_CALLBACK
    boxing_unboxer_utility* utility = boxing_unboxer_utility_create(parameters.format, parameters.is_raw, unboxing_complete_callback, unboxing_metadata_complete_callback);
#else
    boxing_unboxer_utility* utility = boxing_unboxer_utility_create(parameters.format, parameters.is_raw);
#endif

    if (utility == NULL)
    {
        fprintf(stderr, "Error creating unboxer utility...\n");
        return -1;
    }

    int result = -1;
    int strip_size = boxing_codecdispatcher_get_stripe_size(utility->parameters->format) - 1;

    // Determine the number of the first frame with data (If the strip size is greater than 1).
    int first_frame_with_data = (strip_size > 0) ? current_toc->start_frame + strip_size : current_toc->start_frame;
    printf("First frame = %d, first frame with data = %d, last frame = %d\n", current_toc->start_frame, first_frame_with_data, current_toc->end_frame);

    // Unbox all frames in range
    for (int i = current_toc->start_frame; i <= current_toc->end_frame; i++)
    {
        // Reset the signs of the first and last frames
        DBOOL is_last_frame = DFALSE;
        DBOOL is_first_frame = DFALSE;

        // Determine the sign of the first frame with data
        if (i == first_frame_with_data)
        {
            is_first_frame = DTRUE;
        }

        // Determine the sign of the last frame with data
        if (i == current_toc->end_frame)
        {
            is_last_frame = DTRUE;
        }

        // Make the input file name from template
        char* input_file_name = get_input_file_name(parameters.input_files, i);

        printf("Unboxing frame %s\n", input_file_name);

        // Read the input file into the boxing_image8 instance
        boxing_image8 * input_image = read_input_file(input_file_name, (const unsigned int)parameters.x_size, (const unsigned int)parameters.y_size);

        // Allocate vector for the output data
        gvector* output_data = gvector_create(1, 0);

        // Unbox current file
        result = boxing_unboxer_utility_unbox(utility, input_image, output_data);

        // Save output data
        if (save_output_data(output_data, output_file_name, is_first_frame, is_last_frame, start_byte_number, end_byte_number) != 0)
        {
            return -1;
        }

        // Freeing memory
        free(input_file_name);
        boxing_image8_free(input_image);
        gvector_free(output_data);
    }

    // Checking the hash
    char * hash = afs_sha1_sum1(output_file_name);
    if (strcmp(current_toc->checksum, hash) != 0)
    {
        fprintf(stderr, "Invalid checksum for file %s\n", output_file_name);
        result = -1;
    }

    // Reset output file name
    free(output_file_name);
    output_file_name = NULL;

    // Freeing memory
    boxing_unboxer_utility_free(utility);
    free(hash);

    return result;
}


//----------------------------------------------------------------------------
/*!
 *  \brief The function unboxing a control frame file.
 *
 *  The function unboxing a control frame file.
 *  If input template for the files is NULL or width or height is zero then the function returns an error.
 *
 *  \ingroup unbox
 *  \param[in]   input_files  Template to detrmine the nemes of the input frame files.
 *  \param[in]   width        Width of the frame in pixel.
 *  \param[in]   height       Height of the frame in pixel.
 *  \return process result.
 */

int boxing_unbox_control_frame_file(unboxing_parameters parameters)
{
    // Make the input file name from template
    char* input_file_name = get_input_file_name(parameters.input_files, 1);

    // Set the output file name
    const char* output_file_name = "control_data.xml";

#ifndef WIN32
    FILE * out_file = fopen(output_file_name, "w+");
#else
    FILE * out_file = fopen(output_file_name, "w+b");
#endif
    fclose(out_file);

    // Read the input file into the boxing_image8 instance
    boxing_image8 * input_image = read_input_file(input_file_name, (const unsigned int)parameters.x_size, (const unsigned int)parameters.y_size);

    // We try all the configurations for the control frame until we find the correct one
    unsigned int count = boxing_get_control_frame_format_count();

    // Looking for formats for control frame
    printf("Looking for formats for control frame\n");
    int result = -1;

    for (int i = count - 1; i >= 0; i--)
    {
        // Get the name of the configuration from the configuration structure
        const char* current_format_name = boxing_get_control_frame_configuration_name(i);

        boxing_config *current_format_config = boxing_create_boxing_config(current_format_name);
       
        // Create utility to unbox file
#ifdef BOXINGLIB_CALLBACK
        boxing_unboxer_utility* utility = boxing_unboxer_utility_create(current_format_config, parameters.is_raw, unboxing_complete_callback, unboxing_metadata_complete_callback);
#else
        boxing_unboxer_utility* utility = boxing_unboxer_utility_create(current_format_config, parameters.is_raw);
#endif

        if (utility == NULL)
        {
            continue;
        }

        // Allocate vector for for the output data
        gvector* output_data = gvector_create(1, 0);

        // Reset the metadata_content_type
        metadata_content_type = BOXING_METADATA_CONTENT_TYPES_CONTROLFRAME;

        // Try to unbox control frame with current configuration name
        result = boxing_unboxer_utility_unbox(utility, input_image, output_data);

        if (result == BOXING_UNBOXER_OK && metadata_content_type == BOXING_METADATA_CONTENT_TYPES_CONTROLFRAME)
        {
            printf("Control frame successfully unboxed with configuration %s\n", current_format_name);

            // Save output data
            if (save_output_data(output_data, output_file_name, DTRUE, DTRUE, 0, (unsigned int)output_data->size - 1) != 0)
            {
                boxing_unboxer_utility_free(utility);
                gvector_free(output_data);

                return -1;
            }

            boxing_unboxer_utility_free(utility);
            gvector_free(output_data);
            break;
        }

        remove(output_file_name);
        boxing_unboxer_utility_free(utility);
        gvector_free(output_data);
    }

    // Reset output file name
    output_file_name = NULL;

    // Freeing memory
    free(input_file_name);
    boxing_image8_free(input_image);
    
    return result;
}


// PRIVATE UNBOXING TOOL FUNCTIONS
//

static DBOOL save_output_data(gvector* output_data, const char * output_file_name, DBOOL is_first_frame, DBOOL is_last_frame, unsigned int start_byte_number, unsigned int end_byte_number)
{ 
    // Open the output file for writing. The output file can be assembled from several parts, so we open it with the ability to add data.
#ifndef WIN32
    FILE * out_file = fopen(output_file_name, "a+");
#else
    FILE * out_file = fopen(output_file_name, "a+b");
#endif

    // We check if the file opened for writing
    if (out_file == NULL)
    {
        fprintf(stderr, "Failed to create output file %s\n", output_file_name);
        return -1;
    }

    unsigned char * data_location = (unsigned char *)output_data->buffer; // Variable to store a pointer to the beginning of the output data. By default, it indicates the beginning of the output buffer.
    size_t data_size = output_data->size; // Variable for storing the output data size for the current frame. By default, it is equal to the size of the output data buffer.

    unsigned int start_byte = 0; // Variable for storing the number of the start byte of the output data. By default, this indicates the beginning of the output.
    unsigned int end_byte = (unsigned int)output_data->size - 1; // Variable for storing the number of the end byte of the output data. By default, this indicates the end of the output.

    // For the first frame, correct the start byte
    if (is_first_frame == DTRUE)
    {
        start_byte = start_byte_number;
    }

    // For the last frame, correct the end byte
    if (is_last_frame == DTRUE)
    {
        end_byte = end_byte_number;
    }

    // Determine the size of the data and their position
    if (end_byte > start_byte && end_byte < output_data->size)
    {
        data_size = end_byte - start_byte + 1;
        data_location = ((unsigned char *)output_data->buffer) + start_byte;
    }

    // Write the data to the output file
    if (data_size != fwrite(data_location, output_data->item_size, data_size, out_file))
    {
        fclose(out_file);
        fprintf(stderr, "Output file write error.\n");
        return -1;
    }
    else
    {
        if (is_last_frame == DTRUE)
        {
            printf("Output data saved to the file %s\n\n", output_file_name);
        }
    }
    fclose(out_file);

    return BOXING_UNBOXER_OK;
}


static boxing_image8 * read_input_file(const char* input_file_name, const unsigned int width, const unsigned int height)
{
#ifndef WIN32
    FILE * file = fopen(input_file_name, "r");
#else
    FILE * file = fopen(input_file_name, "rb");
#endif
    if (file == NULL)
    {
        fprintf(stderr, "Failed to read file '%s'.\n", input_file_name);
        return NULL;
    }

    // Obtain file size
    if (DFSEEK(file, 0L, SEEK_END) != 0)
    {
        return NULL;
    }

    const boxing_int64 file_size = DFTELL(file);
    if (file_size < 0)
    {
        return NULL;
    }

    // Go to the starting data position
    if (DFSEEK(file, 0L, SEEK_SET) != 0)
    {
        return NULL;
    }

    // We can only match file size with input image size to validate input file
    int color_depth;
    DBOOL hasHeader;

    if (check_frame_size(file_size, width, height, &color_depth, &hasHeader) == DFALSE)
    {
        fprintf(stderr, "File '%s' size (%lld) does not match with input image size ", input_file_name, (signed long long)file_size);
        fprintf(stderr, "(width = %d, height = %d).\n", width, height);
        return NULL;
    }

    // Correct the start of the data if the file has a header
    if (hasHeader)
    {
        DFSEEK(file, 32, SEEK_SET);
    }

    // Read the data from the file
    boxing_image8* input_image = boxing_image8_create(width, height);
    if (1 != fread(input_image->data, (size_t)width * (size_t)height * color_depth / 8, 1, file))
    {
        fprintf(stderr, "Failed to read image data.\n");
        return NULL;
    }

    // Normalize data to 8 bit color depth
    if (color_depth < 8)
    {
        normalize_image_data(input_image, color_depth);
    }

    fclose(file);

    return input_image;
}


static DBOOL normalize_image_data(boxing_image8* input_image, unsigned int color_depth)
{
    if (input_image == NULL || color_depth > 8)
    {
        return DFALSE;
    }

    if (color_depth >= 8)
    {
        return DTRUE;
    }

    unsigned int input_bytes_count = input_image->width * input_image->height;
    unsigned int normalized_bytes_count = input_bytes_count * 8 / color_depth;

    boxing_image8_pixel * input_data = malloc(input_bytes_count);
    memcpy(input_data, input_image->data, input_bytes_count);

    free(input_image->data);
    input_image->data = malloc(normalized_bytes_count);

    for (unsigned int i = 0; i < input_bytes_count; i++)
    {
        for (unsigned int j = 0; j < 8 / color_depth; j++)
        {
            boxing_image8_pixel current_byte = input_data[i];

            unsigned int left_shift = 8 - color_depth * (j + 1);
            unsigned int right_shift = 8 - color_depth;

            current_byte = current_byte << left_shift;
            current_byte = current_byte >> right_shift;

            current_byte *= 255 / get_max_decimal_value(color_depth);
            input_image->data[8 / color_depth * i + j] = current_byte;
        }
    }

    free(input_data);

    return DTRUE;
}


static unsigned int get_max_decimal_value(unsigned int bit_rate)
{
    unsigned int max_decimal_value = 0;

    for (unsigned int i = 0; i < bit_rate; i++)
    {
        unsigned int current_bit_decimal_value = 1;

        for (unsigned int j = 0; j < i; j++)
        {
            current_bit_decimal_value *= 2;
        }

        max_decimal_value += current_bit_decimal_value;
    }

    return max_decimal_value;
}


static DBOOL check_frame_size(const boxing_int64 file_size, const unsigned int width, const unsigned int height, int* color_depth, DBOOL* hasHeader)
{
    const int max_degree = 4;
    int bit_rate = 1;

    for (int i = 0; i < max_degree; i++)
    {
        if (width * height * bit_rate / 8 == file_size)
        {
            *color_depth = bit_rate;
            *hasHeader = DFALSE;
            return DTRUE;
        }

        if (width * height * bit_rate / 8 + 64 == file_size)
        {
            *color_depth = bit_rate;
            *hasHeader = DTRUE;
            return DTRUE;
        }

        bit_rate *= 2;
    }

    return DFALSE;
}


//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 *
 *  Unboxing complete callback function.
 *
 *  \param user   User data
 *  \param res    Unboxing result.
 *  \param stats  Unboxing statistics
 */

static int unboxing_complete_callback(void * user, int* res, boxing_stats_decode * stats)
{
    // Hide unused parameters
    BOXING_UNUSED_PARAMETER(stats);
    BOXING_UNUSED_PARAMETER(user);

    // Checking the success of the unboxing
    if (*res != BOXING_UNBOXER_OK)
    {
        fprintf(stderr, "Unboxing failed! Errorcode = %i - %s\n", *res, get_process_result_name(*res));
        return -1;
    }

    return 0;
}


//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 *
 *  Unboxing metadata complete callback function.
 *
 *  \param user       User data
 *  \param res        Unboxing result.
 *  \param meta_data  Unboxing file meta data
 */

static int unboxing_metadata_complete_callback(void * user, int* res, boxing_metadata_list * meta_data)
{
    BOXING_UNUSED_PARAMETER(user);

    if (*res == BOXING_UNBOXER_OK)
    {
        GHashTableIter iter;
        gpointer key, value;

        g_hash_table_iter_init(&iter, meta_data);

        while (g_hash_table_iter_next(&iter, &key, &value))
        {
            boxing_metadata_item * item = (boxing_metadata_item *)value;

            boxing_metadata_type type = item->base.type;
            switch (type)
            {
            case BOXING_METADATA_TYPE_CONTENTTYPE:
                metadata_content_type = ((boxing_metadata_item_content_type*)item)->value;
                break;
            default:
                break;
            }
        }
    }
    return 0;
}



//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 *
 *  Getting the string representation of enum.
 *
 *  \param result  Result enum.
 */

static const char * get_process_result_name(enum boxing_unboxer_result result)
{
    return result_names[result];
}


//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 *
 *  Prepare inpute template string to use.
 */

static void change_symbols(char* input_file_name)
{
    for (unsigned int i = 0; i < strlen(input_file_name); i++)
    {
        if (input_file_name[i] == '*')
        {
            input_file_name[i] = '%';
        }
    }
}


static char * get_input_file_name(const char * file_name_template, unsigned int frame_number)
{
    if (file_name_template == NULL)
    {
        return NULL;
    }

    // Make a copy of the input string template to be able to modify it
    char* input_file_name = boxing_string_clone(file_name_template);
    // Prepare the string template
    change_symbols(input_file_name);
    // Make a control data frame file name
    char* control_data_frame_name = (char *)malloc(strlen(input_file_name) + 10);
    sprintf(control_data_frame_name, input_file_name, frame_number);

    free(input_file_name);

    return control_data_frame_name;
}


static char* prepare_file_name(afs_toc_file * current_toc)
{
    if (current_toc == NULL)
    {
        return NULL;
    }

    char* path = current_toc->name;
    
    if (afs_toc_file_has_parent(current_toc) && check_tar_format(current_toc->file_format))
    {
        char* file_name = get_file_name_from_path(path);
        char* tar_name = malloc(strlen(file_name) + 4 + 1);
        
        sprintf(tar_name, "%s.tar", file_name);

        free(file_name);
        return tar_name;
    }

    return get_file_name_with_extension_from_path(path);
}


//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 *
 *  Get the file name from its file path.
 *
 *  \param[in] path   Path to the file.
 *  \return name of the file extracted from its path.
 */

static char * get_file_name_from_path(const char * path)
{
    size_t length_path = strlen(path);
    size_t begin = 0;
    size_t end = length_path;

    // We are looking for the position of the start and end of the name
    for (size_t i = (length_path - 1);; i--)
    {
        if (path[i] == '.')
        {
            end = i;
        }

        if (path[i] == '\\' || path[i] == '/')
        {
            begin = i + 1;
        }

        if ((begin != 0 && end != 0) || i == 0)
        {
            break;
        }
    }

    char * output_name = malloc(sizeof(char) * ((end - begin) + 1));
    const char * begin_name = path + (sizeof(char) * (begin));
    output_name = strncpy(output_name, begin_name, (end - begin));
    output_name[(end - begin)] = '\0';

    return output_name;
}


static char * get_file_name_with_extension_from_path(const char * path)
{
    size_t length_path = strlen(path);
    size_t begin = 0;
    size_t end = length_path;

    // We are looking for the position of the start and end of the name
    for (size_t i = (length_path - 1);; i--)
    {
        if (path[i] == '\\' || path[i] == '/')
        {
            begin = i + 1;
        }

        if ((begin != 0 && end != 0) || i == 0)
        {
            break;
        }
    }

    char * output_name = malloc(sizeof(char) * ((end - begin) + 1));
    const char * begin_name = path + (sizeof(char) * (begin));
    output_name = strncpy(output_name, begin_name, (end - begin));
    output_name[(end - begin)] = '\0';

    return output_name;
}


static DBOOL check_tar_format(char * input_string)
{
    if (input_string == NULL)
    {
        return DFALSE;
    }

    char * format = "afs/tar";

    if (strlen(input_string) < strlen(format))
    {
        return DFALSE;
    }

    for (unsigned int i = 0; i < strlen(format); i++)
    {
        if (format[i] != input_string[i])
        {
            return DFALSE;
        }
    }

    return DTRUE;
}
