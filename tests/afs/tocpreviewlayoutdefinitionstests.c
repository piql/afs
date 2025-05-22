#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "toc/previewlayoutdefinitions.h"
#include "boxing/platform/types.h"
#include "boxing/utils.h"
#include "boxing/string.h"
#include "mxml.h"

#if defined ( D_OS_WIN32 )
#define DFSEEK _fseeki64
#define DFTELL _ftelli64
#else
#define DFSEEK fseeko
#define DFTELL ftello
#endif


static char * read_xml_toc_preview_layout_definitions_file(const char* file_name)
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


static mxml_node_t * get_toc_preview_layout_definitions_xml_instance(const char * file_name)
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


static void test_empty_afs_toc_preview_layout_definitions(afs_toc_preview_layout_definitions * toc_preview_layout_definitions)
{
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions->layout_definitions == NULL);
}


static void test_not_empty_afs_toc_preview_section(
    afs_toc_preview_section * toc_preview_section,
    const unsigned int id,
    const unsigned int x,
    const unsigned int y,
    const unsigned int width,
    const unsigned int height,
    const unsigned int rotation)
{
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(toc_preview_section->id == id);
    BOXING_ASSERT(toc_preview_section->x == x);
    BOXING_ASSERT(toc_preview_section->y == y);
    BOXING_ASSERT(toc_preview_section->width == width);
    BOXING_ASSERT(toc_preview_section->height == height);
    BOXING_ASSERT(toc_preview_section->rotation == rotation);
}


static void test_not_empty_afs_toc_preview_sections(afs_toc_preview_sections * toc_preview_sections, unsigned int sections_count)
{
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(toc_preview_sections->sections != NULL);
    BOXING_ASSERT(toc_preview_sections->sections->size == sections_count);

    for (unsigned int i = 0; i < toc_preview_sections->sections->size; i++)
    {
        test_not_empty_afs_toc_preview_section(GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, i), 0 + i, 100 * i, 100 * i, 64 + i, 48 + i, (90 * i) % 360);
    }
}


static void test_not_empty_afs_toc_preview_layout_definition(afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * id, const char * name, unsigned int sections_count, DBOOL sections_is_null)
{
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT_STR_EQ(toc_preview_layout_definition->id, id);
    BOXING_ASSERT_STR_EQ(toc_preview_layout_definition->name, name);

    if (sections_is_null == DTRUE)
    {
        BOXING_ASSERT(toc_preview_layout_definition->sections == NULL);
    }
    else
    {
        test_not_empty_afs_toc_preview_sections(toc_preview_layout_definition->sections, sections_count);
    }
}


static void test_not_empty_afs_toc_preview_layout_definitions(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, unsigned int definitions_count)
{
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions->layout_definitions != NULL);

    BOXING_ASSERT(afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions) == definitions_count);

    for (unsigned int i = 0; i < definitions_count; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, i);

        char current_id[10];
        char current_name[10];

        sprintf(current_id, "id %u", i);
        sprintf(current_name, "name %u", i);

        test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, current_id, current_name, 5, DFALSE);
    }
}


afs_toc_preview_sections * get_afs_toc_preview_sections_instance3(unsigned int sections_count)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    for (unsigned int i = 0; i < sections_count; i++)
    {
        afs_toc_preview_sections_append(toc_preview_sections, afs_toc_preview_section_create2(0 + i, 100 * i, 100 * i, 64 + i, 48 + i, (90 * i) % 360));
    }

    return toc_preview_sections;
}


afs_toc_preview_layout_definition * get_afs_toc_preview_layout_definition_instance2(const char * id, const char * name, unsigned int sections_count)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create2(id, name);
    toc_preview_layout_definition->sections = get_afs_toc_preview_sections_instance3(sections_count);

    return toc_preview_layout_definition;
}


afs_toc_preview_layout_definitions * get_afs_toc_preview_layout_definitions_instance(unsigned int definitions_count)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    
    for (unsigned int i = 0; i < definitions_count; i++)
    {
        char current_id[14];
        char current_name[16];

        sprintf(current_id, "id %u", i);
        sprintf(current_name, "name %u", i);

        afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance2(current_id, current_name, 5);
        afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);
    }

    return toc_preview_layout_definitions;
}


static void test_equals_toc_preview_layout_definitions(afs_toc_preview_layout_definitions * toc_preview_layout_definitions1, afs_toc_preview_layout_definitions * toc_preview_layout_definitions2, DBOOL expected_result)
{
    DBOOL result = afs_toc_preview_layout_definitions_equal(toc_preview_layout_definitions2, toc_preview_layout_definitions1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_preview_layout_definitions_equal(toc_preview_layout_definitions1, toc_preview_layout_definitions2);
    BOXING_ASSERT(result == expected_result);
}


// TEST TOC PREVIEW LAYOUT DEFINITIONS FUNCTIONS
//

// Test function afs_toc_preview_layout_definitions_create
BOXING_START_TEST(afs_toc_preview_layout_definitions_create_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_clone with input pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_clone_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = NULL;

    BOXING_ASSERT(toc_preview_layout_definitions1 == NULL);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_clone(toc_preview_layout_definitions1);

    BOXING_ASSERT(toc_preview_layout_definitions2 == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_clone with empty input afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_clone_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = afs_toc_preview_layout_definitions_create();

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_clone(toc_preview_layout_definitions1);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions2);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_clone with not empty input afs_toc_preview_layout_definitions instance with one layout definition
BOXING_START_TEST(afs_toc_preview_layout_definitions_clone_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = get_afs_toc_preview_layout_definitions_instance(1);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1, 1);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_clone(toc_preview_layout_definitions1);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2, 1);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions2);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_clone with not empty input afs_toc_preview_layout_definitions instance with three layout definitions
BOXING_START_TEST(afs_toc_preview_layout_definitions_clone_test4)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = get_afs_toc_preview_layout_definitions_instance(3);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1, 3);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_clone(toc_preview_layout_definitions1);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2, 3);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions2);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_new_reference_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = NULL;

    BOXING_ASSERT(toc_preview_layout_definitions1 == NULL);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_get_new_reference(toc_preview_layout_definitions1);

    BOXING_ASSERT(toc_preview_layout_definitions2 == NULL);

    toc_preview_layout_definitions1 = afs_toc_preview_layout_definitions_create();

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1);
    BOXING_ASSERT(toc_preview_layout_definitions1->reference_count == 1);
    BOXING_ASSERT(toc_preview_layout_definitions2 == NULL);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_new_reference with empty input afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_new_reference_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = afs_toc_preview_layout_definitions_create();

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1);
    BOXING_ASSERT(toc_preview_layout_definitions1->reference_count == 1);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_get_new_reference(toc_preview_layout_definitions1);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2);
    BOXING_ASSERT(toc_preview_layout_definitions1->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definitions2->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definitions1 == toc_preview_layout_definitions2);

    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance2("id 0", "name 0", 5);
    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions1, toc_preview_layout_definition);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2, 1);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_new_reference with not empty input afs_toc_preview_layout_definitions instance with one layout definition
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_new_reference_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = get_afs_toc_preview_layout_definitions_instance(1);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1, 1);
    BOXING_ASSERT(toc_preview_layout_definitions1->reference_count == 1);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_get_new_reference(toc_preview_layout_definitions1);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2, 1);
    BOXING_ASSERT(toc_preview_layout_definitions1->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definitions2->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definitions1 == toc_preview_layout_definitions2);

    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance2("id 1", "name 1", 5);
    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions1, toc_preview_layout_definition);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2, 2);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_new_reference with not empty input afs_toc_preview_layout_definitions instance with three layout definitions
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_new_reference_test4)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = get_afs_toc_preview_layout_definitions_instance(3);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1, 3);
    BOXING_ASSERT(toc_preview_layout_definitions1->reference_count == 1);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_get_new_reference(toc_preview_layout_definitions1);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2, 3);
    BOXING_ASSERT(toc_preview_layout_definitions1->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definitions2->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definitions1 == toc_preview_layout_definitions2);

    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance2("id 3", "name 3", 5);
    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions1, toc_preview_layout_definition);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2, 4);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_equal with input afs_toc_preview_layout_definitions pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_equal_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = NULL;

    test_equals_toc_preview_layout_definitions(toc_preview_layout_definitions1, toc_preview_layout_definitions2, DTRUE);
}
END_TEST

// Test function afs_toc_preview_layout_definitions_equal with empty input afs_toc_preview_layout_definitions instances 
BOXING_START_TEST(afs_toc_preview_layout_definitions_equal_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = afs_toc_preview_layout_definitions_create();
    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_create();
    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2);

    test_equals_toc_preview_layout_definitions(toc_preview_layout_definitions1, toc_preview_layout_definitions2, DTRUE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions2);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_equal with not empty input afs_toc_preview_layout_definitions instances with one layout definition
BOXING_START_TEST(afs_toc_preview_layout_definitions_equal_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = get_afs_toc_preview_layout_definitions_instance(1);
    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1, 1);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = get_afs_toc_preview_layout_definitions_instance(1);
    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2, 1);

    test_equals_toc_preview_layout_definitions(toc_preview_layout_definitions1, toc_preview_layout_definitions2, DTRUE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions2);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_equal with not empty input afs_toc_preview_layout_definitions instances with three layout definitions
BOXING_START_TEST(afs_toc_preview_layout_definitions_equal_test4)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = get_afs_toc_preview_layout_definitions_instance(3);
    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1, 3);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = get_afs_toc_preview_layout_definitions_instance(3);
    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2, 3);

    test_equals_toc_preview_layout_definitions(toc_preview_layout_definitions1, toc_preview_layout_definitions2, DTRUE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions2);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_equal with empty input afs_toc_preview_layout_definitions instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_preview_layout_definitions_equal_test5)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = afs_toc_preview_layout_definitions_create();
    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = NULL;

    test_equals_toc_preview_layout_definitions(toc_preview_layout_definitions1, toc_preview_layout_definitions2, DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_equal with input afs_toc_preview_layout_definitions pointer equal to NULL and instance with filled layout definitions vector 
BOXING_START_TEST(afs_toc_preview_layout_definitions_equal_test6)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = get_afs_toc_preview_layout_definitions_instance(3);
    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1, 3);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = NULL;

    test_equals_toc_preview_layout_definitions(toc_preview_layout_definitions1, toc_preview_layout_definitions2, DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_equal with empty input afs_toc_preview_layout_definitions instance and instance with filled layout definitions vector 
BOXING_START_TEST(afs_toc_preview_layout_definitions_equal_test7)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = get_afs_toc_preview_layout_definitions_instance(3);
    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1, 3);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_create();
    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2);

    test_equals_toc_preview_layout_definitions(toc_preview_layout_definitions1, toc_preview_layout_definitions2, DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions2);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_equal with input afs_toc_preview_layout_definitions instances with different layout definitions vector size 
BOXING_START_TEST(afs_toc_preview_layout_definitions_equal_test8)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = get_afs_toc_preview_layout_definitions_instance(3);
    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions1, 3);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = get_afs_toc_preview_layout_definitions_instance(4);
    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions2, 4);

    test_equals_toc_preview_layout_definitions(toc_preview_layout_definitions1, toc_preview_layout_definitions2, DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions2);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_equal with input afs_toc_preview_layout_definitions instances with different layout definition data 
BOXING_START_TEST(afs_toc_preview_layout_definitions_equal_test9)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions1 = afs_toc_preview_layout_definitions_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance2("Some id 1", "Some name 1", 5);
    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions1, toc_preview_layout_definition1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions2 = afs_toc_preview_layout_definitions_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = get_afs_toc_preview_layout_definition_instance2("Some id 2", "Some name 2", 5);
    result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions2, toc_preview_layout_definition2);

    BOXING_ASSERT(result == DTRUE);

    test_equals_toc_preview_layout_definitions(toc_preview_layout_definitions1, toc_preview_layout_definitions2, DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions1);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions2);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_is_valid with input afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_is_valid_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_is_valid(toc_preview_layout_definitions);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_is_valid with empty input afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_is_valid_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_is_valid(toc_preview_layout_definitions);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_is_valid with input afs_toc_preview_layout_definitions instance where layout definitions vector filled not valid data
BOXING_START_TEST(afs_toc_preview_layout_definitions_is_valid_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(3);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 3);

    free(GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, 1)->id);
    GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, 1)->id = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_is_valid(toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_is_valid with input afs_toc_preview_layout_definitions instance where layout definitions vector filled valid data
BOXING_START_TEST(afs_toc_preview_layout_definitions_is_valid_test4)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(5);

    DBOOL result = afs_toc_preview_layout_definitions_is_valid(toc_preview_layout_definitions);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 5);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_count with input toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_count_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    unsigned int definitions_count = afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(definitions_count == 0);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_count with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_count_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    unsigned int definitions_count = afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(definitions_count == 0);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_count with afs_toc_preview_layout_definitions instance with filled layout definitions vector
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_count_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(3);

    unsigned int definitions_count = afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 3);
    BOXING_ASSERT(definitions_count == 3);

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, get_afs_toc_preview_layout_definition_instance2("id 3", "name 3", 5));

    BOXING_ASSERT(result == DTRUE);
    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 4);

    definitions_count = afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions);

    BOXING_ASSERT(definitions_count == 4);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_index with input afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_index_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 0);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_index with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_index_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 0);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_index with not empty afs_toc_preview_layout_definitions instance and index out of range
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_index_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 2);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_index with not empty afs_toc_preview_layout_definitions instance and index in range
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_index_test4)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(6);

    for (unsigned int i = 0; i < afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions); i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, i);
        BOXING_ASSERT(afs_toc_preview_layout_definition_equal(GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i), toc_preview_layout_definition) == DTRUE);
    }

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id with input afs_toc_preview_layout_definitions pointer equal to NULL and id pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_id_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    const char * id_string = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, id_string);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(id_string == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id with input afs_toc_preview_layout_definitions pointer equal to NULL and empty id string
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_id_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    const char * id_string = "";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, id_string);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(id_string, "");
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id with input afs_toc_preview_layout_definitions pointer equal to NULL and not empty id string
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_id_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    const char * id_string = "id 0";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, id_string);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(id_string, "id 0");
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id with empty afs_toc_preview_layout_definitions instance and id pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_id_test4)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    const char * id_string = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, id_string);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(id_string == NULL);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id with empty afs_toc_preview_layout_definitions instance and empty id string
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_id_test5)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    const char * id_string = "";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, id_string);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(id_string, "");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id with empty afs_toc_preview_layout_definitions instance and not empty id string
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_id_test6)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    const char * id_string = "id 0";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, id_string);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(id_string, "id 0");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id with not empty afs_toc_preview_layout_definitions instance and id pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_id_test7)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    const char * id_string = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, id_string);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(id_string == NULL);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id with not empty afs_toc_preview_layout_definitions instance and empty id string
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_id_test8)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    const char * id_string = "";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, id_string);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(id_string, "");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id with not empty afs_toc_preview_layout_definitions instance and not exist id
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_id_test9)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    const char * id_string = "id 9";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, id_string);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(id_string, "id 9");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id with not empty afs_toc_preview_layout_definitions instance and exist id
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_id_test10)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(6);

    for (unsigned int i = 0; i < afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions); i++)
    {
        char current_id[14];
        sprintf(current_id, "id %u", i);

        afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, current_id);
        BOXING_ASSERT(afs_toc_preview_layout_definition_equal(GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i), toc_preview_layout_definition) == DTRUE);
    }

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name with input afs_toc_preview_layout_definitions pointer equal to NULL and name pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_name_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    const char * name_string = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, name_string);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(name_string == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name with input afs_toc_preview_layout_definitions pointer equal to NULL and empty name string
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_name_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    const char * name_string = "";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, name_string);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(name_string, "");
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name with input afs_toc_preview_layout_definitions pointer equal to NULL and not empty name string
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_name_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    const char * name_string = "name 0";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, name_string);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(name_string, "name 0");
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name with empty afs_toc_preview_layout_definitions instance and name pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_name_test4)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    const char * name_string = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, name_string);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(name_string == NULL);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name with empty afs_toc_preview_layout_definitions instance and empty name string
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_name_test5)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    const char * name_string = "";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, name_string);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(name_string, "");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name with empty afs_toc_preview_layout_definitions instance and not empty name string
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_name_test6)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    const char * name_string = "name 0";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, name_string);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(name_string, "name 0");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name with not empty afs_toc_preview_layout_definitions instance and name pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_name_test7)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    const char * name_string = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, name_string);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(name_string == NULL);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name with not empty afs_toc_preview_layout_definitions instance and empty name string
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_name_test8)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    const char * name_string = "";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, name_string);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(name_string, "");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name with not empty afs_toc_preview_layout_definitions instance and not exist name
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_name_test9)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    const char * name_string = "name 9";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_id(toc_preview_layout_definitions, name_string);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT_STR_EQ(name_string, "name 9");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name with not empty afs_toc_preview_layout_definitions instance and exist name
BOXING_START_TEST(afs_toc_preview_layout_definitions_get_layout_definition_by_name_test10)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(6);

    for (unsigned int i = 0; i < afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions); i++)
    {
        char current_name[16];
        sprintf(current_name, "name %u", i);

        afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_name(toc_preview_layout_definitions, current_name);
        BOXING_ASSERT(afs_toc_preview_layout_definition_equal(GVECTORN(toc_preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i), toc_preview_layout_definition) == DTRUE);
    }

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with input afs_toc_preview_layout_definitions pointers equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with input afs_toc_preview_layout_definitions pointers equal to NULL and valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance2("id 0", "name 0", 5);

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with input afs_toc_preview_layout_definitions pointers equal to NULL and not valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance2("", "", 5);

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with empty input afs_toc_preview_layout_definitions instance and afs_toc_preview_layout_definition pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test4)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with empty input afs_toc_preview_layout_definitions instance and empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test5)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with empty input afs_toc_preview_layout_definitions instance and not valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test6)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance2("", "", 5);

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with empty input afs_toc_preview_layout_definitions instance and valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test7)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance2("id 0", "name 0", 5);

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 1);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with not empty input afs_toc_preview_layout_definitions instance and afs_toc_preview_layout_definition pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test8)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with not empty input afs_toc_preview_layout_definitions instance and empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test9)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with not empty input afs_toc_preview_layout_definitions instance and not valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test10)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance2("", "", 5);

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_add_layout_definition with not empty input afs_toc_preview_layout_definitions instance and valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_add_layout_definition_test11)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance2("id 2", "name 2", 5);

    DBOOL result = afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 3);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_has_layout_definition with input afs_toc_preview_layout_definitions pointer equal to NULL and id pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_has_layout_definition_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    const char * id_string = NULL;
    DBOOL result = afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, id_string);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
    BOXING_ASSERT(id_string == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_has_layout_definition with input afs_toc_preview_layout_definitions pointer equal to NULL and empty id string
BOXING_START_TEST(afs_toc_preview_layout_definitions_has_layout_definition_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    const char * id_string = "";
    DBOOL result = afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, id_string);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
    BOXING_ASSERT_STR_EQ(id_string, "");
}
END_TEST


// Test function afs_toc_preview_layout_definitions_has_layout_definition with input afs_toc_preview_layout_definitions pointer equal to NULL and not empty id string
BOXING_START_TEST(afs_toc_preview_layout_definitions_has_layout_definition_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    const char * id_string = "id 0";
    DBOOL result = afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, id_string);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
    BOXING_ASSERT_STR_EQ(id_string, "id 0");
}
END_TEST


// Test function afs_toc_preview_layout_definitions_has_layout_definition with empty afs_toc_preview_layout_definitions instance and id pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_has_layout_definition_test4)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    const char * id_string = NULL;
    DBOOL result = afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, id_string);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(result == DFALSE);
    BOXING_ASSERT(id_string == NULL);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_has_layout_definition with empty afs_toc_preview_layout_definitions instance and empty id string
BOXING_START_TEST(afs_toc_preview_layout_definitions_has_layout_definition_test5)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    const char * id_string = "";
    DBOOL result = afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, id_string);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(result == DFALSE);
    BOXING_ASSERT_STR_EQ(id_string, "");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_has_layout_definition with empty afs_toc_preview_layout_definitions instance and not empty id string
BOXING_START_TEST(afs_toc_preview_layout_definitions_has_layout_definition_test6)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    const char * id_string = "id 0";
    DBOOL result = afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, id_string);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(result == DFALSE);
    BOXING_ASSERT_STR_EQ(id_string, "id 0");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_has_layout_definition with not empty afs_toc_preview_layout_definitions instance and id pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_has_layout_definition_test7)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    const char * id_string = NULL;
    DBOOL result = afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, id_string);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);
    BOXING_ASSERT(result == DFALSE);
    BOXING_ASSERT(id_string == NULL);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_has_layout_definition with not empty afs_toc_preview_layout_definitions instance and empty id string
BOXING_START_TEST(afs_toc_preview_layout_definitions_has_layout_definition_test8)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    const char * id_string = "";
    DBOOL result = afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, id_string);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);
    BOXING_ASSERT(result == DFALSE);
    BOXING_ASSERT_STR_EQ(id_string, "");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_has_layout_definition with not empty afs_toc_preview_layout_definitions instance and not exist id
BOXING_START_TEST(afs_toc_preview_layout_definitions_has_layout_definition_test9)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(2);
    const char * id_string = "id 9";
    DBOOL result = afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, id_string);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 2);
    BOXING_ASSERT(result == DFALSE);
    BOXING_ASSERT_STR_EQ(id_string, "id 9");

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_has_layout_definition with not empty afs_toc_preview_layout_definitions instance and exist id
BOXING_START_TEST(afs_toc_preview_layout_definitions_has_layout_definition_test10)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(6);

    for (unsigned int i = 0; i < afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions); i++)
    {
        char current_id[14];
        sprintf(current_id, "id %u", i);

        DBOOL result = afs_toc_preview_layout_definitions_has_layout_definition(toc_preview_layout_definitions, current_id);
        BOXING_ASSERT(result == DTRUE);
    }

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_file with file_name pointer equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_save_file(toc_preview_layout_definitions, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_file with file_name pointer equal to NULL and afs_toc_preview_layout_definitions pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(5);

    DBOOL result = afs_toc_preview_layout_definitions_save_file(toc_preview_layout_definitions, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_file with file_name pointer not equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_file_test3)
{
    const char * file_name = "afs_toc_preview_layout_definitions_save_file_test.xml";
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_save_file(toc_preview_layout_definitions, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_file with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_file_test4)
{
    const char * file_name = "afs_toc_preview_layout_definitions_save_file_empty_test.xml";
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_save_file(toc_preview_layout_definitions, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_file with afs_toc_preview_layout_definitions instance with layout definitions vector contains not valid data
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_file_test5)
{
    const char * file_name = "afs_toc_preview_layout_definitions_save_file_not_valid_test.xml";
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(5);

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 0);
    free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;
    
    DBOOL result = afs_toc_preview_layout_definitions_save_file(toc_preview_layout_definitions, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_file with afs_toc_preview_layout_definitions instance with filled layout definitions vector, in compact format
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_file_test6)
{
    const char * file_name = "afs_toc_preview_layout_definitions_save_file_compact_test.xml";

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(5);

    DBOOL result = afs_toc_preview_layout_definitions_save_file(toc_preview_layout_definitions, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_file with afs_toc_preview_layout_definitions instance with filled layout definitions vector, in not compact format
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_file_test7)
{
    const char * file_name = "afs_toc_preview_layout_definitions_save_file_not_compact_test.xml";

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(5);

    DBOOL result = afs_toc_preview_layout_definitions_save_file(toc_preview_layout_definitions, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_string with afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_string_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    char * result = afs_toc_preview_layout_definitions_save_string(toc_preview_layout_definitions, DTRUE);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_string with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_string_test2)
{
    char * test_string = read_xml_toc_preview_layout_definitions_file("afs_toc_preview_layout_definitions_save_file_empty_test.xml");

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    char * result = afs_toc_preview_layout_definitions_save_string(toc_preview_layout_definitions, DTRUE);

    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT_STR_EQ(result, test_string);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_string with not valid afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_string_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(5);

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 0);
    free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;

    char * result = afs_toc_preview_layout_definitions_save_string(toc_preview_layout_definitions, DTRUE);

    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_string with valid afs_toc_preview_layout_definitions instance, in compact format
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_string_test4)
{
    char * test_string = read_xml_toc_preview_layout_definitions_file("afs_toc_preview_layout_definitions_save_file_compact_test.xml");

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(5);

    char * result = afs_toc_preview_layout_definitions_save_string(toc_preview_layout_definitions, DTRUE);

    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT_STR_EQ(result, test_string);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_string with valid afs_toc_preview_layout_definitions instance, in not compact format
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_string_test5)
{
    char * test_string = read_xml_toc_preview_layout_definitions_file("afs_toc_preview_layout_definitions_save_file_not_compact_test.xml");

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(5);

    char * result = afs_toc_preview_layout_definitions_save_string(toc_preview_layout_definitions, DFALSE);

    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT_STR_EQ(result, test_string);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_as_table with input toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_as_table_test1)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;
    char * table_string = afs_toc_preview_layout_definitions_save_as_table(toc_preview_layout_definitions);

    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(table_string == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_as_table with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_as_table_test2)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    char * table_string = afs_toc_preview_layout_definitions_save_as_table(toc_preview_layout_definitions);

    const char * test_string = 
        "VISUAL LAYOUT DEFINITIONS\n"
        "=========================\n"
        "<layoutId> <sections> <name>\n"
        "\n"
        "<layoutId> <sectionId> <x> <y> <width> <height> <rotation>\n";
    
    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);
    BOXING_ASSERT(table_string != NULL);
    BOXING_ASSERT_STR_EQ(table_string, test_string);
    
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    free(table_string);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_as_table with afs_toc_preview_layout_definitions instance with filled layout definitions vector
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_as_table_test3)
{
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(3);

    char * table_string = afs_toc_preview_layout_definitions_save_as_table(toc_preview_layout_definitions);

    const char * test_string =
        "VISUAL LAYOUT DEFINITIONS\n"
        "=========================\n"
        "<layoutId> <sections> <name>\n"
        "id 0 5 name 0\n"
        "id 1 5 name 1\n"
        "id 2 5 name 2\n"
        "\n"
        "<layoutId> <sectionId> <x> <y> <width> <height> <rotation>\n"
        "id 0 0 000 000 64 48 000\n"
        "id 0 1 100 100 65 49 090\n"
        "id 0 2 200 200 66 50 180\n"
        "id 0 3 300 300 67 51 270\n"
        "id 0 4 400 400 68 52 000\n"
        "id 1 0 000 000 64 48 000\n"
        "id 1 1 100 100 65 49 090\n"
        "id 1 2 200 200 66 50 180\n"
        "id 1 3 300 300 67 51 270\n"
        "id 1 4 400 400 68 52 000\n"
        "id 2 0 000 000 64 48 000\n"
        "id 2 1 100 100 65 49 090\n"
        "id 2 2 200 200 66 50 180\n"
        "id 2 3 300 300 67 51 270\n"
        "id 2 4 400 400 68 52 000\n"
        "\n";

    BOXING_ASSERT(table_string != NULL);
    BOXING_ASSERT_STR_EQ(table_string, test_string);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    free(table_string);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_xml with mxml_node_t pointer equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_save_xml(toc_preview_layout_definitions, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_xml with mxml_node_t pointer equal to NULL and afs_toc_preview_layout_definitions pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(3);

    DBOOL result = afs_toc_preview_layout_definitions_save_xml(toc_preview_layout_definitions, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_save_xml(toc_preview_layout_definitions, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_xml with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_save_xml(toc_preview_layout_definitions, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_xml with not valid afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(3);

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 0);
    free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_save_xml(toc_preview_layout_definitions, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_save_xml with valid afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance(3);

    DBOOL result = afs_toc_preview_layout_definitions_save_xml(toc_preview_layout_definitions, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_file with file_name pointer equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_load_file(toc_preview_layout_definitions, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_file with file_name pointer equal to NULL and afs_toc_preview_layout_definitions pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_file(toc_preview_layout_definitions, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_file with file_name pointer not equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_file_test3)
{
    const char * file_name = "afs_toc_preview_layout_definitions_save_file_compact_test.xml";
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_load_file(toc_preview_layout_definitions, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_file(toc_preview_layout_definitions, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_file with initialized afs_toc_preview_layout_definitions instance, in compact format
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_file_test5)
{
    const char * file_name = "afs_toc_preview_layout_definitions_save_file_compact_test.xml";
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_file(toc_preview_layout_definitions, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 5);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_file with initialized afs_toc_preview_layout_definitions instance, in not compact format
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_file_test6)
{
    const char * file_name = "afs_toc_preview_layout_definitions_save_file_not_compact_test.xml";
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_file(toc_preview_layout_definitions, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 5);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_string with input string pointer equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_load_string(toc_preview_layout_definitions, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_string with input string pointer equal to NULL and afs_toc_preview_layout_definitions pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_string(toc_preview_layout_definitions, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_string with input string pointer not equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_string_test3)
{
    char * input_string = read_xml_toc_preview_layout_definitions_file("afs_toc_preview_layout_definitions_save_file_compact_test.xml");
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_load_string(toc_preview_layout_definitions, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_string with empty input string
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_string_test4)
{
    char * input_string = "";
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_string(toc_preview_layout_definitions, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_string with input string for initialized afs_toc_preview_layout_definitions instance, in compact format
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_string_test5)
{
    char * input_string = read_xml_toc_preview_layout_definitions_file("afs_toc_preview_layout_definitions_save_file_compact_test.xml");

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_string(toc_preview_layout_definitions, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 5);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    free(input_string);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_string with input string for initialized afs_toc_preview_layout_definitions instance, in not compact format
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_string_test6)
{
    char * input_string = read_xml_toc_preview_layout_definitions_file("afs_toc_preview_layout_definitions_save_file_not_compact_test.xml");

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_string(toc_preview_layout_definitions, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 5);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    free(input_string);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_xml with xml instance pointer equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_load_xml(toc_preview_layout_definitions, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_xml with xml instance pointer equal to NULL and afs_toc_preview_layout_definitions pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_xml(toc_preview_layout_definitions, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_xml with xml instance pointer not equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_xml_test3)
{
    mxml_node_t *tree = get_toc_preview_layout_definitions_xml_instance("afs_toc_preview_layout_definitions_save_file_compact_test.xml");
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_preview_layout_definitions_load_xml(toc_preview_layout_definitions, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_layout_definitions == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_xml(toc_preview_layout_definitions, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_xml with xml instance for initialized afs_toc_preview_layout_definitions instance, in compact format
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_xml_test5)
{
    mxml_node_t *tree = get_toc_preview_layout_definitions_xml_instance("afs_toc_preview_layout_definitions_save_file_compact_test.xml");
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_xml(toc_preview_layout_definitions, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 5);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_load_xml with xml instance for initialized afs_toc_preview_layout_definitions instance, in not compact format
BOXING_START_TEST(afs_toc_preview_layout_definitions_load_xml_test6)
{
    mxml_node_t *tree = get_toc_preview_layout_definitions_xml_instance("afs_toc_preview_layout_definitions_save_file_not_compact_test.xml");
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_preview_layout_definitions_load_xml(toc_preview_layout_definitions, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definitions(toc_preview_layout_definitions, 5);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
    mxmlDelete(tree);
}
END_TEST


Suite * tocpreviewlayoutdefinitions_tests(void)
{
    TCase * tc_tocpreviewlayoutdefinitions_functions_tests = tcase_create("tocpreviewlayoutdefinitions_functions_tests");

    // Test function afs_toc_preview_layout_definitions_create
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_create_test1);
    // Test function afs_toc_preview_layout_definitions_clone
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_clone_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_clone_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_clone_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_clone_test4);
    // Test function afs_toc_preview_layout_definitions_get_new_reference
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_new_reference_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_new_reference_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_new_reference_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_new_reference_test4);
    // Test function afs_toc_preview_layout_definitions_equal
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_equal_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_equal_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_equal_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_equal_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_equal_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_equal_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_equal_test7);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_equal_test8);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_equal_test9);
    // Test function afs_toc_preview_layout_definitions_is_valid
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_is_valid_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_is_valid_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_is_valid_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_is_valid_test4);
    // Test function afs_toc_preview_layout_definitions_get_count
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_count_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_count_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_count_test3);
    // Test function afs_toc_preview_layout_definitions_get_layout_definition_by_index
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_index_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_index_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_index_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_index_test4);
    // Test function afs_toc_preview_layout_definitions_get_layout_definition_by_id
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_id_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_id_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_id_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_id_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_id_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_id_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_id_test7);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_id_test8);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_id_test9);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_id_test10);
    // Test function afs_toc_preview_layout_definitions_get_layout_definition_by_name
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_name_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_name_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_name_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_name_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_name_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_name_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_name_test7);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_name_test8);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_name_test9);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_get_layout_definition_by_name_test10);
    // Test function afs_toc_preview_layout_definitions_add_layout_definition
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test7);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test8);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test9);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test10);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_add_layout_definition_test11);
    // Test function afs_toc_preview_layout_definitions_has_layout_definition
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_has_layout_definition_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_has_layout_definition_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_has_layout_definition_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_has_layout_definition_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_has_layout_definition_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_has_layout_definition_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_has_layout_definition_test7);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_has_layout_definition_test8);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_has_layout_definition_test9);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_has_layout_definition_test10);
    // Test function afs_toc_preview_layout_definitions_save_file
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_file_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_file_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_file_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_file_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_file_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_file_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_file_test7);
    // Test function afs_toc_preview_layout_definitions_save_string
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_string_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_string_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_string_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_string_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_string_test5);
    // Test function afs_toc_preview_layout_definitions_save_as_table
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_as_table_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_as_table_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_as_table_test3);
    // Test function afs_toc_preview_layout_definitions_save_xml
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_xml_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_xml_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_xml_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_xml_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_xml_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_save_xml_test6);
    // Test function afs_toc_preview_layout_definitions_load_file
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_file_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_file_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_file_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_file_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_file_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_file_test6);
    // Test function afs_toc_preview_layout_definitions_load_string
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_string_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_string_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_string_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_string_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_string_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_string_test6);
    // Test function afs_toc_preview_layout_definitions_load_xml
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_xml_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_xml_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_xml_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_xml_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_xml_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinitions_functions_tests, afs_toc_preview_layout_definitions_load_xml_test6);

    Suite * s = suite_create("tocpreviewlayoutdefinitions_test_util");
    suite_add_tcase(s, tc_tocpreviewlayoutdefinitions_functions_tests);

    return s;
}
