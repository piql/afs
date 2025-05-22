#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "toc/previewsections.h"
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


static char * read_xml_toc_preview_sections_file(const char* file_name)
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


static mxml_node_t * get_toc_preview_sections_xml_instance(const char * file_name)
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


static void test_empty_afs_toc_preview_sections(afs_toc_preview_sections * toc_preview_sections)
{
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(toc_preview_sections->sections == NULL);
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


afs_toc_preview_sections * get_afs_toc_preview_sections_instance(unsigned int sections_count)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    for (unsigned int i = 0; i < sections_count; i++)
    {
        afs_toc_preview_sections_append(toc_preview_sections, afs_toc_preview_section_create2(0 + i, 100 * i, 100 * i, 64 + i, 48 + i, (90 * i) % 360));
    }

    return toc_preview_sections;
}


static void test_equals_toc_preview_sections(afs_toc_preview_sections * toc_preview_sections1, afs_toc_preview_sections * toc_preview_sections2, DBOOL expected_result)
{
    DBOOL result = afs_toc_preview_sections_equal(toc_preview_sections2, toc_preview_sections1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_preview_sections_equal(toc_preview_sections1, toc_preview_sections2);
    BOXING_ASSERT(result == expected_result);
}

// TEST TOC PREVIEW SECTIONS FUNCTIONS
//

// Test function afs_toc_preview_sections_create
BOXING_START_TEST(afs_toc_preview_sections_create_test1)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    test_empty_afs_toc_preview_sections(toc_preview_sections);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_create2 test1 with XML node pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_create2_test1)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create2(NULL);

    test_empty_afs_toc_preview_sections(toc_preview_sections);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_create2 test2 with empty XML node
BOXING_START_TEST(afs_toc_preview_sections_create2_test2)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create2(tree);

    test_empty_afs_toc_preview_sections(toc_preview_sections);

    afs_toc_preview_sections_free(toc_preview_sections);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_sections_create2 test3 with correct XML node
BOXING_START_TEST(afs_toc_preview_sections_create2_test3)
{
    afs_toc_preview_sections * toc_preview_sections1 = get_afs_toc_preview_sections_instance(3);

    mxml_node_t *tree = mxmlNewXML("1.0");
    DBOOL result = afs_toc_preview_sections_save_xml(toc_preview_sections1, tree);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_create2(tree);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections2, 3);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections2);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_sections_clone test with input pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_clone_test1)
{
    afs_toc_preview_sections * toc_preview_sections1 = NULL;

    BOXING_ASSERT(toc_preview_sections1 == NULL);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_clone(toc_preview_sections1);

    BOXING_ASSERT(toc_preview_sections2 == NULL);
}
END_TEST


// Test function afs_toc_preview_sections_clone test with empty input afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_clone_test2)
{
    afs_toc_preview_sections * toc_preview_sections1 = afs_toc_preview_sections_create();

    test_empty_afs_toc_preview_sections(toc_preview_sections1);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_clone(toc_preview_sections1);

    test_empty_afs_toc_preview_sections(toc_preview_sections2);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections2);
}
END_TEST


// Test function afs_toc_preview_sections_clone test with not empty input afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_clone_test3)
{
    afs_toc_preview_sections * toc_preview_sections1 = get_afs_toc_preview_sections_instance(3);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections1, 3);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_clone(toc_preview_sections1);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections2, 3);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections2);
}
END_TEST


// Test function afs_toc_preview_sections_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_get_new_reference_test1)
{
    afs_toc_preview_sections * toc_preview_sections1 = NULL;

    BOXING_ASSERT(toc_preview_sections1 == NULL);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_get_new_reference(toc_preview_sections1);

    BOXING_ASSERT(toc_preview_sections2 == NULL);

    toc_preview_sections1 = afs_toc_preview_sections_create();

    test_empty_afs_toc_preview_sections(toc_preview_sections1);
    BOXING_ASSERT(toc_preview_sections1->reference_count == 1);
    BOXING_ASSERT(toc_preview_sections2 == NULL);

    afs_toc_preview_sections_free(toc_preview_sections1);
}
END_TEST


// Test function afs_toc_preview_sections_get_new_reference with empty input afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_get_new_reference_test2)
{
    afs_toc_preview_sections * toc_preview_sections1 = afs_toc_preview_sections_create();

    test_empty_afs_toc_preview_sections(toc_preview_sections1);
    BOXING_ASSERT(toc_preview_sections1->reference_count == 1);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_get_new_reference(toc_preview_sections1);

    test_empty_afs_toc_preview_sections(toc_preview_sections2);
    BOXING_ASSERT(toc_preview_sections1->reference_count == 2);
    BOXING_ASSERT(toc_preview_sections2->reference_count == 2);
    BOXING_ASSERT(toc_preview_sections1 == toc_preview_sections2);

    afs_toc_preview_section * section = afs_toc_preview_section_create2(0, 0, 0, 64, 48, 0);
    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections1, section);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections2, 1);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections1);
}
END_TEST


// Test function afs_toc_preview_sections_get_new_reference with not empty input afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_get_new_reference_test3)
{
    afs_toc_preview_sections * toc_preview_sections1 = get_afs_toc_preview_sections_instance(3);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections1, 3);
    BOXING_ASSERT(toc_preview_sections1->reference_count == 1);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_get_new_reference(toc_preview_sections1);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections2, 3);
    BOXING_ASSERT(toc_preview_sections1->reference_count == 2);
    BOXING_ASSERT(toc_preview_sections2->reference_count == 2);
    BOXING_ASSERT(toc_preview_sections1 == toc_preview_sections2);

    afs_toc_preview_section * section = afs_toc_preview_section_create2(3, 300, 300, 67, 51, 270);
    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections1, section);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections2, 4);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections1);
}
END_TEST


// Test function afs_toc_preview_sections_equal test with input afs_toc_preview_sections pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_equal_test1)
{
    afs_toc_preview_sections * toc_preview_sections1 = NULL;
    afs_toc_preview_sections * toc_preview_sections2 = NULL;

    test_equals_toc_preview_sections(toc_preview_sections1, toc_preview_sections2, DTRUE);
}
END_TEST

// Test function afs_toc_preview_sections_equal test with empty input afs_toc_preview_sections instances 
BOXING_START_TEST(afs_toc_preview_sections_equal_test2)
{
    afs_toc_preview_sections * toc_preview_sections1 = afs_toc_preview_sections_create();
    test_empty_afs_toc_preview_sections(toc_preview_sections1);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_create();
    test_empty_afs_toc_preview_sections(toc_preview_sections2);

    test_equals_toc_preview_sections(toc_preview_sections1, toc_preview_sections2, DTRUE);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections2);
}
END_TEST


// Test function afs_toc_preview_sections_equal test with not empty input afs_toc_preview_sections instances with one section
BOXING_START_TEST(afs_toc_preview_sections_equal_test3)
{
    afs_toc_preview_sections * toc_preview_sections1 = get_afs_toc_preview_sections_instance(1);
    test_not_empty_afs_toc_preview_sections(toc_preview_sections1, 1);

    afs_toc_preview_sections * toc_preview_sections2 = get_afs_toc_preview_sections_instance(1);
    test_not_empty_afs_toc_preview_sections(toc_preview_sections2, 1);

    test_equals_toc_preview_sections(toc_preview_sections1, toc_preview_sections2, DTRUE);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections2);
}
END_TEST


// Test function afs_toc_preview_sections_equal test with not empty input afs_toc_preview_sections instances with three sections
BOXING_START_TEST(afs_toc_preview_sections_equal_test4)
{
    afs_toc_preview_sections * toc_preview_sections1 = get_afs_toc_preview_sections_instance(3);
    test_not_empty_afs_toc_preview_sections(toc_preview_sections1, 3);

    afs_toc_preview_sections * toc_preview_sections2 = get_afs_toc_preview_sections_instance(3);
    test_not_empty_afs_toc_preview_sections(toc_preview_sections2, 3);

    test_equals_toc_preview_sections(toc_preview_sections1, toc_preview_sections2, DTRUE);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections2);
}
END_TEST


// Test function afs_toc_preview_sections_equal test with empty input afs_toc_preview_sections instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_preview_sections_equal_test5)
{
    afs_toc_preview_sections * toc_preview_sections1 = afs_toc_preview_sections_create();
    test_empty_afs_toc_preview_sections(toc_preview_sections1);

    afs_toc_preview_sections * toc_preview_sections2 = NULL;

    test_equals_toc_preview_sections(toc_preview_sections1, toc_preview_sections2, DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections1);
}
END_TEST


// Test function afs_toc_preview_sections_equal with input afs_toc_preview_sections pointer equal to NULL and instance with filled sections vector 
BOXING_START_TEST(afs_toc_preview_sections_equal_test6)
{
    afs_toc_preview_sections * toc_preview_sections1 = get_afs_toc_preview_sections_instance(3);
    test_not_empty_afs_toc_preview_sections(toc_preview_sections1, 3);

    afs_toc_preview_sections * toc_preview_sections2 = NULL;

    test_equals_toc_preview_sections(toc_preview_sections1, toc_preview_sections2, DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections1);
}
END_TEST


// Test function afs_toc_preview_sections_equal with empty input afs_toc_preview_sections instance and instance with filled sections vector 
BOXING_START_TEST(afs_toc_preview_sections_equal_test7)
{
    afs_toc_preview_sections * toc_preview_sections1 = get_afs_toc_preview_sections_instance(3);
    test_not_empty_afs_toc_preview_sections(toc_preview_sections1, 3);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_create();
    test_empty_afs_toc_preview_sections(toc_preview_sections2);

    test_equals_toc_preview_sections(toc_preview_sections1, toc_preview_sections2, DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections2);
}
END_TEST


// Test function afs_toc_preview_sections_equal with input afs_toc_preview_sections instances with different sections vector size 
BOXING_START_TEST(afs_toc_preview_sections_equal_test8)
{
    afs_toc_preview_sections * toc_preview_sections1 = get_afs_toc_preview_sections_instance(3);
    test_not_empty_afs_toc_preview_sections(toc_preview_sections1, 3);

    afs_toc_preview_sections * toc_preview_sections2 = get_afs_toc_preview_sections_instance(6);
    test_not_empty_afs_toc_preview_sections(toc_preview_sections2, 6);

    test_equals_toc_preview_sections(toc_preview_sections1, toc_preview_sections2, DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections2);
}
END_TEST


// Test function afs_toc_metadata_equal test with input afs_toc_metadata instances with different sources data 
BOXING_START_TEST(afs_toc_preview_sections_equal_test9)
{
    afs_toc_preview_sections * toc_preview_sections1 = afs_toc_preview_sections_create();
    afs_toc_preview_sections_append(toc_preview_sections1, afs_toc_preview_section_create2(0, 100, 100, 640, 480, 0));

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_create();
    afs_toc_preview_sections_append(toc_preview_sections2, afs_toc_preview_section_create2(1, 200, 200, 1080, 920, 90));

    test_equals_toc_preview_sections(toc_preview_sections1, toc_preview_sections2, DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections1);
    afs_toc_preview_sections_free(toc_preview_sections2);
}
END_TEST


// Test function afs_toc_preview_sections_is_valid with input afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_is_valid_test1)
{
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_is_valid(toc_preview_sections);

    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_sections_is_valid with empty input afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_is_valid_test2)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_is_valid(toc_preview_sections);

    test_empty_afs_toc_preview_sections(toc_preview_sections);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_is_valid with input afs_toc_preview_sections instance where sections vector filled not valid data
BOXING_START_TEST(afs_toc_preview_sections_is_valid_test3)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(3);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 3);

    GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, 1)->width = 0;

    DBOOL result = afs_toc_preview_sections_is_valid(toc_preview_sections);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_is_valid with input afs_toc_preview_sections instance where sections vector filled valid data
BOXING_START_TEST(afs_toc_preview_sections_is_valid_test4)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(5);

    DBOOL result = afs_toc_preview_sections_is_valid(toc_preview_sections);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 5);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_is_valid with input afs_toc_preview_sections instance with overlapping sections
BOXING_START_TEST(afs_toc_preview_sections_is_valid_test5)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(5);
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_sections_get(toc_preview_sections, 1);
    toc_preview_section->x = 0;
    toc_preview_section->y = 0;

    DBOOL result = afs_toc_preview_sections_is_valid(toc_preview_sections);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_size test with input toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_size_test1)
{
    afs_toc_preview_sections * toc_preview_sections = NULL;
    unsigned int sections_count = afs_toc_preview_sections_size(toc_preview_sections);

    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(sections_count == 0);
}
END_TEST


// Test function afs_toc_preview_sections_size test with empty afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_size_test2)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    unsigned int sections_count = afs_toc_preview_sections_size(toc_preview_sections);

    test_empty_afs_toc_preview_sections(toc_preview_sections);
    BOXING_ASSERT(sections_count == 0);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_size test with afs_toc_preview_sections instance with filled sections vector
BOXING_START_TEST(afs_toc_preview_sections_size_test3)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(3);

    unsigned int sections_count = afs_toc_preview_sections_size(toc_preview_sections);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 3);
    BOXING_ASSERT(sections_count == 3);

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, afs_toc_preview_section_create2(3, 300, 300, 67, 51, 270));

    BOXING_ASSERT(result == DTRUE);
    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 4);

    sections_count = afs_toc_preview_sections_size(toc_preview_sections);

    BOXING_ASSERT(sections_count == 4);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_append with input afs_toc_preview_sections pointers equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_append_test1)
{
    afs_toc_preview_sections * toc_preview_sections = NULL;
    afs_toc_preview_section * section = NULL;

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_sections_append with input afs_toc_preview_sections pointers equal to NULL and valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_sections_append_test2)
{
    afs_toc_preview_sections * toc_preview_sections = NULL;
    afs_toc_preview_section * section = afs_toc_preview_section_create2(3, 103, 103, 643, 483, 270);

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_sections_append with input afs_toc_preview_sections pointers equal to NULL and not valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_sections_append_test3)
{
    afs_toc_preview_sections * toc_preview_sections = NULL;
    afs_toc_preview_section * section = afs_toc_preview_section_create2(3, 103, 103, 0, 483, 270);

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_sections_append with empty input afs_toc_preview_sections instance and afs_toc_preview_section pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_append_test4)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    afs_toc_preview_section * section = NULL;

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_append with empty input afs_toc_preview_sections instance and empty afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_sections_append_test5)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    afs_toc_preview_section * section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_sections_append with empty input afs_toc_preview_sections instance and not valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_sections_append_test6)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    afs_toc_preview_section * section = afs_toc_preview_section_create2(3, 103, 103, 0, 483, 270);

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_sections_append with empty input afs_toc_preview_sections instance and valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_sections_append_test7)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    afs_toc_preview_section * section = afs_toc_preview_section_create2(0, 0, 0, 64, 48, 0);

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 1);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_append with input afs_toc_preview_sections instance with filled sections vector and afs_toc_preview_section pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_append_test8)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(2);
    afs_toc_preview_section * section = NULL;

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 2);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_append with input afs_toc_preview_sections instance with filled sections vector and empty afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_sections_append_test9)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(2);
    afs_toc_preview_section * section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 2);

    afs_toc_preview_sections_free(toc_preview_sections);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_sections_append with input afs_toc_preview_sections instance with filled sections vector and not valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_sections_append_test10)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(2);
    afs_toc_preview_section * section = afs_toc_preview_section_create2(3, 103, 103, 0, 483, 270);
 
    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 2);

    afs_toc_preview_sections_free(toc_preview_sections);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_sections_append with input afs_toc_preview_sections instance with filled sections vector and valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_sections_append_test11)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(2);
    afs_toc_preview_section * section = afs_toc_preview_section_create2(2, 200, 200, 66, 50, 180);

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 3);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_append with input afs_toc_preview_sections instance with filled sections vector and afs_toc_preview_section instance with overlapping
BOXING_START_TEST(afs_toc_preview_sections_append_test12)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(2);
    afs_toc_preview_section * section = afs_toc_preview_section_create2(2, 0, 0, 66, 50, 180);

    DBOOL result = afs_toc_preview_sections_append(toc_preview_sections, section);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 2);

    afs_toc_preview_sections_free(toc_preview_sections);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_sections_get with input afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_get_test1)
{
    afs_toc_preview_sections * toc_preview_sections = NULL;
    afs_toc_preview_section * section = afs_toc_preview_sections_get(toc_preview_sections, 0);

    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(section == NULL);
}
END_TEST


// Test function afs_toc_preview_sections_get with empty afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_get_test2)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    afs_toc_preview_section * section = afs_toc_preview_sections_get(toc_preview_sections, 0);

    test_empty_afs_toc_preview_sections(toc_preview_sections);
    BOXING_ASSERT(section == NULL);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_get with not empty afs_toc_preview_sections instance and index out of range
BOXING_START_TEST(afs_toc_preview_sections_get_test3)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(2);

    afs_toc_preview_section * section = afs_toc_preview_sections_get(toc_preview_sections, 2);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 2);
    BOXING_ASSERT(section == NULL);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_get with not empty afs_toc_preview_sections instance and index in range
BOXING_START_TEST(afs_toc_preview_sections_get_test4)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(6);

    for (unsigned int i = 0; i < toc_preview_sections->sections->size; i++)
    {
        afs_toc_preview_section * section = afs_toc_preview_sections_get(toc_preview_sections, i);
        BOXING_ASSERT(afs_toc_preview_section_equal(GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, i), section) == DTRUE);
    }

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_save_file with file_name pointer equal to NULL and afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_save_file(toc_preview_sections, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_sections_save_file with file_name pointer equal to NULL and afs_toc_preview_sections pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(5);

    DBOOL result = afs_toc_preview_sections_save_file(toc_preview_sections, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_save_file with file_name pointer not equal to NULL and afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_save_file_test3)
{
    const char * file_name = "afs_toc_preview_sections_save_file_test.xml";
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_save_file(toc_preview_sections, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_sections_save_file with empty afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_save_file_test4)
{
    const char * file_name = "afs_toc_preview_sections_save_file_empty_test.xml";
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_save_file(toc_preview_sections, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_save_file with afs_toc_preview_sections instance with sections vector contains not valid data
BOXING_START_TEST(afs_toc_preview_sections_save_file_test5)
{
    const char * file_name = "afs_toc_preview_sections_save_file_not_valid_test.xml";
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(5);

    GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, 1)->width = 0;

    DBOOL result = afs_toc_preview_sections_save_file(toc_preview_sections, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_save_file with afs_toc_preview_sections instance with filled sections vector, in compact format
BOXING_START_TEST(afs_toc_preview_sections_save_file_test6)
{
    const char * file_name = "afs_toc_preview_sections_save_file_compact_test.xml";

    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(5);

    DBOOL result = afs_toc_preview_sections_save_file(toc_preview_sections, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_save_file with afs_toc_preview_sections instance with filled sections vector, in not compact format
BOXING_START_TEST(afs_toc_preview_sections_save_file_test7)
{
    const char * file_name = "afs_toc_preview_sections_save_file_not_compact_test.xml";

    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(5);

    DBOOL result = afs_toc_preview_sections_save_file(toc_preview_sections, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_save_string with afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_save_string_test1)
{
    afs_toc_preview_sections * toc_preview_sections = NULL;

    char * result = afs_toc_preview_sections_save_string(toc_preview_sections, DTRUE);

    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_preview_sections_save_string with empty afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_save_string_test2)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    char * result = afs_toc_preview_sections_save_string(toc_preview_sections, DTRUE);

    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_save_string with not valid afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_save_string_test3)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(5);

    GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, 1)->width = 0;

    char * result = afs_toc_preview_sections_save_string(toc_preview_sections, DTRUE);

    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_save_string with valid afs_toc_preview_sections instance, in compact format
BOXING_START_TEST(afs_toc_preview_sections_save_string_test4)
{
    char * test_string = read_xml_toc_preview_sections_file("afs_toc_preview_sections_save_file_compact_test.xml");

    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(5);

    char * result = afs_toc_preview_sections_save_string(toc_preview_sections, DTRUE);

    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_preview_sections_free(toc_preview_sections);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_preview_sections_save_string with valid afs_toc_preview_sections instance, in not compact format
BOXING_START_TEST(afs_toc_preview_sections_save_string_test5)
{
    char * test_string = read_xml_toc_preview_sections_file("afs_toc_preview_sections_save_file_not_compact_test.xml");
    
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(5);

    char * result = afs_toc_preview_sections_save_string(toc_preview_sections, DFALSE);

    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_preview_sections_free(toc_preview_sections);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_preview_sections_save_xml with mxml_node_t pointer equal to NULL and afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_save_xml(toc_preview_sections, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_sections_save_xml with mxml_node_t pointer equal to NULL and afs_toc_preview_sections pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(3);

    DBOOL result = afs_toc_preview_sections_save_xml(toc_preview_sections, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_save_xml(toc_preview_sections, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_sections_save_xml with empty afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_save_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_save_xml(toc_preview_sections, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_sections_save_xml with not valid afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(3);

    GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, 1)->width = 0;

    DBOOL result = afs_toc_preview_sections_save_xml(toc_preview_sections, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_sections_save_xml with valid afs_toc_preview_sections instance
BOXING_START_TEST(afs_toc_preview_sections_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance(3);

    DBOOL result = afs_toc_preview_sections_save_xml(toc_preview_sections, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_sections_free(toc_preview_sections);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_sections_load_file with file_name pointer equal to NULL and afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_load_file(toc_preview_sections, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_sections_load_file with file_name pointer equal to NULL and afs_toc_preview_sections pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_file(toc_preview_sections, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_sections != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_load_file with file_name pointer not equal to NULL and afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_load_file_test3)
{
    const char * file_name = "afs_toc_preview_sections_save_file_compact_test.xml";
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_load_file(toc_preview_sections, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_sections_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_preview_sections_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_file(toc_preview_sections, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_sections(toc_preview_sections);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_load_file with initialized afs_toc_preview_sections instance, in compact format
BOXING_START_TEST(afs_toc_preview_sections_load_file_test5)
{
    const char * file_name = "afs_toc_preview_sections_save_file_compact_test.xml";
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_file(toc_preview_sections, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 5);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_load_file with initialized afs_toc_preview_sections instance, in not compact format
BOXING_START_TEST(afs_toc_preview_sections_load_file_test6)
{
    const char * file_name = "afs_toc_preview_sections_save_file_not_compact_test.xml";
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_file(toc_preview_sections, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 5);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_load_string with input string pointer equal to NULL and afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_load_string(toc_preview_sections, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_sections_load_string with input string pointer equal to NULL and afs_toc_preview_sections pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_string(toc_preview_sections, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_sections(toc_preview_sections);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_load_string with input string pointer not equal to NULL and afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_load_string_test3)
{
    char * input_string = read_xml_toc_preview_sections_file("afs_toc_preview_sections_save_file_compact_test.xml");
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_load_string(toc_preview_sections, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_preview_sections_load_string with empty input string
BOXING_START_TEST(afs_toc_preview_sections_load_string_test4)
{
    char * input_string = "";
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_string(toc_preview_sections, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_sections(toc_preview_sections);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_load_string with input string for initialized afs_toc_preview_sections instance, in compact format
BOXING_START_TEST(afs_toc_preview_sections_load_string_test5)
{
    char * input_string = read_xml_toc_preview_sections_file("afs_toc_preview_sections_save_file_compact_test.xml");

    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_string(toc_preview_sections, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 5);

    afs_toc_preview_sections_free(toc_preview_sections);
    free(input_string);
}
END_TEST


// Test function afs_toc_preview_sections_load_string with input string for initialized afs_toc_preview_sections instance, in not compact format
BOXING_START_TEST(afs_toc_preview_sections_load_string_test6)
{
    char * input_string = read_xml_toc_preview_sections_file("afs_toc_preview_sections_save_file_not_compact_test.xml");

    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_string(toc_preview_sections, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 5);

    afs_toc_preview_sections_free(toc_preview_sections);
    free(input_string);
}
END_TEST


// Test function afs_toc_preview_sections_load_xml with xml instance pointer equal to NULL and afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_load_xml(toc_preview_sections, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_sections_load_xml with xml instance pointer equal to NULL and afs_toc_preview_sections pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_xml(toc_preview_sections, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_sections(toc_preview_sections);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_sections_load_xml with xml instance pointer not equal to NULL and afs_toc_preview_sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_sections_load_xml_test3)
{
    mxml_node_t *tree = get_toc_preview_sections_xml_instance("afs_toc_preview_sections_save_file_compact_test.xml");
    afs_toc_preview_sections * toc_preview_sections = NULL;

    DBOOL result = afs_toc_preview_sections_load_xml(toc_preview_sections, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_sections == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_sections_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_preview_sections_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_xml(toc_preview_sections, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_sections(toc_preview_sections);

    afs_toc_preview_sections_free(toc_preview_sections);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_sections_load_xml with xml instance for initialized afs_toc_preview_sections instance, in compact format
BOXING_START_TEST(afs_toc_preview_sections_load_xml_test5)
{
    mxml_node_t *tree = get_toc_preview_sections_xml_instance("afs_toc_preview_sections_save_file_compact_test.xml");
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_xml(toc_preview_sections, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 5);

    afs_toc_preview_sections_free(toc_preview_sections);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_sections_load_xml with xml instance for initialized afs_toc_preview_sections instance, in not compact format
BOXING_START_TEST(afs_toc_preview_sections_load_xml_test6)
{
    mxml_node_t *tree = get_toc_preview_sections_xml_instance("afs_toc_preview_sections_save_file_not_compact_test.xml");
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    DBOOL result = afs_toc_preview_sections_load_xml(toc_preview_sections, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_sections(toc_preview_sections, 5);

    afs_toc_preview_sections_free(toc_preview_sections);
    mxmlDelete(tree);
}
END_TEST


Suite * tocpreviewsections_tests(void)
{
    TCase * tc_tocpreviewsections_functions_tests = tcase_create("tocpreviewsections_functions_tests");

    // Test function afs_toc_preview_sections_create
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_create_test1);
    // Test function afs_toc_preview_sections_create2
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_create2_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_create2_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_create2_test3);
    // Test function afs_toc_preview_sections_clone
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_clone_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_clone_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_clone_test3);
    // Test function afs_toc_preview_sections_get_new_reference
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_get_new_reference_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_get_new_reference_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_get_new_reference_test3);
    // Test function afs_toc_preview_sections_equal
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_equal_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_equal_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_equal_test3);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_equal_test4);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_equal_test5);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_equal_test6);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_equal_test7);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_equal_test8);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_equal_test9);
    // Test function afs_toc_preview_sections_is_valid
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_is_valid_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_is_valid_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_is_valid_test3);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_is_valid_test4);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_is_valid_test5);
    // Test function afs_toc_preview_sections_size
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_size_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_size_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_size_test3);
    // Test function afs_toc_preview_sections_append
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test3);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test4);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test5);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test6);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test7);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test8);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test9);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test10);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test11);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_append_test12);
    // Test function afs_toc_preview_sections_get
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_get_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_get_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_get_test3);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_get_test4);
    // Test function afs_toc_preview_sections_save_file
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_file_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_file_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_file_test3);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_file_test4);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_file_test5);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_file_test6);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_file_test7);
    // Test function afs_toc_preview_sections_save_string
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_string_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_string_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_string_test3);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_string_test4);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_string_test5);
    // Test function afs_toc_preview_sections_save_xml
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_xml_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_xml_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_xml_test3);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_xml_test4);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_xml_test5);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_save_xml_test6);
    // Test function afs_toc_preview_sections_load_file
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_file_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_file_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_file_test3);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_file_test4);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_file_test5);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_file_test6);
    // Test function afs_toc_preview_sections_load_string
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_string_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_string_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_string_test3);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_string_test4);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_string_test5);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_string_test6);
    // Test function afs_toc_preview_sections_load_xml
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_xml_test1);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_xml_test2);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_xml_test3);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_xml_test4);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_xml_test5);
    tcase_add_test(tc_tocpreviewsections_functions_tests, afs_toc_preview_sections_load_xml_test6);

    Suite * s = suite_create("tocpreviewsections_test_util");
    suite_add_tcase(s, tc_tocpreviewsections_functions_tests);

    return s;
}
