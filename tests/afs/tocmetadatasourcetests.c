#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "tocmetadatasource_c.h"
#include "boxing/utils.h"
#include "mxml.h"


#if defined ( D_OS_WIN32 )
#define DFSEEK _fseeki64
#define DFTELL _ftelli64
#else
#define DFSEEK fseeko
#define DFTELL ftello
#endif


static char * read_xml_toc_metadata_source_file(const char* file_name)
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
    char * xml_string = boxing_string_allocate((size_t)size + 1);

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


static mxml_node_t * get_toc_metadata_source_xml_instance(const char * file_name)
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


static void test_empty_afs_toc_metadata_source(afs_toc_metadata_source* toc_metadata_source)
{
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(toc_metadata_source->format == NULL);
    BOXING_ASSERT(toc_metadata_source->data == NULL);
    BOXING_ASSERT(toc_metadata_source->tags == NULL);

    BOXING_ASSERT(toc_metadata_source->id == ID_UNDEFINED);
    BOXING_ASSERT(toc_metadata_source->file_id == FILE_ID_UNDEFINED);
}


static void test_not_empty_afs_toc_metadata_source(afs_toc_metadata_source* toc_metadata_source, const char * format, int file_id, const char * data, const char * tags, int id)
{
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(boxing_string_equal(toc_metadata_source->format, format) == DTRUE);
    BOXING_ASSERT(boxing_string_equal(toc_metadata_source->data, data) == DTRUE);
    char * source_tags = afs_toc_metadata_source_get_tags(toc_metadata_source);
    BOXING_ASSERT(boxing_string_equal(source_tags, tags) == DTRUE);
    boxing_string_free(source_tags);

    BOXING_ASSERT(toc_metadata_source->id == id);
    BOXING_ASSERT(toc_metadata_source->file_id == file_id);
}


static void test_equals_toc_matadata_sources(afs_toc_metadata_source* toc_metadata_source1, afs_toc_metadata_source* toc_metadata_source2, DBOOL expected_result)
{
    DBOOL result = afs_toc_metadata_source_equal(toc_metadata_source2, toc_metadata_source1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_metadata_source_equal(toc_metadata_source1, toc_metadata_source2);
    BOXING_ASSERT(result == expected_result);
}


// TEST TOC METADATA SOURCE FUNCTIONS
//

// Test function afs_toc_metadata_source_create test1
BOXING_START_TEST(afs_toc_metadata_source_create_test1)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create2 test1 with empty initial values
BOXING_START_TEST(afs_toc_metadata_source_create2_test1)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create2(NULL, FILE_ID_UNDEFINED);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create2 test2 with normal initial values
BOXING_START_TEST(afs_toc_metadata_source_create2_test2)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create2("afs/metadata-csv", 1);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 1, NULL, NULL, ID_UNDEFINED);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create3 test1 with empty initial values
BOXING_START_TEST(afs_toc_metadata_source_create3_test1)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create3(NULL, FILE_ID_UNDEFINED, ID_UNDEFINED);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create3 test2 with normal initial values
BOXING_START_TEST(afs_toc_metadata_source_create3_test2)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create3("afs/metadata-csv", 1, 2);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 1, NULL, NULL, 2);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create4 test1 with empty initial values
BOXING_START_TEST(afs_toc_metadata_source_create4_test1)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create4(NULL, FILE_ID_UNDEFINED, NULL);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create4 test2 with normal initial values
BOXING_START_TEST(afs_toc_metadata_source_create4_test2)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create4("afs/metadata-csv", 4, "some xml data");

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 4, "some xml data", NULL, ID_UNDEFINED);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create5 test1 with empty initial values
BOXING_START_TEST(afs_toc_metadata_source_create5_test1)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create5(NULL, FILE_ID_UNDEFINED, NULL, ID_UNDEFINED);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create5 test2 with normal initial values
BOXING_START_TEST(afs_toc_metadata_source_create5_test2)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create5("afs/metadata-csv", 5, "some xml data", 6);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 5, "some xml data", NULL, 6);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create6 test1 with empty initial values
BOXING_START_TEST(afs_toc_metadata_source_create6_test1)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create6(NULL, FILE_ID_UNDEFINED, NULL, NULL);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create6 test2 with normal initial values
BOXING_START_TEST(afs_toc_metadata_source_create6_test2)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create6("afs/metadata-csv", 6, "some xml data", "tag1;tag2;tag3");

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 6, "some xml data", "tag1;tag2;tag3", ID_UNDEFINED);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create7 test1 with empty initial values
BOXING_START_TEST(afs_toc_metadata_source_create7_test1)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7(NULL, FILE_ID_UNDEFINED, NULL, NULL, ID_UNDEFINED);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create7 test2 with normal initial values
BOXING_START_TEST(afs_toc_metadata_source_create7_test2)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 7, "some xml data", "tag1;tag2;tag3", 8);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 7, "some xml data", "tag1;tag2;tag3", 8);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create8 test1 with empty initial values
BOXING_START_TEST(afs_toc_metadata_source_create8_test1)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create8(NULL, FILE_ID_UNDEFINED, NULL);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create8 test2 with normal initial values
BOXING_START_TEST(afs_toc_metadata_source_create8_test2)
{
    afs_toc_metadata_source_tags * toc_metadata_source_tags = boxing_string_split("tag1;tag2;tag3", ";");
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create8("afs/metadata-csv", 8, toc_metadata_source_tags);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 8, NULL, "tag1;tag2;tag3", ID_UNDEFINED);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create9 test1 with empty initial values
BOXING_START_TEST(afs_toc_metadata_source_create9_test1)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create9(NULL, FILE_ID_UNDEFINED, NULL, ID_UNDEFINED);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_create9 test2 with normal initial values
BOXING_START_TEST(afs_toc_metadata_source_create9_test2)
{
    afs_toc_metadata_source_tags * toc_metadata_source_tags = boxing_string_split("tag1;tag2;tag3", ";");
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create9("afs/metadata-csv", 9, toc_metadata_source_tags, 10);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 9, NULL, "tag1;tag2;tag3", 10);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_init test with afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_init_test1)
{
    afs_toc_metadata_source * toc_metadata_source = NULL;

    afs_toc_metadata_source_init(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source == NULL);
}
END_TEST


// Test function afs_toc_metadata_source_init test2 with allocated toc_metadata_source pointer
BOXING_START_TEST(afs_toc_metadata_source_init_test2)
{
    afs_toc_metadata_source * toc_metadata_source = malloc(sizeof(afs_toc_metadata_source));

    afs_toc_metadata_source_init(toc_metadata_source);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_init2 test with afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_init2_test1)
{
    afs_toc_metadata_source * toc_metadata_source = NULL;

    afs_toc_metadata_source_init2(toc_metadata_source, "afs/metadata-csv", 0, "some xml data", "tag1;tag2;tag3", 1);

    BOXING_ASSERT(toc_metadata_source == NULL);
}
END_TEST


// Test function afs_toc_metadata_source_init2 test with normal initialization data
BOXING_START_TEST(afs_toc_metadata_source_init2_test2)
{
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_init2(toc_metadata_source, "afs/metadata-csv", 0, "some xml data", "tag1;tag2;tag3", 1);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 0, "some xml data", "tag1;tag2;tag3", 1);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_init2 test with empty initialization data
BOXING_START_TEST(afs_toc_metadata_source_init2_test3)
{
    afs_toc_metadata_source * toc_metadata_source = malloc(sizeof(afs_toc_metadata_source));

    afs_toc_metadata_source_init2(toc_metadata_source, NULL, FILE_ID_UNDEFINED, NULL, NULL, ID_UNDEFINED);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_init3 test with afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_init3_test1)
{
    afs_toc_metadata_source * toc_metadata_source = NULL;
    afs_toc_metadata_source_tags * toc_metadata_source_tags = boxing_string_split("tag1;tag2;tag3", ";");

    afs_toc_metadata_source_init3(toc_metadata_source, "afs/metadata-csv", 0, toc_metadata_source_tags, 1);

    BOXING_ASSERT(toc_metadata_source == NULL);

    gvector_free(toc_metadata_source_tags);
}
END_TEST


// Test function afs_toc_metadata_source_init3 test with normal initialization data
BOXING_START_TEST(afs_toc_metadata_source_init3_test2)
{
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();
    afs_toc_metadata_source_tags * toc_metadata_source_tags = boxing_string_split("tag1;tag2;tag3", ";");

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_init3(toc_metadata_source, "afs/metadata-csv", 0, toc_metadata_source_tags, 1);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 0, NULL, "tag1;tag2;tag3", 1);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_init3 test with empty initialization data
BOXING_START_TEST(afs_toc_metadata_source_init3_test3)
{
    afs_toc_metadata_source * toc_metadata_source = malloc(sizeof(afs_toc_metadata_source));

    afs_toc_metadata_source_init3(toc_metadata_source, NULL, FILE_ID_UNDEFINED, NULL, ID_UNDEFINED);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_clone test with input pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_clone_test1)
{
    afs_toc_metadata_source * toc_metadata_source1 = NULL;

    BOXING_ASSERT(toc_metadata_source1 == NULL);

    afs_toc_metadata_source * toc_metadata_source2 = afs_toc_metadata_source_clone(toc_metadata_source1);

    BOXING_ASSERT(toc_metadata_source2 == NULL);
}
END_TEST


// Test function afs_toc_metadata_source_clone test with empty input afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_clone_test2)
{
    afs_toc_metadata_source * toc_metadata_source1 = afs_toc_metadata_source_create();

    test_empty_afs_toc_metadata_source(toc_metadata_source1);

    afs_toc_metadata_source * toc_metadata_source2 = afs_toc_metadata_source_clone(toc_metadata_source1);

    test_empty_afs_toc_metadata_source(toc_metadata_source2);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source2);
}
END_TEST


// Test function afs_toc_metadata_source_clone test with normal input afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_clone_test3)
{
    afs_toc_metadata_source* toc_metadata_source1 = afs_toc_metadata_source_create7("afs/metadata-csv", 7, "some xml data", "tag1;tag2;tag3", 8);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source1, "afs/metadata-csv", 7, "some xml data", "tag1;tag2;tag3", 8);

    afs_toc_metadata_source* toc_metadata_source2 = afs_toc_metadata_source_clone(toc_metadata_source1);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source2, "afs/metadata-csv", 7, "some xml data", "tag1;tag2;tag3", 8);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source2);
}
END_TEST


// Test function afs_toc_metadata_source_clone test with empty tags vector in the afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_clone_test4)
{
    afs_toc_metadata_source_tags * toc_metadata_source_tags = gvector_create_pointers(0);
    afs_toc_metadata_source* toc_metadata_source1 = afs_toc_metadata_source_create9("afs/metadata-csv", 7, toc_metadata_source_tags, 8);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source1, "afs/metadata-csv", 7, NULL, "", 8);

    afs_toc_metadata_source* toc_metadata_source2 = afs_toc_metadata_source_clone(toc_metadata_source1);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source2, "afs/metadata-csv", 7, NULL, "", 8);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source2);
}
END_TEST


// Test function afs_toc_metadata_source_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_get_new_reference_test1)
{
    afs_toc_metadata_source * toc_metadata_source1 = NULL;

    BOXING_ASSERT(toc_metadata_source1 == NULL);

    afs_toc_metadata_source * toc_metadata_source2 = afs_toc_metadata_source_get_new_reference(toc_metadata_source1);

    BOXING_ASSERT(toc_metadata_source2 == NULL);

    toc_metadata_source1 = afs_toc_metadata_source_create();

    test_empty_afs_toc_metadata_source(toc_metadata_source1);
    BOXING_ASSERT(toc_metadata_source1->reference_count == 1);
    BOXING_ASSERT(toc_metadata_source2 == NULL);

    afs_toc_metadata_source_free(toc_metadata_source1);
}
END_TEST


// Test function afs_toc_metadata_source_get_new_reference with empty input afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_get_new_reference_test2)
{
    afs_toc_metadata_source * toc_metadata_source1 = afs_toc_metadata_source_create();

    test_empty_afs_toc_metadata_source(toc_metadata_source1);
    BOXING_ASSERT(toc_metadata_source1->reference_count == 1);

    afs_toc_metadata_source * toc_metadata_source2 = afs_toc_metadata_source_get_new_reference(toc_metadata_source1);

    test_empty_afs_toc_metadata_source(toc_metadata_source2);
    BOXING_ASSERT(toc_metadata_source1->reference_count == 2);
    BOXING_ASSERT(toc_metadata_source2->reference_count == 2);
    BOXING_ASSERT(toc_metadata_source1 == toc_metadata_source2);

    toc_metadata_source1->file_id = 99;

    BOXING_ASSERT(toc_metadata_source2->file_id == 99);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source1);
}
END_TEST


// Test function afs_toc_metadata_source_get_new_reference with normal input afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_get_new_reference_test3)
{
    afs_toc_metadata_source * toc_metadata_source1 = afs_toc_metadata_source_create7("afs/metadata-csv", 7, "some xml data", "tag1;tag2;tag3", 8);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source1, "afs/metadata-csv", 7, "some xml data", "tag1;tag2;tag3", 8);
    BOXING_ASSERT(toc_metadata_source1->reference_count == 1);

    afs_toc_metadata_source * toc_metadata_source2 = afs_toc_metadata_source_get_new_reference(toc_metadata_source1);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source2, "afs/metadata-csv", 7, "some xml data", "tag1;tag2;tag3", 8);
    BOXING_ASSERT(toc_metadata_source1->reference_count == 2);
    BOXING_ASSERT(toc_metadata_source2->reference_count == 2);
    BOXING_ASSERT(toc_metadata_source1 == toc_metadata_source2);

    toc_metadata_source1->file_id = 99;

    BOXING_ASSERT(toc_metadata_source2->file_id == 99);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source1);
}
END_TEST


// Test function afs_toc_metadata_source_get_new_reference with empty tags vector in the afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_get_new_reference_test4)
{
    afs_toc_metadata_source_tags * toc_metadata_source_tags = gvector_create_pointers(0);
    afs_toc_metadata_source * toc_metadata_source1 = afs_toc_metadata_source_create9("afs/metadata-csv", 7, toc_metadata_source_tags, 8);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source1, "afs/metadata-csv", 7, NULL, "", 8);
    BOXING_ASSERT(toc_metadata_source1->reference_count == 1);

    afs_toc_metadata_source * toc_metadata_source2 = afs_toc_metadata_source_get_new_reference(toc_metadata_source1);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source2, "afs/metadata-csv", 7, NULL, "", 8);
    BOXING_ASSERT(toc_metadata_source1->reference_count == 2);
    BOXING_ASSERT(toc_metadata_source2->reference_count == 2);
    BOXING_ASSERT(toc_metadata_source1 == toc_metadata_source2);

    toc_metadata_source1->file_id = 99;

    BOXING_ASSERT(toc_metadata_source2->file_id == 99);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source1);
}
END_TEST


// Test function afs_toc_metadata_source_equal test with input afs_toc_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_equal_test1)
{
    afs_toc_metadata_source* toc_metadata_source1 = NULL;
    afs_toc_metadata_source* toc_metadata_source2 = NULL;

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DTRUE);
}
END_TEST

// Test function afs_toc_metadata_source_equal test with empty input afs_toc_metadata_source pointers 
BOXING_START_TEST(afs_toc_metadata_source_equal_test2)
{
    afs_toc_metadata_source* toc_metadata_source1 = afs_toc_metadata_source_create();
    test_empty_afs_toc_metadata_source(toc_metadata_source1);

    afs_toc_metadata_source* toc_metadata_source2 = afs_toc_metadata_source_create();
    test_empty_afs_toc_metadata_source(toc_metadata_source2);

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source2);
}
END_TEST


// Test function afs_toc_metadata_source_equal test with normal input afs_toc_metadata_source pointers 
BOXING_START_TEST(afs_toc_metadata_source_equal_test3)
{
    afs_toc_metadata_source* toc_metadata_source1 = afs_toc_metadata_source_create7("afs/metadata-csv", 5, "some xml data", "tag1;tag2;tag3", 6);
    test_not_empty_afs_toc_metadata_source(toc_metadata_source1, "afs/metadata-csv", 5, "some xml data", "tag1;tag2;tag3", 6);

    afs_toc_metadata_source* toc_metadata_source2 = afs_toc_metadata_source_create7("afs/metadata-csv", 5, "some xml data", "tag1;tag2;tag3", 6);
    test_not_empty_afs_toc_metadata_source(toc_metadata_source1, "afs/metadata-csv", 5, "some xml data", "tag1;tag2;tag3", 6);

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DTRUE);

    afs_toc_metadata_source_set_tags(toc_metadata_source1, "");
    afs_toc_metadata_source_set_tags(toc_metadata_source2, "");

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source2);
}
END_TEST


// Test function afs_toc_metadata_source_equal test with empty input afs_toc_metadata_source pointer and pointer equal to NULL 
BOXING_START_TEST(afs_toc_metadata_source_equal_test4)
{
    afs_toc_metadata_source* toc_metadata_source1 = afs_toc_metadata_source_create();
    test_empty_afs_toc_metadata_source(toc_metadata_source1);

    afs_toc_metadata_source* toc_metadata_source2 = NULL;

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source1);
}
END_TEST


// Test function afs_toc_metadata_source_equal test with normal input afs_toc_metadata_source pointer and pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_equal_test5)
{
    afs_toc_metadata_source* toc_metadata_source1 = afs_toc_metadata_source_create7("afs/metadata-csv", 2, "some xml data", "tag1;tag2;tag3", 2);
    test_not_empty_afs_toc_metadata_source(toc_metadata_source1, "afs/metadata-csv", 2, "some xml data", "tag1;tag2;tag3", 2);

    afs_toc_metadata_source* toc_metadata_source2 = NULL;

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source1);
}
END_TEST


// Test function afs_toc_metadata_source_equal test with normal afs_toc_metadata_source pointer and empty afs_toc_metadata_source pointer
BOXING_START_TEST(afs_toc_metadata_source_equal_test6)
{
    afs_toc_metadata_source* toc_metadata_source1 = afs_toc_metadata_source_create7("afs/metadata-csv", 2, "some xml data", "tag1;tag2;tag3", 2);
    test_not_empty_afs_toc_metadata_source(toc_metadata_source1, "afs/metadata-csv", 2, "some xml data", "tag1;tag2;tag3", 2);

    afs_toc_metadata_source* toc_metadata_source2 = afs_toc_metadata_source_create();
    test_empty_afs_toc_metadata_source(toc_metadata_source2);

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source2);
}
END_TEST


// Test function afs_toc_metadata_source_equal test to afs_toc_metadata_source instances with different data
BOXING_START_TEST(afs_toc_metadata_source_equal_test7)
{
    afs_toc_metadata_source* toc_metadata_source1 = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);
    test_not_empty_afs_toc_metadata_source(toc_metadata_source1, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);

    afs_toc_metadata_source* toc_metadata_source2 = afs_toc_metadata_source_create7("afs/metadata-csv2", 3, "some xml data", "tag1;tag2;tag3", 4);
    test_not_empty_afs_toc_metadata_source(toc_metadata_source2, "afs/metadata-csv2", 3, "some xml data", "tag1;tag2;tag3", 4);

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    boxing_string_free(toc_metadata_source2->format);
    toc_metadata_source2->format = boxing_string_clone(toc_metadata_source1->format);
    toc_metadata_source2->id = 0;

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    toc_metadata_source2->id = toc_metadata_source1->id;
    toc_metadata_source2->file_id = 0;

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    toc_metadata_source2->file_id = toc_metadata_source1->file_id;
    toc_metadata_source2->data[0] = 'f';

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source2);
}
END_TEST


// Test function afs_toc_metadata_source_equal test to afs_toc_metadata_source instances with different tags
BOXING_START_TEST(afs_toc_metadata_source_equal_test8)
{
    afs_toc_metadata_source* toc_metadata_source1 = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", NULL, 4);
    test_not_empty_afs_toc_metadata_source(toc_metadata_source1, "afs/metadata-csv", 3, "some xml data", NULL, 4);

    afs_toc_metadata_source* toc_metadata_source2 = afs_toc_metadata_source_create7("afs/metadata-csv2", 3, "some xml data", "", 4);
    test_not_empty_afs_toc_metadata_source(toc_metadata_source2, "afs/metadata-csv2", 3, "some xml data", "", 4);

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    afs_toc_metadata_source_set_tags(toc_metadata_source2, "tag1,tag2,tag3");

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    afs_toc_metadata_source_set_tags(toc_metadata_source1, "");

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    afs_toc_metadata_source_set_tags(toc_metadata_source1, "tag3,tag2,tag1");

    test_equals_toc_matadata_sources(toc_metadata_source1, toc_metadata_source2, DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source2);
}
END_TEST


// Test function afs_toc_metadata_source_is_valid test with input afs_toc_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_is_valid_test1)
{
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_is_valid(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_source_is_valid test with empty afs_toc_metadata_source pointers
BOXING_START_TEST(afs_toc_metadata_source_is_valid_test2)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_is_valid(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_is_valid test with format string pointer equal to NULL 
BOXING_START_TEST(afs_toc_metadata_source_is_valid_test3)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7(NULL, 3, "some xml data", "tag1;tag2;tag3", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, NULL, 3, "some xml data", "tag1;tag2;tag3", 4);

    DBOOL result = afs_toc_metadata_source_is_valid(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_is_valid test with empty format string
BOXING_START_TEST(afs_toc_metadata_source_is_valid_test4)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("", 3, "some xml data", "tag1;tag2;tag3", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "", 3, "some xml data", "tag1;tag2;tag3", 4);

    DBOOL result = afs_toc_metadata_source_is_valid(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_is_valid test with incorrect file id value
BOXING_START_TEST(afs_toc_metadata_source_is_valid_test5)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", -2, "some xml data", "tag1;tag2;tag3", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", -2, "some xml data", "tag1;tag2;tag3", 4);

    DBOOL result = afs_toc_metadata_source_is_valid(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_is_valid test with xmldata pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_is_valid_test6)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", FILE_ID_UNDEFINED, NULL, "tag1;tag2;tag3", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", FILE_ID_UNDEFINED, NULL, "tag1;tag2;tag3", 4);

    DBOOL result = afs_toc_metadata_source_is_valid(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_is_valid with empty xmldata string
BOXING_START_TEST(afs_toc_metadata_source_is_valid_test7)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", FILE_ID_UNDEFINED, "", "tag1;tag2;tag3", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", FILE_ID_UNDEFINED, "", "tag1;tag2;tag3", 4);

    DBOOL result = afs_toc_metadata_source_is_valid(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_is_valid test with normal data and incorrect xmldata
BOXING_START_TEST(afs_toc_metadata_source_is_valid_test8)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "", "tag1;tag2;tag3", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "", "tag1;tag2;tag3", 4);

    DBOOL result = afs_toc_metadata_source_is_valid(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_is_valid test with normal data and undefined file id
BOXING_START_TEST(afs_toc_metadata_source_is_valid_test9)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", FILE_ID_UNDEFINED, "some xml data", "tag1;tag2;tag3", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", FILE_ID_UNDEFINED, "some xml data", "tag1;tag2;tag3", 4);

    DBOOL result = afs_toc_metadata_source_is_valid(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_set_tags test with input afs_toc_metadata_source pointers and tags string pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_set_tags_test1)
{
    afs_toc_metadata_source* toc_metadata_source = NULL;
    const char * tags = NULL;

    afs_toc_metadata_source_set_tags(toc_metadata_source, tags);

    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(tags == NULL);
}
END_TEST


// Test function afs_toc_metadata_source_set_tags test with input afs_toc_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_set_tags_test2)
{
    afs_toc_metadata_source* toc_metadata_source = NULL;
    const char * tags = "tag1;tag2;tag3";

    afs_toc_metadata_source_set_tags(toc_metadata_source, tags);

    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(boxing_string_equal(tags, "tag1;tag2;tag3") == DTRUE);
}
END_TEST


// Test function afs_toc_metadata_source_set_tags test with normal afs_toc_metadata_source instance and tags pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_set_tags_test3)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);
    const char * tags = NULL;

    afs_toc_metadata_source_set_tags(toc_metadata_source, tags);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);

    BOXING_ASSERT(tags == NULL);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_set_tags test with normal afs_toc_metadata_source instance and empty tags string
BOXING_START_TEST(afs_toc_metadata_source_set_tags_test4)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);
    const char * tags = "";

    afs_toc_metadata_source_set_tags(toc_metadata_source, tags);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);

    BOXING_ASSERT(boxing_string_equal(tags, "") == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_set_tags test with normal afs_toc_metadata_source instance and tags string with one value
BOXING_START_TEST(afs_toc_metadata_source_set_tags_test5)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);
    const char * tags = "tag4";

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);

    afs_toc_metadata_source_set_tags(toc_metadata_source, tags);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag4", 4);

    BOXING_ASSERT(boxing_string_equal(tags, "tag4") == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_set_tags test with normal afs_toc_metadata_source instance and tags string with three values
BOXING_START_TEST(afs_toc_metadata_source_set_tags_test6)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);
    const char * tags = "tag4;tag5;tag6";

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);

    afs_toc_metadata_source_set_tags(toc_metadata_source, tags);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag4;tag5;tag6", 4);

    BOXING_ASSERT(boxing_string_equal(tags, "tag4;tag5;tag6") == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_set_tags test with normal afs_toc_metadata_source instance and tags string with three empty values
BOXING_START_TEST(afs_toc_metadata_source_set_tags_test7)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);
    const char * tags = ";;";

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3", 4);

    afs_toc_metadata_source_set_tags(toc_metadata_source, tags);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", ";;", 4);

    BOXING_ASSERT(boxing_string_equal(tags, ";;") == DTRUE);
    BOXING_ASSERT(toc_metadata_source->tags->size == 3);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_get_tags test with input afs_toc_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_get_tags_test1)
{
    afs_toc_metadata_source* toc_metadata_source = NULL;

    const char * tags = afs_toc_metadata_source_get_tags(toc_metadata_source);

    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(tags == NULL);
}
END_TEST


// Test function afs_toc_metadata_source_get_tags test with tags pointers equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_get_tags_test2)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create9("afs/metadata-csv", 3, NULL, 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, NULL, NULL, 4);

    const char * tags = afs_toc_metadata_source_get_tags(toc_metadata_source);

    BOXING_ASSERT(tags == NULL);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_get_tags test with empty tags vector
BOXING_START_TEST(afs_toc_metadata_source_get_tags_test3)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "", 4);

    char * tags = afs_toc_metadata_source_get_tags(toc_metadata_source);

    BOXING_ASSERT(boxing_string_equal(tags, "") == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
    boxing_string_free(tags);
}
END_TEST


// Test function afs_toc_metadata_source_get_tags test with three empty tags
BOXING_START_TEST(afs_toc_metadata_source_get_tags_test4)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", ";;", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", ";;", 4);

    char * tags = afs_toc_metadata_source_get_tags(toc_metadata_source);

    BOXING_ASSERT(boxing_string_equal(tags, ";;") == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
    boxing_string_free(tags);
}
END_TEST


// Test function afs_toc_metadata_source_get_tags test with one tag
BOXING_START_TEST(afs_toc_metadata_source_get_tags_test5)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1", 4);

    char * tags = afs_toc_metadata_source_get_tags(toc_metadata_source);

    BOXING_ASSERT(boxing_string_equal(tags, "tag1") == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
    boxing_string_free(tags);
}
END_TEST


// Test function afs_toc_metadata_source_get_tags test with four tags
BOXING_START_TEST(afs_toc_metadata_source_get_tags_test6)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    char * tags = afs_toc_metadata_source_get_tags(toc_metadata_source);

    BOXING_ASSERT(boxing_string_equal(tags, "tag1;tag2;tag3;tag4") == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
    boxing_string_free(tags);
}
END_TEST


// Test function afs_toc_metadata_source_save_file with file_name pointer equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_save_file(toc_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_source_save_file with file_name pointer equal to NULL and afs_toc_metadata_source pointer not equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    DBOOL result = afs_toc_metadata_source_save_file(toc_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_save_file with file_name pointer not equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_save_file_test3)
{
    const char * file_name = "afs_toc_metadata_source_save_file_test.xml";
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_save_file(toc_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_source_save_file with empty afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_save_file_test4)
{
    const char * file_name = "afs_toc_metadata_source_save_file_empty_test.xml";
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_save_file(toc_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_save_file with not valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_save_file_test5)
{
    const char * file_name = "afs_toc_metadata_source_save_file_empty_test.xml";
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", FILE_ID_UNDEFINED, "", "tag1;tag2;tag3", 4);

    DBOOL result = afs_toc_metadata_source_save_file(toc_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_save_file with initialized toc_metadata_source instance, in compact format
BOXING_START_TEST(afs_toc_metadata_source_save_file_test6)
{
    const char * file_name = "afs_toc_metadata_source_save_file_compact_test.xml";

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    DBOOL result = afs_toc_metadata_source_save_file(toc_metadata_source, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_save_file with initialized toc_metadata_source instance, in not compact format
BOXING_START_TEST(afs_toc_metadata_source_save_file_test7)
{
    const char * file_name = "afs_toc_metadata_source_save_file_not_compact_test.xml";

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    DBOOL result = afs_toc_metadata_source_save_file(toc_metadata_source, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_save_file with initialized toc_metadata_source instance with empty XML data string
BOXING_START_TEST(afs_toc_metadata_source_save_file_test8)
{
    const char * file_name = "afs_toc_metadata_source_save_file_empty_data.xml";

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "", "tag1;tag2;tag3;tag4", 4);

    DBOOL result = afs_toc_metadata_source_save_file(toc_metadata_source, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_save_string with afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_save_string_test1)
{
    afs_toc_metadata_source* toc_metadata_source = NULL;

    char * result = afs_toc_metadata_source_save_string(toc_metadata_source, DTRUE);

    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_metadata_source_save_string with empty toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_save_string_test2)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    char * result = afs_toc_metadata_source_save_string(toc_metadata_source, DTRUE);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_save_string with not valid toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_save_string_test3)
{
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", FILE_ID_UNDEFINED, "", "tag1;tag2;tag3", 4);

    char * result = afs_toc_metadata_source_save_string(toc_metadata_source, DTRUE);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_save_string with initialized toc_metadata_source instance, in compact format
BOXING_START_TEST(afs_toc_metadata_source_save_string_test4)
{
    char * test_string = read_xml_toc_metadata_source_file("afs_toc_metadata_source_save_file_compact_test.xml");

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    char * result = afs_toc_metadata_source_save_string(toc_metadata_source, DTRUE);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(boxing_string_equal(result, test_string) == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_toc_metadata_source_save_string with initialized afs_toc_metadata_source instance, in not compact format
BOXING_START_TEST(afs_toc_metadata_source_save_string_test5)
{
    char * test_string = read_xml_toc_metadata_source_file("afs_toc_metadata_source_save_file_not_compact_test.xml");

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    char * result = afs_toc_metadata_source_save_string(toc_metadata_source, DFALSE);

    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(boxing_string_equal(result, test_string) == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_toc_metadata_source_save_xml with mxml_node_t pointer equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_save_xml(toc_metadata_source, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_source_save_xml with mxml_node_t pointer equal to NULL and afs_toc_metadata_source pointer not equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    DBOOL result = afs_toc_metadata_source_save_xml(toc_metadata_source, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_save_xml(toc_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_source_save_xml with empty afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_save_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_save_xml(toc_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_source_save_xml with not valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", FILE_ID_UNDEFINED, "", "tag1;tag2;tag3", 4);

    DBOOL result = afs_toc_metadata_source_save_xml(toc_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_source_save_xml with initialized afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    DBOOL result = afs_toc_metadata_source_save_xml(toc_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_metadata_source_free(toc_metadata_source);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_source_load_file with file_name pointer equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_load_file(toc_metadata_source, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_source_load_file with file_name pointer equal to NULL and afs_toc_metadata_source pointer not equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_file(toc_metadata_source, file_name);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_load_file with file_name pointer not equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_load_file_test3)
{
    const char * file_name = "afs_toc_metadata_source_save_file_compact_test.xml";
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_load_file(toc_metadata_source, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_source_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_metadata_source_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_file(toc_metadata_source, file_name);

    test_empty_afs_toc_metadata_source(toc_metadata_source);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_load_file with initialized afs_toc_metadata_source instance, in compact format
BOXING_START_TEST(afs_toc_metadata_source_load_file_test5)
{
    const char * file_name = "afs_toc_metadata_source_save_file_compact_test.xml";
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_file(toc_metadata_source, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);
    
    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_load_file with initialized afs_toc_metadata_source instance, in not compact format
BOXING_START_TEST(afs_toc_metadata_source_load_file_test6)
{
    const char * file_name = "afs_toc_metadata_source_save_file_not_compact_test.xml";
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_file(toc_metadata_source, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_load_file with initialized afs_toc_metadata_source instance with empty XML data
BOXING_START_TEST(afs_toc_metadata_source_load_file_test7)
{
    const char * file_name = "afs_toc_metadata_source_save_file_empty_data.xml";
    afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_file(toc_metadata_source, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "", "tag1;tag2;tag3;tag4", 4);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_load_string with input string pointer equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_load_string(toc_metadata_source, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_source_load_string with input string pointer equal to NULL and afs_toc_metadata_source pointer not equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_string(toc_metadata_source, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_load_string with input string pointer not equal to NULL and toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_load_string_test3)
{
    char * input_string = read_xml_toc_metadata_source_file("afs_toc_metadata_source_save_file_not_compact_test.xml");
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_load_string(toc_metadata_source, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);

    boxing_string_free(input_string);
}
END_TEST


// Test function afs_toc_metadata_source_load_string with empty input string
BOXING_START_TEST(afs_toc_metadata_source_load_string_test4)
{
    char * input_string = "";
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_string(toc_metadata_source, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_load_string with input string for initialized afs_toc_metadata_source instance, in compact format
BOXING_START_TEST(afs_toc_metadata_source_load_string_test5)
{
    char * input_string = read_xml_toc_metadata_source_file("afs_toc_metadata_source_save_file_compact_test.xml");

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_string(toc_metadata_source, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    afs_toc_metadata_source_free(toc_metadata_source);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_toc_metadata_source_load_string with input string for initialized afs_toc_metadata_source instance, in not compact format
BOXING_START_TEST(afs_toc_metadata_source_load_string_test6)
{
    char * input_string = read_xml_toc_metadata_source_file("afs_toc_metadata_source_save_file_not_compact_test.xml");
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_string(toc_metadata_source, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    afs_toc_metadata_source_free(toc_metadata_source);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_toc_metadata_source_load_xml with xml instance pointer equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_load_xml(toc_metadata_source, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_metadata_source_load_xml with xml instance pointer equal to NULL and afs_toc_metadata_source pointer not equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_xml(toc_metadata_source, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
}
END_TEST


// Test function afs_toc_metadata_source_load_xml with xml instance pointer not equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_metadata_source_load_xml_test3)
{
    mxml_node_t *tree = get_toc_metadata_source_xml_instance("afs_toc_metadata_source_save_file_compact_test.xml");
    afs_toc_metadata_source* toc_metadata_source = NULL;

    DBOOL result = afs_toc_metadata_source_load_xml(toc_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata_source == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_source_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_metadata_source_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_xml(toc_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(toc_metadata_source);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_metadata_source_load_xml with xml instance for initialized afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_metadata_source_load_xml_test5)
{
    mxml_node_t *tree = get_toc_metadata_source_xml_instance("afs_toc_metadata_source_save_file_compact_test.xml");

    afs_toc_metadata_source* toc_metadata_source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_metadata_source_load_xml(toc_metadata_source, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata_source != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    afs_toc_metadata_source_free(toc_metadata_source);
    mxmlDelete(tree);
}
END_TEST


// Test save/load XML functions
BOXING_START_TEST(afs_toc_metadata_source_save_load_xml_test1)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_metadata_source* toc_metadata_source1 = afs_toc_metadata_source_create7("afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    DBOOL result = afs_toc_metadata_source_save_xml(toc_metadata_source1, tree);

    afs_toc_metadata_source* toc_metadata_source2 = afs_toc_metadata_source_create();

    result = afs_toc_metadata_source_load_xml(toc_metadata_source2, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_metadata_source2 != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_metadata_source(toc_metadata_source2, "afs/metadata-csv", 3, "some xml data", "tag1;tag2;tag3;tag4", 4);

    afs_toc_metadata_source_free(toc_metadata_source1);
    afs_toc_metadata_source_free(toc_metadata_source2);
    mxmlDelete(tree);
}
END_TEST



Suite * tocmetadatasource_tests(void)
{
    TCase * tc_tocmetadatasource_functions_tests = tcase_create("tocmetadatasource_functions_tests");

    // Test function afs_toc_metadata_source_create
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create_test1);
    // Test function afs_toc_metadata_source_create2
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create2_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create2_test2);
    // Test function afs_toc_metadata_source_create3
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create3_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create3_test2);
    // Test function afs_toc_metadata_source_create4
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create4_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create4_test2);
    // Test function afs_toc_metadata_source_create5
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create5_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create5_test2);
    // Test function afs_toc_metadata_source_create6
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create6_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create6_test2);
    // Test function afs_toc_metadata_source_create7
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create7_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create7_test2);
    // Test function afs_toc_metadata_source_create8
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create8_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create8_test2);
    // Test function afs_toc_metadata_source_create9
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create9_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_create9_test2);
    // Test function afs_toc_metadata_source_init
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_init_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_init_test2);
    // Test function afs_toc_metadata_source_init2
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_init2_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_init2_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_init2_test3);
    // Test function afs_toc_metadata_source_init3
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_init3_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_init3_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_init3_test3);
    // Test function afs_toc_metadata_source_clone
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_clone_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_clone_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_clone_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_clone_test4);
    // Test function afs_toc_metadata_source_get_new_reference
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_get_new_reference_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_get_new_reference_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_get_new_reference_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_get_new_reference_test4);
    // Test function afs_toc_metadata_source_equal
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_equal_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_equal_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_equal_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_equal_test4);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_equal_test5);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_equal_test6);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_equal_test7);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_equal_test8);
    // Test function afs_toc_metadata_source_is_valid
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_is_valid_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_is_valid_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_is_valid_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_is_valid_test4);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_is_valid_test5);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_is_valid_test6);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_is_valid_test7);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_is_valid_test8);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_is_valid_test9);
    // Test function afs_toc_metadata_source_set_tags
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_set_tags_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_set_tags_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_set_tags_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_set_tags_test4);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_set_tags_test5);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_set_tags_test6);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_set_tags_test7);
    // Test function afs_toc_metadata_source_get_tags
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_get_tags_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_get_tags_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_get_tags_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_get_tags_test4);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_get_tags_test5);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_get_tags_test6);
    // Test function afs_toc_metadata_source_save_file
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_file_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_file_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_file_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_file_test4);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_file_test5);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_file_test6);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_file_test7);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_file_test8);
    // Test function afs_toc_metadata_source_save_string
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_string_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_string_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_string_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_string_test4);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_string_test5);
    // Test function afs_toc_metadata_source_save_xml
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_xml_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_xml_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_xml_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_xml_test4);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_xml_test5);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_xml_test6);
    // Test function afs_toc_metadata_source_load_file
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_file_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_file_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_file_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_file_test4);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_file_test5);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_file_test6);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_file_test7);
    // Test function afs_toc_metadata_source_load_string
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_string_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_string_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_string_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_string_test4);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_string_test5);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_string_test6);
    // Test function afs_toc_metadata_source_load_xml
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_xml_test1);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_xml_test2);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_xml_test3);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_xml_test4);
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_load_xml_test5);
    // Test save/load XML function
    tcase_add_test(tc_tocmetadatasource_functions_tests, afs_toc_metadata_source_save_load_xml_test1);

    Suite * s = suite_create("tocmetadatasource_test_util");
    suite_add_tcase(s, tc_tocmetadatasource_functions_tests);

    return s;
}
