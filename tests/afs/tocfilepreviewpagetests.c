#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "tocfilepreviewpage.h"
#include "boxing/platform/types.h"
#include "boxing/utils.h"
#include "boxing/string.h"
#include "mxml.h"
#include "platform/io.h"

#include <string.h>

static void test_empty_afs_toc_file_preview_page(afs_toc_file_preview_page * toc_file_preview_page)
{
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(toc_file_preview_page->layout_id == NULL);
    BOXING_ASSERT(toc_file_preview_page->start_frame == 0);
    BOXING_ASSERT(toc_file_preview_page->start_section == 0);
    BOXING_ASSERT(toc_file_preview_page->section_count == 0);
    BOXING_ASSERT(toc_file_preview_page->dimension_x == 0);
    BOXING_ASSERT(toc_file_preview_page->dimension_y == 0);
    BOXING_ASSERT(toc_file_preview_page->overlap_x == 0);
    BOXING_ASSERT(toc_file_preview_page->overlap_y == 0);
}


static void test_not_empty_afs_toc_file_preview_page(afs_toc_file_preview_page * toc_file_preview_page)
{
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(strcmp(toc_file_preview_page->layout_id, "Some layout ID") == 0);
    BOXING_ASSERT(toc_file_preview_page->start_frame == 9);
    BOXING_ASSERT(toc_file_preview_page->start_section == 8);
    BOXING_ASSERT(toc_file_preview_page->section_count == 7);
    BOXING_ASSERT(toc_file_preview_page->dimension_x == 6);
    BOXING_ASSERT(toc_file_preview_page->dimension_y == 5);
    BOXING_ASSERT(toc_file_preview_page->overlap_x == 4);
    BOXING_ASSERT(toc_file_preview_page->overlap_y == 3);
}


static char * read_xml_preview_page_file(const char* file_name)
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


static mxml_node_t * get_preview_page_xml_instance(const char * file_name)
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


afs_toc_preview_sections * get_afs_toc_preview_sections_instance5(unsigned int sections_count)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    for (unsigned int i = 0; i < sections_count; i++)
    {
        afs_toc_preview_sections_append(toc_preview_sections, afs_toc_preview_section_create2(0 + i, 100 * i, 100 * i, 64 + i, 48 + i, (90 * i) % 360));
    }

    return toc_preview_sections;
}


afs_toc_preview_layout_definition * get_afs_toc_preview_layout_definition_instance5(const char * id, const char * name, unsigned int sections_count)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create2(id, name);
    toc_preview_layout_definition->sections = get_afs_toc_preview_sections_instance5(sections_count);

    return toc_preview_layout_definition;
}


afs_toc_preview_layout_definitions * get_afs_toc_preview_layout_definitions_instance5(unsigned int definitions_count)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    for (unsigned int i = 0; i < definitions_count; i++)
    {
        char current_id[14];
        char current_name[16];

        sprintf(current_id, "id %u", i);
        sprintf(current_name, "name %u", i);

        afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance5(current_id, current_name, 5);
        afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);
    }

    return toc_preview_layout_definitions;
}


// TEST TOC FILE PREVIEW PAGE FUNCTIONS
//

// Test function afs_toc_file_preview_page_create test
BOXING_START_TEST(afs_toc_file_preview_page_create_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_create2 test1 with empty initial values
BOXING_START_TEST(afs_toc_file_preview_page_create2_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2(NULL, 0, 0, 0, 0, 0, 0, 0);
    
    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_create2 test2 with normal initial values
BOXING_START_TEST(afs_toc_file_preview_page_create2_test2)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_init test with afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_init_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    afs_toc_file_preview_page_init(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page == NULL);
}
END_TEST


// Test function afs_toc_file_preview_page_init test with allocated afs_toc_file_preview_page pointer
BOXING_START_TEST(afs_toc_file_preview_page_init_test2)
{
    afs_toc_file_preview_page * toc_file_preview_page = malloc(sizeof(afs_toc_file_preview_page));

    afs_toc_file_preview_page_init(toc_file_preview_page);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_init2 test with afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_init2_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    afs_toc_file_preview_page_init2(toc_file_preview_page, "Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    BOXING_ASSERT(toc_file_preview_page == NULL);
}
END_TEST


// Test function afs_toc_file_preview_page_init2 test with normal initialization data
BOXING_START_TEST(afs_toc_file_preview_page_init2_test2)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_init2(toc_file_preview_page, "Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_init2 test with empty initialization data
BOXING_START_TEST(afs_toc_file_preview_page_init2_test3)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page);

    free(toc_file_preview_page->layout_id);
    afs_toc_file_preview_page_init2(toc_file_preview_page, NULL, 0, 0, 0, 0, 0, 0, 0);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_clone test with input pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_clone_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = NULL;

    BOXING_ASSERT(toc_file_preview_page1 == NULL);

    afs_toc_file_preview_page* toc_file_preview_page2 = afs_toc_file_preview_page_clone(toc_file_preview_page1);

    BOXING_ASSERT(toc_file_preview_page2 == NULL);
}
END_TEST


// Test function afs_toc_file_preview_page_clone test with empty input afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_clone_test2)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = afs_toc_file_preview_page_create();

    test_empty_afs_toc_file_preview_page(toc_file_preview_page1);

    afs_toc_file_preview_page* toc_file_preview_page2 = afs_toc_file_preview_page_clone(toc_file_preview_page1);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page2);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
    afs_toc_file_preview_page_free(toc_file_preview_page2);
}
END_TEST


// Test function afs_toc_file_preview_page_clone test with normal input afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_clone_test3)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page1);

    afs_toc_file_preview_page* toc_file_preview_page2 = afs_toc_file_preview_page_clone(toc_file_preview_page1);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page2);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
    afs_toc_file_preview_page_free(toc_file_preview_page2);
}
END_TEST


// Test function afs_toc_file_preview_page_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_new_reference_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = NULL;

    BOXING_ASSERT(toc_file_preview_page1 == NULL);

    afs_toc_file_preview_page * toc_file_preview_page2 = afs_toc_file_preview_page_get_new_reference(toc_file_preview_page1);

    BOXING_ASSERT(toc_file_preview_page2 == NULL);

    toc_file_preview_page1 = afs_toc_file_preview_page_create();

    BOXING_ASSERT(toc_file_preview_page2 == NULL);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
}
END_TEST


// Test function afs_toc_file_preview_page_get_new_reference with empty input afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_get_new_reference_test2)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = afs_toc_file_preview_page_create();

    test_empty_afs_toc_file_preview_page(toc_file_preview_page1);
    BOXING_ASSERT(toc_file_preview_page1->reference_count == 1);

    afs_toc_file_preview_page * toc_file_preview_page2 = afs_toc_file_preview_page_get_new_reference(toc_file_preview_page1);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page2);
    BOXING_ASSERT(toc_file_preview_page1->reference_count == 2);
    BOXING_ASSERT(toc_file_preview_page2->reference_count == 2);
    BOXING_ASSERT(toc_file_preview_page1 == toc_file_preview_page2);

    toc_file_preview_page1->section_count = 99;

    BOXING_ASSERT(toc_file_preview_page2->section_count == 99);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
    afs_toc_file_preview_page_free(toc_file_preview_page1);
}
END_TEST


// Test function afs_toc_file_preview_page_get_new_reference with normal input afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_get_new_reference_test3)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page1);
    BOXING_ASSERT(toc_file_preview_page1->reference_count == 1);

    afs_toc_file_preview_page * toc_file_preview_page2 = afs_toc_file_preview_page_get_new_reference(toc_file_preview_page1);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page2);
    BOXING_ASSERT(toc_file_preview_page1->reference_count == 2);
    BOXING_ASSERT(toc_file_preview_page2->reference_count == 2);
    BOXING_ASSERT(toc_file_preview_page1 == toc_file_preview_page2);

    toc_file_preview_page1->start_frame = 99;

    BOXING_ASSERT(toc_file_preview_page2->start_frame == 99);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
    afs_toc_file_preview_page_free(toc_file_preview_page1);
}
END_TEST


// Test function afs_toc_file_preview_page_equal test with input afs_toc_file_preview_page pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_equal_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = NULL;
    afs_toc_file_preview_page * toc_file_preview_page2 = NULL;

    DBOOL result = afs_toc_file_preview_page_equal(toc_file_preview_page1, toc_file_preview_page2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_page_equal(toc_file_preview_page2, toc_file_preview_page1);

    BOXING_ASSERT(result == DTRUE);
}
END_TEST

// Test function afs_toc_file_preview_page_equal test with empty input afs_toc_file_preview_page instances 
BOXING_START_TEST(afs_toc_file_preview_page_equal_test2)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = afs_toc_file_preview_page_create();
    test_empty_afs_toc_file_preview_page(toc_file_preview_page1);

    afs_toc_file_preview_page * toc_file_preview_page2 = afs_toc_file_preview_page_create();
    test_empty_afs_toc_file_preview_page(toc_file_preview_page2);

    DBOOL result = afs_toc_file_preview_page_equal(toc_file_preview_page1, toc_file_preview_page2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_page_equal(toc_file_preview_page2, toc_file_preview_page1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
    afs_toc_file_preview_page_free(toc_file_preview_page2);
}
END_TEST


// Test function afs_toc_file_preview_page_equal test with normal input afs_toc_file_preview_page pointers 
BOXING_START_TEST(afs_toc_file_preview_page_equal_test3)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);
    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page1);

    afs_toc_file_preview_page* toc_file_preview_page2 = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);
    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page2);

    DBOOL result = afs_toc_file_preview_page_equal(toc_file_preview_page1, toc_file_preview_page2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_page_equal(toc_file_preview_page2, toc_file_preview_page1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
    afs_toc_file_preview_page_free(toc_file_preview_page2);
}
END_TEST


// Test function afs_toc_file_preview_page_equal test with empty input afs_toc_file_preview_page pointer and pointer equal to NULL 
BOXING_START_TEST(afs_toc_file_preview_page_equal_test4)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = afs_toc_file_preview_page_create();
    test_empty_afs_toc_file_preview_page(toc_file_preview_page1);

    afs_toc_file_preview_page * toc_file_preview_page2 = NULL;

    DBOOL result = afs_toc_file_preview_page_equal(toc_file_preview_page1, toc_file_preview_page2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_page_equal(toc_file_preview_page2, toc_file_preview_page1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
}
END_TEST


// Test function afs_toc_file_preview_page_equal test with normal input afs_toc_file_preview_page pointer and pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_equal_test5)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);
    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page1);

    afs_toc_file_preview_page * toc_file_preview_page2 = NULL;

    DBOOL result = afs_toc_file_preview_page_equal(toc_file_preview_page1, toc_file_preview_page2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_page_equal(toc_file_preview_page2, toc_file_preview_page1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
}
END_TEST


// Test function afs_toc_file_preview_page_equal test with normal afs_toc_file_preview_page pointer and empty afs_toc_file_preview_page pointer
BOXING_START_TEST(afs_toc_file_preview_page_equal_test6)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);
    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page1);

    afs_toc_file_preview_page * toc_file_preview_page2 = afs_toc_file_preview_page_create();
    test_empty_afs_toc_file_preview_page(toc_file_preview_page2);

    DBOOL result = afs_toc_file_preview_page_equal(toc_file_preview_page1, toc_file_preview_page2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_page_equal(toc_file_preview_page2, toc_file_preview_page1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
    afs_toc_file_preview_page_free(toc_file_preview_page2);
}
END_TEST


// Test function afs_toc_file_preview_page_equal test to afs_toc_file_preview_page instances with different data
BOXING_START_TEST(afs_toc_file_preview_page_equal_test7)
{
    afs_toc_file_preview_page * toc_file_preview_page1 = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);
    
    afs_toc_file_preview_page* toc_file_preview_page2 = afs_toc_file_preview_page_create2("Some other layout ID", 9, 8, 7, 6, 5, 4, 3);
    
    DBOOL result = afs_toc_file_preview_page_equal(toc_file_preview_page1, toc_file_preview_page2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_file_preview_page_equal(toc_file_preview_page2, toc_file_preview_page1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page1);
    afs_toc_file_preview_page_free(toc_file_preview_page2);
}
END_TEST


// Test function afs_toc_file_preview_page_is_valid test with input afs_toc_file_preview_page pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_is_valid_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_is_valid(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_is_valid test with empty afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_is_valid_test2)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_is_valid(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_is_valid test with layout id equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_is_valid_test3)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2(NULL, 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_is_valid(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_is_valid test with empty layout_id value
BOXING_START_TEST(afs_toc_file_preview_page_is_valid_test4)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_is_valid(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_is_valid test with section countn equal to zero
BOXING_START_TEST(afs_toc_file_preview_page_is_valid_test5)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 0, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_is_valid(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_is_valid test with X-axis dimension equal to zero
BOXING_START_TEST(afs_toc_file_preview_page_is_valid_test6)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 0, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_is_valid(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_is_valid test with Y-axis dimension equal to zero
BOXING_START_TEST(afs_toc_file_preview_page_is_valid_test7)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 0, 4, 3);

    DBOOL result = afs_toc_file_preview_page_is_valid(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_is_valid test with overlap is larger than dimension on X-axis
BOXING_START_TEST(afs_toc_file_preview_page_is_valid_test8)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 7, 3);

    DBOOL result = afs_toc_file_preview_page_is_valid(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_is_valid test with overlap is larger than dimension on Y-axis
BOXING_START_TEST(afs_toc_file_preview_page_is_valid_test9)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 6);

    DBOOL result = afs_toc_file_preview_page_is_valid(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_is_valid test with valid instance
BOXING_START_TEST(afs_toc_file_preview_page_is_valid_test10)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_is_valid(toc_file_preview_page);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with afs_toc_file_preview_page, definitions and frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with afs_toc_file_preview_page and frames count pointers equal to NULL and empty definitions instance
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test2)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with afs_toc_file_preview_page and definitions pointers equal to NULL and frames count pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test3)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with afs_toc_file_preview_page pointer equal to NULL, empty definitions instance and frames count pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test4)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with empty afs_toc_file_preview_page instance and definitions, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test5)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with empty afs_toc_file_preview_page and definitions instances, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test6)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with empty afs_toc_file_preview_page instance, frames count pointers not equal to NULL and definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test7)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with empty afs_toc_file_preview_page and definitions instances, frames count pointers not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test8)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with afs_toc_file_preview_page instance with layout id equal to NULL and definitions, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test9)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2(NULL, 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with afs_toc_file_preview_page instance with layout id equal to NULL, empty definitions instances, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test10)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2(NULL, 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with afs_toc_file_preview_page instance with layout id equal to NULL, frames count pointers not equal to NULL and definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test11)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2(NULL, 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with afs_toc_file_preview_page instance with layout id equal to NULL, empty definitions instances, frames count pointers not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test12)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2(NULL, 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with normal afs_toc_file_preview_page instance and definitions, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test13)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("01", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with normal afs_toc_file_preview_page instance, empty definitions instances, frames count pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test14)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("01", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * frames_count = NULL;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with normal afs_toc_file_preview_page instance, frames count pointers not equal to NULL and definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test15)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("01", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with normal afs_toc_file_preview_page instance, empty definitions instances, frames count pointers not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test16)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("01", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count when required layout does not exist
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test17)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("01", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = get_afs_toc_preview_layout_definitions_instance5(2);
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_frames_count with different preview page data
BOXING_START_TEST(afs_toc_file_preview_page_get_frames_count_test18)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("id 1", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = get_afs_toc_preview_layout_definitions_instance5(2);
    unsigned int frames_count = 0;

    DBOOL result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(frames_count == 3);
    BOXING_ASSERT(result == DTRUE);

    toc_file_preview_page->section_count = 8;

    result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(frames_count == 4);
    BOXING_ASSERT(result == DTRUE);

    toc_file_preview_page->start_section = 0;

    result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(frames_count == 2);
    BOXING_ASSERT(result == DTRUE);

    toc_file_preview_page->section_count = 0;

    result = afs_toc_file_preview_page_get_frames_count(toc_file_preview_page, &frames_count, definitions);

    BOXING_ASSERT(frames_count == 0);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with afs_toc_file_preview_page, definitions and sections pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * sections = NULL;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(sections == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with afs_toc_file_preview_page and sections pointers equal to NULL and empty definitions instance
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test2)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * sections = NULL;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(sections == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with afs_toc_file_preview_page and definitions pointers equal to NULL and sections pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test3)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int sections = 0;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with afs_toc_file_preview_page pointer equal to NULL, empty definitions instance and sections pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test4)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int sections = 0;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with empty afs_toc_file_preview_page instance and definitions, sections pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test5)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * sections = NULL;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(sections == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with empty afs_toc_file_preview_page and definitions instances, sections pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test6)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * sections = NULL;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(sections == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with empty afs_toc_file_preview_page instance, sections pointers not equal to NULL and definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test7)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int sections = 0;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with empty afs_toc_file_preview_page and definitions instances, sections pointers not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test8)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int sections = 0;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with afs_toc_file_preview_page instance with layout id equal to NULL and definitions, sections pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test9)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2(NULL, 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * sections = NULL;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(sections == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with afs_toc_file_preview_page instance with layout id equal to NULL, empty definitions instances, sections pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test10)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2(NULL, 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * sections = NULL;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(sections == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with afs_toc_file_preview_page instance with layout id equal to NULL, sections pointers not equal to NULL and definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test11)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2(NULL, 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int sections = 0;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with afs_toc_file_preview_page instance with layout id equal to NULL, empty definitions instances, sections pointers not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test12)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2(NULL, 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int sections = 0;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with normal afs_toc_file_preview_page instance and definitions, sections pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test13)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("01", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int * sections = NULL;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(sections == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with normal afs_toc_file_preview_page instance, empty definitions instances, sections pointers equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test14)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("01", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int * sections = NULL;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(sections == NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with normal afs_toc_file_preview_page instance, sections pointers not equal to NULL and definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test15)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("01", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = NULL;
    unsigned int sections = 0;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions == NULL);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with normal afs_toc_file_preview_page instance, empty definitions instances, sections pointers not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test16)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("01", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = afs_toc_preview_layout_definitions_create();
    unsigned int sections = 0;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame when required layout does not exist
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test17)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("01", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = get_afs_toc_preview_layout_definitions_instance5(2);
    unsigned int sections = 0;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_get_sections_on_frame with different preview page data
BOXING_START_TEST(afs_toc_file_preview_page_get_sections_on_frame_test18)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("id 1", 9, 8, 7, 6, 5, 4, 3);
    afs_toc_preview_layout_definitions * definitions = get_afs_toc_preview_layout_definitions_instance5(2);
    unsigned int sections = 0;

    DBOOL result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(definitions != NULL);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 1, definitions);
    BOXING_ASSERT(sections == 2);
    BOXING_ASSERT(result == DTRUE);

    toc_file_preview_page->section_count = 8;
    result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 2, definitions);
    
    BOXING_ASSERT(sections == 5);
    BOXING_ASSERT(result == DTRUE);

    toc_file_preview_page->start_section = 0;

    result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(sections == 5);
    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 4, definitions);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DTRUE);

    toc_file_preview_page->section_count = 0;

    result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 0, definitions);

    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_file_preview_page_get_sections_on_frame(toc_file_preview_page, &sections, 3, definitions);
    BOXING_ASSERT(sections == 0);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    afs_toc_preview_layout_definitions_free(definitions);
}
END_TEST


// Test function afs_toc_file_preview_page_save_file with file_name pointer equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_save_file(toc_file_preview_page, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_save_file with file_name pointer equal to NULL and afs_toc_file_preview_page pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_save_file(toc_file_preview_page, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_save_file with file_name pointer not equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_save_file_test3)
{
    const char * file_name = "afs_toc_file_preview_page_save_file_test.xml";
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_save_file(toc_file_preview_page, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_save_file with empty toc_data_file instance
BOXING_START_TEST(afs_toc_file_preview_page_save_file_test4)
{
    const char * file_name = "afs_toc_file_preview_page_save_file_empty_test.xml";
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_save_file(toc_file_preview_page, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_save_file with initialized afs_toc_file_preview_page instance, in compact format
BOXING_START_TEST(afs_toc_file_preview_page_save_file_test5)
{
    const char * file_name = "afs_toc_file_preview_page_save_file_compact_test.xml";

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_save_file(toc_file_preview_page, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_save_file with initialized afs_toc_file_preview_page instance, in not compact format
BOXING_START_TEST(afs_toc_file_preview_page_save_file_test6)
{
    const char * file_name = "afs_toc_file_preview_page_save_file_not_compact_test.xml";

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_save_file(toc_file_preview_page, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_save_string with afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_save_string_test1)
{
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    char * result = afs_toc_file_preview_page_save_string(toc_file_preview_page, DTRUE);

    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_file_preview_page_save_string with empty afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_save_string_test2)
{
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    char * result = afs_toc_file_preview_page_save_string(toc_file_preview_page, DTRUE);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, "<?xml version=\"1.0\" encoding=\"utf-8\"?><pages frame=\"0\" section=\"0\" count=\"0\" />\n") == 0);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    free(result);
}
END_TEST


// Test function afs_toc_file_preview_page_save_string with initialized afs_toc_file_preview_page instance, in compact format
BOXING_START_TEST(afs_toc_file_preview_page_save_string_test3)
{
    char * test_string = read_xml_preview_page_file("afs_toc_file_preview_page_save_file_compact_test.xml");

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    char * result = afs_toc_file_preview_page_save_string(toc_file_preview_page, DTRUE);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_file_preview_page_save_string with initialized afs_toc_file_preview_page instance, in not compact format and use data_v1 type
BOXING_START_TEST(afs_toc_file_preview_page_save_string_test4)
{
    char * test_string = read_xml_preview_page_file("afs_toc_file_preview_page_save_file_not_compact_test.xml");

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    char * result = afs_toc_file_preview_page_save_string(toc_file_preview_page, DFALSE);

    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_file_preview_page_save_xml with mxml_node_t pointer equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_save_xml(toc_file_preview_page, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_save_xml with mxml_node_t pointer equal to NULL and afs_toc_file_preview_page pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_save_xml(toc_file_preview_page, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_save_xml(toc_file_preview_page, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_page_save_xml with empty afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_save_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_file_preview_page* toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_save_xml(toc_file_preview_page, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_page_save_xml with initialized afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_save_xml(toc_file_preview_page, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_page_load_file with file_name pointer equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_load_file(toc_file_preview_page, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_load_file with file_name pointer equal to NULL and afs_toc_file_preview_page pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create2("Some layout ID", 9, 8, 7, 6, 5, 4, 3);

    DBOOL result = afs_toc_file_preview_page_load_file(toc_file_preview_page, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_load_file with file_name pointer not equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_load_file_test3)
{
    const char * file_name = "afs_toc_file_preview_page_save_file_compact_test.xml";
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_load_file(toc_file_preview_page, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_file_preview_page_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_file(toc_file_preview_page, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_load_file with empty afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_load_file_test5)
{
    const char * file_name = "afs_toc_file_preview_page_save_file_empty_test.xml";
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_file(toc_file_preview_page, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_file_preview_page != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_load_file with initialized afs_toc_file_preview_page instance, in compact format
BOXING_START_TEST(afs_toc_file_preview_page_load_file_test6)
{
    const char * file_name = "afs_toc_file_preview_page_save_file_compact_test.xml";

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_file(toc_file_preview_page, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_load_file with initialized afs_toc_file_preview_page instance, in not compact format
BOXING_START_TEST(afs_toc_file_preview_page_load_file_test7)
{
    const char * file_name = "afs_toc_file_preview_page_save_file_not_compact_test.xml";
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_file(toc_file_preview_page, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_load_string with input string pointer equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_load_string(toc_file_preview_page, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_load_string with input string pointer equal to NULL and afs_toc_file_preview_page pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_string(toc_file_preview_page, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_load_string with input string pointer not equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_load_string_test3)
{
    char * input_string = read_xml_preview_page_file("afs_toc_file_preview_page_save_file_compact_test.xml");
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_load_string(toc_file_preview_page, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_file_preview_page_load_string with empty input string
BOXING_START_TEST(afs_toc_file_preview_page_load_string_test4)
{
    char * input_string = "";
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_string(toc_file_preview_page, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_load_string with input string for empty afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_load_string_test5)
{
    char * input_string = read_xml_preview_page_file("afs_toc_file_preview_page_save_file_empty_test.xml");

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_string(toc_file_preview_page, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_preview_page_load_string with input string for initialized afs_toc_file_preview_page instance, in compact format
BOXING_START_TEST(afs_toc_file_preview_page_load_string_test6)
{
    char * input_string = read_xml_preview_page_file("afs_toc_file_preview_page_save_file_compact_test.xml");

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_string(toc_file_preview_page, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_preview_page_load_string with input string for initialized afs_toc_file_preview_page instance, in not compact format
BOXING_START_TEST(afs_toc_file_preview_page_load_string_test7)
{
    char * input_string = read_xml_preview_page_file("afs_toc_file_preview_page_save_file_not_compact_test.xml");

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_string(toc_file_preview_page, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    free(input_string);
}
END_TEST


// Test function afs_toc_file_preview_page_load_xml with xml instance pointer equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_load_xml(toc_file_preview_page, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_file_preview_page_load_xml with xml instance pointer equal to NULL and afs_toc_file_preview_page pointer not equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_xml(toc_file_preview_page, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
}
END_TEST


// Test function afs_toc_file_preview_page_load_xml with xml instance pointer not equal to NULL and afs_toc_file_preview_page pointer equal to NULL
BOXING_START_TEST(afs_toc_file_preview_page_load_xml_test3)
{
    mxml_node_t *tree = get_preview_page_xml_instance("afs_toc_file_preview_page_save_file_compact_test.xml");
    afs_toc_file_preview_page * toc_file_preview_page = NULL;

    DBOOL result = afs_toc_file_preview_page_load_xml(toc_file_preview_page, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_file_preview_page == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_page_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_file_preview_page_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_xml(toc_file_preview_page, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_page_load_xml with xml instance for empty afs_toc_file_preview_page instance
BOXING_START_TEST(afs_toc_file_preview_page_load_xml_test5)
{
    mxml_node_t *tree = get_preview_page_xml_instance("afs_toc_file_preview_page_save_file_empty_test.xml");
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_xml(toc_file_preview_page, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_page_load_xml with xml instance for initialized afs_toc_file_preview_page instance, in compact format
BOXING_START_TEST(afs_toc_file_preview_page_load_xml_test6)
{
    mxml_node_t *tree = get_preview_page_xml_instance("afs_toc_file_preview_page_save_file_compact_test.xml");
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_xml(toc_file_preview_page, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_file_preview_page_load_xml with xml instance for initialized afs_toc_file_preview_page instance, in not compact format
BOXING_START_TEST(afs_toc_file_preview_page_load_xml_test7)
{
    mxml_node_t *tree = get_preview_page_xml_instance("afs_toc_file_preview_page_save_file_not_compact_test.xml");
    afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_page_create();

    DBOOL result = afs_toc_file_preview_page_load_xml(toc_file_preview_page, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_file_preview_page(toc_file_preview_page);

    afs_toc_file_preview_page_free(toc_file_preview_page);
    mxmlDelete(tree);
}
END_TEST



Suite * tocfilepreviewpage_tests(void)
{
    TCase * tc_tocfilepreviewpage_functions_tests = tcase_create("tocfilepreviewpage_functions_tests");

    // Test function afs_toc_file_preview_page_create
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_create_test1);
    // Test function afs_toc_file_preview_page_create2
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_create2_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_create2_test2);
    // Test function afs_toc_file_preview_page_init
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_init_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_init_test2);
    // Test function afs_toc_file_preview_page_init2
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_init2_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_init2_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_init2_test3);
    // Test function afs_toc_file_preview_page_clone
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_clone_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_clone_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_clone_test3);
    // Test function afs_toc_file_preview_page_get_new_reference
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_new_reference_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_new_reference_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_new_reference_test3);
    // Test function afs_toc_file_preview_page_equal
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_equal_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_equal_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_equal_test3);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_equal_test4);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_equal_test5);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_equal_test6);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_equal_test7);
    // Test function afs_toc_file_preview_page_is_valid
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_is_valid_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_is_valid_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_is_valid_test3);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_is_valid_test4);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_is_valid_test5);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_is_valid_test6);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_is_valid_test7);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_is_valid_test8);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_is_valid_test9);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_is_valid_test10);
    // Test function afs_toc_file_preview_page_get_frames_count
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test3);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test4);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test5);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test6);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test7);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test8);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test9);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test10);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test11);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test12);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test13);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test14);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test15);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test16);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test17);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_frames_count_test18);
    // Test function afs_toc_file_preview_page_get_sections_on_frame
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test3);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test4);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test5);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test6);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test7);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test8);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test9);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test10);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test11);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test12);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test13);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test14);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test15);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test16);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test17);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_get_sections_on_frame_test18);
    // Test function afs_toc_file_preview_page_save_file
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_file_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_file_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_file_test3);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_file_test4);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_file_test5);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_file_test6);
    // Test function afs_toc_file_preview_page_save_string
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_string_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_string_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_string_test3);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_string_test4);
    // Test function afs_toc_file_preview_page_save_xml
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_xml_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_xml_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_xml_test3);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_xml_test4);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_save_xml_test5);
    // Test function afs_toc_file_preview_page_load_file
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_file_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_file_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_file_test3);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_file_test4);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_file_test5);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_file_test6);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_file_test7);
    // Test function afs_toc_file_preview_page_load_string
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_string_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_string_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_string_test3);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_string_test4);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_string_test5);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_string_test6);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_string_test7);
    // Test function afs_toc_file_preview_page_load_xml
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_xml_test1);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_xml_test2);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_xml_test3);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_xml_test4);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_xml_test5);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_xml_test6);
    tcase_add_test(tc_tocfilepreviewpage_functions_tests, afs_toc_file_preview_page_load_xml_test7);


    Suite * s = suite_create("tocfilepreviewpage_test_util");
    suite_add_tcase(s, tc_tocfilepreviewpage_functions_tests);

    return s;
}
