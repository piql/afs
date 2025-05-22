#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "controldata.h"
#include <stdio.h>
#include "boxing_config.h"
#include "boxing/utils.h"
#include "mxml.h"

#if defined ( D_OS_WIN32 )
#define DFSEEK _fseeki64
#define DFTELL _ftelli64
#else
#define DFSEEK fseeko
#define DFTELL ftello
#endif

static boxing_config *     get_config_instance();
static afs_boxing_format * create_boxing_format_instance();
static afs_toc_files *     create_toc_files_instance();


static afs_toc_file_preview_pages* get_filled_pages_vector4(unsigned int pages_count)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(pages_count);

    for (unsigned int i = 0; i < pages_count; i++)
    {
        GVECTORN(pages, afs_toc_file_preview_page *, i) = afs_toc_file_preview_page_create2("Some layout ID", 9 + i, 8 + i, 7 + i, 6 + i, 5 + i, 4 + i, 3 + i);
    }

    return pages;
}


static afs_toc_data_file_metadata_sources * get_filled_toc_data_file_metadata_sources_vector(unsigned int sources_count)
{
    afs_toc_data_file_metadata_sources * sources = gvector_create_pointers(sources_count);

    for (unsigned int i = 0; i < sources_count; i++)
    {
        GVECTORN(sources, afs_toc_data_file_metadata_source *, i) = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", i + 1, i + 2);
    }

    return sources;
}


static afs_toc_data_file_metadata * get_afs_toc_data_file_metadata_instance(unsigned int sources_count)
{
    afs_toc_data_file_metadata_sources * sources = get_filled_toc_data_file_metadata_sources_vector(sources_count);
    afs_toc_data_file_metadata * toc_data_file_metadata = afs_toc_data_file_metadata_create2(sources);

    return toc_data_file_metadata;
}


static afs_toc_file* get_afs_toc_file_instance2(unsigned int number)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector4(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(5);

    return afs_toc_file_create2(
        "input-toc.xml",
        "2016-11-22 09:07:11",
        "f2eb0d6c6964d206012695a10ea6cd877c95c340",
        29672 + number,
        AFS_TOC_FILE_NO_PARENT,
        2 + number,
        0 + number,
        2 + number,
        29671 + number,
        "38A0AAAB-16B2-640C-5353-6DB8AE4367B9",
        AFS_TOC_FILE_NO_PARENT,
        AFS_TOC_FILE_TYPE_ALL,
        toc_file_preview,
        "file_format",
        toc_data_file_metadata);
}


static afs_toc_indices * get_filled_afs_tocs_vector(const size_t tocs_count)
{
    afs_toc_indices* tocs = gvector_create_pointers(tocs_count);

    for (size_t i = 0; i < tocs_count; i++)
    {
        afs_toc_file* toc_file = get_afs_toc_file_instance2((unsigned int)i);
        GVECTORN(tocs, afs_toc_file *, i) = toc_file;
    }

    return tocs;
}


static void test_empty_afs_administrative_metadata(afs_administrative_metadata* administrative_metadata)
{
    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(administrative_metadata->creation_date == NULL);
    BOXING_ASSERT(administrative_metadata->creator == NULL);
    BOXING_ASSERT(administrative_metadata->description == NULL);
    BOXING_ASSERT(administrative_metadata->print_reel_id == NULL);
    BOXING_ASSERT(administrative_metadata->reel_id == NULL);
    BOXING_ASSERT(administrative_metadata->title == NULL);
}


static void test_strings_afs_administrative_metadata(
    afs_administrative_metadata* administrative_metadata,
    const char* reel_id,
    const char* print_reel_id,
    const char* title,
    const char* description,
    const char* creator,
    const char* creation_date)
{
    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(administrative_metadata->creation_date != NULL);
    BOXING_ASSERT(boxing_string_equal(creation_date, administrative_metadata->creation_date) == DTRUE);
    BOXING_ASSERT(administrative_metadata->creator != NULL);
    BOXING_ASSERT(boxing_string_equal(creator, administrative_metadata->creator) == DTRUE);
    BOXING_ASSERT(administrative_metadata->description != NULL);
    BOXING_ASSERT(boxing_string_equal(description, administrative_metadata->description) == DTRUE);
    BOXING_ASSERT(administrative_metadata->print_reel_id != NULL);
    BOXING_ASSERT(boxing_string_equal(print_reel_id, administrative_metadata->print_reel_id) == DTRUE);
    BOXING_ASSERT(administrative_metadata->reel_id != NULL);
    BOXING_ASSERT(boxing_string_equal(reel_id, administrative_metadata->reel_id) == DTRUE);
    BOXING_ASSERT(administrative_metadata->title != NULL);
    BOXING_ASSERT(boxing_string_equal(title, administrative_metadata->title) == DTRUE);
}


static void test_empty_afs_technical_metadata(afs_technical_metadata * technical_metadata)
{
    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(technical_metadata->afs_tocs == NULL);
    BOXING_ASSERT(technical_metadata->afs_content_boxing_format == NULL);
}


static void test_afs_technical_metadata(afs_technical_metadata * technical_metadata, int format_test_mode, int tocs_count)
{
    BOXING_ASSERT(technical_metadata != NULL);

    if (format_test_mode == -1)
    {
        BOXING_ASSERT(technical_metadata->afs_content_boxing_format == NULL);
    }
    else if (format_test_mode == 0)
    {
        BOXING_ASSERT(technical_metadata->afs_content_boxing_format != NULL);
        BOXING_ASSERT(technical_metadata->afs_content_boxing_format->config == NULL);
    }
    else
    {
        boxing_config * config = get_config_instance();

        BOXING_ASSERT(technical_metadata->afs_content_boxing_format != NULL);
        BOXING_ASSERT(technical_metadata->afs_content_boxing_format->config != NULL);
        BOXING_ASSERT(boxing_config_is_equal(config, technical_metadata->afs_content_boxing_format->config) == DTRUE);
        boxing_config_free(config);
    }

    if (tocs_count < 0)
    {
        BOXING_ASSERT(technical_metadata->afs_tocs == NULL);
    }
    else
    {
        BOXING_ASSERT(technical_metadata->afs_tocs != NULL);
        BOXING_ASSERT((size_t)tocs_count == afs_toc_files_get_tocs_count(technical_metadata->afs_tocs));

        for (int i = 0; i < tocs_count; i++)
        {
            afs_toc_file * afs_toc_file_instance = get_afs_toc_file_instance2((unsigned int)i);
            BOXING_ASSERT(afs_toc_file_equal(afs_toc_files_get_toc(technical_metadata->afs_tocs, i), afs_toc_file_instance) == DTRUE);
            afs_toc_file_free(afs_toc_file_instance);
        }
    }
}


static void test_empty_afs_control_data(afs_control_data * control_data)
{
    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(control_data->administrative_metadata == NULL);
    BOXING_ASSERT(control_data->technical_metadata == NULL);
}


static void test_afs_control_data(afs_control_data * control_data, int administrative_metadata_test_mode, int technical_metadata_test_mode)
{
    BOXING_ASSERT(control_data != NULL);

    if (administrative_metadata_test_mode == -1)
    {
        BOXING_ASSERT(control_data->administrative_metadata == NULL);
    }
    else if (administrative_metadata_test_mode == 0)
    {
        test_empty_afs_administrative_metadata(control_data->administrative_metadata);
    }
    else
    {
        test_strings_afs_administrative_metadata(control_data->administrative_metadata, "reel id", "print reel id", "title", "description", "creator", "creation date");
    }

    if (technical_metadata_test_mode == -1)
    {
        BOXING_ASSERT(control_data->technical_metadata == NULL);
    }
    else if (technical_metadata_test_mode == 0)
    {
        test_empty_afs_technical_metadata(control_data->technical_metadata);
    }
    else
    {
        test_afs_technical_metadata(control_data->technical_metadata, 1, 3);
    }
}


static boxing_config * get_config_instance()
{
    boxing_config * config = boxing_create_boxing_config("4kv7");

    return config;
}

static afs_boxing_format * create_boxing_format_instance()
{
    boxing_config * config = get_config_instance();
    afs_boxing_format* boxing_format = afs_boxing_format_create2(config);
    boxing_config_free(config);
    return boxing_format;
}


static afs_toc_files * create_toc_files_instance(int files_count)
{
    afs_toc_indices * tocs_data = get_filled_afs_tocs_vector(files_count);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs_data);
    return toc_files;
}


static afs_technical_metadata * create_technical_metadata_instance(int files_count, int boxing_format_mode)
{
    afs_toc_files * toc_files;

    if (files_count < 0)
    {
        toc_files = NULL;
    }
    else if (files_count == 0)
    {
        toc_files = afs_toc_files_create();
    }
    else
    {
        toc_files = create_toc_files_instance(files_count);
    }

    afs_boxing_format * boxing_format;

    if (boxing_format_mode < 0)
    {
        boxing_format = NULL;
    }
    else if (boxing_format_mode == 0)
    {
        boxing_format = afs_boxing_format_create();
    }
    else
    {
        boxing_format = create_boxing_format_instance();
    }

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create2(toc_files, boxing_format);

    return technical_metadata;
}


static afs_control_data * create_control_data_instance(int administrative_metadata_mode, int technical_metadata_mode)
{
    afs_administrative_metadata * administrative_metadata;

    if (administrative_metadata_mode < 0)
    {
        administrative_metadata = NULL;
    }
    else if (administrative_metadata_mode == 0)
    {
        administrative_metadata = afs_administrative_metadata_create();
    }
    else
    {
        administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    }

    afs_technical_metadata * technical_metadata;

    if (technical_metadata_mode < 0)
    {
        technical_metadata = NULL;
    }
    else if (technical_metadata_mode == 0)
    {
        technical_metadata = afs_technical_metadata_create();
    }
    else
    {
        technical_metadata = create_technical_metadata_instance(3, 1);
    }

    afs_control_data * control_data = afs_control_data_create2(administrative_metadata, technical_metadata);

    return control_data;
}


static void test_equals_administrative_metadata(afs_administrative_metadata * administrative_metadata1, afs_administrative_metadata * administrative_metadata2, DBOOL expected_result)
{
    DBOOL result = afs_administrative_metadata_equal(administrative_metadata2, administrative_metadata1);
    BOXING_ASSERT(result == expected_result);
    result = afs_administrative_metadata_equal(administrative_metadata1, administrative_metadata2);
    BOXING_ASSERT(result == expected_result);
}


static void test_equals_technical_metadata(afs_technical_metadata * technical_metadata1, afs_technical_metadata * technical_metadata2, DBOOL expected_result)
{
    DBOOL result = afs_technical_metadata_equal(technical_metadata2, technical_metadata1);
    BOXING_ASSERT(result == expected_result);
    result = afs_technical_metadata_equal(technical_metadata1, technical_metadata2);
    BOXING_ASSERT(result == expected_result);
}


static void test_equals_control_data(afs_control_data * control_data1, afs_control_data * control_data2, DBOOL expected_result)
{
    DBOOL result = afs_control_data_equal(control_data2, control_data1);
    BOXING_ASSERT(result == expected_result);
    result = afs_control_data_equal(control_data1, control_data2);
    BOXING_ASSERT(result == expected_result);
}


static char * read_xml_control_data(const char * file_name)
{
    // If there is no file name, then return NULL
    if (file_name == NULL)
    {
        return NULL;
    }

    // Open the file for reading
#ifndef WIN32
    FILE * file = fopen(file_name, "r");
#else
    FILE * file = fopen(file_name, "rb");
#endif

    // If the file does not open, then return NULL
    if (file == NULL)
    {
        return NULL;
    }

    // Define the filesize
    if (DFSEEK(file, 0L, SEEK_END) != 0)
    {
        fclose(file);
        return NULL;
    }

    boxing_int64 size = DFTELL(file);

    // Go to the starting data position
    if (DFSEEK(file, 0, SEEK_SET) != 0)
    {
        fclose(file);
        return NULL;
    }

    // Creates a vector vor the input data
    char * xml_string = boxing_string_allocate((size_t)size + 1);

    // Reads the data from the input file
    if (1 != fread(xml_string, (size_t)size, 1, file))
    {
        fclose(file);
        return NULL;
    }

    xml_string[size] = '\0';

    fclose(file);
    return xml_string;
}


static mxml_node_t * get_control_data_xml_instance(const char * file_name)
{
#ifndef WIN32
    FILE * fp_load = fopen(file_name, "r");
#else
    FILE * fp_load = fopen(file_name, "rb");
#endif

    if (fp_load == NULL)
    {
        return NULL;
    }

    mxml_node_t * document = mxmlLoadFile(NULL, fp_load, MXML_OPAQUE_CALLBACK);

    fclose(fp_load);

    return document;
}


// TEST ADMINISTRATIVE METADATA FUNCTIONS
//

// Test function afs_administrative_metadata_create test
BOXING_START_TEST(afs_administrative_metadata_create_test1)
{
    afs_administrative_metadata* administrative_metadata = afs_administrative_metadata_create();

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_create2 with input pointers equal to NULL
BOXING_START_TEST(afs_administrative_metadata_create2_test1)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2(NULL, NULL, NULL, NULL, NULL, NULL);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_create2 with empty input strings
BOXING_START_TEST(afs_administrative_metadata_create2_test2)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("", "", "", "", "", "");

    test_strings_afs_administrative_metadata(administrative_metadata, "", "", "", "", "", "");

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_create2 with not empty input strings
BOXING_START_TEST(afs_administrative_metadata_create2_test3)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    test_strings_afs_administrative_metadata(administrative_metadata, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_clone with input pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_clone_test1)
{
    afs_administrative_metadata * administrative_metadata1 = NULL;

    BOXING_ASSERT(administrative_metadata1 == NULL);

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_clone(administrative_metadata1);

    BOXING_ASSERT(administrative_metadata2 == NULL);
}
END_TEST


// Test function afs_administrative_metadata_clone with empty input afs_administrative_metadata instance
BOXING_START_TEST(afs_administrative_metadata_clone_test2)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create();

    test_empty_afs_administrative_metadata(administrative_metadata1);

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_clone(administrative_metadata1);

    test_empty_afs_administrative_metadata(administrative_metadata2);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_clone with not empty input afs_administrative_metadata instance with empty strings
BOXING_START_TEST(afs_administrative_metadata_clone_test3)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("", "", "", "", "", "");

    test_strings_afs_administrative_metadata(administrative_metadata1, "", "", "", "", "", "");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_clone(administrative_metadata1);

    test_strings_afs_administrative_metadata(administrative_metadata2, "", "", "", "", "", "");

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_clone test with not empty input afs_administrative_metadata instance
BOXING_START_TEST(afs_administrative_metadata_clone_test4)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_clone(administrative_metadata1);

    test_strings_afs_administrative_metadata(administrative_metadata2, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_get_new_reference_test1)
{
    afs_administrative_metadata * administrative_metadata1 = NULL;

    BOXING_ASSERT(administrative_metadata1 == NULL);

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_get_new_reference(administrative_metadata1);

    BOXING_ASSERT(administrative_metadata2 == NULL);

    administrative_metadata1 = afs_administrative_metadata_create();

    test_empty_afs_administrative_metadata(administrative_metadata1);
    BOXING_ASSERT(administrative_metadata1->reference_count == 1);
    BOXING_ASSERT(administrative_metadata2 == NULL);

    afs_administrative_metadata_free(administrative_metadata1);
}
END_TEST


// Test function afs_administrative_metadata_get_new_reference with empty input afs_administrative_metadata instance
BOXING_START_TEST(afs_administrative_metadata_get_new_reference_test2)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create();

    test_empty_afs_administrative_metadata(administrative_metadata1);
    BOXING_ASSERT(administrative_metadata1->reference_count == 1);

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_get_new_reference(administrative_metadata1);

    test_empty_afs_administrative_metadata(administrative_metadata2);
    BOXING_ASSERT(administrative_metadata1->reference_count == 2);
    BOXING_ASSERT(administrative_metadata2->reference_count == 2);
    BOXING_ASSERT(administrative_metadata1 == administrative_metadata2);

    administrative_metadata1->creator = boxing_string_clone("Some other creator!");

    BOXING_ASSERT(boxing_string_equal(administrative_metadata2->creator, "Some other creator!") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_get_new_reference with not empty input afs_administrative_metadata instance with empty strings
BOXING_START_TEST(afs_administrative_metadata_get_new_reference_test3)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("", "", "", "", "", "");

    test_strings_afs_administrative_metadata(administrative_metadata1, "", "", "", "", "", "");
    BOXING_ASSERT(administrative_metadata1->reference_count == 1);

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_get_new_reference(administrative_metadata1);

    test_strings_afs_administrative_metadata(administrative_metadata2, "", "", "", "", "", "");
    BOXING_ASSERT(administrative_metadata1->reference_count == 2);
    BOXING_ASSERT(administrative_metadata2->reference_count == 2);
    BOXING_ASSERT(administrative_metadata1 == administrative_metadata2);

    boxing_string_free(administrative_metadata1->creator);
    administrative_metadata1->creator = boxing_string_clone("Some other creator!");

    BOXING_ASSERT(boxing_string_equal(administrative_metadata2->creator, "Some other creator!") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_get_new_reference test with not empty input afs_administrative_metadata instance
BOXING_START_TEST(afs_administrative_metadata_get_new_reference_test4)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(administrative_metadata1->reference_count == 1);

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_get_new_reference(administrative_metadata1);

    test_strings_afs_administrative_metadata(administrative_metadata2, "reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(administrative_metadata1->reference_count == 2);
    BOXING_ASSERT(administrative_metadata2->reference_count == 2);
    BOXING_ASSERT(administrative_metadata1 == administrative_metadata2);

    boxing_string_free(administrative_metadata1->creator);
    administrative_metadata1->creator = boxing_string_clone("Some other creator!");

    BOXING_ASSERT(boxing_string_equal(administrative_metadata2->creator, "Some other creator!") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with input afs_administrative_metadata pointers equal to NULL
BOXING_START_TEST(afs_administrative_metadata_equal_test1)
{
    afs_administrative_metadata * administrative_metadata1 = NULL;
    afs_administrative_metadata * administrative_metadata2 = NULL;

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DTRUE);
}
END_TEST

// Test function afs_administrative_metadata_equal with empty input afs_administrative_metadata instances 
BOXING_START_TEST(afs_administrative_metadata_equal_test2)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create();
    test_empty_afs_administrative_metadata(administrative_metadata1);

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create();
    test_empty_afs_administrative_metadata(administrative_metadata2);

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DTRUE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with not empty input afs_administrative_metadata instances with empty strings
BOXING_START_TEST(afs_administrative_metadata_equal_test3)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("", "", "", "", "", "");
    test_strings_afs_administrative_metadata(administrative_metadata1, "", "", "", "", "", "");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create2("", "", "", "", "", "");
    test_strings_afs_administrative_metadata(administrative_metadata2, "", "", "", "", "", "");

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DTRUE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with not empty input afs_administrative_metadata instances with not empty strings
BOXING_START_TEST(afs_administrative_metadata_equal_test4)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata2, "reel id", "print reel id", "title", "description", "creator", "creation date");

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DTRUE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with empty input afs_administrative_metadata instance and pointer equal to NULL 
BOXING_START_TEST(afs_administrative_metadata_equal_test5)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create();
    test_empty_afs_administrative_metadata(administrative_metadata1);

    afs_administrative_metadata * administrative_metadata2 = NULL;

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
}
END_TEST


// Test function afs_administrative_metadata_equal with input afs_administrative_metadata instance with empty strings and pointer equal to NULL 
BOXING_START_TEST(afs_administrative_metadata_equal_test6)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("", "", "", "", "", "");
    test_strings_afs_administrative_metadata(administrative_metadata1, "", "", "", "", "", "");

    afs_administrative_metadata * administrative_metadata2 = NULL;

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
}
END_TEST


// Test function afs_administrative_metadata_equal with input afs_administrative_metadata instance with not empty strings and pointer equal to NULL 
BOXING_START_TEST(afs_administrative_metadata_equal_test7)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata * administrative_metadata2 = NULL;

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
}
END_TEST


// Test function afs_administrative_metadata_equal with afs_administrative_metadata instance with empty strings and empty afs_administrative_metadata instance
BOXING_START_TEST(afs_administrative_metadata_equal_test8)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("", "", "", "", "", "");
    test_strings_afs_administrative_metadata(administrative_metadata1, "", "", "", "", "", "");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create();
    test_empty_afs_administrative_metadata(administrative_metadata2);

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with afs_administrative_metadata instance with not empty strings and empty afs_administrative_metadata instance
BOXING_START_TEST(afs_administrative_metadata_equal_test9)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create();
    test_empty_afs_administrative_metadata(administrative_metadata2);

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with afs_administrative_metadata instance with not empty strings and afs_administrative_metadata instance with empty strings
BOXING_START_TEST(afs_administrative_metadata_equal_test10)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create2("", "", "", "", "", "");
    test_strings_afs_administrative_metadata(administrative_metadata2, "", "", "", "", "", "");

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with afs_administrative_metadata instances with different reel_id strings
BOXING_START_TEST(afs_administrative_metadata_equal_test11)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id 1", "print reel id", "title", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id 1", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create2("reel id 2", "print reel id", "title", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata2, "reel id 2", "print reel id", "title", "description", "creator", "creation date");

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with afs_administrative_metadata instances with different print_reel_id strings
BOXING_START_TEST(afs_administrative_metadata_equal_test12)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id 1", "title", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id 1", "title", "description", "creator", "creation date");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create2("reel id", "print reel id 2", "title", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata2, "reel id", "print reel id 2", "title", "description", "creator", "creation date");

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with afs_administrative_metadata instances with different title strings
BOXING_START_TEST(afs_administrative_metadata_equal_test13)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id", "title 1", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id", "title 1", "description", "creator", "creation date");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create2("reel id", "print reel id", "title 2", "description", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata2, "reel id", "print reel id", "title 2", "description", "creator", "creation date");

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with afs_administrative_metadata instances with different description strings
BOXING_START_TEST(afs_administrative_metadata_equal_test14)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description 1", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id", "title", "description 1", "creator", "creation date");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description 2", "creator", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata2, "reel id", "print reel id", "title", "description 2", "creator", "creation date");

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with afs_administrative_metadata instances with different creator strings
BOXING_START_TEST(afs_administrative_metadata_equal_test15)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator 1", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id", "title", "description", "creator 1", "creation date");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator 2", "creation date");
    test_strings_afs_administrative_metadata(administrative_metadata2, "reel id", "print reel id", "title", "description", "creator 2", "creation date");

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_equal with afs_administrative_metadata instances with different creation_date strings
BOXING_START_TEST(afs_administrative_metadata_equal_test16)
{
    afs_administrative_metadata * administrative_metadata1 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date 1");
    test_strings_afs_administrative_metadata(administrative_metadata1, "reel id", "print reel id", "title", "description", "creator", "creation date 1");

    afs_administrative_metadata * administrative_metadata2 = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date 2");
    test_strings_afs_administrative_metadata(administrative_metadata2, "reel id", "print reel id", "title", "description", "creator", "creation date 2");

    test_equals_administrative_metadata(administrative_metadata2, administrative_metadata1, DFALSE);

    afs_administrative_metadata_free(administrative_metadata1);
    afs_administrative_metadata_free(administrative_metadata2);
}
END_TEST


// Test function afs_administrative_metadata_set_reel_id with input afs_administrative_metadata pointers equal to NULL and reel id string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_reel_id_test1)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * reel_id = NULL;

    afs_administrative_metadata_set_reel_id(administrative_metadata, reel_id);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(reel_id == NULL);
}
END_TEST


// Test function afs_administrative_metadata_set_reel_id with input afs_administrative_metadata pointers equal to NULL and empty reel id string
BOXING_START_TEST(afs_administrative_metadata_set_reel_id_test2)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * reel_id = "";

    afs_administrative_metadata_set_reel_id(administrative_metadata, reel_id);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(reel_id, "") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_reel_id with input afs_administrative_metadata pointers equal to NULL and not empty reel id string
BOXING_START_TEST(afs_administrative_metadata_set_reel_id_test3)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * reel_id = "some reel id";

    afs_administrative_metadata_set_reel_id(administrative_metadata, reel_id);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(reel_id, "some reel id") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_reel_id with empty input afs_administrative_metadata instance and reel id string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_reel_id_test4)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    const char * reel_id = NULL;

    afs_administrative_metadata_set_reel_id(administrative_metadata, reel_id);

    BOXING_ASSERT(administrative_metadata->reel_id == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_reel_id with empty input afs_administrative_metadata instance and empty reel id string
BOXING_START_TEST(afs_administrative_metadata_set_reel_id_test5)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->reel_id == NULL);

    const char * reel_id = "";

    afs_administrative_metadata_set_reel_id(administrative_metadata, reel_id);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->reel_id, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_reel_id with empty input afs_administrative_metadata instance and not empty reel id string
BOXING_START_TEST(afs_administrative_metadata_set_reel_id_test6)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->reel_id == NULL);

    const char * reel_id = "some reel id";

    afs_administrative_metadata_set_reel_id(administrative_metadata, reel_id);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->reel_id, "some reel id") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_reel_id with not empty input afs_administrative_metadata instance and reel id string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_reel_id_test7)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->reel_id, "reel id") == DTRUE);

    const char * reel_id = NULL;

    afs_administrative_metadata_set_reel_id(administrative_metadata, reel_id);

    BOXING_ASSERT(administrative_metadata->reel_id == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_reel_id with not empty input afs_administrative_metadata instance and empty reel id string
BOXING_START_TEST(afs_administrative_metadata_set_reel_id_test8)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->reel_id, "reel id") == DTRUE);

    const char * reel_id = "";

    afs_administrative_metadata_set_reel_id(administrative_metadata, reel_id);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->reel_id, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_reel_id with not empty input afs_administrative_metadata instance and not empty reel id string
BOXING_START_TEST(afs_administrative_metadata_set_reel_id_test9)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->reel_id, "reel id") == DTRUE);

    const char * reel_id = "some other reel id";

    afs_administrative_metadata_set_reel_id(administrative_metadata, reel_id);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->reel_id, "some other reel id") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_print_reel_id with input afs_administrative_metadata pointers equal to NULL and print reel id string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_print_reel_id_test1)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * print_reel_id = NULL;

    afs_administrative_metadata_set_print_reel_id(administrative_metadata, print_reel_id);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(print_reel_id == NULL);
}
END_TEST


// Test function afs_administrative_metadata_set_print_reel_id with input afs_administrative_metadata pointers equal to NULL and empty print reel id string
BOXING_START_TEST(afs_administrative_metadata_set_print_reel_id_test2)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * print_reel_id = "";

    afs_administrative_metadata_set_print_reel_id(administrative_metadata, print_reel_id);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(print_reel_id, "") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_print_reel_id with input afs_administrative_metadata pointers equal to NULL and not empty print reel id string
BOXING_START_TEST(afs_administrative_metadata_set_print_reel_id_test3)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * print_reel_id = "some print reel id";

    afs_administrative_metadata_set_print_reel_id(administrative_metadata, print_reel_id);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(print_reel_id, "some print reel id") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_print_reel_id with empty input afs_administrative_metadata instance and print reel id string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_print_reel_id_test4)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    const char * print_reel_id = NULL;

    afs_administrative_metadata_set_print_reel_id(administrative_metadata, print_reel_id);

    BOXING_ASSERT(administrative_metadata->print_reel_id == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_print_reel_id with empty input afs_administrative_metadata instance and empty print reel id string
BOXING_START_TEST(afs_administrative_metadata_set_print_reel_id_test5)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->print_reel_id == NULL);

    const char * print_reel_id = "";

    afs_administrative_metadata_set_print_reel_id(administrative_metadata, print_reel_id);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->print_reel_id, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_print_reel_id with empty input afs_administrative_metadata instance and not empty print reel id string
BOXING_START_TEST(afs_administrative_metadata_set_print_reel_id_test6)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->print_reel_id == NULL);

    const char * print_reel_id = "some print reel id";

    afs_administrative_metadata_set_print_reel_id(administrative_metadata, print_reel_id);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->print_reel_id, "some print reel id") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_print_reel_id with not empty input afs_administrative_metadata instance and print reel id string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_print_reel_id_test7)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->print_reel_id, "print reel id") == DTRUE);

    const char * print_reel_id = NULL;

    afs_administrative_metadata_set_print_reel_id(administrative_metadata, print_reel_id);

    BOXING_ASSERT(administrative_metadata->print_reel_id == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_print_reel_id with not empty input afs_administrative_metadata instance and empty print reel id string
BOXING_START_TEST(afs_administrative_metadata_set_print_reel_id_test8)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->print_reel_id, "print reel id") == DTRUE);

    const char * print_reel_id = "";

    afs_administrative_metadata_set_print_reel_id(administrative_metadata, print_reel_id);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->print_reel_id, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_print_reel_id with not empty input afs_administrative_metadata instance and not empty print reel id string
BOXING_START_TEST(afs_administrative_metadata_set_print_reel_id_test9)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->print_reel_id, "print reel id") == DTRUE);

    const char * print_reel_id = "some other print reel id";

    afs_administrative_metadata_set_print_reel_id(administrative_metadata, print_reel_id);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->print_reel_id, "some other print reel id") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_title with input afs_administrative_metadata pointers equal to NULL and title string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_title_test1)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * title = NULL;

    afs_administrative_metadata_set_title(administrative_metadata, title);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(title == NULL);
}
END_TEST


// Test function afs_administrative_metadata_set_title with input afs_administrative_metadata pointers equal to NULL and empty title string
BOXING_START_TEST(afs_administrative_metadata_set_title_test2)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * title = "";

    afs_administrative_metadata_set_title(administrative_metadata, title);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(title, "") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_title with input afs_administrative_metadata pointers equal to NULL and not empty title string
BOXING_START_TEST(afs_administrative_metadata_set_title_test3)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * title = "some title";

    afs_administrative_metadata_set_title(administrative_metadata, title);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(title, "some title") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_title with empty input afs_administrative_metadata instance and title string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_title_test4)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    const char * title = NULL;

    afs_administrative_metadata_set_title(administrative_metadata, title);

    BOXING_ASSERT(administrative_metadata->title == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_title with empty input afs_administrative_metadata instance and empty title string
BOXING_START_TEST(afs_administrative_metadata_set_title_test5)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->title == NULL);

    const char * title = "";

    afs_administrative_metadata_set_title(administrative_metadata, title);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->title, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_title with empty input afs_administrative_metadata instance and not empty title string
BOXING_START_TEST(afs_administrative_metadata_set_title_test6)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->title == NULL);

    const char * title = "some title";

    afs_administrative_metadata_set_title(administrative_metadata, title);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->title, "some title") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_title with not empty input afs_administrative_metadata instance and title string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_title_test7)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->title, "title") == DTRUE);

    const char * title = NULL;

    afs_administrative_metadata_set_title(administrative_metadata, title);

    BOXING_ASSERT(administrative_metadata->title == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_title with not empty input afs_administrative_metadata instance and empty title string
BOXING_START_TEST(afs_administrative_metadata_set_title_test8)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->title, "title") == DTRUE);

    const char * title = "";

    afs_administrative_metadata_set_title(administrative_metadata, title);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->title, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_title with not empty input afs_administrative_metadata instance and not empty title string
BOXING_START_TEST(afs_administrative_metadata_set_title_test9)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->title, "title") == DTRUE);

    const char * title = "some other title";

    afs_administrative_metadata_set_title(administrative_metadata, title);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->title, "some other title") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_description with input afs_administrative_metadata pointers equal to NULL and description string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_description_test1)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * description = NULL;

    afs_administrative_metadata_set_description(administrative_metadata, description);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(description == NULL);
}
END_TEST


// Test function afs_administrative_metadata_set_description with input afs_administrative_metadata pointers equal to NULL and empty description string
BOXING_START_TEST(afs_administrative_metadata_set_description_test2)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * description = "";

    afs_administrative_metadata_set_description(administrative_metadata, description);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(description, "") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_description with input afs_administrative_metadata pointers equal to NULL and not empty description string
BOXING_START_TEST(afs_administrative_metadata_set_description_test3)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * description = "some description";

    afs_administrative_metadata_set_description(administrative_metadata, description);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(description, "some description") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_description with empty input afs_administrative_metadata instance and description string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_description_test4)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    const char * description = NULL;

    afs_administrative_metadata_set_description(administrative_metadata, description);

    BOXING_ASSERT(administrative_metadata->description == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_description with empty input afs_administrative_metadata instance and empty description string
BOXING_START_TEST(afs_administrative_metadata_set_description_test5)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->description == NULL);

    const char * description = "";

    afs_administrative_metadata_set_description(administrative_metadata, description);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->description, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_description with empty input afs_administrative_metadata instance and not empty description string
BOXING_START_TEST(afs_administrative_metadata_set_description_test6)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->description == NULL);

    const char * description = "some description";

    afs_administrative_metadata_set_description(administrative_metadata, description);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->description, "some description") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_description with not empty input afs_administrative_metadata instance and description string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_description_test7)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->description, "description") == DTRUE);

    const char * description = NULL;

    afs_administrative_metadata_set_description(administrative_metadata, description);

    BOXING_ASSERT(administrative_metadata->description == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_description with not empty input afs_administrative_metadata instance and empty description string
BOXING_START_TEST(afs_administrative_metadata_set_description_test8)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->description, "description") == DTRUE);

    const char * description = "";

    afs_administrative_metadata_set_description(administrative_metadata, description);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->description, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_description with not empty input afs_administrative_metadata instance and not empty description string
BOXING_START_TEST(afs_administrative_metadata_set_description_test9)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->description, "description") == DTRUE);

    const char * description = "some other description";

    afs_administrative_metadata_set_description(administrative_metadata, description);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->description, "some other description") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creator with input afs_administrative_metadata pointers equal to NULL and creator string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_creator_test1)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * creator = NULL;

    afs_administrative_metadata_set_creator(administrative_metadata, creator);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(creator == NULL);
}
END_TEST


// Test function afs_administrative_metadata_set_creator with input afs_administrative_metadata pointers equal to NULL and empty creator string
BOXING_START_TEST(afs_administrative_metadata_set_creator_test2)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * creator = "";

    afs_administrative_metadata_set_creator(administrative_metadata, creator);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(creator, "") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_creator with input afs_administrative_metadata pointers equal to NULL and not empty creator string
BOXING_START_TEST(afs_administrative_metadata_set_creator_test3)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * creator = "some creator";

    afs_administrative_metadata_set_creator(administrative_metadata, creator);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(creator, "some creator") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_creator with empty input afs_administrative_metadata instance and creator string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_creator_test4)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    const char * creator = NULL;

    afs_administrative_metadata_set_creator(administrative_metadata, creator);

    BOXING_ASSERT(administrative_metadata->creator == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creator with empty input afs_administrative_metadata instance and empty creator string
BOXING_START_TEST(afs_administrative_metadata_set_creator_test5)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->creator == NULL);

    const char * creator = "";

    afs_administrative_metadata_set_creator(administrative_metadata, creator);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creator, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creator with empty input afs_administrative_metadata instance and not empty creator string
BOXING_START_TEST(afs_administrative_metadata_set_creator_test6)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->creator == NULL);

    const char * creator = "some creator";

    afs_administrative_metadata_set_creator(administrative_metadata, creator);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creator, "some creator") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creator with not empty input afs_administrative_metadata instance and creator string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_creator_test7)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creator, "creator") == DTRUE);

    const char * creator = NULL;

    afs_administrative_metadata_set_creator(administrative_metadata, creator);

    BOXING_ASSERT(administrative_metadata->creator == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creator with not empty input afs_administrative_metadata instance and empty creator string
BOXING_START_TEST(afs_administrative_metadata_set_creator_test8)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creator, "creator") == DTRUE);

    const char * creator = "";

    afs_administrative_metadata_set_creator(administrative_metadata, creator);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creator, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creator with not empty input afs_administrative_metadata instance and not empty creator string
BOXING_START_TEST(afs_administrative_metadata_set_creator_test9)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creator, "creator") == DTRUE);

    const char * creator = "some other creator";

    afs_administrative_metadata_set_creator(administrative_metadata, creator);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creator, "some other creator") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creation_date with input afs_administrative_metadata pointers equal to NULL and creation date string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_creation_date_test1)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * creation_date = NULL;

    afs_administrative_metadata_set_creation_date(administrative_metadata, creation_date);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(creation_date == NULL);
}
END_TEST


// Test function afs_administrative_metadata_set_creation_date with input afs_administrative_metadata pointers equal to NULL and empty creation date string
BOXING_START_TEST(afs_administrative_metadata_set_creation_date_test2)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * creation_date = "";

    afs_administrative_metadata_set_creation_date(administrative_metadata, creation_date);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(creation_date, "") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_creation_date with input afs_administrative_metadata pointers equal to NULL and not empty creation date string
BOXING_START_TEST(afs_administrative_metadata_set_creation_date_test3)
{
    afs_administrative_metadata * administrative_metadata = NULL;
    const char * creation_date = "some creation date";

    afs_administrative_metadata_set_creation_date(administrative_metadata, creation_date);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(boxing_string_equal(creation_date, "some creation date") == DTRUE);
}
END_TEST


// Test function afs_administrative_metadata_set_creation_date with empty input afs_administrative_metadata instance and creation date string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_creation_date_test4)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    const char * creation_date = NULL;

    afs_administrative_metadata_set_creation_date(administrative_metadata, creation_date);

    BOXING_ASSERT(administrative_metadata->creation_date == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creation_date with empty input afs_administrative_metadata instance and empty creation date string
BOXING_START_TEST(afs_administrative_metadata_set_creation_date_test5)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->creation_date == NULL);

    const char * creation_date = "";

    afs_administrative_metadata_set_creation_date(administrative_metadata, creation_date);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creation_date, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creation_date with empty input afs_administrative_metadata instance and not empty creation date string
BOXING_START_TEST(afs_administrative_metadata_set_creation_date_test6)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    BOXING_ASSERT(administrative_metadata->creation_date == NULL);

    const char * creation_date = "some creation date";

    afs_administrative_metadata_set_creation_date(administrative_metadata, creation_date);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creation_date, "some creation date") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creation_date with not empty input afs_administrative_metadata instance and creation date string pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_set_creation_date_test7)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creation_date, "creation date") == DTRUE);

    const char * creation_date = NULL;

    afs_administrative_metadata_set_creation_date(administrative_metadata, creation_date);

    BOXING_ASSERT(administrative_metadata->creation_date == NULL);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creation_date with not empty input afs_administrative_metadata instance and empty creation date string
BOXING_START_TEST(afs_administrative_metadata_set_creation_date_test8)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creation_date, "creation date") == DTRUE);

    const char * creation_date = "";

    afs_administrative_metadata_set_creation_date(administrative_metadata, creation_date);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creation_date, "") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_set_creation_date with not empty input afs_administrative_metadata instance and not empty creation date string
BOXING_START_TEST(afs_administrative_metadata_set_creation_date_test9)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creation_date, "creation date") == DTRUE);

    const char * creation_date = "some other creation date";

    afs_administrative_metadata_set_creation_date(administrative_metadata, creation_date);

    BOXING_ASSERT(boxing_string_equal(administrative_metadata->creation_date, "some other creation date") == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_save_file with file_name pointer equal to NULL and afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_save_file_test1)
{
    const char * file_name = NULL;
    afs_administrative_metadata * administrative_metadata = NULL;

    DBOOL result = afs_administrative_metadata_save_file(administrative_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_administrative_metadata_save_file with file_name pointer equal to NULL and afs_administrative_metadata pointer not equal to NULL
BOXING_START_TEST(afs_administrative_metadata_save_file_test2)
{
    const char * file_name = NULL;

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    DBOOL result = afs_administrative_metadata_save_file(administrative_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_save_file with file_name pointer not equal to NULL and afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_save_file_test3)
{
    const char * file_name = "afs_administrative_metadata_save_file_test.xml";
    afs_administrative_metadata * administrative_metadata = NULL;

    DBOOL result = afs_administrative_metadata_save_file(administrative_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_administrative_metadata_save_file with empty afs_administrative_metadata instance, in compact format
BOXING_START_TEST(afs_administrative_metadata_save_file_test4)
{
    const char * file_name = "afs_administrative_metadata_save_file_empty_compact_test.xml";
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_save_file(administrative_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_save_file with empty afs_administrative_metadata instance, in not compact format
BOXING_START_TEST(afs_administrative_metadata_save_file_test5)
{
    const char * file_name = "afs_administrative_metadata_save_file_empty_not_compact_test.xml";
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_save_file(administrative_metadata, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_save_file with afs_administrative_metadata instance with empty strings, in compact format
BOXING_START_TEST(afs_administrative_metadata_save_file_test6)
{
    const char * file_name = "afs_administrative_metadata_save_file_empty_strings_compact_test.xml";
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("", "", "", "", "", "");

    DBOOL result = afs_administrative_metadata_save_file(administrative_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_save_file with afs_administrative_metadata instance with empty strings, in not compact format
BOXING_START_TEST(afs_administrative_metadata_save_file_test7)
{
    const char * file_name = "afs_administrative_metadata_save_file_empty_strings_not_compact_test.xml";
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("", "", "", "", "", "");

    DBOOL result = afs_administrative_metadata_save_file(administrative_metadata, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_save_file with afs_administrative_metadata instance with not empty strings, in compact format
BOXING_START_TEST(afs_administrative_metadata_save_file_test8)
{
    const char * file_name = "afs_administrative_metadata_save_file_compact_test.xml";

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    DBOOL result = afs_administrative_metadata_save_file(administrative_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_save_file with afs_administrative_metadata instance with not empty strings, in not compact format
BOXING_START_TEST(afs_administrative_metadata_save_file_test9)
{
    const char * file_name = "afs_administrative_metadata_save_file_not_compact_test.xml";

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    DBOOL result = afs_administrative_metadata_save_file(administrative_metadata, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_save_string with afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_save_string_test1)
{
    afs_administrative_metadata * administrative_metadata = NULL;

    char * result = afs_administrative_metadata_save_string(administrative_metadata, DTRUE);

    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_administrative_metadata_save_string with empty afs_administrative_metadata instance, in compact format
BOXING_START_TEST(afs_administrative_metadata_save_string_test2)
{
    char * test_string = read_xml_control_data("afs_administrative_metadata_save_file_empty_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    char * result = afs_administrative_metadata_save_string(administrative_metadata, DTRUE);

    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_administrative_metadata_save_string with empty afs_administrative_metadata instance, in not compact format
BOXING_START_TEST(afs_administrative_metadata_save_string_test3)
{
    char * test_string = read_xml_control_data("afs_administrative_metadata_save_file_empty_not_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    char * result = afs_administrative_metadata_save_string(administrative_metadata, DFALSE);

    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_administrative_metadata_save_string with afs_administrative_metadata instance with empty strings, in compact format
BOXING_START_TEST(afs_administrative_metadata_save_string_test4)
{
    char * test_string = read_xml_control_data("afs_administrative_metadata_save_file_empty_strings_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("", "", "", "", "", "");

    char * result = afs_administrative_metadata_save_string(administrative_metadata, DTRUE);

    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_administrative_metadata_save_string with afs_administrative_metadata instance with empty strings, in not compact format
BOXING_START_TEST(afs_administrative_metadata_save_string_test5)
{
    char * test_string = read_xml_control_data("afs_administrative_metadata_save_file_empty_strings_not_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("", "", "", "", "", "");

    char * result = afs_administrative_metadata_save_string(administrative_metadata, DFALSE);

    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_administrative_metadata_save_string with afs_administrative_metadata instance with not empty strings, in compact format
BOXING_START_TEST(afs_administrative_metadata_save_string_test6)
{
    char * test_string = read_xml_control_data("afs_administrative_metadata_save_file_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    char * result = afs_administrative_metadata_save_string(administrative_metadata, DTRUE);

    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_administrative_metadata_save_string with afs_administrative_metadata instance with not empty strings, in not compact format
BOXING_START_TEST(afs_administrative_metadata_save_string_test7)
{
    char * test_string = read_xml_control_data("afs_administrative_metadata_save_file_not_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    char * result = afs_administrative_metadata_save_string(administrative_metadata, DFALSE);

    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_administrative_metadata_save_xml with mxml_node_t pointer equal to NULL and afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_save_xml_test1)
{
    afs_administrative_metadata * administrative_metadata = NULL;

    mxml_node_t * tree = NULL;

    DBOOL return_value = afs_administrative_metadata_save_xml(administrative_metadata, tree);

    BOXING_ASSERT(return_value == DFALSE);
}
END_TEST


// Test function afs_administrative_metadata_save_xml with mxml_node_t pointer equal to NULL and afs_administrative_metadata pointer not equal to NULL
BOXING_START_TEST(afs_administrative_metadata_save_xml_test2)
{
    afs_administrative_metadata* administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    mxml_node_t * tree = NULL;

    DBOOL return_value = afs_administrative_metadata_save_xml(administrative_metadata, tree);

    BOXING_ASSERT(return_value == DFALSE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_save_xml with afs_administrative_metadata pointer equal to NULL and mxml_node_t pointer not equal to NULL
BOXING_START_TEST(afs_administrative_metadata_save_xml_test3)
{
    afs_administrative_metadata * administrative_metadata = NULL;

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_administrative_metadata_save_xml(administrative_metadata, tree);

    BOXING_ASSERT(return_value == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_save_xml with empty strings
BOXING_START_TEST(afs_administrative_metadata_save_xml_test4)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("", "", "", "", "", "");

    mxml_node_t *tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_administrative_metadata_save_xml(administrative_metadata, tree);

    BOXING_ASSERT(return_value == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_save_xml with initialized strings
BOXING_START_TEST(afs_administrative_metadata_save_xml_test5)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    mxml_node_t *tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_administrative_metadata_save_xml(administrative_metadata, tree);

    BOXING_ASSERT(return_value == DTRUE);

    afs_administrative_metadata_free(administrative_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_load_file with file_name pointer equal to NULL and afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_load_file_test1)
{
    const char * file_name = NULL;
    afs_administrative_metadata * administrative_metadata = NULL;

    DBOOL result = afs_administrative_metadata_load_file(administrative_metadata, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_administrative_metadata_load_file with file_name pointer equal to NULL and afs_administrative_metadata pointer not equal to NULL
BOXING_START_TEST(afs_administrative_metadata_load_file_test2)
{
    const char * file_name = NULL;

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_file(administrative_metadata, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(administrative_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_file with file_name pointer not equal to NULL and afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_load_file_test3)
{
    const char * file_name = "afs_administrative_metadata_save_file_compact_test.xml";
    afs_administrative_metadata * administrative_metadata = NULL;

    DBOOL result = afs_administrative_metadata_load_file(administrative_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_administrative_metadata_load_file when XML file does not exist
BOXING_START_TEST(afs_administrative_metadata_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_file(administrative_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_file for empty afs_administrative_metadata instance, in compact format
BOXING_START_TEST(afs_administrative_metadata_load_file_test5)
{
    const char * file_name = "afs_administrative_metadata_save_file_empty_compact_test.xml";

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_file(administrative_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_file for empty afs_administrative_metadata instance, in not compact format
BOXING_START_TEST(afs_administrative_metadata_load_file_test6)
{
    const char * file_name = "afs_administrative_metadata_save_file_empty_not_compact_test.xml";

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_file(administrative_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_file for afs_administrative_metadata instance with empty strings, in compact format
BOXING_START_TEST(afs_administrative_metadata_load_file_test7)
{
    const char * file_name = "afs_administrative_metadata_save_file_empty_strings_compact_test.xml";

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_file(administrative_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "", "", "", "", "", "");

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_file for afs_administrative_metadata instance with empty strings, in not compact format
BOXING_START_TEST(afs_administrative_metadata_load_file_test8)
{
    const char * file_name = "afs_administrative_metadata_save_file_empty_strings_not_compact_test.xml";

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_file(administrative_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "", "", "", "", "", "");

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_file for afs_administrative_metadata instance with not empty strings, in compact format
BOXING_START_TEST(afs_administrative_metadata_load_file_test9)
{
    const char * file_name = "afs_administrative_metadata_save_file_compact_test.xml";

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_file(administrative_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_file for afs_administrative_metadata instance with not empty strings, in not compact format
BOXING_START_TEST(afs_administrative_metadata_load_file_test10)
{
    const char * file_name = "afs_administrative_metadata_save_file_not_compact_test.xml";

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_file(administrative_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_string with input string pointer equal to NULL and afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_load_string_test1)
{
    const char * input_string = NULL;
    afs_administrative_metadata * administrative_metadata = NULL;

    DBOOL result = afs_administrative_metadata_load_string(administrative_metadata, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_administrative_metadata_load_string with input string pointer equal to NULL and afs_administrative_metadata pointer not equal to NULL
BOXING_START_TEST(afs_administrative_metadata_load_string_test2)
{
    const char * input_string = NULL;

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_string(administrative_metadata, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_string with input string pointer not equal to NULL and afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_load_string_test3)
{
    char * input_string = read_xml_control_data("afs_administrative_metadata_save_file_compact_test.xml");
    afs_administrative_metadata * administrative_metadata = NULL;

    DBOOL result = afs_administrative_metadata_load_string(administrative_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);

    boxing_string_free(input_string);
}
END_TEST


// Test function afs_administrative_metadata_load_string with empty input string
BOXING_START_TEST(afs_administrative_metadata_load_string_test4)
{
    char * input_string = "";
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_string(administrative_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_string with input string for empty afs_administrative_metadata instance, in compact format
BOXING_START_TEST(afs_administrative_metadata_load_string_test5)
{
    char * input_string = read_xml_control_data("afs_administrative_metadata_save_file_empty_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_string(administrative_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_administrative_metadata_load_string with input string for empty afs_administrative_metadata instance, in not compact format
BOXING_START_TEST(afs_administrative_metadata_load_string_test6)
{
    char * input_string = read_xml_control_data("afs_administrative_metadata_save_file_empty_not_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_string(administrative_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_administrative_metadata_load_string with input string for afs_administrative_metadata instance with empty strings, in compact format
BOXING_START_TEST(afs_administrative_metadata_load_string_test7)
{
    char * input_string = read_xml_control_data("afs_administrative_metadata_save_file_empty_strings_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_string(administrative_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "", "", "", "", "", "");

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_administrative_metadata_load_string with input string for afs_administrative_metadata instance with empty strings, in not compact format
BOXING_START_TEST(afs_administrative_metadata_load_string_test8)
{
    char * input_string = read_xml_control_data("afs_administrative_metadata_save_file_empty_strings_not_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_string(administrative_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "", "", "", "", "", "");

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_administrative_metadata_load_string with input string for afs_administrative_metadata instance with not empty strings, in compact format
BOXING_START_TEST(afs_administrative_metadata_load_string_test9)
{
    char * input_string = read_xml_control_data("afs_administrative_metadata_save_file_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_string(administrative_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_administrative_metadata_load_string with input string for afs_administrative_metadata instance with not empty strings, in not compact format
BOXING_START_TEST(afs_administrative_metadata_load_string_test10)
{
    char * input_string = read_xml_control_data("afs_administrative_metadata_save_file_not_compact_test.xml");

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_string(administrative_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata_free(administrative_metadata);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_administrative_metadata_load_xml with xml instance pointer equal to NULL and afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_administrative_metadata * administrative_metadata = NULL;

    DBOOL result = afs_administrative_metadata_load_xml(administrative_metadata, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_administrative_metadata_load_xml with xml instance pointer equal to NULL and afs_administrative_metadata pointer not equal to NULL
BOXING_START_TEST(afs_administrative_metadata_load_xml_test2)
{
    mxml_node_t * tree = NULL;

    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_xml(administrative_metadata, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
}
END_TEST


// Test function afs_administrative_metadata_load_xml with xml instance pointer not equal to NULL and afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_administrative_metadata_load_xml_test3)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_administrative_metadata_save_file_compact_test.xml");
    afs_administrative_metadata * administrative_metadata = NULL;

    DBOOL result = afs_administrative_metadata_load_xml(administrative_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(administrative_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_load_xml with empty xml instance
BOXING_START_TEST(afs_administrative_metadata_load_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_xml(administrative_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_load_xml with xml instance for empty afs_toc_data_reel instance, in compact format
BOXING_START_TEST(afs_administrative_metadata_load_xml_test5)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_administrative_metadata_save_file_empty_compact_test.xml");
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_xml(administrative_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_load_xml with xml instance for empty afs_toc_data_reel instance, in not compact format
BOXING_START_TEST(afs_administrative_metadata_load_xml_test6)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_administrative_metadata_save_file_empty_not_compact_test.xml");
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_xml(administrative_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_administrative_metadata(administrative_metadata);

    afs_administrative_metadata_free(administrative_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_load_xml with xml instance for afs_toc_data_reel instance with empty strings, in compact format
BOXING_START_TEST(afs_administrative_metadata_load_xml_test7)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_administrative_metadata_save_file_empty_strings_compact_test.xml");
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_xml(administrative_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "", "", "", "", "", "");

    afs_administrative_metadata_free(administrative_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_load_xml with xml instance for afs_toc_data_reel instance with empty strings, in not compact format
BOXING_START_TEST(afs_administrative_metadata_load_xml_test8)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_administrative_metadata_save_file_empty_strings_not_compact_test.xml");
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_xml(administrative_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "", "", "", "", "", "");

    afs_administrative_metadata_free(administrative_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_load_xml with xml instance for afs_toc_data_reel instance with not empty strings, in compact format
BOXING_START_TEST(afs_administrative_metadata_load_xml_test9)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_administrative_metadata_save_file_compact_test.xml");
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_xml(administrative_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata_free(administrative_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_load_xml with xml instance for afs_toc_data_reel instance with not empty strings, in not compact format
BOXING_START_TEST(afs_administrative_metadata_load_xml_test10)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_administrative_metadata_save_file_not_compact_test.xml");
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    DBOOL result = afs_administrative_metadata_load_xml(administrative_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_strings_afs_administrative_metadata(administrative_metadata, "reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_administrative_metadata_free(administrative_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_administrative_metadata_save_xml and afs_administrative_metadata_load_xml with NULL strings values
BOXING_START_TEST(afs_administrative_metadata_save_load_xml_test1)
{
    afs_administrative_metadata* administrative_metadata_save = afs_administrative_metadata_create();
    
    mxml_node_t *tree = mxmlNewXML("1.0");

#ifndef WIN32
    FILE * fp_save = fopen("afs_controldata_test1.xml", "w+");
#else
    FILE * fp_save = fopen("afs_controldata_test1.xml", "w+b");
#endif

    DBOOL return_value = afs_administrative_metadata_save_xml(administrative_metadata_save, tree);

    BOXING_ASSERT(return_value == DTRUE);

    mxmlSaveFile(tree, fp_save, MXML_NO_CALLBACK);
    fclose(fp_save);

    afs_administrative_metadata_free(administrative_metadata_save);


    afs_administrative_metadata* administrative_metadata_load = afs_administrative_metadata_create();

#ifndef WIN32
    FILE * fp_load = fopen("afs_controldata_test1.xml", "r");
#else
    FILE * fp_load = fopen("afs_controldata_test1.xml", "rb");
#endif

    mxml_node_t * document = mxmlLoadFile(NULL, fp_load, MXML_OPAQUE_CALLBACK);

    return_value = afs_administrative_metadata_load_xml(administrative_metadata_load, document);

    BOXING_ASSERT(return_value == DTRUE);
    test_empty_afs_administrative_metadata(administrative_metadata_load);

    fclose(fp_load);

    afs_administrative_metadata_free(administrative_metadata_load);
    mxmlDelete(tree);
    mxmlDelete(document);
}
END_TEST


// Test functions afs_administrative_metadata_save_xml and afs_administrative_metadata_load_xml with empty strings values
BOXING_START_TEST(afs_administrative_metadata_save_load_xml_test2)
{
    afs_administrative_metadata* administrative_metadata_save = afs_administrative_metadata_create2("", "", "", "", "", "");

    mxml_node_t *tree = mxmlNewXML("1.0");

#ifndef WIN32
    FILE * fp_save = fopen("afs_controldata_test1.xml", "w+");
#else
    FILE * fp_save = fopen("afs_controldata_test1.xml", "w+b");
#endif

    DBOOL return_value = afs_administrative_metadata_save_xml(administrative_metadata_save, tree);

    BOXING_ASSERT(return_value == DTRUE);

    mxmlSaveFile(tree, fp_save, MXML_NO_CALLBACK);
    fclose(fp_save);

    afs_administrative_metadata_free(administrative_metadata_save);


    afs_administrative_metadata* administrative_metadata_load = afs_administrative_metadata_create();

#ifndef WIN32
    FILE * fp_load = fopen("afs_controldata_test1.xml", "r");
#else
    FILE * fp_load = fopen("afs_controldata_test1.xml", "rb");
#endif

    mxml_node_t * document = mxmlLoadFile(NULL, fp_load, MXML_OPAQUE_CALLBACK);

    return_value = afs_administrative_metadata_load_xml(administrative_metadata_load, document);

    BOXING_ASSERT(return_value == DTRUE);
    test_strings_afs_administrative_metadata(administrative_metadata_load, "", "", "", "", "", "");

    fclose(fp_load);

    afs_administrative_metadata_free(administrative_metadata_load);
    mxmlDelete(tree);
    mxmlDelete(document);
}
END_TEST


// TEST TECHNICAL METADATA FUNCTIONS
//


// Test function afs_technical_metadata_create test
BOXING_START_TEST(afs_technical_metadata_create_test1)
{
    afs_technical_metadata* technical_metadata = afs_technical_metadata_create();

    test_empty_afs_technical_metadata(technical_metadata);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_create2 with input pointers equal to NULL
BOXING_START_TEST(afs_technical_metadata_create2_test1)
{
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create2(NULL, NULL);

    test_empty_afs_technical_metadata(technical_metadata);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_create2 with afs_toc_files pointer equal to NULL and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_create2_test2)
{
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create2(NULL, boxing_format);

    test_afs_technical_metadata(technical_metadata, 0, -1);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_create2 with afs_toc_files pointer equal to NULL and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_create2_test3)
{
    afs_boxing_format * boxing_format = create_boxing_format_instance();

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create2(NULL, boxing_format);

    test_afs_technical_metadata(technical_metadata, 1, -1);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_create2 with empty afs_toc_files instance and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_create2_test4)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create2(toc_files, NULL);

    test_afs_technical_metadata(technical_metadata, -1, 0);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_create2 with empty afs_toc_files instance and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_create2_test5)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata, 0, 0);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_create2 with empty afs_toc_files instance and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_create2_test6)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_boxing_format * boxing_format = create_boxing_format_instance();

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata, 1, 0);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_create2 with not empty afs_toc_files instance and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_create2_test7)
{
    afs_toc_files * toc_files = create_toc_files_instance(3);

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create2(toc_files, NULL);

    test_afs_technical_metadata(technical_metadata, -1, 3);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_create2 with not empty afs_toc_files instance and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_create2_test8)
{
    afs_toc_files * toc_files = create_toc_files_instance(3);
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata, 0, 3);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_create2 with not empty afs_toc_files instance and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_create2_test9)
{
    afs_toc_files * toc_files = create_toc_files_instance(3);
    afs_boxing_format * boxing_format = create_boxing_format_instance();

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata, 1, 3);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_clone with input pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_clone_test1)
{
    afs_technical_metadata * technical_metadata1 = NULL;

    BOXING_ASSERT(technical_metadata1 == NULL);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_clone(technical_metadata1);

    BOXING_ASSERT(technical_metadata2 == NULL);
}
END_TEST


// Test function afs_technical_metadata_clone with empty input afs_technical_metadata instance
BOXING_START_TEST(afs_technical_metadata_clone_test2)
{
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create();

    test_empty_afs_technical_metadata(technical_metadata1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_clone(technical_metadata1);

    test_empty_afs_technical_metadata(technical_metadata2);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_clone with afs_toc_files pointer equal to NULL and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_clone_test3)
{
    afs_boxing_format * boxing_format = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(NULL, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 0, -1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_clone(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 0, -1);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_clone with afs_toc_files pointer equal to NULL and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_clone_test4)
{
    afs_boxing_format * boxing_format = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(NULL, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 1, -1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_clone(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 1, -1);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_clone with empty afs_toc_files instance and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_clone_test5)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, NULL);

    test_afs_technical_metadata(technical_metadata1, -1, 0);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_clone(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, -1, 0);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_clone with empty afs_toc_files instance and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_clone_test6)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_boxing_format * boxing_format = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 0, 0);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_clone(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 0, 0);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_clone with empty afs_toc_files instance and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_clone_test7)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_boxing_format * boxing_format = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 1, 0);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_clone(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 1, 0);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_clone with not empty afs_toc_files instance and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_clone_test8)
{
    afs_toc_files * toc_files = create_toc_files_instance(3);
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, NULL);

    test_afs_technical_metadata(technical_metadata1, -1, 3);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_clone(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, -1, 3);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_clone with not empty afs_toc_files instance and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_clone_test9)
{
    afs_toc_files * toc_files = create_toc_files_instance(3);
    afs_boxing_format * boxing_format = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 0, 3);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_clone(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 0, 3);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_clone with not empty afs_toc_files instance and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_clone_test10)
{
    afs_toc_files * toc_files = create_toc_files_instance(3);
    afs_boxing_format * boxing_format = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 1, 3);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_clone(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 1, 3);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_get_new_reference_test1)
{
    afs_technical_metadata * technical_metadata1 = NULL;

    BOXING_ASSERT(technical_metadata1 == NULL);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_get_new_reference(technical_metadata1);

    BOXING_ASSERT(technical_metadata2 == NULL);

    technical_metadata1 = afs_technical_metadata_create();

    test_empty_afs_technical_metadata(technical_metadata1);
    BOXING_ASSERT(technical_metadata1->reference_count == 1);
    BOXING_ASSERT(technical_metadata2 == NULL);

    afs_technical_metadata_free(technical_metadata1);
}
END_TEST


// Test function afs_technical_metadata_get_new_reference with empty input afs_technical_metadata instance
BOXING_START_TEST(afs_technical_metadata_get_new_reference_test2)
{
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create();

    test_empty_afs_technical_metadata(technical_metadata1);
    BOXING_ASSERT(technical_metadata1->reference_count == 1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_get_new_reference(technical_metadata1);

    test_empty_afs_technical_metadata(technical_metadata2);
    BOXING_ASSERT(technical_metadata1->reference_count == 2);
    BOXING_ASSERT(technical_metadata2->reference_count == 2);
    BOXING_ASSERT(technical_metadata1 == technical_metadata2);

    technical_metadata1->afs_content_boxing_format = afs_boxing_format_create();
    technical_metadata1->afs_content_boxing_format->bytes_per_frame = 99;

    BOXING_ASSERT(technical_metadata2->afs_content_boxing_format->bytes_per_frame == 99);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_get_new_reference with afs_toc_files pointer equal to NULL and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_get_new_reference_test3)
{
    afs_boxing_format * boxing_format = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(NULL, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 0, -1);
    BOXING_ASSERT(technical_metadata1->reference_count == 1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_get_new_reference(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 0, -1);
    BOXING_ASSERT(technical_metadata1->reference_count == 2);
    BOXING_ASSERT(technical_metadata2->reference_count == 2);
    BOXING_ASSERT(technical_metadata1 == technical_metadata2);

    technical_metadata1->afs_content_boxing_format->bytes_per_frame = 99;

    BOXING_ASSERT(technical_metadata2->afs_content_boxing_format->bytes_per_frame == 99);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_get_new_reference with afs_toc_files pointer equal to NULL and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_get_new_reference_test4)
{
    afs_boxing_format * boxing_format = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(NULL, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 1, -1);
    BOXING_ASSERT(technical_metadata1->reference_count == 1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_get_new_reference(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 1, -1);
    BOXING_ASSERT(technical_metadata1->reference_count == 2);
    BOXING_ASSERT(technical_metadata2->reference_count == 2);
    BOXING_ASSERT(technical_metadata1 == technical_metadata2);

    technical_metadata1->afs_content_boxing_format->bytes_per_frame = 99;

    BOXING_ASSERT(technical_metadata2->afs_content_boxing_format->bytes_per_frame == 99);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_get_new_reference with empty afs_toc_files instance and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_get_new_reference_test5)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, NULL);

    test_afs_technical_metadata(technical_metadata1, -1, 0);
    BOXING_ASSERT(technical_metadata1->reference_count == 1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_get_new_reference(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, -1, 0);
    BOXING_ASSERT(technical_metadata1->reference_count == 2);
    BOXING_ASSERT(technical_metadata2->reference_count == 2);
    BOXING_ASSERT(technical_metadata1 == technical_metadata2);

    technical_metadata1->afs_content_boxing_format = afs_boxing_format_create();
    technical_metadata1->afs_content_boxing_format->bytes_per_frame = 99;

    BOXING_ASSERT(technical_metadata2->afs_content_boxing_format->bytes_per_frame == 99);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_get_new_reference with empty afs_toc_files instance and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_get_new_reference_test6)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_boxing_format * boxing_format = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 0, 0);
    BOXING_ASSERT(technical_metadata1->reference_count == 1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_get_new_reference(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 0, 0);
    BOXING_ASSERT(technical_metadata1->reference_count == 2);
    BOXING_ASSERT(technical_metadata2->reference_count == 2);
    BOXING_ASSERT(technical_metadata1 == technical_metadata2);

    technical_metadata1->afs_content_boxing_format->bytes_per_frame = 99;

    BOXING_ASSERT(technical_metadata2->afs_content_boxing_format->bytes_per_frame == 99);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_get_new_reference with empty afs_toc_files instance and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_get_new_reference_test7)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_boxing_format * boxing_format = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 1, 0);
    BOXING_ASSERT(technical_metadata1->reference_count == 1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_get_new_reference(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 1, 0);
    BOXING_ASSERT(technical_metadata1->reference_count == 2);
    BOXING_ASSERT(technical_metadata2->reference_count == 2);
    BOXING_ASSERT(technical_metadata1 == technical_metadata2);

    technical_metadata1->afs_content_boxing_format->bytes_per_frame = 99;

    BOXING_ASSERT(technical_metadata2->afs_content_boxing_format->bytes_per_frame == 99);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_get_new_reference with not empty afs_toc_files instance and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_get_new_reference_test8)
{
    afs_toc_files * toc_files = create_toc_files_instance(3);
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, NULL);

    test_afs_technical_metadata(technical_metadata1, -1, 3);
    BOXING_ASSERT(technical_metadata1->reference_count == 1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_get_new_reference(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, -1, 3);
    BOXING_ASSERT(technical_metadata1->reference_count == 2);
    BOXING_ASSERT(technical_metadata2->reference_count == 2);
    BOXING_ASSERT(technical_metadata1 == technical_metadata2);

    technical_metadata1->afs_content_boxing_format = afs_boxing_format_create();
    technical_metadata1->afs_content_boxing_format->bytes_per_frame = 99;

    BOXING_ASSERT(technical_metadata2->afs_content_boxing_format->bytes_per_frame == 99);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_get_new_reference with not empty afs_toc_files instance and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_get_new_reference_test9)
{
    afs_toc_files * toc_files = create_toc_files_instance(3);
    afs_boxing_format * boxing_format = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 0, 3);
    BOXING_ASSERT(technical_metadata1->reference_count == 1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_get_new_reference(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 0, 3);
    BOXING_ASSERT(technical_metadata1->reference_count == 2);
    BOXING_ASSERT(technical_metadata2->reference_count == 2);
    BOXING_ASSERT(technical_metadata1 == technical_metadata2);

    technical_metadata1->afs_content_boxing_format->bytes_per_frame = 99;

    BOXING_ASSERT(technical_metadata2->afs_content_boxing_format->bytes_per_frame == 99);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_get_new_reference with not empty afs_toc_files instance and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_get_new_reference_test10)
{
    afs_toc_files * toc_files = create_toc_files_instance(3);
    afs_boxing_format * boxing_format = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files, boxing_format);

    test_afs_technical_metadata(technical_metadata1, 1, 3);
    BOXING_ASSERT(technical_metadata1->reference_count == 1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_get_new_reference(technical_metadata1);

    test_afs_technical_metadata(technical_metadata2, 1, 3);
    BOXING_ASSERT(technical_metadata1->reference_count == 2);
    BOXING_ASSERT(technical_metadata2->reference_count == 2);
    BOXING_ASSERT(technical_metadata1 == technical_metadata2);

    technical_metadata1->afs_content_boxing_format->bytes_per_frame = 99;

    BOXING_ASSERT(technical_metadata2->afs_content_boxing_format->bytes_per_frame == 99);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with input afs_technical_metadata pointers equal to NULL
BOXING_START_TEST(afs_technical_metadata_equal_test1)
{
    afs_technical_metadata * technical_metadata1 = NULL;
    afs_technical_metadata * technical_metadata2 = NULL;

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DTRUE);
}
END_TEST

// Test function afs_technical_metadata_equal with empty input afs_technical_metadata instances 
BOXING_START_TEST(afs_technical_metadata_equal_test2)
{
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create();
    test_empty_afs_technical_metadata(technical_metadata1);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create();
    test_empty_afs_technical_metadata(technical_metadata2);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DTRUE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with afs_technical_metadata instances with afs_toc_files pointer equal to NULL and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_equal_test3)
{
    afs_boxing_format * boxing_format1 = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(NULL, boxing_format1);
    test_afs_technical_metadata(technical_metadata1, 0, -1);

    afs_boxing_format * boxing_format2 = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create2(NULL, boxing_format2);
    test_afs_technical_metadata(technical_metadata2, 0, -1);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DTRUE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with afs_technical_metadata instances with afs_toc_files pointer equal to NULL and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_equal_test4)
{
    afs_boxing_format * boxing_format1 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(NULL, boxing_format1);
    test_afs_technical_metadata(technical_metadata1, 1, -1);

    afs_boxing_format * boxing_format2 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create2(NULL, boxing_format2);
    test_afs_technical_metadata(technical_metadata2, 1, -1);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DTRUE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with afs_technical_metadata instances with empty afs_toc_files instance and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_equal_test5)
{
    afs_toc_files * toc_files1 = afs_toc_files_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files1, NULL);
    test_afs_technical_metadata(technical_metadata1, -1, 0);

    afs_toc_files * toc_files2 = afs_toc_files_create();
    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create2(toc_files2, NULL);
    test_afs_technical_metadata(technical_metadata2, -1, 0);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DTRUE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with afs_technical_metadata instances with empty afs_toc_files instance and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_equal_test6)
{
    afs_toc_files * toc_files1 = afs_toc_files_create();
    afs_boxing_format * boxing_format1 = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files1, boxing_format1);
    test_afs_technical_metadata(technical_metadata1, 0, 0);

    afs_toc_files * toc_files2 = afs_toc_files_create();
    afs_boxing_format * boxing_format2 = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create2(toc_files2, boxing_format2);
    test_afs_technical_metadata(technical_metadata2, 0, 0);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DTRUE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with afs_technical_metadata instances with empty afs_toc_files instance and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_equal_test7)
{
    afs_toc_files * toc_files1 = afs_toc_files_create();
    afs_boxing_format * boxing_format1 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files1, boxing_format1);
    test_afs_technical_metadata(technical_metadata1, 1, 0);

    afs_toc_files * toc_files2 = afs_toc_files_create();
    afs_boxing_format * boxing_format2 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create2(toc_files2, boxing_format2);
    test_afs_technical_metadata(technical_metadata2, 1, 0);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DTRUE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with afs_technical_metadata instances with not empty afs_toc_files instance and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_equal_test8)
{
    afs_toc_files * toc_files1 = create_toc_files_instance(3);
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files1, NULL);
    test_afs_technical_metadata(technical_metadata1, -1, 3);

    afs_toc_files * toc_files2 = create_toc_files_instance(3);
    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create2(toc_files2, NULL);
    test_afs_technical_metadata(technical_metadata2, -1, 3);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DTRUE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with afs_technical_metadata instances with not empty afs_toc_files instance and empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_equal_test9)
{
    afs_toc_files * toc_files1 = create_toc_files_instance(3);
    afs_boxing_format * boxing_format1 = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files1, boxing_format1);
    test_afs_technical_metadata(technical_metadata1, 0, 3);

    afs_toc_files * toc_files2 = create_toc_files_instance(3);
    afs_boxing_format * boxing_format2 = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create2(toc_files2, boxing_format2);
    test_afs_technical_metadata(technical_metadata2, 0, 3);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DTRUE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with afs_technical_metadata instances with not empty afs_toc_files instance and not empty afs_boxing_format instance
BOXING_START_TEST(afs_technical_metadata_equal_test10)
{
    afs_toc_files * toc_files1 = create_toc_files_instance(3);
    afs_boxing_format * boxing_format1 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files1, boxing_format1);
    test_afs_technical_metadata(technical_metadata1, 1, 3);

    afs_toc_files * toc_files2 = create_toc_files_instance(3);
    afs_boxing_format * boxing_format2 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create2(toc_files2, boxing_format2);
    test_afs_technical_metadata(technical_metadata2, 1, 3);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DTRUE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with empty input afs_technical_metadata instance and pointer equal to NULL 
BOXING_START_TEST(afs_technical_metadata_equal_test11)
{
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create();
    test_empty_afs_technical_metadata(technical_metadata1);

    afs_technical_metadata * technical_metadata2 = NULL;

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DFALSE);

    afs_technical_metadata_free(technical_metadata1);
}
END_TEST


// Test function afs_technical_metadata_equal with not empty input afs_technical_metadata instance and pointer equal to NULL 
BOXING_START_TEST(afs_technical_metadata_equal_test12)
{
    afs_toc_files * toc_files1 = create_toc_files_instance(3);
    afs_boxing_format * boxing_format1 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files1, boxing_format1);
    test_afs_technical_metadata(technical_metadata1, 1, 3);

    afs_technical_metadata * technical_metadata2 = NULL;

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DFALSE);

    afs_technical_metadata_free(technical_metadata1);
}
END_TEST


// Test function afs_technical_metadata_equal with not empty input afs_technical_metadata instance and empty input afs_technical_metadata instance 
BOXING_START_TEST(afs_technical_metadata_equal_test13)
{
    afs_toc_files * toc_files1 = create_toc_files_instance(3);
    afs_boxing_format * boxing_format1 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files1, boxing_format1);
    test_afs_technical_metadata(technical_metadata1, 1, 3);

    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create();
    test_empty_afs_technical_metadata(technical_metadata2);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DFALSE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with input afs_technical_metadata instances with different afs_toc_files instances
BOXING_START_TEST(afs_technical_metadata_equal_test14)
{
    afs_toc_files * toc_files1 = create_toc_files_instance(3);
    afs_boxing_format * boxing_format1 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files1, boxing_format1);
    test_afs_technical_metadata(technical_metadata1, 1, 3);

    afs_toc_files * toc_files2 = afs_toc_files_create();
    afs_boxing_format * boxing_format2 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create2(toc_files2, boxing_format2);
    test_afs_technical_metadata(technical_metadata2, 1, 0);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DFALSE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_equal with input afs_technical_metadata instances with different afs_boxing_format instances 
BOXING_START_TEST(afs_technical_metadata_equal_test15)
{
    afs_toc_files * toc_files1 = create_toc_files_instance(3);
    afs_boxing_format * boxing_format1 = create_boxing_format_instance();
    afs_technical_metadata * technical_metadata1 = afs_technical_metadata_create2(toc_files1, boxing_format1);
    test_afs_technical_metadata(technical_metadata1, 1, 3);

    afs_toc_files * toc_files2 = create_toc_files_instance(3);
    afs_boxing_format * boxing_format2 = afs_boxing_format_create();
    afs_technical_metadata * technical_metadata2 = afs_technical_metadata_create2(toc_files2, boxing_format2);
    test_afs_technical_metadata(technical_metadata2, 0, 3);

    test_equals_technical_metadata(technical_metadata2, technical_metadata1, DFALSE);

    afs_technical_metadata_free(technical_metadata1);
    afs_technical_metadata_free(technical_metadata2);
}
END_TEST


// Test function afs_technical_metadata_save_file with file_name pointer equal to NULL and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_file_test1)
{
    const char * file_name = NULL;
    afs_technical_metadata * technical_metadata = NULL;

    DBOOL result = afs_technical_metadata_save_file(technical_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(technical_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_technical_metadata_save_file with file_name pointer equal to NULL and afs_technical_metadata pointer not equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_file_test2)
{
    const char * file_name = NULL;

    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, 1);

    DBOOL result = afs_technical_metadata_save_file(technical_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_file with file_name pointer not equal to NULL and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_file_test3)
{
    const char * file_name = "afs_technical_metadata_save_file_test.xml";
    afs_technical_metadata * technical_metadata = NULL;

    DBOOL result = afs_technical_metadata_save_file(technical_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(technical_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_technical_metadata_save_file with empty afs_technical_metadata instance
BOXING_START_TEST(afs_technical_metadata_save_file_test4)
{
    const char * file_name = "afs_technical_metadata_save_file_empty_test.xml";
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_save_file(technical_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_file with afs_technical_metadata instance with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_file_test5)
{
    const char * file_name = "afs_technical_metadata_save_file_null_toc_files_test.xml";
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(-1, 1);

    DBOOL result = afs_technical_metadata_save_file(technical_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_file with afs_technical_metadata instance with afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_file_test6)
{
    const char * file_name = "afs_technical_metadata_save_file_null_boxing_format_test.xml";
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, -1);

    DBOOL result = afs_technical_metadata_save_file(technical_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_file with afs_technical_metadata instance with empty internal instances, in compact format
BOXING_START_TEST(afs_technical_metadata_save_file_test7)
{
    const char * file_name = "afs_technical_metadata_save_file_empty_instances_compact_test.xml";
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(0, 0);

    DBOOL result = afs_technical_metadata_save_file(technical_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_file with afs_technical_metadata instance with empty internal instances, in not compact format
BOXING_START_TEST(afs_technical_metadata_save_file_test8)
{
    const char * file_name = "afs_technical_metadata_save_file_empty_instances_not_compact_test.xml";
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(0, 0);

    DBOOL result = afs_technical_metadata_save_file(technical_metadata, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_file with afs_technical_metadata instance with not empty internal instances, in compact format
BOXING_START_TEST(afs_technical_metadata_save_file_test9)
{
    const char * file_name = "afs_technical_metadata_save_file_compact_test.xml";

    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, 1);

    DBOOL result = afs_technical_metadata_save_file(technical_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_file with afs_technical_metadata instance with not empty internal instances, in not compact format
BOXING_START_TEST(afs_technical_metadata_save_file_test10)
{
    const char * file_name = "afs_technical_metadata_save_file_not_compact_test.xml";

    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, 1);

    DBOOL result = afs_technical_metadata_save_file(technical_metadata, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_string with afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_string_test1)
{
    afs_technical_metadata * technical_metadata = NULL;

    char * result = afs_technical_metadata_save_string(technical_metadata, DTRUE);

    BOXING_ASSERT(technical_metadata == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_technical_metadata_save_string with empty afs_technical_metadata instance
BOXING_START_TEST(afs_technical_metadata_save_string_test2)
{
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    char * result = afs_technical_metadata_save_string(technical_metadata, DTRUE);

    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == NULL);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_string with afs_technical_metadata instance with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_string_test3)
{
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(-1, 1);

    char * result = afs_technical_metadata_save_string(technical_metadata, DTRUE);

    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == NULL);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_string with afs_technical_metadata instance with afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_string_test4)
{
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, -1);

    char * result = afs_technical_metadata_save_string(technical_metadata, DTRUE);

    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == NULL);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_string with afs_technical_metadata instance with empty internal instances, in compact format
BOXING_START_TEST(afs_technical_metadata_save_string_test5)
{
    char * test_string = read_xml_control_data("afs_technical_metadata_save_file_empty_instances_compact_test.xml");

    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(0, 0);

    char * result = afs_technical_metadata_save_string(technical_metadata, DTRUE);

    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_technical_metadata_free(technical_metadata);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_technical_metadata_save_string with afs_technical_metadata instance with empty internal instances, in not compact format
BOXING_START_TEST(afs_technical_metadata_save_string_test6)
{
    char * test_string = read_xml_control_data("afs_technical_metadata_save_file_empty_instances_not_compact_test.xml");

    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(0, 0);

    char * result = afs_technical_metadata_save_string(technical_metadata, DFALSE);

    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_technical_metadata_free(technical_metadata);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_technical_metadata_save_string with afs_technical_metadata instance with not empty internal instances, in compact format
BOXING_START_TEST(afs_technical_metadata_save_string_test7)
{
    char * test_string = read_xml_control_data("afs_technical_metadata_save_file_compact_test.xml");

    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, 1);

    char * result = afs_technical_metadata_save_string(technical_metadata, DTRUE);

    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_technical_metadata_free(technical_metadata);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_technical_metadata_save_string with afs_technical_metadata instance with not empty internal instances, in not compact format
BOXING_START_TEST(afs_technical_metadata_save_string_test8)
{
    char * test_string = read_xml_control_data("afs_technical_metadata_save_file_not_compact_test.xml");

    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, 1);

    char * result = afs_technical_metadata_save_string(technical_metadata, DFALSE);

    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_technical_metadata_free(technical_metadata);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_technical_metadata_save_xml with mxml_node_t pointer equal to NULL and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_xml_test1)
{
    afs_technical_metadata * technical_metadata = NULL;

    mxml_node_t * tree = NULL;

    DBOOL return_value = afs_technical_metadata_save_xml(technical_metadata, tree);

    BOXING_ASSERT(return_value == DFALSE);
}
END_TEST


// Test function afs_technical_metadata_save_xml with mxml_node_t pointer equal to NULL and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_technical_metadata_save_xml_test2)
{
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, 1);

    mxml_node_t * tree = NULL;

    DBOOL return_value = afs_technical_metadata_save_xml(technical_metadata, tree);

    BOXING_ASSERT(return_value == DFALSE);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_save_xml with afs_technical_metadata pointer equal to NULL and mxml_node_t pointer not equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_xml_test3)
{
    afs_technical_metadata * technical_metadata = NULL;

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_technical_metadata_save_xml(technical_metadata, tree);

    BOXING_ASSERT(return_value == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_save_xml with empty afs_technical_metadata instance
BOXING_START_TEST(afs_technical_metadata_save_xml_test4)
{
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_technical_metadata_save_xml(technical_metadata, tree);

    BOXING_ASSERT(return_value == DFALSE);

    afs_technical_metadata_free(technical_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_save_xml with empty afs_technical_metadata instance with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_xml_test5)
{
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(-1, 1);

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_technical_metadata_save_xml(technical_metadata, tree);

    BOXING_ASSERT(return_value == DTRUE);

    afs_technical_metadata_free(technical_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_save_xml with empty afs_technical_metadata instance with afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_save_xml_test6)
{
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, -1);

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_technical_metadata_save_xml(technical_metadata, tree);

    BOXING_ASSERT(return_value == DFALSE);

    afs_technical_metadata_free(technical_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_save_xml with empty afs_technical_metadata instance with empty internal instances
BOXING_START_TEST(afs_technical_metadata_save_xml_test7)
{
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(0, 0);

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_technical_metadata_save_xml(technical_metadata, tree);

    BOXING_ASSERT(return_value == DTRUE);

    afs_technical_metadata_free(technical_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_save_xml with empty afs_technical_metadata instance with not empty internal instances
BOXING_START_TEST(afs_technical_metadata_save_xml_test8)
{
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, 1);

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_technical_metadata_save_xml(technical_metadata, tree);

    BOXING_ASSERT(return_value == DTRUE);

    afs_technical_metadata_free(technical_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_load_file with file_name pointer equal to NULL and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_load_file_test1)
{
    const char * file_name = NULL;
    afs_technical_metadata * technical_metadata = NULL;

    DBOOL result = afs_technical_metadata_load_file(technical_metadata, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(technical_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_technical_metadata_load_file with file_name pointer equal to NULL and afs_technical_metadata pointer not equal to NULL
BOXING_START_TEST(afs_technical_metadata_load_file_test2)
{
    const char * file_name = NULL;

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_file(technical_metadata, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(technical_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_load_file with file_name pointer not equal to NULL and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_load_file_test3)
{
    const char * file_name = "afs_technical_metadata_save_file_compact_test.xml";
    afs_technical_metadata * technical_metadata = NULL;

    DBOOL result = afs_technical_metadata_load_file(technical_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(technical_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_technical_metadata_load_file when XML file does not exist
BOXING_START_TEST(afs_technical_metadata_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_file(technical_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_technical_metadata(technical_metadata);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_load_file for afs_technical_metadata instance with empty internal instances, in compact format
BOXING_START_TEST(afs_technical_metadata_load_file_test5)
{
    const char * file_name = "afs_technical_metadata_save_file_empty_instances_compact_test.xml";

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_file(technical_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 0, 0);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_load_file for afs_technical_metadata instance with empty internal instances, in not compact format
BOXING_START_TEST(afs_technical_metadata_load_file_test6)
{
    const char * file_name = "afs_technical_metadata_save_file_empty_instances_not_compact_test.xml";

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_file(technical_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 0, 0);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_load_file for afs_technical_metadata instance with not empty internal instances, in compact format
BOXING_START_TEST(afs_technical_metadata_load_file_test7)
{
    const char * file_name = "afs_technical_metadata_save_file_compact_test.xml";

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_file(technical_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 1, 3);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_load_file for afs_technical_metadata instance with not empty internal instances, in not compact format
BOXING_START_TEST(afs_technical_metadata_load_file_test8)
{
    const char * file_name = "afs_technical_metadata_save_file_not_compact_test.xml";

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_file(technical_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 1, 3);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_load_string with input string pointer equal to NULL and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_load_string_test1)
{
    const char * input_string = NULL;
    afs_technical_metadata * technical_metadata = NULL;

    DBOOL result = afs_technical_metadata_load_string(technical_metadata, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(technical_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_technical_metadata_load_string with input string pointer equal to NULL and afs_technical_metadata pointer not equal to NULL
BOXING_START_TEST(afs_technical_metadata_load_string_test2)
{
    const char * input_string = NULL;

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_string(technical_metadata, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_technical_metadata(technical_metadata);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_load_string with input string pointer not equal to NULL and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_load_string_test3)
{
    char * input_string = read_xml_control_data("afs_technical_metadata_save_file_compact_test.xml");
    afs_technical_metadata * technical_metadata = NULL;

    DBOOL result = afs_technical_metadata_load_string(technical_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(technical_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);

    boxing_string_free(input_string);
}
END_TEST


// Test function afs_technical_metadata_load_string with empty input string
BOXING_START_TEST(afs_technical_metadata_load_string_test4)
{
    char * input_string = "";
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_string(technical_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_technical_metadata(technical_metadata);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_load_string with input string for afs_technical_metadata instance with empty internal instances, in compact format
BOXING_START_TEST(afs_technical_metadata_load_string_test5)
{
    char * input_string = read_xml_control_data("afs_technical_metadata_save_file_empty_instances_compact_test.xml");

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_string(technical_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 0, 0);

    afs_technical_metadata_free(technical_metadata);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_technical_metadata_load_string with input string for afs_technical_metadata instance with empty internal instances, in not compact format
BOXING_START_TEST(afs_technical_metadata_load_string_test6)
{
    char * input_string = read_xml_control_data("afs_technical_metadata_save_file_empty_instances_not_compact_test.xml");

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_string(technical_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 0, 0);

    afs_technical_metadata_free(technical_metadata);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_technical_metadata_load_string with input string for afs_technical_metadata instance with not empty internal instances, in compact format
BOXING_START_TEST(afs_technical_metadata_load_string_test7)
{
    char * input_string = read_xml_control_data("afs_technical_metadata_save_file_compact_test.xml");

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_string(technical_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 1, 3);

    afs_technical_metadata_free(technical_metadata);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_technical_metadata_load_string with input string for afs_technical_metadata instance with not empty internal instances, in not compact format
BOXING_START_TEST(afs_technical_metadata_load_string_test8)
{
    char * input_string = read_xml_control_data("afs_technical_metadata_save_file_not_compact_test.xml");

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_string(technical_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 1, 3);

    afs_technical_metadata_free(technical_metadata);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_technical_metadata_load_xml with xml instance pointer equal to NULL and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_load_xml_test1)
{
    mxml_node_t * tree = NULL;
    afs_technical_metadata * technical_metadata = NULL;

    DBOOL result = afs_technical_metadata_load_xml(technical_metadata, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(technical_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_technical_metadata_load_xml with xml instance pointer equal to NULL and afs_technical_metadata pointer not equal to NULL
BOXING_START_TEST(afs_technical_metadata_load_xml_test2)
{
    mxml_node_t * tree = NULL;

    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_xml(technical_metadata, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_technical_metadata(technical_metadata);

    afs_technical_metadata_free(technical_metadata);
}
END_TEST


// Test function afs_technical_metadata_load_xml with xml instance pointer not equal to NULL and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_technical_metadata_load_xml_test3)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_technical_metadata_save_file_compact_test.xml");
    afs_technical_metadata * technical_metadata = NULL;

    DBOOL result = afs_technical_metadata_load_xml(technical_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(technical_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_load_xml with empty xml instance
BOXING_START_TEST(afs_technical_metadata_load_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_xml(technical_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_technical_metadata(technical_metadata);

    afs_technical_metadata_free(technical_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_load_xml with xml instance for afs_technical_metadata instance with empty internal instances, in compact format
BOXING_START_TEST(afs_technical_metadata_load_xml_test5)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_technical_metadata_save_file_empty_instances_compact_test.xml");
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_xml(technical_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 0, 0);

    afs_technical_metadata_free(technical_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_load_xml with xml instance for afs_technical_metadata instance with empty internal instances, in not compact format
BOXING_START_TEST(afs_technical_metadata_load_xml_test6)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_technical_metadata_save_file_empty_instances_not_compact_test.xml");
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_xml(technical_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 0, 0);

    afs_technical_metadata_free(technical_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_load_xml with xml instance for afs_technical_metadata instance with not empty internal instances, in compact format
BOXING_START_TEST(afs_technical_metadata_load_xml_test7)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_technical_metadata_save_file_compact_test.xml");
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_xml(technical_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 1, 3);

    afs_technical_metadata_free(technical_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_load_xml with xml instance for afs_technical_metadata instance with not empty internal instances, in not compact format
BOXING_START_TEST(afs_technical_metadata_load_xml_test8)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_technical_metadata_save_file_not_compact_test.xml");
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    DBOOL result = afs_technical_metadata_load_xml(technical_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_technical_metadata(technical_metadata, 1, 3);

    afs_technical_metadata_free(technical_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_technical_metadata_save_xml and afs_technical_metadata_load_xml with NULL afs_boxing_format pointer and NULL afs_toc_files pointer
BOXING_START_TEST(afs_technical_metadata_save_load_xml_test1)
{
    afs_technical_metadata* technical_metadata_save = afs_technical_metadata_create();

    mxml_node_t *tree = mxmlNewXML("1.0");

#ifndef WIN32
    FILE * fp_save = fopen("afs_technical_test1.xml", "w+");
#else
    FILE * fp_save = fopen("afs_technical_test1.xml", "w+b");
#endif

    DBOOL return_value = afs_technical_metadata_save_xml(technical_metadata_save, tree);

    BOXING_ASSERT(return_value == DFALSE);

    mxmlSaveFile(tree, fp_save, MXML_NO_CALLBACK);
    fclose(fp_save);

    afs_technical_metadata_free(technical_metadata_save);
    mxmlDelete(tree);


    afs_technical_metadata* technical_metadata_load = afs_technical_metadata_create();

#ifndef WIN32
    FILE * fp_load = fopen("afs_technical_test1.xml", "r");
#else
    FILE * fp_load = fopen("afs_technical_test1.xml", "rb");
#endif

    mxml_node_t * document = mxmlLoadFile(NULL, fp_load, MXML_OPAQUE_CALLBACK);

    return_value = afs_technical_metadata_load_xml(technical_metadata_load, document);

    BOXING_ASSERT(return_value == DTRUE);
    test_empty_afs_technical_metadata(technical_metadata_load);

    fclose(fp_load);

    afs_technical_metadata_free(technical_metadata_load);
    mxmlDelete(document);
}
END_TEST


// Test functions afs_technical_metadata_save_xml and afs_technical_metadata_load_xml with empty afs_boxing_format instance and empty afs_toc_files instance
BOXING_START_TEST(afs_technical_metadata_save_load_xml_test2)
{
    afs_toc_files * tocs_data = afs_toc_files_create();
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    afs_technical_metadata * technical_metadata_save = afs_technical_metadata_create2(tocs_data, boxing_format);

    mxml_node_t *tree = mxmlNewXML("1.0");

#ifndef WIN32
    FILE * fp_save = fopen("afs_technical_test2.xml", "w+");
#else
    FILE * fp_save = fopen("afs_technical_test2.xml", "w+b");
#endif

    DBOOL return_value = afs_technical_metadata_save_xml(technical_metadata_save, tree);

    BOXING_ASSERT(return_value == DTRUE);

    mxmlSaveFile(tree, fp_save, MXML_NO_CALLBACK);
    fclose(fp_save);

    afs_technical_metadata_free(technical_metadata_save);
    mxmlDelete(tree);


    afs_technical_metadata* technical_metadata_load = afs_technical_metadata_create();

#ifndef WIN32
    FILE * fp_load = fopen("afs_technical_test2.xml", "r");
#else
    FILE * fp_load = fopen("afs_technical_test2.xml", "rb");
#endif

    mxml_node_t * document = mxmlLoadFile(NULL, fp_load, MXML_OPAQUE_CALLBACK);

    return_value = afs_technical_metadata_load_xml(technical_metadata_load, document);

    BOXING_ASSERT(return_value == DTRUE);

    BOXING_ASSERT(technical_metadata_load->afs_tocs != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(technical_metadata_load->afs_tocs) == 0);

    BOXING_ASSERT(technical_metadata_load->afs_content_boxing_format != NULL);
    BOXING_ASSERT(technical_metadata_load->afs_content_boxing_format->config == NULL);

    fclose(fp_load);

    afs_technical_metadata_free(technical_metadata_load);
    mxmlDelete(document);
}
END_TEST


// TEST CONTROL DATA FUNCTIONS
//


// Test function afs_control_data_create
BOXING_START_TEST(afs_control_data_create_test1)
{
    afs_control_data* control_data = afs_control_data_create();

    test_empty_afs_control_data(control_data);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_create2 with input pointers equal to NULL
BOXING_START_TEST(afs_control_data_create2_test1)
{
    afs_control_data * control_data = afs_control_data_create2(NULL, NULL);

    test_empty_afs_control_data(control_data);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_create2 with afs_administrative_metadata pointer equal to NULL and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_create2_test2)
{
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    afs_control_data * control_data = afs_control_data_create2(NULL, technical_metadata);

    test_afs_control_data(control_data, -1, 0);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_create2 with afs_administrative_metadata pointer equal to NULL and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_create2_test3)
{
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, 1);

    afs_control_data * control_data = afs_control_data_create2(NULL, technical_metadata);

    test_afs_control_data(control_data, -1, 1);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_create2 with empty afs_administrative_metadata instance and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_create2_test4)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();

    afs_control_data * control_data = afs_control_data_create2(administrative_metadata, NULL);

    test_afs_control_data(control_data, 0, -1);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_create2 with empty afs_administrative_metadata instance and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_create2_test5)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    afs_control_data * control_data = afs_control_data_create2(administrative_metadata, technical_metadata);

    test_afs_control_data(control_data, 0, 0);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_create2 with empty afs_administrative_metadata instance and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_create2_test6)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create();
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, 1);

    afs_control_data * control_data = afs_control_data_create2(administrative_metadata, technical_metadata);

    test_afs_control_data(control_data, 0, 1);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_create2 with not empty afs_administrative_metadata instance and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_create2_test7)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");

    afs_control_data * control_data = afs_control_data_create2(administrative_metadata, NULL);

    test_afs_control_data(control_data, 1, -1);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_create2 with not empty afs_administrative_metadata instance and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_create2_test8)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    afs_technical_metadata * technical_metadata = afs_technical_metadata_create();

    afs_control_data * control_data = afs_control_data_create2(administrative_metadata, technical_metadata);

    test_afs_control_data(control_data, 1, 0);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_create2 with not empty afs_administrative_metadata instance and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_create2_test9)
{
    afs_administrative_metadata * administrative_metadata = afs_administrative_metadata_create2("reel id", "print reel id", "title", "description", "creator", "creation date");
    afs_technical_metadata * technical_metadata = create_technical_metadata_instance(3, 1);

    afs_control_data * control_data = afs_control_data_create2(administrative_metadata, technical_metadata);

    test_afs_control_data(control_data, 1, 1);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_clone with input pointer equal to NULL
BOXING_START_TEST(afs_control_data_clone_test1)
{
    afs_control_data * control_data1 = NULL;

    BOXING_ASSERT(control_data1 == NULL);

    afs_control_data * control_data2 = afs_control_data_clone(control_data1);

    BOXING_ASSERT(control_data2 == NULL);
}
END_TEST


// Test function afs_control_data_clone with empty input afs_control_data instance
BOXING_START_TEST(afs_control_data_clone_test2)
{
    afs_control_data * control_data1 = afs_control_data_create();

    test_empty_afs_control_data(control_data1);

    afs_control_data * control_data2 = afs_control_data_clone(control_data1);

    test_empty_afs_control_data(control_data2);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_clone with afs_administrative_metadata pointer equal to NULL and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_clone_test3)
{
    afs_control_data * control_data1 = create_control_data_instance(-1, 0);

    test_afs_control_data(control_data1, -1, 0);

    afs_control_data * control_data2 = afs_control_data_clone(control_data1);

    test_afs_control_data(control_data2, -1, 0);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_clone with afs_administrative_metadata pointer equal to NULL and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_clone_test4)
{
    afs_control_data * control_data1 = create_control_data_instance(-1, 1);

    test_afs_control_data(control_data1, -1, 1);

    afs_control_data * control_data2 = afs_control_data_clone(control_data1);

    test_afs_control_data(control_data2, -1, 1);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_clone with empty afs_administrative_metadata instance and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_clone_test5)
{
    afs_control_data * control_data1 = create_control_data_instance(0, -1);

    test_afs_control_data(control_data1, 0, -1);

    afs_control_data * control_data2 = afs_control_data_clone(control_data1);

    test_afs_control_data(control_data2, 0, -1);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_clone with empty afs_administrative_metadata instance and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_clone_test6)
{
    afs_control_data * control_data1 = create_control_data_instance(0, 0);;

    test_afs_control_data(control_data1, 0, 0);

    afs_control_data * control_data2 = afs_control_data_clone(control_data1);

    test_afs_control_data(control_data2, 0, 0);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_clone with empty afs_administrative_metadata instance and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_clone_test7)
{
    afs_control_data * control_data1 = create_control_data_instance(0, 1);

    test_afs_control_data(control_data1, 0, 1);

    afs_control_data * control_data2 = afs_control_data_clone(control_data1);

    test_afs_control_data(control_data2, 0, 1);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_clone with not empty afs_administrative_metadata instance and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_clone_test8)
{
    afs_control_data * control_data1 = create_control_data_instance(1, -1);

    test_afs_control_data(control_data1, 1, -1);
    
    afs_control_data * control_data2 = afs_control_data_clone(control_data1);

    test_afs_control_data(control_data2, 1, -1);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_clone with not empty afs_administrative_metadata instance and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_clone_test9)
{
    afs_control_data * control_data1 = create_control_data_instance(1, 0);

    test_afs_control_data(control_data1, 1, 0);

    afs_control_data * control_data2 = afs_control_data_clone(control_data1);

    test_afs_control_data(control_data2, 1, 0);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_clone with not empty afs_administrative_metadata instance and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_clone_test10)
{
    afs_control_data * control_data1 = create_control_data_instance(1, 1);

    test_afs_control_data(control_data1, 1, 1);

    afs_control_data * control_data2 = afs_control_data_clone(control_data1);

    test_afs_control_data(control_data2, 1, 1);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_control_data_get_new_reference_test1)
{
    afs_control_data * control_data1 = NULL;

    BOXING_ASSERT(control_data1 == NULL);

    afs_control_data * control_data2 = afs_control_data_get_new_reference(control_data1);

    BOXING_ASSERT(control_data2 == NULL);

    control_data1 = afs_control_data_create();

    test_empty_afs_control_data(control_data1);
    BOXING_ASSERT(control_data1->reference_count == 1);
    BOXING_ASSERT(control_data2 == NULL);

    afs_control_data_free(control_data1);
}
END_TEST


// Test function afs_control_data_get_new_reference with empty input afs_control_data instance
BOXING_START_TEST(afs_control_data_get_new_reference_test2)
{
    afs_control_data * control_data1 = afs_control_data_create();

    test_empty_afs_control_data(control_data1);
    BOXING_ASSERT(control_data1->reference_count == 1);

    afs_control_data * control_data2 = afs_control_data_get_new_reference(control_data1);

    test_empty_afs_control_data(control_data2);
    BOXING_ASSERT(control_data1->reference_count == 2);
    BOXING_ASSERT(control_data2->reference_count == 2);
    BOXING_ASSERT(control_data1 == control_data2);

    control_data1->administrative_metadata = afs_administrative_metadata_create();

    test_afs_control_data(control_data2, 0, -1);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_get_new_reference with afs_administrative_metadata pointer equal to NULL and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_get_new_reference_test3)
{
    afs_control_data * control_data1 = create_control_data_instance(-1, 0);

    test_afs_control_data(control_data1, -1, 0);
    BOXING_ASSERT(control_data1->reference_count == 1);

    afs_control_data * control_data2 = afs_control_data_get_new_reference(control_data1);

    test_afs_control_data(control_data2, -1, 0);
    BOXING_ASSERT(control_data1->reference_count == 2);
    BOXING_ASSERT(control_data2->reference_count == 2);
    BOXING_ASSERT(control_data1 == control_data2);

    control_data1->administrative_metadata = afs_administrative_metadata_create();

    test_afs_control_data(control_data2, 0, 0);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_get_new_reference with afs_administrative_metadata pointer equal to NULL and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_get_new_reference_test4)
{
    afs_control_data * control_data1 = create_control_data_instance(-1, 1);

    test_afs_control_data(control_data1, -1, 1);
    BOXING_ASSERT(control_data1->reference_count == 1);

    afs_control_data * control_data2 = afs_control_data_get_new_reference(control_data1);

    test_afs_control_data(control_data2, -1, 1);
    BOXING_ASSERT(control_data1->reference_count == 2);
    BOXING_ASSERT(control_data2->reference_count == 2);
    BOXING_ASSERT(control_data1 == control_data2);

    control_data1->administrative_metadata = afs_administrative_metadata_create();

    test_afs_control_data(control_data2, 0, 1);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_get_new_reference with empty afs_administrative_metadata instance and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_get_new_reference_test5)
{
    afs_control_data * control_data1 = create_control_data_instance(0, -1);

    test_afs_control_data(control_data1, 0, -1);
    BOXING_ASSERT(control_data1->reference_count == 1);

    afs_control_data * control_data2 = afs_control_data_get_new_reference(control_data1);

    test_afs_control_data(control_data2, 0, -1);
    BOXING_ASSERT(control_data1->reference_count == 2);
    BOXING_ASSERT(control_data2->reference_count == 2);
    BOXING_ASSERT(control_data1 == control_data2);

    control_data1->administrative_metadata->creator = boxing_string_clone("Some other creator!");

    BOXING_ASSERT(boxing_string_equal(control_data2->administrative_metadata->creator, "Some other creator!") == DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_get_new_reference with empty afs_administrative_metadata instance and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_get_new_reference_test6)
{
    afs_control_data * control_data1 = create_control_data_instance(0, 0);;

    test_afs_control_data(control_data1, 0, 0);
    BOXING_ASSERT(control_data1->reference_count == 1);

    afs_control_data * control_data2 = afs_control_data_get_new_reference(control_data1);

    test_afs_control_data(control_data2, 0, 0);
    BOXING_ASSERT(control_data1->reference_count == 2);
    BOXING_ASSERT(control_data2->reference_count == 2);
    BOXING_ASSERT(control_data1 == control_data2);

    control_data1->administrative_metadata->creator = boxing_string_clone("Some other creator!");

    BOXING_ASSERT(boxing_string_equal(control_data2->administrative_metadata->creator, "Some other creator!") == DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_get_new_reference with empty afs_administrative_metadata instance and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_get_new_reference_test7)
{
    afs_control_data * control_data1 = create_control_data_instance(0, 1);

    test_afs_control_data(control_data1, 0, 1);
    BOXING_ASSERT(control_data1->reference_count == 1);

    afs_control_data * control_data2 = afs_control_data_get_new_reference(control_data1);

    test_afs_control_data(control_data2, 0, 1);
    BOXING_ASSERT(control_data1->reference_count == 2);
    BOXING_ASSERT(control_data2->reference_count == 2);
    BOXING_ASSERT(control_data1 == control_data2);

    control_data1->administrative_metadata->creator = boxing_string_clone("Some other creator!");

    BOXING_ASSERT(boxing_string_equal(control_data2->administrative_metadata->creator, "Some other creator!") == DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_get_new_reference with not empty afs_administrative_metadata instance and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_get_new_reference_test8)
{
    afs_control_data * control_data1 = create_control_data_instance(1, -1);

    test_afs_control_data(control_data1, 1, -1);
    BOXING_ASSERT(control_data1->reference_count == 1);

    afs_control_data * control_data2 = afs_control_data_get_new_reference(control_data1);

    test_afs_control_data(control_data2, 1, -1);
    BOXING_ASSERT(control_data1->reference_count == 2);
    BOXING_ASSERT(control_data2->reference_count == 2);
    BOXING_ASSERT(control_data1 == control_data2);

    boxing_string_free(control_data1->administrative_metadata->creator);
    control_data1->administrative_metadata->creator = boxing_string_clone("Some other creator!");

    BOXING_ASSERT(boxing_string_equal(control_data2->administrative_metadata->creator, "Some other creator!") == DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_get_new_reference with not empty afs_administrative_metadata instance and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_get_new_reference_test9)
{
    afs_control_data * control_data1 = create_control_data_instance(1, 0);

    test_afs_control_data(control_data1, 1, 0);
    BOXING_ASSERT(control_data1->reference_count == 1);

    afs_control_data * control_data2 = afs_control_data_get_new_reference(control_data1);

    test_afs_control_data(control_data2, 1, 0);
    BOXING_ASSERT(control_data1->reference_count == 2);
    BOXING_ASSERT(control_data2->reference_count == 2);
    BOXING_ASSERT(control_data1 == control_data2);

    boxing_string_free(control_data1->administrative_metadata->creator);
    control_data1->administrative_metadata->creator = boxing_string_clone("Some other creator!");

    BOXING_ASSERT(boxing_string_equal(control_data2->administrative_metadata->creator, "Some other creator!") == DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_get_new_reference with not empty afs_administrative_metadata instance and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_get_new_reference_test10)
{
    afs_control_data * control_data1 = create_control_data_instance(1, 1);

    test_afs_control_data(control_data1, 1, 1);
    BOXING_ASSERT(control_data1->reference_count == 1);

    afs_control_data * control_data2 = afs_control_data_get_new_reference(control_data1);

    test_afs_control_data(control_data2, 1, 1);
    BOXING_ASSERT(control_data1->reference_count == 2);
    BOXING_ASSERT(control_data2->reference_count == 2);
    BOXING_ASSERT(control_data1 == control_data2);

    boxing_string_free(control_data1->administrative_metadata->creator);
    control_data1->administrative_metadata->creator = boxing_string_clone("Some other creator!");

    BOXING_ASSERT(boxing_string_equal(control_data2->administrative_metadata->creator, "Some other creator!") == DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with input afs_control_data pointers equal to NULL
BOXING_START_TEST(afs_control_data_equal_test1)
{
    afs_control_data * control_data1 = NULL;
    afs_control_data * control_data2 = NULL;

    test_equals_control_data(control_data2, control_data1, DTRUE);
}
END_TEST

// Test function afs_control_data_equal with empty input afs_control_data instances 
BOXING_START_TEST(afs_control_data_equal_test2)
{
    afs_control_data * control_data1 = afs_control_data_create();
    test_empty_afs_control_data(control_data1);

    afs_control_data * control_data2 = afs_control_data_create();
    test_empty_afs_control_data(control_data2);

    test_equals_control_data(control_data2, control_data1, DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with afs_control_data instances with afs_administrative_metadata pointer equal to NULL and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_equal_test3)
{
    afs_control_data * control_data1 = create_control_data_instance(-1, 0);
    test_afs_control_data(control_data1, -1, 0);

    afs_control_data * control_data2 = create_control_data_instance(-1, 0);
    test_afs_control_data(control_data2, -1, 0);

    test_equals_control_data(control_data2, control_data1, DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with afs_control_data instances with afs_administrative_metadata pointer equal to NULL and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_equal_test4)
{
    afs_control_data * control_data1 = create_control_data_instance(-1, 1);
    test_afs_control_data(control_data1, -1, 1);

    afs_control_data * control_data2 = create_control_data_instance(-1, 1);
    test_afs_control_data(control_data2, -1, 1);

    test_equals_control_data(control_data2, control_data1, DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with afs_control_data instances with empty afs_administrative_metadata instance and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_equal_test5)
{
    afs_control_data * control_data1 = create_control_data_instance(0, -1);
    test_afs_control_data(control_data1, 0, -1);

    afs_control_data * control_data2 = create_control_data_instance(0, -1);
    test_afs_control_data(control_data2, 0, -1);

    test_equals_control_data(control_data2, control_data1, DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with afs_control_data instances with empty afs_administrative_metadata instance and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_equal_test6)
{
    afs_control_data * control_data1 = create_control_data_instance(0, 0);
    test_afs_control_data(control_data1, 0, 0);

    afs_control_data * control_data2 = create_control_data_instance(0, 0);
    test_afs_control_data(control_data2, 0, 0);

    test_equals_control_data(control_data2, control_data1, DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with afs_control_data instances with empty afs_administrative_metadata instance and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_equal_test7)
{
    afs_control_data * control_data1 = create_control_data_instance(0, 1);
    test_afs_control_data(control_data1, 0, 1);

    afs_control_data * control_data2 = create_control_data_instance(0, 1);
    test_afs_control_data(control_data2, 0, 1);

    test_equals_control_data(control_data2, control_data1, DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with afs_control_data instances with not empty afs_administrative_metadata instance and afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_equal_test8)
{
    afs_control_data * control_data1 = create_control_data_instance(1, -1);
    test_afs_control_data(control_data1, 1, -1);

    afs_control_data * control_data2 = create_control_data_instance(1, -1);
    test_afs_control_data(control_data2, 1, -1);

    test_equals_control_data(control_data2, control_data1, DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with afs_control_data instances with not empty afs_administrative_metadata instance and empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_equal_test9)
{
    afs_control_data * control_data1 = create_control_data_instance(1, 0);
    test_afs_control_data(control_data1, 1, 0);

    afs_control_data * control_data2 = create_control_data_instance(1, 0);
    test_afs_control_data(control_data2, 1, 0);

    test_equals_control_data(control_data2, control_data1, DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with afs_control_data instances with not empty afs_administrative_metadata instance and not empty afs_technical_metadata instance
BOXING_START_TEST(afs_control_data_equal_test10)
{
    afs_control_data * control_data1 = create_control_data_instance(1, 1);
    test_afs_control_data(control_data1, 1, 1);

    afs_control_data * control_data2 = create_control_data_instance(1, 1);
    test_afs_control_data(control_data2, 1, 1);

    test_equals_control_data(control_data2, control_data1, DTRUE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with empty input afs_control_data instance and pointer equal to NULL 
BOXING_START_TEST(afs_control_data_equal_test11)
{
    afs_control_data * control_data1 = afs_control_data_create();
    test_empty_afs_control_data(control_data1);

    afs_control_data * control_data2 = NULL;

    test_equals_control_data(control_data2, control_data1, DFALSE);

    afs_control_data_free(control_data1);
}
END_TEST


// Test function afs_control_data_equal with not empty input afs_control_data instance and pointer equal to NULL 
BOXING_START_TEST(afs_control_data_equal_test12)
{
    afs_control_data * control_data1 = create_control_data_instance(1, 1);
    test_afs_control_data(control_data1, 1, 1);

    afs_control_data * control_data2 = NULL;

    test_equals_control_data(control_data2, control_data1, DFALSE);

    afs_control_data_free(control_data1);
}
END_TEST


// Test function afs_control_data_equal with not empty input afs_control_data instance and empty input afs_control_data instance 
BOXING_START_TEST(afs_control_data_equal_test13)
{
    afs_control_data * control_data1 = create_control_data_instance(1, 1);
    test_afs_control_data(control_data1, 1, 1);

    afs_control_data * control_data2 = afs_control_data_create();
    test_empty_afs_control_data(control_data2);

    test_equals_control_data(control_data2, control_data1, DFALSE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with input afs_control_data instances with different afs_administrative_metadata instances
BOXING_START_TEST(afs_control_data_equal_test14)
{
    afs_control_data * control_data1 = create_control_data_instance(1, 1);
    test_afs_control_data(control_data1, 1, 1);

    afs_control_data * control_data2 = create_control_data_instance(0, 1);
    test_afs_control_data(control_data2, 0, 1);

    test_equals_control_data(control_data2, control_data1, DFALSE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_equal with input afs_control_data instances with different afs_technical_metadata instances 
BOXING_START_TEST(afs_control_data_equal_test15)
{
    afs_control_data * control_data1 = create_control_data_instance(1, 1);
    test_afs_control_data(control_data1, 1, 1);

    afs_control_data * control_data2 = create_control_data_instance(1, 0);
    test_afs_control_data(control_data2, 1, 0);

    test_equals_control_data(control_data2, control_data1, DFALSE);

    afs_control_data_free(control_data1);
    afs_control_data_free(control_data2);
}
END_TEST


// Test function afs_control_data_save_file with file_name pointer equal to NULL and afs_control_data pointer equal to NULL
BOXING_START_TEST(afs_control_data_save_file_test1)
{
    const char * file_name = NULL;
    afs_control_data * control_data = NULL;

    DBOOL result = afs_control_data_save_file(control_data, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(control_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_control_data_save_file with file_name pointer equal to NULL and afs_control_data pointer not equal to NULL
BOXING_START_TEST(afs_control_data_save_file_test2)
{
    const char * file_name = NULL;

    afs_control_data * control_data = create_control_data_instance(1, 1);

    DBOOL result = afs_control_data_save_file(control_data, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_file with file_name pointer not equal to NULL and afs_control_data pointer equal to NULL
BOXING_START_TEST(afs_control_data_save_file_test3)
{
    const char * file_name = "afs_control_data_save_file_test.xml";
    afs_control_data * control_data = NULL;

    DBOOL result = afs_control_data_save_file(control_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(control_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_control_data_save_file with empty afs_control_data instance
BOXING_START_TEST(afs_control_data_save_file_test4)
{
    const char * file_name = "afs_control_data_save_file_empty_test.xml";
    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_save_file(control_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_file with afs_control_data instance with afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_save_file_test5)
{
    const char * file_name = "afs_control_data_save_file_null_administrative_metadata_test.xml";
    afs_control_data * control_data = create_control_data_instance(-1, 1);

    DBOOL result = afs_control_data_save_file(control_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_file with afs_control_data instance with afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_save_file_test6)
{
    const char * file_name = "afs_control_data_save_file_null_technical_metadata_test.xml";
    afs_control_data * control_data = create_control_data_instance(1, -1);

    DBOOL result = afs_control_data_save_file(control_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_file with afs_control_data instance with empty internal instances, in compact format
BOXING_START_TEST(afs_control_data_save_file_test7)
{
    const char * file_name = "afs_control_data_save_file_empty_instances_compact_test.xml";
    afs_control_data * control_data = create_control_data_instance(0, 0);

    DBOOL result = afs_control_data_save_file(control_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_file with afs_control_data instance with empty internal instances, in not compact format
BOXING_START_TEST(afs_control_data_save_file_test8)
{
    const char * file_name = "afs_control_data_save_file_empty_instances_not_compact_test.xml";
    afs_control_data * control_data = create_control_data_instance(0, 0);

    DBOOL result = afs_control_data_save_file(control_data, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_file with afs_control_data instance with not empty internal instances, in compact format
BOXING_START_TEST(afs_control_data_save_file_test9)
{
    const char * file_name = "afs_control_data_save_file_compact_test.xml";

    afs_control_data * control_data = create_control_data_instance(1, 1);

    DBOOL result = afs_control_data_save_file(control_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_file with afs_control_data instance with not empty internal instances, in not compact format
BOXING_START_TEST(afs_control_data_save_file_test10)
{
    const char * file_name = "afs_control_data_save_file_not_compact_test.xml";

    afs_control_data * control_data = create_control_data_instance(1, 1);

    DBOOL result = afs_control_data_save_file(control_data, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_string with afs_control_data pointer equal to NULL
BOXING_START_TEST(afs_control_data_save_string_test1)
{
    afs_control_data * control_data = NULL;

    char * result = afs_control_data_save_string(control_data, DTRUE);

    BOXING_ASSERT(control_data == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_control_data_save_string with empty afs_control_data instance
BOXING_START_TEST(afs_control_data_save_string_test2)
{
    afs_control_data * control_data = afs_control_data_create();

    char * result = afs_control_data_save_string(control_data, DTRUE);

    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == NULL);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_string with afs_control_data instance with afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_save_string_test3)
{
    afs_control_data * control_data = create_control_data_instance(-1, 1);

    char * result = afs_control_data_save_string(control_data, DTRUE);

    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == NULL);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_string with afs_control_data instance with afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_save_string_test4)
{
    afs_control_data * control_data = create_control_data_instance(1, -1);

    char * result = afs_control_data_save_string(control_data, DTRUE);

    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == NULL);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_string with afs_control_data instance with empty internal instances, in compact format
BOXING_START_TEST(afs_control_data_save_string_test5)
{
    afs_control_data * control_data = create_control_data_instance(0, 0);

    char * result = afs_control_data_save_string(control_data, DTRUE);

    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == NULL);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_string with afs_control_data instance with empty internal instances, in not compact format
BOXING_START_TEST(afs_control_data_save_string_test6)
{
    afs_control_data * control_data = create_control_data_instance(0, 0);

    char * result = afs_control_data_save_string(control_data, DFALSE);

    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == NULL);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_string with afs_control_data instance with not empty internal instances, in compact format
BOXING_START_TEST(afs_control_data_save_string_test7)
{
    char * test_string = read_xml_control_data("afs_control_data_save_file_compact_test.xml");

    afs_control_data * control_data = create_control_data_instance(1, 1);

    char * result = afs_control_data_save_string(control_data, DTRUE);

    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_control_data_free(control_data);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_control_data_save_string with afs_control_data instance with not empty internal instances, in not compact format
BOXING_START_TEST(afs_control_data_save_string_test8)
{
    char * test_string = read_xml_control_data("afs_control_data_save_file_not_compact_test.xml");

    afs_control_data * control_data = create_control_data_instance(1, 1);

    char * result = afs_control_data_save_string(control_data, DFALSE);

    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(boxing_string_equal(test_string, result) == DTRUE);

    afs_control_data_free(control_data);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_control_data_save_xml with mxml_node_t pointer equal to NULL and afs_control_data pointer equal to NULL
BOXING_START_TEST(afs_control_data_save_xml_test1)
{
    afs_control_data * control_data = NULL;

    mxml_node_t * tree = NULL;

    DBOOL return_value = afs_control_data_save_xml(control_data, tree);

    BOXING_ASSERT(return_value == DFALSE);
}
END_TEST


// Test function afs_control_data_save_xml with mxml_node_t pointer equal to NULL and not empty afs_control_data instance
BOXING_START_TEST(afs_control_data_save_xml_test2)
{
    afs_control_data * control_data = create_control_data_instance(1, 1);

    mxml_node_t * tree = NULL;

    DBOOL return_value = afs_control_data_save_xml(control_data, tree);

    BOXING_ASSERT(return_value == DFALSE);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_save_xml with afs_control_data pointer equal to NULL and mxml_node_t pointer not equal to NULL
BOXING_START_TEST(afs_control_data_save_xml_test3)
{
    afs_control_data * control_data = NULL;

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_control_data_save_xml(control_data, tree);

    BOXING_ASSERT(return_value == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_control_data_save_xml with empty afs_control_data instance
BOXING_START_TEST(afs_control_data_save_xml_test4)
{
    afs_control_data * control_data = afs_control_data_create();

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_control_data_save_xml(control_data, tree);

    BOXING_ASSERT(return_value == DFALSE);

    afs_control_data_free(control_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_control_data_save_xml with afs_control_data instance with afs_administrative_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_save_xml_test5)
{
    afs_control_data * control_data = create_control_data_instance(-1, 1);

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_control_data_save_xml(control_data, tree);

    BOXING_ASSERT(return_value == DFALSE);

    afs_control_data_free(control_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_control_data_save_xml with afs_control_data instance with afs_technical_metadata pointer equal to NULL
BOXING_START_TEST(afs_control_data_save_xml_test6)
{
    afs_control_data * control_data = create_control_data_instance(1, -1);

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_control_data_save_xml(control_data, tree);

    BOXING_ASSERT(return_value == DFALSE);

    afs_control_data_free(control_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_control_data_save_xml with afs_control_data instance with empty internal instances
BOXING_START_TEST(afs_control_data_save_xml_test7)
{
    afs_control_data * control_data = create_control_data_instance(0, 0);

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_control_data_save_xml(control_data, tree);

    BOXING_ASSERT(return_value == DFALSE);

    afs_control_data_free(control_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_control_data_save_xml with afs_control_data instance with not empty internal instances
BOXING_START_TEST(afs_control_data_save_xml_test8)
{
    afs_control_data * control_data = create_control_data_instance(1, 1);

    mxml_node_t * tree = mxmlNewXML("1.0");

    DBOOL return_value = afs_control_data_save_xml(control_data, tree);

    BOXING_ASSERT(return_value == DTRUE);

    afs_control_data_free(control_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_control_data_load_file with file_name pointer equal to NULL and afs_control_data pointer equal to NULL
BOXING_START_TEST(afs_control_data_load_file_test1)
{
    const char * file_name = NULL;
    afs_control_data * control_data = NULL;

    DBOOL result = afs_control_data_load_file(control_data, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(control_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_control_data_load_file with file_name pointer equal to NULL and afs_control_data pointer not equal to NULL
BOXING_START_TEST(afs_control_data_load_file_test2)
{
    const char * file_name = NULL;

    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_file(control_data, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(control_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_load_file with file_name pointer not equal to NULL and afs_control_data pointer equal to NULL
BOXING_START_TEST(afs_control_data_load_file_test3)
{
    const char * file_name = "afs_control_data_save_file_compact_test.xml";
    afs_control_data * control_data = NULL;

    DBOOL result = afs_control_data_load_file(control_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(control_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_control_data_load_file when XML file does not exist
BOXING_START_TEST(afs_control_data_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_file(control_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_control_data(control_data);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_load_file for afs_control_data instance with not empty internal instances, in compact format
BOXING_START_TEST(afs_control_data_load_file_test5)
{
    const char * file_name = "afs_control_data_save_file_compact_test.xml";

    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_file(control_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_control_data(control_data, 1, 1);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_load_file for afs_control_data instance with not empty internal instances, in not compact format
BOXING_START_TEST(afs_control_data_load_file_test6)
{
    const char * file_name = "afs_control_data_save_file_not_compact_test.xml";

    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_file(control_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_control_data(control_data, 1, 1);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_load_string with input string pointer equal to NULL and afs_control_data pointer equal to NULL
BOXING_START_TEST(afs_control_data_load_string_test1)
{
    const char * input_string = NULL;
    afs_control_data * control_data = NULL;

    DBOOL result = afs_control_data_load_string(control_data, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(control_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_control_data_load_string with input string pointer equal to NULL and afs_control_data pointer not equal to NULL
BOXING_START_TEST(afs_control_data_load_string_test2)
{
    const char * input_string = NULL;

    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_string(control_data, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_control_data(control_data);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_load_string with input string pointer not equal to NULL and afs_control_data pointer equal to NULL
BOXING_START_TEST(afs_control_data_load_string_test3)
{
    char * input_string = read_xml_control_data("afs_control_data_save_file_compact_test.xml");
    afs_control_data * control_data = NULL;

    DBOOL result = afs_control_data_load_string(control_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(control_data == NULL);
    BOXING_ASSERT(result == DFALSE);

    boxing_string_free(input_string);
}
END_TEST


// Test function afs_control_data_load_string with empty input string
BOXING_START_TEST(afs_control_data_load_string_test4)
{
    char * input_string = "";
    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_string(control_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_control_data(control_data);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_load_string with input string for afs_control_data instance with not empty internal instances, in compact format
BOXING_START_TEST(afs_control_data_load_string_test5)
{
    char * input_string = read_xml_control_data("afs_control_data_save_file_compact_test.xml");

    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_string(control_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_control_data(control_data, 1, 1);

    afs_control_data_free(control_data);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_control_data_load_string with input string for afs_control_data instance with not empty internal instances, in not compact format
BOXING_START_TEST(afs_control_data_load_string_test6)
{
    char * input_string = read_xml_control_data("afs_control_data_save_file_not_compact_test.xml");

    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_string(control_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_control_data(control_data, 1, 1);

    afs_control_data_free(control_data);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_control_data_load_xml with xml instance pointer equal to NULL and afs_control_data pointer equal to NULL
BOXING_START_TEST(afs_control_data_load_xml_test1)
{
    mxml_node_t * tree = NULL;
    afs_control_data * control_data = NULL;

    DBOOL result = afs_control_data_load_xml(control_data, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(control_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_control_data_load_xml with xml instance pointer equal to NULL and afs_control_data pointer not equal to NULL
BOXING_START_TEST(afs_control_data_load_xml_test2)
{
    mxml_node_t * tree = NULL;

    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_xml(control_data, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_control_data(control_data);

    afs_control_data_free(control_data);
}
END_TEST


// Test function afs_control_data_load_xml with xml instance pointer not equal to NULL and afs_control_data pointer equal to NULL
BOXING_START_TEST(afs_control_data_load_xml_test3)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_control_data_save_file_compact_test.xml");
    afs_control_data * control_data = NULL;

    DBOOL result = afs_control_data_load_xml(control_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(control_data == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_control_data_load_xml with empty xml instance
BOXING_START_TEST(afs_control_data_load_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_xml(control_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_control_data(control_data);

    afs_control_data_free(control_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_control_data_load_xml with xml instance for afs_control_data instance with not empty internal instances, in compact format
BOXING_START_TEST(afs_control_data_load_xml_test5)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_control_data_save_file_compact_test.xml");
    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_xml(control_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_control_data(control_data, 1, 1);

    afs_control_data_free(control_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_control_data_load_xml with xml instance for afs_control_data instance with not empty internal instances, in not compact format
BOXING_START_TEST(afs_control_data_load_xml_test6)
{
    mxml_node_t * tree = get_control_data_xml_instance("afs_control_data_save_file_not_compact_test.xml");
    afs_control_data * control_data = afs_control_data_create();

    DBOOL result = afs_control_data_load_xml(control_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_control_data(control_data, 1, 1);

    afs_control_data_free(control_data);
    mxmlDelete(tree);
}
END_TEST


Suite * controldata_tests(void)
{
    TCase * tc_controldata_functions_tests = tcase_create("controldata_functions_tests");
    // Test function afs_administrative_metadata_create
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_create_test1);
    // Test function afs_administrative_metadata_create2
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_create2_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_create2_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_create2_test3);
    // Test function afs_administrative_metadata_clone
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_clone_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_clone_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_clone_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_clone_test4);
    // Test function afs_administrative_metadata_get_new_reference
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_get_new_reference_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_get_new_reference_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_get_new_reference_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_get_new_reference_test4);
    // Test function afs_administrative_metadata_equal
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test10);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test11);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test12);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test13);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test14);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test15);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_equal_test16);
    // Test function afs_administrative_metadata_set_reel_id
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_reel_id_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_reel_id_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_reel_id_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_reel_id_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_reel_id_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_reel_id_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_reel_id_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_reel_id_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_reel_id_test9);
    // Test function afs_administrative_metadata_set_print_reel_id
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_print_reel_id_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_print_reel_id_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_print_reel_id_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_print_reel_id_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_print_reel_id_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_print_reel_id_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_print_reel_id_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_print_reel_id_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_print_reel_id_test9);
    // Test function afs_administrative_metadata_set_title
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_title_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_title_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_title_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_title_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_title_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_title_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_title_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_title_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_title_test9);
    // Test function afs_administrative_metadata_set_description
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_description_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_description_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_description_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_description_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_description_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_description_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_description_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_description_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_description_test9);
    // Test function afs_administrative_metadata_set_creator
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creator_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creator_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creator_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creator_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creator_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creator_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creator_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creator_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creator_test9);
    // Test function afs_administrative_metadata_set_creation_date
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creation_date_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creation_date_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creation_date_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creation_date_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creation_date_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creation_date_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creation_date_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creation_date_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_set_creation_date_test9);
    // Test function afs_administrative_metadata_save_file
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_file_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_file_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_file_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_file_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_file_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_file_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_file_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_file_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_file_test9);
    // Test function afs_administrative_metadata_save_string
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_string_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_string_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_string_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_string_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_string_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_string_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_string_test7);
    // Test function afs_administrative_metadata_save_xml
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_xml_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_xml_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_xml_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_xml_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_xml_test5);
    // Test function afs_administrative_metadata_load_file
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_file_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_file_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_file_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_file_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_file_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_file_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_file_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_file_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_file_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_file_test10);
    // Test function afs_administrative_metadata_load_string
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_string_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_string_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_string_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_string_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_string_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_string_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_string_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_string_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_string_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_string_test10);
    // Test function afs_administrative_metadata_load_xml
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_xml_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_xml_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_xml_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_xml_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_xml_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_xml_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_xml_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_xml_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_xml_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_load_xml_test10);
    // Test function afs_administrative_metadata_save_xml and afs_administrative_metadata_load_xml
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_load_xml_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_administrative_metadata_save_load_xml_test2);

    // Test function afs_technical_metadata_create
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_create_test1);
    // Test function afs_technical_metadata_create2
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_create2_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_create2_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_create2_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_create2_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_create2_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_create2_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_create2_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_create2_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_create2_test9);
    // Test function afs_technical_metadata_clone
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_clone_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_clone_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_clone_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_clone_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_clone_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_clone_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_clone_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_clone_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_clone_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_clone_test10);
    // Test function afs_technical_metadata_get_new_reference
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_get_new_reference_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_get_new_reference_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_get_new_reference_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_get_new_reference_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_get_new_reference_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_get_new_reference_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_get_new_reference_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_get_new_reference_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_get_new_reference_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_get_new_reference_test10);
    // Test function afs_technical_metadata_equal
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test10);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test11);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test12);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test13);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test14);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_equal_test15);
    // Test function afs_technical_metadata_save_file
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_file_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_file_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_file_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_file_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_file_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_file_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_file_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_file_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_file_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_file_test10);
    // Test function afs_technical_metadata_save_string
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_string_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_string_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_string_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_string_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_string_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_string_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_string_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_string_test8);
    // Test function afs_technical_metadata_save_xml
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_xml_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_xml_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_xml_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_xml_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_xml_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_xml_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_xml_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_xml_test8);
    // Test function afs_technical_metadata_load_file
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_file_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_file_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_file_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_file_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_file_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_file_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_file_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_file_test8);
    // Test function afs_technical_metadata_load_string
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_string_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_string_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_string_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_string_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_string_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_string_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_string_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_string_test8);
    // Test function afs_technical_metadata_load_xml
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_xml_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_xml_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_xml_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_xml_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_xml_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_xml_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_xml_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_load_xml_test8);
    // Test function afs_technical_metadata_save_xml and afs_technical_metadata_load_xml
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_load_xml_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_technical_metadata_save_load_xml_test2);

    // Test function afs_control_data_create
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_create_test1);
    // Test function afs_control_data_create2
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_create2_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_create2_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_create2_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_create2_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_create2_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_create2_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_create2_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_create2_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_create2_test9);
    // Test function afs_control_data_clone
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_clone_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_clone_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_clone_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_clone_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_clone_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_clone_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_clone_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_clone_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_clone_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_clone_test10);
    // Test function afs_control_data_get_new_reference
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_get_new_reference_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_get_new_reference_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_get_new_reference_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_get_new_reference_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_get_new_reference_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_get_new_reference_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_get_new_reference_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_get_new_reference_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_get_new_reference_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_get_new_reference_test10);
    // Test function afs_control_data_equal
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test10);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test11);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test12);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test13);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test14);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_equal_test15);
    // Test function afs_control_data_save_file
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_file_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_file_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_file_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_file_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_file_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_file_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_file_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_file_test8);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_file_test9);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_file_test10);
    // Test function afs_control_data_save_string
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_string_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_string_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_string_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_string_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_string_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_string_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_string_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_string_test8);
    // Test function afs_control_data_save_xml
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_xml_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_xml_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_xml_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_xml_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_xml_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_xml_test6);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_xml_test7);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_save_xml_test8);
    // Test function afs_control_data_load_file
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_file_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_file_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_file_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_file_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_file_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_file_test6);
    // Test function afs_control_data_load_string
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_string_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_string_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_string_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_string_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_string_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_string_test6);
    // Test function afs_control_data_load_xml
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_xml_test1);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_xml_test2);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_xml_test3);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_xml_test4);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_xml_test5);
    tcase_add_test(tc_controldata_functions_tests, afs_control_data_load_xml_test6);

    Suite * s = suite_create("controldata_test_util");
    suite_add_tcase(s, tc_controldata_functions_tests);

    return s;
}
