#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "tocfilepreview.h"
#include "boxing/platform/types.h"
#include "boxing/utils.h"
#include "boxing/string.h"
#include "mxml.h"

#include <string.h>

#if defined ( D_OS_WIN32 )
#define DFSEEK _fseeki64
#define DFTELL _ftelli64
#else
#define DFSEEK fseeko
#define DFTELL ftello
#endif

static void test_null_afs_toc_file_preview(afs_toc_file_preview * toc_file_preview)
{
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(toc_file_preview->pages == NULL);
    BOXING_ASSERT(toc_file_preview->dpi == -1);
}


static void test_empty_afs_toc_file_preview(afs_toc_file_preview * toc_file_preview)
{
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(toc_file_preview->pages != NULL);
    BOXING_ASSERT(toc_file_preview->pages->size == 0);
    BOXING_ASSERT(toc_file_preview->dpi == -1);
}


static void test_not_empty_afs_toc_file_preview_page2(afs_toc_file_preview_page * toc_file_preview_page, unsigned int page_number)
{
    BOXING_ASSERT(toc_file_preview_page != NULL);
    char current_id[10];
    sprintf(current_id, "id %u", page_number);
    BOXING_ASSERT(strcmp(toc_file_preview_page->layout_id, current_id) == 0);
    BOXING_ASSERT(toc_file_preview_page->start_frame == 9 + page_number);
    BOXING_ASSERT(toc_file_preview_page->start_section == 8 + page_number);
    BOXING_ASSERT(toc_file_preview_page->section_count == 7 + page_number);
    BOXING_ASSERT(toc_file_preview_page->dimension_x == 6 + page_number);
    BOXING_ASSERT(toc_file_preview_page->dimension_y == 5 + page_number);
    BOXING_ASSERT(toc_file_preview_page->overlap_x == 4 + page_number);
    BOXING_ASSERT(toc_file_preview_page->overlap_y == 3 + page_number);
}


static void test_not_empty_afs_toc_file_preview(afs_toc_file_preview * toc_file_preview, unsigned int pages_count)
{
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(toc_file_preview->pages != NULL);
    BOXING_ASSERT(toc_file_preview->pages->size == pages_count);
    BOXING_ASSERT(toc_file_preview->dpi == -1);

    for (unsigned int i = 0; i < pages_count; i++)
    {
        afs_toc_file_preview_page * toc_file_preview_page = GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, i);
        test_not_empty_afs_toc_file_preview_page2(toc_file_preview_page, i);
    }
}


static afs_toc_file_preview_pages * get_filled_pages_vector(unsigned int pages_count)
{
    afs_toc_file_preview_pages * pages = gvector_create_pointers(pages_count);

    for (unsigned int i = 0; i < pages_count; i++)
    {
        char current_id[10];
        sprintf(current_id, "id %u", i);

        GVECTORN(pages, afs_toc_file_preview_page *, i) = afs_toc_file_preview_page_create2(current_id, 9 + i, 8 + i, 7 + i, 6 + i, 5 + i, 4 + i, 3 + i);
    }

    return pages;
}

static afs_toc_file_preview_pages * get_realistic_pages_vector( unsigned int pages_count, unsigned int frame_sections, unsigned int page_sections )
{
    afs_toc_file_preview_pages * pages = gvector_create_pointers( pages_count );

    unsigned int section_index = 0;

    for ( unsigned int i = 0; i < pages_count; i++ )
    {
        char current_id[10];
        sprintf( current_id, "id %u", i );

        unsigned int current_frame = 10 + section_index / frame_sections;
        unsigned int current_section = section_index % frame_sections;

        GVECTORN( pages, afs_toc_file_preview_page *, i )
            = afs_toc_file_preview_page_create2(
                current_id,
                current_frame,
                current_section,
                page_sections,
                100, 100, 0, 0 );

        section_index += page_sections;
    }

    return pages;
}

static afs_toc_file_preview * get_preview_with_null_layout_id(unsigned int pages_count)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(pages_count);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    free(GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, 1)->layout_id);
    GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, 1)->layout_id = NULL;

    return toc_file_preview;
}


afs_toc_preview_sections * get_afs_toc_preview_sections_instance6(unsigned int sections_count)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    for (unsigned int i = 0; i < sections_count; i++)
    {
        afs_toc_preview_sections_append(toc_preview_sections, afs_toc_preview_section_create2(0 + i, 100 * i, 100 * i, 64 + i, 48 + i, (90 * i) % 360));
    }

    return toc_preview_sections;
}


afs_toc_preview_layout_definition * get_afs_toc_preview_layout_definition_instance6(const char * id, const char * name, unsigned int sections_count)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create2(id, name);
    toc_preview_layout_definition->sections = get_afs_toc_preview_sections_instance6(sections_count);

    return toc_preview_layout_definition;
}


afs_toc_preview_layout_definitions * get_afs_toc_preview_layout_definitions_instance6(unsigned int definitions_count)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    for (unsigned int i = 0; i < definitions_count; i++)
    {
        char current_id[14];
        char current_name[16];

        sprintf(current_id, "id %u", i);
        sprintf(current_name, "name %u", i);

        afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance6(current_id, current_name, 5);
        afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);
    }

    return toc_preview_layout_definitions;
}


static char * read_xml_preview_file(const char* file_name)
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


static mxml_node_t * get_preview_xml_instance(const char * file_name)
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

// Test function afs_toc_file_preview_create test
BOXING_START_TEST(afs_toc_file_preview_create_test1)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    test_null_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_create2 test1 with pages pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_create2_test1)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(NULL);

    test_null_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_create2 test2 with empty pages vector
BOXING_START_TEST(afs_toc_file_preview_create2_test2)
{
    afs_toc_file_preview_pages * pages = gvector_create_pointers(0);

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    test_empty_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_create2 test3 with filled pages vector
BOXING_START_TEST(afs_toc_file_preview_create2_test3)
{
    afs_toc_file_preview_pages * pages = get_filled_pages_vector(1);

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 1);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_init test with afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_init_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;

    afs_toc_file_preview_init(toc_file_preview);

    BOXING_ASSERT(toc_file_preview == NULL);
}
END_TEST


// Test function afs_toc_file_preview_init test with allocated afs_toc_file_preview pointer
BOXING_START_TEST(afs_toc_file_preview_init_test2)
{
    afs_toc_file_preview * toc_file_preview = malloc(sizeof(afs_toc_file_preview));

    afs_toc_file_preview_init(toc_file_preview);

    test_null_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_init2 test with afs_toc_file_preview pointer equal to NULL and vector pointer equa to NULL
BOXING_START_TEST(afs_toc_file_preview_init2_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    afs_toc_file_preview_pages* pages = NULL;

    afs_toc_file_preview_init2(toc_file_preview, pages);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(pages == NULL);
}
END_TEST


// Test function afs_toc_file_preview_init2 test with afs_toc_file_preview pointer equal to NULL and vector pointer not equa to NULL
BOXING_START_TEST(afs_toc_file_preview_init2_test2)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);

    afs_toc_file_preview_init2(toc_file_preview, pages);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(pages != NULL);
    BOXING_ASSERT(pages->size == 0);

    gvector_free(pages);
}
END_TEST


// Test function afs_toc_file_preview_init2 test with afs_toc_file_preview pointer not equal to NULL and vector pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_init2_test3)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_file_preview_pages * pages = NULL;

    afs_toc_file_preview_init2(toc_file_preview, pages);

    test_null_afs_toc_file_preview(toc_file_preview);

    BOXING_ASSERT(pages == NULL);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_init2 test with afs_toc_file_preview pointer not equal to NULL and empty vector
BOXING_START_TEST(afs_toc_file_preview_init2_test4)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);

    test_null_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_init2(toc_file_preview, pages);

    test_empty_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_init2 test with afs_toc_file_preview pointer not equal to NULL and filled vector
BOXING_START_TEST(afs_toc_file_preview_init2_test5)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);

    test_null_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_init2(toc_file_preview, pages);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_clone test with input pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_clone_test1)
{
    afs_toc_file_preview * toc_file_preview1 = NULL;

    BOXING_ASSERT(toc_file_preview1 == NULL);

    afs_toc_file_preview* toc_file_preview2 = afs_toc_file_preview_clone(toc_file_preview1);

    BOXING_ASSERT(toc_file_preview2 == NULL);
}
END_TEST


// Test function afs_toc_file_preview_clone test with empty input afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_clone_test2)
{
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create();

    test_null_afs_toc_file_preview(toc_file_preview1);

    afs_toc_file_preview* toc_file_preview2 = afs_toc_file_preview_clone(toc_file_preview1);

    test_null_afs_toc_file_preview(toc_file_preview2);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_clone test with not empty input afs_toc_file_preview instance and empty vector
BOXING_START_TEST(afs_toc_file_preview_clone_test3)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages);

    test_empty_afs_toc_file_preview(toc_file_preview1);

    afs_toc_file_preview* toc_file_preview2 = afs_toc_file_preview_clone(toc_file_preview1);

    test_empty_afs_toc_file_preview(toc_file_preview2);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_clone test with not empty input afs_toc_file_preview instance and not empty vctor
BOXING_START_TEST(afs_toc_file_preview_clone_test4)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages);

    test_not_empty_afs_toc_file_preview(toc_file_preview1, 3);

    afs_toc_file_preview* toc_file_preview2 = afs_toc_file_preview_clone(toc_file_preview1);

    test_not_empty_afs_toc_file_preview(toc_file_preview2, 3);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_new_reference_test1)
{
    afs_toc_file_preview * toc_file_preview1 = NULL;

    BOXING_ASSERT(toc_file_preview1 == NULL);

    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_get_new_reference(toc_file_preview1);

    BOXING_ASSERT(toc_file_preview2 == NULL);

    toc_file_preview1 = afs_toc_file_preview_create();

    test_null_afs_toc_file_preview(toc_file_preview1);
    BOXING_ASSERT(toc_file_preview1->reference_count == 1);
    BOXING_ASSERT(toc_file_preview2 == NULL);

    afs_toc_file_preview_free(toc_file_preview1);
}
END_TEST


// Test function afs_toc_file_preview_get_new_reference with empty input afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_get_new_reference_test2)
{
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create();

    test_null_afs_toc_file_preview(toc_file_preview1);
    BOXING_ASSERT(toc_file_preview1->reference_count == 1);

    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_get_new_reference(toc_file_preview1);

    test_null_afs_toc_file_preview(toc_file_preview2);
    BOXING_ASSERT(toc_file_preview1->reference_count == 2);
    BOXING_ASSERT(toc_file_preview2->reference_count == 2);
    BOXING_ASSERT(toc_file_preview1 == toc_file_preview2);

    toc_file_preview1->dpi = 99;

    BOXING_ASSERT(toc_file_preview2->dpi == 99);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_get_new_reference with not empty input afs_toc_file_preview instance and empty vector
BOXING_START_TEST(afs_toc_file_preview_get_new_reference_test3)
{
    afs_toc_file_preview_pages * pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages);

    test_empty_afs_toc_file_preview(toc_file_preview1);
    BOXING_ASSERT(toc_file_preview1->reference_count == 1);

    afs_toc_file_preview* toc_file_preview2 = afs_toc_file_preview_get_new_reference(toc_file_preview1);

    test_empty_afs_toc_file_preview(toc_file_preview2);
    BOXING_ASSERT(toc_file_preview1->reference_count == 2);
    BOXING_ASSERT(toc_file_preview2->reference_count == 2);
    BOXING_ASSERT(toc_file_preview1 == toc_file_preview2);

    toc_file_preview1->dpi = 99;

    BOXING_ASSERT(toc_file_preview2->dpi == 99);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_get_new_reference with not empty input afs_toc_file_preview instance and not empty vector
BOXING_START_TEST(afs_toc_file_preview_get_new_reference_test4)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages);

    test_not_empty_afs_toc_file_preview(toc_file_preview1, 3);
    BOXING_ASSERT(toc_file_preview1->reference_count == 1);

    afs_toc_file_preview* toc_file_preview2 = afs_toc_file_preview_get_new_reference(toc_file_preview1);

    test_not_empty_afs_toc_file_preview(toc_file_preview2, 3);
    BOXING_ASSERT(toc_file_preview1->reference_count == 2);
    BOXING_ASSERT(toc_file_preview2->reference_count == 2);
    BOXING_ASSERT(toc_file_preview1 == toc_file_preview2);

    toc_file_preview1->dpi = 99;

    BOXING_ASSERT(toc_file_preview2->dpi == 99);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_equal test with input afs_toc_file_preview pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_equal_test1)
{
    afs_toc_file_preview * toc_file_preview1 = NULL;
    afs_toc_file_preview * toc_file_preview2 = NULL;

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DTRUE);
}
END_TEST

// Test function afs_toc_file_preview_equal test with empty input afs_toc_file_preview instances 
BOXING_START_TEST(afs_toc_file_preview_equal_test2)
{
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create();
    test_null_afs_toc_file_preview(toc_file_preview1);

    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_create();
    test_null_afs_toc_file_preview(toc_file_preview2);

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_equal test with not empty input afs_toc_file_preview instances and empty vector
BOXING_START_TEST(afs_toc_file_preview_equal_test3)
{
    afs_toc_file_preview_pages* pages1 = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_empty_afs_toc_file_preview(toc_file_preview1);

    afs_toc_file_preview_pages* pages2 = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_create2(pages2);
    test_empty_afs_toc_file_preview(toc_file_preview2);

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_equal test with not empty input afs_toc_file_preview instances and vector with one pointer
BOXING_START_TEST(afs_toc_file_preview_equal_test4)
{
    afs_toc_file_preview_pages* pages1 = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_not_empty_afs_toc_file_preview(toc_file_preview1, 1);

    afs_toc_file_preview_pages* pages2 = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_create2(pages2);
    test_not_empty_afs_toc_file_preview(toc_file_preview2, 1);

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_equal test with not empty input afs_toc_file_preview instances and vector with three pointers
BOXING_START_TEST(afs_toc_file_preview_equal_test5)
{
    afs_toc_file_preview_pages* pages1 = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_not_empty_afs_toc_file_preview(toc_file_preview1, 3);

    afs_toc_file_preview_pages* pages2 = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_create2(pages2);
    test_not_empty_afs_toc_file_preview(toc_file_preview2, 3);

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_equal test with empty input afs_toc_file_preview instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_file_preview_equal_test6)
{
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create();
    test_null_afs_toc_file_preview(toc_file_preview1);

    afs_toc_file_preview * toc_file_preview2 = NULL;

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview1);
}
END_TEST


// Test function afs_toc_file_preview_equal test with input afs_toc_file_preview equal to NULL and instance with empty vector 
BOXING_START_TEST(afs_toc_file_preview_equal_test7)
{
    afs_toc_file_preview_pages* pages1 = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_empty_afs_toc_file_preview(toc_file_preview1);

    afs_toc_file_preview * toc_file_preview2 = NULL;

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview1);
}
END_TEST


// Test function afs_toc_file_preview_equal test with input afs_toc_file_preview pointer equal to NULL and instance with filled vector 
BOXING_START_TEST(afs_toc_file_preview_equal_test8)
{
    afs_toc_file_preview_pages* pages1 = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_not_empty_afs_toc_file_preview(toc_file_preview1, 3);

    afs_toc_file_preview * toc_file_preview2 = NULL;

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview1);
}
END_TEST


// Test function afs_toc_file_preview_equal test with empty input afs_toc_file_preview instance and instance with empty vector 
BOXING_START_TEST(afs_toc_file_preview_equal_test9)
{
    afs_toc_file_preview_pages* pages1 = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_empty_afs_toc_file_preview(toc_file_preview1);

    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_create();
    test_null_afs_toc_file_preview(toc_file_preview2);

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_equal test with empty input afs_toc_file_preview instance and instance with filled vector 
BOXING_START_TEST(afs_toc_file_preview_equal_test10)
{
    afs_toc_file_preview_pages* pages1 = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_not_empty_afs_toc_file_preview(toc_file_preview1, 3);

    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_create();
    test_null_afs_toc_file_preview(toc_file_preview2);

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_equal test with input afs_toc_file_preview instance with empty vector and instance with filled vector 
BOXING_START_TEST(afs_toc_file_preview_equal_test11)
{
    afs_toc_file_preview_pages* pages1 = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_not_empty_afs_toc_file_preview(toc_file_preview1, 3);

    afs_toc_file_preview_pages* pages2 = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_create2(pages2);
    test_empty_afs_toc_file_preview(toc_file_preview2);

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_equal test with input afs_toc_file_preview instances with filled vector with different count pointers
BOXING_START_TEST(afs_toc_file_preview_equal_test12)
{
    afs_toc_file_preview_pages* pages1 = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_not_empty_afs_toc_file_preview(toc_file_preview1, 1);

    afs_toc_file_preview_pages* pages2 = get_filled_pages_vector(2);
    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_create2(pages2);
    test_not_empty_afs_toc_file_preview(toc_file_preview2, 2);

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_equal test to afs_toc_file_preview instances with different data
BOXING_START_TEST(afs_toc_file_preview_equal_test13)
{
    afs_toc_file_preview_pages* pages1 = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_not_empty_afs_toc_file_preview(toc_file_preview1, 1);

    afs_toc_file_preview_pages* pages2 = gvector_create_pointers(1);

    GVECTORN(pages2, afs_toc_file_preview_page *, 0) = afs_toc_file_preview_page_create2("Some layout ID2", 10, 9, 8, 7, 6, 5, 4);
    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_create2(pages2);

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_equal test to afs_toc_file_preview instances with different dpi
BOXING_START_TEST(afs_toc_file_preview_equal_test14)
{
    afs_toc_file_preview_pages* pages1 = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview1 = afs_toc_file_preview_create2(pages1);
    test_not_empty_afs_toc_file_preview(toc_file_preview1, 1);
    toc_file_preview1->dpi = 720;

    afs_toc_file_preview_pages* pages2 = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview2 = afs_toc_file_preview_create2(pages2);
    test_not_empty_afs_toc_file_preview(toc_file_preview2, 1);
    toc_file_preview2->dpi = 1080;

    DBOOL result = afs_toc_file_preview_equal(toc_file_preview1, toc_file_preview2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_equal(toc_file_preview2, toc_file_preview1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview1);
    afs_toc_file_preview_free(toc_file_preview2);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview pointers equal to NULL and afs_toc_file_preview_page pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_add_page_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview pointers equal to NULL and valid afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test2)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview pointers equal to NULL and not valid afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test3)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 3, 4, 5, 6, 7, 8, 9);

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with empty input afs_toc_file_preview instance and afs_toc_file_preview_page pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_add_page_test4)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with empty input afs_toc_file_preview instance and empty afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test5)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with empty input afs_toc_file_preview instance and not valid afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test6)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 3, 4, 5, 6, 7, 8, 9);

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with empty input afs_toc_file_preview instance and valid afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test7)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("id 0", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 1);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview instance with empty vector and afs_toc_file_preview_page pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_add_page_test8)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview instance with empty vector and empty afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test9)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview instance with empty vector and not valid afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test10)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 3, 4, 5, 6, 7, 8, 9);

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview instance with empty vector and valid afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test11)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("id 0", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 1);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview instance with filled vector and afs_toc_file_preview_page pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_add_page_test12)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 1);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview instance with filled vector and empty afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test13)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 1);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview instance with filled vector and not valid afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test14)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 3, 4, 5, 6, 7, 8, 9);

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 1);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_add_page test with input afs_toc_file_preview instance with filled vector and valid afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_add_page_test15)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("id 1", 10, 9, 8, 7, 6, 5, 4);

    DBOOL result = afs_toc_file_preview_add_page(toc_file_preview, toc_file_preview_page);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 2);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_page test with input afs_toc_file_preview pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_page_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file_preview, 0);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
}
END_TEST


// Test function afs_toc_file_preview_get_page test with empty afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_get_page_test2)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file_preview, 0);

    test_null_afs_toc_file_preview(toc_file_preview);
    BOXING_ASSERT(toc_file_preview_page == NULL);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_page test with afs_toc_file_preview instance with empty vector
BOXING_START_TEST(afs_toc_file_preview_get_page_test3)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file_preview, 0);

    test_empty_afs_toc_file_preview(toc_file_preview);
    BOXING_ASSERT(toc_file_preview_page == NULL);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_page test with not empty afs_toc_file_preview instance and index out of range
BOXING_START_TEST(afs_toc_file_preview_get_page_test4)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(1);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file_preview, 1);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 1);
    BOXING_ASSERT(toc_file_preview_page == NULL);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_page test with not empty afs_toc_file_preview instance and index in range
BOXING_START_TEST(afs_toc_file_preview_get_page_test5)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    for (unsigned int i = 0; i < toc_file_preview->pages->size; i++)
    {
        test_not_empty_afs_toc_file_preview_page2(afs_toc_file_preview_get_page(toc_file_preview, i), i);
    }

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_page_count test with input afs_toc_file_preview pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_page_count_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    unsigned int pages_count = afs_toc_file_preview_get_page_count(toc_file_preview);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(pages_count == 0);
}
END_TEST


// Test function afs_toc_file_preview_get_page_count test with empty afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_get_page_count_test2)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    unsigned int pages_count = afs_toc_file_preview_get_page_count(toc_file_preview);

    test_null_afs_toc_file_preview(toc_file_preview);
    BOXING_ASSERT(pages_count == 0);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_page_count test with afs_toc_file_preview instance with empty vector
BOXING_START_TEST(afs_toc_file_preview_get_page_count_test3)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    unsigned int pages_count = afs_toc_file_preview_get_page_count(toc_file_preview);

    test_empty_afs_toc_file_preview(toc_file_preview);
    BOXING_ASSERT(pages_count == 0);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_page_count test with afs_toc_file_preview instance with filled vector
BOXING_START_TEST(afs_toc_file_preview_get_page_count_test4)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    unsigned int pages_count = afs_toc_file_preview_get_page_count(toc_file_preview);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);
    BOXING_ASSERT(pages_count == 3);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_start_frame test with input afs_toc_file_preview pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_start_frame_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;

    int start_frame = afs_toc_file_preview_get_start_frame(toc_file_preview);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(start_frame == -1);
}
END_TEST


// Test function afs_toc_file_preview_get_start_frame test with empty input afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_get_start_frame_test2)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    int start_frame = afs_toc_file_preview_get_start_frame(toc_file_preview);

    test_null_afs_toc_file_preview(toc_file_preview);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_start_frame test with input afs_toc_file_preview instance with empty vector
BOXING_START_TEST(afs_toc_file_preview_get_start_frame_test3)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    int start_frame = afs_toc_file_preview_get_start_frame(toc_file_preview);

    test_empty_afs_toc_file_preview(toc_file_preview);
    BOXING_ASSERT(start_frame == -1);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_start_frame test with input afs_toc_file_preview instance with filled vector
BOXING_START_TEST(afs_toc_file_preview_get_start_frame_test4)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    int start_frame = afs_toc_file_preview_get_start_frame(toc_file_preview);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);
    BOXING_ASSERT(start_frame == 9);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_add_frame_offset test with input afs_toc_file_preview pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_add_frame_offset_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    unsigned int offset = 5;

    afs_toc_file_preview_add_frame_offset(toc_file_preview, offset);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_file_preview_add_frame_offset test with empty input afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_add_frame_offset_test2)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    unsigned int offset = 5;

    afs_toc_file_preview_add_frame_offset(toc_file_preview, offset);

    test_null_afs_toc_file_preview(toc_file_preview);
    BOXING_ASSERT(offset == 5);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_add_frame_offset test with input afs_toc_file_preview instance with empty vector
BOXING_START_TEST(afs_toc_file_preview_add_frame_offset_test3)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    unsigned int offset = 5;

    afs_toc_file_preview_add_frame_offset(toc_file_preview, offset);

    test_empty_afs_toc_file_preview(toc_file_preview);
    BOXING_ASSERT(offset == 5);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_add_frame_offset test with input afs_toc_file_preview instance with filled vector
BOXING_START_TEST(afs_toc_file_preview_add_frame_offset_test4)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);

    unsigned int offset = 5;

    afs_toc_file_preview_add_frame_offset(toc_file_preview, offset);

    BOXING_ASSERT(offset == 5);
    for (unsigned int i = 0; i < toc_file_preview->pages->size; i++)
    {
        BOXING_ASSERT(GVECTORN(pages, afs_toc_file_preview_page *, i)->start_frame == 9 + i + offset);
    }

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_is_valid with input afs_toc_file_preview pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_is_valid_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    
    DBOOL result = afs_toc_file_preview_is_valid(toc_file_preview);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_is_valid with empty input afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_is_valid_test2)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_is_valid(toc_file_preview);

    test_null_afs_toc_file_preview(toc_file_preview);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_is_valid with input afs_toc_file_preview instance with empty vector
BOXING_START_TEST(afs_toc_file_preview_is_valid_test3)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    DBOOL result = afs_toc_file_preview_is_valid(toc_file_preview);

    test_empty_afs_toc_file_preview(toc_file_preview);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_is_valid with input afs_toc_file_preview instance where vector filled not valid data
BOXING_START_TEST(afs_toc_file_preview_is_valid_test4)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);

    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_x = 30;
    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_y = 50;

    DBOOL result = afs_toc_file_preview_is_valid(toc_file_preview);
    
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_is_valid with input afs_toc_file_preview instance where vector filled valid data
BOXING_START_TEST(afs_toc_file_preview_is_valid_test5)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    DBOOL result = afs_toc_file_preview_is_valid(toc_file_preview);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_file with file_name pointer equal to NULL and afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_file_preview * toc_file_preview = NULL;

    DBOOL result = afs_toc_file_preview_save_file(toc_file_preview, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_save_file with file_name pointer equal to NULL and afs_toc_file_preview pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    DBOOL result = afs_toc_file_preview_save_file(toc_file_preview, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_file with file_name pointer not equal to NULL and afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_save_file_test3)
{
    const char * file_name = "afs_toc_file_preview_save_file_test.xml";
    afs_toc_file_preview * toc_file_preview = NULL;

    DBOOL result = afs_toc_file_preview_save_file(toc_file_preview, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_save_file with empty afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_save_file_test4)
{
    const char * file_name = "afs_toc_file_preview_save_file_empty_test.xml";
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_save_file(toc_file_preview, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_file with afs_toc_file_preview instance with empty vector
BOXING_START_TEST(afs_toc_file_preview_save_file_test5)
{
    const char * file_name = "afs_toc_file_preview_save_file_empty_test.xml";
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    DBOOL result = afs_toc_file_preview_save_file(toc_file_preview, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_file with afs_toc_file_preview instance not valid vector
BOXING_START_TEST(afs_toc_file_preview_save_file_test6)
{
    const char * file_name = "afs_toc_file_preview_save_file_not_valid_test.xml";
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_x = 30;
    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_y = 50;

    DBOOL result = afs_toc_file_preview_save_file(toc_file_preview, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_file with afs_toc_file_preview instance with filled vector, in compact format
BOXING_START_TEST(afs_toc_file_preview_save_file_test7)
{
    const char * file_name = "afs_toc_file_preview_save_file_compact_test.xml";

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    DBOOL result = afs_toc_file_preview_save_file(toc_file_preview, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_file with afs_toc_file_preview instance with filled vector, in not compact format
BOXING_START_TEST(afs_toc_file_preview_save_file_test8)
{
    const char * file_name = "afs_toc_file_preview_save_file_not_compact_test.xml";

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    DBOOL result = afs_toc_file_preview_save_file(toc_file_preview, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_file with afs_toc_file_preview instance with filled vector and dpi greater than zero, in compact format
BOXING_START_TEST(afs_toc_file_preview_save_file_test9)
{
    const char * file_name = "afs_toc_file_preview_save_file_compact_dpi_test.xml";

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    toc_file_preview->dpi = 1080;

    DBOOL result = afs_toc_file_preview_save_file(toc_file_preview, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_file with afs_toc_file_preview instance with filled vector and dpi greater than zero, in not compact format
BOXING_START_TEST(afs_toc_file_preview_save_file_test10)
{
    const char * file_name = "afs_toc_file_preview_save_file_not_compact_dpi_test.xml";

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    toc_file_preview->dpi = 1080;

    DBOOL result = afs_toc_file_preview_save_file(toc_file_preview, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_page_string with afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_save_string_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;

    char * result = afs_toc_file_preview_save_string(toc_file_preview, DTRUE);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_file_preview_save_string with empty afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_save_string_test2)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    char * result = afs_toc_file_preview_save_string(toc_file_preview, DTRUE);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_string with afs_toc_file_preview instance with empty vector
BOXING_START_TEST(afs_toc_file_preview_save_string_test3)
{
    afs_toc_file_preview_pages* pages = gvector_create_pointers(0);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    char * result = afs_toc_file_preview_save_string(toc_file_preview, DTRUE);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_string with not valid afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_save_string_test4)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_x = 30;
    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_y = 50;

    char * result = afs_toc_file_preview_save_string(toc_file_preview, DTRUE);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_string with valid afs_toc_file_preview instance, in compact format
BOXING_START_TEST(afs_toc_file_preview_save_string_test5)
{
    char * test_string = read_xml_preview_file("afs_toc_file_preview_save_file_compact_test.xml");

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    char * result = afs_toc_file_preview_save_string(toc_file_preview, DTRUE);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_file_preview_free(toc_file_preview);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_file_preview_save_string with valid afs_toc_file_preview instance, in not compact format
BOXING_START_TEST(afs_toc_file_preview_save_string_test6)
{
    char * test_string = read_xml_preview_file("afs_toc_file_preview_save_file_not_compact_test.xml");

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    char * result = afs_toc_file_preview_save_string(toc_file_preview, DFALSE);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_file_preview_free(toc_file_preview);
    free(test_string);
    free(result);
}
END_TEST

// Test function afs_toc_file_preview_save_string with valid afs_toc_file_preview instance and dpi greater than zero, in not compact format
BOXING_START_TEST(afs_toc_file_preview_save_string_test7)
{
    char * test_string = read_xml_preview_file("afs_toc_file_preview_save_file_not_compact_dpi_test.xml");

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    toc_file_preview->dpi = 1080;

    char * result = afs_toc_file_preview_save_string(toc_file_preview, DFALSE);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_file_preview_free(toc_file_preview);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_file_preview_save_string with valid afs_toc_file_preview instance and dpi greater than zero, in compact format
BOXING_START_TEST(afs_toc_file_preview_save_string_test8)
{
    char * test_string = read_xml_preview_file("afs_toc_file_preview_save_file_compact_dpi_test.xml");

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    toc_file_preview->dpi = 1080;

    char * result = afs_toc_file_preview_save_string(toc_file_preview, DTRUE);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_file_preview_free(toc_file_preview);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_file_preview_save_xml with mxml_node_t pointer equal to NULL and afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_file_preview * toc_file_preview = NULL;

    DBOOL result = afs_toc_file_preview_save_xml(toc_file_preview, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_save_xml with mxml_node_t pointer equal to NULL and afs_toc_file_preview pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    DBOOL result = afs_toc_file_preview_save_xml(toc_file_preview, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_file_preview * toc_file_preview = NULL;

    DBOOL result = afs_toc_file_preview_save_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_save_xml with empty afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_save_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_file_preview* toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_save_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_save_xml with not valid afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_x = 30;
    GVECTORN(pages, afs_toc_file_preview_page *, 1)->overlap_y = 50;

    DBOOL result = afs_toc_file_preview_save_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_save_xml with valid afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_file_preview_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    toc_file_preview->dpi = 1080;

    DBOOL result = afs_toc_file_preview_save_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_load_file with file_name pointer equal to NULL and afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_file_preview * toc_file_preview = NULL;

    DBOOL result = afs_toc_file_preview_load_file(toc_file_preview, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_load_file with file_name pointer equal to NULL and afs_toc_file_preview pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_file(toc_file_preview, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_load_file with file_name pointer not equal to NULL and afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_load_file_test3)
{
    const char * file_name = "afs_toc_file_preview_save_file_compact_test.xml";
    afs_toc_file_preview * toc_file_preview = NULL;

    DBOOL result = afs_toc_file_preview_load_file(toc_file_preview, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_file_preview_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_file(toc_file_preview, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_load_file with initialized afs_toc_file_preview instance, in compact format
BOXING_START_TEST(afs_toc_file_preview_load_file_test5)
{
    const char * file_name = "afs_toc_file_preview_save_file_compact_test.xml";

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_file(toc_file_preview, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_load_file with initialized afs_toc_file_preview instance, in not compact format
BOXING_START_TEST(afs_toc_file_preview_load_file_test6)
{
    const char * file_name = "afs_toc_file_preview_save_file_not_compact_test.xml";
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_file(toc_file_preview, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_load_file with initialized afs_toc_file_preview instance and dpi greater than zero, in compact format
BOXING_START_TEST(afs_toc_file_preview_load_file_test7)
{
    const char * file_name = "afs_toc_file_preview_save_file_compact_dpi_test.xml";

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_file(toc_file_preview, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(toc_file_preview->dpi == 1080);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_load_file with initialized afs_toc_file_preview instance and dpi greater than zero, in not compact format
BOXING_START_TEST(afs_toc_file_preview_load_file_test8)
{
    const char * file_name = "afs_toc_file_preview_save_file_not_compact_dpi_test.xml";
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_file(toc_file_preview, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(toc_file_preview->dpi == 1080);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_load_string with input string pointer equal to NULL and afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_file_preview * toc_file_preview = NULL;

    DBOOL result = afs_toc_file_preview_load_string(toc_file_preview, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_load_string with input string pointer equal to NULL and afs_toc_file_preview pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_string(toc_file_preview, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_load_string with input string pointer not equal to NULL and afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_load_string_test3)
{
    char * input_string = read_xml_preview_file("afs_toc_file_preview_save_file_compact_test.xml");
    afs_toc_file_preview * toc_file_preview = NULL;

    DBOOL result = afs_toc_file_preview_load_string(toc_file_preview, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_file_preview_load_string with empty input string
BOXING_START_TEST(afs_toc_file_preview_load_string_test4)
{
    char * input_string = "";
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_string(toc_file_preview, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_load_string with input string for initialized afs_toc_file_preview instance, in compact format
BOXING_START_TEST(afs_toc_file_preview_load_string_test5)
{
    char * input_string = read_xml_preview_file("afs_toc_file_preview_save_file_compact_test.xml");

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_string(toc_file_preview, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);

    afs_toc_file_preview_free(toc_file_preview);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_preview_load_string with input string for initialized afs_toc_file_preview instance, in not compact format
BOXING_START_TEST(afs_toc_file_preview_load_string_test6)
{
    char * input_string = read_xml_preview_file("afs_toc_file_preview_save_file_not_compact_test.xml");

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_string(toc_file_preview, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);

    afs_toc_file_preview_free(toc_file_preview);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_preview_load_string with input string for initialized afs_toc_file_preview instance and dpi greater than zero, in compact format
BOXING_START_TEST(afs_toc_file_preview_load_string_test7)
{
    char * input_string = read_xml_preview_file("afs_toc_file_preview_save_file_compact_dpi_test.xml");

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_string(toc_file_preview, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(toc_file_preview->dpi == 1080);

    afs_toc_file_preview_free(toc_file_preview);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_preview_load_string with input string for initialized afs_toc_file_preview instance and dpi greater than zero, in not compact format
BOXING_START_TEST(afs_toc_file_preview_load_string_test8)
{
    char * input_string = read_xml_preview_file("afs_toc_file_preview_save_file_not_compact_dpi_test.xml");

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_string(toc_file_preview, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(toc_file_preview->dpi == 1080);

    afs_toc_file_preview_free(toc_file_preview);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_preview_load_xml with xml instance pointer equal to NULL and afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_file_preview * toc_file_preview = NULL;

    DBOOL result = afs_toc_file_preview_load_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_load_xml with xml instance pointer equal to NULL and afs_toc_file_preview pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_page_load_xml with xml instance pointer not equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_load_xml_test3)
{
    mxml_node_t *tree = get_preview_xml_instance("afs_toc_file_preview_save_file_compact_test.xml");
    afs_toc_file_preview * toc_file_preview = NULL;

    DBOOL result = afs_toc_file_preview_load_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_file_preview_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_file_preview(toc_file_preview);

    afs_toc_file_preview_free(toc_file_preview);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_load_xml with xml instance for initialized afs_toc_file_preview instance, in compact format
BOXING_START_TEST(afs_toc_file_preview_load_xml_test5)
{
    mxml_node_t *tree = get_preview_xml_instance("afs_toc_file_preview_save_file_compact_test.xml");
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);

    afs_toc_file_preview_free(toc_file_preview);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_load_xml with xml instance for initialized afs_toc_file_preview instance, in not compact format
BOXING_START_TEST(afs_toc_file_preview_load_xml_test6)
{
    mxml_node_t *tree = get_preview_xml_instance("afs_toc_file_preview_save_file_not_compact_test.xml");
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview(toc_file_preview, 3);

    afs_toc_file_preview_free(toc_file_preview);
    mxmlDelete(tree);
}
END_TEST

// Test function afs_toc_file_preview_load_xml with xml instance for initialized afs_toc_file_preview instance and dpi greater than zero, in compact format
BOXING_START_TEST(afs_toc_file_preview_load_xml_test7)
{
    mxml_node_t *tree = get_preview_xml_instance("afs_toc_file_preview_save_file_compact_dpi_test.xml");
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(toc_file_preview->dpi == 1080);

    afs_toc_file_preview_free(toc_file_preview);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_load_xml with xml instance for initialized afs_toc_file_preview instance dpi greater than zero, in not compact format
BOXING_START_TEST(afs_toc_file_preview_load_xml_test8)
{
    mxml_node_t *tree = get_preview_xml_instance("afs_toc_file_preview_save_file_not_compact_dpi_test.xml");
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    DBOOL result = afs_toc_file_preview_load_xml(toc_file_preview, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(toc_file_preview->dpi == 1080);

    afs_toc_file_preview_free(toc_file_preview);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_set_dpi with afs_toc_file_preview instance equal to NULL and dpi value is less than zero
BOXING_START_TEST(afs_toc_file_preview_set_dpi_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    int dpi = -100;

    afs_toc_file_preview_set_dpi(toc_file_preview, dpi);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(dpi == -100);
}
END_TEST


// Test function afs_toc_file_preview_set_dpi with afs_toc_file_preview instance not equal to NULL and dpi value is less than zero
BOXING_START_TEST(afs_toc_file_preview_set_dpi_test2)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    int dpi = -100;

    afs_toc_file_preview_set_dpi(toc_file_preview, dpi);

    BOXING_ASSERT(toc_file_preview->dpi == -100);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_set_dpi with afs_toc_file_preview instance equal to NULL and dpi value is greater than zero
BOXING_START_TEST(afs_toc_file_preview_set_dpi_test3)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    int dpi = 1080;

    afs_toc_file_preview_set_dpi(toc_file_preview, dpi);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(dpi == 1080);
}
END_TEST


// Test function afs_toc_file_preview_set_dpi with afs_toc_file_preview instance not equal to NULL and dpi value is greater than zero
BOXING_START_TEST(afs_toc_file_preview_set_dpi_test4)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();

    test_null_afs_toc_file_preview(toc_file_preview);

    int dpi = 0;
    afs_toc_file_preview_set_dpi(toc_file_preview, dpi);
    BOXING_ASSERT(toc_file_preview->dpi == 0);

    dpi = 720;
    afs_toc_file_preview_set_dpi(toc_file_preview, dpi);
    BOXING_ASSERT(toc_file_preview->dpi == 720);

    dpi = 1080;
    afs_toc_file_preview_set_dpi(toc_file_preview, dpi);
    BOXING_ASSERT(toc_file_preview->dpi == 1080);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with afs_toc_file_preview, definitions and frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test1)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with afs_toc_file_preview and frames count pointers equal to NULL and empty definitions instance
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test2)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with afs_toc_file_preview and definitions pointers equal to NULL and frames count pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test3)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with afs_toc_file_preview pointer equal to NULL, empty definitions instance and frames count pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test4)
{
    afs_toc_file_preview * toc_file_preview = NULL;
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview == NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with empty afs_toc_file_preview instance and definitions, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test5)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with empty afs_toc_file_preview and definitions instances, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test6)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with empty afs_toc_file_preview instance, frames count pointers not equal to NULL and definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test7)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with empty afs_toc_file_preview and definitions instances, frames count pointers not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test8)
{
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create();
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with afs_toc_file_preview instance with one of the layout id equal to NULL and definitions, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test9)
{
    afs_toc_file_preview * toc_file_preview = get_preview_with_null_layout_id(3);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with afs_toc_file_preview instance with one of the layout id equal to NULL, empty definitions instances, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test10)
{
    afs_toc_file_preview * toc_file_preview = get_preview_with_null_layout_id(3);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with afs_toc_file_preview instance with one of the layout id equal to NULL, frames count pointers not equal to NULL and definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test11)
{
    afs_toc_file_preview * toc_file_preview = get_preview_with_null_layout_id(3);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with afs_toc_file_preview instance with one of the layout id equal to NULL, empty definitions instances, frames count pointers not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test12)
{
    afs_toc_file_preview * toc_file_preview = get_preview_with_null_layout_id(3);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with normal afs_toc_file_preview instance and definitions, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test13)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with normal afs_toc_file_preview instance, empty definitions instances, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test14)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with normal afs_toc_file_preview instance, frames count pointers not equal to NULL and definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test15)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with normal afs_toc_file_preview instance, empty definitions instances, frames count pointers not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test16)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count when one of the required layout does not exist
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test17)
{
    afs_toc_file_preview_pages* pages = get_filled_pages_vector(3);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_preview_layout_definitions * definitions = get_afs_toc_preview_layout_definitions_instance6(2);
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_get_frames_count with different preview data
BOXING_START_TEST(afs_toc_file_preview_get_frames_count_test18)
{
    // TESTSET: 3 pages, 5 sections per frame, 8 sections per page
    afs_toc_file_preview_pages* pages = get_realistic_pages_vector( 3, 5, 8 );

    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
    afs_toc_preview_layout_definitions * definitions = get_afs_toc_preview_layout_definitions_instance6(4);
    unsigned int frames_count = 0;
    afs_toc_file_preview_page * toc_file_preview_page = GVECTORN(toc_file_preview->pages, afs_toc_file_preview_page *, 2);

    DBOOL result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 5);
    BOXING_ASSERT(result == DTRUE);

    toc_file_preview_page->section_count += 5;

    result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(frames_count == 6);
    BOXING_ASSERT(result == DTRUE);

    toc_file_preview_page->start_frame += 5;

    result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(frames_count == 11);
    BOXING_ASSERT(result == DTRUE);

    toc_file_preview_page->section_count = 0;

    result = afs_toc_file_preview_get_frames_count(toc_file_preview, &frames_count, definitions);

    BOXING_ASSERT(frames_count == 4);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_free(toc_file_preview);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST



Suite * tocfilepreview_tests(void)
{
    TCase * tc_tocfilepreview_functions_tests = tcase_create("tocfilepreview_functions_tests");

    // Test function afs_toc_file_preview_create
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_create_test1);
    // Test function afs_toc_file_preview_create2
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_create2_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_create2_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_create2_test3);
    // Test function afs_toc_file_preview_init
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_init_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_init_test2);
    // Test function afs_toc_file_preview_init2
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_init2_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_init2_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_init2_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_init2_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_init2_test5);
    // Test function afs_toc_file_preview_clone
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_clone_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_clone_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_clone_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_clone_test4);
    // Test function afs_toc_file_preview_get_new_reference
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_new_reference_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_new_reference_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_new_reference_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_new_reference_test4);
    // Test function afs_toc_file_preview_equal
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test5);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test6);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test7);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test8);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test9);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test10);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test11);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test12);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test13);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_equal_test14);
    // Test function afs_toc_file_preview_add_page
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test5);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test6);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test7);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test8);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test9);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test10);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test11);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test12);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test13);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test14);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_page_test15);
    // Test function afs_toc_file_preview_get_page
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_page_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_page_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_page_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_page_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_page_test5);
    // Test function afs_toc_file_preview_get_page_count
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_page_count_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_page_count_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_page_count_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_page_count_test4);
    // Test function afs_toc_file_preview_get_start_frame
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_start_frame_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_start_frame_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_start_frame_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_start_frame_test4);
    // Test function afs_toc_file_preview_add_frame_offset
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_frame_offset_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_frame_offset_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_frame_offset_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_add_frame_offset_test4);
    // Test function afs_toc_file_preview_is_valid
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_is_valid_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_is_valid_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_is_valid_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_is_valid_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_is_valid_test5);
    // Test function afs_toc_file_preview_save_file
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_file_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_file_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_file_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_file_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_file_test5);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_file_test6);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_file_test7);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_file_test8);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_file_test9);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_file_test10);
    // Test function afs_toc_file_preview_save_string
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_string_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_string_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_string_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_string_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_string_test5);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_string_test6);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_string_test7);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_string_test8);
    // Test function afs_toc_file_preview_save_xml
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_xml_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_xml_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_xml_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_xml_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_xml_test5);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_save_xml_test6);
    // Test function afs_toc_file_preview_load_file
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_file_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_file_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_file_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_file_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_file_test5);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_file_test6);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_file_test7);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_file_test8);
    // Test function afs_toc_file_preview_load_string
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_string_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_string_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_string_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_string_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_string_test5);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_string_test6);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_string_test7);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_string_test8);
    // Test function afs_toc_file_preview_xml_string
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_xml_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_xml_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_xml_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_xml_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_xml_test5);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_xml_test6);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_xml_test7);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_load_xml_test8);
    // Test function afs_toc_file_preview_set_dpi
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_set_dpi_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_set_dpi_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_set_dpi_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_set_dpi_test4);
    // Test function afs_toc_file_preview_get_frames_count
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test1);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test2);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test3);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test4);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test5);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test6);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test7);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test8);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test9);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test10);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test11);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test12);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test13);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test14);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test15);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test16);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test17);
    tcase_add_test(tc_tocfilepreview_functions_tests, afs_toc_file_preview_get_frames_count_test18);

    Suite * s = suite_create("tocfilepreview_test_util");
    suite_add_tcase(s, tc_tocfilepreview_functions_tests);

    return s;
}
