#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "tocdatafilemetadatasource_c.h"
#include "boxing/platform/types.h"
#include "boxing/utils.h"
#include "mxml.h"
#include "platform/io.h"

#include <string.h>

static char * read_xml_toc_data_file_metadata_source_file(const char* file_name)
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


static mxml_node_t * get_toc_data_file_metadata_source_xml_instance(const char * file_name)
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


static void test_empty_afs_toc_data_file_metadata_source(afs_toc_data_file_metadata_source * toc_data_file_metadata_source)
{
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source->data == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source->format_id == NULL);

    BOXING_ASSERT(toc_data_file_metadata_source->file_id == -1);
    BOXING_ASSERT(toc_data_file_metadata_source->source_id == -1);
}


static void test_not_empty_afs_toc_data_file_metadata_source(afs_toc_data_file_metadata_source * toc_data_file_metadata_source, const char * data, const char * format_id, int file_id, int source_id)
{
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(strcmp(toc_data_file_metadata_source->data ? toc_data_file_metadata_source->data : "", data ? data : "") == 0);
    BOXING_ASSERT(strcmp(toc_data_file_metadata_source->format_id ? toc_data_file_metadata_source->format_id : "", format_id ? format_id : "") == 0);

    BOXING_ASSERT(toc_data_file_metadata_source->file_id == file_id);
    BOXING_ASSERT(toc_data_file_metadata_source->source_id == source_id);
}


static void test_equals_toc_data_file_metadata_sources(afs_toc_data_file_metadata_source * toc_data_file_metadata_source1, afs_toc_data_file_metadata_source * toc_data_file_metadata_source2, DBOOL expected_result)
{
    DBOOL result = afs_toc_data_file_metadata_source_equal(toc_data_file_metadata_source2, toc_data_file_metadata_source1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_data_file_metadata_source_equal(toc_data_file_metadata_source1, toc_data_file_metadata_source2);
    BOXING_ASSERT(result == expected_result);
}


// TEST TOC DATA FILE METADATA SOURCE FUNCTIONS
//

// Test function afs_toc_data_file_metadata_source_create test1
BOXING_START_TEST(afs_toc_data_file_metadata_source_create_test1)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_create2 test1 with empty initial values
BOXING_START_TEST(afs_toc_data_file_metadata_source_create2_test1)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2(NULL, NULL, -1, -1);

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_create2 test2 with normal initial values
BOXING_START_TEST(afs_toc_data_file_metadata_source_create2_test2)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_init test with afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_init_test1)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    afs_toc_data_file_metadata_source_init(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_init test with normal initialization data
BOXING_START_TEST(afs_toc_data_file_metadata_source_init_test2)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    afs_toc_data_file_metadata_source_init(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_init test with empty initialization data
BOXING_START_TEST(afs_toc_data_file_metadata_source_init_test3)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = malloc(sizeof(afs_toc_data_file_metadata_source));

    afs_toc_data_file_metadata_source_init(toc_data_file_metadata_source, NULL, NULL, -1, -1);

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_clone with input pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_clone_test1)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = NULL;

    BOXING_ASSERT(toc_data_file_metadata_source1 == NULL);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_clone(toc_data_file_metadata_source1);

    BOXING_ASSERT(toc_data_file_metadata_source2 == NULL);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_clone with empty input afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_clone_test2)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create();

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_clone(toc_data_file_metadata_source1);

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_clone with normal input afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_clone_test3)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_clone(toc_data_file_metadata_source1);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_clone with empty string values
BOXING_START_TEST(afs_toc_data_file_metadata_source_clone_test4)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("", "", 6, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "", "", 6, 7);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_clone(toc_data_file_metadata_source1);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "", "", 6, 7);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_clone with normal string values and integer values equla to -1
BOXING_START_TEST(afs_toc_data_file_metadata_source_clone_test5)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", -1, -1);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", -1, -1);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_clone(toc_data_file_metadata_source1);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "some xml data", "afs/metadata-csv", -1, -1);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_get_new_reference_test1)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = NULL;

    BOXING_ASSERT(toc_data_file_metadata_source1 == NULL);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_get_new_reference(toc_data_file_metadata_source1);

    BOXING_ASSERT(toc_data_file_metadata_source2 == NULL);

    toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create();

    BOXING_ASSERT(toc_data_file_metadata_source2 == NULL);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_get_new_reference with empty input afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_get_new_reference_test2)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create();

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1);
    BOXING_ASSERT(toc_data_file_metadata_source1->reference_count == 1);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_get_new_reference(toc_data_file_metadata_source1);

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2);
    BOXING_ASSERT(toc_data_file_metadata_source1->reference_count == 2);
    BOXING_ASSERT(toc_data_file_metadata_source2->reference_count == 2);
    BOXING_ASSERT(toc_data_file_metadata_source1 == toc_data_file_metadata_source2);

    toc_data_file_metadata_source1->file_id = 101;

    BOXING_ASSERT(toc_data_file_metadata_source2->file_id == 101);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_get_new_reference with normal input afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_get_new_reference_test3)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", 6, 7);
    BOXING_ASSERT(toc_data_file_metadata_source1->reference_count == 1);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_get_new_reference(toc_data_file_metadata_source1);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "some xml data", "afs/metadata-csv", 6, 7);
    BOXING_ASSERT(toc_data_file_metadata_source1->reference_count == 2);
    BOXING_ASSERT(toc_data_file_metadata_source2->reference_count == 2);
    BOXING_ASSERT(toc_data_file_metadata_source1 == toc_data_file_metadata_source2);

    toc_data_file_metadata_source1->file_id = 101;

    BOXING_ASSERT(toc_data_file_metadata_source2->file_id == 101);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_get_new_reference with empty string values
BOXING_START_TEST(afs_toc_data_file_metadata_source_get_new_reference_test4)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("", "", 6, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "", "", 6, 7);
    BOXING_ASSERT(toc_data_file_metadata_source1->reference_count == 1);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_get_new_reference(toc_data_file_metadata_source1);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "", "", 6, 7);
    BOXING_ASSERT(toc_data_file_metadata_source1->reference_count == 2);
    BOXING_ASSERT(toc_data_file_metadata_source2->reference_count == 2);
    BOXING_ASSERT(toc_data_file_metadata_source1 == toc_data_file_metadata_source2);

    toc_data_file_metadata_source1->file_id = 101;

    BOXING_ASSERT(toc_data_file_metadata_source2->file_id == 101);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_get_new_reference with normal string values and integer values equal to -1
BOXING_START_TEST(afs_toc_data_file_metadata_source_get_new_reference_test5)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", -1, -1);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", -1, -1);
    BOXING_ASSERT(toc_data_file_metadata_source1->reference_count == 1);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_get_new_reference(toc_data_file_metadata_source1);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "some xml data", "afs/metadata-csv", -1, -1);
    BOXING_ASSERT(toc_data_file_metadata_source1->reference_count == 2);
    BOXING_ASSERT(toc_data_file_metadata_source2->reference_count == 2);
    BOXING_ASSERT(toc_data_file_metadata_source1 == toc_data_file_metadata_source2);

    toc_data_file_metadata_source1->file_id = 101;

    BOXING_ASSERT(toc_data_file_metadata_source2->file_id == 101);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with input afs_toc_data_file_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test1)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = NULL;
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = NULL;

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DTRUE);
}
END_TEST

// Test function afs_toc_data_file_metadata_source_equal test with empty input afs_toc_data_file_metadata_source instances 
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test2)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create();
    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_create();
    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2);

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with normal input afs_toc_data_file_metadata_source instances 
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test3)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "some xml data", "afs/metadata-csv", 6, 7);

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with empty string values 
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test4)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("", "", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "", "", 6, 7);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_create2("", "", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "", "", 6, 7);

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with normal string values and integer values equal to -1
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test5)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", -1, -1);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", -1, -1);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", -1, -1);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "some xml data", "afs/metadata-csv", -1, -1);

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with empty input afs_toc_data_file_metadata_source instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test6)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create();
    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = NULL;

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with normal input afs_toc_data_file_metadata_source instance and pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test7)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = NULL;

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with normal afs_toc_data_file_metadata_source instance and empty afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test8)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_create();
    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2);

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with afs_toc_data_file_metadata_source instances with different data
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test9)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_create2("some xml data2", "afs/metadata-csv", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "some xml data2", "afs/metadata-csv", 6, 7);

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with afs_toc_data_file_metadata_source instances with different format id
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test10)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv2", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "some xml data", "afs/metadata-csv2", 6, 7);

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with afs_toc_data_file_metadata_source instances with different file id
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test11)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 3, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "some xml data", "afs/metadata-csv", 3, 7);

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_equal with afs_toc_data_file_metadata_source instances with different source id
BOXING_START_TEST(afs_toc_data_file_metadata_source_equal_test12)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source1 = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source1, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source2 = afs_toc_data_file_metadata_source_create2("some xml data2", "afs/metadata-csv", 6, 5);
    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source2, "some xml data2", "afs/metadata-csv", 6, 5);

    test_equals_toc_data_file_metadata_sources(toc_data_file_metadata_source1, toc_data_file_metadata_source2, DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source1);
    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source2);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_is_valid with input afs_toc_data_file_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_is_valid_test1)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source);

    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_is_valid with empty afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_is_valid_test2)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_is_valid test with data string pointer equal to NULL 
BOXING_START_TEST(afs_toc_data_file_metadata_source_is_valid_test3)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2(NULL, "afs/metadata-csv", 6, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, NULL, "afs/metadata-csv", 6, 7);

    DBOOL result = afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_is_valid test with empty data string 
BOXING_START_TEST(afs_toc_data_file_metadata_source_is_valid_test4)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("", "afs/metadata-csv", 6, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "", "afs/metadata-csv", 6, 7);

    DBOOL result = afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_is_valid test with file id equal to -1 
BOXING_START_TEST(afs_toc_data_file_metadata_source_is_valid_test5)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", -1, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", -1, 7);

    DBOOL result = afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_is_valid test with file id equal to -1 and data string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_is_valid_test6)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2(NULL, "afs/metadata-csv", -1, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, NULL, "afs/metadata-csv", -1, 7);

    DBOOL result = afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_is_valid test with file id equal to -1 and empty data string
BOXING_START_TEST(afs_toc_data_file_metadata_source_is_valid_test7)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("", "afs/metadata-csv", -1, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "", "afs/metadata-csv", -1, 7);

    DBOOL result = afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_is_valid test with all normal data
BOXING_START_TEST(afs_toc_data_file_metadata_source_is_valid_test8)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    DBOOL result = afs_toc_data_file_metadata_source_is_valid(toc_data_file_metadata_source);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_data with input afs_toc_data_file_metadata_source pointer and data string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_data_test1)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;
    const char * data = NULL;

    afs_toc_data_file_metadata_source_set_data(toc_data_file_metadata_source, data);

    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(data == NULL);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_data with input afs_toc_data_file_metadata_source pointer equal to NULL and empty data string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_data_test2)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;
    const char * data = "";

    afs_toc_data_file_metadata_source_set_data(toc_data_file_metadata_source, data);

    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(strcmp(data, "") == 0);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_data with input afs_toc_data_file_metadata_source pointer equal to NULL and normal data string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_data_test3)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;
    const char * data = "some xml data";

    afs_toc_data_file_metadata_source_set_data(toc_data_file_metadata_source, data);

    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(strcmp(data, "some xml data") == 0);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_data with empty input afs_toc_data_file_metadata_source instance and data string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_data_test4)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();
    const char * data = NULL;

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    afs_toc_data_file_metadata_source_set_data(toc_data_file_metadata_source, data);

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);
    BOXING_ASSERT(data == NULL);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_data with empty input afs_toc_data_file_metadata_source instance and empty data string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_data_test5)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();
    const char * data = "";

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    afs_toc_data_file_metadata_source_set_data(toc_data_file_metadata_source, data);

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);
    BOXING_ASSERT(strcmp(data, "") == 0);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_data with empty input afs_toc_data_file_metadata_source instance and normal data string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_data_test6)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();
    const char * data = "some xml data";

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    afs_toc_data_file_metadata_source_set_data(toc_data_file_metadata_source, data);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", NULL, -1, -1);
    BOXING_ASSERT(strcmp(data, "some xml data") == 0);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_data with normal input afs_toc_data_file_metadata_source instance and data string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_data_test7)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    const char * data = NULL;

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_set_data(toc_data_file_metadata_source, data);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);
    BOXING_ASSERT(data == NULL);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_data with normal input afs_toc_data_file_metadata_source instance and empty data string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_data_test8)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    const char * data = "";

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_set_data(toc_data_file_metadata_source, data);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);
    BOXING_ASSERT(strcmp(data, "") == 0);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_data with normal input afs_toc_data_file_metadata_source instance and normal data string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_data_test9)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    const char * data = "some other xml data";

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_set_data(toc_data_file_metadata_source, data);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some other xml data", "afs/metadata-csv", 6, 7);
    BOXING_ASSERT(strcmp(data, "some other xml data") == 0);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_format_id with input afs_toc_data_file_metadata_source pointer and format id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_format_id_test1)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;
    const char * format_id = NULL;

    afs_toc_data_file_metadata_source_set_format_id(toc_data_file_metadata_source, format_id);

    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(format_id == NULL);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_format_id with input afs_toc_data_file_metadata_source pointer equal to NULL and empty format id string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_format_id_test2)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;
    const char * format_id = "";

    afs_toc_data_file_metadata_source_set_format_id(toc_data_file_metadata_source, format_id);

    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(strcmp(format_id, "") == 0);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_format_id with input afs_toc_data_file_metadata_source pointer equal to NULL and normal format id string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_format_id_test3)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;
    const char * format_id = "afs/metadata-csv";

    afs_toc_data_file_metadata_source_set_format_id(toc_data_file_metadata_source, format_id);

    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(strcmp(format_id, "afs/metadata-csv") == 0);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_format_id with empty input afs_toc_data_file_metadata_source instance and format id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_format_id_test4)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();
    const char * format_id = NULL;

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    afs_toc_data_file_metadata_source_set_format_id(toc_data_file_metadata_source, format_id);

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);
    BOXING_ASSERT(format_id == NULL);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_format_id with empty input afs_toc_data_file_metadata_source instance and empty format_id string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_format_id_test5)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();
    const char * format_id = "";

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    afs_toc_data_file_metadata_source_set_format_id(toc_data_file_metadata_source, format_id);

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);
    BOXING_ASSERT(strcmp(format_id, "") == 0);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_format_id with empty input afs_toc_data_file_metadata_source instance and normal format id string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_format_id_test6)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();
    const char * format_id = "afs/metadata-csv";

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    afs_toc_data_file_metadata_source_set_format_id(toc_data_file_metadata_source, format_id);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, NULL, "afs/metadata-csv", -1, -1);
    BOXING_ASSERT(strcmp(format_id, "afs/metadata-csv") == 0);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_format_id with normal input afs_toc_data_file_metadata_source instance and format id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_format_id_test7)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    const char * format_id = NULL;

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_set_format_id(toc_data_file_metadata_source, format_id);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);
    BOXING_ASSERT(format_id == NULL);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_format_id with normal input afs_toc_data_file_metadata_source instance and empty format id string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_format_id_test8)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    const char * format_id = "";

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_set_format_id(toc_data_file_metadata_source, format_id);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);
    BOXING_ASSERT(strcmp(format_id, "") == 0);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_set_format_id with normal input afs_toc_data_file_metadata_source instance and normal format id string
BOXING_START_TEST(afs_toc_data_file_metadata_source_set_format_id_test9)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);
    const char * format_id = "other afs/metadata-csv";

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_set_format_id(toc_data_file_metadata_source, format_id);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "other afs/metadata-csv", 6, 7);
    BOXING_ASSERT(strcmp(format_id, "other afs/metadata-csv") == 0);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_file with file_name pointer equal to NULL and afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_save_file(toc_data_file_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_file with file_name pointer equal to NULL and empty afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_save_file(toc_data_file_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_file with file_name pointer equal to NULL and normal afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_file_test3)
{
    const char * file_name = NULL;

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    DBOOL result = afs_toc_data_file_metadata_source_save_file(toc_data_file_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_file with file_name pointer equal to NULL and not valid afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_file_test4)
{
    const char * file_name = NULL;

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("", "afs/metadata-csv", -1, 7);

    DBOOL result = afs_toc_data_file_metadata_source_save_file(toc_data_file_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_file with file_name pointer not equal to NULL and afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_file_test5)
{
    const char * file_name = "afs_toc_data_file_metadata_source_save_file_test.xml";
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_save_file(toc_data_file_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_file with empty afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_file_test6)
{
    const char * file_name = "afs_toc_data_file_metadata_source_save_file_empty_test.xml";
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_save_file(toc_data_file_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_file with not valid afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_file_test7)
{
    const char * file_name = "afs_toc_data_file_metadata_source_save_file_not_valid_test.xml";
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("", "afs/metadata-csv", -1, 7);

    DBOOL result = afs_toc_data_file_metadata_source_save_file(toc_data_file_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_file with initialized afs_toc_data_file_metadata_source instance, in compact format
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_file_test8)
{
    const char * file_name = "afs_toc_data_file_metadata_source_save_file_compact_test.xml";

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    DBOOL result = afs_toc_data_file_metadata_source_save_file(toc_data_file_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_file with initialized afs_toc_data_file_metadata_source instance, in not compact format
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_file_test9)
{
    const char * file_name = "afs_toc_data_file_metadata_source_save_file_not_compact_test.xml";

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    DBOOL result = afs_toc_data_file_metadata_source_save_file(toc_data_file_metadata_source, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_file with initialized afs_toc_data_file_metadata_source instance and empty input file name
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_file_test10)
{
    const char * file_name = "";

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    DBOOL result = afs_toc_data_file_metadata_source_save_file(toc_data_file_metadata_source, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_string with afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_string_test1)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    char * result = afs_toc_data_file_metadata_source_save_string(toc_data_file_metadata_source, DTRUE);

    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_string with empty afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_string_test2)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    char * result = afs_toc_data_file_metadata_source_save_string(toc_data_file_metadata_source, DTRUE);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_string with not valid afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_string_test3)
{
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("", "afs/metadata-csv", -1, 7);

    char * result = afs_toc_data_file_metadata_source_save_string(toc_data_file_metadata_source, DTRUE);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_string with initialized afs_toc_data_file_metadata_source instance, in compact format
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_string_test4)
{
    char * test_string = read_xml_toc_data_file_metadata_source_file("afs_toc_data_file_metadata_source_save_file_compact_test.xml");

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    char * result = afs_toc_data_file_metadata_source_save_string(toc_data_file_metadata_source, DTRUE);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_string with initialized afs_toc_data_file_metadata_source instance, in not compact format
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_string_test5)
{
    char * test_string = read_xml_toc_data_file_metadata_source_file("afs_toc_data_file_metadata_source_save_file_not_compact_test.xml");

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    char * result = afs_toc_data_file_metadata_source_save_string(toc_data_file_metadata_source, DFALSE);

    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_xml with mxml_node_t pointer equal to NULL and afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_save_xml(toc_data_file_metadata_source, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_xml with mxml_node_t pointer equal to NULL and afs_toc_data_file_metadata_source pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    DBOOL result = afs_toc_data_file_metadata_source_save_xml(toc_data_file_metadata_source, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_save_xml(toc_data_file_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_xml with empty afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_save_xml(toc_data_file_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_xml with not valid afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("", "afs/metadata-csv", -1, 7);

    DBOOL result = afs_toc_data_file_metadata_source_save_xml(toc_data_file_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_save_xml with initialized afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", 6, 7);

    DBOOL result = afs_toc_data_file_metadata_source_save_xml(toc_data_file_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_file with file_name pointer equal to NULL and afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_load_file(toc_data_file_metadata_source, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_file with file_name pointer equal to NULL and afs_toc_data_file_metadata_source pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_file(toc_data_file_metadata_source, file_name);

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_file with file_name pointer not equal to NULL and afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_file_test3)
{
    const char * file_name = "afs_toc_data_file_metadata_source_save_file_compact_test.xml";
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_load_file(toc_data_file_metadata_source, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_file(toc_data_file_metadata_source, file_name);

    test_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_file with initialized afs_toc_data_file_metadata_source instance, in compact format
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_file_test5)
{
    const char * file_name = "afs_toc_data_file_metadata_source_save_file_compact_test.xml";
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_file(toc_data_file_metadata_source, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_file with initialized afs_toc_data_file_metadata_source instance, in not compact format
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_file_test6)
{
    const char * file_name = "afs_toc_data_file_metadata_source_save_file_not_compact_test.xml";
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_file(toc_data_file_metadata_source, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_string with input string pointer equal to NULL and afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_load_string(toc_data_file_metadata_source, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_string with input string pointer equal to NULL and afs_toc_data_file_metadata_source pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_string(toc_data_file_metadata_source, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_string with input string pointer not equal to NULL and afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_string_test3)
{
    char * input_string = read_xml_toc_data_file_metadata_source_file("afs_toc_data_file_metadata_source_save_file_not_compact_test.xml");
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_load_string(toc_data_file_metadata_source, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_string with empty input string
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_string_test4)
{
    char * input_string = "";
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_string(toc_data_file_metadata_source, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_string with input string for initialized afs_toc_data_file_metadata_source instance, in compact format
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_string_test5)
{
    char * input_string = read_xml_toc_data_file_metadata_source_file("afs_toc_data_file_metadata_source_save_file_compact_test.xml");

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_string(toc_data_file_metadata_source, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_string with input string for initialized afs_toc_data_file_metadata_source instance, in not compact format
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_string_test6)
{
    char * input_string = read_xml_toc_data_file_metadata_source_file("afs_toc_data_file_metadata_source_save_file_not_compact_test.xml");
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_string(toc_data_file_metadata_source, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
    free(input_string);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_xml with xml instance pointer equal to NULL and afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_load_xml(toc_data_file_metadata_source, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_xml with xml instance pointer equal to NULL and afs_toc_data_file_metadata_source pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_xml(toc_data_file_metadata_source, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_xml with xml instance pointer not equal to NULL and afs_toc_data_file_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_xml_test3)
{
    mxml_node_t *tree = get_toc_data_file_metadata_source_xml_instance("afs_toc_data_file_metadata_source_save_file_compact_test.xml");
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = NULL;

    DBOOL result = afs_toc_data_file_metadata_source_load_xml(toc_data_file_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_xml(toc_data_file_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_file_metadata_source_load_xml with xml instance for initialized afs_toc_data_file_metadata_source instance
BOXING_START_TEST(afs_toc_data_file_metadata_source_load_xml_test5)
{
    mxml_node_t *tree = get_toc_data_file_metadata_source_xml_instance("afs_toc_data_file_metadata_source_save_file_compact_test.xml");

    afs_toc_data_file_metadata_source * toc_data_file_metadata_source = afs_toc_data_file_metadata_source_create();

    DBOOL result = afs_toc_data_file_metadata_source_load_xml(toc_data_file_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data_file_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_data_file_metadata_source(toc_data_file_metadata_source, "some xml data", "afs/metadata-csv", 6, 7);

    afs_toc_data_file_metadata_source_free(toc_data_file_metadata_source);
    mxmlDelete(tree);
}
END_TEST


Suite * tocdatafilemetadatasource_tests(void)
{
    TCase * tc_tocdatafilemetadatasource_functions_tests = tcase_create("tocdatafilemetadatasource_functions_tests");

    // Test function afs_toc_data_file_metadata_source_create
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_create_test1);
    // Test function afs_toc_data_file_metadata_source_create2
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_create2_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_create2_test2);
    // Test function afs_toc_data_file_metadata_source_init
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_init_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_init_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_init_test3);
    // Test function afs_toc_data_file_metadata_source_clone
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_clone_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_clone_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_clone_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_clone_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_clone_test5);
    // Test function afs_toc_data_file_metadata_source_get_new_reference
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_get_new_reference_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_get_new_reference_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_get_new_reference_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_get_new_reference_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_get_new_reference_test5);
    // Test function afs_toc_data_file_metadata_source_equal
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test5);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test6);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test7);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test8);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test9);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test10);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test11);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_equal_test12);
    // Test function afs_toc_data_file_metadata_source_is_valid
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_is_valid_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_is_valid_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_is_valid_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_is_valid_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_is_valid_test5);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_is_valid_test6);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_is_valid_test7);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_is_valid_test8);
    // Test function afs_toc_data_file_metadata_source_set_data
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_data_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_data_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_data_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_data_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_data_test5);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_data_test6);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_data_test7);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_data_test8);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_data_test9);
    // Test function afs_toc_data_file_metadata_source_set_format_id
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_format_id_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_format_id_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_format_id_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_format_id_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_format_id_test5);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_format_id_test6);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_format_id_test7);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_format_id_test8);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_set_format_id_test9);
    // Test function afs_toc_data_file_metadata_source_save_file
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_file_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_file_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_file_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_file_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_file_test5);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_file_test6);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_file_test7);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_file_test8);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_file_test9);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_file_test10);
    // Test function afs_toc_data_file_metadata_source_save_string
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_string_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_string_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_string_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_string_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_string_test5);
    // Test function afs_toc_data_file_metadata_source_save_xml
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_xml_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_xml_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_xml_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_xml_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_xml_test5);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_save_xml_test6);
    // Test function afs_toc_data_file_metadata_source_load_file
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_file_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_file_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_file_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_file_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_file_test5);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_file_test6);
    // Test function afs_toc_data_file_metadata_source_load_string
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_string_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_string_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_string_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_string_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_string_test5);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_string_test6);
    // Test function afs_toc_data_file_metadata_source_load_xml
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_xml_test1);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_xml_test2);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_xml_test3);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_xml_test4);
    tcase_add_test(tc_tocdatafilemetadatasource_functions_tests, afs_toc_data_file_metadata_source_load_xml_test5);

    Suite * s = suite_create("tocdatafilemetadatasource_test_util");
    suite_add_tcase(s, tc_tocdatafilemetadatasource_functions_tests);

    return s;
}
