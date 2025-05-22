#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "tocdatareel_c.h"
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

static afs_toc_file_preview_pages* get_filled_pages_vector6(unsigned int pages_count)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(pages_count);

    for (unsigned int i = 0; i < pages_count; i++)
    {
        GVECTORN(pages, afs_toc_file_preview_page *, i) = afs_toc_file_preview_page_create2("Some layout ID", 9 + i, 8 + i, 7 + i, 6 + i, 5 + i, 4 + i, 3 + i);
    }

    return pages;
}


static afs_toc_data_file_metadata_sources * get_filled_toc_data_file_metadata_sources_vector6(unsigned int sources_count)
{
    afs_toc_data_file_metadata_sources * sources = gvector_create_pointers(sources_count);

    for (unsigned int i = 0; i < sources_count; i++)
    {
        GVECTORN(sources, afs_toc_data_file_metadata_source *, i) = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", i + 1, i + 2);
    }

    return sources;
}


static afs_toc_data_file_metadata * get_afs_toc_data_file_metadata_instance6(unsigned int sources_count)
{
    afs_toc_data_file_metadata_sources * sources = get_filled_toc_data_file_metadata_sources_vector6(sources_count);
    afs_toc_data_file_metadata * toc_data_file_metadata = afs_toc_data_file_metadata_create2(sources);

    return toc_data_file_metadata;
}


static afs_toc_file * get_afs_toc_file_instance6(unsigned int number)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector6(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance6(5);
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


static afs_toc_files * get_afs_toc_files_instance6(const size_t files_count)
{
    afs_toc_indices * tocs = gvector_create_pointers(files_count);

    for (size_t i = 0; i < files_count; i++)
    {
        afs_toc_file * toc_file = get_afs_toc_file_instance6((unsigned int)i);
        GVECTORN(tocs, afs_toc_file *, i) = toc_file;
    }

    return afs_toc_files_create2(tocs);
}


afs_toc_data_reel * get_afs_toc_data_reel_instance6(const char * id, unsigned int files_count)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2(id);
    toc_data_reel->files = get_afs_toc_files_instance6(files_count);

    return toc_data_reel;
}


static void test_afs_toc_data_reel(afs_toc_data_reel * toc_data_reel, const char * id, const size_t files_count)
{
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(strcmp(id, toc_data_reel->id) == 0);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_data_reel->files) == files_count);

    for (size_t i = 0; i < files_count; i++)
    {
        afs_toc_file* afs_toc_file_instance = get_afs_toc_file_instance6((unsigned int)i);
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i), afs_toc_file_instance) == DTRUE);
        afs_toc_file_free(afs_toc_file_instance);
    }
}


static char * read_xml_toc_data_reel(const char * file_name)
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


static mxml_node_t * get_toc_data_reel_xml_instance(const char * file_name)
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


static void test_equals_toc_data_reel(afs_toc_data_reel * toc_data_reel1, afs_toc_data_reel * toc_data_reel2, DBOOL expected_result)
{
    DBOOL result = afs_toc_data_reel_equal(toc_data_reel2, toc_data_reel1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_data_reel_equal(toc_data_reel1, toc_data_reel2);
    BOXING_ASSERT(result == expected_result);
}


afs_toc_data_reel * get_afs_toc_data_reel_no_parent()
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("Some reel id string!", 3);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->types = AFS_TOC_FILE_TYPE_DIGITAL;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1)->types = AFS_TOC_FILE_TYPE_PREVIEW;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2)->types = AFS_TOC_FILE_TYPE_ALL;

    return toc_data_reel;
}


afs_toc_data_reel * get_afs_toc_data_reel_parent()
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("Some reel id string!", 3);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->types = AFS_TOC_FILE_TYPE_DIGITAL;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->parent_id = AFS_TOC_FILE_PARENT;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1)->types = AFS_TOC_FILE_TYPE_PREVIEW;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1)->parent_id = AFS_TOC_FILE_PARENT;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2)->types = AFS_TOC_FILE_TYPE_ALL;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2)->parent_id = AFS_TOC_FILE_PARENT;

    return toc_data_reel;
}


afs_toc_data_reel * get_afs_toc_data_reel_different_parent()
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("Some reel id string!", 6);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->types = AFS_TOC_FILE_TYPE_DIGITAL;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->parent_id = AFS_TOC_FILE_PARENT;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1)->types = AFS_TOC_FILE_TYPE_PREVIEW;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1)->parent_id = AFS_TOC_FILE_PARENT;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2)->types = AFS_TOC_FILE_TYPE_ALL;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2)->parent_id = AFS_TOC_FILE_PARENT;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 3)->types = AFS_TOC_FILE_TYPE_DIGITAL;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 3)->parent_id = AFS_TOC_FILE_NO_PARENT;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 4)->types = AFS_TOC_FILE_TYPE_PREVIEW;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 4)->parent_id = AFS_TOC_FILE_NO_PARENT;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 5)->types = AFS_TOC_FILE_TYPE_ALL;
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 5)->parent_id = AFS_TOC_FILE_NO_PARENT;

    return toc_data_reel;
}


afs_toc_data_reel * get_afs_toc_data_reel_incorrect_start_frame()
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i)->start_frame = -1;
    }
    return toc_data_reel;
}


afs_toc_data_reel * get_afs_toc_data_reel_incorrect_end_frame()
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i)->end_frame = -1;
    }
    return toc_data_reel;
}


afs_toc_data_reel * get_afs_toc_data_reel_start_frame_larger_end_frame()
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i)->start_frame = i + 50;
    }
    return toc_data_reel;
}


// TEST TOC DATA REEL FUNCTIONS
//


// Test function afs_toc_data_reel_create
BOXING_START_TEST(afs_toc_data_reel_create_test1)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_create2 with id pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_create2_test1)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2(NULL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_create2 with empty id string
BOXING_START_TEST(afs_toc_data_reel_create2_test2)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("");

    test_afs_toc_data_reel(toc_data_reel, "", 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_create2 with not empty id string
BOXING_START_TEST(afs_toc_data_reel_create2_test3)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("Some id.");

    test_afs_toc_data_reel(toc_data_reel, "Some id.", 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_clone with input pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_clone_test1)
{
    afs_toc_data_reel * toc_data_reel1 = NULL;

    BOXING_ASSERT(toc_data_reel1 == NULL);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_clone(toc_data_reel1);

    BOXING_ASSERT(toc_data_reel2 == NULL);
}
END_TEST


// Test function afs_toc_data_reel_clone with empty input afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_clone_test2)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create();

    test_afs_toc_data_reel(toc_data_reel1, NULL, 0);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_clone(toc_data_reel1);

    test_afs_toc_data_reel(toc_data_reel2, NULL, 0);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_clone with not empty input afs_toc_data_reel instance and empty id string and afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_clone_test3)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create2("");
    toc_data_reel1->files = toc_files;

    test_afs_toc_data_reel(toc_data_reel1, "", 0);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_clone(toc_data_reel1);

    test_afs_toc_data_reel(toc_data_reel2, "", 0);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_clone test with not empty input afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_clone_test4)
{
    afs_toc_data_reel * toc_data_reel1 = get_afs_toc_data_reel_instance6("Some id.", 3);

    test_afs_toc_data_reel(toc_data_reel1, "Some id.", 3);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_clone(toc_data_reel1);

    test_afs_toc_data_reel(toc_data_reel2, "Some id.", 3);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_get_new_reference_test1)
{
    afs_toc_data_reel * toc_data_reel1 = NULL;

    BOXING_ASSERT(toc_data_reel1 == NULL);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_get_new_reference(toc_data_reel1);

    BOXING_ASSERT(toc_data_reel2 == NULL);

    toc_data_reel1 = afs_toc_data_reel_create();

    test_afs_toc_data_reel(toc_data_reel1, NULL, 0);
    BOXING_ASSERT(toc_data_reel1->reference_count == 1);
    BOXING_ASSERT(toc_data_reel2 == NULL);

    afs_toc_data_reel_free(toc_data_reel1);
}
END_TEST


// Test function afs_toc_data_reel_get_new_reference with empty input afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_get_new_reference_test2)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create();

    test_afs_toc_data_reel(toc_data_reel1, NULL, 0);
    BOXING_ASSERT(toc_data_reel1->reference_count == 1);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_get_new_reference(toc_data_reel1);

    test_afs_toc_data_reel(toc_data_reel2, NULL, 0);
    BOXING_ASSERT(toc_data_reel1->reference_count == 2);
    BOXING_ASSERT(toc_data_reel2->reference_count == 2);
    BOXING_ASSERT(toc_data_reel1 == toc_data_reel2);

    afs_toc_data_reel_set_id(toc_data_reel1, "Some reel id");

    BOXING_ASSERT(strcmp(toc_data_reel2->id, "Some reel id") == 0);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel1);
}
END_TEST


// Test function afs_toc_data_reel_get_new_reference with not empty input afs_toc_data_reel instance and empty id string and afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_get_new_reference_test3)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create2("");
    toc_data_reel1->files = toc_files;

    test_afs_toc_data_reel(toc_data_reel1, "", 0);
    BOXING_ASSERT(toc_data_reel1->reference_count == 1);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_get_new_reference(toc_data_reel1);

    test_afs_toc_data_reel(toc_data_reel2, "", 0);
    BOXING_ASSERT(toc_data_reel1->reference_count == 2);
    BOXING_ASSERT(toc_data_reel2->reference_count == 2);
    BOXING_ASSERT(toc_data_reel1 == toc_data_reel2);

    afs_toc_data_reel_set_id(toc_data_reel1, "Some reel id");

    BOXING_ASSERT(strcmp(toc_data_reel2->id, "Some reel id") == 0);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel1);
}
END_TEST


// Test function afs_toc_data_reel_get_new_reference test with not empty input afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_get_new_reference_test4)
{
    afs_toc_data_reel * toc_data_reel1 = get_afs_toc_data_reel_instance6("Some id.", 3);

    test_afs_toc_data_reel(toc_data_reel1, "Some id.", 3);
    BOXING_ASSERT(toc_data_reel1->reference_count == 1);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_get_new_reference(toc_data_reel1);

    test_afs_toc_data_reel(toc_data_reel2, "Some id.", 3);
    BOXING_ASSERT(toc_data_reel1->reference_count == 2);
    BOXING_ASSERT(toc_data_reel2->reference_count == 2);
    BOXING_ASSERT(toc_data_reel1 == toc_data_reel2);

    afs_toc_data_reel_set_id(toc_data_reel1, "Some other reel id");

    BOXING_ASSERT(strcmp(toc_data_reel2->id, "Some other reel id") == 0);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel1);
}
END_TEST


// Test function afs_toc_data_reel_equal with input afs_toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reel_equal_test1)
{
    afs_toc_data_reel * toc_data_reel1 = NULL;
    afs_toc_data_reel * toc_data_reel2 = NULL;

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DTRUE);
}
END_TEST

// Test function afs_toc_data_reel_equal with empty input afs_toc_data_reel instances 
BOXING_START_TEST(afs_toc_data_reel_equal_test2)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create();
    test_afs_toc_data_reel(toc_data_reel1, NULL, 0);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_create();
    test_afs_toc_data_reel(toc_data_reel2, NULL, 0);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DTRUE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_equal with not empty input afs_toc_data_reel instances with id string equal to NULL and empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_equal_test3)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create();
    afs_toc_files * toc_files1 = afs_toc_files_create();
    toc_data_reel1->files = toc_files1;
    test_afs_toc_data_reel(toc_data_reel1, NULL, 0);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_create();
    afs_toc_files * toc_files2 = afs_toc_files_create();
    toc_data_reel2->files = toc_files2;
    test_afs_toc_data_reel(toc_data_reel2, NULL, 0);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DTRUE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_equal with not empty input afs_toc_data_reel instances with id string equal to NULL and not empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_equal_test4)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create();
    afs_toc_files * toc_files1 = get_afs_toc_files_instance6(3);
    toc_data_reel1->files = toc_files1;
    test_afs_toc_data_reel(toc_data_reel1, NULL, 3);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_create();
    afs_toc_files * toc_files2 = get_afs_toc_files_instance6(3);
    toc_data_reel2->files = toc_files2;
    test_afs_toc_data_reel(toc_data_reel2, NULL, 3);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DTRUE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_equal with not empty input afs_toc_data_reel instances with empty id string and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_equal_test5)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create2("");
    test_afs_toc_data_reel(toc_data_reel1, "", 0);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_create2("");
    test_afs_toc_data_reel(toc_data_reel2, "", 0);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DTRUE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_equal with not empty input afs_toc_data_reel instances with empty id string and empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_equal_test6)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create2("");
    afs_toc_files * toc_files1 = afs_toc_files_create();
    toc_data_reel1->files = toc_files1;
    test_afs_toc_data_reel(toc_data_reel1, "", 0);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_create2("");
    afs_toc_files * toc_files2 = afs_toc_files_create();
    toc_data_reel2->files = toc_files2;
    test_afs_toc_data_reel(toc_data_reel2, "", 0);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DTRUE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_equal with not empty input afs_toc_data_reel instances with empty id string and not empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_equal_test7)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create2("");
    afs_toc_files * toc_files1 = get_afs_toc_files_instance6(3);
    toc_data_reel1->files = toc_files1;
    test_afs_toc_data_reel(toc_data_reel1, "", 3);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_create2("");
    afs_toc_files * toc_files2 = get_afs_toc_files_instance6(3);
    toc_data_reel2->files = toc_files2;
    test_afs_toc_data_reel(toc_data_reel2, "", 3);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DTRUE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_equal with not empty input afs_toc_data_reel instances with not empty id string and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_equal_test8)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create2("Some id!@#");
    test_afs_toc_data_reel(toc_data_reel1, "Some id!@#", 0);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_create2("Some id!@#");
    test_afs_toc_data_reel(toc_data_reel2, "Some id!@#", 0);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DTRUE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_equal with not empty input afs_toc_data_reel instances with not empty id string and empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_equal_test9)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create2("Some id!@#");
    afs_toc_files * toc_files1 = afs_toc_files_create();
    toc_data_reel1->files = toc_files1;
    test_afs_toc_data_reel(toc_data_reel1, "Some id!@#", 0);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_create2("Some id!@#");
    afs_toc_files * toc_files2 = afs_toc_files_create();
    toc_data_reel2->files = toc_files2;
    test_afs_toc_data_reel(toc_data_reel2, "Some id!@#", 0);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DTRUE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_equal with not empty input afs_toc_data_reel instances with not empty id string and not empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_equal_test10)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create2("Some id!@#");
    afs_toc_files * toc_files1 = get_afs_toc_files_instance6(3);
    toc_data_reel1->files = toc_files1;
    test_afs_toc_data_reel(toc_data_reel1, "Some id!@#", 3);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_create2("Some id!@#");
    afs_toc_files * toc_files2 = get_afs_toc_files_instance6(3);
    toc_data_reel2->files = toc_files2;
    test_afs_toc_data_reel(toc_data_reel2, "Some id!@#", 3);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DTRUE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_equal with empty input afs_toc_data_reel instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_data_reel_equal_test11)
{
    afs_toc_data_reel * toc_data_reel1 = afs_toc_data_reel_create();
    test_afs_toc_data_reel(toc_data_reel1, NULL, 0);

    afs_toc_data_reel * toc_data_reel2 = NULL;

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DFALSE);

    afs_toc_data_reel_free(toc_data_reel1);
}
END_TEST


// Test function afs_toc_data_reel_equal with empty input afs_toc_data_reel instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_data_reel_equal_test12)
{
    afs_toc_data_reel * toc_data_reel1 = get_afs_toc_data_reel_instance6("Some id!@#", 3);
    test_afs_toc_data_reel(toc_data_reel1, "Some id!@#", 3);

    afs_toc_data_reel * toc_data_reel2 = NULL;

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DFALSE);

    afs_toc_data_reel_free(toc_data_reel1);
}
END_TEST


// Test function afs_toc_data_reel_equal with empty input afs_toc_data_reel instance and not empty input afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_equal_test13)
{
    afs_toc_data_reel * toc_data_reel1 = get_afs_toc_data_reel_instance6("Some id!@#", 3);
    test_afs_toc_data_reel(toc_data_reel1, "Some id!@#", 3);

    afs_toc_data_reel * toc_data_reel2 = afs_toc_data_reel_create();

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DFALSE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_equal with not empty input afs_toc_data_reel instances with different id
BOXING_START_TEST(afs_toc_data_reel_equal_test14)
{
    afs_toc_data_reel * toc_data_reel1 = get_afs_toc_data_reel_instance6("Some id!@#", 3);
    test_afs_toc_data_reel(toc_data_reel1, "Some id!@#", 3);

    afs_toc_data_reel * toc_data_reel2 = get_afs_toc_data_reel_instance6("Some id!@#&", 3);
    test_afs_toc_data_reel(toc_data_reel2, "Some id!@#&", 3);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DFALSE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST
// Test function afs_toc_data_reel_equal with not empty input afs_toc_data_reel instances with different afs_toc_files instances
BOXING_START_TEST(afs_toc_data_reel_equal_test15)
{
    afs_toc_data_reel * toc_data_reel1 = get_afs_toc_data_reel_instance6("Some id!@#", 3);
    test_afs_toc_data_reel(toc_data_reel1, "Some id!@#", 3);

    afs_toc_data_reel * toc_data_reel2 = get_afs_toc_data_reel_instance6("Some id!@#", 4);
    test_afs_toc_data_reel(toc_data_reel2, "Some id!@#", 4);

    test_equals_toc_data_reel(toc_data_reel2, toc_data_reel1, DFALSE);

    afs_toc_data_reel_free(toc_data_reel1);
    afs_toc_data_reel_free(toc_data_reel2);
}
END_TEST


// Test function afs_toc_data_reel_set_id with input afs_toc_data_reel pointers equal to NULL and id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_set_id_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const char * id = NULL;

    afs_toc_data_reel_set_id(toc_data_reel, id);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(id == NULL);
}
END_TEST


// Test function afs_toc_data_reel_set_id with input afs_toc_data_reel pointers equal to NULL and empty id string
BOXING_START_TEST(afs_toc_data_reel_set_id_test2)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const char * id = "";

    afs_toc_data_reel_set_id(toc_data_reel, id);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(strcmp(id, "") == 0);
}
END_TEST


// Test function afs_toc_data_reel_set_id with input afs_toc_data_reel pointers equal to NULL and not empty id string
BOXING_START_TEST(afs_toc_data_reel_set_id_test3)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const char * id = "some id string";

    afs_toc_data_reel_set_id(toc_data_reel, id);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(strcmp(id, "some id string") == 0);
}
END_TEST


// Test function afs_toc_data_reel_set_id with empty input afs_toc_data_reel instance and id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_set_id_test4)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const char * id = NULL;

    afs_toc_data_reel_set_id(toc_data_reel, id);

    BOXING_ASSERT(toc_data_reel->id == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_set_id with empty input afs_toc_data_reel instance and empty id string
BOXING_START_TEST(afs_toc_data_reel_set_id_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    BOXING_ASSERT(toc_data_reel->id == NULL);

    const char * id = "";

    afs_toc_data_reel_set_id(toc_data_reel, id);

    BOXING_ASSERT(strcmp(toc_data_reel->id, "") == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_set_id with empty input afs_toc_data_reel instance and not empty id string
BOXING_START_TEST(afs_toc_data_reel_set_id_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    BOXING_ASSERT(toc_data_reel->id == NULL);

    const char * id = "some id string";

    afs_toc_data_reel_set_id(toc_data_reel, id);

    BOXING_ASSERT(strcmp(toc_data_reel->id, "some id string") == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_set_id with not empty input afs_toc_data_reel instance and id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_set_id_test7)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);
    BOXING_ASSERT(strcmp(toc_data_reel->id, "some id string") == 0);

    const char * id = NULL;

    afs_toc_data_reel_set_id(toc_data_reel, id);

    BOXING_ASSERT(toc_data_reel->id == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_set_id with not empty input afs_toc_data_reel instance and empty id string
BOXING_START_TEST(afs_toc_data_reel_set_id_test8)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);
    BOXING_ASSERT(strcmp(toc_data_reel->id, "some id string") == 0);

    const char * id = "";

    afs_toc_data_reel_set_id(toc_data_reel, id);

    BOXING_ASSERT(strcmp(toc_data_reel->id, "") == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_set_id with not empty input afs_toc_data_reel instance and not empty id string
BOXING_START_TEST(afs_toc_data_reel_set_id_test9)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);
    BOXING_ASSERT(strcmp(toc_data_reel->id, "some id string") == 0);

    const char * id = "some other id string";

    afs_toc_data_reel_set_id(toc_data_reel, id);

    BOXING_ASSERT(strcmp(toc_data_reel->id, "some other id string") == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_file with input afs_toc_data_reel pointer equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_add_file_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    afs_toc_file * toc_file = NULL;

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reel_add_file with input afs_toc_data_reel pointer equal to NULL and valid afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_add_file_test2)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    afs_toc_file * toc_file = get_afs_toc_file_instance6(1);

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_data_reel_add_file with input afs_toc_data_reel pointer equal to NULL and not valid afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_add_file_test3)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    afs_toc_file * toc_file = get_afs_toc_file_instance6(1);
    toc_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_file->start_frame = -1;

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_data_reel_add_file with empty input afs_toc_data_reel instance and afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reel_add_file_test4)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_file * toc_file = NULL;

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_file with empty input afs_toc_data_reel instance and empty afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_add_file_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_file * toc_file = afs_toc_file_create();

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_data_reel_add_file with empty input afs_toc_data_reel instance and not valid afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_add_file_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_file * toc_file = get_afs_toc_file_instance6(1);
    toc_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_file->start_frame = -1;

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_data_reel_add_file with empty input afs_toc_data_reel instance and valid afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_add_file_test7)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_file * toc_file = get_afs_toc_file_instance6(0);

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, NULL, 1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_file with input afs_toc_data_reel instance with empty afs_toc_files instance and afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reel_add_file_test8)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("Some id string!@#$");
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    afs_toc_file * toc_file = NULL;

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_file with input afs_toc_data_reel instance with empty afs_toc_files instance and empty afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_add_file_test9)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("Some id string!@#$");
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    afs_toc_file * toc_file = afs_toc_file_create();

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_data_reel_add_file with input afs_toc_data_reel instance with empty afs_toc_files instance and not valid afs_toc_file
BOXING_START_TEST(afs_toc_data_reel_add_file_test10)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("Some id string!@#$");
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    afs_toc_file * toc_file = get_afs_toc_file_instance6(1);
    toc_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_file->start_frame = -1;

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_data_reel_add_file with input afs_toc_data_reel instance with empty afs_toc_files instance and valid afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_add_file_test11)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("Some id string!@#$");
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    afs_toc_file * toc_file = get_afs_toc_file_instance6(0);

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "Some id string!@#$", 1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_file with not empty input afs_toc_data_reel instance and afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reel_add_file_test12)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);
    afs_toc_file * toc_file = NULL;

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 5);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_file with not empty input afs_toc_data_reel instance and empty afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_add_file_test13)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);
    afs_toc_file * toc_file = afs_toc_file_create();

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 5);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_data_reel_add_file with not empty input afs_toc_data_reel instance and not valid afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_add_file_test14)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);

    afs_toc_file * toc_file = get_afs_toc_file_instance6(1);
    toc_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_file->start_frame = -1;

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 5);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_data_reel_add_file with not empty input afs_toc_data_reel instance and valid afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_add_file_test15)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);
    afs_toc_file * toc_file = get_afs_toc_file_instance6(5);

    DBOOL result = afs_toc_data_reel_add_file(toc_data_reel, toc_file);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 6);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_index with input afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_get_file_by_index_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, 0);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(toc_file == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_index with empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_get_file_by_index_test2)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, 0);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(toc_file == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_index with not empty afs_toc_data_reel instance and index out of range
BOXING_START_TEST(afs_toc_data_reel_get_file_by_index_test3)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);

    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, 5);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 5);
    BOXING_ASSERT(toc_file == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_index with not empty afs_toc_data_reel instance and index in range
BOXING_START_TEST(afs_toc_data_reel_get_file_by_index_test4)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 6);

    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, i);
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i), toc_file) == DTRUE);
    }

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_id with input afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_get_file_by_id_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_id(toc_data_reel, 0);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(toc_file == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_id with empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_get_file_by_id_test2)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_id(toc_data_reel, 0);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(toc_file == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_id with not empty afs_toc_data_reel instance with required id not exist
BOXING_START_TEST(afs_toc_data_reel_get_file_by_id_test3)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);

    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_id(toc_data_reel, 5);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 5);
    BOXING_ASSERT(toc_file == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_id with not empty toc_data_reel instance and exist required id
BOXING_START_TEST(afs_toc_data_reel_get_file_by_id_test4)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);
    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i)->id = i;
    }

    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_id(toc_data_reel, i);
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i), toc_file) == DTRUE);
    }

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_unique_id with input toc_data_reel pointer equal to NULL and unique id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_get_file_by_unique_id_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const char * unique_id_string = NULL;
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, unique_id_string);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(unique_id_string == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_unique_id with input toc_data_reel pointer equal to NULL and empty unique id string
BOXING_START_TEST(afs_toc_data_reel_get_file_by_unique_id_test2)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const char * unique_id_string = "";
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, unique_id_string);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(strcmp(unique_id_string, "") == 0);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_unique_id with input afs_toc_data_reel pointer equal to NULL and not empty unique id string
BOXING_START_TEST(afs_toc_data_reel_get_file_by_unique_id_test3)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const char * unique_id_string = "id 0";
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, unique_id_string);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(strcmp(unique_id_string, "id 0") == 0);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_unique_id with empty afs_toc_data_reel instance and unique id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_get_file_by_unique_id_test4)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const char * unique_id_string = NULL;
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, unique_id_string);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(unique_id_string == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_unique_id with empty afs_toc_data_reel instance and empty unique id string
BOXING_START_TEST(afs_toc_data_reel_get_file_by_unique_id_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const char * unique_id_string = "";
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, unique_id_string);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(strcmp(unique_id_string, "") == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_unique_id with empty afs_toc_data_reel instance and not empty unique id string
BOXING_START_TEST(afs_toc_data_reel_get_file_by_unique_id_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const char * unique_id_string = "id 0";
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, unique_id_string);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(strcmp(unique_id_string, "id 0") == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_unique_id with not empty afs_toc_data_reel instance and unique id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_get_file_by_unique_id_test7)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("Some reel id string!", 2);
    const char * unique_id_string = NULL;
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, unique_id_string);

    test_afs_toc_data_reel(toc_data_reel, "Some reel id string!", 2);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(unique_id_string == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_unique_id with not empty afs_toc_data_reel instance and empty unique id string
BOXING_START_TEST(afs_toc_data_reel_get_file_by_unique_id_test8)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("Some reel id string!", 2);
    const char * unique_id_string = "";
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, unique_id_string);

    test_afs_toc_data_reel(toc_data_reel, "Some reel id string!", 2);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(strcmp(unique_id_string, "") == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_unique_id with not empty afs_toc_data_reel instance and not exist unique id
BOXING_START_TEST(afs_toc_data_reel_get_file_by_unique_id_test9)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("Some reel id string!", 2);
    const char * unique_id_string = "Some unique id!";
    afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, unique_id_string);

    test_afs_toc_data_reel(toc_data_reel, "Some reel id string!", 2);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(strcmp(unique_id_string, "Some unique id!") == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_file_by_unique_id with not empty afs_toc_data_reel instance and exist unique id
BOXING_START_TEST(afs_toc_data_reel_get_file_by_unique_id_test10)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("Some reel id string!", 6);

    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        char current_unique_id[100];
        sprintf(current_unique_id, "38A0AAAB-16B2-640C-5353-6DB8AE4367B%u", i);

        afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_unique_id(toc_data_reel, current_unique_id);
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i), toc_file) == DTRUE);
    }

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test2)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test3)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test4)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test7)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test8)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with not exist parent files and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test9)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with not exist parent files and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test10)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with not exist parent files and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test11)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with not exist parent files and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test12)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with all files is parent and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test13)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with all files is parent and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test14)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with all files is parent and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test15)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with all files is parent and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test16)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 3);
    for (size_t i = 0; i < afs_toc_files_get_tocs_count(toc_files); i++)
    {
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i), GVECTORN(toc_files->tocs, afs_toc_file *, i)) == DTRUE);
    }

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with different parent status and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test17)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with different parent status and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test18)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with different parent status and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test19)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_parent_files with empty afs_toc_data_reel structure instance with different parent status and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_parent_files_test20)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_parent_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 3);
    for (size_t i = 0; i < afs_toc_files_get_tocs_count(toc_files) / 2; i++)
    {
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i), GVECTORN(toc_files->tocs, afs_toc_file *, i)) == DTRUE);
    }

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test2)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test3)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test4)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test7)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test8)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with not exist child files and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test9)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with not exist child files and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test10)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with not exist child files and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test11)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with not exist child files and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test12)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with all files is child and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test13)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with all files is child and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test14)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with all files is child and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test15)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with all files is child and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test16)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 3);
    for (size_t i = 0; i < afs_toc_files_get_tocs_count(toc_files); i++)
    {
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i), GVECTORN(toc_files->tocs, afs_toc_file *, i)) == DTRUE);
    }

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with different child status and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test17)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 3), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 5), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with different child status and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test18)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 4), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 5), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with different child status and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test19)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_child_files with empty afs_toc_data_reel structure instance with different child status and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_child_files_test20)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_child_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 3);
    for (size_t i = 0; i < afs_toc_files_get_tocs_count(toc_files) / 2; i++)
    {
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i + 3), GVECTORN(toc_files->tocs, afs_toc_file *, i)) == DTRUE);
    }

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test2)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test3)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with afs_toc_data_reel structure pointer equal to NULL and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test4)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test7)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with empty afs_toc_data_reel structure instance and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test8)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with not exist standalone files and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test9)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with not exist standalone files and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test10)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with not exist standalone files and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test11)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with not exist standalone files and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test12)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with all files is standalone and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test13)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with all files is standalone and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test14)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with all files is standalone and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test15)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with all files is standalone and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test16)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_no_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 3);
    for (size_t i = 0; i < afs_toc_files_get_tocs_count(toc_files); i++)
    {
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i), GVECTORN(toc_files->tocs, afs_toc_file *, i)) == DTRUE);
    }

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with different standalone status and types equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test17)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 3), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 5), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with different standalone status and types equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test18)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 2);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 4), GVECTORN(toc_files->tocs, afs_toc_file *, 0)) == DTRUE);
    BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 5), GVECTORN(toc_files->tocs, afs_toc_file *, 1)) == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with different standalone status and types equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test19)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_standalone_files with not empty afs_toc_data_reel structure instance with different standalone status and types equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_get_standalone_files_test20)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int types = AFS_TOC_DATA_REEL_SECTION_ALL;

    afs_toc_files * toc_files = afs_toc_data_reel_get_standalone_files(toc_data_reel, types);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_files) == 3);
    for (size_t i = 0; i < afs_toc_files_get_tocs_count(toc_files) / 2; i++)
    {
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i + 3), GVECTORN(toc_files->tocs, afs_toc_file *, i)) == DTRUE);
    }

    afs_toc_data_reel_free(toc_data_reel);
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_file_count with input toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reel_file_count_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    unsigned int file_count = afs_toc_data_reel_file_count(toc_data_reel);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(file_count == 0);
}
END_TEST


// Test function afs_toc_data_reel_file_count with empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_file_count_test2)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    unsigned int file_count = afs_toc_data_reel_file_count(toc_data_reel);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(file_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_file_count with afs_toc_data_reel instance with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_file_count_test3)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    unsigned int file_count = afs_toc_data_reel_file_count(toc_data_reel);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(file_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_file_count with not empty toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_file_count_test4)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("Some reel id string!", 6);

    unsigned int file_count = afs_toc_data_reel_file_count(toc_data_reel);

    test_afs_toc_data_reel(toc_data_reel, "Some reel id string!", 6);
    BOXING_ASSERT(file_count == 6);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure pointer equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int section = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure pointer equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_frame_count_test2)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int section = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure pointer equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_frame_count_test3)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int section = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure pointer equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test4)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    const int section = AFS_TOC_DATA_REEL_SECTION_ALL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with empty afs_toc_data_reel structure instance and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int section = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with empty afs_toc_data_reel structure instance and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_frame_count_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int section = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with empty afs_toc_data_reel structure instance and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_frame_count_test7)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int section = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with empty afs_toc_data_reel structure instance and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test8)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    const int section = AFS_TOC_DATA_REEL_SECTION_ALL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test9)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;
    const int section = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_frame_count_test10)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;
    const int section = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_frame_count_test11)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;
    const int section = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test12)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;
    const int section = AFS_TOC_DATA_REEL_SECTION_ALL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with different file type and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test13)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int section = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 6);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with different file type and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_frame_count_test14)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int section = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with different file type and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_frame_count_test15)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int section = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with different file type and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test16)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();
    const int section = AFS_TOC_DATA_REEL_SECTION_ALL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 6);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with incorrect start frame and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test17)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_incorrect_start_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with incorrect start frame and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_frame_count_test18)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_incorrect_start_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with incorrect start frame and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_frame_count_test19)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_incorrect_start_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with incorrect start frame and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test20)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_incorrect_start_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_ALL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with incorrect end frame and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test21)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_incorrect_end_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with incorrect end frame and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_frame_count_test22)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_incorrect_end_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with incorrect end frame and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_frame_count_test23)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_incorrect_end_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with incorrect end frame and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test24)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_incorrect_end_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_ALL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with start frame larger then end frame and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test25)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_start_frame_larger_end_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_DIGITAL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with start frame larger then end frame and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_frame_count_test26)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_start_frame_larger_end_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_PREVIEW;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with start frame larger then end frame and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_frame_count_test27)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_start_frame_larger_end_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_frame_count with afs_toc_data_reel structure instance with start frame larger then end frame and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_frame_count_test28)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_start_frame_larger_end_frame();
    const int section = AFS_TOC_DATA_REEL_SECTION_ALL;

    unsigned int frame_count = afs_toc_data_reel_frame_count(toc_data_reel, section);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with input afs_toc_data_reel pointer equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_first_frame_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(start_frame == -1);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with input afs_toc_data_reel pointer equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_first_frame_test2)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(start_frame == -1);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with input afs_toc_data_reel pointer equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_first_frame_test3)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(start_frame == -1);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with input afs_toc_data_reel pointers equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_first_frame_test4)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_ALL);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(start_frame == -1);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_first_frame_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_first_frame_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_first_frame_test7)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_first_frame_test8)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_ALL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with input afs_toc_data_reel instance with empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_first_frame_test9)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with input afs_toc_files instance with ewith empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_first_frame_test10)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with input afs_toc_files instance with with empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_first_frame_test11)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with input afs_toc_files instance with with empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_first_frame_test12)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_ALL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with not empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_first_frame_test13)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    BOXING_ASSERT(start_frame == 2);

    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->start_frame = 12;

    start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);
    BOXING_ASSERT(start_frame == 12);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with not empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_first_frame_test14)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    BOXING_ASSERT(start_frame == 9);

    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->start_frame = 12;

    start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_PREVIEW);
    BOXING_ASSERT(start_frame == 9);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with not empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_first_frame_test15)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);
    BOXING_ASSERT(start_frame == -1);

    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->start_frame = 12;

    start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_first_frame with not empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_first_frame_test16)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    int start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_ALL);

    BOXING_ASSERT(start_frame == 2);

    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->start_frame = 12;

    start_frame = afs_toc_data_reel_first_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_ALL);
    BOXING_ASSERT(start_frame == 9);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with input afs_toc_data_reel pointer equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_last_frame_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(last_frame == -1);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with input afs_toc_data_reel pointer equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_last_frame_test2)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(last_frame == -1);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with input afs_toc_data_reel pointer equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_last_frame_test3)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(last_frame == -1);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with input afs_toc_data_reel pointers equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_last_frame_test4)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_ALL);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(last_frame == -1);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_last_frame_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_last_frame_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_last_frame_test7)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_last_frame_test8)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_ALL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with input afs_toc_data_reel instance with empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_last_frame_test9)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with input afs_toc_files instance with ewith empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_last_frame_test10)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with input afs_toc_files instance with with empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_last_frame_test11)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with input afs_toc_files instance with with empty afs_toc_files instance and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_last_frame_test12)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_ALL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with not empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_last_frame_test13)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    BOXING_ASSERT(last_frame == 7);

    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 5)->end_frame = 12;

    last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_DIGITAL);
    BOXING_ASSERT(last_frame == 12);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with not empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_last_frame_test14)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    BOXING_ASSERT(last_frame == -1);

    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->end_frame = 12;

    last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_PREVIEW);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with not empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_last_frame_test15)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);
    BOXING_ASSERT(last_frame == -1);

    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->end_frame = 12;

    last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_last_frame with not empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_last_frame_test16)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    int last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_ALL);

    BOXING_ASSERT(last_frame == 7);

    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 5)->end_frame = 12;

    last_frame = afs_toc_data_reel_last_frame(toc_data_reel, AFS_TOC_DATA_REEL_SECTION_ALL);
    BOXING_ASSERT(last_frame == 12);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_data_reel pointers equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_data_reel pointers equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test2)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_data_reel pointers equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test3)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_data_reel pointers equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test4)
{
    afs_toc_data_reel * toc_data_reel = NULL;
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_ALL);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test7)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with empty input afs_toc_data_reel instance and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test8)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_ALL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_data_reel instance with empty afs_toc_files instance and type equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test9)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_data_reel instance with empty afs_toc_files instance and type equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test10)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_data_reel instance with empty afs_toc_files instance and type equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test11)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_data_reel instance with empty afs_toc_files instance and type equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test12)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;
    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_ALL);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_files instance with filled vector and type equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test13)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    unsigned int offset = 5;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    BOXING_ASSERT(offset == 5);
    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, i);
        BOXING_ASSERT(toc_file->start_frame == (int)(2 + i + offset));
        BOXING_ASSERT(toc_file->end_frame == (int)(2 + i + offset));

        for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
        {
            afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
            BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j));
        }
    }

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_files instance with filled vector and type equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test14)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    unsigned int offset = 6;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    BOXING_ASSERT(offset == 6);
    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, i);
        BOXING_ASSERT(toc_file->start_frame == (int)(2 + i));
        BOXING_ASSERT(toc_file->end_frame == (int)(2 + i));

        for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
        {
            afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
            BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j + offset));
        }
    }

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_files instance with filled vector and type equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test15)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    unsigned int offset = 7;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    BOXING_ASSERT(offset == 7);
    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, i);
        BOXING_ASSERT(toc_file->start_frame == (int)(2 + i));
        BOXING_ASSERT(toc_file->end_frame == (int)(2 + i));

        for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
        {
            afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
            BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j));
        }
    }

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_add_frame_offset with input afs_toc_files instance with filled vector and type equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_reel_add_frame_offset_test16)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_different_parent();

    unsigned int offset = 8;

    afs_toc_data_reel_add_frame_offset(toc_data_reel, offset, AFS_TOC_DATA_REEL_SECTION_ALL);

    BOXING_ASSERT(offset == 8);
    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, i);
        BOXING_ASSERT(toc_file->start_frame == (int)(2 + i + offset));
        BOXING_ASSERT(toc_file->end_frame == (int)(2 + i + offset));

        for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
        {
            afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
            BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j + offset));
        }
    }

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_size_bytes with input afs_toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reel_size_bytes_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    boxing_int64 size = afs_toc_data_reel_size_bytes(toc_data_reel);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(size == 0);
}
END_TEST


// Test function afs_toc_data_reel_size_bytes with empty input afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_size_bytes_test2)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    boxing_int64 size = afs_toc_data_reel_size_bytes(toc_data_reel);

    BOXING_ASSERT(size == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_size_bytes with input afs_toc_data_reel instance with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_size_bytes_test3)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    boxing_int64 size = afs_toc_data_reel_size_bytes(toc_data_reel);

    BOXING_ASSERT(size == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_size_bytes with input afs_toc_data_reel instance with one afs_toc_file instance with start byte equal to end byte
BOXING_START_TEST(afs_toc_data_reel_size_bytes_test4)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 1);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->end_byte = 0;

    boxing_int64 size = afs_toc_data_reel_size_bytes(toc_data_reel);

    BOXING_ASSERT(size == 1);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_size_bytes with input afs_toc_data_reel instance with one afs_toc_file instance with start byte not equal to end byte
BOXING_START_TEST(afs_toc_data_reel_size_bytes_test5)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 1);

    boxing_int64 size = afs_toc_data_reel_size_bytes(toc_data_reel);

    BOXING_ASSERT(size == 29672);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_size_bytes with input afs_toc_data_reel instance with one afs_toc_file instance with start byte greater than end byte
BOXING_START_TEST(afs_toc_data_reel_size_bytes_test6)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 1);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->start_byte = 30000;

    boxing_int64 size = afs_toc_data_reel_size_bytes(toc_data_reel);

    BOXING_ASSERT(size == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_size_bytes with input afs_toc_data_reel instance with three afs_toc_file instances with start byte equal to end byte
BOXING_START_TEST(afs_toc_data_reel_size_bytes_test7)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 3);
    for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
    {
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i)->end_byte = GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i)->start_byte;
    }

    boxing_int64 size = afs_toc_data_reel_size_bytes(toc_data_reel);

    BOXING_ASSERT(size == 3);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_size_bytes with input afs_toc_data_reel instance with three afs_toc_file instances with start byte not equal to end byte
BOXING_START_TEST(afs_toc_data_reel_size_bytes_test8)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 3);

    boxing_int64 size = afs_toc_data_reel_size_bytes(toc_data_reel);

    BOXING_ASSERT(size == 89016);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_size_bytes with input afs_toc_data_reel instance with three afs_toc_file instances with start byte greater than end byte
BOXING_START_TEST(afs_toc_data_reel_size_bytes_test9)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 3);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1)->start_byte = 30000;

    boxing_int64 size = afs_toc_data_reel_size_bytes(toc_data_reel);

    BOXING_ASSERT(size == 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_is_valid with input afs_toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reel_is_valid_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_is_valid(toc_data_reel);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reel_is_valid with empty input afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_is_valid_test2)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_is_valid(toc_data_reel);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_is_valid with input afs_toc_data_reel instance with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_is_valid_test3)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("Some reel id!");

    DBOOL result = afs_toc_data_reel_is_valid(toc_data_reel);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_is_valid with input afs_toc_data_reel instance with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_is_valid_test4)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("Some reel id!");
    afs_toc_files * toc_files = afs_toc_files_create();
    toc_data_reel->files = toc_files;

    DBOOL result = afs_toc_data_reel_is_valid(toc_data_reel);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_is_valid with input afs_toc_data_reel instance with not valid afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_is_valid_test5)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("Some reel id!");

    afs_toc_files * toc_files = get_afs_toc_files_instance6(5);
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->types = AFS_TOC_FILE_TYPE_DIGITAL;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->start_frame = -1;

    toc_data_reel->files = toc_files;

    DBOOL result = afs_toc_data_reel_is_valid(toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_is_valid with input afs_toc_data_reel instance with reel id equal to NULL
BOXING_START_TEST(afs_toc_data_reel_is_valid_test6)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();
    afs_toc_files * toc_files = get_afs_toc_files_instance6(5);
    toc_data_reel->files = toc_files;

    DBOOL result = afs_toc_data_reel_is_valid(toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_is_valid with input afs_toc_data_reel instance with empty reel id string
BOXING_START_TEST(afs_toc_data_reel_is_valid_test7)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("");
    afs_toc_files * toc_files = get_afs_toc_files_instance6(5);
    toc_data_reel->files = toc_files;

    DBOOL result = afs_toc_data_reel_is_valid(toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_is_valid with input afs_toc_data_reel instance with valid data
BOXING_START_TEST(afs_toc_data_reel_is_valid_test8)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);

    DBOOL result = afs_toc_data_reel_is_valid(toc_data_reel);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_get_frame_ranges with input afs_toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reel_get_frame_ranges_test1)
{
    afs_toc_files * toc_files = NULL;

    afs_frame_ranges * frame_ranges = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(frame_ranges == NULL);
}
END_TEST


// Test function afs_toc_data_reel_get_frame_ranges with empty input afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_get_frame_ranges_test2)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    afs_frame_ranges * frame_ranges = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(frame_ranges == NULL);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_frame_ranges with input afs_toc_files instance with empty tocs vector
BOXING_START_TEST(afs_toc_data_reel_get_frame_ranges_test3)
{
    afs_toc_indices * tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    afs_frame_ranges * frame_ranges = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(frame_ranges == NULL);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_data_reel_get_frame_ranges with input afs_toc_files instance with one afs_toc_file instance
BOXING_START_TEST(afs_toc_data_reel_get_frame_ranges_test4)
{
    afs_toc_files * toc_files = get_afs_toc_files_instance6(1);

    afs_frame_ranges * frame_ranges1 = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(frame_ranges1 != NULL);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges1) == 1);
    BOXING_ASSERT(GVECTORN(frame_ranges1->frame_ranges, afs_frame_range *, 0)->start == 2);
    BOXING_ASSERT(GVECTORN(frame_ranges1->frame_ranges, afs_frame_range *, 0)->count == 1);

    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->end_frame = 10;

    afs_frame_ranges * frame_ranges2 = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(frame_ranges2 != NULL);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges2) == 1);
    BOXING_ASSERT(GVECTORN(frame_ranges2->frame_ranges, afs_frame_range *, 0)->start == 2);
    BOXING_ASSERT(GVECTORN(frame_ranges2->frame_ranges, afs_frame_range *, 0)->count == 9);

    afs_toc_files_free(toc_files);
    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges2);
}
END_TEST


// Test function afs_toc_data_reel_get_frame_ranges with input afs_toc_files instance with two separate ranges
BOXING_START_TEST(afs_toc_data_reel_get_frame_ranges_test5)
{
    afs_toc_files * toc_files = get_afs_toc_files_instance6(2);
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->start_frame = 0;
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->end_frame = 10;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->start_frame = 20;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->end_frame = 30;

    afs_frame_ranges * frame_ranges = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(frame_ranges != NULL);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == 2);
    afs_frame_range * frame_range1 = afs_frame_ranges_get_frame_range(frame_ranges, 0);
    BOXING_ASSERT(frame_range1->start == 0);
    BOXING_ASSERT(frame_range1->count == 11);
    afs_frame_range * frame_range2 = afs_frame_ranges_get_frame_range(frame_ranges, 1);
    BOXING_ASSERT(frame_range2->start == 20);
    BOXING_ASSERT(frame_range2->count == 11);

    afs_toc_files_free(toc_files);
    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_toc_data_reel_get_frame_ranges with input afs_toc_files instance with two overlapping ranges
BOXING_START_TEST(afs_toc_data_reel_get_frame_ranges_test6)
{
    afs_toc_files * toc_files = get_afs_toc_files_instance6(2);
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->start_frame = 0;
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->end_frame = 10;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->start_frame = 5;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->end_frame = 15;

    afs_frame_ranges * frame_ranges = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(frame_ranges != NULL);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == 1);
    afs_frame_range * frame_range1 = afs_frame_ranges_get_frame_range(frame_ranges, 0);
    BOXING_ASSERT(frame_range1->start == 0);
    BOXING_ASSERT(frame_range1->count == 16);

    afs_toc_files_free(toc_files);
    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_toc_data_reel_get_frame_ranges with input afs_toc_files instance with one range within another
BOXING_START_TEST(afs_toc_data_reel_get_frame_ranges_test7)
{
    afs_toc_files * toc_files = get_afs_toc_files_instance6(2);
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->start_frame = 0;
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->end_frame = 30;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->start_frame = 10;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->end_frame = 20;

    afs_frame_ranges * frame_ranges = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(frame_ranges != NULL);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == 1);
    afs_frame_range * frame_range1 = afs_frame_ranges_get_frame_range(frame_ranges, 0);
    BOXING_ASSERT(frame_range1->start == 0);
    BOXING_ASSERT(frame_range1->count == 31);

    afs_toc_files_free(toc_files);
    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_toc_data_reel_get_frame_ranges with input afs_toc_files instance with two adjoining ranges
BOXING_START_TEST(afs_toc_data_reel_get_frame_ranges_test8)
{
    afs_toc_files * toc_files = get_afs_toc_files_instance6(2);
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->start_frame = 0;
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->end_frame = 10;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->start_frame = 11;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->end_frame = 20;

    afs_frame_ranges * frame_ranges = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(frame_ranges != NULL);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == 1);
    afs_frame_range * frame_range1 = afs_frame_ranges_get_frame_range(frame_ranges, 0);
    BOXING_ASSERT(frame_range1->start == 0);
    BOXING_ASSERT(frame_range1->count == 21);

    afs_toc_files_free(toc_files);
    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_toc_data_reel_get_frame_ranges with input afs_toc_files instance with different ranges
BOXING_START_TEST(afs_toc_data_reel_get_frame_ranges_test9)
{
    afs_toc_files * toc_files = get_afs_toc_files_instance6(8);
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->start_frame = 0;
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->end_frame = 10;

    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->start_frame = 100;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->end_frame = 110;

    GVECTORN(toc_files->tocs, afs_toc_file *, 2)->start_frame = 200;
    GVECTORN(toc_files->tocs, afs_toc_file *, 2)->end_frame = 210;

    GVECTORN(toc_files->tocs, afs_toc_file *, 3)->start_frame = 300;
    GVECTORN(toc_files->tocs, afs_toc_file *, 3)->end_frame = 310;

    GVECTORN(toc_files->tocs, afs_toc_file *, 4)->start_frame = 20;
    GVECTORN(toc_files->tocs, afs_toc_file *, 4)->end_frame = 50;

    GVECTORN(toc_files->tocs, afs_toc_file *, 5)->start_frame = 105;
    GVECTORN(toc_files->tocs, afs_toc_file *, 5)->end_frame = 145;

    GVECTORN(toc_files->tocs, afs_toc_file *, 6)->start_frame = 201;
    GVECTORN(toc_files->tocs, afs_toc_file *, 6)->end_frame = 208;

    GVECTORN(toc_files->tocs, afs_toc_file *, 7)->start_frame = 311;
    GVECTORN(toc_files->tocs, afs_toc_file *, 7)->end_frame = 340;

    afs_frame_ranges * frame_ranges = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(frame_ranges != NULL);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == 5);
    afs_frame_range * frame_range1 = afs_frame_ranges_get_frame_range(frame_ranges, 0);
    BOXING_ASSERT(frame_range1->start == 0);
    BOXING_ASSERT(frame_range1->count == 11);
    afs_frame_range * frame_range2 = afs_frame_ranges_get_frame_range(frame_ranges, 1);
    BOXING_ASSERT(frame_range2->start == 20);
    BOXING_ASSERT(frame_range2->count == 31);
    afs_frame_range * frame_range3 = afs_frame_ranges_get_frame_range(frame_ranges, 2);
    BOXING_ASSERT(frame_range3->start == 100);
    BOXING_ASSERT(frame_range3->count == 46);
    afs_frame_range * frame_range4 = afs_frame_ranges_get_frame_range(frame_ranges, 3);
    BOXING_ASSERT(frame_range4->start == 200);
    BOXING_ASSERT(frame_range4->count == 11);
    afs_frame_range * frame_range5 = afs_frame_ranges_get_frame_range(frame_ranges, 4);
    BOXING_ASSERT(frame_range5->start == 300);
    BOXING_ASSERT(frame_range5->count == 41);

    afs_toc_files_free(toc_files);
    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_toc_data_reel_get_frame_ranges with input afs_toc_files instance with different ranges with reverse ranges order
BOXING_START_TEST(afs_toc_data_reel_get_frame_ranges_test10)
{
    afs_toc_files * toc_files = get_afs_toc_files_instance6(8);
    GVECTORN(toc_files->tocs, afs_toc_file *, 7)->start_frame = 0;
    GVECTORN(toc_files->tocs, afs_toc_file *, 7)->end_frame = 10;

    GVECTORN(toc_files->tocs, afs_toc_file *, 6)->start_frame = 100;
    GVECTORN(toc_files->tocs, afs_toc_file *, 6)->end_frame = 110;

    GVECTORN(toc_files->tocs, afs_toc_file *, 5)->start_frame = 200;
    GVECTORN(toc_files->tocs, afs_toc_file *, 5)->end_frame = 210;

    GVECTORN(toc_files->tocs, afs_toc_file *, 4)->start_frame = 300;
    GVECTORN(toc_files->tocs, afs_toc_file *, 4)->end_frame = 310;

    GVECTORN(toc_files->tocs, afs_toc_file *, 3)->start_frame = 20;
    GVECTORN(toc_files->tocs, afs_toc_file *, 3)->end_frame = 50;

    GVECTORN(toc_files->tocs, afs_toc_file *, 2)->start_frame = 105;
    GVECTORN(toc_files->tocs, afs_toc_file *, 2)->end_frame = 145;

    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->start_frame = 201;
    GVECTORN(toc_files->tocs, afs_toc_file *, 1)->end_frame = 208;

    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->start_frame = 311;
    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->end_frame = 340;

    afs_frame_ranges * frame_ranges = afs_toc_data_reel_get_frame_ranges(toc_files);

    BOXING_ASSERT(frame_ranges != NULL);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == 5);
    afs_frame_range * frame_range1 = afs_frame_ranges_get_frame_range(frame_ranges, 0);
    BOXING_ASSERT(frame_range1->start == 0);
    BOXING_ASSERT(frame_range1->count == 11);
    afs_frame_range * frame_range2 = afs_frame_ranges_get_frame_range(frame_ranges, 1);
    BOXING_ASSERT(frame_range2->start == 20);
    BOXING_ASSERT(frame_range2->count == 31);
    afs_frame_range * frame_range3 = afs_frame_ranges_get_frame_range(frame_ranges, 2);
    BOXING_ASSERT(frame_range3->start == 100);
    BOXING_ASSERT(frame_range3->count == 46);
    afs_frame_range * frame_range4 = afs_frame_ranges_get_frame_range(frame_ranges, 3);
    BOXING_ASSERT(frame_range4->start == 200);
    BOXING_ASSERT(frame_range4->count == 11);
    afs_frame_range * frame_range5 = afs_frame_ranges_get_frame_range(frame_ranges, 4);
    BOXING_ASSERT(frame_range5->start == 300);
    BOXING_ASSERT(frame_range5->count == 41);

    afs_toc_files_free(toc_files);
    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_toc_data_reel_save_file with file_name pointer equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_save_file(toc_data_reel, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reel_save_file with file_name pointer equal to NULL and afs_toc_data_reel pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_reel_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 5);

    DBOOL result = afs_toc_data_reel_save_file(toc_data_reel, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_save_file with file_name pointer not equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_save_file_test3)
{
    const char * file_name = "afs_toc_data_reel_save_file_test.xml";
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_save_file(toc_data_reel, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reel_save_file with empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_save_file_test4)
{
    const char * file_name = "afs_toc_data_reel_save_file_empty_test.xml";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_save_file(toc_data_reel, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_save_file with afs_toc_data_reel instance with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_save_file_test5)
{
    const char * file_name = "afs_toc_data_reel_save_file_null_files_test.xml";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("some id string");

    DBOOL result = afs_toc_data_reel_save_file(toc_data_reel, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_save_file with afs_toc_data_reel instance with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_save_file_test6)
{
    const char * file_name = "afs_toc_data_reel_save_file_empty_test.xml";
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 0);

    DBOOL result = afs_toc_data_reel_save_file(toc_data_reel, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_save_file with afs_toc_data_reel instance with not valid data
BOXING_START_TEST(afs_toc_data_reel_save_file_test7)
{
    const char * file_name = "afs_toc_data_reel_save_file_not_valid_test.xml";
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("", 6);

    DBOOL result = afs_toc_data_reel_save_file(toc_data_reel, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_save_file with not empty afs_toc_data_reel instance, in compact format
BOXING_START_TEST(afs_toc_data_reel_save_file_test8)
{
    const char * file_name = "afs_toc_data_reel_save_file_compact_test.xml";

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 6);

    DBOOL result = afs_toc_data_reel_save_file(toc_data_reel, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_save_file with not empty afs_toc_data_reel instance, in not compact format
BOXING_START_TEST(afs_toc_data_reel_save_file_test9)
{
    const char * file_name = "afs_toc_data_reel_save_file_not_compact_test.xml";

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 6);

    DBOOL result = afs_toc_data_reel_save_file(toc_data_reel, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_save_string with afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_save_string_test1)
{
    afs_toc_data_reel * toc_data_reel = NULL;

    char * result = afs_toc_data_reel_save_string(toc_data_reel, DTRUE);

    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_data_reel_save_string with empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_save_string_test2)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    char * result = afs_toc_data_reel_save_string(toc_data_reel, DTRUE);

    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_save_string with afs_toc_data_reel instance with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_save_string_test3)
{
    char * test_string = "<?xml version=\"1.0\" encoding=\"utf-8\"?><reel><id>some id string</id></reel>\n";

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("some id string");

    char * result = afs_toc_data_reel_save_string(toc_data_reel, DTRUE);

    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(strcmp(test_string, result) == 0);

    afs_toc_data_reel_free(toc_data_reel);
    free(result);
}
END_TEST


// Test function afs_toc_data_reel_save_string with afs_toc_data_reel instance with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_save_string_test4)
{
    char * test_string = "<?xml version=\"1.0\" encoding=\"utf-8\"?><reel><id>some id string</id><files /></reel>\n";

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 0);

    char * result = afs_toc_data_reel_save_string(toc_data_reel, DTRUE);

    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(strcmp(test_string, result) == 0);

    afs_toc_data_reel_free(toc_data_reel);
    free(result);
}
END_TEST


// Test function afs_toc_data_reel_save_string with not valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_save_string_test5)
{
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("", 6);

    char * result = afs_toc_data_reel_save_string(toc_data_reel, DTRUE);

    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_save_string with valid afs_toc_data_reel instance, in compact format
BOXING_START_TEST(afs_toc_data_reel_save_string_test6)
{
    char * test_string = read_xml_toc_data_reel("afs_toc_data_reel_save_file_compact_test.xml");

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 6);

    char * result = afs_toc_data_reel_save_string(toc_data_reel, DTRUE);

    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_data_reel_free(toc_data_reel);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_data_reel_save_string with valid afs_toc_data_reel instance, in not compact format
BOXING_START_TEST(afs_toc_data_reel_save_string_test7)
{
    char * test_string = read_xml_toc_data_reel("afs_toc_data_reel_save_file_not_compact_test.xml");

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 6);

    char * result = afs_toc_data_reel_save_string(toc_data_reel, DFALSE);

    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_data_reel_free(toc_data_reel);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_data_reel_save_xml with mxml_node_t pointer equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_save_xml(toc_data_reel, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reel_save_xml with mxml_node_t pointer equal to NULL and afs_toc_data_reel pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_reel_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 6);

    DBOOL result = afs_toc_data_reel_save_xml(toc_data_reel, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_save_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reel_save_xml with empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_save_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_save_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reel_save_xml with not empty afs_toc_data_reel instance with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_save_xml_test5)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2("some id string");

    DBOOL result = afs_toc_data_reel_save_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reel_save_xml with not valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("", 6);

    DBOOL result = afs_toc_data_reel_save_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reel_save_xml with valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_reel_save_xml_test7)
{
    mxml_node_t * tree = mxmlNewXML("1.0");

    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance6("some id string", 6);

    DBOOL result = afs_toc_data_reel_save_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_reel_free(toc_data_reel);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reel_load_file with file_name pointer equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_load_file(toc_data_reel, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reel_load_file with file_name pointer equal to NULL and afs_toc_data_reel pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_file(toc_data_reel, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_load_file with file_name pointer not equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_file_test3)
{
    const char * file_name = "afs_toc_data_reel_save_file_compact_test.xml";
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_load_file(toc_data_reel, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reel_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_data_reel_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_file(toc_data_reel, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_load_file with initialized afs_toc_data_reel instance with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_file_test5)
{
    const char * file_name = "afs_toc_data_reel_save_file_null_files_test.xml";

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_file(toc_data_reel, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 0);
    BOXING_ASSERT(toc_data_reel->files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_load_file with initialized afs_toc_data_reel instance with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_load_file_test6)
{
    const char * file_name = "afs_toc_data_reel_save_file_empty_test.xml";

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_file(toc_data_reel, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 0);
    BOXING_ASSERT(toc_data_reel->files != NULL);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_load_file with initialized afs_toc_data_reel instance, in compact format
BOXING_START_TEST(afs_toc_data_reel_load_file_test7)
{
    const char * file_name = "afs_toc_data_reel_save_file_compact_test.xml";

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_file(toc_data_reel, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);
    
    test_afs_toc_data_reel(toc_data_reel, "some id string", 6);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_load_file with initialized afs_toc_data_reel instance, in not compact format
BOXING_START_TEST(afs_toc_data_reel_load_file_test8)
{
    const char * file_name = "afs_toc_data_reel_save_file_not_compact_test.xml";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_file(toc_data_reel, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 6);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_load_string with input string pointer equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_load_string(toc_data_reel, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reel_load_string with input string pointer equal to NULL and afs_toc_data_reel pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_string(toc_data_reel, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_load_string with input string pointer not equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_string_test3)
{
    char * input_string = read_xml_toc_data_reel("afs_toc_data_reel_save_file_compact_test.xml");
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_load_string(toc_data_reel, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_data_reel_load_string with empty input string
BOXING_START_TEST(afs_toc_data_reel_load_string_test4)
{
    char * input_string = "";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_string(toc_data_reel, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_load_string with input string for initialized afs_toc_data_reel instance with afs_toc_files pointer equak to NULL
BOXING_START_TEST(afs_toc_data_reel_load_string_test5)
{
    char * input_string = read_xml_toc_data_reel("afs_toc_data_reel_save_file_null_files_test.xml");

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_string(toc_data_reel, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 0);
    BOXING_ASSERT(toc_data_reel->files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_reel_load_string with input string for initialized afs_toc_data_reel instance with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_load_string_test6)
{
    char * input_string = read_xml_toc_data_reel("afs_toc_data_reel_save_file_empty_test.xml");

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_string(toc_data_reel, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 0);
    BOXING_ASSERT(toc_data_reel->files != NULL);

    afs_toc_data_reel_free(toc_data_reel);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_reel_load_string with input string for initialized afs_toc_data_reel instance, in compact format
BOXING_START_TEST(afs_toc_data_reel_load_string_test7)
{
    char * input_string = read_xml_toc_data_reel("afs_toc_data_reel_save_file_compact_test.xml");

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_string(toc_data_reel, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 6);

    afs_toc_data_reel_free(toc_data_reel);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_reel_load_string with input string for initialized afs_toc_data_reel instance, in not compact format
BOXING_START_TEST(afs_toc_data_reel_load_string_test8)
{
    char * input_string = read_xml_toc_data_reel("afs_toc_data_reel_save_file_not_compact_test.xml");

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_string(toc_data_reel, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 6);

    afs_toc_data_reel_free(toc_data_reel);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_reel_load_xml with xml instance pointer equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_load_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_reel_load_xml with xml instance pointer equal to NULL and afs_toc_data_reel pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_reel_load_xml with xml instance pointer not equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_xml_test3)
{
    mxml_node_t *tree = get_toc_data_reel_xml_instance("afs_toc_data_reel_save_file_compact_test.xml");
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_reel_load_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reel_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_data_reel_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data_reel(toc_data_reel, NULL, 0);

    afs_toc_data_reel_free(toc_data_reel);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reel_load_xml with xml instance for initialized afs_toc_data_reel instance with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_data_reel_load_xml_test5)
{
    mxml_node_t *tree = get_toc_data_reel_xml_instance("afs_toc_data_reel_save_file_null_files_test.xml");
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 0);
    BOXING_ASSERT(toc_data_reel->files == NULL);

    afs_toc_data_reel_free(toc_data_reel);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reel_load_xml with xml instance for initialized afs_toc_data_reel instance with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_data_reel_load_xml_test6)
{
    mxml_node_t *tree = get_toc_data_reel_xml_instance("afs_toc_data_reel_save_file_empty_test.xml");
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 0);
    BOXING_ASSERT(toc_data_reel->files != NULL);

    afs_toc_data_reel_free(toc_data_reel);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reel_load_xml with xml instance for initialized afs_toc_data_reel instance, in compact format
BOXING_START_TEST(afs_toc_data_reel_load_xml_test7)
{
    mxml_node_t *tree = get_toc_data_reel_xml_instance("afs_toc_data_reel_save_file_compact_test.xml");
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 6);

    afs_toc_data_reel_free(toc_data_reel);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_reel_load_xml with xml instance for initialized afs_toc_data_reel instance, in not compact format
BOXING_START_TEST(afs_toc_data_reel_load_xml_test8)
{
    mxml_node_t * tree = get_toc_data_reel_xml_instance("afs_toc_data_reel_save_file_not_compact_test.xml");
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_reel_load_xml(toc_data_reel, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data_reel(toc_data_reel, "some id string", 6);

    afs_toc_data_reel_free(toc_data_reel);
    mxmlDelete(tree);
}
END_TEST


Suite * tocdatareel_tests(void)
{
    TCase * tc_tocdatareel_functions_tests = tcase_create("tocdatareel_functions_tests");

    // Test function afs_toc_files_create
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_create_test1);
    // Test function afs_toc_files_create2
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_create2_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_create2_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_create2_test3);
    // Test function afs_toc_data_reel_clone
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_clone_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_clone_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_clone_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_clone_test4);
    // Test function afs_toc_data_reel_get_new_reference
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_new_reference_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_new_reference_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_new_reference_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_new_reference_test4);
    // Test function afs_toc_data_reel_equal
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test10);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test11);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test12);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test13);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test14);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_equal_test15);
    // Test function afs_toc_data_reel_set_id
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_set_id_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_set_id_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_set_id_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_set_id_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_set_id_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_set_id_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_set_id_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_set_id_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_set_id_test9);
    // Test function afs_toc_data_reel_add_file
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test10);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test11);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test12);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test13);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test14);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_file_test15);
    // Test function afs_toc_data_reel_get_file_by_index
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_index_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_index_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_index_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_index_test4);
    // Test function afs_toc_data_reel_get_file_by_id
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_id_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_id_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_id_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_id_test4);
    // Test function afs_toc_data_reel_get_file_by_unique_id
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_unique_id_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_unique_id_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_unique_id_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_unique_id_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_unique_id_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_unique_id_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_unique_id_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_unique_id_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_unique_id_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_file_by_unique_id_test10);
    // Test function afs_toc_data_reel_get_parent_files
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test10);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test11);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test12);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test13);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test14);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test15);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test16);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test17);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test18);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test19);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_parent_files_test20);
    // Test function afs_toc_data_reel_get_child_files
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test10);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test11);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test12);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test13);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test14);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test15);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test16);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test17);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test18);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test19);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_child_files_test20);
    // Test function afs_toc_data_reel_get_standalone_files
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test10);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test11);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test12);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test13);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test14);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test15);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test16);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test17);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test18);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test19);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_standalone_files_test20);
    // Test function afs_toc_data_reel_file_count
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_file_count_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_file_count_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_file_count_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_file_count_test4);
    // Test function afs_toc_data_reel_frame_count
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test10);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test11);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test12);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test13);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test14);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test15);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test16);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test17);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test18);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test19);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test20);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test21);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test22);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test23);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test24);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test25);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test26);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test27);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_frame_count_test28);
    // Test function afs_toc_data_reel_first_frame
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test10);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test11);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test12);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test13);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test14);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test15);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_first_frame_test16);
    // Test function afs_toc_data_reel_last_frame
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test10);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test11);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test12);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test13);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test14);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test15);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_last_frame_test16);
    // Test function afs_toc_data_reel_add_frame_offset
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test10);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test11);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test12);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test13);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test14);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test15);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_add_frame_offset_test16);
    // Test function afs_toc_data_reel_size_bytes
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_size_bytes_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_size_bytes_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_size_bytes_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_size_bytes_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_size_bytes_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_size_bytes_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_size_bytes_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_size_bytes_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_size_bytes_test9);
    // Test function afs_toc_data_reel_is_valid
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_is_valid_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_is_valid_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_is_valid_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_is_valid_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_is_valid_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_is_valid_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_is_valid_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_is_valid_test8);
    // Test function afs_toc_data_reel_get_frame_ranges
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_frame_ranges_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_frame_ranges_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_frame_ranges_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_frame_ranges_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_frame_ranges_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_frame_ranges_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_frame_ranges_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_frame_ranges_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_frame_ranges_test9);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_get_frame_ranges_test10);
    // Test function afs_toc_data_reel_save_file
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_file_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_file_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_file_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_file_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_file_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_file_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_file_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_file_test8);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_file_test9);
    // Test function afs_toc_data_reel_save_string
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_string_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_string_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_string_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_string_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_string_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_string_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_string_test7);
    // Test function afs_toc_data_reel_save_xml
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_xml_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_xml_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_xml_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_xml_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_xml_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_xml_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_save_xml_test7);
    // Test function afs_toc_data_reel_load_file
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_file_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_file_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_file_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_file_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_file_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_file_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_file_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_file_test8);
    // Test function afs_toc_data_reel_load_string
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_string_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_string_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_string_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_string_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_string_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_string_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_string_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_string_test8);
    // Test function afs_toc_data_reel_load_xml
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_xml_test1);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_xml_test2);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_xml_test3);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_xml_test4);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_xml_test5);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_xml_test6);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_xml_test7);
    tcase_add_test(tc_tocdatareel_functions_tests, afs_toc_data_reel_load_xml_test8);

    Suite * s = suite_create("tocdatareel_test_util");
    suite_add_tcase(s, tc_tocdatareel_functions_tests);

    return s;
}
