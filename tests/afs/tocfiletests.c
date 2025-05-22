#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "tocfile.h"
#include "boxing/utils.h"
#include "mxml.h"

#include <string.h>


#if defined ( D_OS_WIN32 )
#define DFSEEK _fseeki64
#define DFTELL _ftelli64
#else
#define DFSEEK fseeko
#define DFTELL ftello
#endif

static void test_empty_afs_toc_file(afs_toc_file* toc_data_file)
{
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(toc_data_file->name == NULL);
    BOXING_ASSERT(toc_data_file->date == NULL);
    BOXING_ASSERT(toc_data_file->checksum == NULL);
    BOXING_ASSERT(toc_data_file->unique_id == NULL);
    BOXING_ASSERT(toc_data_file->preview == NULL);
    BOXING_ASSERT(toc_data_file->file_format == NULL);
    BOXING_ASSERT(toc_data_file->metadata == NULL);

    BOXING_ASSERT(toc_data_file->size == 0);
    BOXING_ASSERT(toc_data_file->id == AFS_TOC_FILE_ID_UNDEFINED);
    BOXING_ASSERT(toc_data_file->start_frame == -1);
    BOXING_ASSERT(toc_data_file->start_byte == -1);
    BOXING_ASSERT(toc_data_file->end_frame == -1);
    BOXING_ASSERT(toc_data_file->end_byte == -1);
    BOXING_ASSERT(toc_data_file->parent_id == AFS_TOC_FILE_NO_PARENT);
    BOXING_ASSERT(toc_data_file->types == AFS_TOC_FILE_TYPE_UNDEFINED);
}


static void test_not_empty_afs_toc_file_preview_page3(afs_toc_file_preview_page * toc_file_preview_page, unsigned int page_number)
{
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(strcmp(toc_file_preview_page->layout_id, "Some layout ID") == 0);
    BOXING_ASSERT(toc_file_preview_page->start_frame == 9 + page_number);
    BOXING_ASSERT(toc_file_preview_page->start_section == 8 + page_number);
    BOXING_ASSERT(toc_file_preview_page->section_count == 7 + page_number);
    BOXING_ASSERT(toc_file_preview_page->dimension_x == 6 + page_number);
    BOXING_ASSERT(toc_file_preview_page->dimension_y == 5 + page_number);
    BOXING_ASSERT(toc_file_preview_page->overlap_x == 4 + page_number);
    BOXING_ASSERT(toc_file_preview_page->overlap_y == 3 + page_number);
}


static void test_not_empty_afs_toc_file_preview3(afs_toc_file_preview * toc_file_preview, unsigned int pages_count)
{
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(toc_file_preview->pages != NULL);
    BOXING_ASSERT(toc_file_preview->pages->size == pages_count);

    for (unsigned int i = 0; i < pages_count; i++)
    {
        afs_toc_file_preview_page * toc_file_preview_page = GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, i);
        test_not_empty_afs_toc_file_preview_page3(toc_file_preview_page, i);
    }
}


static void test_null_afs_toc_data_file_metadata2(afs_toc_data_file_metadata * toc_data_file_metadata)
{
    BOXING_ASSERT(toc_data_file_metadata != NULL);
    BOXING_ASSERT(toc_data_file_metadata->sources == NULL);
}

static void test_afs_toc_data_file_metadata_source2(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * data, const char * format_id, int file_id, int source_id)
{
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(strcmp(toc_data_file_metadata_source->data, data) == 0);
    BOXING_ASSERT(strcmp(toc_data_file_metadata_source->format_id, format_id) == 0);

    BOXING_ASSERT(toc_data_file_metadata_source->file_id == file_id);
    BOXING_ASSERT(toc_data_file_metadata_source->source_id == source_id);
}


static void test_not_empty_afs_toc_data_file_metadata2(afs_toc_data_file_metadata * toc_data_file_metadata, unsigned int sources_count)
{
    BOXING_ASSERT(toc_data_file_metadata != NULL);
    BOXING_ASSERT(toc_data_file_metadata->sources != NULL);
    BOXING_ASSERT(toc_data_file_metadata->sources->size == sources_count);

    for (unsigned int i = 0; i < sources_count; i++)
    {
        afs_toc_data_file_metadata_source * toc_data_file_metadata_source = GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, i);

        test_afs_toc_data_file_metadata_source2(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", i + 1, i + 2);
    }
}


static void test_not_empty_afs_toc_file(afs_toc_file* toc_data_file)
{
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(strcmp(toc_data_file->name, "input-toc.xml") == 0);
    BOXING_ASSERT(strcmp(toc_data_file->date, "2016-11-22 09:07:11") == 0);
    BOXING_ASSERT(strcmp(toc_data_file->checksum, "f2eb0d6c6964d206012695a10ea6cd877c95c340") == 0);
    BOXING_ASSERT(strcmp(toc_data_file->unique_id, "38A0AAAB-16B2-640C-5353-6DB8AE4367B9") == 0);
    test_not_empty_afs_toc_file_preview3(toc_data_file->preview, 3);
    BOXING_ASSERT(strcmp(toc_data_file->file_format, "file_format") == 0);
    test_not_empty_afs_toc_data_file_metadata2(toc_data_file->metadata, 5);

    BOXING_ASSERT(toc_data_file->size == 29672);
    BOXING_ASSERT(toc_data_file->id == AFS_TOC_FILE_NO_PARENT);
    BOXING_ASSERT(toc_data_file->start_frame == 2);
    BOXING_ASSERT(toc_data_file->start_byte == 0);
    BOXING_ASSERT(toc_data_file->end_frame == 2);
    BOXING_ASSERT(toc_data_file->end_byte == 29671);
    BOXING_ASSERT(toc_data_file->parent_id == AFS_TOC_FILE_NO_PARENT);
    BOXING_ASSERT(toc_data_file->types == AFS_TOC_FILE_TYPE_ALL);
}


static afs_toc_file_preview_pages* get_filled_pages_vector3(unsigned int pages_count)
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


static afs_toc_file * get_afs_toc_file_instance()
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(5);

    return afs_toc_file_create2(
        "input-toc.xml",
        "2016-11-22 09:07:11",
        "f2eb0d6c6964d206012695a10ea6cd877c95c340",
        29672,
        AFS_TOC_FILE_NO_PARENT,
        2,
        0,
        2,
        29671,
        "38A0AAAB-16B2-640C-5353-6DB8AE4367B9",
        AFS_TOC_FILE_NO_PARENT,
        AFS_TOC_FILE_TYPE_ALL,
        toc_file_preview,
        "file_format",
        toc_data_file_metadata);
}


static char * read_xml_toc_file(const char* file_name)
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
    char * xml_string = malloc((size_t)size + 1);

    // Reads the data from the input file
    if (1 != fread(xml_string, (size_t)size, 1, file))
    {
        fclose(file);
        return NULL;
    }

    xml_string[size] = '\0';

    size_t xml_header_size = strlen("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
    char * xml_string_wh = boxing_string_clone(xml_string + xml_header_size);

    free(xml_string);

    fclose(file);
    return xml_string_wh;
}


static mxml_node_t * get_toc_xml_instance(const char * file_name)
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


static void test_equals_toc_file(afs_toc_file * toc_file1, afs_toc_file * toc_file2, DBOOL expected_result)
{
    DBOOL result = afs_toc_file_equal(toc_file2, toc_file1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_file_equal(toc_file1, toc_file2);
    BOXING_ASSERT(result == expected_result);
}


static void test_not_equals_toc_file(afs_toc_file * toc_file1, afs_toc_file * toc_file2, DBOOL expected_result)
{
    DBOOL result = afs_toc_file_not_equal(toc_file2, toc_file1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_file_not_equal(toc_file1, toc_file2);
    BOXING_ASSERT(result == expected_result);
}



// TEST TOC DATA FILE FUNCTIONS
//

// Test function afs_toc_file_create test
BOXING_START_TEST(afs_toc_file_create_test1)
{
    afs_toc_file* toc_data_file = afs_toc_file_create();

    test_empty_afs_toc_file(toc_data_file);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_create2 test1 with empty initial values
BOXING_START_TEST(afs_toc_file_create2_test1)
{
    afs_toc_file* toc_data_file = afs_toc_file_create2(
        NULL,
        NULL,
        NULL,
        0,
        AFS_TOC_FILE_ID_UNDEFINED,
        -1,
        -1,
        -1,
        -1,
        NULL,
        AFS_TOC_FILE_NO_PARENT,
        AFS_TOC_FILE_TYPE_UNDEFINED,
        NULL,
        NULL,
        NULL);

    test_empty_afs_toc_file(toc_data_file);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_create2 test1 with normal initial values
BOXING_START_TEST(afs_toc_file_create2_test2)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(5);

    afs_toc_file* toc_data_file = afs_toc_file_create2(
        "input-toc.xml",
        "2016-11-22 09:07:11",
        "f2eb0d6c6964d206012695a10ea6cd877c95c340",
        29672,
        AFS_TOC_FILE_NO_PARENT,
        2,
        0,
        2,
        29671,
        "38A0AAAB-16B2-640C-5353-6DB8AE4367B9",
        AFS_TOC_FILE_NO_PARENT,
        AFS_TOC_FILE_TYPE_ALL,
        toc_file_preview,
        "file_format",
        toc_data_file_metadata);

    test_not_empty_afs_toc_file(toc_data_file);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_init test with afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_init_test1)
{
    afs_toc_file* toc_data_file = NULL;

    afs_toc_file_init(toc_data_file);

    BOXING_ASSERT(toc_data_file == NULL);
}
END_TEST


// Test function afs_toc_file_init test with allocated afs_toc_file pointer
BOXING_START_TEST(afs_toc_file_init_test2)
{
    afs_toc_file* toc_data_file = malloc(sizeof(afs_toc_file));

    afs_toc_file_init(toc_data_file);

    test_empty_afs_toc_file(toc_data_file);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_init2 test with afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_init2_test1)
{
    afs_toc_file_preview_pages * pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(5);

    afs_toc_file* toc_data_file = NULL;

    afs_toc_file_init2(
        toc_data_file,
        "input-toc.xml",
        "2016-11-22 09:07:11",
        "f2eb0d6c6964d206012695a10ea6cd877c95c340",
        29672,
        AFS_TOC_FILE_NO_PARENT,
        2,
        0,
        2,
        29671,
        "38A0AAAB-16B2-640C-5353-6DB8AE4367B9",
        AFS_TOC_FILE_NO_PARENT,
        AFS_TOC_FILE_TYPE_UNDEFINED,
        toc_file_preview,
        "file_format",
        toc_data_file_metadata);

    BOXING_ASSERT(toc_data_file == NULL);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_init2 test with normal initialization data
BOXING_START_TEST(afs_toc_file_init2_test2)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();

    afs_toc_file_preview_pages* pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(5);

    afs_toc_file_init2(
        toc_data_file,
        "input-toc.xml",
        "2016-11-22 09:07:11",
        "f2eb0d6c6964d206012695a10ea6cd877c95c340",
        29672,
        AFS_TOC_FILE_NO_PARENT,
        2,
        0,
        2,
        29671,
        "38A0AAAB-16B2-640C-5353-6DB8AE4367B9",
        AFS_TOC_FILE_NO_PARENT,
        AFS_TOC_FILE_TYPE_ALL,
        toc_file_preview,
        "file_format",
        toc_data_file_metadata);

    test_not_empty_afs_toc_file(toc_data_file);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_init2 test with empty initialization data
BOXING_START_TEST(afs_toc_file_init2_test3)
{
    afs_toc_file_preview_pages * pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(5);

    afs_toc_file * toc_data_file = afs_toc_file_create2(
        "input-toc.xml",
        "2016-11-22 09:07:11",
        "f2eb0d6c6964d206012695a10ea6cd877c95c340",
        29672,
        AFS_TOC_FILE_NO_PARENT,
        2,
        0,
        2,
        29671,
        "38A0AAAB-16B2-640C-5353-6DB8AE4367B9",
        AFS_TOC_FILE_NO_PARENT,
        AFS_TOC_FILE_TYPE_ALL,
        toc_file_preview,
        "file_format",
        toc_data_file_metadata);

    test_not_empty_afs_toc_file(toc_data_file);

    free(toc_data_file->name);
    free(toc_data_file->date);
    free(toc_data_file->checksum);
    free(toc_data_file->unique_id);
    free(toc_data_file->file_format);

    afs_toc_file_init2(
        toc_data_file,
        NULL,
        NULL,
        NULL,
        0,
        AFS_TOC_FILE_ID_UNDEFINED,
        -1,
        -1,
        -1,
        -1,
        NULL,
        AFS_TOC_FILE_NO_PARENT,
        AFS_TOC_FILE_TYPE_UNDEFINED,
        NULL,
        NULL,
        NULL);

    test_empty_afs_toc_file(toc_data_file);

    afs_toc_file_free(toc_data_file);
    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_clone test with input pointer equal to NULL
BOXING_START_TEST(afs_toc_file_clone_test1)
{
    afs_toc_file* toc_data_file1 = NULL;

    BOXING_ASSERT(toc_data_file1 == NULL);

    afs_toc_file* toc_data_file2 = afs_toc_file_clone(toc_data_file1);

    BOXING_ASSERT(toc_data_file2 == NULL);
}
END_TEST


// Test function afs_toc_file_clone test with empty input afs_toc_file instance
BOXING_START_TEST(afs_toc_file_clone_test2)
{
    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    test_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = afs_toc_file_clone(toc_data_file1);

    test_empty_afs_toc_file(toc_data_file2);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_clone test with normal input afs_toc_file instance
BOXING_START_TEST(afs_toc_file_clone_test3)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();

    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = afs_toc_file_clone(toc_data_file1);

    test_not_empty_afs_toc_file(toc_data_file2);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_file_get_new_reference_test1)
{
    afs_toc_file * toc_data_file1 = NULL;

    BOXING_ASSERT(toc_data_file1 == NULL);

    afs_toc_file * toc_data_file2 = afs_toc_file_get_new_reference(toc_data_file1);

    BOXING_ASSERT(toc_data_file2 == NULL);

    toc_data_file1 = afs_toc_file_create();

    test_empty_afs_toc_file(toc_data_file1);
    BOXING_ASSERT(toc_data_file1->reference_count == 1);
    BOXING_ASSERT(toc_data_file2 == NULL);

    afs_toc_file_free(toc_data_file1);
}
END_TEST


// Test function afs_toc_file_get_new_reference with empty input afs_toc_file instance
BOXING_START_TEST(afs_toc_file_get_new_reference_test2)
{
    afs_toc_file * toc_data_file1 = afs_toc_file_create();

    test_empty_afs_toc_file(toc_data_file1);
    BOXING_ASSERT(toc_data_file1->reference_count == 1);

    afs_toc_file * toc_data_file2 = afs_toc_file_get_new_reference(toc_data_file1);

    test_empty_afs_toc_file(toc_data_file2);
    BOXING_ASSERT(toc_data_file1->reference_count == 2);
    BOXING_ASSERT(toc_data_file2->reference_count == 2);
    BOXING_ASSERT(toc_data_file1 == toc_data_file2);

    toc_data_file1->start_byte = 99;

    BOXING_ASSERT(toc_data_file2->start_byte == 99);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_get_new_reference with normal input afs_toc_file instance
BOXING_START_TEST(afs_toc_file_get_new_reference_test3)
{
    afs_toc_file * toc_data_file1 = get_afs_toc_file_instance();

    test_not_empty_afs_toc_file(toc_data_file1);
    BOXING_ASSERT(toc_data_file1->reference_count == 1);

    afs_toc_file * toc_data_file2 = afs_toc_file_get_new_reference(toc_data_file1);

    test_not_empty_afs_toc_file(toc_data_file2);
    BOXING_ASSERT(toc_data_file1->reference_count == 2);
    BOXING_ASSERT(toc_data_file2->reference_count == 2);
    BOXING_ASSERT(toc_data_file1 == toc_data_file2);

    toc_data_file1->start_byte = 999;

    BOXING_ASSERT(toc_data_file2->start_byte == 999);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_equal_test1)
{
    afs_toc_file* toc_data_file1 = NULL;
    afs_toc_file* toc_data_file2 = NULL;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);
}
END_TEST

// Test function afs_toc_file_equal test with empty input afs_toc_file pointers 
BOXING_START_TEST(afs_toc_file_equal_test2)
{
    afs_toc_file* toc_data_file1 = afs_toc_file_create();
    test_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = afs_toc_file_create();
    test_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test with normal input afs_toc_file pointers 
BOXING_START_TEST(afs_toc_file_equal_test3)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test with empty input afs_toc_file pointer and pointer equal to NULL 
BOXING_START_TEST(afs_toc_file_equal_test4)
{
    afs_toc_file* toc_data_file1 = afs_toc_file_create();
    test_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = NULL;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
}
END_TEST


// Test function afs_toc_file_equal test with normal input afs_toc_file pointer and pointer equal to NULL
BOXING_START_TEST(afs_toc_file_equal_test5)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = NULL;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
}
END_TEST


// Test function afs_toc_file_equal test with normal afs_toc_file pointer and empty afs_toc_file pointer
BOXING_START_TEST(afs_toc_file_equal_test6)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = afs_toc_file_create();
    test_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different name
BOXING_START_TEST(afs_toc_file_equal_test7)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_set_name(toc_data_file2, "some other name");
    
    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different date
BOXING_START_TEST(afs_toc_file_equal_test8)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_set_date(toc_data_file2, "some other date");

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different checksum
BOXING_START_TEST(afs_toc_file_equal_test9)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_set_checksum(toc_data_file2, "some other checksum");

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different size
BOXING_START_TEST(afs_toc_file_equal_test10)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    toc_data_file2->size = 123;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different id
BOXING_START_TEST(afs_toc_file_equal_test11)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    toc_data_file2->id = 123;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different start frame
BOXING_START_TEST(afs_toc_file_equal_test12)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    toc_data_file2->start_frame = 123;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different start byte
BOXING_START_TEST(afs_toc_file_equal_test13)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    toc_data_file2->start_byte = 123;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different end frame
BOXING_START_TEST(afs_toc_file_equal_test14)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    toc_data_file2->end_frame = 123;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different end byte
BOXING_START_TEST(afs_toc_file_equal_test15)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    toc_data_file2->end_byte = 123;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different unique id
BOXING_START_TEST(afs_toc_file_equal_test16)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_set_unique_id(toc_data_file2, "some other unique id");

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different parent id
BOXING_START_TEST(afs_toc_file_equal_test17)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    toc_data_file2->parent_id = 123;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different types
BOXING_START_TEST(afs_toc_file_equal_test18)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    toc_data_file2->types = AFS_TOC_FILE_TYPE_DIGITAL;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different preview instances
BOXING_START_TEST(afs_toc_file_equal_test19)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    toc_data_file2->preview->dpi = 123;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different file format
BOXING_START_TEST(afs_toc_file_equal_test20)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_set_file_format(toc_data_file2, "some other file format");

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_equal test to afs_toc_file instances with different metadata instances
BOXING_START_TEST(afs_toc_file_equal_test21)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    GVECTORN(toc_data_file2->metadata->sources, afs_toc_data_file_metadata_source *, 1)->file_id = 10;

    test_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal test with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_not_equal_test1)
{
    afs_toc_file* toc_data_file1 = NULL;
    afs_toc_file* toc_data_file2 = NULL;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);
}
END_TEST

// Test function afs_toc_file_not_equal test with empty input afs_toc_file pointers 
BOXING_START_TEST(afs_toc_file_not_equal_test2)
{
    afs_toc_file* toc_data_file1 = afs_toc_file_create();
    test_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = afs_toc_file_create();
    test_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal test with normal input afs_toc_file pointers 
BOXING_START_TEST(afs_toc_file_not_equal_test3)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal test with empty input afs_toc_file pointer and pointer equal to NULL 
BOXING_START_TEST(afs_toc_file_not_equal_test4)
{
    afs_toc_file* toc_data_file1 = afs_toc_file_create();
    test_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = NULL;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
}
END_TEST


// Test function afs_toc_file_not_equal test with normal input afs_toc_file pointer and pointer equal to NULL
BOXING_START_TEST(afs_toc_file_not_equal_test5)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = NULL;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
}
END_TEST


// Test function afs_toc_file_not_equal test with normal afs_toc_file pointer and empty afs_toc_file pointer
BOXING_START_TEST(afs_toc_file_not_equal_test6)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = afs_toc_file_create();
    test_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different name
BOXING_START_TEST(afs_toc_file_not_equal_test7)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_set_name(toc_data_file2, "some other name");

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different date
BOXING_START_TEST(afs_toc_file_not_equal_test8)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_set_date(toc_data_file2, "some other date");

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different checksum
BOXING_START_TEST(afs_toc_file_not_equal_test9)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_set_checksum(toc_data_file2, "some other checksum");

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different size
BOXING_START_TEST(afs_toc_file_not_equal_test10)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    toc_data_file2->size = 123;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different id
BOXING_START_TEST(afs_toc_file_not_equal_test11)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    toc_data_file2->id = 123;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different start frame
BOXING_START_TEST(afs_toc_file_not_equal_test12)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    toc_data_file2->start_frame = 123;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different start byte
BOXING_START_TEST(afs_toc_file_not_equal_test13)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    toc_data_file2->start_byte = 123;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different end frame
BOXING_START_TEST(afs_toc_file_not_equal_test14)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    toc_data_file2->end_frame = 123;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different end byte
BOXING_START_TEST(afs_toc_file_not_equal_test15)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    toc_data_file2->end_byte = 123;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different unique id
BOXING_START_TEST(afs_toc_file_not_equal_test16)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_set_unique_id(toc_data_file2, "some other unique id");

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different parent id
BOXING_START_TEST(afs_toc_file_not_equal_test17)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    toc_data_file2->parent_id = 123;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different types
BOXING_START_TEST(afs_toc_file_not_equal_test18)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    toc_data_file2->types = AFS_TOC_FILE_TYPE_DIGITAL;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different preview instances
BOXING_START_TEST(afs_toc_file_not_equal_test19)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    toc_data_file2->preview->dpi = 123;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different file format
BOXING_START_TEST(afs_toc_file_not_equal_test20)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    afs_toc_file_set_file_format(toc_data_file2, "some other file format");

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_not_equal with afs_toc_file instances with different metadata instances
BOXING_START_TEST(afs_toc_file_not_equal_test21)
{
    afs_toc_file* toc_data_file1 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file1);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file(toc_data_file2);

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DFALSE);

    GVECTORN(toc_data_file2->metadata->sources, afs_toc_data_file_metadata_source *, 1)->file_id = 10;

    test_not_equals_toc_file(toc_data_file1, toc_data_file2, DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_add_frame_offset test with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_add_frame_offset_test1)
{
    afs_toc_file* toc_data_file = NULL;

    afs_toc_file_add_frame_offset(toc_data_file, 5);

    BOXING_ASSERT(toc_data_file == NULL);
}
END_TEST


// Test function afs_toc_file_add_frame_offset test with empty input afs_toc_file instance
BOXING_START_TEST(afs_toc_file_add_frame_offset_test2)
{
    afs_toc_file* toc_data_file = afs_toc_file_create();

    test_empty_afs_toc_file(toc_data_file);

    afs_toc_file_add_frame_offset(toc_data_file, 5);

    test_empty_afs_toc_file(toc_data_file);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_add_frame_offset test with not empty input afs_toc_file instance but with incorrect frame data
BOXING_START_TEST(afs_toc_file_add_frame_offset_test3)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    test_not_empty_afs_toc_file(toc_data_file);

    toc_data_file->start_frame = -1;
    toc_data_file->end_frame = -1;

    afs_toc_file_add_frame_offset(toc_data_file, 5);

    BOXING_ASSERT(toc_data_file->start_frame == -1);
    BOXING_ASSERT(toc_data_file->end_frame == -1);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_add_frame_offset test with not empty input afs_toc_file instance
BOXING_START_TEST(afs_toc_file_add_frame_offset_test4)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    test_not_empty_afs_toc_file(toc_data_file);

    afs_toc_file_add_frame_offset(toc_data_file, 5);

    BOXING_ASSERT(toc_data_file->start_frame == 7);
    BOXING_ASSERT(toc_data_file->end_frame == 7);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_add_type test with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_add_type_test1)
{
    afs_toc_file* toc_data_file = NULL;

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_UNDEFINED);

    BOXING_ASSERT(toc_data_file == NULL);
}
END_TEST


// Test function afs_toc_file_add_type test with add AFS_TOC_FILE_TYPE_UNDEFINED type
BOXING_START_TEST(afs_toc_file_add_type_test2)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_UNDEFINED);

    test_not_empty_afs_toc_file(toc_data_file);

    toc_data_file->types = 0x1;

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_UNDEFINED);

    BOXING_ASSERT(toc_data_file->types == 0x1);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_add_type test with add AFS_TOC_FILE_TYPE_DIGITAL and AFS_TOC_FILE_TYPE_PREVIEW types
BOXING_START_TEST(afs_toc_file_add_type_test3)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    test_not_empty_afs_toc_file(toc_data_file);
    toc_data_file->types = AFS_TOC_FILE_TYPE_UNDEFINED;

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_DIGITAL);

    BOXING_ASSERT(toc_data_file->types == 0x1);

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_DIGITAL);

    BOXING_ASSERT(toc_data_file->types == 0x1);

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_PREVIEW);

    BOXING_ASSERT(toc_data_file->types == 0x3);

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_PREVIEW);

    BOXING_ASSERT(toc_data_file->types == 0x3);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_add_type test with add AFS_TOC_FILE_TYPE_DIGITAL and AFS_TOC_FILE_TYPE_PREVIEW types
BOXING_START_TEST(afs_toc_file_add_type_test4)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    test_not_empty_afs_toc_file(toc_data_file);
    toc_data_file->types = AFS_TOC_FILE_TYPE_UNDEFINED;

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_PREVIEW);

    BOXING_ASSERT(toc_data_file->types == 0x2);

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_PREVIEW);

    BOXING_ASSERT(toc_data_file->types == 0x2);

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_DIGITAL);

    BOXING_ASSERT(toc_data_file->types == 0x3);

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_DIGITAL);

    BOXING_ASSERT(toc_data_file->types == 0x3);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_name with input afs_toc_file pointers equal to NULL and name string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_name_test1)
{
    afs_toc_file * toc_data_file = NULL;
    const char * name = NULL;

    afs_toc_file_set_name(toc_data_file, name);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(name == NULL);
}
END_TEST


// Test function afs_toc_file_set_name with input afs_toc_file pointers equal to NULL and empty name string
BOXING_START_TEST(afs_toc_file_set_name_test2)
{
    afs_toc_file * toc_data_file = NULL;
    const char * name = "";

    afs_toc_file_set_name(toc_data_file, name);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(strcmp(name, "") == 0);
}
END_TEST


// Test function afs_toc_file_set_name with input afs_toc_file pointers equal to NULL and not empty name string
BOXING_START_TEST(afs_toc_file_set_name_test3)
{
    afs_toc_file * toc_data_file = NULL;
    const char * name = "some name";

    afs_toc_file_set_name(toc_data_file, name);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(strcmp(name, "some name") == 0);
}
END_TEST


// Test function afs_toc_file_set_name with empty input afs_toc_file instance and name string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_name_test4)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    const char * name = NULL;

    afs_toc_file_set_name(toc_data_file, name);

    BOXING_ASSERT(toc_data_file->name == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_name with empty input afs_toc_file instance and empty name string
BOXING_START_TEST(afs_toc_file_set_name_test5)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->name == NULL);

    const char * name = "";

    afs_toc_file_set_name(toc_data_file, name);

    BOXING_ASSERT(strcmp(toc_data_file->name, "") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_name with empty input afs_toc_file instance and not empty name string
BOXING_START_TEST(afs_toc_file_set_name_test6)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->name == NULL);

    const char * name = "some name";

    afs_toc_file_set_name(toc_data_file, name);

    BOXING_ASSERT(strcmp(toc_data_file->name, "some name") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_name with not empty input afs_toc_file instance and name string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_name_test7)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->name, "input-toc.xml") == 0);

    const char * name = NULL;

    afs_toc_file_set_name(toc_data_file, name);

    BOXING_ASSERT(toc_data_file->name == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_name with not empty input afs_toc_file instance and empty name string
BOXING_START_TEST(afs_toc_file_set_name_test8)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->name, "input-toc.xml") == 0);

    const char * name = "";

    afs_toc_file_set_name(toc_data_file, name);

    BOXING_ASSERT(strcmp(toc_data_file->name, "") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_name with not empty input afs_toc_file instance and not empty name string
BOXING_START_TEST(afs_toc_file_set_name_test9)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->name, "input-toc.xml") == 0);

    const char * name = "some name";

    afs_toc_file_set_name(toc_data_file, name);

    BOXING_ASSERT(strcmp(toc_data_file->name, "some name") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_date with input afs_toc_file pointers equal to NULL and date string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_date_test1)
{
    afs_toc_file * toc_data_file = NULL;
    const char * date = NULL;

    afs_toc_file_set_date(toc_data_file, date);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(date == NULL);
}
END_TEST


// Test function afs_toc_file_set_date with input afs_toc_file pointers equal to NULL and empty date string
BOXING_START_TEST(afs_toc_file_set_date_test2)
{
    afs_toc_file * toc_data_file = NULL;
    const char * date = "";

    afs_toc_file_set_date(toc_data_file, date);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(strcmp(date, "") == 0);
}
END_TEST


// Test function afs_toc_file_set_date with input afs_toc_file pointers equal to NULL and not empty date string
BOXING_START_TEST(afs_toc_file_set_date_test3)
{
    afs_toc_file * toc_data_file = NULL;
    const char * date = "some date";

    afs_toc_file_set_date(toc_data_file, date);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(strcmp(date, "some date") == 0);
}
END_TEST


// Test function afs_toc_file_set_date with empty input afs_toc_file instance and date string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_date_test4)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    const char * date = NULL;

    afs_toc_file_set_date(toc_data_file, date);

    BOXING_ASSERT(toc_data_file->date == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_date with empty input afs_toc_file instance and empty date string
BOXING_START_TEST(afs_toc_file_set_date_test5)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->date == NULL);

    const char * date = "";

    afs_toc_file_set_date(toc_data_file, date);

    BOXING_ASSERT(strcmp(toc_data_file->date, "") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_date with empty input afs_toc_file instance and not empty date string
BOXING_START_TEST(afs_toc_file_set_date_test6)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->date == NULL);

    const char * date = "some date";

    afs_toc_file_set_date(toc_data_file, date);

    BOXING_ASSERT(strcmp(toc_data_file->date, "some date") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_date with not empty input afs_toc_file instance and date string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_date_test7)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->date, "2016-11-22 09:07:11") == 0);

    const char * date = NULL;

    afs_toc_file_set_date(toc_data_file, date);

    BOXING_ASSERT(toc_data_file->date == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_date with not empty input afs_toc_file instance and empty date string
BOXING_START_TEST(afs_toc_file_set_date_test8)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->date, "2016-11-22 09:07:11") == 0);

    const char * date = "";

    afs_toc_file_set_date(toc_data_file, date);

    BOXING_ASSERT(strcmp(toc_data_file->date, "") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_date with not empty input afs_toc_file instance and not empty date string
BOXING_START_TEST(afs_toc_file_set_date_test9)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->date, "2016-11-22 09:07:11") == 0);

    const char * date = "some date";

    afs_toc_file_set_date(toc_data_file, date);

    BOXING_ASSERT(strcmp(toc_data_file->date, "some date") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_checksum with input afs_toc_file pointers equal to NULL and checksum string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_checksum_test1)
{
    afs_toc_file * toc_data_file = NULL;
    const char * checksum = NULL;

    afs_toc_file_set_checksum(toc_data_file, checksum);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(checksum == NULL);
}
END_TEST


// Test function afs_toc_file_set_checksum with input afs_toc_file pointers equal to NULL and empty checksum string
BOXING_START_TEST(afs_toc_file_set_checksum_test2)
{
    afs_toc_file * toc_data_file = NULL;
    const char * checksum = "";

    afs_toc_file_set_checksum(toc_data_file, checksum);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(strcmp(checksum, "") == 0);
}
END_TEST


// Test function afs_toc_file_set_checksum with input afs_toc_file pointers equal to NULL and not empty checksum string
BOXING_START_TEST(afs_toc_file_set_checksum_test3)
{
    afs_toc_file * toc_data_file = NULL;
    const char * checksum = "some checksum";

    afs_toc_file_set_checksum(toc_data_file, checksum);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(strcmp(checksum, "some checksum") == 0);
}
END_TEST


// Test function afs_toc_file_set_checksum with empty input afs_toc_file instance and checksum string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_checksum_test4)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    const char * checksum = NULL;

    afs_toc_file_set_checksum(toc_data_file, checksum);

    BOXING_ASSERT(toc_data_file->checksum == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_checksum with empty input afs_toc_file instance and empty checksum string
BOXING_START_TEST(afs_toc_file_set_checksum_test5)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->checksum == NULL);

    const char * checksum = "";

    afs_toc_file_set_checksum(toc_data_file, checksum);

    BOXING_ASSERT(strcmp(toc_data_file->checksum, "") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_checksum with empty input afs_toc_file instance and not empty checksum string
BOXING_START_TEST(afs_toc_file_set_checksum_test6)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->checksum == NULL);

    const char * checksum = "some checksum";

    afs_toc_file_set_checksum(toc_data_file, checksum);

    BOXING_ASSERT(strcmp(toc_data_file->checksum, "some checksum") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_checksum with not empty input afs_toc_file instance and checksum string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_checksum_test7)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->checksum, "f2eb0d6c6964d206012695a10ea6cd877c95c340") == 0);

    const char * checksum = NULL;

    afs_toc_file_set_checksum(toc_data_file, checksum);

    BOXING_ASSERT(toc_data_file->checksum == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_checksum with not empty input afs_toc_file instance and empty checksum string
BOXING_START_TEST(afs_toc_file_set_checksum_test8)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->checksum, "f2eb0d6c6964d206012695a10ea6cd877c95c340") == 0);

    const char * checksum = "";

    afs_toc_file_set_checksum(toc_data_file, checksum);

    BOXING_ASSERT(strcmp(toc_data_file->checksum, "") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_checksum with not empty input afs_toc_file instance and not empty checksum string
BOXING_START_TEST(afs_toc_file_set_checksum_test9)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->checksum, "f2eb0d6c6964d206012695a10ea6cd877c95c340") == 0);

    const char * checksum = "some checksum";

    afs_toc_file_set_checksum(toc_data_file, checksum);

    BOXING_ASSERT(strcmp(toc_data_file->checksum, "some checksum") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_unique_id with input afs_toc_file pointers equal to NULL and unique id string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_unique_id_test1)
{
    afs_toc_file * toc_data_file = NULL;
    const char * unique_id = NULL;

    afs_toc_file_set_unique_id(toc_data_file, unique_id);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(unique_id == NULL);
}
END_TEST


// Test function afs_toc_file_set_unique_id with input afs_toc_file pointers equal to NULL and empty unique id string
BOXING_START_TEST(afs_toc_file_set_unique_id_test2)
{
    afs_toc_file * toc_data_file = NULL;
    const char * unique_id = "";

    afs_toc_file_set_unique_id(toc_data_file, unique_id);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(strcmp(unique_id, "") == 0);
}
END_TEST


// Test function afs_toc_file_set_unique_id with input afs_toc_file pointers equal to NULL and not empty unique id string
BOXING_START_TEST(afs_toc_file_set_unique_id_test3)
{
    afs_toc_file * toc_data_file = NULL;
    const char * unique_id = "some unique id";

    afs_toc_file_set_unique_id(toc_data_file, unique_id);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(strcmp(unique_id, "some unique id") == 0);
}
END_TEST


// Test function afs_toc_file_set_unique_id with empty input afs_toc_file instance and unique id string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_unique_id_test4)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    const char * unique_id = NULL;

    afs_toc_file_set_unique_id(toc_data_file, unique_id);

    BOXING_ASSERT(toc_data_file->unique_id == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_unique_id with empty input afs_toc_file instance and empty unique id string
BOXING_START_TEST(afs_toc_file_set_unique_id_test5)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->unique_id == NULL);

    const char * unique_id = "";

    afs_toc_file_set_unique_id(toc_data_file, unique_id);

    BOXING_ASSERT(strcmp(toc_data_file->unique_id, "") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_unique_id with empty input afs_toc_file instance and not empty unique id string
BOXING_START_TEST(afs_toc_file_set_unique_id_test6)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->unique_id == NULL);

    const char * unique_id = "some unique id";

    afs_toc_file_set_unique_id(toc_data_file, unique_id);

    BOXING_ASSERT(strcmp(toc_data_file->unique_id, "some unique id") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_unique_id with not empty input afs_toc_file instance and unique id string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_unique_id_test7)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->unique_id, "38A0AAAB-16B2-640C-5353-6DB8AE4367B9") == 0);

    const char * unique_id = NULL;

    afs_toc_file_set_unique_id(toc_data_file, unique_id);

    BOXING_ASSERT(toc_data_file->unique_id == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_unique_id with not empty input afs_toc_file instance and empty unique id string
BOXING_START_TEST(afs_toc_file_set_unique_id_test8)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->unique_id, "38A0AAAB-16B2-640C-5353-6DB8AE4367B9") == 0);

    const char * unique_id = "";

    afs_toc_file_set_unique_id(toc_data_file, unique_id);

    BOXING_ASSERT(strcmp(toc_data_file->unique_id, "") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_unique_id with not empty input afs_toc_file instance and not empty unique_id string
BOXING_START_TEST(afs_toc_file_set_unique_id_test9)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->unique_id, "38A0AAAB-16B2-640C-5353-6DB8AE4367B9") == 0);

    const char * unique_id = "some unique id";

    afs_toc_file_set_unique_id(toc_data_file, unique_id);

    BOXING_ASSERT(strcmp(toc_data_file->unique_id, "some unique id") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_file_format with input afs_toc_file pointers equal to NULL and file format string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_file_format_test1)
{
    afs_toc_file * toc_data_file = NULL;
    const char * file_format = NULL;

    afs_toc_file_set_file_format(toc_data_file, file_format);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(file_format == NULL);
}
END_TEST


// Test function afs_toc_file_set_file_format with input afs_toc_file pointers equal to NULL and empty file format string
BOXING_START_TEST(afs_toc_file_set_file_format_test2)
{
    afs_toc_file * toc_data_file = NULL;
    const char * file_format = "";

    afs_toc_file_set_file_format(toc_data_file, file_format);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(strcmp(file_format, "") == 0);
}
END_TEST


// Test function afs_toc_file_set_file_format with input afs_toc_file pointers equal to NULL and not empty file format string
BOXING_START_TEST(afs_toc_file_set_file_format_test3)
{
    afs_toc_file * toc_data_file = NULL;
    const char * file_format = "some file format";

    afs_toc_file_set_file_format(toc_data_file, file_format);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(strcmp(file_format, "some file format") == 0);
}
END_TEST


// Test function afs_toc_file_set_file_format with empty input afs_toc_file instance and file format string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_file_format_test4)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    const char * file_format = NULL;

    afs_toc_file_set_file_format(toc_data_file, file_format);

    BOXING_ASSERT(toc_data_file->file_format == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_file_format with empty input afs_toc_file instance and empty file format string
BOXING_START_TEST(afs_toc_file_set_file_format_test5)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->file_format == NULL);

    const char * file_format = "";

    afs_toc_file_set_file_format(toc_data_file, file_format);

    BOXING_ASSERT(strcmp(toc_data_file->file_format, "") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_file_format with empty input afs_toc_file instance and not empty file format string
BOXING_START_TEST(afs_toc_file_set_file_format_test6)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->file_format == NULL);

    const char * file_format = "some file format";

    afs_toc_file_set_file_format(toc_data_file, file_format);

    BOXING_ASSERT(strcmp(toc_data_file->file_format, "some file format") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_file_format with not empty input afs_toc_file instance and file format string pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_file_format_test7)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->file_format, "file_format") == 0);

    const char * file_format = NULL;

    afs_toc_file_set_file_format(toc_data_file, file_format);

    BOXING_ASSERT(toc_data_file->file_format == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_file_format with not empty input afs_toc_file instance and empty file format string
BOXING_START_TEST(afs_toc_file_set_file_format_test8)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->file_format, "file_format") == 0);

    const char * file_format = "";

    afs_toc_file_set_file_format(toc_data_file, file_format);

    BOXING_ASSERT(strcmp(toc_data_file->file_format, "") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_file_format with not empty input afs_toc_file instance and not empty file format string
BOXING_START_TEST(afs_toc_file_set_file_format_test9)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    BOXING_ASSERT(strcmp(toc_data_file->file_format, "file_format") == 0);

    const char * file_format = "some file format";

    afs_toc_file_set_file_format(toc_data_file, file_format);

    BOXING_ASSERT(strcmp(toc_data_file->file_format, "some file format") == 0);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_preview with input afs_toc_file pointers equal to NULL and preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_preview_test1)
{
    afs_toc_file * toc_data_file = NULL;
    const afs_toc_file_preview * toc_file_preview = NULL;

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
}
END_TEST


// Test function afs_toc_file_set_preview with input afs_toc_file pointers equal to NULL and empty preview instance
BOXING_START_TEST(afs_toc_file_set_preview_test2)
{
    afs_toc_file * toc_data_file = NULL;
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(toc_file_preview->pages == NULL);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_set_preview with input afs_toc_file pointers equal to NULL and not valid preview instance
BOXING_START_TEST(afs_toc_file_set_preview_test3)
{
    afs_toc_file * toc_data_file = NULL;
    afs_toc_file_preview_pages * pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_x = 30;
    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_y = 50;

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_x == 30);
    BOXING_ASSERT(GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_y == 50);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_set_preview with input afs_toc_file pointers equal to NULL and valid preview instance
BOXING_START_TEST(afs_toc_file_set_preview_test4)
{
    afs_toc_file * toc_data_file = NULL;
    afs_toc_file_preview_pages * pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    BOXING_ASSERT(toc_data_file == NULL);
    test_not_empty_afs_toc_file_preview3(toc_file_preview, 3);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_set_preview with empty input afs_toc_file instance and preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_preview_test5)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    afs_toc_file_preview * toc_file_preview = NULL;

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    BOXING_ASSERT(toc_data_file->preview == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_preview with empty input afs_toc_file instance and empty preview instance
BOXING_START_TEST(afs_toc_file_set_preview_test6)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->preview == NULL);

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    BOXING_ASSERT(toc_data_file->preview != NULL);
    BOXING_ASSERT(toc_data_file->preview->pages == NULL);

    afs_toc_file_free(toc_data_file);
    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_set_preview with empty input afs_toc_file instance and not valid preview instance
BOXING_START_TEST(afs_toc_file_set_preview_test7)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->preview == NULL);

    afs_toc_file_preview_pages * pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_x = 30;
    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_y = 50;

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    BOXING_ASSERT(GVECTORN(toc_data_file->preview->pages, afs_toc_file_preview_page *, 1)->overlap_x == 30);
    BOXING_ASSERT(GVECTORN(toc_data_file->preview->pages, afs_toc_file_preview_page *, 1)->overlap_y == 50);

    afs_toc_file_free(toc_data_file);
    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_set_preview with empty input afs_toc_file instance and valid preview instance
BOXING_START_TEST(afs_toc_file_set_preview_test8)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->preview == NULL);

    afs_toc_file_preview_pages * pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    test_not_empty_afs_toc_file_preview3(toc_data_file->preview, 3);

    afs_toc_file_free(toc_data_file);
    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_set_preview with not empty input afs_toc_file instance and preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_preview_test9)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file_preview3(toc_data_file->preview, 3);

    afs_toc_file_preview * toc_file_preview = NULL;

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    BOXING_ASSERT(toc_data_file->preview == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_preview with not empty input afs_toc_file instance and empty preview instance
BOXING_START_TEST(afs_toc_file_set_preview_test10)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file_preview3(toc_data_file->preview, 3);

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    BOXING_ASSERT(toc_data_file->preview != NULL);
    BOXING_ASSERT(toc_data_file->preview->pages == NULL);

    afs_toc_file_free(toc_data_file);
    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_set_preview with not empty input afs_toc_file instance and not valid preview instance
BOXING_START_TEST(afs_toc_file_set_preview_test11)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file_preview3(toc_data_file->preview, 3);

    afs_toc_file_preview_pages * pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_x = 30;
    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_y = 50;

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    BOXING_ASSERT(GVECTORN(toc_data_file->preview->pages, afs_toc_file_preview_page *, 1)->overlap_x == 30);
    BOXING_ASSERT(GVECTORN(toc_data_file->preview->pages, afs_toc_file_preview_page *, 1)->overlap_y == 50);

    afs_toc_file_free(toc_data_file);
    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_set_preview with not empty input afs_toc_file instance and valid preview instance
BOXING_START_TEST(afs_toc_file_set_preview_test12)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    test_not_empty_afs_toc_file_preview3(toc_data_file->preview, 3);

    afs_toc_file_preview_pages * pages = get_filled_pages_vector3(5);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_file_set_preview(toc_data_file, toc_file_preview);

    test_not_empty_afs_toc_file_preview3(toc_data_file->preview, 5);

    afs_toc_file_free(toc_data_file);
    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_set_metadata with input afs_toc_file pointers equal to NULL and metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_metadata_test1)
{
    afs_toc_file * toc_data_file = NULL;
    afs_toc_data_file_metadata * toc_data_file_metadata = NULL;

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(toc_data_file_metadata == NULL);
}
END_TEST


// Test function afs_toc_file_set_metadata with input afs_toc_file pointers equal to NULL and empty metadata instance
BOXING_START_TEST(afs_toc_file_set_metadata_test2)
{
    afs_toc_file * toc_data_file = NULL;
    afs_toc_data_file_metadata * toc_data_file_metadata = afs_toc_data_file_metadata_create();

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    BOXING_ASSERT(toc_data_file == NULL);
    test_null_afs_toc_data_file_metadata2(toc_data_file_metadata);

    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_set_metadata with input afs_toc_file pointers equal to NULL and not valid metadata instance
BOXING_START_TEST(afs_toc_file_set_metadata_test3)
{
    afs_toc_file * toc_data_file = NULL;
    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(3);

    free(GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->data);
    GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->data = NULL;
    GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->file_id = -1;

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->data == NULL);
    BOXING_ASSERT(GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->file_id == -1);

    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_set_metadata with input afs_toc_file pointers equal to NULL and valid metadata instance
BOXING_START_TEST(afs_toc_file_set_metadata_test4)
{
    afs_toc_file * toc_data_file = NULL;
    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(3);

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    BOXING_ASSERT(toc_data_file == NULL);
    test_not_empty_afs_toc_data_file_metadata2(toc_data_file_metadata, 3);

    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_set_metadata with empty input afs_toc_file instance and metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_metadata_test5)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    afs_toc_data_file_metadata * toc_data_file_metadata = NULL;

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    BOXING_ASSERT(toc_data_file->metadata == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_metadata with empty input afs_toc_file instance and empty metadata instance
BOXING_START_TEST(afs_toc_file_set_metadata_test6)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->metadata == NULL);

    afs_toc_data_file_metadata * toc_data_file_metadata = afs_toc_data_file_metadata_create();

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    test_null_afs_toc_data_file_metadata2(toc_data_file->metadata);

    afs_toc_file_free(toc_data_file);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_set_metadata with empty input afs_toc_file instance and not valid metadata instance
BOXING_START_TEST(afs_toc_file_set_metadata_test7)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->metadata == NULL);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(3);

    free(GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->data);
    GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->data = NULL;
    GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->file_id = -1;

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    BOXING_ASSERT(GVECTORN(toc_data_file->metadata->sources, afs_toc_data_file_metadata_source *, 1)->data == NULL);
    BOXING_ASSERT(GVECTORN(toc_data_file->metadata->sources, afs_toc_data_file_metadata_source *, 1)->file_id == -1);

    afs_toc_file_free(toc_data_file);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_set_metadata with empty input afs_toc_file instance and valid metadata instance
BOXING_START_TEST(afs_toc_file_set_metadata_test8)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();
    BOXING_ASSERT(toc_data_file->metadata == NULL);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(3);

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    test_not_empty_afs_toc_data_file_metadata2(toc_data_file->metadata, 3);

    afs_toc_file_free(toc_data_file);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_set_metadata with not empty input afs_toc_file instance and metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_file_set_metadata_test9)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    test_not_empty_afs_toc_data_file_metadata2(toc_data_file->metadata, 5);

    afs_toc_data_file_metadata * toc_data_file_metadata = NULL;

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    BOXING_ASSERT(toc_data_file->metadata == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_set_metadata with not empty input afs_toc_file instance and empty metadata instance
BOXING_START_TEST(afs_toc_file_set_metadata_test10)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    test_not_empty_afs_toc_data_file_metadata2(toc_data_file->metadata, 5);

    afs_toc_data_file_metadata * toc_data_file_metadata = afs_toc_data_file_metadata_create();

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    test_null_afs_toc_data_file_metadata2(toc_data_file->metadata);

    afs_toc_file_free(toc_data_file);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_set_metadata with not empty input afs_toc_file instance and not valid metadata instance
BOXING_START_TEST(afs_toc_file_set_metadata_test11)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    test_not_empty_afs_toc_data_file_metadata2(toc_data_file->metadata, 5);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(3);

    free(GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->data);
    GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->data = NULL;
    GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->file_id = -1;

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    BOXING_ASSERT(GVECTORN(toc_data_file->metadata->sources, afs_toc_data_file_metadata_source *, 1)->data == NULL);
    BOXING_ASSERT(GVECTORN(toc_data_file->metadata->sources, afs_toc_data_file_metadata_source *, 1)->file_id == -1);

    afs_toc_file_free(toc_data_file);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_set_metadata with not empty input afs_toc_file instance and valid metadata instance
BOXING_START_TEST(afs_toc_file_set_metadata_test12)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    test_not_empty_afs_toc_data_file_metadata2(toc_data_file->metadata, 5);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(6);

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    test_not_empty_afs_toc_data_file_metadata2(toc_data_file->metadata, 6);

    afs_toc_file_free(toc_data_file);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_is_parent test with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_is_parent_test1)
{
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_is_parent(toc_data_file);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_is_parent test with parent_id value equal to AFS_TOC_FILE_NO_PARENT
BOXING_START_TEST(afs_toc_file_is_parent_test2)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    DBOOL result = afs_toc_file_is_parent(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_parent test with parent_id value equal to AFS_TOC_FILE_PARENT
BOXING_START_TEST(afs_toc_file_is_parent_test3)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->parent_id = AFS_TOC_FILE_PARENT;

    DBOOL result = afs_toc_file_is_parent(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_has_parent test with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_has_parent_test1)
{
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_has_parent(toc_data_file);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_has_parent test with parent_id value equal to AFS_TOC_FILE_NO_PARENT
BOXING_START_TEST(afs_toc_file_has_parent_test2)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    DBOOL result = afs_toc_file_has_parent(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_has_parent test with parent_id value equal to AFS_TOC_FILE_PARENT
BOXING_START_TEST(afs_toc_file_has_parent_test3)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->parent_id = AFS_TOC_FILE_PARENT;

    DBOOL result = afs_toc_file_has_parent(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_has_parent test with parent_id value not equal to AFS_TOC_FILE_NO_PARENT and not equal to AFS_TOC_FILE_PARENT
BOXING_START_TEST(afs_toc_file_has_parent_test4)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->parent_id = -3;

    DBOOL result = afs_toc_file_has_parent(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_standalone test with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_is_standalone_test1)
{
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_is_standalone(toc_data_file);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_is_standalone test with parent_id value equal to AFS_TOC_FILE_PARENT
BOXING_START_TEST(afs_toc_file_is_standalone_test2)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->parent_id = AFS_TOC_FILE_PARENT;

    DBOOL result = afs_toc_file_is_standalone(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_standalone test with parent_id value not equal to AFS_TOC_FILE_NO_PARENT and not equal to AFS_TOC_FILE_PARENT
BOXING_START_TEST(afs_toc_file_is_standalone_test3)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->parent_id = -3;

    DBOOL result = afs_toc_file_is_standalone(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_standalone test with parent_id value not equal to AFS_TOC_FILE_NO_PARENT
BOXING_START_TEST(afs_toc_file_is_standalone_test4)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->parent_id = AFS_TOC_FILE_NO_PARENT;

    DBOOL result = afs_toc_file_is_standalone(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_digital test with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_is_digital_test1)
{
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_is_digital(toc_data_file);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_is_digital test with with types value equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_file_is_digital_test2)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    toc_data_file->types = AFS_TOC_FILE_TYPE_UNDEFINED;

    DBOOL result = afs_toc_file_is_digital(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_digital test with with types value equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_file_is_digital_test3)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;

    DBOOL result = afs_toc_file_is_digital(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_digital test with with types value equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_file_is_digital_test4)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_PREVIEW;

    DBOOL result = afs_toc_file_is_digital(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_digital test with with types value equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_file_is_digital_test5)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_ALL;

    DBOOL result = afs_toc_file_is_digital(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_preview test with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_is_preview_test1)
{
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_is_preview(toc_data_file);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_is_preview test with with types value equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_file_is_preview_test2)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();
    toc_data_file->types = AFS_TOC_FILE_TYPE_UNDEFINED;

    DBOOL result = afs_toc_file_is_preview(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_preview test with with types value equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_file_is_preview_test3)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;

    DBOOL result = afs_toc_file_is_preview(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_preview test with with types value equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_file_is_preview_test4)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_PREVIEW;

    DBOOL result = afs_toc_file_is_preview(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_preview test with with types value equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_file_is_preview_test5)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_ALL;

    DBOOL result = afs_toc_file_is_preview(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_has_metadata with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_has_metadata_test1)
{
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_has_metadata(toc_data_file);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_has_metadata with empty afs_toc_file instance
BOXING_START_TEST(afs_toc_file_has_metadata_test2)
{
    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_has_metadata(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_has_metadata with normal afs_toc_file instance
BOXING_START_TEST(afs_toc_file_has_metadata_test3)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    DBOOL result = afs_toc_file_has_metadata(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_has_metadata with not empty afs_toc_file instance when afs_toc_data_file_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_file_has_metadata_test4)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();
    afs_toc_file_set_metadata(toc_data_file, NULL);

    DBOOL result = afs_toc_file_has_metadata(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_has_metadata with not empty afs_toc_file instance when afs_toc_data_file_metadata instance is empty
BOXING_START_TEST(afs_toc_file_has_metadata_test5)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();
    afs_toc_data_file_metadata * toc_data_file_metadata = afs_toc_data_file_metadata_create();

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    DBOOL result = afs_toc_file_has_metadata(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_has_metadata with not empty afs_toc_file instance when afs_toc_data_file_metadata instance with empty sources vector
BOXING_START_TEST(afs_toc_file_has_metadata_test6)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();
    afs_toc_data_file_metadata_sources * sources = gvector_create_pointers(0);
    afs_toc_data_file_metadata * toc_data_file_metadata = afs_toc_data_file_metadata_create2(sources);

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    DBOOL result = afs_toc_file_has_metadata(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_is_valid test with input afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_file_is_valid_test1)
{
    afs_toc_file * toc_data_file = NULL;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_is_valid test without specifying the type
BOXING_START_TEST(afs_toc_file_is_valid_test2)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    toc_data_file->types = AFS_TOC_FILE_TYPE_UNDEFINED;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_DIGITAL type and with illegal id 
BOXING_START_TEST(afs_toc_file_is_valid_test3)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_data_file->id = -3;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_DIGITAL type and with incorrect name
BOXING_START_TEST(afs_toc_file_is_valid_test4)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    free(toc_data_file->name);
    toc_data_file->name = NULL;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_DIGITAL type and with incorrect date
BOXING_START_TEST(afs_toc_file_is_valid_test5)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    free(toc_data_file->date);
    toc_data_file->date = NULL;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_DIGITAL type and with incorrect start frame
BOXING_START_TEST(afs_toc_file_is_valid_test6)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_data_file->start_frame = -1;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_DIGITAL type and with incorrect start byte
BOXING_START_TEST(afs_toc_file_is_valid_test7)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_data_file->start_byte = -1;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_DIGITAL type and with incorrect end frame
BOXING_START_TEST(afs_toc_file_is_valid_test8)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_data_file->end_frame = -1;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_DIGITAL type and with incorrect end byte
BOXING_START_TEST(afs_toc_file_is_valid_test9)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_data_file->end_byte = -1;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_DIGITAL type and with incorrect checksum
BOXING_START_TEST(afs_toc_file_is_valid_test10)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    free(toc_data_file->checksum);
    toc_data_file->checksum = NULL;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    toc_data_file->checksum = boxing_string_clone("");

    result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_DIGITAL type and with incorrect digital data
BOXING_START_TEST(afs_toc_file_is_valid_test11)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_data_file->start_byte = 30000;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_PREVIEW type and with incorrect preview
BOXING_START_TEST(afs_toc_file_is_valid_test12)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_PREVIEW;
    afs_toc_file_preview_free(toc_data_file->preview);

    afs_toc_file_preview_pages* pages = get_filled_pages_vector3(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_x = 30;
    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_y = 50;
    toc_data_file->preview = toc_file_preview;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_data_file->preview);
    toc_data_file->preview = NULL;

    result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_PREVIEW type and with incorrect metadata
BOXING_START_TEST(afs_toc_file_is_valid_test13)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(3);

    free(GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->data);
    GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->data = NULL;
    GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1)->file_id = -1;

    afs_toc_file_set_metadata(toc_data_file, toc_data_file_metadata);

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_free(toc_data_file->metadata);
    toc_data_file->metadata = NULL;

    result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
    afs_toc_data_file_metadata_free(toc_data_file_metadata);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_DIGITAL type
BOXING_START_TEST(afs_toc_file_is_valid_test14)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    
    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_is_valid test with AFS_TOC_FILE_TYPE_PREVIEW type
BOXING_START_TEST(afs_toc_file_is_valid_test15)
{
    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    toc_data_file->types = AFS_TOC_FILE_TYPE_PREVIEW;

    DBOOL result = afs_toc_file_is_valid(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_get_duration_frames with afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_get_duration_frames_test1)
{
    afs_toc_file * toc_data_file = NULL;

    int result = afs_toc_file_get_duration_frames(toc_data_file);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == 0);
}
END_TEST


// Test function afs_toc_file_get_duration_frames with empty afs_toc_file instance
BOXING_START_TEST(afs_toc_file_get_duration_frames_test2)
{
    afs_toc_file * toc_data_file = afs_toc_file_create();

    int result = afs_toc_file_get_duration_frames(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == 1);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_get_duration_frames with the same start and end frames
BOXING_START_TEST(afs_toc_file_get_duration_frames_test3)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();

    int result = afs_toc_file_get_duration_frames(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == 1);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_get_duration_frames with the end frame number larger than the start frame
BOXING_START_TEST(afs_toc_file_get_duration_frames_test4)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    toc_data_file->end_frame = 5;

    int result = afs_toc_file_get_duration_frames(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == 4);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_get_duration_frames with the start frame number larger than the end frame
BOXING_START_TEST(afs_toc_file_get_duration_frames_test5)
{
    afs_toc_file * toc_data_file = get_afs_toc_file_instance();
    toc_data_file->start_frame = 5;

    int result = afs_toc_file_get_duration_frames(toc_data_file);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == -2);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_save_xml with mxml_node_t pointer equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_save_xml(toc_data_file, out, DFALSE);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_save_xml with mxml_node_t pointer equal to NULL and afs_toc_file pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    DBOOL result = afs_toc_file_save_xml(toc_data_file, out, DFALSE);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_save_xml(toc_data_file, tree, DFALSE);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_save_xml with empty afs_toc_file instance
BOXING_START_TEST(afs_toc_file_save_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_save_xml(toc_data_file, tree, DFALSE);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_save_xml with initialized afs_toc_file instance and use data_v1 type
BOXING_START_TEST(afs_toc_file_save_xml_test5)
{
    mxml_node_t * tree = mxmlNewXML("1.0");

    afs_toc_file * toc_data_file = get_afs_toc_file_instance();

    DBOOL result = afs_toc_file_save_xml(toc_data_file, tree, DTRUE);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_save_xml with initialized afs_toc_file instance and use data_v2 type
BOXING_START_TEST(afs_toc_file_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    DBOOL result = afs_toc_file_save_xml(toc_data_file, tree, DFALSE);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_save_file with file_name pointer equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_save_file(toc_data_file, file_name, DTRUE, DFALSE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_save_file with file_name pointer equal to NULL and afs_toc_file pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    DBOOL result = afs_toc_file_save_file(toc_data_file, file_name, DTRUE, DFALSE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_save_file with file_name pointer not equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_save_file_test3)
{
    const char * file_name = "afs_toc_file_save_file_test.xml";
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_save_file(toc_data_file, file_name, DTRUE, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_save_file with empty toc_data_file instance
BOXING_START_TEST(afs_toc_file_save_file_test4)
{
    const char * file_name = "afs_toc_file_save_empty_toc_test.xml";
    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_save_file(toc_data_file, file_name, DTRUE, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_save_file with initialized toc_data_file instance, in compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_save_file_test5)
{
    const char * file_name = "afs_toc_file_save_file_compact_v1_test.xml";

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_ALL);

    DBOOL result = afs_toc_file_save_file(toc_data_file, file_name, DTRUE, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_save_file with initialized toc_data_file instance, in compact format and use data_v2 type
BOXING_START_TEST(afs_toc_file_save_file_test6)
{
    const char * file_name = "afs_toc_file_save_file_compact_v2_test.xml";

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_ALL);

    DBOOL result = afs_toc_file_save_file(toc_data_file, file_name, DTRUE, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_save_file with initialized toc_data_file instance, in not compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_save_file_test7)
{
    const char * file_name = "afs_toc_file_save_file_not_compact_v1_test.xml";

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_ALL);

    DBOOL result = afs_toc_file_save_file(toc_data_file, file_name, DFALSE, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_save_file with initialized toc_data_file instance, in not compact format and use data_v2 type
BOXING_START_TEST(afs_toc_file_save_file_test8)
{
    const char * file_name = "afs_toc_file_save_file_not_compact_v2_test.xml";

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_ALL);

    DBOOL result = afs_toc_file_save_file(toc_data_file, file_name, DFALSE, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_save_file with initialized toc_data_file instance and empty checksum string, in not compact format
BOXING_START_TEST(afs_toc_file_save_file_test9)
{
    const char * file_name = "afs_toc_file_save_file_empty_checksum_test.xml";

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();
    afs_toc_file_set_checksum(toc_data_file, "");

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_ALL);

    DBOOL result = afs_toc_file_save_file(toc_data_file, file_name, DFALSE, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_save_string with afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_save_string_test1)
{
    afs_toc_file* toc_data_file = NULL;

    char * result = afs_toc_file_save_string(toc_data_file, DTRUE, DTRUE);

    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_file_save_string with empty toc_data_file instance
BOXING_START_TEST(afs_toc_file_save_string_test2)
{
    afs_toc_file* toc_data_file = afs_toc_file_create();

    char * result = afs_toc_file_save_string(toc_data_file, DTRUE, DTRUE);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_save_string with initialized toc_data_file instance, in compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_save_string_test3)
{
    char * test_string = read_xml_toc_file("afs_toc_file_save_file_compact_v1_test.xml");

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_ALL);

    char * result = afs_toc_file_save_string(toc_data_file, DTRUE, DTRUE);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_file_free(toc_data_file);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_file_save_string with initialized toc_data_file instance, in compact format and use data_v2 type
BOXING_START_TEST(afs_toc_file_save_string_test4)
{
    char * test_string = read_xml_toc_file("afs_toc_file_save_file_compact_v2_test.xml");

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_ALL);

    char * result = afs_toc_file_save_string(toc_data_file, DTRUE, DFALSE);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_file_free(toc_data_file);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_file_save_string with initialized toc_data_file instance, in not compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_save_string_test5)
{
    char * test_string = read_xml_toc_file("afs_toc_file_save_file_not_compact_v1_test.xml");

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_ALL);

    char * result = afs_toc_file_save_string(toc_data_file, DFALSE, DTRUE);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_file_free(toc_data_file);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_file_save_string with initialized toc_data_file instance, in not compact format and use data_v2 type
BOXING_START_TEST(afs_toc_file_save_string_test6)
{
    char * test_string = read_xml_toc_file("afs_toc_file_save_file_not_compact_v2_test.xml");

    afs_toc_file* toc_data_file = get_afs_toc_file_instance();

    afs_toc_file_add_type(toc_data_file, AFS_TOC_FILE_TYPE_ALL);

    char * result = afs_toc_file_save_string(toc_data_file, DFALSE, DFALSE);

    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_file_free(toc_data_file);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_file_load_file with file_name pointer equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_load_file(toc_data_file, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_load_file with file_name pointer equal to NULL and afs_toc_file pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_file(toc_data_file, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_load_file with file_name pointer not equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_load_file_test3)
{
    const char * file_name = "afs_toc_file_save_file_compact_v1_test.xml";
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_load_file(toc_data_file, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_file_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_file(toc_data_file, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_load_file with initialized toc_data_file instance, in compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_load_file_test5)
{
    const char * file_name = "afs_toc_file_save_file_compact_v1_test.xml";
    
    afs_toc_file* toc_data_file1 = afs_toc_file_create();
    
    DBOOL result = afs_toc_file_load_file(toc_data_file1, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    free(toc_data_file2->checksum);
    toc_data_file2->checksum = boxing_string_clone("");

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_load_file with initialized toc_data_file instance, in compact format and use data_v2 type
BOXING_START_TEST(afs_toc_file_load_file_test6)
{
    const char * file_name = "afs_toc_file_save_file_compact_v2_test.xml";
    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_file(toc_data_file1, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    afs_toc_file_add_type(toc_data_file2, AFS_TOC_FILE_TYPE_ALL);

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_load_file with initialized toc_data_file instance, in not compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_load_file_test7)
{
    const char * file_name = "afs_toc_file_save_file_not_compact_v1_test.xml";
    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_file(toc_data_file1, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    free(toc_data_file2->checksum);
    toc_data_file2->checksum = boxing_string_clone("");

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_load_file with initialized toc_data_file instance, in not compact format and use data_v2 type
BOXING_START_TEST(afs_toc_file_load_file_test8)
{
    const char * file_name = "afs_toc_file_save_file_not_compact_v2_test.xml";
    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_file(toc_data_file1, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    afs_toc_file_add_type(toc_data_file2, AFS_TOC_FILE_TYPE_ALL);

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_load_file with initialized toc_data_file instance, with empty checksum string
BOXING_START_TEST(afs_toc_file_load_file_test9)
{
    const char * file_name = "afs_toc_file_save_file_empty_checksum_test.xml";
    afs_toc_file * toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_file(toc_data_file1, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file * toc_data_file2 = get_afs_toc_file_instance();
    free(toc_data_file2->checksum);
    toc_data_file2->checksum = boxing_string_clone("");
    afs_toc_file_add_type(toc_data_file2, AFS_TOC_FILE_TYPE_ALL);

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
}
END_TEST


// Test function afs_toc_file_load_string with input string pointer equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_toc_file_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_load_string(toc_data_file, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_load_string with input string pointer equal to NULL and afs_toc_file pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_string(toc_data_file, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_load_string with input string pointer not equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_load_string_test3)
{
    char * input_string = read_xml_toc_file("afs_toc_file_save_file_compact_v1_test.xml");
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_load_string(toc_data_file, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_file_load_string with empty input string
BOXING_START_TEST(afs_toc_file_load_string_test4)
{
    char * input_string = "";
    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_string(toc_data_file, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_load_string with input string for initialized toc_data_file instance, in compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_load_string_test5)
{
    char * input_string = read_xml_toc_file("afs_toc_file_save_file_compact_v1_test.xml");

    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_string(toc_data_file1, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    free(toc_data_file2->checksum);
    toc_data_file2->checksum = boxing_string_clone("");

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_load_string with input string for initialized toc_data_file instance, in compact format and use data_v2 type
BOXING_START_TEST(afs_toc_file_load_string_test6)
{
    char * input_string = read_xml_toc_file("afs_toc_file_save_file_compact_v2_test.xml");
    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_string(toc_data_file1, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    afs_toc_file_add_type(toc_data_file2, AFS_TOC_FILE_TYPE_ALL);

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_load_string with input string for initialized toc_data_file instance, in not compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_load_string_test7)
{
    char * input_string = read_xml_toc_file("afs_toc_file_save_file_not_compact_v1_test.xml");
    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_string(toc_data_file1, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    free(toc_data_file2->checksum);
    toc_data_file2->checksum = boxing_string_clone("");

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_load_string with input string for initialized toc_data_file instance, in not compact format and use data_v2 type
BOXING_START_TEST(afs_toc_file_load_string_test8)
{
    char * input_string = read_xml_toc_file("afs_toc_file_save_file_not_compact_v2_test.xml");
    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_string(toc_data_file1, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    afs_toc_file_add_type(toc_data_file2, AFS_TOC_FILE_TYPE_ALL);

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_load_xml with xml instance pointer equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_toc_file_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_load_xml(toc_data_file, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_load_xml with xml instance pointer equal to NULL and afs_toc_file pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_xml(toc_data_file, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_load_xml with xml instance pointer not equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_load_xml_test3)
{
    mxml_node_t *tree = get_toc_xml_instance("afs_toc_file_save_file_compact_v1_test.xml");
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_load_xml(toc_data_file, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_file_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_xml(toc_data_file, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_load_xml with xml instance for initialized toc_data_file instance, in compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_load_xml_test5)
{
    mxml_node_t *tree = get_toc_xml_instance("afs_toc_file_save_file_compact_v1_test.xml");

    afs_toc_file * toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_xml(toc_data_file1, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file * toc_data_file2 = get_afs_toc_file_instance();
    free(toc_data_file2->checksum);
    toc_data_file2->checksum = boxing_string_clone("");

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_load_xml with xml instance for initialized toc_data_file instance, in compact format and use data_v2 type
BOXING_START_TEST(afs_toc_file_load_xml_test6)
{
    mxml_node_t *tree = get_toc_xml_instance("afs_toc_file_save_file_compact_v2_test.xml");
    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_xml(toc_data_file1, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    afs_toc_file_add_type(toc_data_file2, AFS_TOC_FILE_TYPE_ALL);

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_load_xml with xml instance for initialized toc_data_file instance, in not compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_load_xml_test7)
{
    mxml_node_t *tree = get_toc_xml_instance("afs_toc_file_save_file_not_compact_v1_test.xml");
    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_xml(toc_data_file1, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    free(toc_data_file2->checksum);
    toc_data_file2->checksum = boxing_string_clone("");

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_load_xml with xml instance for initialized toc_data_file instance, in not compact format and use data_v2 type
BOXING_START_TEST(afs_toc_file_load_xml_test8)
{
    mxml_node_t *tree = get_toc_xml_instance("afs_toc_file_save_file_not_compact_v2_test.xml");
    afs_toc_file* toc_data_file1 = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_xml(toc_data_file1, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file1 != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file* toc_data_file2 = get_afs_toc_file_instance();
    afs_toc_file_add_type(toc_data_file2, AFS_TOC_FILE_TYPE_ALL);

    BOXING_ASSERT(afs_toc_file_equal(toc_data_file1, toc_data_file2) == DTRUE);

    afs_toc_file_free(toc_data_file1);
    afs_toc_file_free(toc_data_file2);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_load_data_v1 with xml instance pointer equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_toc_file_load_data_v1_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_load_data_v1(toc_data_file, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_load_data_v1 with xml instance pointer equal to NULL and afs_toc_file pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_load_data_v1_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_data_v1(toc_data_file, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_load_data_v1 with xml instance pointer not equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_load_data_v1_test3)
{
    mxml_node_t *tree = get_toc_xml_instance("afs_toc_file_save_file_compact_v1_test.xml");
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_load_data_v1(toc_data_file, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_load_data_v1 with xml instance pointer not equal to NULL and afs_toc_file pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_load_data_v1_test4)
{
    mxml_node_t *tree = get_toc_xml_instance("afs_toc_file_save_file_compact_v1_test.xml");
    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_data_v1(toc_data_file, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    BOXING_ASSERT(toc_data_file->size == 29672);
    BOXING_ASSERT(toc_data_file->start_frame == 2);
    BOXING_ASSERT(toc_data_file->start_byte == 0);
    BOXING_ASSERT(toc_data_file->end_frame == 2);
    BOXING_ASSERT(toc_data_file->end_byte == 29671);

    afs_toc_file_free(toc_data_file);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_load_data_v2 with xml instance pointer equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_toc_file_load_data_v2_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_load_data_v2(toc_data_file, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_load_data_v2 with xml instance pointer equal to NULL and afs_toc_file pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_load_data_v2_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_data_v2(toc_data_file, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_data_file);
}
END_TEST


// Test function afs_toc_file_load_data_v2 with xml instance pointer not equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_file_load_data_v2_test3)
{
    mxml_node_t *tree = get_toc_xml_instance("afs_toc_file_save_file_compact_v2_test.xml");
    afs_toc_file* toc_data_file = NULL;

    DBOOL result = afs_toc_file_load_data_v2(toc_data_file, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_load_data_v2 with xml instance pointer not equal to NULL and afs_toc_file pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_load_data_v2_test4)
{
    mxml_node_t *tree = get_toc_xml_instance("afs_toc_file_save_file_compact_v2_test.xml");
    afs_toc_file* toc_data_file = afs_toc_file_create();

    DBOOL result = afs_toc_file_load_data_v2(toc_data_file, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file != NULL);
    BOXING_ASSERT(result == DTRUE);

    BOXING_ASSERT(toc_data_file->size == 29672);
    BOXING_ASSERT(strcmp(toc_data_file->checksum, "f2eb0d6c6964d206012695a10ea6cd877c95c340") == 0);
    BOXING_ASSERT(toc_data_file->start_frame == 2);
    BOXING_ASSERT(toc_data_file->start_byte == 0);
    BOXING_ASSERT(toc_data_file->end_frame == 2);
    BOXING_ASSERT(toc_data_file->end_byte == 29671);

    afs_toc_file_free(toc_data_file);
    mxmlDelete(tree);
}
END_TEST

#ifdef UNIT_TESTS_ENABLED
#define GET_TEST_FN(name) (name)->fn
#else
#define GET_TEST_FN(name) (name)
#endif


Suite * tocdatafile_tests(void)
{
    TCase * tc_tocdatafile_functions_tests = tcase_create("tocdatafile_functions_tests");
    typedef void (*testFp)(int);

    testFp tests[] =
    {
        // Test function afs_administrative_metadata_create
        GET_TEST_FN(afs_toc_file_create_test1),
        // Test function afs_administrative_metadata_create2
        GET_TEST_FN(afs_toc_file_create2_test1),
        GET_TEST_FN(afs_toc_file_create2_test2),
        // Test function afs_toc_file_init
        GET_TEST_FN(afs_toc_file_init_test1),
        GET_TEST_FN(afs_toc_file_init_test2),
        // Test function afs_toc_file_init2
        GET_TEST_FN(afs_toc_file_init2_test1),
        GET_TEST_FN(afs_toc_file_init2_test2),
        GET_TEST_FN(afs_toc_file_init2_test3),
        // Test function afs_toc_file_clone
        GET_TEST_FN(afs_toc_file_clone_test1),
        GET_TEST_FN(afs_toc_file_clone_test2),
        GET_TEST_FN(afs_toc_file_clone_test3),
        // Test function afs_toc_file_get_new_reference
        GET_TEST_FN(afs_toc_file_get_new_reference_test1),
        GET_TEST_FN(afs_toc_file_get_new_reference_test2),
        GET_TEST_FN(afs_toc_file_get_new_reference_test3),
        // Test function afs_toc_file_equal
        GET_TEST_FN(afs_toc_file_equal_test1),
        GET_TEST_FN(afs_toc_file_equal_test2),
        GET_TEST_FN(afs_toc_file_equal_test3),
        GET_TEST_FN(afs_toc_file_equal_test4),
        GET_TEST_FN(afs_toc_file_equal_test5),
        GET_TEST_FN(afs_toc_file_equal_test6),
        GET_TEST_FN(afs_toc_file_equal_test7),
        GET_TEST_FN(afs_toc_file_equal_test8),
        GET_TEST_FN(afs_toc_file_equal_test9),
        GET_TEST_FN(afs_toc_file_equal_test10),
        GET_TEST_FN(afs_toc_file_equal_test11),
        GET_TEST_FN(afs_toc_file_equal_test12),
        GET_TEST_FN(afs_toc_file_equal_test13),
        GET_TEST_FN(afs_toc_file_equal_test14),
        GET_TEST_FN(afs_toc_file_equal_test15),
        GET_TEST_FN(afs_toc_file_equal_test16),
        GET_TEST_FN(afs_toc_file_equal_test17),
        GET_TEST_FN(afs_toc_file_equal_test18),
        GET_TEST_FN(afs_toc_file_equal_test19),
        GET_TEST_FN(afs_toc_file_equal_test20),
        GET_TEST_FN(afs_toc_file_equal_test21),
        // Test function afs_toc_file_not_equal
        GET_TEST_FN(afs_toc_file_not_equal_test1),
        GET_TEST_FN(afs_toc_file_not_equal_test2),
        GET_TEST_FN(afs_toc_file_not_equal_test3),
        GET_TEST_FN(afs_toc_file_not_equal_test4),
        GET_TEST_FN(afs_toc_file_not_equal_test5),
        GET_TEST_FN(afs_toc_file_not_equal_test6),
        GET_TEST_FN(afs_toc_file_not_equal_test7),
        GET_TEST_FN(afs_toc_file_not_equal_test8),
        GET_TEST_FN(afs_toc_file_not_equal_test9),
        GET_TEST_FN(afs_toc_file_not_equal_test10),
        GET_TEST_FN(afs_toc_file_not_equal_test11),
        GET_TEST_FN(afs_toc_file_not_equal_test12),
        GET_TEST_FN(afs_toc_file_not_equal_test13),
        GET_TEST_FN(afs_toc_file_not_equal_test14),
        GET_TEST_FN(afs_toc_file_not_equal_test15),
        GET_TEST_FN(afs_toc_file_not_equal_test16),
        GET_TEST_FN(afs_toc_file_not_equal_test17),
        GET_TEST_FN(afs_toc_file_not_equal_test18),
        GET_TEST_FN(afs_toc_file_not_equal_test19),
        GET_TEST_FN(afs_toc_file_not_equal_test20),
        GET_TEST_FN(afs_toc_file_not_equal_test21),
        // Test function afs_toc_file_add_frame_offset
        GET_TEST_FN(afs_toc_file_add_frame_offset_test1),
        GET_TEST_FN(afs_toc_file_add_frame_offset_test2),
        GET_TEST_FN(afs_toc_file_add_frame_offset_test3),
        GET_TEST_FN(afs_toc_file_add_frame_offset_test4),
        // Test function afs_toc_file_add_type
        GET_TEST_FN(afs_toc_file_add_type_test1),
        GET_TEST_FN(afs_toc_file_add_type_test2),
        GET_TEST_FN(afs_toc_file_add_type_test3),
        GET_TEST_FN(afs_toc_file_add_type_test4),
        // Test function afs_toc_file_set_name
        GET_TEST_FN(afs_toc_file_set_name_test1),
        GET_TEST_FN(afs_toc_file_set_name_test2),
        GET_TEST_FN(afs_toc_file_set_name_test3),
        GET_TEST_FN(afs_toc_file_set_name_test4),
        GET_TEST_FN(afs_toc_file_set_name_test5),
        GET_TEST_FN(afs_toc_file_set_name_test6),
        GET_TEST_FN(afs_toc_file_set_name_test7),
        GET_TEST_FN(afs_toc_file_set_name_test8),
        GET_TEST_FN(afs_toc_file_set_name_test9),
        // Test function afs_toc_file_set_date
        GET_TEST_FN(afs_toc_file_set_date_test1),
        GET_TEST_FN(afs_toc_file_set_date_test2),
        GET_TEST_FN(afs_toc_file_set_date_test3),
        GET_TEST_FN(afs_toc_file_set_date_test4),
        GET_TEST_FN(afs_toc_file_set_date_test5),
        GET_TEST_FN(afs_toc_file_set_date_test6),
        GET_TEST_FN(afs_toc_file_set_date_test7),
        GET_TEST_FN(afs_toc_file_set_date_test8),
        GET_TEST_FN(afs_toc_file_set_date_test9),
        // Test function afs_toc_file_set_checksum
        GET_TEST_FN(afs_toc_file_set_checksum_test1),
        GET_TEST_FN(afs_toc_file_set_checksum_test2),
        GET_TEST_FN(afs_toc_file_set_checksum_test3),
        GET_TEST_FN(afs_toc_file_set_checksum_test4),
        GET_TEST_FN(afs_toc_file_set_checksum_test5),
        GET_TEST_FN(afs_toc_file_set_checksum_test6),
        GET_TEST_FN(afs_toc_file_set_checksum_test7),
        GET_TEST_FN(afs_toc_file_set_checksum_test8),
        GET_TEST_FN(afs_toc_file_set_checksum_test9),
        // Test function afs_toc_file_set_unique_id
        GET_TEST_FN(afs_toc_file_set_unique_id_test1),
        GET_TEST_FN(afs_toc_file_set_unique_id_test2),
        GET_TEST_FN(afs_toc_file_set_unique_id_test3),
        GET_TEST_FN(afs_toc_file_set_unique_id_test4),
        GET_TEST_FN(afs_toc_file_set_unique_id_test5),
        GET_TEST_FN(afs_toc_file_set_unique_id_test6),
        GET_TEST_FN(afs_toc_file_set_unique_id_test7),
        GET_TEST_FN(afs_toc_file_set_unique_id_test8),
        GET_TEST_FN(afs_toc_file_set_unique_id_test9),
        // Test function afs_toc_file_set_file_format
        GET_TEST_FN(afs_toc_file_set_file_format_test1),
        GET_TEST_FN(afs_toc_file_set_file_format_test2),
        GET_TEST_FN(afs_toc_file_set_file_format_test3),
        GET_TEST_FN(afs_toc_file_set_file_format_test4),
        GET_TEST_FN(afs_toc_file_set_file_format_test5),
        GET_TEST_FN(afs_toc_file_set_file_format_test6),
        GET_TEST_FN(afs_toc_file_set_file_format_test7),
        GET_TEST_FN(afs_toc_file_set_file_format_test8),
        GET_TEST_FN(afs_toc_file_set_file_format_test9),
        // Test function afs_toc_file_set_preview
        GET_TEST_FN(afs_toc_file_set_preview_test1),
        GET_TEST_FN(afs_toc_file_set_preview_test2),
        GET_TEST_FN(afs_toc_file_set_preview_test3),
        GET_TEST_FN(afs_toc_file_set_preview_test4),
        GET_TEST_FN(afs_toc_file_set_preview_test5),
        GET_TEST_FN(afs_toc_file_set_preview_test6),
        GET_TEST_FN(afs_toc_file_set_preview_test7),
        GET_TEST_FN(afs_toc_file_set_preview_test8),
        GET_TEST_FN(afs_toc_file_set_preview_test9),
        GET_TEST_FN(afs_toc_file_set_preview_test10),
        GET_TEST_FN(afs_toc_file_set_preview_test11),
        GET_TEST_FN(afs_toc_file_set_preview_test12),
        // Test function afs_toc_file_set_metadata
        GET_TEST_FN(afs_toc_file_set_metadata_test1),
        GET_TEST_FN(afs_toc_file_set_metadata_test2),
        GET_TEST_FN(afs_toc_file_set_metadata_test3),
        GET_TEST_FN(afs_toc_file_set_metadata_test4),
        GET_TEST_FN(afs_toc_file_set_metadata_test5),
        GET_TEST_FN(afs_toc_file_set_metadata_test6),
        GET_TEST_FN(afs_toc_file_set_metadata_test7),
        GET_TEST_FN(afs_toc_file_set_metadata_test8),
        GET_TEST_FN(afs_toc_file_set_metadata_test9),
        GET_TEST_FN(afs_toc_file_set_metadata_test10),
        GET_TEST_FN(afs_toc_file_set_metadata_test11),
        GET_TEST_FN(afs_toc_file_set_metadata_test12),
        // Test function afs_toc_file_is_parent
        GET_TEST_FN(afs_toc_file_is_parent_test1),
        GET_TEST_FN(afs_toc_file_is_parent_test2),
        GET_TEST_FN(afs_toc_file_is_parent_test3),
        // Test function afs_toc_file_has_parent
        GET_TEST_FN(afs_toc_file_has_parent_test1),
        GET_TEST_FN(afs_toc_file_has_parent_test2),
        GET_TEST_FN(afs_toc_file_has_parent_test3),
        GET_TEST_FN(afs_toc_file_has_parent_test4),
        // Test function afs_toc_file_is_standalone
        GET_TEST_FN(afs_toc_file_is_standalone_test1),
        GET_TEST_FN(afs_toc_file_is_standalone_test2),
        GET_TEST_FN(afs_toc_file_is_standalone_test3),
        GET_TEST_FN(afs_toc_file_is_standalone_test4),
        // Test function afs_toc_file_is_digital
        GET_TEST_FN(afs_toc_file_is_digital_test1),
        GET_TEST_FN(afs_toc_file_is_digital_test2),
        GET_TEST_FN(afs_toc_file_is_digital_test3),
        GET_TEST_FN(afs_toc_file_is_digital_test4),
        GET_TEST_FN(afs_toc_file_is_digital_test5),
        // Test function afs_toc_file_is_preview
        GET_TEST_FN(afs_toc_file_is_preview_test1),
        GET_TEST_FN(afs_toc_file_is_preview_test2),
        GET_TEST_FN(afs_toc_file_is_preview_test3),
        GET_TEST_FN(afs_toc_file_is_preview_test4),
        GET_TEST_FN(afs_toc_file_is_preview_test5),
        // Test function afs_toc_file_has_metadata
        GET_TEST_FN(afs_toc_file_has_metadata_test1),
        GET_TEST_FN(afs_toc_file_has_metadata_test2),
        GET_TEST_FN(afs_toc_file_has_metadata_test3),
        GET_TEST_FN(afs_toc_file_has_metadata_test4),
        GET_TEST_FN(afs_toc_file_has_metadata_test5),
        GET_TEST_FN(afs_toc_file_has_metadata_test6),
        // Test function afs_toc_file_is_valid
        GET_TEST_FN(afs_toc_file_is_valid_test1),
        GET_TEST_FN(afs_toc_file_is_valid_test2),
        GET_TEST_FN(afs_toc_file_is_valid_test3),
        GET_TEST_FN(afs_toc_file_is_valid_test4),
        GET_TEST_FN(afs_toc_file_is_valid_test5),
        GET_TEST_FN(afs_toc_file_is_valid_test6),
        GET_TEST_FN(afs_toc_file_is_valid_test7),
        GET_TEST_FN(afs_toc_file_is_valid_test8),
        GET_TEST_FN(afs_toc_file_is_valid_test9),
        GET_TEST_FN(afs_toc_file_is_valid_test10),
        GET_TEST_FN(afs_toc_file_is_valid_test11),
        GET_TEST_FN(afs_toc_file_is_valid_test12),
        GET_TEST_FN(afs_toc_file_is_valid_test13),
        GET_TEST_FN(afs_toc_file_is_valid_test14),
        GET_TEST_FN(afs_toc_file_is_valid_test15),
        // Test function afs_toc_file_get_duration_frames
        GET_TEST_FN(afs_toc_file_get_duration_frames_test1),
        GET_TEST_FN(afs_toc_file_get_duration_frames_test2),
        GET_TEST_FN(afs_toc_file_get_duration_frames_test3),
        GET_TEST_FN(afs_toc_file_get_duration_frames_test4),
        GET_TEST_FN(afs_toc_file_get_duration_frames_test5),
        // Test function afs_toc_file_save_xml
        GET_TEST_FN(afs_toc_file_save_xml_test1),
        GET_TEST_FN(afs_toc_file_save_xml_test2),
        GET_TEST_FN(afs_toc_file_save_xml_test3),
        GET_TEST_FN(afs_toc_file_save_xml_test4),
        GET_TEST_FN(afs_toc_file_save_xml_test5),
        GET_TEST_FN(afs_toc_file_save_xml_test6),
        // Test function afs_toc_file_save_file
        GET_TEST_FN(afs_toc_file_save_file_test1),
        GET_TEST_FN(afs_toc_file_save_file_test2),
        GET_TEST_FN(afs_toc_file_save_file_test3),
        GET_TEST_FN(afs_toc_file_save_file_test4),
        GET_TEST_FN(afs_toc_file_save_file_test5),
        GET_TEST_FN(afs_toc_file_save_file_test6),
        GET_TEST_FN(afs_toc_file_save_file_test7),
        GET_TEST_FN(afs_toc_file_save_file_test8),
        GET_TEST_FN(afs_toc_file_save_file_test9),
        // Test function afs_toc_file_save_string
        GET_TEST_FN(afs_toc_file_save_string_test1),
        GET_TEST_FN(afs_toc_file_save_string_test2),
        GET_TEST_FN(afs_toc_file_save_string_test3),
        GET_TEST_FN(afs_toc_file_save_string_test4),
        GET_TEST_FN(afs_toc_file_save_string_test5),
        GET_TEST_FN(afs_toc_file_save_string_test6),
        // Test function afs_toc_file_load_file
        GET_TEST_FN(afs_toc_file_load_file_test1),
        GET_TEST_FN(afs_toc_file_load_file_test2),
        GET_TEST_FN(afs_toc_file_load_file_test3),
        GET_TEST_FN(afs_toc_file_load_file_test4),
        GET_TEST_FN(afs_toc_file_load_file_test5),
        GET_TEST_FN(afs_toc_file_load_file_test6),
        GET_TEST_FN(afs_toc_file_load_file_test7),
        GET_TEST_FN(afs_toc_file_load_file_test8),
        GET_TEST_FN(afs_toc_file_load_file_test9),
        // Test function afs_toc_file_load_string
        GET_TEST_FN(afs_toc_file_load_string_test1),
        GET_TEST_FN(afs_toc_file_load_string_test2),
        GET_TEST_FN(afs_toc_file_load_string_test3),
        GET_TEST_FN(afs_toc_file_load_string_test4),
        GET_TEST_FN(afs_toc_file_load_string_test5),
        GET_TEST_FN(afs_toc_file_load_string_test6),
        GET_TEST_FN(afs_toc_file_load_string_test7),
        GET_TEST_FN(afs_toc_file_load_string_test8),
        // Test function afs_toc_file_load_xml
        GET_TEST_FN(afs_toc_file_load_xml_test1),
        GET_TEST_FN(afs_toc_file_load_xml_test2),
        GET_TEST_FN(afs_toc_file_load_xml_test3),
        GET_TEST_FN(afs_toc_file_load_xml_test4),
        GET_TEST_FN(afs_toc_file_load_xml_test5),
        GET_TEST_FN(afs_toc_file_load_xml_test6),
        GET_TEST_FN(afs_toc_file_load_xml_test7),
        GET_TEST_FN(afs_toc_file_load_xml_test8),
        // Test function afs_toc_file_load_data_v1
        GET_TEST_FN(afs_toc_file_load_data_v1_test1),
        GET_TEST_FN(afs_toc_file_load_data_v1_test2),
        GET_TEST_FN(afs_toc_file_load_data_v1_test3),
        GET_TEST_FN(afs_toc_file_load_data_v1_test4),
        // Test function afs_toc_file_load_data_v2
        GET_TEST_FN(afs_toc_file_load_data_v2_test1),
        GET_TEST_FN(afs_toc_file_load_data_v2_test2),
        GET_TEST_FN(afs_toc_file_load_data_v2_test3),
        GET_TEST_FN(afs_toc_file_load_data_v2_test4)
    };
    
    for (size_t t = 0; t < sizeof(tests)/sizeof(tests[0]); t++)
    {
        tcase_add_test_l(tc_tocdatafile_functions_tests, tests[t]);
    }
    
    Suite * s = suite_create("tocdatafile_test_util");
    suite_add_tcase(s, tc_tocdatafile_functions_tests);

    return s;
}
