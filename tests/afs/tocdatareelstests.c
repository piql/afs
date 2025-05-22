#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "tocdatareels.h"
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

static afs_toc_file_preview_pages* get_filled_pages_vector7(unsigned int pages_count)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(pages_count);

    for (unsigned int i = 0; i < pages_count; i++)
    {
        GVECTORN(pages, afs_toc_file_preview_page *, i) = afs_toc_file_preview_page_create2("Some layout ID", 9 + i, 8 + i, 7 + i, 6 + i, 5 + i, 4 + i, 3 + i);
    }

    return pages;
}


static afs_toc_data_file_metadata_sources * get_filled_toc_data_file_metadata_sources_vector7(unsigned int sources_count)
{
    afs_toc_data_file_metadata_sources * sources = gvector_create_pointers(sources_count);

    for (unsigned int i = 0; i < sources_count; i++)
    {
        GVECTORN(sources, afs_toc_data_file_metadata_source *, i) = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", i + 1, i + 2);
    }

    return sources;
}


static afs_toc_data_file_metadata * get_afs_toc_data_file_metadata_instance7(unsigned int sources_count)
{
    afs_toc_data_file_metadata_sources * sources = get_filled_toc_data_file_metadata_sources_vector7(sources_count);
    afs_toc_data_file_metadata * toc_data_file_metadata = afs_toc_data_file_metadata_create2(sources);

    return toc_data_file_metadata;
}


static afs_toc_file * get_afs_toc_file_instance7(unsigned int number)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector7(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance7(5);
    char current_unique_id[100];
    sprintf(current_unique_id, "38A0AAAB-16B2-640C-5353-6DB8AE4367B%u", number);

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
        current_unique_id,
        AFS_TOC_FILE_NO_PARENT,
        AFS_TOC_FILE_TYPE_ALL,
        toc_file_preview,
        "file_format",
        toc_data_file_metadata);
}


static afs_toc_files * get_afs_toc_files_instance7(const size_t files_count)
{
    afs_toc_indices * tocs = gvector_create_pointers(files_count);

    for (size_t i = 0; i < files_count; i++)
    {
        afs_toc_file * toc_file = get_afs_toc_file_instance7((unsigned int)i);
        GVECTORN(tocs, afs_toc_file *, i) = toc_file;
    }

    return afs_toc_files_create2(tocs);
}


afs_toc_data_reel * get_afs_toc_data_reel_instance7(const char * id, unsigned int files_count)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2(id);
    toc_data_reel->files = get_afs_toc_files_instance7(files_count);

    return toc_data_reel;
}


afs_toc_data_reels * get_afs_toc_data_reels_instance7(unsigned int reels_count)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    for (unsigned int i = 0; i < reels_count; i++)
    {
        char current_reel_id[100];
        sprintf(current_reel_id, "Reel id %u", i);
        afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7(current_reel_id, 3);
        afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);
    }

    return toc_data_reels;
}


static void test_afs_toc_data_reel(afs_toc_data_reel * toc_data_reel, const char * id, const size_t files_count)
{
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(strcmp(id, toc_data_reel->id) == 0);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_data_reel->files) == files_count);

    for (size_t i = 0; i < files_count; i++)
    {
        afs_toc_file* afs_toc_file_instance = get_afs_toc_file_instance7((unsigned int)i);
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i), afs_toc_file_instance) == DTRUE);
        afs_toc_file_free(afs_toc_file_instance);
    }
}


static void test_afs_toc_data_reels(afs_toc_data_reels * toc_data_reels, unsigned int reels_count)
{
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(afs_toc_data_reels_get_reels_count(toc_data_reels) == reels_count);

    for (unsigned int i = 0; i < reels_count; i++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_reels_get_reel(toc_data_reels, i);
        
        char current_reel_id[100];
        sprintf(current_reel_id, "Reel id %u", i);

        test_afs_toc_data_reel(toc_data_reel, current_reel_id, 3);
    }
}


static char * read_xml_toc_data_reels(const char * file_name)
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

    fclose(file);
    return xml_string;
}


static mxml_node_t * get_toc_data_reels_xml_instance(const char * file_name)
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


static void test_equals_toc_data_reels(afs_toc_data_reels * toc_data_reels1, afs_toc_data_reels * toc_data_reels2, DBOOL expected_result)
{
    DBOOL result = afs_toc_data_reels_equal(toc_data_reels2, toc_data_reels1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_data_reels_equal(toc_data_reels1, toc_data_reels2);
    BOXING_ASSERT(result == expected_result);
}


// TEST TOC DATA REELS FUNCTIONS
//


// Test function afs_toc_data_reels_create
BOXING_START_TEST(afs_toc_data_reels_create_test1)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_create2 with afs_toc_data_reels_vector pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_create2_test1)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(NULL);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_create2 with empty afs_toc_data_reels_vector
BOXING_START_TEST(afs_toc_data_reels_create2_test2)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_create2 with filled reels vector
BOXING_START_TEST(afs_toc_data_reels_create2_test3)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(3);

    for (unsigned int i = 0; i < reels->size; i++)
    {
        char current_reel_id[100];
        sprintf(current_reel_id, "Reel id %u", i);
        afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7(current_reel_id, 3);
        GVECTORN(reels, afs_toc_data_reel *, i) = toc_data_reel;
    }

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);

    test_afs_toc_data_reels(toc_data_reels, 3);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_clone with input pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_clone_test1)
{
    afs_toc_data_reels * toc_data_reels1 = NULL;

    BOXING_ASSERT(toc_data_reels1 == NULL);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_clone(toc_data_reels1);

    BOXING_ASSERT(toc_data_reels2 == NULL);
}
END_TEST


// Test function afs_toc_data_reels_clone with empty input afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_clone_test2)
{
    afs_toc_data_reels * toc_data_reels1 = afs_toc_data_reels_create();

    test_afs_toc_data_reels(toc_data_reels1, 0);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_clone(toc_data_reels1);

    test_afs_toc_data_reels(toc_data_reels2, 0);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_clone with not empty input afs_toc_data_reels instance and empty reels vector
BOXING_START_TEST(afs_toc_data_reels_clone_test3)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels1 = afs_toc_data_reels_create2(reels);

    test_afs_toc_data_reels(toc_data_reels1, 0);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_clone(toc_data_reels1);

    test_afs_toc_data_reels(toc_data_reels2, 0);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_clone with not empty input afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_clone_test4)
{
    afs_toc_data_reels * toc_data_reels1 = get_afs_toc_data_reels_instance7(5);

    test_afs_toc_data_reels(toc_data_reels1, 5);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_clone(toc_data_reels1);

    test_afs_toc_data_reels(toc_data_reels2, 5);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_get_new_reference_test1)
{
    afs_toc_data_reels * toc_data_reels1 = NULL;

    BOXING_ASSERT(toc_data_reels1 == NULL);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_get_new_reference(toc_data_reels1);

    BOXING_ASSERT(toc_data_reels2 == NULL);

    toc_data_reels1 = afs_toc_data_reels_create();

    test_afs_toc_data_reels(toc_data_reels1, 0);
    BOXING_ASSERT(toc_data_reels1->reference_count == 1);
    BOXING_ASSERT(toc_data_reels2 == NULL);

    afs_toc_data_reels_free(toc_data_reels1);
}
END_TEST


// Test function afs_toc_data_reels_get_new_reference with empty input afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_get_new_reference_test2)
{
    afs_toc_data_reels * toc_data_reels1 = afs_toc_data_reels_create();

    test_afs_toc_data_reels(toc_data_reels1, 0);
    BOXING_ASSERT(toc_data_reels1->reference_count == 1);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_get_new_reference(toc_data_reels1);

    test_afs_toc_data_reels(toc_data_reels2, 0);
    BOXING_ASSERT(toc_data_reels1->reference_count == 2);
    BOXING_ASSERT(toc_data_reels2->reference_count == 2);
    BOXING_ASSERT(toc_data_reels1 == toc_data_reels2);

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("Reel id 0", 3);
    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels1, toc_data_reel);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels2, 1);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels1);
}
END_TEST


// Test function afs_toc_data_reels_get_new_reference with not empty input afs_toc_data_reels instance and empty reels vector
BOXING_START_TEST(afs_toc_data_reels_get_new_reference_test3)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels1 = afs_toc_data_reels_create2(reels);

    test_afs_toc_data_reels(toc_data_reels1, 0);
    BOXING_ASSERT(toc_data_reels1->reference_count == 1);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_get_new_reference(toc_data_reels1);

    test_afs_toc_data_reels(toc_data_reels2, 0);
    BOXING_ASSERT(toc_data_reels1->reference_count == 2);
    BOXING_ASSERT(toc_data_reels2->reference_count == 2);
    BOXING_ASSERT(toc_data_reels1 == toc_data_reels2);

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("Reel id 0", 3);
    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels1, toc_data_reel);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels2, 1);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels1);
}
END_TEST


// Test function afs_toc_data_reels_get_new_reference with not empty input afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_get_new_reference_test4)
{
    afs_toc_data_reels * toc_data_reels1 = get_afs_toc_data_reels_instance7(5);

    test_afs_toc_data_reels(toc_data_reels1, 5);
    BOXING_ASSERT(toc_data_reels1->reference_count == 1);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_get_new_reference(toc_data_reels1);

    test_afs_toc_data_reels(toc_data_reels2, 5);
    BOXING_ASSERT(toc_data_reels1->reference_count == 2);
    BOXING_ASSERT(toc_data_reels2->reference_count == 2);
    BOXING_ASSERT(toc_data_reels1 == toc_data_reels2);

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("Reel id 5", 3);
    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels1, toc_data_reel);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels2, 6);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels1);
}
END_TEST


// Test function afs_toc_data_reels_equal with input afs_toc_data_reels pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reels_equal_test1)
{
    afs_toc_data_reels * toc_data_reels1 = NULL;
    afs_toc_data_reels * toc_data_reels2 = NULL;

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DTRUE);
}
END_TEST

// Test function afs_toc_data_reels_equal with empty input afs_toc_data_reels instances 
BOXING_START_TEST(afs_toc_data_reels_equal_test2)
{
    afs_toc_data_reels * toc_data_reels1 = afs_toc_data_reels_create();
    test_afs_toc_data_reels(toc_data_reels1, 0);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_create();
    test_afs_toc_data_reels(toc_data_reels2, 0);

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DTRUE);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_equal with not empty input afs_toc_data_reels instances and empty reels vector
BOXING_START_TEST(afs_toc_data_reels_equal_test3)
{
    afs_toc_data_reels_vector * reels1 = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels1 = afs_toc_data_reels_create2(reels1);
    test_afs_toc_data_reels(toc_data_reels1, 0);

    afs_toc_data_reels_vector * reels2 = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_create2(reels2);
    test_afs_toc_data_reels(toc_data_reels2, 0);

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DTRUE);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_equal with not empty input afs_toc_data_reels instances and reels vector with one pointer
BOXING_START_TEST(afs_toc_data_reels_equal_test4)
{
    afs_toc_data_reels * toc_data_reels1 = get_afs_toc_data_reels_instance7(1);
    test_afs_toc_data_reels(toc_data_reels1, 1);

    afs_toc_data_reels * toc_data_reels2 = get_afs_toc_data_reels_instance7(1);
    test_afs_toc_data_reels(toc_data_reels2, 1);

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DTRUE);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_equal with not empty input afs_toc_data_reels instances and reels vector with three pointers
BOXING_START_TEST(afs_toc_data_reels_equal_test5)
{
    afs_toc_data_reels * toc_data_reels1 = get_afs_toc_data_reels_instance7(3);
    test_afs_toc_data_reels(toc_data_reels1, 3);

    afs_toc_data_reels * toc_data_reels2 = get_afs_toc_data_reels_instance7(3);
    test_afs_toc_data_reels(toc_data_reels2, 3);

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DTRUE);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_equal with empty input afs_toc_data_reels instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_data_reels_equal_test6)
{
    afs_toc_data_reels * toc_data_reels1 = afs_toc_data_reels_create();
    test_afs_toc_data_reels(toc_data_reels1, 0);

    afs_toc_data_reels * toc_data_reels2 = NULL;

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DFALSE);

    afs_toc_data_reels_free(toc_data_reels1);
}
END_TEST


// Test function afs_toc_data_reels_equal with input afs_toc_data_reels instance equal to NULL and instance with empty reels vector 
BOXING_START_TEST(afs_toc_data_reels_equal_test7)
{
    afs_toc_data_reels_vector * reels1 = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels1 = afs_toc_data_reels_create2(reels1);
    test_afs_toc_data_reels(toc_data_reels1, 0);

    afs_toc_data_reels * toc_data_reels2 = NULL;

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DFALSE);

    afs_toc_data_reels_free(toc_data_reels1);
}
END_TEST


// Test function afs_toc_data_reels_equal with input afs_toc_data_reels pointer equal to NULL and instance with filled reels vector 
BOXING_START_TEST(afs_toc_data_reels_equal_test8)
{
    afs_toc_data_reels * toc_data_reels1 = get_afs_toc_data_reels_instance7(3);
    test_afs_toc_data_reels(toc_data_reels1, 3);

    afs_toc_data_reels * toc_data_reels2 = NULL;

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DFALSE);

    afs_toc_data_reels_free(toc_data_reels1);
}
END_TEST


// Test function afs_toc_data_reels_equal with empty input afs_toc_data_reels instance and instance with empty reels vector 
BOXING_START_TEST(afs_toc_data_reels_equal_test9)
{
    afs_toc_data_reels_vector * reels1 = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels1 = afs_toc_data_reels_create2(reels1);
    test_afs_toc_data_reels(toc_data_reels1, 0);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_create();
    test_afs_toc_data_reels(toc_data_reels2, 0);

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DFALSE);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_equal with empty input afs_toc_data_reels instance and instance with filled reels vector 
BOXING_START_TEST(afs_toc_data_reels_equal_test10)
{
    afs_toc_data_reels * toc_data_reels1 = get_afs_toc_data_reels_instance7(3);
    test_afs_toc_data_reels(toc_data_reels1, 3);

    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_create();
    test_afs_toc_data_reels(toc_data_reels2, 0);

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DFALSE);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_equal with input afs_toc_data_reels instance with empty reels vector and instance with filled reels vector 
BOXING_START_TEST(afs_toc_data_reels_equal_test11)
{
    afs_toc_data_reels * toc_data_reels1 = get_afs_toc_data_reels_instance7(3);
    test_afs_toc_data_reels(toc_data_reels1, 3);

    afs_toc_data_reels_vector * reels2 = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels2 = afs_toc_data_reels_create2(reels2);
    test_afs_toc_data_reels(toc_data_reels2, 0);

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DFALSE);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_equal with input afs_toc_data_reels instances with filled reels vector with different count pointers
BOXING_START_TEST(afs_toc_data_reels_equal_test12)
{
    afs_toc_data_reels * toc_data_reels1 = get_afs_toc_data_reels_instance7(1);
    test_afs_toc_data_reels(toc_data_reels1, 1);

    afs_toc_data_reels * toc_data_reels2 = get_afs_toc_data_reels_instance7(2);
    test_afs_toc_data_reels(toc_data_reels2, 2);

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DFALSE);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_equal with afs_toc_data_reels instances with different data
BOXING_START_TEST(afs_toc_data_reels_equal_test13)
{
    afs_toc_data_reels * toc_data_reels1 = get_afs_toc_data_reels_instance7(1);
    test_afs_toc_data_reels(toc_data_reels1, 1);

    afs_toc_data_reels * toc_data_reels2 = get_afs_toc_data_reels_instance7(1);
    afs_toc_data_reel_set_id(GVECTORN(toc_data_reels2->reels, afs_toc_data_reel *, 0), "");

    test_equals_toc_data_reels(toc_data_reels1, toc_data_reels2, DFALSE);

    afs_toc_data_reels_free(toc_data_reels1);
    afs_toc_data_reels_free(toc_data_reels2);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels pointers equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_add_reel_test1)
{
    afs_toc_data_reels * toc_data_reels = NULL;
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels pointers equal to NULL and valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test2)
{
    afs_toc_data_reels * toc_data_reels = NULL;
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("Reel id 0", 0);

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels pointers equal to NULL and not valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test3)
{
    afs_toc_data_reels * toc_data_reels = NULL;
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("", 0);

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with empty input afs_toc_data_reels instance and afs_toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reels_add_reel_test4)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with empty input afs_toc_data_reels instance and empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test5)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with empty input afs_toc_data_reels instance and not valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test6)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("", 0);

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with empty input afs_toc_data_reels instance and valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test7)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("Reel id 0", 3);

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 1);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels instance with empty reels vector and afs_toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reels_add_reel_test8)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels instance with empty reels vector and empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test9)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels instance with empty reels vector and not valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test10)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("", 0);

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels instance with empty reels vector and valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test11)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("Reel id 0", 3);

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 1);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels instance with filled reels vector and afs_toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reels_add_reel_test12)
{
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(5);
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reels(toc_data_reels, 5);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels instance with filled reels vector and empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test13)
{
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(5);
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reels(toc_data_reels, 5);

    afs_toc_data_reels_free(toc_data_reels);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels instance with filled reels vector and not valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test14)
{
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(5);
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("", 5);

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reels(toc_data_reels, 5);

    afs_toc_data_reels_free(toc_data_reels);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reels_add_reel with input afs_toc_data_reels instance with filled reels vector and valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reels_add_reel_test15)
{
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(5);
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance7("Reel id 5", 3);

    DBOOL result = afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 6);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_get_reel with input afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_get_reel_test1)
{
    afs_toc_data_reels * toc_data_reels = NULL;
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reels_get_reel(toc_data_reels, 0);

    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
}
END_TEST


// Test function afs_toc_data_reels_get_reel with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_get_reel_test2)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reels_get_reel(toc_data_reels, 0);

    test_afs_toc_data_reels(toc_data_reels, 0);
    BOXING_ASSERT(toc_data_reel == NULL);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_get_reel with afs_toc_files instance with empty reels vector
BOXING_START_TEST(afs_toc_data_reels_get_reel_test3)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reels_get_reel(toc_data_reels, 0);

    test_afs_toc_data_reels(toc_data_reels, 0);
    BOXING_ASSERT(toc_data_reel == NULL);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_get_reel with not empty afs_toc_data_reels instance and index out of range
BOXING_START_TEST(afs_toc_data_reels_get_reel_test4)
{
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(1);

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reels_get_reel(toc_data_reels, 1);

    test_afs_toc_data_reels(toc_data_reels, 1);
    BOXING_ASSERT(toc_data_reel == NULL);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_get_reel with not empty afs_toc_data_reels instance and index in range
BOXING_START_TEST(afs_toc_data_reels_get_reel_test5)
{
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(5);

    for (unsigned int i = 0; i < afs_toc_data_reels_get_reels_count(toc_data_reels); i++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_reels_get_reel(toc_data_reels, i);
        BOXING_ASSERT(afs_toc_data_reel_equal(GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, i), toc_data_reel) == DTRUE);
    }

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_get_reels_count with input afs_toc_data_reels pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reels_get_reels_count_test1)
{
    afs_toc_data_reels * toc_data_reels = NULL;
    unsigned int reels_count = afs_toc_data_reels_get_reels_count(toc_data_reels);

    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(reels_count == 0);
}
END_TEST


// Test function afs_toc_data_reels_get_reels_count with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_get_reels_count_test2)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();
    unsigned int reels_count = afs_toc_data_reels_get_reels_count(toc_data_reels);

    test_afs_toc_data_reels(toc_data_reels, 0);
    BOXING_ASSERT(reels_count == 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_get_reels_count with afs_toc_data_reels instance with empty reels vector
BOXING_START_TEST(afs_toc_data_reels_get_reels_count_test3)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);

    unsigned int reels_count = afs_toc_data_reels_get_reels_count(toc_data_reels);

    test_afs_toc_data_reels(toc_data_reels, 0);
    BOXING_ASSERT(reels_count == 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_get_reels_count with afs_toc_data_reels instance with filled reels vector
BOXING_START_TEST(afs_toc_data_reels_get_reels_count_test4)
{
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(5);

    unsigned int reels_count = afs_toc_data_reels_get_reels_count(toc_data_reels);

    test_afs_toc_data_reels(toc_data_reels, 5);
    BOXING_ASSERT(reels_count == 5);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_is_valid with input afs_toc_data_reels pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reels_is_valid_test1)
{
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_is_valid(toc_data_reels);

    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reels_is_valid with empty input afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_is_valid_test2)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_is_valid(toc_data_reels);

    test_afs_toc_data_reels(toc_data_reels, 0);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_is_valid with input afs_toc_data_reels instance with empty reels vector
BOXING_START_TEST(afs_toc_data_reels_is_valid_test3)
{
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);

    DBOOL result = afs_toc_data_reels_is_valid(toc_data_reels);

    test_afs_toc_data_reels(toc_data_reels, 0);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_is_valid with input afs_toc_data_reels instance where reels vector filled not valid data
BOXING_START_TEST(afs_toc_data_reels_is_valid_test4)
{
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(5);

    test_afs_toc_data_reels(toc_data_reels, 5);

    afs_toc_data_reel_set_id(GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, 1), "");

    DBOOL result = afs_toc_data_reels_is_valid(toc_data_reels);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_is_valid with input afs_toc_files instance where vector filled valid data
BOXING_START_TEST(afs_toc_data_reels_is_valid_test5)
{
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(5);

    DBOOL result = afs_toc_data_reels_is_valid(toc_data_reels);

    test_afs_toc_data_reels(toc_data_reels, 5);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_file with file_name pointer equal to NULL and afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_save_file(toc_data_reels, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reels_save_file with file_name pointer equal to NULL and afs_toc_data_reels pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_reels_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(3);

    DBOOL result = afs_toc_data_reels_save_file(toc_data_reels, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_file with file_name pointer not equal to NULL and afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_save_file_test3)
{
    const char * file_name = "afs_toc_data_reels_save_file_test.xml";
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_save_file(toc_data_reels, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reels_save_file with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_save_file_test4)
{
    const char * file_name = "afs_toc_data_reels_save_file_empty_compact_test.xml";
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_save_file(toc_data_reels, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_file with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_save_file_test5)
{
    const char * file_name = "afs_toc_data_reels_save_file_empty_not_compact_test.xml";
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_save_file(toc_data_reels, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_file with afs_toc_data_reels instance with empty reels vector
BOXING_START_TEST(afs_toc_data_reels_save_file_test6)
{
    const char * file_name = "afs_toc_data_reels_save_file_empty_vector_compact_test.xml";
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);

    DBOOL result = afs_toc_data_reels_save_file(toc_data_reels, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_file with afs_toc_data_reels instance with empty reels vector
BOXING_START_TEST(afs_toc_data_reels_save_file_test7)
{
    const char * file_name = "afs_toc_data_reels_save_file_empty_vector_not_compact_test.xml";
    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);

    DBOOL result = afs_toc_data_reels_save_file(toc_data_reels, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_file with not valid afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_save_file_test8)
{
    const char * file_name = "afs_toc_data_reels_save_file_not_valid_test.xml";
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(3);
    afs_toc_data_reel_set_id(GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, 1), "");

    DBOOL result = afs_toc_data_reels_save_file(toc_data_reels, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_file with afs_toc_data_reels instance with filled reels vector, in compact format
BOXING_START_TEST(afs_toc_data_reels_save_file_test9)
{
    const char * file_name = "afs_toc_data_reels_save_file_compact_test.xml";

    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(3);

    DBOOL result = afs_toc_data_reels_save_file(toc_data_reels, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_file with afs_toc_data_reels instance with filled reels vector, in not compact format
BOXING_START_TEST(afs_toc_data_reels_save_file_test10)
{
    const char * file_name = "afs_toc_data_reels_save_file_not_compact_test.xml";

    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(3);

    DBOOL result = afs_toc_data_reels_save_file(toc_data_reels, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_string with afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_save_string_test1)
{
    afs_toc_data_reels * toc_data_reels = NULL;

    char * result = afs_toc_data_reels_save_string(toc_data_reels, DTRUE);

    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_data_reels_save_string with empty afs_toc_data_reels instance in compact format
BOXING_START_TEST(afs_toc_data_reels_save_string_test2)
{
    char * test_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_empty_compact_test.xml");

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    char * result = afs_toc_data_reels_save_string(toc_data_reels, DTRUE);

    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_data_reels_free(toc_data_reels);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_data_reels_save_string with empty afs_toc_data_reels instance in not compact format
BOXING_START_TEST(afs_toc_data_reels_save_string_test3)
{
    char * test_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_empty_not_compact_test.xml");

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    char * result = afs_toc_data_reels_save_string(toc_data_reels, DFALSE);

    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_data_reels_free(toc_data_reels);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_data_reels_save_string with afs_toc_data_reels instance with empty reels vector in compact format
BOXING_START_TEST(afs_toc_data_reels_save_string_test4)
{
    char * test_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_empty_vector_compact_test.xml");

    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);

    char * result = afs_toc_data_reels_save_string(toc_data_reels, DTRUE);

    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_data_reels_free(toc_data_reels);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_data_reels_save_string with afs_toc_data_reels instance with empty reels vector in not compact format
BOXING_START_TEST(afs_toc_data_reels_save_string_test5)
{
    char * test_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_empty_vector_not_compact_test.xml");

    afs_toc_data_reels_vector * reels = gvector_create_pointers(0);
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create2(reels);

    char * result = afs_toc_data_reels_save_string(toc_data_reels, DFALSE);

    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_data_reels_free(toc_data_reels);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_data_reels_save_string with not valid afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_save_string_test6)
{
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(3);
    afs_toc_data_reel_set_id(GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, 1), "");

    char * result = afs_toc_data_reels_save_string(toc_data_reels, DTRUE);

    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_string with valid afs_toc_data_reels instance, in compact format
BOXING_START_TEST(afs_toc_data_reels_save_string_test7)
{
    char * test_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_compact_test.xml");

    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(3);

    char * result = afs_toc_data_reels_save_string(toc_data_reels, DTRUE);

    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_data_reels_free(toc_data_reels);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_data_reels_save_string with valid afs_toc_data_reels instance, in not compact format
BOXING_START_TEST(afs_toc_data_reels_save_string_test8)
{
    char * test_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_not_compact_test.xml");

    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(3);

    char * result = afs_toc_data_reels_save_string(toc_data_reels, DFALSE);

    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_data_reels_free(toc_data_reels);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_data_reels_save_xml with mxml_node_t pointer equal to NULL and afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_save_xml(toc_data_reels, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reels_save_xml with mxml_node_t pointer equal to NULL and afs_toc_data_reels pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_reels_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(3);

    DBOOL result = afs_toc_data_reels_save_xml(toc_data_reels, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_save_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reels_save_xml with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_save_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_save_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reels_save_xml with not valid afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(3);
    afs_toc_data_reel_set_id(GVECTORN(toc_data_reels->reels, afs_toc_data_reel *, 1), "");

    DBOOL result = afs_toc_data_reels_save_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reels_save_xml with valid afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reels_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_data_reels * toc_data_reels = get_afs_toc_data_reels_instance7(3);

    DBOOL result = afs_toc_data_reels_save_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reels_free(toc_data_reels);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reels_load_file with file_name pointer equal to NULL and afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_load_file(toc_data_reels, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reels_load_file with file_name pointer equal to NULL and afs_toc_data_reels pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_reels_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_file(toc_data_reels, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_file with file_name pointer not equal to NULL and afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_load_file_test3)
{
    const char * file_name = "afs_toc_data_reels_save_file_compact_test.xml";
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_load_file(toc_data_reels, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reels_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_data_reels_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_file(toc_data_reels, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_file with empty afs_toc_data_reels instance, in compact format
BOXING_START_TEST(afs_toc_data_reels_load_file_test5)
{
    const char * file_name = "afs_toc_data_reels_save_file_empty_compact_test.xml";

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_file(toc_data_reels, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_file with empty afs_toc_data_reels instance, in not compact format
BOXING_START_TEST(afs_toc_data_reels_load_file_test6)
{
    const char * file_name = "afs_toc_data_reels_save_file_empty_not_compact_test.xml";

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_file(toc_data_reels, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_file with afs_toc_data_reels instance with empty reels vector, in compact format
BOXING_START_TEST(afs_toc_data_reels_load_file_test7)
{
    const char * file_name = "afs_toc_data_reels_save_file_empty_vector_compact_test.xml";

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_file(toc_data_reels, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_file with afs_toc_data_reels instance with empty reels vector, in not compact format
BOXING_START_TEST(afs_toc_data_reels_load_file_test8)
{
    const char * file_name = "afs_toc_data_reels_save_file_empty_vector_not_compact_test.xml";

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_file(toc_data_reels, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_file with initialized afs_toc_data_reels instance, in compact format
BOXING_START_TEST(afs_toc_data_reels_load_file_test9)
{
    const char * file_name = "afs_toc_data_reels_save_file_compact_test.xml";

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_file(toc_data_reels, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 3);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_file with initialized afs_toc_data_reels instance, in not compact format
BOXING_START_TEST(afs_toc_data_reels_load_file_test10)
{
    const char * file_name = "afs_toc_data_reels_save_file_not_compact_test.xml";
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_file(toc_data_reels, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 3);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_string with input string pointer equal to NULL and afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_load_string(toc_data_reels, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reels_load_string with input string pointer equal to NULL and afs_toc_data_reels pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_reels_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_string(toc_data_reels, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_string with input string pointer not equal to NULL and afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_load_string_test3)
{
    char * input_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_compact_test.xml");
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_load_string(toc_data_reels, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_data_reels_load_string with empty input string
BOXING_START_TEST(afs_toc_data_reels_load_string_test4)
{
    char * input_string = "";
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_string(toc_data_reels, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_string with input string for empty afs_toc_data_reels instance, in compact format
BOXING_START_TEST(afs_toc_data_reels_load_string_test5)
{
    char * input_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_empty_compact_test.xml");

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_string(toc_data_reels, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_reels_load_string with input string for empty afs_toc_data_reels instance, in not compact format
BOXING_START_TEST(afs_toc_data_reels_load_string_test6)
{
    char * input_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_empty_not_compact_test.xml");

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_string(toc_data_reels, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_reels_load_string with input string for afs_toc_data_reels instance with empty reels vetor, in compact format
BOXING_START_TEST(afs_toc_data_reels_load_string_test7)
{
    char * input_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_empty_vector_compact_test.xml");

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_string(toc_data_reels, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_reels_load_string with input string for afs_toc_data_reels instance with empty reels vector, in not compact format
BOXING_START_TEST(afs_toc_data_reels_load_string_test8)
{
    char * input_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_empty_vector_not_compact_test.xml");

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_string(toc_data_reels, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_reels_load_string with input string for initialized afs_toc_data_reels instance, in compact format
BOXING_START_TEST(afs_toc_data_reels_load_string_test9)
{
    char * input_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_compact_test.xml");

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_string(toc_data_reels, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 3);

    afs_toc_data_reels_free(toc_data_reels);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_reels_load_string with input string for initialized afs_toc_data_reels instance, in not compact format
BOXING_START_TEST(afs_toc_data_reels_load_string_test10)
{
    char * input_string = read_xml_toc_data_reels("afs_toc_data_reels_save_file_not_compact_test.xml");

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_string(toc_data_reels, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 3);

    afs_toc_data_reels_free(toc_data_reels);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_reels_load_xml with xml instance pointer equal to NULL and afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_load_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reels_load_xml with xml instance pointer equal to NULL and afs_toc_data_reels pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_reels_load_xml_test2)
{
    mxml_node_t * tree = NULL;

    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
}
END_TEST


// Test function afs_toc_data_reels_load_xml with xml instance pointer not equal to NULL and afs_toc_data_reels pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reels_load_xml_test3)
{
    mxml_node_t * tree = get_toc_data_reels_xml_instance("afs_toc_data_reels_save_file_compact_test.xml");
    afs_toc_data_reels * toc_data_reels = NULL;

    DBOOL result = afs_toc_data_reels_load_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reels == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reels_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_data_reels_load_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reels_load_xml with xml instance for empty afs_toc_data_reels instance, in compact format
BOXING_START_TEST(afs_toc_data_reels_load_xml_test5)
{
    mxml_node_t * tree = get_toc_data_reels_xml_instance("afs_toc_data_reels_save_file_empty_compact_test.xml");
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reels_load_xml with xml instance for afs_toc_data_reels instance with empty reels vector, in compact format
BOXING_START_TEST(afs_toc_data_reels_load_xml_test6)
{
    mxml_node_t * tree = get_toc_data_reels_xml_instance("afs_toc_data_reels_save_file_empty_vector_compact_test.xml");
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 0);

    afs_toc_data_reels_free(toc_data_reels);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reels_load_xml with xml instance for initialized afs_toc_data_reels instance, in compact format
BOXING_START_TEST(afs_toc_data_reels_load_xml_test7)
{
    mxml_node_t * tree = get_toc_data_reels_xml_instance("afs_toc_data_reels_save_file_compact_test.xml");
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 3);

    afs_toc_data_reels_free(toc_data_reels);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reels_load_xml with xml instance for initialized afs_toc_data_reels instance, in not compact format
BOXING_START_TEST(afs_toc_data_reels_load_xml_test8)
{
    mxml_node_t * tree = get_toc_data_reels_xml_instance("afs_toc_data_reels_save_file_not_compact_test.xml");
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    DBOOL result = afs_toc_data_reels_load_xml(toc_data_reels, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reels(toc_data_reels, 3);

    afs_toc_data_reels_free(toc_data_reels);
    mxmlDelete(tree);
}
END_TEST


Suite * tocdatareels_tests(void)
{
    TCase * tc_tocdatareels_functions_tests = tcase_create("tocdatareels_functions_tests");

    // Test function afs_toc_data_reels_create
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_create_test1);
    // Test function afs_toc_data_reels_create2
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_create2_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_create2_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_create2_test3);
    // Test function afs_toc_data_reels_clone
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_clone_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_clone_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_clone_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_clone_test4);
    // Test function afs_toc_data_reels_get_new_reference
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_new_reference_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_new_reference_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_new_reference_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_new_reference_test4);
    // Test function afs_toc_data_reels_equal
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test4);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test5);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test6);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test7);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test8);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test9);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test10);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test11);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test12);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_equal_test13);
    // Test function afs_toc_data_reels_add_reel
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test4);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test5);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test6);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test7);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test8);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test9);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test10);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test11);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test12);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test13);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test14);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_add_reel_test15);
    // Test function afs_toc_data_reels_get_reel
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_reel_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_reel_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_reel_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_reel_test4);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_reel_test5);
    // Test function afs_toc_data_reels_get_reels_count
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_reels_count_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_reels_count_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_reels_count_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_get_reels_count_test4);
    // Test function afs_toc_data_reels_is_valid
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_is_valid_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_is_valid_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_is_valid_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_is_valid_test4);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_is_valid_test5);
    // Test function afs_toc_data_reels_save_file
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_file_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_file_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_file_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_file_test4);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_file_test5);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_file_test6);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_file_test7);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_file_test8);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_file_test9);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_file_test10);
    // Test function afs_toc_data_reels_save_string
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_string_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_string_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_string_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_string_test4);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_string_test5);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_string_test6);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_string_test7);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_string_test8);
    // Test function afs_toc_data_reels_save_xml
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_xml_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_xml_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_xml_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_xml_test4);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_xml_test5);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_save_xml_test6);
    // Test function afs_toc_data_reels_load_file
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_file_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_file_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_file_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_file_test4);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_file_test5);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_file_test6);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_file_test7);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_file_test8);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_file_test9);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_file_test10);
    // Test function afs_toc_data_reels_load_string
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_string_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_string_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_string_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_string_test4);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_string_test5);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_string_test6);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_string_test7);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_string_test8);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_string_test9);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_string_test10);
    // Test function afs_toc_data_reels_load_xml
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_xml_test1);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_xml_test2);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_xml_test3);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_xml_test4);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_xml_test5);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_xml_test6);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_xml_test7);
    tcase_add_test(tc_tocdatareels_functions_tests, afs_toc_data_reels_load_xml_test8);

    Suite * s = suite_create("tocdatareels_test_util");
    suite_add_tcase(s, tc_tocdatareels_functions_tests);

    return s;
}
