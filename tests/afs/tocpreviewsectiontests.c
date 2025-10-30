#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "toc/previewsection.h"
#include "boxing/platform/types.h"
#include "boxing/utils.h"
#include "boxing/string.h"
#include "mxml.h"
#include "platform/io.h"

#include <string.h>

static char * read_xml_toc_preview_section_file(const char* file_name)
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


static mxml_node_t * get_toc_preview_section_xml_instance(const char * file_name)
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


static void test_empty_afs_toc_preview_section(afs_toc_preview_section * toc_preview_section)
{
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(toc_preview_section->id == 0);
    BOXING_ASSERT(toc_preview_section->x == 0);
    BOXING_ASSERT(toc_preview_section->y == 0);
    BOXING_ASSERT(toc_preview_section->width == 0);
    BOXING_ASSERT(toc_preview_section->height == 0);
    BOXING_ASSERT(toc_preview_section->rotation == 0);
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


static void test_equals_toc_preview_section(afs_toc_preview_section * toc_preview_section1, afs_toc_preview_section * toc_preview_section2, DBOOL expected_result)
{
    DBOOL result = afs_toc_preview_section_equal(toc_preview_section2, toc_preview_section1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_preview_section_equal(toc_preview_section1, toc_preview_section2);
    BOXING_ASSERT(result == expected_result);
}


static void test_is_overlap_toc_preview_section(afs_toc_preview_section * toc_preview_section1, afs_toc_preview_section * toc_preview_section2, DBOOL expected_result)
{
    DBOOL result = afs_toc_preview_section_is_overlap(toc_preview_section2, toc_preview_section1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_preview_section_is_overlap(toc_preview_section1, toc_preview_section2);
    BOXING_ASSERT(result == expected_result);
}


// TEST TOC PREVIEW SECTION FUNCTIONS
//

// Test function afs_toc_preview_section_create test1
BOXING_START_TEST(afs_toc_preview_section_create_test1)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    test_empty_afs_toc_preview_section(toc_preview_section);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_create2 test1 with zero initial values
BOXING_START_TEST(afs_toc_preview_section_create2_test1)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(0, 0, 0, 0, 0, 0);

    test_empty_afs_toc_preview_section(toc_preview_section);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_create2 test2 with normal initial values
BOXING_START_TEST(afs_toc_preview_section_create2_test2)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);

    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 480, 90);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_init test with afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_init_test1)
{
    afs_toc_preview_section * toc_preview_section = NULL;

    afs_toc_preview_section_init(toc_preview_section, 1, 10, 10, 640, 480, 90);

    BOXING_ASSERT(toc_preview_section == NULL);
}
END_TEST


// Test function afs_toc_preview_section_init test with normal initialization data
BOXING_START_TEST(afs_toc_preview_section_init_test2)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    test_empty_afs_toc_preview_section(toc_preview_section);

    afs_toc_preview_section_init(toc_preview_section, 1, 10, 10, 640, 480, 90);

    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 480, 90);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_init test with empty initialization data
BOXING_START_TEST(afs_toc_preview_section_init_test3)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);

    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 480, 90);

    afs_toc_preview_section_init(toc_preview_section, 0, 0, 0, 0, 0, 0);

    test_empty_afs_toc_preview_section(toc_preview_section);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_clone test with input pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_clone_test1)
{
    afs_toc_preview_section * toc_preview_section1 = NULL;

    BOXING_ASSERT(toc_preview_section1 == NULL);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_clone(toc_preview_section1);

    BOXING_ASSERT(toc_preview_section2 == NULL);
}
END_TEST


// Test function afs_toc_preview_section_clone test with empty input afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_clone_test2)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create();

    test_empty_afs_toc_preview_section(toc_preview_section1);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_clone(toc_preview_section1);

    test_empty_afs_toc_preview_section(toc_preview_section2);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_clone test with input afs_toc_preview_section instance with valid values
BOXING_START_TEST(afs_toc_preview_section_clone_test3)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);

    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 640, 480, 90);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_clone(toc_preview_section1);

    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 10, 640, 480, 90);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_clone test with input afs_toc_preview_section instance with not valid values
BOXING_START_TEST(afs_toc_preview_section_clone_test4)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 0, 0, 12);

    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 0, 0, 12);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_clone(toc_preview_section1);

    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 10, 0, 0, 12);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_get_new_reference test with input pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_get_new_reference_test1)
{
    afs_toc_preview_section * toc_preview_section1 = NULL;

    BOXING_ASSERT(toc_preview_section1 == NULL);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_get_new_reference(toc_preview_section1);

    BOXING_ASSERT(toc_preview_section2 == NULL);

    toc_preview_section1 = afs_toc_preview_section_create();

    test_empty_afs_toc_preview_section(toc_preview_section1);
    BOXING_ASSERT(toc_preview_section1->reference_count == 1);
    BOXING_ASSERT(toc_preview_section2 == NULL);

    afs_toc_preview_section_free(toc_preview_section1);
}
END_TEST


// Test function afs_toc_preview_section_get_new_reference test with empty input afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_get_new_reference_test2)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create();

    test_empty_afs_toc_preview_section(toc_preview_section1);
    BOXING_ASSERT(toc_preview_section1->reference_count == 1);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_get_new_reference(toc_preview_section1);

    test_empty_afs_toc_preview_section(toc_preview_section2);
    BOXING_ASSERT(toc_preview_section1->reference_count == 2);
    BOXING_ASSERT(toc_preview_section2->reference_count == 2);
    BOXING_ASSERT(toc_preview_section1 == toc_preview_section2);

    toc_preview_section1->height = 99;

    BOXING_ASSERT(toc_preview_section1->height == 99);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section1);
}
END_TEST


// Test function afs_toc_preview_section_get_new_reference test with input afs_toc_preview_section instance with valid values
BOXING_START_TEST(afs_toc_preview_section_get_new_reference_test3)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);

    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 640, 480, 90);
    BOXING_ASSERT(toc_preview_section1->reference_count == 1);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_get_new_reference(toc_preview_section1);

    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 10, 640, 480, 90);
    BOXING_ASSERT(toc_preview_section1->reference_count == 2);
    BOXING_ASSERT(toc_preview_section2->reference_count == 2);
    BOXING_ASSERT(toc_preview_section1 == toc_preview_section2);

    toc_preview_section1->height = 99;

    BOXING_ASSERT(toc_preview_section1->height == 99);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section1);
}
END_TEST


// Test function afs_toc_preview_section_get_new_reference test with input afs_toc_preview_section instance with not valid values
BOXING_START_TEST(afs_toc_preview_section_get_new_reference_test4)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 0, 0, 12);

    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 0, 0, 12);
    BOXING_ASSERT(toc_preview_section1->reference_count == 1);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_get_new_reference(toc_preview_section1);

    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 10, 0, 0, 12);
    BOXING_ASSERT(toc_preview_section1->reference_count == 2);
    BOXING_ASSERT(toc_preview_section2->reference_count == 2);
    BOXING_ASSERT(toc_preview_section1 == toc_preview_section2);

    toc_preview_section1->height = 99;

    BOXING_ASSERT(toc_preview_section1->height == 99);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section1);
}
END_TEST


// Test function afs_toc_preview_section_equal test with input afs_toc_preview_section pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_section_equal_test1)
{
    afs_toc_preview_section * toc_preview_section1 = NULL;
    afs_toc_preview_section * toc_preview_section2 = NULL;

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DTRUE);
}
END_TEST

// Test function afs_toc_preview_section_equal test with input afs_toc_preview_section instances with zero values 
BOXING_START_TEST(afs_toc_preview_section_equal_test2)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create();
    test_empty_afs_toc_preview_section(toc_preview_section1);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create();
    test_empty_afs_toc_preview_section(toc_preview_section2);

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DTRUE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_equal test with input afs_toc_preview_section instances with valid values 
BOXING_START_TEST(afs_toc_preview_section_equal_test3)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 640, 480, 90);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 10, 640, 480, 90);

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DTRUE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_equal test with input afs_toc_preview_section instances with not valid values 
BOXING_START_TEST(afs_toc_preview_section_equal_test4)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 0, 0, 11);
    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 0, 0, 11);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 10, 10, 0, 0, 11);
    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 10, 0, 0, 11);

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DTRUE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_equal test with empty input afs_toc_preview_section instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_preview_section_equal_test5)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create();
    test_empty_afs_toc_preview_section(toc_preview_section1);

    afs_toc_preview_section * toc_preview_section2 = NULL;

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
}
END_TEST


// Test function afs_toc_preview_section_equal test with normal input afs_toc_preview_section instance and pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_equal_test6)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 640, 480, 90);

    afs_toc_preview_section * toc_preview_section2 = NULL;

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
}
END_TEST


// Test function afs_toc_preview_section_equal test with normal afs_toc_preview_section instance and afs_toc_preview_section instance with zero values
BOXING_START_TEST(afs_toc_preview_section_equal_test7)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 640, 480, 90);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create();
    test_empty_afs_toc_preview_section(toc_preview_section2);

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_equal test with afs_toc_preview_section instances with different id values
BOXING_START_TEST(afs_toc_preview_section_equal_test8)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(0, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section1, 0, 10, 10, 640, 480, 90);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 10, 640, 480, 90);

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_equal test with afs_toc_preview_section instances with different x values
BOXING_START_TEST(afs_toc_preview_section_equal_test9)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 640, 480, 90);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 20, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 20, 10, 640, 480, 90);

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_equal test with afs_toc_preview_section instances with different y values
BOXING_START_TEST(afs_toc_preview_section_equal_test10)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 640, 480, 90);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 10, 20, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 20, 640, 480, 90);

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_equal test with afs_toc_preview_section instances with different width values
BOXING_START_TEST(afs_toc_preview_section_equal_test11)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 1080, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 1080, 480, 90);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 10, 640, 480, 90);

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_equal test with afs_toc_preview_section instances with different height values
BOXING_START_TEST(afs_toc_preview_section_equal_test12)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 960, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 640, 960, 90);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 10, 640, 480, 90);

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_equal test with afs_toc_preview_section instances with different rotate values
BOXING_START_TEST(afs_toc_preview_section_equal_test13)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);
    test_not_empty_afs_toc_preview_section(toc_preview_section1, 1, 10, 10, 640, 480, 270);

    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);
    test_not_empty_afs_toc_preview_section(toc_preview_section2, 1, 10, 10, 640, 480, 90);

    test_equals_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_is_valid with input afs_toc_preview_section pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_section_is_valid_test1)
{
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_is_valid(toc_preview_section);

    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_section_is_valid with empty afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_is_valid_test2)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_is_valid(toc_preview_section);

    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_is_valid with invalid rotation value 
BOXING_START_TEST(afs_toc_preview_section_is_valid_test3)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 27);
    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 480, 27);

    DBOOL result = afs_toc_preview_section_is_valid(toc_preview_section);

    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_is_valid with width value equal to zero 
BOXING_START_TEST(afs_toc_preview_section_is_valid_test4)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 0, 480, 270);
    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 0, 480, 270);

    DBOOL result = afs_toc_preview_section_is_valid(toc_preview_section);

    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_is_valid with height value equal to zero 
BOXING_START_TEST(afs_toc_preview_section_is_valid_test5)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 0, 270);
    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 0, 270);

    DBOOL result = afs_toc_preview_section_is_valid(toc_preview_section);

    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_is_valid with different valid rotation value 
BOXING_START_TEST(afs_toc_preview_section_is_valid_test6)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 0);
    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 480, 0);

    DBOOL result = afs_toc_preview_section_is_valid(toc_preview_section);
    BOXING_ASSERT(result == DTRUE);

    toc_preview_section->rotation = 90;
    result = afs_toc_preview_section_is_valid(toc_preview_section);
    BOXING_ASSERT(result == DTRUE);

    toc_preview_section->rotation = 180;
    result = afs_toc_preview_section_is_valid(toc_preview_section);
    BOXING_ASSERT(result == DTRUE);

    toc_preview_section->rotation = 270;
    result = afs_toc_preview_section_is_valid(toc_preview_section);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_is_valid with different valid data 
BOXING_START_TEST(afs_toc_preview_section_is_valid_test7)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(0, 0, 0, 640, 480, 0);
    test_not_empty_afs_toc_preview_section(toc_preview_section, 0, 0, 0, 640, 480, 0);

    DBOOL result = afs_toc_preview_section_is_valid(toc_preview_section);
    BOXING_ASSERT(result == DTRUE);

    toc_preview_section->id = 3;
    result = afs_toc_preview_section_is_valid(toc_preview_section);
    BOXING_ASSERT(result == DTRUE);

    toc_preview_section->x = 18;
    result = afs_toc_preview_section_is_valid(toc_preview_section);
    BOXING_ASSERT(result == DTRUE);

    toc_preview_section->y = 27;
    result = afs_toc_preview_section_is_valid(toc_preview_section);
    BOXING_ASSERT(result == DTRUE);

    toc_preview_section->width = 1080;
    result = afs_toc_preview_section_is_valid(toc_preview_section);
    BOXING_ASSERT(result == DTRUE);

    toc_preview_section->height = 920;
    result = afs_toc_preview_section_is_valid(toc_preview_section);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_width with input afs_toc_preview_section pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_section_content_width_test1)
{
    afs_toc_preview_section * toc_preview_section = NULL;

    unsigned int result = afs_toc_preview_section_content_width(toc_preview_section);

    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == 0);
}
END_TEST


// Test function afs_toc_preview_section_content_width with input afs_toc_preview_section instance with zero values
BOXING_START_TEST(afs_toc_preview_section_content_width_test2)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    unsigned int result = afs_toc_preview_section_content_width(toc_preview_section);

    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_width with input afs_toc_preview_section instance with different width values and rotation equal to 0
BOXING_START_TEST(afs_toc_preview_section_content_width_test3)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 0);

    unsigned int result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 640);

    toc_preview_section->width = 1080;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 1080);

    toc_preview_section->width = 0;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_width with input afs_toc_preview_section instance with different width values and rotation equal to 90
BOXING_START_TEST(afs_toc_preview_section_content_width_test4)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);

    unsigned int result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 480);

    toc_preview_section->height = 920;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 920);

    toc_preview_section->height = 0;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_width with input afs_toc_preview_section instance with different width values and rotation equal to 180
BOXING_START_TEST(afs_toc_preview_section_content_width_test5)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 180);

    unsigned int result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 640);

    toc_preview_section->width = 1080;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 1080);

    toc_preview_section->width = 0;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_width with input afs_toc_preview_section instance with different width values and rotation equal to 270
BOXING_START_TEST(afs_toc_preview_section_content_width_test6)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);

    unsigned int result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 480);

    toc_preview_section->height = 920;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 920);

    toc_preview_section->height = 0;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_width with not valid rotation value
BOXING_START_TEST(afs_toc_preview_section_content_width_test7)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 18);

    unsigned int result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 0);

    toc_preview_section->rotation = 103;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 0);

    toc_preview_section->rotation = 196;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 0);

    toc_preview_section->rotation = 301;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_height with input afs_toc_preview_section pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_section_content_height_test1)
{
    afs_toc_preview_section * toc_preview_section = NULL;

    unsigned int result = afs_toc_preview_section_content_height(toc_preview_section);

    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == 0);
}
END_TEST


// Test function afs_toc_preview_section_content_height with input afs_toc_preview_section instance with zero values
BOXING_START_TEST(afs_toc_preview_section_content_height_test2)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    unsigned int result = afs_toc_preview_section_content_height(toc_preview_section);

    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_height with input afs_toc_preview_section instance with different height values and rotation equal to 0
BOXING_START_TEST(afs_toc_preview_section_content_height_test3)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 0);

    unsigned int result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 480);

    toc_preview_section->height = 920;
    result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 920);

    toc_preview_section->height = 0;
    result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_height with input afs_toc_preview_section instance with different height values and rotation equal to 90
BOXING_START_TEST(afs_toc_preview_section_content_height_test4)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 90);

    unsigned int result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 640);

    toc_preview_section->width = 1080;
    result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 1080);

    toc_preview_section->width = 0;
    result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_height with input afs_toc_preview_section instance with different height values and rotation equal to 180
BOXING_START_TEST(afs_toc_preview_section_content_height_test5)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 180);

    unsigned int result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 480);

    toc_preview_section->height = 920;
    result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 920);

    toc_preview_section->height = 0;
    result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_height with input afs_toc_preview_section instance with different height values and rotation equal to 270
BOXING_START_TEST(afs_toc_preview_section_content_height_test6)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);

    unsigned int result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 640);

    toc_preview_section->width = 1080;
    result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 1080);

    toc_preview_section->width = 0;
    result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_content_height with input afs_toc_preview_section instance with not valid rotation value
BOXING_START_TEST(afs_toc_preview_section_content_height_test7)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 18);

    unsigned int result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 0);

    toc_preview_section->rotation = 103;
    result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 0);

    toc_preview_section->rotation = 196;
    result = afs_toc_preview_section_content_height(toc_preview_section);
    BOXING_ASSERT(result == 0);

    toc_preview_section->rotation = 301;
    result = afs_toc_preview_section_content_width(toc_preview_section);
    BOXING_ASSERT(result == 0);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_is_overlap test with input afs_toc_preview_section pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_section_is_overlap_test1)
{
    afs_toc_preview_section * toc_preview_section1 = NULL;
    afs_toc_preview_section * toc_preview_section2 = NULL;

    test_is_overlap_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);
}
END_TEST


// Test function afs_toc_preview_section_is_overlap test with input afs_toc_preview_section pointers equal to NULL and empy afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_is_overlap_test2)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create();
    afs_toc_preview_section * toc_preview_section2 = NULL;

    test_is_overlap_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
}
END_TEST


// Test function afs_toc_preview_section_is_overlap test with input afs_toc_preview_section pointers equal to NULL and not valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_is_overlap_test3)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 0, 480, 270);
    afs_toc_preview_section * toc_preview_section2 = NULL;

    test_is_overlap_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
}
END_TEST


// Test function afs_toc_preview_section_is_overlap test with input afs_toc_preview_section pointers equal to NULL and valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_is_overlap_test4)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);
    afs_toc_preview_section * toc_preview_section2 = NULL;

    test_is_overlap_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
}
END_TEST


// Test function afs_toc_preview_section_is_overlap test with input empty afs_toc_preview_section instance and valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_is_overlap_test5)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);
    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create();

    test_is_overlap_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_is_overlap test with input not valid afs_toc_preview_section instance and valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_is_overlap_test6)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);
    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 10, 10, 0, 0, 270);

    test_is_overlap_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_is_overlap test with input valid afs_toc_preview_section instances and not overlap data
BOXING_START_TEST(afs_toc_preview_section_is_overlap_test7)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(0, 0, 0, 100, 100, 0);
    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 110, 110, 100, 100, 0);

    test_is_overlap_toc_preview_section(toc_preview_section1, toc_preview_section2, DFALSE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_is_overlap test with input valid afs_toc_preview_section instances and overlap data
BOXING_START_TEST(afs_toc_preview_section_is_overlap_test8)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(0, 0, 0, 100, 100, 0);
    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 0, 99, 100, 100, 0);

    test_is_overlap_toc_preview_section(toc_preview_section1, toc_preview_section2, DTRUE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_is_overlap test with input valid afs_toc_preview_section instances and overlap data
BOXING_START_TEST(afs_toc_preview_section_is_overlap_test9)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(0, 0, 0, 100, 100, 0);
    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 50, 50, 100, 100, 0);

    test_is_overlap_toc_preview_section(toc_preview_section1, toc_preview_section2, DTRUE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_is_overlap test with one input afs_toc_preview_section inside other
BOXING_START_TEST(afs_toc_preview_section_is_overlap_test10)
{
    afs_toc_preview_section * toc_preview_section1 = afs_toc_preview_section_create2(0, 0, 0, 100, 100, 0);
    afs_toc_preview_section * toc_preview_section2 = afs_toc_preview_section_create2(1, 50, 50, 10, 10, 0);

    test_is_overlap_toc_preview_section(toc_preview_section1, toc_preview_section2, DTRUE);

    afs_toc_preview_section_free(toc_preview_section1);
    afs_toc_preview_section_free(toc_preview_section2);
}
END_TEST


// Test function afs_toc_preview_section_save_file with file_name pointer equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_save_file(toc_preview_section, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_section_save_file with file_name pointer equal to NULL and afs_toc_preview_section pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_section_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);

    DBOOL result = afs_toc_preview_section_save_file(toc_preview_section, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_save_file with file_name pointer not equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_save_file_test3)
{
    const char * file_name = "afs_toc_preview_section_save_file_test.xml";
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_save_file(toc_preview_section, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_section_save_file with empty afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_save_file_test4)
{
    const char * file_name = "afs_toc_preview_section_save_file_empty_test.xml";
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_save_file(toc_preview_section, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_save_file with not valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_save_file_test5)
{
    const char * file_name = "afs_toc_preview_section_save_file_empty_test.xml";
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 0, 480, 270);

    DBOOL result = afs_toc_preview_section_save_file(toc_preview_section, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_save_file with initialized toc_preview_section instance, in compact format
BOXING_START_TEST(afs_toc_preview_section_save_file_test6)
{
    const char * file_name = "afs_toc_preview_section_save_file_compact_test.xml";

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);

    DBOOL result = afs_toc_preview_section_save_file(toc_preview_section, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_save_file with initialized toc_preview_section instance, in not compact format
BOXING_START_TEST(afs_toc_preview_section_save_file_test7)
{
    const char * file_name = "afs_toc_preview_section_save_file_not_compact_test.xml";

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);

    DBOOL result = afs_toc_preview_section_save_file(toc_preview_section, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_save_string with afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_save_string_test1)
{
    afs_toc_preview_section * toc_preview_section = NULL;

    char * result = afs_toc_preview_section_save_string(toc_preview_section, DTRUE);

    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_preview_section_save_string with empty toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_save_string_test2)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    char * result = afs_toc_preview_section_save_string(toc_preview_section, DTRUE);

    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_save_string with not valid toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_save_string_test3)
{
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 0, 480, 270);

    char * result = afs_toc_preview_section_save_string(toc_preview_section, DTRUE);

    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_save_string with initialized toc_preview_section instance, in compact format
BOXING_START_TEST(afs_toc_preview_section_save_string_test4)
{
    char * test_string = read_xml_toc_preview_section_file("afs_toc_preview_section_save_file_compact_test.xml");

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);

    char * result = afs_toc_preview_section_save_string(toc_preview_section, DTRUE);

    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_preview_section_free(toc_preview_section);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_preview_section_save_string with initialized afs_toc_preview_section instance, in not compact format
BOXING_START_TEST(afs_toc_preview_section_save_string_test5)
{
    char * test_string = read_xml_toc_preview_section_file("afs_toc_preview_section_save_file_not_compact_test.xml");

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);

    char * result = afs_toc_preview_section_save_string(toc_preview_section, DFALSE);

    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_preview_section_free(toc_preview_section);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_preview_section_save_xml with mxml_node_t pointer equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_save_xml(toc_preview_section, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_section_save_xml with mxml_node_t pointer equal to NULL and afs_toc_preview_section pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_section_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);

    DBOOL result = afs_toc_preview_section_save_xml(toc_preview_section, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_save_xml(toc_preview_section, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_section_save_xml with empty afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_save_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_save_xml(toc_preview_section, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_section_save_xml with not valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 0, 480, 270);

    DBOOL result = afs_toc_preview_section_save_xml(toc_preview_section, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_section_save_xml with valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create2(1, 10, 10, 640, 480, 270);

    DBOOL result = afs_toc_preview_section_save_xml(toc_preview_section, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_section_free(toc_preview_section);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_section_load_file with file_name pointer equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_load_file(toc_preview_section, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_section_load_file with file_name pointer equal to NULL and afs_toc_preview_section pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_section_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_file(toc_preview_section, file_name);

    test_empty_afs_toc_preview_section(toc_preview_section);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_load_file with file_name pointer not equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_load_file_test3)
{
    const char * file_name = "afs_toc_preview_section_save_file_compact_test.xml";
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_load_file(toc_preview_section, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_section_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_preview_section_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_file(toc_preview_section, file_name);

    test_empty_afs_toc_preview_section(toc_preview_section);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_load_file with initialized afs_toc_preview_section instance, in compact format
BOXING_START_TEST(afs_toc_preview_section_load_file_test5)
{
    const char * file_name = "afs_toc_preview_section_save_file_compact_test.xml";
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_file(toc_preview_section, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 480, 270);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_load_file with initialized afs_toc_preview_section instance, in not compact format
BOXING_START_TEST(afs_toc_preview_section_load_file_test6)
{
    const char * file_name = "afs_toc_preview_section_save_file_not_compact_test.xml";
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_file(toc_preview_section, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 480, 270);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_load_string with input string pointer equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_load_string(toc_preview_section, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_section_load_string with input string pointer equal to NULL and afs_toc_preview_section pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_section_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_string(toc_preview_section, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_load_string with input string pointer not equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_load_string_test3)
{
    char * input_string = read_xml_toc_preview_section_file("afs_toc_preview_section_save_file_not_compact_test.xml");
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_load_string(toc_preview_section, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_preview_section_load_string with empty input string
BOXING_START_TEST(afs_toc_preview_section_load_string_test4)
{
    char * input_string = "";
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_string(toc_preview_section, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_load_string with input string for initialized afs_toc_preview_section instance, in compact format
BOXING_START_TEST(afs_toc_preview_section_load_string_test5)
{
    char * input_string = read_xml_toc_preview_section_file("afs_toc_preview_section_save_file_compact_test.xml");

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_string(toc_preview_section, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 480, 270);

    afs_toc_preview_section_free(toc_preview_section);
    free(input_string);
}
END_TEST


// Test function afs_toc_preview_section_load_string with input string for initialized afs_toc_preview_section instance, in not compact format
BOXING_START_TEST(afs_toc_preview_section_load_string_test6)
{
    char * input_string = read_xml_toc_preview_section_file("afs_toc_preview_section_save_file_not_compact_test.xml");
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_string(toc_preview_section, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 480, 270);

    afs_toc_preview_section_free(toc_preview_section);
    free(input_string);
}
END_TEST


// Test function afs_toc_preview_section_load_xml with xml instance pointer equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_load_xml(toc_preview_section, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_section_load_xml with xml instance pointer equal to NULL and afs_toc_preview_section pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_section_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_xml(toc_preview_section, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
}
END_TEST


// Test function afs_toc_preview_section_load_xml with xml instance pointer not equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_section_load_xml_test3)
{
    mxml_node_t *tree = get_toc_preview_section_xml_instance("afs_toc_preview_section_save_file_compact_test.xml");
    afs_toc_preview_section * toc_preview_section = NULL;

    DBOOL result = afs_toc_preview_section_load_xml(toc_preview_section, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_section_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_preview_section_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_xml(toc_preview_section, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(toc_preview_section);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_section_load_xml with xml instance for initialized afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_section_load_xml_test5)
{
    mxml_node_t * tree = get_toc_preview_section_xml_instance("afs_toc_preview_section_save_file_compact_test.xml");

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_section_load_xml(toc_preview_section, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_section != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_section(toc_preview_section, 1, 10, 10, 640, 480, 270);

    afs_toc_preview_section_free(toc_preview_section);
    mxmlDelete(tree);
}
END_TEST


Suite * tocpreviewsection_tests(void)
{
    TCase * tc_tocpreviewsection_functions_tests = tcase_create("tocpreviewsection_functions_tests");

    // Test function afs_toc_preview_section_create
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_create_test1);
    // Test function afs_toc_preview_section_create2
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_create2_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_create2_test2);
    // Test function afs_toc_preview_section_init
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_init_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_init_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_init_test3);
    // Test function afs_toc_preview_section_clone
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_clone_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_clone_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_clone_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_clone_test4);
    // Test function afs_toc_preview_section_get_new_reference
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_get_new_reference_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_get_new_reference_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_get_new_reference_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_get_new_reference_test4);
    // Test function afs_toc_preview_section_equal
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test5);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test6);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test7);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test8);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test9);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test10);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test11);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test12);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_equal_test13);
    // Test function afs_toc_preview_section_is_valid
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_valid_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_valid_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_valid_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_valid_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_valid_test5);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_valid_test6);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_valid_test7);
    // Test function afs_toc_preview_section_content_width
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_width_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_width_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_width_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_width_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_width_test5);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_width_test6);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_width_test7);
    // Test function afs_toc_preview_section_content_height
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_height_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_height_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_height_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_height_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_height_test5);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_height_test6);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_content_height_test7);
    // Test function afs_toc_preview_section_is_overlap
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_overlap_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_overlap_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_overlap_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_overlap_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_overlap_test5);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_overlap_test6);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_overlap_test7);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_overlap_test8);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_overlap_test9);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_is_overlap_test10);
    // Test function afs_toc_preview_section_save_file
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_file_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_file_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_file_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_file_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_file_test5);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_file_test6);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_file_test7);
    // Test function afs_toc_preview_section_save_string
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_string_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_string_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_string_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_string_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_string_test5);
    // Test function afs_toc_preview_section_save_xml
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_xml_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_xml_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_xml_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_xml_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_xml_test5);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_save_xml_test6);
    // Test function afs_toc_preview_section_load_file
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_file_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_file_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_file_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_file_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_file_test5);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_file_test6);
    // Test function afs_toc_preview_section_load_string
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_string_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_string_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_string_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_string_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_string_test5);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_string_test6);
    // Test function afs_toc_preview_section_load_xml
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_xml_test1);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_xml_test2);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_xml_test3);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_xml_test4);
    tcase_add_test(tc_tocpreviewsection_functions_tests, afs_toc_preview_section_load_xml_test5);

    Suite * s = suite_create("tocpreviewsection_test_util");
    suite_add_tcase(s, tc_tocpreviewsection_functions_tests);

    return s;
}
