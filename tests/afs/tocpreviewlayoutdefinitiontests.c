#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "toc/previewlayoutdefinition.h"
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


static char * read_xml_toc_preview_layout_definition_file(const char* file_name)
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


static mxml_node_t * get_toc_preview_layout_definition_xml_instance(const char * file_name)
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


static void test_empty_afs_toc_preview_layout_definition(afs_toc_preview_layout_definition * toc_preview_layout_definition)
{
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(toc_preview_layout_definition->id == NULL);
    BOXING_ASSERT(toc_preview_layout_definition->name == NULL);
    BOXING_ASSERT(toc_preview_layout_definition->sections == NULL);
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
    BOXING_ASSERT(strcmp(toc_preview_layout_definition->id, id) == 0);
    BOXING_ASSERT(strcmp(toc_preview_layout_definition->name, name) == 0);

    if (sections_is_null == DTRUE)
    {
        BOXING_ASSERT(toc_preview_layout_definition->sections == NULL);
    }
    else
    {
        test_not_empty_afs_toc_preview_sections(toc_preview_layout_definition->sections, sections_count);
    }
}


static void test_empty_sections_afs_toc_preview_layout_definition(afs_toc_preview_layout_definition * toc_preview_layout_definition, const char * id, const char * name)
{
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(strcmp(toc_preview_layout_definition->id, id) == 0);
    BOXING_ASSERT(strcmp(toc_preview_layout_definition->name, name) == 0);

    BOXING_ASSERT(toc_preview_layout_definition->sections != NULL);
    BOXING_ASSERT(toc_preview_layout_definition->sections->sections == NULL);
}


afs_toc_preview_sections * get_afs_toc_preview_sections_instance2(unsigned int sections_count)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    for (unsigned int i = 0; i < sections_count; i++)
    {
        afs_toc_preview_sections_append(toc_preview_sections, afs_toc_preview_section_create2(0 + i, 100 * i, 100 * i, 64 + i, 48 + i, (90 * i) % 360));
    }

    return toc_preview_sections;
}


afs_toc_preview_layout_definition * get_afs_toc_preview_layout_definition_instance(const char * id, const char * name, unsigned int sections_count)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create2(id, name);
    toc_preview_layout_definition->sections = get_afs_toc_preview_sections_instance2(sections_count);

    return toc_preview_layout_definition;
}


static void test_equals_toc_preview_layout_definition(afs_toc_preview_layout_definition * toc_preview_layout_definition1, afs_toc_preview_layout_definition * toc_preview_layout_definition2, DBOOL expected_result)
{
    DBOOL result = afs_toc_preview_layout_definition_equal(toc_preview_layout_definition2, toc_preview_layout_definition1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_preview_layout_definition_equal(toc_preview_layout_definition1, toc_preview_layout_definition2);
    BOXING_ASSERT(result == expected_result);
}


// TEST TOC PREVIEW LAYOUT DEFINITION FUNCTIONS
//

// Test function afs_toc_preview_layout_definition_create
BOXING_START_TEST(afs_toc_preview_layout_definition_create_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create2 test1 with empty initial values
BOXING_START_TEST(afs_toc_preview_layout_definition_create2_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create2(NULL, NULL);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create2 test2 with normal id value and NULL name pointer
BOXING_START_TEST(afs_toc_preview_layout_definition_create2_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create2("1", NULL);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "1", NULL, 0, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create2 test3 with NULL id pointer and normal name value
BOXING_START_TEST(afs_toc_preview_layout_definition_create2_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create2(NULL, "some name");

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, NULL, "some name", 0, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create2 test4 with normal values
BOXING_START_TEST(afs_toc_preview_layout_definition_create2_test4)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create2("1", "some name");

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "1", "some name", 0, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create3 test1 with empty initial values
BOXING_START_TEST(afs_toc_preview_layout_definition_create3_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create3(NULL, NULL, NULL);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create3 test2 with id and name pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_create3_test2)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create3(NULL, NULL, toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, NULL, NULL, 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create3 test3 with id and sections pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_create3_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create3(NULL, "some name", NULL);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, NULL, "some name", 0, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create3 test4 with name and sections pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_create3_test4)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create3("1", NULL, NULL);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "1", NULL, 0, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create3 test5 with id pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_create3_test5)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create3(NULL, "some name", toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, NULL, "some name", 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create3 test6 with name pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_create3_test6)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create3("1", NULL, toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "1", NULL, 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create3 test7 with sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_create3_test7)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create3("1", "some name", NULL);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "1", "some name", 0, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create3 test8 with normal values
BOXING_START_TEST(afs_toc_preview_layout_definition_create3_test8)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create3("1", "some name", toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "1", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create4 test1 with XML node pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_create4_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create4(NULL);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create4 test2 with empty XML node
BOXING_START_TEST(afs_toc_preview_layout_definition_create4_test2)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create4(tree);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definition_create4 test3 with correct XML node
BOXING_START_TEST(afs_toc_preview_layout_definition_create4_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("1", "some name", 3);

    mxml_node_t * tree = mxmlNewXML("1.0");
    DBOOL result = afs_toc_preview_layout_definition_save_xml(toc_preview_layout_definition1, tree);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_create4(tree);
    
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, "1", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definition_init with afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_init_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    afs_toc_preview_layout_definition_init(toc_preview_layout_definition, "0", "some name");

    BOXING_ASSERT(toc_preview_layout_definition == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definition_init with normal initialization data
BOXING_START_TEST(afs_toc_preview_layout_definition_init_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_init(toc_preview_layout_definition, "0", "some name");

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "0", "some name", 0, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_init with empty initialization data
BOXING_START_TEST(afs_toc_preview_layout_definition_init_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = malloc(sizeof(afs_toc_preview_layout_definition));
    toc_preview_layout_definition->sections = NULL;

    afs_toc_preview_layout_definition_init(toc_preview_layout_definition, NULL, NULL);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST

// Test function afs_toc_preview_layout_definition_init2 with all input pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_init2_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    afs_toc_preview_layout_definition_init2(toc_preview_layout_definition, NULL, NULL, NULL);

    BOXING_ASSERT(toc_preview_layout_definition == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definition_init2 with id, name and sections pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_init2_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    afs_toc_preview_layout_definition_init2(toc_preview_layout_definition, NULL, NULL, NULL);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_init2 with afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_init2_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);

    afs_toc_preview_layout_definition_init2(toc_preview_layout_definition, "some id", "some name", toc_preview_sections);

    BOXING_ASSERT(toc_preview_layout_definition == NULL);

    afs_toc_preview_sections_free(toc_preview_sections);
}
END_TEST


// Test function afs_toc_preview_layout_definition_init2 with id pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_init2_test4)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);

    afs_toc_preview_layout_definition_init2(toc_preview_layout_definition, NULL, "some name", toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, NULL, "some name", 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_init2 with name pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_init2_test5)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);

    afs_toc_preview_layout_definition_init2(toc_preview_layout_definition, "some id", NULL, toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", NULL, 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_init2 with sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_init2_test6)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    
    afs_toc_preview_layout_definition_init2(toc_preview_layout_definition, "some id", "some name", NULL);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 0, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_init2 with empty sections instance
BOXING_START_TEST(afs_toc_preview_layout_definition_init2_test7)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();

    afs_toc_preview_layout_definition_init2(toc_preview_layout_definition, "some id", "some name", toc_preview_sections);

    test_empty_sections_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name");

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_init2 with normal initialization data
BOXING_START_TEST(afs_toc_preview_layout_definition_init2_test8)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);

    afs_toc_preview_layout_definition_init2(toc_preview_layout_definition, "some id", "some name", toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_clone with input pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_clone_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = NULL;

    BOXING_ASSERT(toc_preview_layout_definition1 == NULL);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_clone(toc_preview_layout_definition1);

    BOXING_ASSERT(toc_preview_layout_definition2 == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definition_clone with empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_clone_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create();

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_clone(toc_preview_layout_definition1);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_clone with normal id value
BOXING_START_TEST(afs_toc_preview_layout_definition_clone_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create2("some id", NULL);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", NULL, 0, DTRUE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_clone(toc_preview_layout_definition1);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, "some id", NULL, 0, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_clone with normal name value
BOXING_START_TEST(afs_toc_preview_layout_definition_clone_test4)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create2(NULL, "some name");

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, NULL, "some name", 0, DTRUE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_clone(toc_preview_layout_definition1);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, NULL, "some name", 0, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_clone with empty sections instance
BOXING_START_TEST(afs_toc_preview_layout_definition_clone_test5)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create3(NULL, NULL, toc_preview_sections);

    BOXING_ASSERT(toc_preview_layout_definition1 != NULL);
    BOXING_ASSERT(toc_preview_layout_definition1->id == NULL);
    BOXING_ASSERT(toc_preview_layout_definition1->name == NULL);
    BOXING_ASSERT(toc_preview_layout_definition1->sections != NULL);
    BOXING_ASSERT(toc_preview_layout_definition1->sections->sections == NULL);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_clone(toc_preview_layout_definition1);

    BOXING_ASSERT(toc_preview_layout_definition2 != NULL);
    BOXING_ASSERT(toc_preview_layout_definition2->id == NULL);
    BOXING_ASSERT(toc_preview_layout_definition2->name == NULL);
    BOXING_ASSERT(toc_preview_layout_definition2->sections != NULL);
    BOXING_ASSERT(toc_preview_layout_definition2->sections->sections == NULL);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_clone with normal sections instance
BOXING_START_TEST(afs_toc_preview_layout_definition_clone_test6)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create3(NULL, NULL, toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, NULL, NULL, 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_clone(toc_preview_layout_definition1);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, NULL, NULL, 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_clone with normal values
BOXING_START_TEST(afs_toc_preview_layout_definition_clone_test7)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create3("some id", "some name", toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_clone(toc_preview_layout_definition1);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, "some id", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_get_new_reference_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = NULL;

    BOXING_ASSERT(toc_preview_layout_definition1 == NULL);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_get_new_reference(toc_preview_layout_definition1);

    BOXING_ASSERT(toc_preview_layout_definition2 == NULL);

    toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create();

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 1);
    BOXING_ASSERT(toc_preview_layout_definition2 == NULL);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_new_reference with empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_get_new_reference_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create();

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 1);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_get_new_reference(toc_preview_layout_definition1);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition2->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition1 == toc_preview_layout_definition2);

    toc_preview_layout_definition1->name = boxing_string_clone("Some other name!");

    BOXING_ASSERT(strcmp(toc_preview_layout_definition2->name, "Some other name!") == 0);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_new_reference with normal id value
BOXING_START_TEST(afs_toc_preview_layout_definition_get_new_reference_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create2("some id", NULL);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", NULL, 0, DTRUE);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 1);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_get_new_reference(toc_preview_layout_definition1);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, "some id", NULL, 0, DTRUE);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition2->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition1 == toc_preview_layout_definition2);

    toc_preview_layout_definition1->name = boxing_string_clone("Some other name!");

    BOXING_ASSERT(strcmp(toc_preview_layout_definition2->name, "Some other name!") == 0);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_new_reference with normal name value
BOXING_START_TEST(afs_toc_preview_layout_definition_get_new_reference_test4)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create2(NULL, "some name");

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, NULL, "some name", 0, DTRUE);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 1);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_get_new_reference(toc_preview_layout_definition1);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, NULL, "some name", 0, DTRUE);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition2->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition1 == toc_preview_layout_definition2);

    free(toc_preview_layout_definition1->name);
    toc_preview_layout_definition1->name = boxing_string_clone("Some other name!");

    BOXING_ASSERT(strcmp(toc_preview_layout_definition2->name, "Some other name!") == 0);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_new_reference with empty sections instance
BOXING_START_TEST(afs_toc_preview_layout_definition_get_new_reference_test5)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create3(NULL, NULL, toc_preview_sections);

    BOXING_ASSERT(toc_preview_layout_definition1 != NULL);
    BOXING_ASSERT(toc_preview_layout_definition1->id == NULL);
    BOXING_ASSERT(toc_preview_layout_definition1->name == NULL);
    BOXING_ASSERT(toc_preview_layout_definition1->sections != NULL);
    BOXING_ASSERT(toc_preview_layout_definition1->sections->sections == NULL);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 1);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_get_new_reference(toc_preview_layout_definition1);

    BOXING_ASSERT(toc_preview_layout_definition2 != NULL);
    BOXING_ASSERT(toc_preview_layout_definition2->id == NULL);
    BOXING_ASSERT(toc_preview_layout_definition2->name == NULL);
    BOXING_ASSERT(toc_preview_layout_definition2->sections != NULL);
    BOXING_ASSERT(toc_preview_layout_definition2->sections->sections == NULL);

    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition2->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition1 == toc_preview_layout_definition2);

    toc_preview_layout_definition1->name = boxing_string_clone("Some other name!");

    BOXING_ASSERT(strcmp(toc_preview_layout_definition2->name, "Some other name!") == 0);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_new_reference with normal sections instance
BOXING_START_TEST(afs_toc_preview_layout_definition_get_new_reference_test6)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create3(NULL, NULL, toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, NULL, NULL, 3, DFALSE);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 1);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_get_new_reference(toc_preview_layout_definition1);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, NULL, NULL, 3, DFALSE);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition2->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition1 == toc_preview_layout_definition2);

    toc_preview_layout_definition1->name = boxing_string_clone("Some other name!");

    BOXING_ASSERT(strcmp(toc_preview_layout_definition2->name, "Some other name!") == 0);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_new_reference with normal values
BOXING_START_TEST(afs_toc_preview_layout_definition_get_new_reference_test7)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create3("some id", "some name", toc_preview_sections);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", "some name", 3, DFALSE);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 1);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_get_new_reference(toc_preview_layout_definition1);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, "some id", "some name", 3, DFALSE);
    BOXING_ASSERT(toc_preview_layout_definition1->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition2->reference_count == 2);
    BOXING_ASSERT(toc_preview_layout_definition1 == toc_preview_layout_definition2);

    free(toc_preview_layout_definition1->name);
    toc_preview_layout_definition1->name = boxing_string_clone("Some other name!");

    BOXING_ASSERT(strcmp(toc_preview_layout_definition2->name, "Some other name!") == 0);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
}
END_TEST


// Test function toc_preview_layout_definition_equal test with input afs_toc_preview_layout_definition pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = NULL;

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DTRUE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal with empty input afs_toc_preview_layout_definition pointers 
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create();
    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_create();
    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2);

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal with input afs_toc_preview_layout_definition instances with empty sections instances 
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test3)
{
    afs_toc_preview_sections * toc_preview_sections1 = afs_toc_preview_sections_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create3(NULL, NULL, toc_preview_sections1);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_create3(NULL, NULL, toc_preview_sections2);

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST

// Test function afs_toc_preview_layout_definition_equal with input afs_toc_preview_layout_definition instances with empty sections instances 
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test4)
{
    afs_toc_preview_sections * toc_preview_sections1 = afs_toc_preview_sections_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create3("some id", "some name", toc_preview_sections1);

    afs_toc_preview_sections * toc_preview_sections2 = afs_toc_preview_sections_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_create3("some id", "some name", toc_preview_sections2);

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with normal input afs_toc_preview_layout_definition instances 
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test5)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, "some id", "some name", 3, DFALSE);

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with NULL id value 
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test6)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance(NULL, "some name", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, NULL, "some name", 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = get_afs_toc_preview_layout_definition_instance(NULL, "some name", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, NULL, "some name", 3, DFALSE);

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with NULL name value 
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test7)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("some id", NULL, 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", NULL, 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = get_afs_toc_preview_layout_definition_instance("some id", NULL, 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, "some id", NULL, 3, DFALSE);

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with empty input afs_toc_preview_layout_definition instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test8)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = afs_toc_preview_layout_definition_create();
    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = NULL;

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with instance with empty sections and pointer equal to NULL 
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test9)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("some id", "some name", 0);
    test_empty_sections_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", "some name");

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = NULL;

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with normal input afs_toc_preview_layout_definition pointer and pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test10)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = NULL;

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with instance with empty sections and empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test11)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("some id", "some name", 0);
    test_empty_sections_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", "some name");

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_create();

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with normal instance and empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test12)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = afs_toc_preview_layout_definition_create();

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with normal instance and instance with empty sections
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test13)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = get_afs_toc_preview_layout_definition_instance("some id", "some name", 0);

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with instances with different id values
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test14)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("some id 1", "some name", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id 1", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = get_afs_toc_preview_layout_definition_instance("some id 2", "some name", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, "some id 2", "some name", 3, DFALSE);

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with instances with different name values
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test15)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("some id", "some name 1", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", "some name 1", 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = get_afs_toc_preview_layout_definition_instance("some id", "some name 2", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, "some id", "some name 2", 3, DFALSE);

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_equal test with instances with different sections
BOXING_START_TEST(afs_toc_preview_layout_definition_equal_test16)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition1 = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition1, "some id", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition * toc_preview_layout_definition2 = get_afs_toc_preview_layout_definition_instance("some id", "some name", 4);
    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition2, "some id", "some name", 4, DFALSE);

    test_equals_toc_preview_layout_definition(toc_preview_layout_definition1, toc_preview_layout_definition2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition1);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition2);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with input afs_toc_preview_layout_definition pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with id string pointer equal to NULL 
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance(NULL, "some name", 3);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, NULL, "some name", 3, DFALSE);

    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with empty id string
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test4)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("", "some name", 3);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "", "some name", 3, DFALSE);

    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with name string pointer equal to NULL 
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test5)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", NULL, 3);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", NULL, 3, DFALSE);

    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with empty name string
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test6)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "", 3);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "", 3, DFALSE);

    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with sections pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test7)
{
    afs_toc_preview_sections * toc_preview_sections = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create3("some id", "some name", toc_preview_sections);
 
    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with empty sections
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test8)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 0);

    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with not valid sections
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test9)
{
    afs_toc_preview_sections * toc_preview_sections = get_afs_toc_preview_sections_instance2(3);
    GVECTORN(toc_preview_sections->sections, afs_toc_preview_section *, 1)->width = 0;

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create3("some id", "some name", toc_preview_sections);

    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with normal values
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test10)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);

    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_valid with overlapping sections
BOXING_START_TEST(afs_toc_preview_layout_definition_is_valid_test11)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, 1);
    toc_preview_section->x = 0;
    toc_preview_section->y = 0;

    DBOOL result = afs_toc_preview_layout_definition_is_valid(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_section_count with input afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_get_section_count_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;
    unsigned int sections_count = afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(sections_count == 0);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_section_count with empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_get_section_count_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    unsigned int sections_count = afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);
    BOXING_ASSERT(sections_count == 0);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_section_count with empty sections instance
BOXING_START_TEST(afs_toc_preview_layout_definition_get_section_count_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 0);
    unsigned int sections_count = afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition);

    BOXING_ASSERT(sections_count == 0);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_section_count with filled sections instance
BOXING_START_TEST(afs_toc_preview_layout_definition_get_section_count_test4)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);
    unsigned int sections_count = afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition);

    BOXING_ASSERT(sections_count == 3);
    
    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, afs_toc_preview_section_create2(3, 300, 300, 67, 51, 270));

    BOXING_ASSERT(result == DTRUE);

    sections_count = afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition);
    BOXING_ASSERT(sections_count == 4);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_section with input afs_toc_preview_layout_definition pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_get_section_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, 0);

    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(toc_preview_section == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_section with empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_get_section_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, 0);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);
    BOXING_ASSERT(toc_preview_section == NULL);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_section with afs_toc_preview_layout_definition instance with empty sections
BOXING_START_TEST(afs_toc_preview_layout_definition_get_section_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 0);
    
    afs_toc_preview_section * toc_preview_section = afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, 0);

    BOXING_ASSERT(toc_preview_section == NULL);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_section with not empty afs_toc_preview_layout_definition instance and index out of range
BOXING_START_TEST(afs_toc_preview_layout_definition_get_section_test4)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);

    afs_toc_preview_section * toc_preview_section = afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, 4);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 3, DFALSE);
    BOXING_ASSERT(toc_preview_section == NULL);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_get_section with not empty afs_toc_preview_layout_definition instance and index in range
BOXING_START_TEST(afs_toc_preview_layout_definition_get_section_test5)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 3);

    for (unsigned int i = 0; i < afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition); i++)
    {
        test_not_empty_afs_toc_preview_section(afs_toc_preview_layout_definition_get_section(toc_preview_layout_definition, i), 0 + i, 100 * i, 100 * i, 64 + i, 48 + i, (90 * i) % 360);
    }

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with input afs_toc_preview_layout_definition pointers equal to NULL and afs_toc_preview_section pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;
    afs_toc_preview_section * section = NULL;

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with input afs_toc_preview_layout_definition pointers equal to NULL and valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;
    afs_toc_preview_section * section = afs_toc_preview_section_create2(3, 300, 300, 67, 51, 270);

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with input afs_toc_preview_layout_definition pointers equal to NULL and not valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;
    afs_toc_preview_section * section = afs_toc_preview_section_create2(3, 300, 300, 0, 51, 270);

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with empty input afs_toc_preview_layout_definition instance and afs_toc_preview_section pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test4)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_section * section = NULL;

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with empty input afs_toc_preview_layout_definition instance and empty afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test5)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_section * section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with empty input afs_toc_preview_layout_definition instance and not valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test6)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_section * section = afs_toc_preview_section_create2(3, 300, 300, 0, 51, 270);

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with empty input afs_toc_preview_layout_definition instance and valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test7)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_section * section = afs_toc_preview_section_create2(0, 100, 100, 64, 48, 0);

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(afs_toc_preview_layout_definition_get_section_count(toc_preview_layout_definition) == 1);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with input afs_toc_preview_layout_definition instance with filled sections and afs_toc_preview_section pointers equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test8)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 2);
    afs_toc_preview_section * section = NULL;

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with input afs_toc_preview_layout_definition instance with filled sections and empty afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test9)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 2);
    afs_toc_preview_section * section = afs_toc_preview_section_create();

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with input afs_toc_preview_layout_definition instance with filled sections and not valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test10)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 2);
    afs_toc_preview_section * section = afs_toc_preview_section_create2(3, 300, 300, 0, 51, 270);

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with input afs_toc_preview_layout_definition instance with filled sections and valid afs_toc_preview_section instance
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test11)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 2);
    afs_toc_preview_section * section = afs_toc_preview_section_create2(2, 200, 200, 66, 50, 180);

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 3, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with input valid afs_toc_preview_section instance with duplicate id
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test12)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 2);
    afs_toc_preview_section * section = afs_toc_preview_section_create2(0, 300, 300, 67, 51, 180);

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with input valid afs_toc_preview_section instance with overlapping
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test13)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 2);
    afs_toc_preview_section * section = afs_toc_preview_section_create2(2, 130, 120, 67, 51, 180);

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_layout_definition_add_section with one input valid afs_toc_preview_section inside other valid section
BOXING_START_TEST(afs_toc_preview_layout_definition_add_section_test14)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 2);
    afs_toc_preview_section * section = afs_toc_preview_section_create2(2, 130, 120, 10, 10, 180);

    DBOOL result = afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, section);

    BOXING_ASSERT(result == DFALSE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 2, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    afs_toc_preview_section_free(section);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_section_size_equal with input afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_is_section_size_equal_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;
    
    DBOOL result = afs_toc_preview_layout_definition_is_section_size_equal(toc_preview_layout_definition);

    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DTRUE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_section_size_equal with empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_is_section_size_equal_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    
    DBOOL result = afs_toc_preview_layout_definition_is_section_size_equal(toc_preview_layout_definition);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_section_size_equal with afs_toc_preview_layout_definition instance with empty sections instance
BOXING_START_TEST(afs_toc_preview_layout_definition_is_section_size_equal_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 0);

    DBOOL result = afs_toc_preview_layout_definition_is_section_size_equal(toc_preview_layout_definition);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_section_size_equal with afs_toc_preview_layout_definition with one section
BOXING_START_TEST(afs_toc_preview_layout_definition_is_section_size_equal_test4)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 1);

    DBOOL result = afs_toc_preview_layout_definition_is_section_size_equal(toc_preview_layout_definition);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_section_size_equal with afs_toc_preview_layout_definition with two sections with different width
BOXING_START_TEST(afs_toc_preview_layout_definition_is_section_size_equal_test5)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, afs_toc_preview_section_create2(0, 0, 0, 640, 480, 0));
    afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, afs_toc_preview_section_create2(1, 1000, 1000, 1920, 480, 0));

    DBOOL result = afs_toc_preview_layout_definition_is_section_size_equal(toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST

// Test function afs_toc_preview_layout_definition_is_section_size_equal with afs_toc_preview_layout_definition with two sections with different height
BOXING_START_TEST(afs_toc_preview_layout_definition_is_section_size_equal_test6)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, afs_toc_preview_section_create2(0, 0, 0, 640, 480, 0));
    afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, afs_toc_preview_section_create2(1, 1000, 1000, 640, 1080, 0));

    DBOOL result = afs_toc_preview_layout_definition_is_section_size_equal(toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_is_section_size_equal with afs_toc_preview_layout_definition with two sections with equal sizes
BOXING_START_TEST(afs_toc_preview_layout_definition_is_section_size_equal_test7)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();
    afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, afs_toc_preview_section_create2(0, 0, 0, 640, 480, 0));
    afs_toc_preview_layout_definition_add_section(toc_preview_layout_definition, afs_toc_preview_section_create2(1, 1000, 1000, 640, 480, 0));

    DBOOL result = afs_toc_preview_layout_definition_is_section_size_equal(toc_preview_layout_definition);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_file with file_name pointer equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_save_file(toc_preview_layout_definition, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_file with file_name pointer equal to NULL and afs_toc_preview_layout_definition pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 5);

    DBOOL result = afs_toc_preview_layout_definition_save_file(toc_preview_layout_definition, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_file with file_name pointer not equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_save_file_test3)
{
    const char * file_name = "afs_toc_preview_layout_definition_save_file_test.xml";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_save_file(toc_preview_layout_definition, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_file with empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_save_file_test4)
{
    const char * file_name = "afs_toc_preview_layout_definition_save_file_empty_test.xml";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_save_file(toc_preview_layout_definition, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_file with afs_toc_preview_layout_definition instance with sections instance contains not valid data
BOXING_START_TEST(afs_toc_preview_layout_definition_save_file_test5)
{
    const char * file_name = "afs_toc_preview_layout_definition_save_file_not_valid_test.xml";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("", "", 5);

    DBOOL result = afs_toc_preview_layout_definition_save_file(toc_preview_layout_definition, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_file with afs_toc_preview_layout_definition instance with filled sections, in compact format
BOXING_START_TEST(afs_toc_preview_layout_definition_save_file_test6)
{
    const char * file_name = "afs_toc_preview_layout_definition_save_file_compact_test.xml";

    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 5);

    DBOOL result = afs_toc_preview_layout_definition_save_file(toc_preview_layout_definition, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_file with afs_toc_preview_layout_definition instance with filled sections, in not compact format
BOXING_START_TEST(afs_toc_preview_layout_definition_save_file_test7)
{
    const char * file_name = "afs_toc_preview_layout_definition_save_file_not_compact_test.xml";

    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 5);

    DBOOL result = afs_toc_preview_layout_definition_save_file(toc_preview_layout_definition, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_string with afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_save_string_test1)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    char * result = afs_toc_preview_layout_definition_save_string(toc_preview_layout_definition, DTRUE);

    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_string with empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_save_string_test2)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    char * result = afs_toc_preview_layout_definition_save_string(toc_preview_layout_definition, DTRUE);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_string with not valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_save_string_test3)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("", "", 5);

    char * result = afs_toc_preview_layout_definition_save_string(toc_preview_layout_definition, DTRUE);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_string with valid afs_toc_preview_layout_definition instance, in compact format
BOXING_START_TEST(afs_toc_preview_layout_definition_save_string_test4)
{
    char * test_string = read_xml_toc_preview_layout_definition_file("afs_toc_preview_layout_definition_save_file_compact_test.xml");

    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 5);

    char * result = afs_toc_preview_layout_definition_save_string(toc_preview_layout_definition, DTRUE);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_string with valid afs_toc_preview_layout_definition instance, in not compact format
BOXING_START_TEST(afs_toc_preview_layout_definition_save_string_test5)
{
    char * test_string = read_xml_toc_preview_layout_definition_file("afs_toc_preview_layout_definition_save_file_not_compact_test.xml");

    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 5);

    char * result = afs_toc_preview_layout_definition_save_string(toc_preview_layout_definition, DFALSE);

    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_xml with mxml_node_t pointer equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_save_xml(toc_preview_layout_definition, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_xml with mxml_node_t pointer equal to NULL and afs_toc_preview_layout_definition pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 5);

    DBOOL result = afs_toc_preview_layout_definition_save_xml(toc_preview_layout_definition, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_save_xml(toc_preview_layout_definition, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_xml with empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_save_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_save_xml(toc_preview_layout_definition, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_xml with not valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_save_xml_test5)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("", "", 5);

    DBOOL result = afs_toc_preview_layout_definition_save_xml(toc_preview_layout_definition, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definition_save_xml with valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_preview_layout_definition_save_xml_test6)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance("some id", "some name", 5);

    DBOOL result = afs_toc_preview_layout_definition_save_xml(toc_preview_layout_definition, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_file with file_name pointer equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_load_file(toc_preview_layout_definition, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_file with file_name pointer equal to NULL and afs_toc_preview_layout_definition pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_file(toc_preview_layout_definition, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_preview_layout_definition != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_file with file_name pointer not equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_load_file_test3)
{
    const char * file_name = "afs_toc_preview_layout_definition_save_file_compact_test.xml";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_load_file(toc_preview_layout_definition, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_preview_layout_definition_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_file(toc_preview_layout_definition, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_file with initialized afs_toc_preview_layout_definition instance, in compact format
BOXING_START_TEST(afs_toc_preview_layout_definition_load_file_test5)
{
    const char * file_name = "afs_toc_preview_layout_definition_save_file_compact_test.xml";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_file(toc_preview_layout_definition, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 5, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_file with initialized afs_toc_preview_layout_definition instance, in not compact format
BOXING_START_TEST(afs_toc_preview_layout_definition_load_file_test6)
{
    const char * file_name = "afs_toc_preview_layout_definition_save_file_not_compact_test.xml";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_file(toc_preview_layout_definition, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 5, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_string with input string pointer equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_load_string(toc_preview_layout_definition, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_string with input string pointer equal to NULL and afs_toc_preview_layout_definition pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_string(toc_preview_layout_definition, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_string with input string pointer not equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_load_string_test3)
{
    char * input_string = read_xml_toc_preview_layout_definition_file("afs_toc_preview_layout_definition_save_file_compact_test.xml");
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_load_string(toc_preview_layout_definition, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_string with empty input string
BOXING_START_TEST(afs_toc_preview_layout_definition_load_string_test4)
{
    char * input_string = "";
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_string(toc_preview_layout_definition, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_string with input string for initialized afs_toc_preview_layout_definition instance, in compact format
BOXING_START_TEST(afs_toc_preview_layout_definition_load_string_test5)
{
    char * input_string = read_xml_toc_preview_layout_definition_file("afs_toc_preview_layout_definition_save_file_compact_test.xml");

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_string(toc_preview_layout_definition, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 5, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    free(input_string);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_string with input string for initialized afs_toc_preview_layout_definition instance, in not compact format
BOXING_START_TEST(afs_toc_preview_layout_definition_load_string_test6)
{
    char * input_string = read_xml_toc_preview_layout_definition_file("afs_toc_preview_layout_definition_save_file_not_compact_test.xml");

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_string(toc_preview_layout_definition, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 5, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    free(input_string);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_xml with xml instance pointer equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_load_xml(toc_preview_layout_definition, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_xml with xml instance pointer equal to NULL and afs_toc_preview_layout_definition pointer not equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_load_xml_test2)
{
    mxml_node_t *tree = NULL;

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_xml(toc_preview_layout_definition, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_xml with xml instance pointer not equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_preview_layout_definition_load_xml_test3)
{
    mxml_node_t *tree = get_toc_preview_layout_definition_xml_instance("afs_toc_preview_layout_definition_save_file_compact_test.xml");
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_preview_layout_definition_load_xml(toc_preview_layout_definition, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_preview_layout_definition_load_xml_test4)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_xml(toc_preview_layout_definition, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_xml with xml instance for initialized afs_toc_preview_layout_definition instance, in compact format
BOXING_START_TEST(afs_toc_preview_layout_definition_load_xml_test5)
{
    mxml_node_t *tree = get_toc_preview_layout_definition_xml_instance("afs_toc_preview_layout_definition_save_file_compact_test.xml");
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_xml(toc_preview_layout_definition, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 5, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_preview_layout_definition_load_xml with xml instance for initialized afs_toc_preview_layout_definition instance, in not compact format
BOXING_START_TEST(afs_toc_preview_layout_definition_load_xml_test6)
{
    mxml_node_t *tree = get_toc_preview_layout_definition_xml_instance("afs_toc_preview_layout_definition_save_file_not_compact_test.xml");
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_preview_layout_definition_load_xml(toc_preview_layout_definition, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_toc_preview_layout_definition(toc_preview_layout_definition, "some id", "some name", 5, DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
    mxmlDelete(tree);
}
END_TEST


Suite * tocpreviewlayoutdefinition_tests(void)
{
    TCase * tc_tocpreviewlayoutdefinition_functions_tests = tcase_create("tocpreviewlayoutdefinition_functions_tests");

    // Test function afs_toc_preview_sections_create
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create_test1);
    // Test function afs_toc_preview_sections_create2
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create2_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create2_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create2_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create2_test4);
    // Test function afs_toc_preview_sections_create3
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create3_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create3_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create3_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create3_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create3_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create3_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create3_test7);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create3_test8);
    // Test function afs_toc_preview_sections_create4
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create4_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create4_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_create4_test3);
    // Test function afs_toc_preview_layout_definition_init
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init_test3);
    // Test function afs_toc_preview_layout_definition_init2
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init2_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init2_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init2_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init2_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init2_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init2_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init2_test7);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_init2_test8);
    // Test function afs_toc_preview_layout_definition_clone
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_clone_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_clone_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_clone_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_clone_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_clone_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_clone_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_clone_test7);
    // Test function afs_toc_preview_layout_definition_get_new_reference
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_new_reference_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_new_reference_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_new_reference_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_new_reference_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_new_reference_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_new_reference_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_new_reference_test7);
    // Test function afs_toc_preview_layout_definition_equal
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test7);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test8);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test9);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test10);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test11);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test12);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test13);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test14);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test15);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_equal_test16);
    // Test function afs_toc_preview_layout_definition_is_valid
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test7);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test8);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test9);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test10);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_valid_test11);
    // Test function afs_toc_preview_layout_definition_get_section_count
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_section_count_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_section_count_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_section_count_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_section_count_test4);
    // Test function afs_toc_preview_layout_definition_get_section
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_section_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_section_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_section_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_section_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_get_section_test5);
    // Test function afs_toc_preview_layout_definition_add_section
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test7);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test8);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test9);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test10);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test11);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test12);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test13);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_add_section_test14);
    // Test function afs_toc_preview_layout_definition_is_section_size_equal
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_section_size_equal_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_section_size_equal_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_section_size_equal_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_section_size_equal_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_section_size_equal_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_section_size_equal_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_is_section_size_equal_test7);
    // Test function afs_toc_preview_layout_definition_save_file
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_file_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_file_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_file_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_file_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_file_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_file_test6);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_file_test7);
    // Test function afs_toc_preview_layout_definition_save_string
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_string_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_string_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_string_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_string_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_string_test5);
    // Test function afs_toc_preview_layout_definition_save_xml
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_xml_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_xml_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_xml_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_xml_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_xml_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_save_xml_test6);
    // Test function afs_toc_preview_layout_definition_load_file
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_file_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_file_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_file_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_file_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_file_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_file_test6);
    // Test function afs_toc_preview_layout_definition_load_string
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_string_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_string_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_string_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_string_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_string_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_string_test6);
    // Test function afs_toc_preview_layout_definition_load_xml
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_xml_test1);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_xml_test2);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_xml_test3);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_xml_test4);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_xml_test5);
    tcase_add_test(tc_tocpreviewlayoutdefinition_functions_tests, afs_toc_preview_layout_definition_load_xml_test6);

    Suite * s = suite_create("tocpreviewlayoutdefinition_test_util");
    suite_add_tcase(s, tc_tocpreviewlayoutdefinition_functions_tests);

    return s;
}
