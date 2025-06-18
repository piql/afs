/*****************************************************************************
**
**  Implementation of the assembly tool test application
**
**  Creation date:  2017/05/10
**  Created by:     Aleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the unboxing library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "unboxingtool.h"
#include "unboxingutility.h"
#include "controldata.h"

//  SYSTEM INCLUDES
//
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//  DEFINES
//
#define LOGGING_ENABLED // Enables log output from the unboxing library

static DBOOL  boxing_is_digit(const char * string);
static DBOOL  boxing_is_range(const char * input_string);
static char * boxing_get_substring(char* input_string, size_t start_index, size_t end_index);

static void boxing_print_help_information()
{
    printf(
        "Assembly example application - unbox files from and archival file system reel.\n"
        "\n"
        "app -i <input files> -s <width> <height> -is_raw -read <file-ids>\n"
        "\n"
        "Where:\n"
        "   -i <input files>    : Input files wildcard.\n"
        "   -s <width> <height> : Specify input image width and height in pixels.\n"
        "   -is_raw             : Input images is in raw format.\n"
        "   -read <file-ids>    : Range of files to read, separated by spaces (1 2 3 4 5)\n"
        "                         or given by a range (start-end).\n"
        "Some examples:\n"
        "   -i ..\\testdata\\reel3\\*010d.raw -s 4096 2160 -is_raw             - Gets the control frame, extracts the TOC and outputs a list of files.\n"
        "   -i ..\\testdata\\reel3\\*010d.raw -s 4096 2160 -is_raw -read       - Gets the control frame, extracts the TOC, displays a list of files and unbox all files from the reel.\n"
        "   -i ..\\testdata\\reel3\\*010d.raw -s 4096 2160 -is_raw -read 0     - Gets the control frame, extracts the TOC, displays a list of files and unbox the file with the number 0.\n"
        "   -i ..\\testdata\\reel3\\*010d.raw -s 4096 2160 -is_raw -read 0 2   - Gets the control frame, extracts the TOC, displays a list of files and unbox the files with the number 0 and 2.\n"
        "   -i ..\\testdata\\reel3\\*010d.raw -s 4096 2160 -is_raw -read 0-2   - Gets the control frame, extracts the TOC, displays a list of files and unbox the files with the number from 0 to 2.\n\n"
        );
}


static unboxing_parameters unboxing_get_command_line_parameters(int argc, char *argv[])
{
    unboxing_parameters return_parameters;

    return_parameters.input_files = NULL;
    return_parameters.unbox_files_numbers = NULL;
    return_parameters.format = NULL;
    return_parameters.is_raw = DFALSE;
    return_parameters.is_read = DFALSE;
    return_parameters.show_help_information = DFALSE;
    return_parameters.x_size = 0;
    return_parameters.y_size = 0;

    int arg_index = 1;
    while (arg_index < argc)
    {
        if (strcmp(argv[arg_index], "-h") == 0)
        {
            return_parameters.show_help_information = DTRUE;
            arg_index++;
        }
        else if (strcmp(argv[arg_index], "-i") == 0)
        {
            return_parameters.input_files = argv[++arg_index];
            arg_index++;
        }
        else if (strcmp(argv[arg_index], "-s") == 0)
        {
            if (argc <= arg_index + 2)
            {
                break;
            }
            if (argv[arg_index + 1][0] == '-' || argv[arg_index + 2][0] == '-')
            {
                break;
            }
            return_parameters.x_size = atoi(argv[++arg_index]);
            return_parameters.y_size = atoi(argv[++arg_index]);
            arg_index++;
        }
        else if (strcmp(argv[arg_index], "-is_raw") == 0)
        {
            return_parameters.is_raw = DTRUE;
            arg_index++;
        }
        else if (strcmp(argv[arg_index], "-read") == 0)
        {
            return_parameters.is_read = DTRUE;
            return_parameters.unbox_files_numbers = gvector_create(sizeof(size_t), 0);

            while (++arg_index < argc && argv[arg_index][0] != '-' && (boxing_is_digit(argv[arg_index]) || boxing_is_range(argv[arg_index])))
            {
                if (boxing_is_digit(argv[arg_index]))
                {
                    gvector_append(return_parameters.unbox_files_numbers, 1);
                    GVECTORN(return_parameters.unbox_files_numbers, size_t, return_parameters.unbox_files_numbers->size - 1) = atoi(argv[arg_index]);
                }
                else
                {
                    int number_from = 0;
                    int number_to = 0;
                    for (size_t i = 0; i < strlen(argv[arg_index]); i++)
                    {
                        if (argv[arg_index][i] == '-')
                        {
                            char * string_number_from = boxing_get_substring(argv[arg_index], 0, i);
                            number_from = atoi(string_number_from);
                            free(string_number_from);

                            char * string_number_to = boxing_get_substring(argv[arg_index], i + 1, strlen(argv[arg_index]));
                            number_to = atoi(string_number_to);
                            free(string_number_to);
                            break;
                        }
                    }

                    if (number_to < number_from)
                    {
                        fprintf(stderr, "Invalid parameter to set range of numbers for unboxed files.\n");
                    }

                    for (int i = number_from; i <= number_to; i++)
                    {
                        gvector_append(return_parameters.unbox_files_numbers, 1);
                        GVECTORN(return_parameters.unbox_files_numbers, size_t, return_parameters.unbox_files_numbers->size - 1) = i;
                    }
                }
            }
        }
        else
        {
            fprintf(stderr, "Unsupported input parameter: %s\n", argv[arg_index++]);
        }
    }

    if (return_parameters.input_files == NULL || return_parameters.x_size == 0 || return_parameters.y_size == 0)
    {
        return_parameters.input_files = NULL;
    }

    if (return_parameters.input_files == NULL || return_parameters.show_help_information)
    {
        boxing_print_help_information();
    }

    return return_parameters;
}


static char * boxing_get_substring(char* input_string, size_t start_index, size_t end_index)
{
    if (input_string == NULL || start_index >= end_index)
    {
        return NULL;
    }

    size_t string_size = strlen(input_string);
    if (start_index >= string_size || end_index > string_size)
    {
        return NULL;
    }

    if (start_index == 0 && end_index == string_size)
    {
        return NULL;
    }

    size_t new_string_size = end_index - start_index;
    char* new_string = malloc(new_string_size + 1);
    
    for (size_t i = 0; i < new_string_size; i++)
    {
        new_string[i] = input_string[i + start_index];
    }

    new_string[new_string_size] = '\0';

    return new_string;
}


static DBOOL boxing_is_digit(const char * string)
{
    if (string == NULL)
    {
        return DFALSE;
    }

    if (strlen(string) == 0)
    {
        return DFALSE;
    }

    for (size_t i = 0; i < strlen(string); i++)
    {
        if (!isdigit(string[i]))
        {
            return DFALSE;
        }
    }

    return DTRUE;
}


static DBOOL boxing_is_range(const char * input_string)
{
    if (input_string == NULL)
    {
        return DFALSE;
    }

    size_t string_length = strlen(input_string);

    if (string_length == 0)
    {
        return DFALSE;
    }

    unsigned int string_index = 0;

    while (isdigit(input_string[string_index]) && string_index < string_length)
    {
        string_index++;
    }

    if (string_index == 0 || string_index > string_length - 2)
    {
        return DFALSE;
    }
    if (input_string[string_index++] != '-')
    {
        return DFALSE;
    }

    while (string_index < string_length)
    {
        if (!isdigit(input_string[string_index]))
        {
            return DFALSE;
        }
        string_index++;
    }

    return DTRUE;
}


static DBOOL boxing_value_is_present(gvector * unbox_files_numbers, size_t number)
{
    if (unbox_files_numbers == NULL)
    {
        return DFALSE;
    }

    if (unbox_files_numbers->size == 0)
    {
        return DFALSE;
    }

    for (size_t i = 0; i < unbox_files_numbers->size; i++)
    {
        if (GVECTORN(unbox_files_numbers, size_t, i) == number)
        {
            return DTRUE;
        }
    }

    return DFALSE;
}


static void boxing_check_files_numbers(gvector * unbox_files_numbers, const size_t tocs_count)
{
    if (unbox_files_numbers == NULL)
    {
        return;
    }

    gvector * temp_vector = gvector_create(sizeof(size_t), 0);

    if (unbox_files_numbers->size == 0)
    {
        for (size_t i = 0; i < tocs_count; i++)
        {
            gvector_append(temp_vector, 1);
            GVECTORN(temp_vector, size_t, temp_vector->size - 1) = i;
        }
    }
    else
    {
        for (size_t i = 0; i < unbox_files_numbers->size; i++)
        {
            size_t current_number = GVECTORN(unbox_files_numbers, size_t, i);
            if (current_number < tocs_count && !boxing_value_is_present(temp_vector, current_number))
            {
                gvector_append(temp_vector, 1);
                GVECTORN(temp_vector, size_t, temp_vector->size - 1) = current_number;
            }
        }
    }

    gvector_swap(unbox_files_numbers, temp_vector);

    gvector_free(temp_vector);
}


static void boxing_print_administrative_metadata(afs_administrative_metadata * administrative_metadata)
{
    printf("\nAdministrative data:\n");
    printf("Reel ID       - %s\n", administrative_metadata->reel_id);
    printf("Print reel ID - %s\n", administrative_metadata->print_reel_id);
    printf("Title         - %s\n", administrative_metadata->title);
    printf("Description   - %s\n", administrative_metadata->description);
    printf("Creator       - %s\n", administrative_metadata->creator);
    printf("Creation date - %s\n\n", administrative_metadata->creation_date);
}


static void boxing_print_technical_metadata(afs_technical_metadata * technical_metadata)
{
    printf("Technical data:\n");
    if (technical_metadata->afs_content_boxing_format != NULL)
    {
        const char * boxing_format_name = technical_metadata->afs_content_boxing_format->name;
        printf("Files unbox format - %s\n", boxing_format_name);
    }

    if (technical_metadata->afs_tocs != NULL)
    {
        size_t files_count = afs_toc_files_get_tocs_count(technical_metadata->afs_tocs);
        printf("Reel has %lu TOC files:\n", files_count);
        for (size_t i = 0; i < files_count; i++)
        {
            afs_toc_file * current_toc = afs_toc_files_get_toc(technical_metadata->afs_tocs, i);
            printf("%d: Name - %s, size - %d bytes, frames from %d to %d, bytes from %d to %d\n", (unsigned int)i, current_toc->name, (unsigned int)current_toc->size, current_toc->start_frame, current_toc->end_frame, current_toc->start_byte, current_toc->end_byte);
        }
    }
}


//---------------------------------------------------------------------------- 
/*! \ingroup unboxtests
 *
 *  Assembly tool sample application.
 *
 */

int main(int argc, char *argv[])
{
    // Get parameters from the command line
    unboxing_parameters parameters = unboxing_get_command_line_parameters(argc, argv);

    // If the parameters in the command line is invalid, then return
    if (parameters.input_files == NULL)
    {
        return -1;
    }

    // Unbox the control frame
    boxing_unbox_control_frame_file(parameters);

    // Obtain control data
    afs_control_data * control_data =  afs_control_data_create();
    DBOOL result = afs_control_data_load_file(control_data, "control_data.xml");
    if (result == DTRUE)
    {
        printf("\nThe control data was read successfully!\n");
        boxing_print_administrative_metadata(control_data->administrative_metadata);
        boxing_print_technical_metadata(control_data->technical_metadata);
    }
    else
    {
        fprintf(stderr, "Failed to read control_data.xml file.\n");
        return -1;
    }

    // Obtain TOC file
    printf("\nUnbox TOC files...\n");

    // Get the configuration
    parameters.format = control_data->technical_metadata->afs_content_boxing_format->config;

    afs_toc_file * current_toc = NULL;
    int unboxing_result = BOXING_UNBOXER_OK;

    // Unbox the TOC file. We are trying to unbox all the existing TOC files in turn, until the unboxing process is completed successfully.
    for (size_t i = 0; i < afs_toc_files_get_tocs_count(control_data->technical_metadata->afs_tocs); i++)
    {
        current_toc = afs_toc_files_get_toc(control_data->technical_metadata->afs_tocs, i);

        // Check if the required TOC file exists
        char * checksum_current_toc = afs_sha1_sum1(current_toc->name);
        if (strcmp(current_toc->checksum, checksum_current_toc) == 0)
        {
            printf("The required TOC file is already present, skip its unboxing.\n");
            free(checksum_current_toc);
            break;
        }
        free(checksum_current_toc);

        unboxing_result = boxing_unbox_data_frame_file(current_toc, parameters);
        
        if (unboxing_result == BOXING_UNBOXER_OK)
        {
            printf("\nThe TOC file was unboxed successfully!\n");
            break;
        }
    }

    if (unboxing_result != BOXING_UNBOXER_OK)
    {
        fprintf(stderr, "Filed to unbox toc file!\n");
        return -1;
    }

    // Read information about files from tocs
    afs_toc_files * toc_files = afs_toc_files_create();
    result = afs_toc_files_load_file(toc_files, current_toc->name);

    if (result == DFALSE)
    {
        fprintf(stderr, "Filed to read toc files strucure!\n");
    }

    // Display information about available files on the reel
    const size_t tocs_count = afs_toc_files_get_tocs_count(toc_files);
    printf("\nReceived data for %d files\n", (unsigned int)tocs_count);
    for (size_t i = 0; i < tocs_count; i++)
    {
        afs_toc_file * file_toc = afs_toc_files_get_toc(toc_files, i);
        printf("%02d: File name - %s, file format - %s \n", (unsigned int)i, file_toc->name, file_toc->file_format);
    }

    // Checking file numbers for unboxing
    if (parameters.is_read)
    {
        boxing_check_files_numbers(parameters.unbox_files_numbers, tocs_count);

        if (parameters.unbox_files_numbers->size != 0)
        {
            printf("\nUnboxing files number: ");

            for (size_t i = 0; i < parameters.unbox_files_numbers->size; i++)
            {
                printf("%lu", GVECTORN(parameters.unbox_files_numbers, size_t, i));
                if (i < parameters.unbox_files_numbers->size - 1)
                {
                    printf(", ");
                }
            }
            printf("\n");
        }
        else
        {
            fprintf(stderr, "\nNo files are specified for unboxing.\n");
        }

        // Unbox available files on the reel
        for (size_t i = 0; i < parameters.unbox_files_numbers->size; i++)
        {
            current_toc = afs_toc_files_get_toc(toc_files, (unsigned int)GVECTORN(parameters.unbox_files_numbers, size_t, i));
            printf("\nUNBOXING: File name - %s, file format - %s \n", current_toc->name, current_toc->file_format);
            unboxing_result = boxing_unbox_data_frame_file(current_toc, parameters);

            if (unboxing_result != BOXING_UNBOXER_OK)
            {
                fprintf(stderr, "Unboxing the file %s failed.\n", current_toc->name);
            }
        }
    }

    afs_control_data_free(control_data);
    gvector_free(parameters.unbox_files_numbers);
    afs_toc_files_free(toc_files);

    return 0;
}

#if defined (LOGGING_ENABLED)
void boxing_log(int log_level, const char * string)
{
    printf("%d : %s\n", log_level, string);
}

void boxing_log_args(int log_level, const char * format, ...)
{
    va_list args;
    va_start(args, format);

    printf("%d : ", log_level);
    vprintf(format, args);
    printf("\n");

    va_end(args);
}
#else
void boxing_log(int log_level, const char * string) { BOXING_UNUSED_PARAMETER(log_level); BOXING_UNUSED_PARAMETER(string); }
void boxing_log_args(int log_level, const char * format, ...) { BOXING_UNUSED_PARAMETER(log_level); BOXING_UNUSED_PARAMETER(format); }
#endif // LOGGING_ENABLED

void(*boxing_log_custom)(int log_level, const char * string) = NULL;
void(*boxing_log_args_custom)(int log_level, const char * format, va_list args) = NULL;
