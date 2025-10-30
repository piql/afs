#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "tocfiles.h"
#include "boxing/utils.h"
#include "mxml.h"

#include <string.h>

#if defined ( D_OS_WIN32 )
#define DFSEEK _fseeki64
#define DFTELL _ftelli64
#else
#define DFSEEK fseek
#define DFTELL ftell
#endif

static void test_null_afs_toc_files(afs_toc_files * toc_files)
{
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(toc_files->tocs == NULL);
}


static void test_empty_afs_toc_files(afs_toc_files * toc_files)
{
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(toc_files->tocs != NULL);
    BOXING_ASSERT(toc_files->tocs->size == 0);
}


static afs_toc_file_preview_pages* get_filled_pages_vector5(unsigned int pages_count)
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


static afs_toc_file* get_afs_toc_file_instance3(unsigned int number)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector5(3);
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


static void test_not_empty_afs_toc_files(afs_toc_files * toc_files, unsigned int tocs_count)
{
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(toc_files->tocs != NULL);
    BOXING_ASSERT(toc_files->tocs->size == tocs_count);

    for (unsigned int i = 0; i < tocs_count; i++)
    {
        afs_toc_file* afs_toc_file_instance = get_afs_toc_file_instance3(i);
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_files->tocs, afs_toc_file *, i), afs_toc_file_instance) == DTRUE);
        afs_toc_file_free(afs_toc_file_instance);
    }
}


static afs_toc_indices* get_filled_tocs_vector(unsigned int tocs_count)
{
    afs_toc_indices* tocs = gvector_create_pointers(tocs_count);

    for (unsigned int i = 0; i < tocs_count; i++)
    {
        afs_toc_file * toc_file = get_afs_toc_file_instance3(i);
        GVECTORN(tocs, afs_toc_file *, i) = toc_file;
    }

    return tocs;
}

static afs_toc_preview_layout_definitions* create_framecount_definitions()
{
    afs_toc_preview_layout_definition* layout = afs_toc_preview_layout_definition_create2("FrameCountLayout","Test layout for framecount toc");
    afs_toc_preview_layout_definition_add_section( 
        layout, afs_toc_preview_section_create2( 0, 0, 0, 2000, 2000, 0 ) );
    afs_toc_preview_layout_definition_add_section(
        layout, afs_toc_preview_section_create2( 1, 2000, 0, 2000, 2000, 0 ) );

    afs_toc_preview_layout_definitions* definitions = afs_toc_preview_layout_definitions_create();
    afs_toc_preview_layout_definitions_add_layout_definition( definitions, layout );
    return definitions;
}

static afs_toc_file_preview_pages* create_framecount_preview_pages( unsigned int startFrame )
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers( 4 );

    for ( unsigned int i = 0; i < 4; i++ )
    {
        unsigned int pageStartFrame = startFrame + (int)floor( i / 2 );
        unsigned int pageStartSection = i % 2;
        GVECTORN( pages, afs_toc_file_preview_page *, i ) =
            afs_toc_file_preview_page_create2(
                "FrameCountLayout", pageStartFrame, pageStartSection, 1,
                2000, 2000, 0, 0 );
    }

    return pages;
}

static afs_toc_file* create_framecount_toc_file( unsigned int number )
{
    afs_toc_file_preview_pages* pages = create_framecount_preview_pages( 2 + number * 2 );
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2( pages );

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance( 5 );

    return afs_toc_file_create2(
        "input-toc.xml",                            /* name */
        "2016-11-22 09:07:11",                      /* date */
        "f2eb0d6c6964d206012695a10ea6cd877c95c340", /* checksum */
        29672 + number,                             /* size */
        1 + number,                                 /* id */
        10 + number,                                /* start frame */
        0 + number,                                 /* start byte */
        20 + number,                                /* end frame */
        29000 + number,                             /* end byte */
        "38A0AAAB-16B2-640C-5353-6DB8AE4367B9",     /* unique id */
        AFS_TOC_FILE_NO_PARENT,                     /* parent id */
        AFS_TOC_FILE_TYPE_ALL,                      /* types */
        toc_file_preview,                           /* preview/visual description */
        "file_format",                              /* file format */
        toc_data_file_metadata );                   /* metadata */
}

static const int FRAMECOUNT_TOC_FIRST_DIGITAL = 10;
static const int FRAMECOUNT_TOC_LAST_DIGITAL = 22;
static const int FRAMECOUNT_TOC_DURATION_DIGITAL = 13;
static const int FRAMECOUNT_TOC_FIRST_VISUAL = 2;
static const int FRAMECOUNT_TOC_LAST_VISUAL = 7;
static const int FRAMECOUNT_TOC_DURATION_VISUAL = 6;
static const int FRAMECOUNT_TOC_DURATION_ALL = 21;


static afs_toc_indices* create_framecount_toc()
{
    // Set up a test toc with 3 hybrid files
    //       file_id   digital_frames   visual_frames
    //             1            10-20             2-3
    //             2            11-21             4-5
    //             3            12-22             6-7

    afs_toc_indices* tocs = gvector_create_pointers( 3 );
    for ( unsigned int i = 0; i < 3; i++ )
    {
        GVECTORN( tocs, afs_toc_file *, i ) = create_framecount_toc_file( i );
    }

    return tocs;
}

static void test_framecount_toc_unchanged( afs_toc_files * toc_files)
{
    BOXING_ASSERT( toc_files != NULL );
    BOXING_ASSERT( toc_files->tocs != NULL );
    BOXING_ASSERT( toc_files->tocs->size == 3 );

    for ( unsigned int i = 0; i < 3; i++ )
    {
        afs_toc_file* afs_toc_file_instance = create_framecount_toc_file( i );
        BOXING_ASSERT( afs_toc_file_equal( GVECTORN( toc_files->tocs, afs_toc_file *, i ), afs_toc_file_instance ) == DTRUE );
        afs_toc_file_free( afs_toc_file_instance );
    }
}

static char * read_xml_tocs_file(const char* file_name)
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


static mxml_node_t * get_tocs_xml_instance(const char * file_name)
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


// TEST TOC FILE PREVIEW FUNCTIONS
//

// Test function afs_toc_files_create test
BOXING_START_TEST(afs_toc_files_create_test1)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    test_null_afs_toc_files(toc_files);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_create2 test1 with tocs pointer equal to NULL
BOXING_START_TEST(afs_toc_files_create2_test1)
{
    afs_toc_files * toc_files = afs_toc_files_create2(NULL);

    test_null_afs_toc_files(toc_files);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_create2 test2 with empty tocs vector
BOXING_START_TEST(afs_toc_files_create2_test2)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);

    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    test_empty_afs_toc_files(toc_files);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_create2 test3 with filled tocs vector
BOXING_START_TEST(afs_toc_files_create2_test3)
{
    afs_toc_indices* tocs = get_filled_tocs_vector(1);

    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    test_not_empty_afs_toc_files(toc_files, 1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_init test with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_init_test1)
{
    afs_toc_files * toc_files = NULL;

    afs_toc_files_init(toc_files);

    BOXING_ASSERT(toc_files == NULL);
}
END_TEST


// Test function afs_toc_files_init test with allocated afs_toc_files pointer
BOXING_START_TEST(afs_toc_files_init_test2)
{
    afs_toc_files * toc_files = malloc(sizeof(afs_toc_files));

    afs_toc_files_init(toc_files);

    test_null_afs_toc_files(toc_files);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_init2 test with afs_toc_files pointer equal to NULL and vector pointer equal to NULL
BOXING_START_TEST(afs_toc_files_init2_test1)
{
    afs_toc_files * toc_files = NULL;
    afs_toc_indices * tocs = NULL;

    afs_toc_files_init2(toc_files, tocs);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(tocs == NULL);
}
END_TEST


// Test function afs_toc_files_init2 test with afs_toc_files pointer equal to NULL and vector pointer not equa to NULL
BOXING_START_TEST(afs_toc_files_init2_test2)
{
    afs_toc_files * toc_files = NULL;
    afs_toc_indices * tocs = gvector_create_pointers(0);

    afs_toc_files_init2(toc_files, tocs);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(tocs != NULL);
    BOXING_ASSERT(tocs->size == 0);

    gvector_free(tocs);
}
END_TEST


// Test function afs_toc_files_init2 test with afs_toc_files pointer not equal to NULL and vector pointer equal to NULL
BOXING_START_TEST(afs_toc_files_init2_test3)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_toc_indices * tocs = NULL;

    afs_toc_files_init2(toc_files, tocs);

    test_null_afs_toc_files(toc_files);

    BOXING_ASSERT(tocs == NULL);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_init2 test with afs_toc_files pointer not equal to NULL and empty vector
BOXING_START_TEST(afs_toc_files_init2_test4)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_toc_indices* tocs = gvector_create_pointers(0);

    test_null_afs_toc_files(toc_files);

    afs_toc_files_init2(toc_files, tocs);

    test_empty_afs_toc_files(toc_files);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_init2 test with afs_toc_files pointer not equal to NULL and filled vector
BOXING_START_TEST(afs_toc_files_init2_test5)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_toc_indices* tocs = get_filled_tocs_vector(3);

    test_null_afs_toc_files(toc_files);

    afs_toc_files_init2(toc_files, tocs);

    test_not_empty_afs_toc_files(toc_files, 3);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_clone test with input pointer equal to NULL
BOXING_START_TEST(afs_toc_files_clone_test1)
{
    afs_toc_files * toc_files1 = NULL;

    BOXING_ASSERT(toc_files1 == NULL);

    afs_toc_files* toc_files2 = afs_toc_files_clone(toc_files1);

    BOXING_ASSERT(toc_files2 == NULL);
}
END_TEST


// Test function afs_toc_files_clone test with empty input afs_toc_files instance
BOXING_START_TEST(afs_toc_files_clone_test2)
{
    afs_toc_files * toc_files1 = afs_toc_files_create();

    test_null_afs_toc_files(toc_files1);

    afs_toc_files * toc_files2 = afs_toc_files_clone(toc_files1);

    test_null_afs_toc_files(toc_files2);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_clone test with not empty input afs_toc_files instance and empty vector
BOXING_START_TEST(afs_toc_files_clone_test3)
{
    afs_toc_indices * tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs);

    test_empty_afs_toc_files(toc_files1);

    afs_toc_files * toc_files2 = afs_toc_files_clone(toc_files1);

    test_empty_afs_toc_files(toc_files2);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_clone test with not empty input afs_toc_files instance and not empty vctor
BOXING_START_TEST(afs_toc_files_clone_test4)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs);

    test_not_empty_afs_toc_files(toc_files1, 3);

    afs_toc_files * toc_files2 = afs_toc_files_clone(toc_files1);

    test_not_empty_afs_toc_files(toc_files2, 3);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_files_get_new_reference_test1)
{
    afs_toc_files * toc_files1 = NULL;

    BOXING_ASSERT(toc_files1 == NULL);

    afs_toc_files* toc_files2 = afs_toc_files_get_new_reference(toc_files1);

    BOXING_ASSERT(toc_files2 == NULL);

    toc_files1 = afs_toc_files_create();

    test_null_afs_toc_files(toc_files1);
    BOXING_ASSERT(toc_files1->reference_count == 1);
    BOXING_ASSERT(toc_files2 == NULL);

    afs_toc_files_free(toc_files1);
}
END_TEST


// Test function afs_toc_files_get_new_reference with empty input afs_toc_files instance
BOXING_START_TEST(afs_toc_files_get_new_reference_test2)
{
    afs_toc_files * toc_files1 = afs_toc_files_create();

    test_null_afs_toc_files(toc_files1);
    BOXING_ASSERT(toc_files1->reference_count == 1);

    afs_toc_files * toc_files2 = afs_toc_files_get_new_reference(toc_files1);

    test_null_afs_toc_files(toc_files2);
    BOXING_ASSERT(toc_files1->reference_count == 2);
    BOXING_ASSERT(toc_files2->reference_count == 2);
    BOXING_ASSERT(toc_files1 == toc_files2);

    afs_toc_file * toc_file = get_afs_toc_file_instance3(0);
    DBOOL result = afs_toc_files_add_toc(toc_files1, toc_file);

    BOXING_ASSERT(result == DTRUE);
    test_not_empty_afs_toc_files(toc_files2, 1);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files1);
}
END_TEST


// Test function afs_toc_files_get_new_reference with not empty input afs_toc_files instance and empty vector
BOXING_START_TEST(afs_toc_files_get_new_reference_test3)
{
    afs_toc_indices * tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs);

    test_empty_afs_toc_files(toc_files1);
    BOXING_ASSERT(toc_files1->reference_count == 1);

    afs_toc_files * toc_files2 = afs_toc_files_get_new_reference(toc_files1);

    test_empty_afs_toc_files(toc_files2);
    BOXING_ASSERT(toc_files1->reference_count == 2);
    BOXING_ASSERT(toc_files2->reference_count == 2);
    BOXING_ASSERT(toc_files1 == toc_files2);

    afs_toc_file * toc_file = get_afs_toc_file_instance3(0);
    DBOOL result = afs_toc_files_add_toc(toc_files1, toc_file);

    BOXING_ASSERT(result == DTRUE);
    test_not_empty_afs_toc_files(toc_files2, 1);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files1);
}
END_TEST


// Test function afs_toc_files_get_new_reference with not empty input afs_toc_files instance and not empty vector
BOXING_START_TEST(afs_toc_files_get_new_reference_test4)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs);

    test_not_empty_afs_toc_files(toc_files1, 3);
    BOXING_ASSERT(toc_files1->reference_count == 1);

    afs_toc_files * toc_files2 = afs_toc_files_get_new_reference(toc_files1);

    test_not_empty_afs_toc_files(toc_files2, 3);
    BOXING_ASSERT(toc_files1->reference_count == 2);
    BOXING_ASSERT(toc_files2->reference_count == 2);
    BOXING_ASSERT(toc_files1 == toc_files2);

    afs_toc_file * toc_file = get_afs_toc_file_instance3(3);
    DBOOL result = afs_toc_files_add_toc(toc_files1, toc_file);

    BOXING_ASSERT(result == DTRUE);
    test_not_empty_afs_toc_files(toc_files2, 4);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files1);
}
END_TEST


// Test function afs_toc_files_equal test with input afs_toc_files pointers equal to NULL
BOXING_START_TEST(afs_toc_files_equal_test1)
{
    afs_toc_files * toc_files1 = NULL;
    afs_toc_files * toc_files2 = NULL;

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DTRUE);
}
END_TEST

// Test function afs_toc_files_equal test with empty input afs_toc_files instances 
BOXING_START_TEST(afs_toc_files_equal_test2)
{
    afs_toc_files * toc_files1 = afs_toc_files_create();
    test_null_afs_toc_files(toc_files1);

    afs_toc_files * toc_files2 = afs_toc_files_create();
    test_null_afs_toc_files(toc_files2);

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_equal test with not empty input afs_toc_files instances and empty vector
BOXING_START_TEST(afs_toc_files_equal_test3)
{
    afs_toc_indices * tocs1 = gvector_create_pointers(0);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs1);
    test_empty_afs_toc_files(toc_files1);

    afs_toc_indices* tocs2 = gvector_create_pointers(0);
    afs_toc_files * toc_files2 = afs_toc_files_create2(tocs2);
    test_empty_afs_toc_files(toc_files2);

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_equal test with not empty input afs_toc_files instances and vector with one pointer
BOXING_START_TEST(afs_toc_files_equal_test4)
{
    afs_toc_indices * tocs1 = get_filled_tocs_vector(1);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs1);
    test_not_empty_afs_toc_files(toc_files1, 1);

    afs_toc_indices * tocs2 = get_filled_tocs_vector(1);
    afs_toc_files * toc_files2 = afs_toc_files_create2(tocs2);
    test_not_empty_afs_toc_files(toc_files2, 1);

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_equal test with not empty input afs_toc_files instances and vector with three pointers
BOXING_START_TEST(afs_toc_files_equal_test5)
{
    afs_toc_indices * tocs1 = get_filled_tocs_vector(3);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs1);
    test_not_empty_afs_toc_files(toc_files1, 3);

    afs_toc_indices * tocs2 = get_filled_tocs_vector(3);
    afs_toc_files * toc_files2 = afs_toc_files_create2(tocs2);
    test_not_empty_afs_toc_files(toc_files2, 3);

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_equal test with empty input afs_toc_files instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_files_equal_test6)
{
    afs_toc_files * toc_files1 = afs_toc_files_create();
    test_null_afs_toc_files(toc_files1);

    afs_toc_files * toc_files2 = NULL;

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files1);
}
END_TEST


// Test function afs_toc_files_equal test with input afs_toc_files instance equal to NULL and instance with empty vector 
BOXING_START_TEST(afs_toc_files_equal_test7)
{
    afs_toc_indices * tocs1 = gvector_create_pointers(0);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs1);
    test_empty_afs_toc_files(toc_files1);

    afs_toc_files * toc_files2 = NULL;

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files1);
}
END_TEST


// Test function afs_toc_files_equal test with input afs_toc_files pointer equal to NULL and instance with filled vector 
BOXING_START_TEST(afs_toc_files_equal_test8)
{
    afs_toc_indices * tocs1 = get_filled_tocs_vector(3);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs1);
    test_not_empty_afs_toc_files(toc_files1, 3);

    afs_toc_files * toc_files2 = NULL;

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files1);
}
END_TEST


// Test function afs_toc_files_equal test with empty input afs_toc_files instance and instance with empty vector 
BOXING_START_TEST(afs_toc_files_equal_test9)
{
    afs_toc_indices * tocs1 = gvector_create_pointers(0);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs1);
    test_empty_afs_toc_files(toc_files1);

    afs_toc_files * toc_files2 = afs_toc_files_create();
    test_null_afs_toc_files(toc_files2);

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_equal test with empty input afs_toc_files instance and instance with filled vector 
BOXING_START_TEST(afs_toc_files_equal_test10)
{
    afs_toc_indices * tocs1 = get_filled_tocs_vector(3);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs1);
    test_not_empty_afs_toc_files(toc_files1, 3);

    afs_toc_files * toc_files2 = afs_toc_files_create();
    test_null_afs_toc_files(toc_files2);

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_equal test with input afs_toc_files instance with empty vector and instance with filled vector 
BOXING_START_TEST(afs_toc_files_equal_test11)
{
    afs_toc_indices * tocs1 = get_filled_tocs_vector(3);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs1);
    test_not_empty_afs_toc_files(toc_files1, 3);

    afs_toc_indices * tocs2 = gvector_create_pointers(0);
    afs_toc_files * toc_files2 = afs_toc_files_create2(tocs2);
    test_empty_afs_toc_files(toc_files2);

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_equal test with input afs_toc_files instances with filled vector with different count pointers
BOXING_START_TEST(afs_toc_files_equal_test12)
{
    afs_toc_indices * tocs1 = get_filled_tocs_vector(1);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs1);
    test_not_empty_afs_toc_files(toc_files1, 1);

    afs_toc_indices * tocs2 = get_filled_tocs_vector(2);
    afs_toc_files * toc_files2 = afs_toc_files_create2(tocs2);
    test_not_empty_afs_toc_files(toc_files2, 2);

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_equal test to afs_toc_files instances with different data
BOXING_START_TEST(afs_toc_files_equal_test13)
{
    afs_toc_indices * tocs1 = get_filled_tocs_vector(1);
    afs_toc_files * toc_files1 = afs_toc_files_create2(tocs1);
    test_not_empty_afs_toc_files(toc_files1, 1);

    afs_toc_indices * tocs2 = gvector_create_pointers(1);

    GVECTORN(tocs2, afs_toc_file *, 0) = get_afs_toc_file_instance3(5);
    afs_toc_files * toc_files2 = afs_toc_files_create2(tocs2);

    DBOOL result = afs_toc_files_equal(toc_files1, toc_files2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_files_equal(toc_files2, toc_files1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files1);
    afs_toc_files_free(toc_files2);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files pointers equal to NULL and afs_toc_file pointer equal to NULL
BOXING_START_TEST(afs_toc_files_add_toc_test1)
{
    afs_toc_files * toc_files = NULL;
    afs_toc_file * toc_file = NULL;

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files pointers equal to NULL and valid afs_toc_file instance
BOXING_START_TEST(afs_toc_files_add_toc_test2)
{
    afs_toc_files * toc_files = NULL;
    afs_toc_file * toc_file = get_afs_toc_file_instance3(1);

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files pointers equal to NULL and not valid afs_toc_file instance
BOXING_START_TEST(afs_toc_files_add_toc_test3)
{
    afs_toc_files * toc_files = NULL;
    afs_toc_file * toc_file = get_afs_toc_file_instance3(1);
    toc_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_file->start_frame = -1;

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_files_add_toc test with empty input afs_toc_files instance and afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_files_add_toc_test4)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_toc_file * toc_file = NULL;

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_toc test with empty input afs_toc_files instance and empty afs_toc_file instance
BOXING_START_TEST(afs_toc_files_add_toc_test5)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_toc_file * toc_file = afs_toc_file_create();

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_files_add_toc test with empty input afs_toc_files instance and not valid afs_toc_file instance
BOXING_START_TEST(afs_toc_files_add_toc_test6)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_toc_file * toc_file = get_afs_toc_file_instance3(1);
    toc_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_file->start_frame = -1;

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_files_add_toc test with empty input afs_toc_files instance and valid afs_toc_file instance
BOXING_START_TEST(afs_toc_files_add_toc_test7)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_toc_file * toc_file = get_afs_toc_file_instance3(0);

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_files(toc_files, 1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files instance with empty vector and afs_toc_file pointers equal to NULL
BOXING_START_TEST(afs_toc_files_add_toc_test8)
{
    afs_toc_indices * tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    afs_toc_file * toc_file = NULL;

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files instance with empty vector and empty afs_toc_file instance
BOXING_START_TEST(afs_toc_files_add_toc_test9)
{
    afs_toc_indices * tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    afs_toc_file * toc_file = afs_toc_file_create();

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files instance with empty vector and not valid afs_toc_file
BOXING_START_TEST(afs_toc_files_add_toc_test10)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    afs_toc_file * toc_file = get_afs_toc_file_instance3(1);
    toc_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_file->start_frame = -1;

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files instance with empty vector and valid afs_toc_file instance
BOXING_START_TEST(afs_toc_files_add_toc_test11)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    afs_toc_file * toc_file = get_afs_toc_file_instance3(0);

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_files(toc_files, 1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files instance with filled vector and afs_toc_files pointers equal to NULL
BOXING_START_TEST(afs_toc_files_add_toc_test12)
{
    afs_toc_indices* tocs = get_filled_tocs_vector(1);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    afs_toc_file * toc_file = NULL;

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_files(toc_files, 1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files instance with filled vector and empty afs_toc_file instance
BOXING_START_TEST(afs_toc_files_add_toc_test13)
{
    afs_toc_indices* tocs = get_filled_tocs_vector(1);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    afs_toc_file * toc_file = afs_toc_file_create();

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_files(toc_files, 1);

    afs_toc_files_free(toc_files);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files instance with filled vector and not valid afs_toc_file instance
BOXING_START_TEST(afs_toc_files_add_toc_test14)
{
    afs_toc_indices* tocs = get_filled_tocs_vector(1);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    afs_toc_file * toc_file = get_afs_toc_file_instance3(1);
    toc_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
    toc_file->start_frame = -1;

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_files(toc_files, 1);

    afs_toc_files_free(toc_files);
    afs_toc_file_free(toc_file);
}
END_TEST


// Test function afs_toc_files_add_toc test with input afs_toc_files instance with filled vector and valid afs_toc_file instance
BOXING_START_TEST(afs_toc_files_add_toc_test15)
{
    afs_toc_indices* tocs = get_filled_tocs_vector(1);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    afs_toc_file * toc_file = get_afs_toc_file_instance3(1);

    DBOOL result = afs_toc_files_add_toc(toc_files, toc_file);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_files(toc_files, 2);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_toc test with input afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_get_toc_test1)
{
    afs_toc_files * toc_files = NULL;
    afs_toc_file * toc_file = afs_toc_files_get_toc(toc_files, 0);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(toc_file == NULL);
}
END_TEST


// Test function afs_toc_files_get_toc test with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_files_get_toc_test2)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    afs_toc_file * toc_file = afs_toc_files_get_toc(toc_files, 0);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(toc_file == NULL);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_toc test with afs_toc_files instance with empty vector
BOXING_START_TEST(afs_toc_files_get_toc_test3)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    afs_toc_file * toc_file = afs_toc_files_get_toc(toc_files, 0);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(toc_file == NULL);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_toc test with not empty afs_toc_files instance and index out of range
BOXING_START_TEST(afs_toc_files_get_toc_test4)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(1);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    afs_toc_file * toc_file = afs_toc_files_get_toc(toc_files, 1);

    test_not_empty_afs_toc_files(toc_files, 1);
    BOXING_ASSERT(toc_file == NULL);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_toc test with not empty afs_toc_files instance and index in range
BOXING_START_TEST(afs_toc_files_get_toc_test5)
{
    afs_toc_indices* tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file* afs_toc_file_instance = get_afs_toc_file_instance3(i);
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_files->tocs, afs_toc_file *, i), afs_toc_file_instance) == DTRUE);
        afs_toc_file_free(afs_toc_file_instance);
    }

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_tocs_count test with input afs_toc_files pointers equal to NULL
BOXING_START_TEST(afs_toc_files_get_tocs_count_test1)
{
    afs_toc_files * toc_files = NULL;
    size_t tocs_count = afs_toc_files_get_tocs_count(toc_files);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(tocs_count == 0);
}
END_TEST


// Test function afs_toc_files_get_tocs_count test with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_files_get_tocs_count_test2)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    size_t tocs_count = afs_toc_files_get_tocs_count(toc_files);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(tocs_count == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_tocs_count test with afs_toc_files instance with empty vector
BOXING_START_TEST(afs_toc_files_get_tocs_count_test3)
{
    afs_toc_indices * tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    size_t tocs_count = afs_toc_files_get_tocs_count(toc_files);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(tocs_count == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_tocs_count test with afs_toc_files instance with filled vector
BOXING_START_TEST(afs_toc_files_get_tocs_count_test4)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    size_t tocs_count = afs_toc_files_get_tocs_count(toc_files);

    test_not_empty_afs_toc_files(toc_files, 3);
    BOXING_ASSERT(tocs_count == 3);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_duration_frames_test1)
{
    afs_toc_files * toc_files = NULL;

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED, NULL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(duration_frames == 0);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_duration_frames_test2)
{
    afs_toc_files * toc_files = NULL;

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(duration_frames == 0);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_duration_frames_test3)
{
    afs_toc_files * toc_files = NULL;

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_PREVIEW, NULL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(duration_frames == 0);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_duration_frames_test4)
{
    afs_toc_files * toc_files = NULL;

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_ALL, NULL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(duration_frames == 0);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_duration_frames_test5)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED, NULL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(duration_frames == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_duration_frames_test6)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(duration_frames == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_duration_frames_test7)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_PREVIEW, NULL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(duration_frames == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_duration_frames_test8)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_ALL, NULL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(duration_frames == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files instance with empty vector and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_duration_frames_test9)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED, NULL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(duration_frames == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files instance with empty vector and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_duration_frames_test10)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(duration_frames == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files instance with empty vector and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_duration_frames_test11)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_PREVIEW, NULL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(duration_frames == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files instance with empty vector and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_duration_frames_test12)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_ALL, NULL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(duration_frames == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_duration_frames_test13)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED, NULL);

    test_framecount_toc_unchanged( toc_files );
    BOXING_ASSERT(duration_frames == 0);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_duration_frames_test14)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL);

    test_framecount_toc_unchanged(toc_files);
    BOXING_ASSERT( duration_frames == FRAMECOUNT_TOC_DURATION_DIGITAL );

    GVECTORN(toc_files->tocs, afs_toc_file *, 2)->end_frame+=10;

    duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL);
    BOXING_ASSERT( duration_frames == FRAMECOUNT_TOC_DURATION_DIGITAL + 10 );

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_duration_frames_test15)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    afs_toc_preview_layout_definitions* definitions = create_framecount_definitions();

    int duration_frames = afs_toc_files_get_duration_frames( toc_files, AFS_TOC_FILE_TYPE_PREVIEW, definitions );

    test_framecount_toc_unchanged( toc_files );
    BOXING_ASSERT( duration_frames == FRAMECOUNT_TOC_DURATION_VISUAL );

    GVECTORN(toc_files->tocs, afs_toc_file *, 2)->end_frame += 10;

    duration_frames = afs_toc_files_get_duration_frames( toc_files, AFS_TOC_FILE_TYPE_PREVIEW, definitions );
    // See comment above
    BOXING_ASSERT( duration_frames == FRAMECOUNT_TOC_DURATION_VISUAL );

    afs_toc_preview_layout_definitions_free( definitions );
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_duration_frames with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_duration_frames_test16)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    afs_toc_preview_layout_definitions* definitions = create_framecount_definitions();

    int duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_ALL, definitions );

    test_framecount_toc_unchanged(toc_files);
    BOXING_ASSERT( duration_frames == FRAMECOUNT_TOC_DURATION_ALL );

    GVECTORN(toc_files->tocs, afs_toc_file *, 2)->end_frame += 10;

    duration_frames = afs_toc_files_get_duration_frames(toc_files, AFS_TOC_FILE_TYPE_ALL, definitions );
    BOXING_ASSERT( duration_frames == FRAMECOUNT_TOC_DURATION_ALL + 10 );

    afs_toc_preview_layout_definitions_free( definitions );
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_first_frame_test1)
{
    afs_toc_files * toc_files = NULL;

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(start_frame == -1);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_first_frame_test2)
{
    afs_toc_files * toc_files = NULL;

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_DIGITAL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(start_frame == -1);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_first_frame_test3)
{
    afs_toc_files * toc_files = NULL;

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_PREVIEW);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(start_frame == -1);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_first_frame_test4)
{
    afs_toc_files * toc_files = NULL;

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_ALL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(start_frame == -1);
}
END_TEST


// Test function afs_toc_files_get_first_frame with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_first_frame_test5)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_first_frame_test6)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_DIGITAL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_first_frame_test7)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_PREVIEW);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_first_frame_test8)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_ALL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files instance with empty tocs vector and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_first_frame_test9)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files instance with empty tocs vector and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_first_frame_test10)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_DIGITAL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files instance with empty tocs vector and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_first_frame_test11)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_PREVIEW);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files instance with empty tocs vector and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_first_frame_test12)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_ALL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_first_frame_test13)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED);

    test_framecount_toc_unchanged( toc_files );
    BOXING_ASSERT(start_frame == -1);

    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->start_frame = 12;

    start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_first_frame_test14)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_DIGITAL);

    test_framecount_toc_unchanged(toc_files);
    BOXING_ASSERT( start_frame == FRAMECOUNT_TOC_FIRST_DIGITAL );

    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->start_frame += 10;

    start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_DIGITAL);
    BOXING_ASSERT( start_frame == FRAMECOUNT_TOC_FIRST_DIGITAL + 10 );

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_first_frame_test15)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_PREVIEW);

    test_framecount_toc_unchanged( toc_files );
    BOXING_ASSERT( start_frame == FRAMECOUNT_TOC_FIRST_VISUAL );

    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->start_frame = 12;

    start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_PREVIEW);
    BOXING_ASSERT( start_frame == FRAMECOUNT_TOC_FIRST_VISUAL );

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_first_frame with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_first_frame_test16)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_ALL);

    test_framecount_toc_unchanged( toc_files );
    BOXING_ASSERT( start_frame == FRAMECOUNT_TOC_FIRST_VISUAL );

    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->start_frame = 12;

    start_frame = afs_toc_files_get_first_frame(toc_files, AFS_TOC_FILE_TYPE_ALL);
    BOXING_ASSERT( start_frame == FRAMECOUNT_TOC_FIRST_VISUAL );

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_last_frame_test1)
{
    afs_toc_files * toc_files = NULL;

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED, NULL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(last_frame == -1);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_last_frame_test2)
{
    afs_toc_files * toc_files = NULL;

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(last_frame == -1);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_last_frame_test3)
{
    afs_toc_files * toc_files = NULL;

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_PREVIEW, NULL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(last_frame == -1);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_last_frame_test4)
{
    afs_toc_files * toc_files = NULL;

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_ALL, NULL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(last_frame == -1);
}
END_TEST


// Test function afs_toc_files_get_last_frame with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_last_frame_test5)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED, NULL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_last_frame_test6)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_last_frame_test7)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_PREVIEW, NULL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_last_frame_test8)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_ALL, NULL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files instance with empty tocs vector and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_last_frame_test9)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED, NULL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files instance with empty tocs vector and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_last_frame_test10)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files instance with empty tocs vector and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_last_frame_test11)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_PREVIEW, NULL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files instance with empty tocs vector and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_last_frame_test12)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_ALL, NULL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_get_last_frame_test13)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED, NULL);

    test_framecount_toc_unchanged( toc_files );
    BOXING_ASSERT(last_frame == -1);

    GVECTORN(toc_files->tocs, afs_toc_file *, 0)->end_frame = 12;

    last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_UNDEFINED, NULL);
    BOXING_ASSERT(last_frame == -1);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_get_last_frame_test14)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL);

    test_framecount_toc_unchanged( toc_files );
    BOXING_ASSERT( last_frame == FRAMECOUNT_TOC_LAST_DIGITAL );

    GVECTORN( toc_files->tocs, afs_toc_file *, 2 )->end_frame = FRAMECOUNT_TOC_LAST_DIGITAL + 100;

    last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_DIGITAL, NULL);
    BOXING_ASSERT( last_frame == FRAMECOUNT_TOC_LAST_DIGITAL + 100 );

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_get_last_frame_test15)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    afs_toc_preview_layout_definitions* definitions = create_framecount_definitions();
    int last_frame = afs_toc_files_get_last_frame( toc_files, AFS_TOC_FILE_TYPE_PREVIEW, definitions );

    test_framecount_toc_unchanged( toc_files );
    BOXING_ASSERT( last_frame == FRAMECOUNT_TOC_LAST_VISUAL );
    GVECTORN(toc_files->tocs, afs_toc_file *, 2)->end_frame += 10;

    last_frame = afs_toc_files_get_last_frame( toc_files, AFS_TOC_FILE_TYPE_PREVIEW, definitions );
    BOXING_ASSERT( last_frame == FRAMECOUNT_TOC_LAST_VISUAL );

    afs_toc_preview_layout_definitions_free( definitions );
    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_get_last_frame with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_get_last_frame_test16)
{
    afs_toc_indices * tocs = create_framecount_toc();
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    int last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_ALL, NULL);

    test_framecount_toc_unchanged(toc_files);
    BOXING_ASSERT(last_frame == 22);

    GVECTORN(toc_files->tocs, afs_toc_file *, 2)->end_frame = 25;

    last_frame = afs_toc_files_get_last_frame(toc_files, AFS_TOC_FILE_TYPE_ALL, NULL);
    BOXING_ASSERT(last_frame == 25);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_add_frame_offset_test1)
{
    afs_toc_files * toc_files = NULL;
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_UNDEFINED);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_add_frame_offset_test2)
{
    afs_toc_files * toc_files = NULL;
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_DIGITAL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_add_frame_offset_test3)
{
    afs_toc_files * toc_files = NULL;
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_PREVIEW);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files pointers equal to NULL and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_add_frame_offset_test4)
{
    afs_toc_files * toc_files = NULL;
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_ALL);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_add_frame_offset_test5)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_UNDEFINED);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(offset == 5);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_add_frame_offset_test6)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_DIGITAL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(offset == 5);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_add_frame_offset_test7)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_PREVIEW);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(offset == 5);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with empty input afs_toc_files instance and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_add_frame_offset_test8)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_ALL);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(offset == 5);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files instance with empty vector and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_add_frame_offset_test9)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_UNDEFINED);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(offset == 5);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files instance with empty vector and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_add_frame_offset_test10)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_DIGITAL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(offset == 5);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files instance with empty vector and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_add_frame_offset_test11)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_PREVIEW);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(offset == 5);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files instance with empty vector and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_add_frame_offset_test12)
{
    afs_toc_indices* tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_ALL);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(offset == 5);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_UNDEFINED
BOXING_START_TEST(afs_toc_files_add_frame_offset_test13)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    test_not_empty_afs_toc_files(toc_files, 3);

    unsigned int offset = 5;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_UNDEFINED);

    BOXING_ASSERT(offset == 5);
    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(tocs, afs_toc_file *, i);
        BOXING_ASSERT(toc_file->start_frame == (int)(2 + i));

        for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
        {
            afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
            BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j));
        }
    }

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_DIGITAL
BOXING_START_TEST(afs_toc_files_add_frame_offset_test14)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    test_not_empty_afs_toc_files(toc_files, 3);

    unsigned int offset = 6;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_DIGITAL);

    BOXING_ASSERT(offset == 6);
    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(tocs, afs_toc_file *, i);
        BOXING_ASSERT(toc_file->start_frame == (int)(2 + i + offset));

        for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
        {
            afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
            BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j));
        }
    }

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_PREVIEW
BOXING_START_TEST(afs_toc_files_add_frame_offset_test15)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    test_not_empty_afs_toc_files(toc_files, 3);

    unsigned int offset = 7;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_PREVIEW);

    BOXING_ASSERT(offset == 7);
    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(tocs, afs_toc_file *, i);
        BOXING_ASSERT(toc_file->start_frame == (int)(2 + i));

        for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
        {
            afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
            BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j + offset));
        }
    }

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_add_frame_offset test with input afs_toc_files instance with filled vector and type equal to AFS_TOC_FILE_TYPE_ALL
BOXING_START_TEST(afs_toc_files_add_frame_offset_test16)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);
    test_not_empty_afs_toc_files(toc_files, 3);

    unsigned int offset = 8;

    afs_toc_files_add_frame_offset(toc_files, offset, AFS_TOC_FILE_TYPE_ALL);

    BOXING_ASSERT(offset == 8);
    for (unsigned int i = 0; i < toc_files->tocs->size; i++)
    {
        afs_toc_file * toc_file = GVECTORN(tocs, afs_toc_file *, i);
        BOXING_ASSERT(toc_file->start_frame == (int)(2 + i + offset));

        for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
        {
            afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
            BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j + offset));
        }
    }

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_is_valid with input afs_toc_files pointers equal to NULL
BOXING_START_TEST(afs_toc_files_is_valid_test1)
{
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_is_valid(toc_files);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_files_is_valid with empty input afs_toc_files instance
BOXING_START_TEST(afs_toc_files_is_valid_test2)
{
    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_is_valid(toc_files);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_is_valid with input afs_toc_files instance with empty vector
BOXING_START_TEST(afs_toc_files_is_valid_test3)
{
    afs_toc_indices * tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    DBOOL result = afs_toc_files_is_valid(toc_files);

    test_empty_afs_toc_files(toc_files);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_is_valid with input afs_toc_files instance where vector filled not valid data
BOXING_START_TEST(afs_toc_files_is_valid_test4)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    test_not_empty_afs_toc_files(toc_files, 3);
    
    GVECTORN(tocs, afs_toc_file *, 1)->types = AFS_TOC_FILE_TYPE_DIGITAL;
    GVECTORN(tocs, afs_toc_file *, 1)->start_frame = -1;

    DBOOL result = afs_toc_files_is_valid(toc_files);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_is_valid with input afs_toc_files instance where vector filled valid data
BOXING_START_TEST(afs_toc_files_is_valid_test5)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    DBOOL result = afs_toc_files_is_valid(toc_files);

    test_not_empty_afs_toc_files(toc_files, 3);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_save_file with file_name pointer equal to NULL and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_save_file(toc_files, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_files_save_file with file_name pointer equal to NULL and afs_toc_files pointer not equal to NULL
BOXING_START_TEST(afs_toc_files_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    DBOOL result = afs_toc_files_save_file(toc_files, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_save_file with file_name pointer not equal to NULL and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_save_file_test3)
{
    const char * file_name = "afs_toc_files_save_file_test.xml";
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_save_file(toc_files, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_files_save_file with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_files_save_file_test4)
{
    const char * file_name = "afs_toc_files_save_file_empty_test.xml";
    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_save_file(toc_files, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_save_file with afs_toc_files instance with empty vector
BOXING_START_TEST(afs_toc_files_save_file_test5)
{
    const char * file_name = "afs_toc_files_save_file_empty_test.xml";
    afs_toc_indices * tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    DBOOL result = afs_toc_files_save_file(toc_files, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_save_file with afs_toc_files instance with not valid vector
BOXING_START_TEST(afs_toc_files_save_file_test6)
{
    const char * file_name = "afs_toc_files_save_file_not_valid_test.xml";
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    GVECTORN(tocs, afs_toc_file *, 1)->types = AFS_TOC_FILE_TYPE_DIGITAL;
    GVECTORN(tocs, afs_toc_file *, 1)->start_frame = -1;

    DBOOL result = afs_toc_files_save_file(toc_files, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_save_file with afs_toc_files instance with filled vector, in compact format
BOXING_START_TEST(afs_toc_files_save_file_test7)
{
    const char * file_name = "afs_toc_files_save_file_compact_test.xml";

    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    DBOOL result = afs_toc_files_save_file(toc_files, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_save_file with afs_toc_files instance with filled vector, in not compact format
BOXING_START_TEST(afs_toc_files_save_file_test8)
{
    const char * file_name = "afs_toc_files_save_file_not_compact_test.xml";

    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    DBOOL result = afs_toc_files_save_file(toc_files, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_save_string with afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_save_string_test1)
{
    afs_toc_files * toc_files = NULL;

    char * result = afs_toc_files_save_string(toc_files, DTRUE);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_files_save_string with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_files_save_string_test2)
{
    char * test_string = "<?xml version=\"1.0\" encoding=\"utf-8\"?><files />\n";
    afs_toc_files * toc_files = afs_toc_files_create();

    char * result = afs_toc_files_save_string(toc_files, DTRUE);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT_STR_EQ(test_string, result);

    afs_toc_files_free(toc_files);
    free(result);
}
END_TEST


// Test function afs_toc_files_save_string with afs_toc_files instance with empty vector
BOXING_START_TEST(afs_toc_files_save_string_test3)
{
    char * test_string = "<?xml version=\"1.0\" encoding=\"utf-8\"?><files />\n";

    afs_toc_indices * tocs = gvector_create_pointers(0);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    char * result = afs_toc_files_save_string(toc_files, DTRUE);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT_STR_EQ(test_string, result);

    afs_toc_files_free(toc_files);
    free(result);
}
END_TEST


// Test function afs_toc_files_save_string with not valid afs_toc_files instance
BOXING_START_TEST(afs_toc_files_save_string_test4)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    GVECTORN(tocs, afs_toc_file *, 1)->types = AFS_TOC_FILE_TYPE_DIGITAL;
    GVECTORN(tocs, afs_toc_file *, 1)->start_frame = -1;

    char * result = afs_toc_files_save_string(toc_files, DTRUE);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_save_string with valid afs_toc_files instance, in compact format
BOXING_START_TEST(afs_toc_files_save_string_test5)
{
    char * test_string = read_xml_tocs_file("afs_toc_files_save_file_compact_test.xml");

    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    char * result = afs_toc_files_save_string(toc_files, DTRUE);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT_STR_EQ(result, test_string);

    afs_toc_files_free(toc_files);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_files_save_string with valid afs_toc_files instance, in not compact format
BOXING_START_TEST(afs_toc_files_save_string_test6)
{
    char * test_string = read_xml_tocs_file("afs_toc_files_save_file_not_compact_test.xml");

    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    char * result = afs_toc_files_save_string(toc_files, DFALSE);

    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT_STR_EQ(result, test_string);

    afs_toc_files_free(toc_files);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_files_save_xml with mxml_node_t pointer equal to NULL and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_save_xml(toc_files, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_files_save_xml with mxml_node_t pointer equal to NULL and afs_toc_files pointer not equal to NULL
BOXING_START_TEST(afs_toc_files_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    DBOOL result = afs_toc_files_save_xml(toc_files, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_save_xml(toc_files, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_files_save_xml with empty afs_toc_files instance
BOXING_START_TEST(afs_toc_files_save_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_save_xml(toc_files, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_files_save_xml with not valid afs_toc_files instance
BOXING_START_TEST(afs_toc_files_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    GVECTORN(tocs, afs_toc_file *, 1)->types = AFS_TOC_FILE_TYPE_DIGITAL;
    GVECTORN(tocs, afs_toc_file *, 1)->start_frame = -1;

    DBOOL result = afs_toc_files_save_xml(toc_files, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_files_save_xml with valid afs_toc_files instance
BOXING_START_TEST(afs_toc_files_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    DBOOL result = afs_toc_files_save_xml(toc_files, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_files_free(toc_files);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_files_load_file with file_name pointer equal to NULL and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_load_file(toc_files, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_files_load_file with file_name pointer equal to NULL and afs_toc_files pointer not equal to NULL
BOXING_START_TEST(afs_toc_files_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_file(toc_files, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_files != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_load_file with file_name pointer not equal to NULL and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_load_file_test3)
{
    const char * file_name = "afs_toc_files_save_file_compact_test.xml";
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_load_file(toc_files, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_files_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_files_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_file(toc_files, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_files(toc_files);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_load_file with initialized afs_toc_files instance, in compact format
BOXING_START_TEST(afs_toc_files_load_file_test5)
{
    const char * file_name = "afs_toc_files_save_file_compact_test.xml";

    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_file(toc_files, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_files(toc_files, 3);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_load_file with initialized afs_toc_files instance, in not compact format
BOXING_START_TEST(afs_toc_files_load_file_test6)
{
    const char * file_name = "afs_toc_files_save_file_not_compact_test.xml";
    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_file(toc_files, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_files(toc_files, 3);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_load_string with input string pointer equal to NULL and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_load_string(toc_files, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_files_load_string with input string pointer equal to NULL and afs_toc_files pointer not equal to NULL
BOXING_START_TEST(afs_toc_files_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_string(toc_files, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_files(toc_files);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_load_string with input string pointer not equal to NULL and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_load_string_test3)
{
    char * input_string = read_xml_tocs_file("afs_toc_files_save_file_compact_test.xml");
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_load_string(toc_files, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_files_load_string with empty input string
BOXING_START_TEST(afs_toc_files_load_string_test4)
{
    char * input_string = "";
    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_string(toc_files, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_files(toc_files);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_load_string with input string for initialized afs_toc_files instance, in compact format
BOXING_START_TEST(afs_toc_files_load_string_test5)
{
    char * input_string = read_xml_tocs_file("afs_toc_files_save_file_compact_test.xml");

    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_string(toc_files, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_files(toc_files, 3);

    afs_toc_files_free(toc_files);
    free(input_string);
}
END_TEST


// Test function afs_toc_files_load_string with input string for initialized afs_toc_files instance, in not compact format
BOXING_START_TEST(afs_toc_files_load_string_test6)
{
    char * input_string = read_xml_tocs_file("afs_toc_files_save_file_not_compact_test.xml");

    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_string(toc_files, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_files(toc_files, 3);

    afs_toc_files_free(toc_files);
    free(input_string);
}
END_TEST


// Test function afs_toc_files_load_xml with xml instance pointer equal to NULL and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_load_xml(toc_files, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_files_load_xml with xml instance pointer equal to NULL and afs_toc_files pointer not equal to NULL
BOXING_START_TEST(afs_toc_files_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_xml(toc_files, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_files(toc_files);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_load_xml with xml instance pointer not equal to NULL and afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_load_xml_test3)
{
    mxml_node_t *tree = get_tocs_xml_instance("afs_toc_files_save_file_compact_test.xml");
    afs_toc_files * toc_files = NULL;

    DBOOL result = afs_toc_files_load_xml(toc_files, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_files_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_files_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_xml(toc_files, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_null_afs_toc_files(toc_files);

    afs_toc_files_free(toc_files);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_files_load_xml with xml instance for initialized afs_toc_files instance, in compact format
BOXING_START_TEST(afs_toc_files_load_xml_test5)
{
    mxml_node_t *tree = get_tocs_xml_instance("afs_toc_files_save_file_compact_test.xml");
    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_xml(toc_files, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_files(toc_files, 3);

    afs_toc_files_free(toc_files);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_files_load_xml with xml instance for initialized afs_toc_files instance, in not compact format
BOXING_START_TEST(afs_toc_files_load_xml_test6)
{
    mxml_node_t *tree = get_tocs_xml_instance("afs_toc_files_save_file_not_compact_test.xml");
    afs_toc_files * toc_files = afs_toc_files_create();

    DBOOL result = afs_toc_files_load_xml(toc_files, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_files(toc_files, 3);

    afs_toc_files_free(toc_files);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_files_save_as_table with input afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_save_as_table_test1)
{
    afs_toc_files * toc_files = NULL;
    char * table_string = afs_toc_files_save_as_table(toc_files);

    const char * test_string = "<fileId> <uniqueId> <parentId> <formatId> <fileName>\n\n";

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT_STR_EQ(table_string, test_string);
    free(table_string);
}
END_TEST


// Test function afs_toc_files_save_as_table with empty input afs_toc_files instance
BOXING_START_TEST(afs_toc_files_save_as_table_test2)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    char * table_string = afs_toc_files_save_as_table(toc_files);

    const char * test_string = "<fileId> <uniqueId> <parentId> <formatId> <fileName>\n\n";

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT_STR_EQ(table_string, test_string);

    afs_toc_files_free(toc_files);
    free(table_string);
}
END_TEST


// Test function afs_toc_files_save_as_table with input afs_toc_files instance with different parent id values
BOXING_START_TEST(afs_toc_files_save_as_table_test3)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(4);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    GVECTORN(tocs, afs_toc_file *, 1)->parent_id = AFS_TOC_FILE_PARENT;
    GVECTORN(tocs, afs_toc_file *, 2)->parent_id = -5;
    GVECTORN(tocs, afs_toc_file *, 3)->parent_id = 5;

    char * table_string = afs_toc_files_save_as_table(toc_files);
    
    const char * test_string =
        "<fileId> <uniqueId> <parentId> <formatId> <fileName>\n"
        "-2 38A0AAAB-16B2-640C-5353-6DB8AE4367B9 0N file_format input-toc.xml\n"
        "-2 38A0AAAB-16B2-640C-5353-6DB8AE4367B9 0P file_format input-toc.xml\n"
        "-2 38A0AAAB-16B2-640C-5353-6DB8AE4367B9 -5 file_format input-toc.xml\n"
        "-2 38A0AAAB-16B2-640C-5353-6DB8AE4367B9 05 file_format input-toc.xml\n";

    BOXING_ASSERT(table_string != NULL);
    BOXING_ASSERT_STR_EQ(table_string, test_string);

    afs_toc_files_free(toc_files);
    free(table_string);
}
END_TEST


// Test function afs_toc_files_save_as_table with input afs_toc_files instance with different file format values
BOXING_START_TEST(afs_toc_files_save_as_table_test4)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    afs_toc_file_set_file_format(GVECTORN(tocs, afs_toc_file *, 1), NULL);
    afs_toc_file_set_file_format(GVECTORN(tocs, afs_toc_file *, 2), "");

    char * table_string = afs_toc_files_save_as_table(toc_files);

    const char * test_string =
        "<fileId> <uniqueId> <parentId> <formatId> <fileName>\n"
        "-2 38A0AAAB-16B2-640C-5353-6DB8AE4367B9 N file_format input-toc.xml\n"
        "-2 38A0AAAB-16B2-640C-5353-6DB8AE4367B9 N N           input-toc.xml\n"
        "-2 38A0AAAB-16B2-640C-5353-6DB8AE4367B9 N N           input-toc.xml\n";

    BOXING_ASSERT_STR_EQ(table_string, test_string);
    BOXING_ASSERT(table_string != NULL);

    afs_toc_files_free(toc_files);
    free(table_string);
}
END_TEST


// Test function afs_toc_files_save_as_table with input afs_toc_files instance with long values
BOXING_START_TEST(afs_toc_files_save_as_table_test5)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    GVECTORN(tocs, afs_toc_file *, 1)->id = -2123456789;
    GVECTORN(tocs, afs_toc_file *, 1)->parent_id = -2123456789;
    afs_toc_file_set_unique_id(GVECTORN(tocs, afs_toc_file *, 1), "long long long unique id string.........");
    afs_toc_file_set_file_format(GVECTORN(tocs, afs_toc_file *, 1), "long long long file format string......");
    afs_toc_file_set_name(GVECTORN(tocs, afs_toc_file *, 1), "long long long file name string......");

    char * table_string = afs_toc_files_save_as_table(toc_files);
    
    const char * test_string =
        "<fileId> <uniqueId> <parentId> <formatId> <fileName>\n"
        "-0000000002 38A0AAAB-16B2-640C-5353-6DB8AE4367B9 0000000000N file_format                             input-toc.xml\n"
        "-2123456789 long long long unique id string......... -2123456789 long long long file format string...... long long long file name string......\n"
        "-0000000002 38A0AAAB-16B2-640C-5353-6DB8AE4367B9 0000000000N file_format                             input-toc.xml\n";
    
    BOXING_ASSERT_STR_EQ(table_string, test_string);
    BOXING_ASSERT(table_string != NULL);

    afs_toc_files_free(toc_files);
    free(table_string);
}
END_TEST


// Test function afs_toc_files_save_as_table with input afs_toc_files instance with short values
BOXING_START_TEST(afs_toc_files_save_as_table_test6)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(1);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    GVECTORN(tocs, afs_toc_file *, 0)->id = 0;
    afs_toc_file_set_unique_id(GVECTORN(tocs, afs_toc_file *, 0), "s");
    afs_toc_file_set_file_format(GVECTORN(tocs, afs_toc_file *, 0), "h");
    afs_toc_file_set_name(GVECTORN(tocs, afs_toc_file *, 0), "o");

    char * table_string = afs_toc_files_save_as_table(toc_files);
    
    const char * test_string =
        "<fileId> <uniqueId> <parentId> <formatId> <fileName>\n"
        "0 s                                    N h o\n";
    
    BOXING_ASSERT_STR_EQ(table_string, test_string);
    BOXING_ASSERT(table_string != NULL);

    afs_toc_files_free(toc_files);
    free(table_string);
}
END_TEST


// Test function afs_toc_files_save_as_location_table with input afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_save_as_location_table_test1)
{
    afs_toc_files * toc_files = NULL;
    char * table_string = afs_toc_files_save_as_location_table(toc_files);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(table_string == NULL);
}
END_TEST


// Test function afs_toc_files_save_as_location_table with empty input afs_toc_files instance
BOXING_START_TEST(afs_toc_files_save_as_location_table_test2)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    char * table_string = afs_toc_files_save_as_location_table(toc_files);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(strcmp(table_string, "") == 0);

    afs_toc_files_free(toc_files);
    free(table_string);
}
END_TEST

 
// Test function afs_toc_files_save_as_location_table with input afs_toc_files instance with long values
BOXING_START_TEST(afs_toc_files_save_as_location_table_test3)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    GVECTORN(tocs, afs_toc_file *, 1)->start_frame = -2123456789;
    GVECTORN(tocs, afs_toc_file *, 1)->start_byte = -2123456789;
    GVECTORN(tocs, afs_toc_file *, 1)->end_frame = -2123456789;
    GVECTORN(tocs, afs_toc_file *, 1)->end_byte = -2123456789;
    GVECTORN(tocs, afs_toc_file *, 1)->size = 9223372036854775807;
    free(GVECTORN(tocs, afs_toc_file *, 1)->checksum);
    GVECTORN(tocs, afs_toc_file *, 1)->checksum = boxing_string_clone("long long long file checksum string................");

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(GVECTORN(tocs, afs_toc_file *, 1)->preview, 1);

    free(toc_file_preview_page->layout_id);
    toc_file_preview_page->layout_id = boxing_string_clone("long long long layout id string..........................");
    toc_file_preview_page->start_frame = 4294967295;
    toc_file_preview_page->start_section = 4294967295;
    toc_file_preview_page->section_count = 4294967295;
    toc_file_preview_page->dimension_x = 4294967295;
    toc_file_preview_page->dimension_y = 4294967295;
    toc_file_preview_page->overlap_x = 4294967295;
    toc_file_preview_page->overlap_y = 4294967295;

    char * table_string = afs_toc_files_save_as_location_table(toc_files);
    
    const char * test_string =
        "DIGITAL DATA LOCATIONS\n"
        "======================\n"
        "<fileId> <startFrame> <startByte> <endFrame> <endByte> <size> <sha1Checksum>\n"
        "-2 00000000002 00000000000 00000000002 00000029671 0000000000000029672 f2eb0d6c6964d206012695a10ea6cd877c95c340\n"
        "-2 -2123456789 -2123456789 -2123456789 -2123456789 9223372036854775807 long long long file checksum string................\n"
        "-2 00000000004 00000000002 00000000004 00000029673 0000000000000029674 f2eb0d6c6964d206012695a10ea6cd877c95c340\n"
        "\n"
        "VISUAL DATA LOCATIONS\n"
        "=====================\n"
        "<fileId> <layoutId> <startFrame> <startSection> <sectionCount> <dimensionWxH> <overlapWxH>\n"
        "-2 0000000000000000000000000000000000000000000Some layout ID 0000000009 0000000008 0000000007 00006x00005 04x03\n"
        "-2 0000000000000000000000000000000000000000000Some layout ID 0000000010 0000000009 0000000008 00007x00006 05x04\n"
        "-2 0000000000000000000000000000000000000000000Some layout ID 0000000011 0000000010 0000000009 00008x00007 06x05\n"
        "-2 0000000000000000000000000000000000000000000Some layout ID 0000000009 0000000008 0000000007 00006x00005 04x03\n"
        "-2 long long long layout id string.......................... 4294967295 4294967295 4294967295 4294967295x4294967295 4294967295x4294967295\n"
        "-2 0000000000000000000000000000000000000000000Some layout ID 0000000011 0000000010 0000000009 00008x00007 06x05\n"
        "-2 0000000000000000000000000000000000000000000Some layout ID 0000000009 0000000008 0000000007 00006x00005 04x03\n"
        "-2 0000000000000000000000000000000000000000000Some layout ID 0000000010 0000000009 0000000008 00007x00006 05x04\n"
        "-2 0000000000000000000000000000000000000000000Some layout ID 0000000011 0000000010 0000000009 00008x00007 06x05\n";

    BOXING_ASSERT_STR_EQ(table_string, test_string);
    BOXING_ASSERT(table_string != NULL);

    afs_toc_files_free(toc_files);
    free(table_string);
}
END_TEST


// Test function afs_toc_files_save_as_location_table with input afs_toc_files instance with short values
BOXING_START_TEST(afs_toc_files_save_as_location_table_test4)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector5(1);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(5);

    afs_toc_file * toc_file = afs_toc_file_create2(
        "input-toc.xml",
        "2016-11-22 09:07:11",
        "f2eb0d6c6964d206012695a10ea6cd877c95c340",
        29672,
        0,
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

    afs_toc_indices * tocs = gvector_create_pointers(1);
    GVECTORN(tocs, afs_toc_file *, 0) = toc_file;
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    GVECTORN(tocs, afs_toc_file *, 0)->start_frame = 0;
    GVECTORN(tocs, afs_toc_file *, 0)->start_byte = 0;
    GVECTORN(tocs, afs_toc_file *, 0)->end_frame = 0;
    GVECTORN(tocs, afs_toc_file *, 0)->end_byte = 0;
    GVECTORN(tocs, afs_toc_file *, 0)->size = 0;
    afs_toc_file_set_checksum(GVECTORN(tocs, afs_toc_file *, 0), "s");

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file_preview, 0);

    free(toc_file_preview_page->layout_id);
    toc_file_preview_page->layout_id = boxing_string_clone("h");
    toc_file_preview_page->start_frame = 0;
    toc_file_preview_page->start_section = 0;
    toc_file_preview_page->section_count = 0;
    toc_file_preview_page->dimension_x = 0;
    toc_file_preview_page->dimension_y = 0;
    toc_file_preview_page->overlap_x = 0;
    toc_file_preview_page->overlap_y = 0;

    char * table_string = afs_toc_files_save_as_location_table(toc_files);
    
    const char * test_string =
        "DIGITAL DATA LOCATIONS\n"
        "======================\n"
        "<fileId> <startFrame> <startByte> <endFrame> <endByte> <size> <sha1Checksum>\n"
        "0 000 0000000 000 0000000 000000000000 s\n"
        "\n"
        "VISUAL DATA LOCATIONS\n"
        "=====================\n"
        "<fileId> <layoutId> <startFrame> <startSection> <sectionCount> <dimensionWxH> <overlapWxH>\n"
        "0 h 000 0 000 00000x00000 00x00\n";

    BOXING_ASSERT_STR_EQ(table_string, test_string);
    BOXING_ASSERT(table_string != NULL);

    afs_toc_files_free(toc_files);
    free(table_string);
}
END_TEST


// Test function afs_toc_files_save_as_metadata_table with input afs_toc_files pointer equal to NULL
BOXING_START_TEST(afs_toc_files_save_as_metadata_table_test1)
{
    afs_toc_files * toc_files = NULL;
    char * table_string = afs_toc_files_save_as_metadata_table(toc_files);

    BOXING_ASSERT(toc_files == NULL);
    BOXING_ASSERT(table_string == NULL);
}
END_TEST


// Test function afs_toc_files_save_as_metadata_table with empty input afs_toc_files instance
BOXING_START_TEST(afs_toc_files_save_as_metadata_table_test2)
{
    afs_toc_files * toc_files = afs_toc_files_create();
    char * table_string = afs_toc_files_save_as_metadata_table(toc_files);

    test_null_afs_toc_files(toc_files);
    BOXING_ASSERT(table_string == NULL);

    afs_toc_files_free(toc_files);
}
END_TEST


// Test function afs_toc_files_save_as_metadata_table with input afs_toc_files instance with long values
BOXING_START_TEST(afs_toc_files_save_as_metadata_table_test3)
{
    afs_toc_indices * tocs = get_filled_tocs_vector(3);
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    GVECTORN(tocs, afs_toc_file *, 1)->id = -2123456789;

    afs_toc_data_file_metadata * toc_data_file_metadata = GVECTORN(tocs, afs_toc_file *, 1)->metadata;
    afs_toc_data_file_metadata_source * metadata_source = GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 1);

    metadata_source->file_id = -2123456789;
    metadata_source->source_id = -2123456789;
    free(metadata_source->format_id);
    metadata_source->format_id = boxing_string_clone("long long long format id string................");
    free(metadata_source->data);
    metadata_source->data = boxing_string_clone("long long long data string...................");

    char * table_string = afs_toc_files_save_as_metadata_table(toc_files);

    const char * test_string =
        "\nUSER DATA FILES METADATA\n"
        "========================\n"
        "<fileId> <sourceFileId> <sourceId> <formatId> <data>\n"
        "-0000000002 00000000001 00000000002 afs/metadata-csv                                some xml data\n"
        "-0000000002 00000000002 00000000003 afs/metadata-csv                                some xml data\n"
        "-0000000002 00000000003 00000000004 afs/metadata-csv                                some xml data\n"
        "-0000000002 00000000004 00000000005 afs/metadata-csv                                some xml data\n"
        "-0000000002 00000000005 00000000006 afs/metadata-csv                                some xml data\n"
        "-2123456789 00000000001 00000000002 afs/metadata-csv                                some xml data\n"
        "-2123456789 -2123456789 -2123456789 long long long format id string................ long long long data string...................\n"
        "-2123456789 00000000003 00000000004 afs/metadata-csv                                some xml data\n"
        "-2123456789 00000000004 00000000005 afs/metadata-csv                                some xml data\n"
        "-2123456789 00000000005 00000000006 afs/metadata-csv                                some xml data\n"
        "-0000000002 00000000001 00000000002 afs/metadata-csv                                some xml data\n"
        "-0000000002 00000000002 00000000003 afs/metadata-csv                                some xml data\n"
        "-0000000002 00000000003 00000000004 afs/metadata-csv                                some xml data\n"
        "-0000000002 00000000004 00000000005 afs/metadata-csv                                some xml data\n"
        "-0000000002 00000000005 00000000006 afs/metadata-csv                                some xml data\n";

    BOXING_ASSERT_STR_EQ(table_string, test_string);
    BOXING_ASSERT(table_string != NULL);

    afs_toc_files_free(toc_files);
    free(table_string);
}
END_TEST


// Test function afs_toc_files_save_as_metadata_table with input afs_toc_files instance with short values
BOXING_START_TEST(afs_toc_files_save_as_metadata_table_test4)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector5(1);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance(1);

    afs_toc_file * toc_file = afs_toc_file_create2(
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

    afs_toc_indices * tocs = gvector_create_pointers(1);
    GVECTORN(tocs, afs_toc_file *, 0) = toc_file;
    afs_toc_files * toc_files = afs_toc_files_create2(tocs);

    GVECTORN(tocs, afs_toc_file *, 0)->id = 0;

    afs_toc_data_file_metadata_source * metadata_source = GVECTORN(toc_data_file_metadata->sources, afs_toc_data_file_metadata_source *, 0);

    metadata_source->file_id = 0;
    metadata_source->source_id = 0;
    free(metadata_source->format_id);
    metadata_source->format_id = boxing_string_clone("s");
    free(metadata_source->data);
    metadata_source->data = boxing_string_clone("h");

    char * table_string = afs_toc_files_save_as_metadata_table(toc_files);

    const char * test_string =
        "\nUSER DATA FILES METADATA\n"
        "========================\n"
        "<fileId> <sourceFileId> <sourceId> <formatId> <data>\n"
        "0 0 0 s h\n";

    BOXING_ASSERT_STR_EQ(table_string, test_string);
    BOXING_ASSERT(table_string != NULL);

    afs_toc_files_free(toc_files);
    free(table_string);
}
END_TEST


Suite * tocdatafiles_tests(void)
{
    TCase * tc_tocfiles_functions_tests = tcase_create("tocfiles_functions_tests");

    // Test function afs_toc_files_create
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_create_test1);
    // Test function afs_toc_files_create2
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_create2_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_create2_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_create2_test3);
    // Test function afs_toc_files_init
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_init_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_init_test2);
    // Test function afs_toc_files_init2
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_init2_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_init2_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_init2_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_init2_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_init2_test5);
    // Test function afs_toc_files_clone
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_clone_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_clone_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_clone_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_clone_test4);
    // Test function afs_toc_files_get_new_reference
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_new_reference_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_new_reference_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_new_reference_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_new_reference_test4);
    // Test function afs_toc_files_equal
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test6);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test7);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test8);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test9);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test10);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test11);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test12);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_equal_test13);
    // Test function afs_toc_files_add_toc
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test6);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test7);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test8);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test9);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test10);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test11);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test12);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test13);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test14);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_toc_test15);
    // Test function afs_toc_files_get_toc
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_toc_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_toc_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_toc_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_toc_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_toc_test5);
    // Test function afs_toc_files_get_tocs_count
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_tocs_count_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_tocs_count_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_tocs_count_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_tocs_count_test4);
    // Test function afs_toc_files_get_duration_frames
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test6);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test7);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test8);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test9);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test10);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test11);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test12);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test13);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test14);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test15);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_duration_frames_test16);
    // Test function afs_toc_files_get_first_frame
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test6);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test7);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test8);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test9);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test10);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test11);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test12);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test13);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test14);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test15);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_first_frame_test16);
    // Test function afs_toc_files_get_last_frame
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test6);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test7);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test8);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test9);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test10);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test11);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test12);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test13);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test14);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test15);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_get_last_frame_test16);
    // Test function afs_toc_files_add_frame_offset
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test6);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test7);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test8);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test9);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test10);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test11);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test12);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test13);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test14);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test15);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_add_frame_offset_test16);
    // Test function afs_toc_files_is_valid
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_is_valid_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_is_valid_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_is_valid_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_is_valid_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_is_valid_test5);
    // Test function afs_toc_files_save_file
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_file_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_file_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_file_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_file_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_file_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_file_test6);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_file_test7);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_file_test8);
    // Test function afs_toc_files_save_string
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_string_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_string_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_string_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_string_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_string_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_string_test6);
    // Test function afs_toc_files_save_xml
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_xml_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_xml_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_xml_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_xml_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_xml_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_xml_test6);
    // Test function afs_toc_files_load_file
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_file_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_file_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_file_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_file_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_file_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_file_test6);
    // Test function afs_toc_files_load_string
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_string_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_string_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_string_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_string_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_string_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_string_test6);
    // Test function afs_toc_files_load_xml
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_xml_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_xml_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_xml_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_xml_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_xml_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_load_xml_test6);
    // Test function afs_toc_files_save_as_table
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_table_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_table_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_table_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_table_test4);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_table_test5);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_table_test6);
    // Test function afs_toc_files_save_as_location_table
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_location_table_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_location_table_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_location_table_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_location_table_test4);
    // Test function afs_toc_files_save_as_metadata_table
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_metadata_table_test1);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_metadata_table_test2);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_metadata_table_test3);
    tcase_add_test(tc_tocfiles_functions_tests, afs_toc_files_save_as_metadata_table_test4);
    
    Suite * s = suite_create("tocdatafiles_test_util");
    suite_add_tcase(s, tc_tocfiles_functions_tests);

    return s;
}
