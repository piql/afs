#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "tocmetadata_c.h"
#include "xmlutils.h"
#include "boxing/platform/types.h"
#include "boxing/utils.h"
#include "mxml.h"
#include "platform/io.h"

#include <string.h>

static void test_null_afs_toc_metadata(afs_toc_metadata * toc_metadata)
{
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(toc_metadata->sources == NULL);
}


static void test_empty_afs_toc_metadata(afs_toc_metadata * toc_metadata)
{
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(toc_metadata->sources != NULL);
    BOXING_ASSERT(toc_metadata->sources->size == 0);
}


static void test_not_empty_afs_toc_metadata(afs_toc_metadata * toc_metadata, unsigned int sources_count)
{
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(toc_metadata->sources != NULL);
    BOXING_ASSERT(toc_metadata->sources->size == sources_count);

    for (unsigned int i = 0; i < sources_count; i++)
    {
        char tags_string[255]; // Temporary string
        sprintf(tags_string, "tag%d;tag%d;tag%d", i * 3, i * 3 + 1, i * 3 + 2);

        afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", i + 1, "some xml data", tags_string, i + 2);
        BOXING_ASSERT(afs_toc_metadata_source_equal(GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, i), toc_metadata_source) == DTRUE);
        afs_toc_metadata_source_free(toc_metadata_source);
    }
}


static afs_toc_metadata_source * get_afs_toc_metadata_source_instance(unsigned int number)
{
    char tags_string[255]; // Temporary string
    sprintf(tags_string, "tag%d;tag%d;tag%d", number * 3, number * 3 + 1, number * 3 + 2);

    return afs_toc_metadata_source_create7("afs/metadata-csv", number + 1, "some xml data", tags_string, number + 2);
}


static afs_toc_metadata_sources * get_filled_sources_vector(unsigned int sources_count)
{
    afs_toc_metadata_sources* sources = gvector_create_pointers(sources_count);

    for (unsigned int i = 0; i < sources_count; i++)
    {
        GVECTORN(sources, afs_toc_metadata_source *, i) = get_afs_toc_metadata_source_instance(i);
    }

    return sources;
}


static afs_toc_metadata* get_afs_toc_metadata_instance(unsigned int sources_count)
{
    afs_toc_metadata_sources * sources = get_filled_sources_vector(sources_count);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);

    return toc_metadata;
}


static char * read_xml_toc_metadata_file(const char* file_name)
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


static mxml_node_t * get_toc_metadata_xml_instance(const char * file_name)
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


// TEST TOC METADATA FUNCTIONS
//


// Test function afs_toc_metadata_create
BOXING_START_TEST(afs_toc_metadata_create_test1)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_create2 test1 with sources pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_create2_test1)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(NULL);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_create2 test2 with empty sources vector
BOXING_START_TEST(afs_toc_metadata_create2_test2)
{
    afs_toc_metadata_sources* sources = gvector_create_pointers(0);

    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);

    test_empty_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_create2 test3 with filled sources vector
BOXING_START_TEST(afs_toc_metadata_create2_test3)
{
    afs_toc_metadata_sources* sources = get_filled_sources_vector(1);

    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);

    test_not_empty_afs_toc_metadata(toc_metadata, 1);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_create3 test1 with XML node pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_create3_test1)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create3(NULL);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_create3 test2 with empty XML node
BOXING_START_TEST(afs_toc_metadata_create3_test2)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_metadata * toc_metadata = afs_toc_metadata_create3(tree);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_create3 test3 with correct XML node
BOXING_START_TEST(afs_toc_metadata_create3_test3)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(3);

    mxml_node_t *tree = mxmlNewXML("1.0");
    DBOOL result = afs_toc_metadata_save_xml(toc_metadata, tree);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata(toc_metadata, 3);

    afs_toc_metadata_free(toc_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_init test with afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_init_test1)
{
    afs_toc_metadata * toc_metadata = NULL;

    afs_toc_metadata_init(toc_metadata);

    BOXING_ASSERT(toc_metadata == NULL);
}
END_TEST


// Test function afs_toc_metadata_init test with allocated afs_toc_metadata pointer
BOXING_START_TEST(afs_toc_metadata_init_test2)
{
    afs_toc_metadata * toc_metadata = malloc(sizeof(afs_toc_metadata));

    afs_toc_metadata_init(toc_metadata);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_init2 test with afs_toc_metadata pointer equal to NULL and sources pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_init2_test1)
{
    afs_toc_metadata * toc_metadata = NULL;
    afs_toc_metadata_sources* sources = NULL;

    afs_toc_metadata_init2(toc_metadata, sources);

    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(sources == NULL);
}
END_TEST


// Test function afs_toc_metadata_init2 test with afs_toc_metadata pointer equal to NULL and sources pointer not equa to NULL
BOXING_START_TEST(afs_toc_metadata_init2_test2)
{
    afs_toc_metadata * toc_metadata = NULL;
    afs_toc_metadata_sources* sources = gvector_create_pointers(0);

    afs_toc_metadata_init2(toc_metadata, sources);

    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(sources != NULL);
    BOXING_ASSERT(sources->size == 0);

    gvector_free(sources);
}
END_TEST


// Test function afs_toc_metadata_init2 test with afs_toc_metadata pointer not equal to NULL and sources pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_init2_test3)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();
    afs_toc_metadata_sources* sources = NULL;

    afs_toc_metadata_init2(toc_metadata, sources);

    test_null_afs_toc_metadata(toc_metadata);

    BOXING_ASSERT(sources == NULL);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_init2 test with afs_toc_metadata pointer not equal to NULL and empty sources vector
BOXING_START_TEST(afs_toc_metadata_init2_test4)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();
    afs_toc_metadata_sources* sources = gvector_create_pointers(0);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_init2(toc_metadata, sources);

    test_empty_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_init2 test with afs_toc_metadata pointer not equal to NULL and filled sources vector
BOXING_START_TEST(afs_toc_metadata_init2_test5)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();
    afs_toc_metadata_sources* sources = get_filled_sources_vector(4);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_init2(toc_metadata, sources);

    test_not_empty_afs_toc_metadata(toc_metadata, 4);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_clone test with input pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_clone_test1)
{
    afs_toc_metadata * toc_metadata1 = NULL;

    BOXING_ASSERT(toc_metadata1 == NULL);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_clone(toc_metadata1);

    BOXING_ASSERT(toc_metadata2 == NULL);
}
END_TEST


// Test function afs_toc_metadata_clone test with empty input afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_clone_test2)
{
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create();

    test_null_afs_toc_metadata(toc_metadata1);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_clone(toc_metadata1);

    test_null_afs_toc_metadata(toc_metadata2);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_clone test with not empty input afs_toc_metadata instance with empty sources vector
BOXING_START_TEST(afs_toc_metadata_clone_test3)
{
    afs_toc_metadata_sources* sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources);

    test_empty_afs_toc_metadata(toc_metadata1);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_clone(toc_metadata1);

    test_empty_afs_toc_metadata(toc_metadata2);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_clone test with not empty input afs_toc_metadata instance with not empty sources vector
BOXING_START_TEST(afs_toc_metadata_clone_test4)
{
    afs_toc_metadata_sources* sources = get_filled_sources_vector(3);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources);

    test_not_empty_afs_toc_metadata(toc_metadata1, 3);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_clone(toc_metadata1);

    test_not_empty_afs_toc_metadata(toc_metadata2, 3);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_get_new_reference_test1)
{
    afs_toc_metadata * toc_metadata1 = NULL;

    BOXING_ASSERT(toc_metadata1 == NULL);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_get_new_reference(toc_metadata1);

    BOXING_ASSERT(toc_metadata2 == NULL);

    toc_metadata1 = afs_toc_metadata_create();

    test_null_afs_toc_metadata(toc_metadata1);
    BOXING_ASSERT(toc_metadata1->reference_count == 1);
    BOXING_ASSERT(toc_metadata2 == NULL);

    afs_toc_metadata_free(toc_metadata1);
}
END_TEST


// Test function afs_toc_metadata_get_new_reference with empty input afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_get_new_reference_test2)
{
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create();

    test_null_afs_toc_metadata(toc_metadata1);
    BOXING_ASSERT(toc_metadata1->reference_count == 1);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_get_new_reference(toc_metadata1);

    test_null_afs_toc_metadata(toc_metadata2);
    BOXING_ASSERT(toc_metadata1->reference_count == 2);
    BOXING_ASSERT(toc_metadata2->reference_count == 2);
    BOXING_ASSERT(toc_metadata1 == toc_metadata2);

    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(0);

    DBOOL result = afs_toc_metadata_add_source(toc_metadata1, source);

    BOXING_ASSERT(result == DTRUE);
    test_not_empty_afs_toc_metadata(toc_metadata2, 1);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata1);
}
END_TEST


// Test function afs_toc_metadata_get_new_reference with not empty input afs_toc_metadata instance with empty sources vector
BOXING_START_TEST(afs_toc_metadata_get_new_reference_test3)
{
    afs_toc_metadata_sources* sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources);

    test_empty_afs_toc_metadata(toc_metadata1);
    BOXING_ASSERT(toc_metadata1->reference_count == 1);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_get_new_reference(toc_metadata1);

    test_empty_afs_toc_metadata(toc_metadata2);
    BOXING_ASSERT(toc_metadata1->reference_count == 2);
    BOXING_ASSERT(toc_metadata2->reference_count == 2);
    BOXING_ASSERT(toc_metadata1 == toc_metadata2);

    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(0);

    DBOOL result = afs_toc_metadata_add_source(toc_metadata1, source);

    BOXING_ASSERT(result == DTRUE);
    test_not_empty_afs_toc_metadata(toc_metadata2, 1);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata1);
}
END_TEST


// Test function afs_toc_metadata_get_new_reference with not empty input afs_toc_metadata instance with not empty sources vector
BOXING_START_TEST(afs_toc_metadata_get_new_reference_test4)
{
    afs_toc_metadata_sources* sources = get_filled_sources_vector(3);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources);

    test_not_empty_afs_toc_metadata(toc_metadata1, 3);
    BOXING_ASSERT(toc_metadata1->reference_count == 1);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_get_new_reference(toc_metadata1);

    test_not_empty_afs_toc_metadata(toc_metadata2, 3);
    BOXING_ASSERT(toc_metadata1->reference_count == 2);
    BOXING_ASSERT(toc_metadata2->reference_count == 2);
    BOXING_ASSERT(toc_metadata1 == toc_metadata2);

    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(3);

    DBOOL result = afs_toc_metadata_add_source(toc_metadata1, source);

    BOXING_ASSERT(result == DTRUE);
    test_not_empty_afs_toc_metadata(toc_metadata2, 4);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata1);
}
END_TEST


// Test function afs_toc_metadata_equal test with input afs_toc_metadata pointers equal to NULL
BOXING_START_TEST(afs_toc_metadata_equal_test1)
{
    afs_toc_metadata * toc_metadata1 = NULL;
    afs_toc_metadata * toc_metadata2 = NULL;

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DTRUE);
}
END_TEST

// Test function afs_toc_metadata_equal test with empty input afs_toc_metadata instances 
BOXING_START_TEST(afs_toc_metadata_equal_test2)
{
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create();
    test_null_afs_toc_metadata(toc_metadata1);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_create();
    test_null_afs_toc_metadata(toc_metadata2);

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_equal test with not empty input afs_toc_metadata instances with empty sources vector
BOXING_START_TEST(afs_toc_metadata_equal_test3)
{
    afs_toc_metadata_sources* sources1 = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources1);
    test_empty_afs_toc_metadata(toc_metadata1);

    afs_toc_metadata_sources* sources2 = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_create2(sources2);
    test_empty_afs_toc_metadata(toc_metadata2);

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_equal test with not empty input afs_toc_metadata instances and sources vector with one pointer
BOXING_START_TEST(afs_toc_metadata_equal_test4)
{
    afs_toc_metadata_sources* sources1 = get_filled_sources_vector(1);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources1);
    test_not_empty_afs_toc_metadata(toc_metadata1, 1);

    afs_toc_metadata_sources* sources2 = get_filled_sources_vector(1);
    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_create2(sources2);
    test_not_empty_afs_toc_metadata(toc_metadata2, 1);

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_equal test with not empty input afs_toc_metadata instances and sources vector with three pointers
BOXING_START_TEST(afs_toc_metadata_equal_test5)
{
    afs_toc_metadata_sources* sources1 = get_filled_sources_vector(3);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources1);
    test_not_empty_afs_toc_metadata(toc_metadata1, 3);

    afs_toc_metadata_sources* sources2 = get_filled_sources_vector(3);
    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_create2(sources2);
    test_not_empty_afs_toc_metadata(toc_metadata2, 3);

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DTRUE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_equal test with empty input afs_toc_metadata instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_metadata_equal_test6)
{
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create();
    test_null_afs_toc_metadata(toc_metadata1);

    afs_toc_metadata * toc_metadata2 = NULL;

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata1);
}
END_TEST


// Test function afs_toc_metadata_equal test with input afs_toc_metadata instance equal to NULL and instance with empty sources vector 
BOXING_START_TEST(afs_toc_metadata_equal_test7)
{
    afs_toc_metadata_sources* sources1 = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources1);
    test_empty_afs_toc_metadata(toc_metadata1);

    afs_toc_metadata * toc_metadata2 = NULL;

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata1);
}
END_TEST


// Test function afs_toc_metadata_equal test with input afs_toc_metadata pointer equal to NULL and instance with filled sources vector 
BOXING_START_TEST(afs_toc_metadata_equal_test8)
{
    afs_toc_metadata_sources* sources1 = get_filled_sources_vector(4);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources1);
    test_not_empty_afs_toc_metadata(toc_metadata1, 4);

    afs_toc_metadata * toc_metadata2 = NULL;

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata1);
}
END_TEST


// Test function afs_toc_metadata_equal test with empty input afs_toc_metadata instance and instance with empty sources vector 
BOXING_START_TEST(afs_toc_metadata_equal_test9)
{
    afs_toc_metadata_sources* sources1 = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources1);
    test_empty_afs_toc_metadata(toc_metadata1);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_create();
    test_null_afs_toc_metadata(toc_metadata2);

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_equal test with empty input afs_toc_metadata instance and instance with filled sources vector 
BOXING_START_TEST(afs_toc_metadata_equal_test10)
{
    afs_toc_metadata_sources* sources1 = get_filled_sources_vector(4);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources1);
    test_not_empty_afs_toc_metadata(toc_metadata1, 4);

    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_create();
    test_null_afs_toc_metadata(toc_metadata2);

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_equal test with input afs_toc_metadata instance with empty sources vector and instance with filled sources vector 
BOXING_START_TEST(afs_toc_metadata_equal_test11)
{
    afs_toc_metadata_sources* sources1 = get_filled_sources_vector(4);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources1);
    test_not_empty_afs_toc_metadata(toc_metadata1, 4);

    afs_toc_metadata_sources* sources2 = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_create2(sources2);
    test_empty_afs_toc_metadata(toc_metadata2);

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_equal test with input afs_toc_metadata instances with different sources vector size 
BOXING_START_TEST(afs_toc_metadata_equal_test12)
{
    afs_toc_metadata_sources* sources1 = get_filled_sources_vector(4);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources1);
    test_not_empty_afs_toc_metadata(toc_metadata1, 4);

    afs_toc_metadata_sources* sources2 = get_filled_sources_vector(6);
    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_create2(sources2);
    test_not_empty_afs_toc_metadata(toc_metadata2, 6);

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_equal test with input afs_toc_metadata instances with different sources data 
BOXING_START_TEST(afs_toc_metadata_equal_test13)
{
    afs_toc_metadata_sources* sources1 = get_filled_sources_vector(2);
    afs_toc_metadata * toc_metadata1 = afs_toc_metadata_create2(sources1);
    test_not_empty_afs_toc_metadata(toc_metadata1, 2);

    afs_toc_metadata_sources* sources2 = get_filled_sources_vector(2);
    afs_toc_metadata * toc_metadata2 = afs_toc_metadata_create2(sources2);
    test_not_empty_afs_toc_metadata(toc_metadata2, 2);
    GVECTORN(toc_metadata2->sources, afs_toc_metadata_source *, 1)->id = 10;

    DBOOL result = afs_toc_metadata_equal(toc_metadata1, toc_metadata2);

    BOXING_ASSERT(result == DFALSE);

    result = afs_toc_metadata_equal(toc_metadata2, toc_metadata1);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata1);
    afs_toc_metadata_free(toc_metadata2);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata pointers equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_add_source_test1)
{
    afs_toc_metadata * toc_metadata = NULL;
    afs_toc_metadata_source * source = NULL;

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata pointers equal to NULL and valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_add_source_test2)
{
    afs_toc_metadata * toc_metadata = NULL;
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(1);

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata pointers equal to NULL and not valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_add_source_test3)
{
    afs_toc_metadata * toc_metadata = NULL;
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(1);
    source->file_id = -3;

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_metadata_add_source test with empty input afs_toc_metadata instance and afs_toc_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_metadata_add_source_test4)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();
    afs_toc_metadata_source * source = NULL;

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_add_source test with empty input afs_toc_metadata instance and empty afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_add_source_test5)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();
    afs_toc_metadata_source * source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_metadata_add_source test with empty input afs_toc_metadata instance and not valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_add_source_test6)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(1);
    source->file_id = -3;

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_metadata_add_source test with empty input afs_toc_metadata instance and valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_add_source_test7)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(0);

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata(toc_metadata, 1);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata instance with empty sources vector and afs_toc_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_metadata_add_source_test8)
{
    afs_toc_metadata_sources * sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);
    afs_toc_metadata_source * source = NULL;

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata instance with empty sources vector and empty afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_add_source_test9)
{
    afs_toc_metadata_sources * sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);
    afs_toc_metadata_source * source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata instance with empty sources vector and not valid afs_toc_metadata_source
BOXING_START_TEST(afs_toc_metadata_add_source_test10)
{
    afs_toc_metadata_sources * sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(1);
    source->file_id = -3;

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata instance with empty sources vector and valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_add_source_test11)
{
    afs_toc_metadata_sources * sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(0);

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata(toc_metadata, 1);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata instance with filled sources vector and afs_toc_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_metadata_add_source_test12)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(2);
    afs_toc_metadata_source * source = NULL;

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_metadata(toc_metadata, 2);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata instance with filled sources vector and empty afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_add_source_test13)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(2);
    afs_toc_metadata_source * source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_metadata(toc_metadata, 2);

    afs_toc_metadata_free(toc_metadata);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata instance with filled sources vector and not valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_add_source_test14)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(2);
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(2);
    source->file_id = -3;

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_metadata(toc_metadata, 2);

    afs_toc_metadata_free(toc_metadata);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_metadata_add_source test with input afs_toc_metadata instance with filled sources vector and valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_add_source_test15)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(2);
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(2);

    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata(toc_metadata, 3);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_get_source test with input afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_get_source_test1)
{
    afs_toc_metadata * toc_metadata = NULL;
    afs_toc_metadata_source * source = afs_toc_metadata_get_source(toc_metadata, 0);

    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(source == NULL);
}
END_TEST


// Test function afs_toc_metadata_get_source test with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_get_source_test2)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();
    afs_toc_metadata_source * source = afs_toc_metadata_get_source(toc_metadata, 0);

    test_null_afs_toc_metadata(toc_metadata);
    BOXING_ASSERT(source == NULL);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_get_source test with afs_toc_metadata instance with empty sources vector
BOXING_START_TEST(afs_toc_metadata_get_source_test3)
{
    afs_toc_metadata_sources * sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);

    afs_toc_metadata_source * source = afs_toc_metadata_get_source(toc_metadata, 0);

    test_empty_afs_toc_metadata(toc_metadata);
    BOXING_ASSERT(source == NULL);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_get_source test with not empty afs_toc_metadata instance and index out of range
BOXING_START_TEST(afs_toc_metadata_get_source_test4)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(2);

    afs_toc_metadata_source * source = afs_toc_metadata_get_source(toc_metadata, 2);

    test_not_empty_afs_toc_metadata(toc_metadata, 2);
    BOXING_ASSERT(source == NULL);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_get_source test with not empty afs_toc_metadata instance and index in range
BOXING_START_TEST(afs_toc_metadata_get_source_test5)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(6);

    for (unsigned int i = 0; i < toc_metadata->sources->size; i++)
    {
        afs_toc_metadata_source * source = afs_toc_metadata_get_source(toc_metadata, i);
        BOXING_ASSERT(afs_toc_metadata_source_equal(GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, i), source) == DTRUE);
    }

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_get_sources_count test with input afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_get_sources_count_test1)
{
    afs_toc_metadata * toc_metadata = NULL;
    unsigned int sources_count = afs_toc_metadata_get_sources_count(toc_metadata);

    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(sources_count == 0);
}
END_TEST


// Test function afs_toc_metadata_get_sources_count test with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_get_sources_count_test2)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();
    unsigned int sources_count = afs_toc_metadata_get_sources_count(toc_metadata);

    test_null_afs_toc_metadata(toc_metadata);
    BOXING_ASSERT(sources_count == 0);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_get_sources_count test with afs_toc_metadata instance with empty sources vector
BOXING_START_TEST(afs_toc_metadata_get_sources_count_test3)
{
    afs_toc_metadata_sources * sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);

    unsigned int sources_count = afs_toc_metadata_get_sources_count(toc_metadata);

    test_empty_afs_toc_metadata(toc_metadata);
    BOXING_ASSERT(sources_count == 0);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_get_sources_count test with afs_toc_metadata instance with filled sources vector
BOXING_START_TEST(afs_toc_metadata_get_sources_count_test4)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(3);

    unsigned int sources_count = afs_toc_metadata_get_sources_count(toc_metadata);

    test_not_empty_afs_toc_metadata(toc_metadata, 3);
    BOXING_ASSERT(sources_count == 3);

    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance(3);
    DBOOL result = afs_toc_metadata_add_source(toc_metadata, source);

    BOXING_ASSERT(result == DTRUE);
    test_not_empty_afs_toc_metadata(toc_metadata, 4);

    sources_count = afs_toc_metadata_get_sources_count(toc_metadata);
    
    BOXING_ASSERT(sources_count == 4);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_is_valid with input afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_is_valid_test1)
{
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_is_valid(toc_metadata);

    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_is_valid with empty input afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_is_valid_test2)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_is_valid(toc_metadata);

    test_null_afs_toc_metadata(toc_metadata);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_is_valid with input afs_toc_metadata instance with empty sources vector
BOXING_START_TEST(afs_toc_metadata_is_valid_test3)
{
    afs_toc_metadata_sources * sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);

    DBOOL result = afs_toc_metadata_is_valid(toc_metadata);

    test_empty_afs_toc_metadata(toc_metadata);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_is_valid with input afs_toc_metadata instance where sources vector filled not valid data
BOXING_START_TEST(afs_toc_metadata_is_valid_test4)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(3);

    test_not_empty_afs_toc_metadata(toc_metadata, 3);

    GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, 1)->file_id = -3;

    DBOOL result = afs_toc_metadata_is_valid(toc_metadata);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_is_valid with input afs_toc_metadata instance where sources vector filled valid data
BOXING_START_TEST(afs_toc_metadata_is_valid_test5)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(5);

    DBOOL result = afs_toc_metadata_is_valid(toc_metadata);

    test_not_empty_afs_toc_metadata(toc_metadata, 5);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_file with file_name pointer equal to NULL and afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_save_file(toc_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_save_file with file_name pointer equal to NULL and afs_toc_metadata pointer not equal to NULL
BOXING_START_TEST(afs_toc_metadata_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(5);

    DBOOL result = afs_toc_metadata_save_file(toc_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_file with file_name pointer not equal to NULL and afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_save_file_test3)
{
    const char * file_name = "afs_toc_metadata_save_file_test.xml";
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_save_file(toc_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_save_file with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_save_file_test4)
{
    const char * file_name = "afs_toc_metadata_save_file_empty_test.xml";
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_save_file(toc_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_file with afs_toc_metadata instance with empty sources vector
BOXING_START_TEST(afs_toc_metadata_save_file_test5)
{
    const char * file_name = "afs_toc_metadata_save_file_empty_test.xml";
    afs_toc_metadata_sources * sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);

    DBOOL result = afs_toc_metadata_save_file(toc_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_file with afs_toc_metadata instance with sources vector contains not valid data
BOXING_START_TEST(afs_toc_metadata_save_file_test6)
{
    const char * file_name = "afs_toc_metadata_save_file_not_valid_test.xml";
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(5);

    GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, 1)->file_id = -3;

    DBOOL result = afs_toc_metadata_save_file(toc_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_file with afs_toc_metadata instance with filled sources vector, in compact format
BOXING_START_TEST(afs_toc_metadata_save_file_test7)
{
    const char * file_name = "afs_toc_metadata_save_file_compact_test.xml";

    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(5);

    DBOOL result = afs_toc_metadata_save_file(toc_metadata, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_file with afs_toc_metadata instance with filled sources vector, in not compact format
BOXING_START_TEST(afs_toc_metadata_save_file_test8)
{
    const char * file_name = "afs_toc_metadata_save_file_not_compact_test.xml";

    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(5);

    DBOOL result = afs_toc_metadata_save_file(toc_metadata, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_string with afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_save_string_test1)
{
    afs_toc_metadata * toc_metadata = NULL;

    char * result = afs_toc_metadata_save_string(toc_metadata, DTRUE);

    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_metadata_save_string with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_save_string_test2)
{
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    char * result = afs_toc_metadata_save_string(toc_metadata, DTRUE);

    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_string with afs_toc_metadata instance with empty sources vector
BOXING_START_TEST(afs_toc_metadata_save_string_test3)
{
    afs_toc_metadata_sources * sources = gvector_create_pointers(0);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);

    char * result = afs_toc_metadata_save_string(toc_metadata, DTRUE);

    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_string with not valid afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_save_string_test4)
{
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(5);

    GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, 1)->file_id = -3;

    char * result = afs_toc_metadata_save_string(toc_metadata, DTRUE);

    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_string with valid afs_toc_metadata instance, in compact format
BOXING_START_TEST(afs_toc_metadata_save_string_test5)
{
    char * test_string = read_xml_toc_metadata_file("afs_toc_metadata_save_file_compact_test.xml");
    afs_xmlutils_string_cut(&test_string, strlen("<?xml version=\"1.0\" encoding=\"utf-8\"?>"), strlen(test_string));

    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(5);

    char * result = afs_toc_metadata_save_string(toc_metadata, DTRUE);
    
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);
    
    afs_toc_metadata_free(toc_metadata);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_metadata_save_string with valid afs_toc_metadata instance, in not compact format
BOXING_START_TEST(afs_toc_metadata_save_string_test6)
{
    char * test_string = read_xml_toc_metadata_file("afs_toc_metadata_save_file_not_compact_test.xml");
    BOXING_ASSERT(test_string != NULL);
    afs_xmlutils_string_cut(&test_string, strlen("<?xml version=\"1.0\" encoding=\"utf-8\"?>"), strlen(test_string));

    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(5);

    char * result = afs_toc_metadata_save_string(toc_metadata, DFALSE);
    
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_metadata_free(toc_metadata);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_metadata_save_xml with mxml_node_t pointer equal to NULL and afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_save_xml(toc_metadata, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_save_xml with mxml_node_t pointer equal to NULL and afs_toc_metadata pointer not equal to NULL
BOXING_START_TEST(afs_toc_metadata_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(3);

    DBOOL result = afs_toc_metadata_save_xml(toc_metadata, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_save_xml(toc_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_save_xml with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_save_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_save_xml(toc_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_save_xml with not valid afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(3);

    GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, 1)->file_id = -3;

    DBOOL result = afs_toc_metadata_save_xml(toc_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_save_xml with valid afs_toc_metadata instance
BOXING_START_TEST(afs_toc_metadata_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_metadata * toc_metadata = get_afs_toc_metadata_instance(3);

    DBOOL result = afs_toc_metadata_save_xml(toc_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_free(toc_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_load_file with file_name pointer equal to NULL and afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_load_file(toc_metadata, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_load_file with file_name pointer equal to NULL and afs_toc_metadata pointer not equal to NULL
BOXING_START_TEST(afs_toc_metadata_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_file(toc_metadata, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_load_file with file_name pointer not equal to NULL and afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_load_file_test3)
{
    const char * file_name = "afs_toc_metadata_save_file_compact_test.xml";
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_load_file(toc_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_metadata_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_file(toc_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_load_file with initialized afs_toc_metadata instance, in compact format
BOXING_START_TEST(afs_toc_metadata_load_file_test5)
{
    const char * file_name = "afs_toc_metadata_save_file_compact_test.xml";
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_file(toc_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata(toc_metadata, 5);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_load_file with initialized afs_toc_metadata instance, in not compact format
BOXING_START_TEST(afs_toc_metadata_load_file_test6)
{
    const char * file_name = "afs_toc_metadata_save_file_not_compact_test.xml";
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_file(toc_metadata, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata(toc_metadata, 5);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_load_string with input string pointer equal to NULL and afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_load_string(toc_metadata, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_load_string with input string pointer equal to NULL and afs_toc_metadata pointer not equal to NULL
BOXING_START_TEST(afs_toc_metadata_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_string(toc_metadata, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_load_string with input string pointer not equal to NULL and afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_load_string_test3)
{
    char * input_string = read_xml_toc_metadata_file("afs_toc_metadata_save_file_compact_test.xml");
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_load_string(toc_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_metadata_load_string with empty input string
BOXING_START_TEST(afs_toc_metadata_load_string_test4)
{
    char * input_string = "";
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_string(toc_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_load_string with input string for initialized afs_toc_metadata instance, in compact format
BOXING_START_TEST(afs_toc_metadata_load_string_test5)
{
    char * input_string = read_xml_toc_metadata_file("afs_toc_metadata_save_file_compact_test.xml");

    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_string(toc_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata(toc_metadata, 5);

    afs_toc_metadata_free(toc_metadata);
    free(input_string);
}
END_TEST


// Test function afs_toc_metadata_load_string with input string for initialized afs_toc_metadata instance, in not compact format
BOXING_START_TEST(afs_toc_metadata_load_string_test6)
{
    char * input_string = read_xml_toc_metadata_file("afs_toc_metadata_save_file_not_compact_test.xml");

    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_string(toc_metadata, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata(toc_metadata, 5);

    afs_toc_metadata_free(toc_metadata);
    free(input_string);
}
END_TEST


// Test function afs_toc_metadata_load_xml with xml instance pointer equal to NULL and afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_load_xml(toc_metadata, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_load_xml with xml instance pointer equal to NULL and afs_toc_metadata pointer not equal to NULL
BOXING_START_TEST(afs_toc_metadata_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_xml(toc_metadata, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
}
END_TEST


// Test function afs_toc_metadata_load_xml with xml instance pointer not equal to NULL and afs_toc_metadata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_load_xml_test3)
{
    mxml_node_t *tree = get_toc_metadata_xml_instance("afs_toc_metadata_save_file_compact_test.xml");
    afs_toc_metadata * toc_metadata = NULL;

    DBOOL result = afs_toc_metadata_load_xml(toc_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_metadata_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_xml(toc_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_null_afs_toc_metadata(toc_metadata);

    afs_toc_metadata_free(toc_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_load_xml with xml instance for initialized afs_toc_metadata instance, in compact format
BOXING_START_TEST(afs_toc_metadata_load_xml_test5)
{
    mxml_node_t *tree = get_toc_metadata_xml_instance("afs_toc_metadata_save_file_compact_test.xml");
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_xml(toc_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata(toc_metadata, 5);

    afs_toc_metadata_free(toc_metadata);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_load_xml with xml instance for initialized afs_toc_metadata instance, in not compact format
BOXING_START_TEST(afs_toc_metadata_load_xml_test6)
{
    mxml_node_t *tree = get_toc_metadata_xml_instance("afs_toc_metadata_save_file_not_compact_test.xml");
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create();

    DBOOL result = afs_toc_metadata_load_xml(toc_metadata, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata(toc_metadata, 5);

    afs_toc_metadata_free(toc_metadata);
    mxmlDelete(tree);
}
END_TEST


Suite * tocmetadata_tests(void)
{
    TCase * tc_tocmetadata_functions_tests = tcase_create("tocmetadata_functions_tests");

    // Test function afs_toc_metadata_create
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_create_test1);
    // Test function afs_toc_files_create2
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_create2_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_create2_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_create2_test3);
    // Test function afs_toc_files_create3
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_create3_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_create3_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_create3_test3);
    // Test function afs_toc_metadata_init
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_init_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_init_test2);
    // Test function afs_toc_metadata_init2
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_init2_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_init2_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_init2_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_init2_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_init2_test5);
    // Test function afs_toc_metadata_clone
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_clone_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_clone_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_clone_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_clone_test4);
    // Test function afs_toc_metadata_get_new_reference
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_new_reference_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_new_reference_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_new_reference_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_new_reference_test4);
    // Test function afs_toc_metadata_equal
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test5);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test6);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test7);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test8);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test9);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test10);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test11);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test12);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_equal_test13);
    // Test function afs_toc_metadata_add_source
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test5);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test6);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test7);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test8);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test9);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test10);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test11);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test12);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test13);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test14);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_add_source_test15);
    // Test function afs_toc_metadata_get_source
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_source_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_source_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_source_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_source_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_source_test5);
    // Test function afs_toc_metadata_get_sources_count
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_sources_count_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_sources_count_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_sources_count_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_get_sources_count_test4);
    // Test function afs_toc_metadata_is_valid
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_is_valid_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_is_valid_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_is_valid_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_is_valid_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_is_valid_test5);
    // Test function afs_toc_metadata_save_file
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_file_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_file_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_file_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_file_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_file_test5);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_file_test6);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_file_test7);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_file_test8);
    // Test function afs_toc_metadata_save_string
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_string_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_string_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_string_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_string_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_string_test5);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_string_test6);
    // Test function afs_toc_metadata_save_xml
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_xml_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_xml_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_xml_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_xml_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_xml_test5);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_save_xml_test6);
    // Test function afs_toc_metadata_load_file
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_file_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_file_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_file_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_file_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_file_test5);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_file_test6);
    // Test function afs_toc_metadata_load_string
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_string_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_string_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_string_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_string_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_string_test5);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_string_test6);
    // Test function afs_toc_metadata_load_xml
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_xml_test1);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_xml_test2);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_xml_test3);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_xml_test4);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_xml_test5);
    tcase_add_test(tc_tocmetadata_functions_tests, afs_toc_metadata_load_xml_test6);

    Suite * s = suite_create("tocmetadata_test_util");
    suite_add_tcase(s, tc_tocmetadata_functions_tests);

    return s;
}
