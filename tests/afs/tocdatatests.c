#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "tocdata_c.h"
#include "boxing/utils.h"
#include "mxml.h"

#if defined ( D_OS_WIN32 )
#define DFSEEK _fseeki64
#define DFTELL _ftelli64
#else
#define DFSEEK fseeko
#define DFTELL ftello
#endif


static afs_toc_file_preview_pages * get_filled_empty_pages_vector8(unsigned int pages_count)
{
	afs_toc_file_preview_pages * pages = gvector_create_pointers(pages_count);

	for (unsigned int i = 0; i < pages_count; i++)
	{
		GVECTORN(pages, afs_toc_file_preview_page *, i) = afs_toc_file_preview_page_create();
	}

	return pages;
}


static afs_toc_file_preview_pages * get_filled_pages_vector8(unsigned int pages_count, const char * current_id)
{
    afs_toc_file_preview_pages * pages = gvector_create_pointers(pages_count);

    for (unsigned int i = 0; i < pages_count; i++)
    {
        GVECTORN(pages, afs_toc_file_preview_page *, i) = afs_toc_file_preview_page_create2(current_id, 9 + i, 8 + i, 7 + i, 6 + i, 5 + i, 4 + i, 3 + i);
    }

    return pages;
}


static afs_toc_data_file_metadata_sources * get_filled_toc_data_file_metadata_sources_vector8(unsigned int sources_count)
{
    afs_toc_data_file_metadata_sources * sources = gvector_create_pointers(sources_count);

    for (unsigned int i = 0; i < sources_count; i++)
    {
        GVECTORN(sources, afs_toc_data_file_metadata_source *, i) = afs_toc_data_file_metadata_source_create2("some xml data", "afs/metadata-csv", i + 1, i + 2);
    }

    return sources;
}


static afs_toc_data_file_metadata * get_afs_toc_data_file_metadata_instance8(unsigned int sources_count)
{
    afs_toc_data_file_metadata_sources * sources = get_filled_toc_data_file_metadata_sources_vector8(sources_count);
    afs_toc_data_file_metadata * toc_data_file_metadata = afs_toc_data_file_metadata_create2(sources);

    return toc_data_file_metadata;
}


static afs_toc_file * get_afs_toc_file_instance8(unsigned int number, unsigned int file_number)
{
    char current_id[10];
    sprintf(current_id, "id %u", number);

    afs_toc_file_preview_pages* pages = get_filled_pages_vector8(3, current_id);
    afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);

    afs_toc_data_file_metadata * toc_data_file_metadata = get_afs_toc_data_file_metadata_instance8(5);
    char current_unique_id[100];
    sprintf(current_unique_id, "38A0AAAB-16B2-640C-5353-6DB8AE4367%02u", file_number * 10 + number);

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
        current_unique_id,
        AFS_TOC_FILE_NO_PARENT,
        AFS_TOC_FILE_TYPE_ALL,
        toc_file_preview,
        "file_format",
        toc_data_file_metadata);
}


static afs_toc_files * get_afs_toc_files_instance8(unsigned int files_count, unsigned int reel_number)
{
    afs_toc_indices * tocs = gvector_create_pointers(files_count);

    for (unsigned int i = 0; i < files_count; i++)
    {
        afs_toc_file * toc_file = get_afs_toc_file_instance8(i, reel_number);
        GVECTORN(tocs, afs_toc_file *, i) = toc_file;
    }

    return afs_toc_files_create2(tocs);
}


afs_toc_data_reel * get_afs_toc_data_reel_instance8(const char * id, unsigned int files_count, unsigned int reel_number)
{
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create2(id);
    toc_data_reel->files = get_afs_toc_files_instance8(files_count, reel_number);

    return toc_data_reel;
}


afs_toc_data_reels * get_afs_toc_data_reels_instance8(unsigned int reels_count, unsigned int files_count)
{
    afs_toc_data_reels * toc_data_reels = afs_toc_data_reels_create();

    for (unsigned int i = 0; i < reels_count; i++)
    {
        char current_reel_id[100];
        sprintf(current_reel_id, "Reel id %u", i);
        afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance8(current_reel_id, files_count, i);
        afs_toc_data_reels_add_reel(toc_data_reels, toc_data_reel);
    }

    return toc_data_reels;
}


static afs_toc_metadata_source * get_afs_toc_metadata_source_instance8(unsigned int number)
{
    char tags_string[255]; // Temporary string
    sprintf(tags_string, "tag%d;tag%d;tag%d", number * 3, number * 3 + 1, number * 3 + 2);

    return afs_toc_metadata_source_create7("afs/metadata-csv", number + 1, "some xml data", tags_string, number + 2);
}


static afs_toc_metadata_sources * get_filled_sources_vector8(unsigned int sources_count)
{
    afs_toc_metadata_sources* sources = gvector_create_pointers(sources_count);

    for (unsigned int i = 0; i < sources_count; i++)
    {
        GVECTORN(sources, afs_toc_metadata_source *, i) = get_afs_toc_metadata_source_instance8(i);
    }

    return sources;
}


static afs_toc_metadata * get_afs_toc_metadata_instance8(unsigned int sources_count)
{
    afs_toc_metadata_sources * sources = get_filled_sources_vector8(sources_count);
    afs_toc_metadata * toc_metadata = afs_toc_metadata_create2(sources);

    return toc_metadata;
}


afs_toc_preview_sections * get_afs_toc_preview_sections_instance8(unsigned int sections_count)
{
    afs_toc_preview_sections * toc_preview_sections = afs_toc_preview_sections_create();
    for (unsigned int i = 0; i < sections_count; i++)
    {
        afs_toc_preview_sections_append(toc_preview_sections, afs_toc_preview_section_create2(0 + i, 100 * i, 100 * i, 64 + i, 48 + i, (90 * i) % 360));
    }

    return toc_preview_sections;
}


afs_toc_preview_layout_definition * get_afs_toc_preview_layout_definition_instance8(const char * id, const char * name, unsigned int sections_count)
{
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create2(id, name);
    toc_preview_layout_definition->sections = get_afs_toc_preview_sections_instance8(sections_count);

    return toc_preview_layout_definition;
}


afs_toc_preview_layout_definitions * get_afs_toc_preview_layout_definitions_instance8(int definitions_count)
{
	if (definitions_count < 0)
	{
		return NULL;
	}

    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    for (int i = 0; i < definitions_count; i++)
    {
        char current_id[14];
        char current_name[16];

        sprintf(current_id, "id %u", i);
        sprintf(current_name, "name %u", i);

        afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8(current_id, current_name, 5);
        afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);
    }

    return toc_preview_layout_definitions;
}


afs_toc_data * get_afs_toc_data_instance8(
    const char * version, 
    const char * index_type, 
    const char * job_id, 
    const char * reel_id, 
    unsigned int reels_count, 
    unsigned int sources_count, 
    int          definitions_count)
{
    afs_toc_data * afs_toc_data = afs_toc_data_create2(version, index_type, job_id, reel_id);
    afs_toc_data->reels = get_afs_toc_data_reels_instance8(reels_count, 3);
    afs_toc_data->metadata = get_afs_toc_metadata_instance8(sources_count);
    afs_toc_data->preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(definitions_count);

    return afs_toc_data;
}


afs_toc_data * get_afs_toc_data_different_reel_parent(unsigned int reel_count)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", reel_count, 3, 3);

    for (unsigned int i = 0; i < reel_count; i++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, i);
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->types = AFS_TOC_FILE_TYPE_DIGITAL;
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->parent_id = AFS_TOC_FILE_PARENT;
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1)->types = AFS_TOC_FILE_TYPE_PREVIEW;
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1)->parent_id = AFS_TOC_FILE_PARENT;
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2)->types = AFS_TOC_FILE_TYPE_ALL;
        GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2)->parent_id = AFS_TOC_FILE_PARENT;
    }

    return toc_data;
}


static void test_afs_toc_data_reel(afs_toc_data_reel * toc_data_reel, const char * id, const size_t files_count, unsigned int reel_number)
{
    BOXING_ASSERT(toc_data_reel != NULL);
    BOXING_ASSERT(boxing_string_equal(id, toc_data_reel->id) == DTRUE);
    BOXING_ASSERT(afs_toc_files_get_tocs_count(toc_data_reel->files) == files_count);

    for (size_t i = 0; i < files_count; i++)
    {
        afs_toc_file * afs_toc_file_instance = get_afs_toc_file_instance8((unsigned int)i, reel_number);
        BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, i), afs_toc_file_instance) == DTRUE);
        afs_toc_file_free(afs_toc_file_instance);
    }
}


static void test_afs_toc_data_reels(afs_toc_data_reels * toc_data_reels, unsigned int reels_count)
{
    BOXING_ASSERT(toc_data_reels != NULL);
    BOXING_ASSERT(afs_toc_data_reels_get_reels_count(toc_data_reels) == reels_count);

    for (unsigned int i = 0; i < reels_count; i++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_reels_get_reel(toc_data_reels, i);

        char current_reel_id[100];
        sprintf(current_reel_id, "Reel id %u", i);

        test_afs_toc_data_reel(toc_data_reel, current_reel_id, 3, i);
    }
}


static void test_afs_toc_metadata(afs_toc_metadata * toc_metadata, unsigned int sources_count)
{
    BOXING_ASSERT(toc_metadata != NULL);
    BOXING_ASSERT(afs_toc_metadata_get_sources_count(toc_metadata) == sources_count);

    for (unsigned int i = 0; i < sources_count; i++)
    {
        char tags_string[255]; // Temporary string
        sprintf(tags_string, "tag%d;tag%d;tag%d", i * 3, i * 3 + 1, i * 3 + 2);

        afs_toc_metadata_source * toc_metadata_source = afs_toc_metadata_source_create7("afs/metadata-csv", i + 1, "some xml data", tags_string, i + 2);
        BOXING_ASSERT(afs_toc_metadata_source_equal(GVECTORN(toc_metadata->sources, afs_toc_metadata_source *, i), toc_metadata_source) == DTRUE);
        afs_toc_metadata_source_free(toc_metadata_source);
    }
}


static void test_afs_toc_preview_layout_definitions(afs_toc_preview_layout_definitions * toc_preview_layout_definitions, unsigned int definitions_count)
{
    BOXING_ASSERT(toc_preview_layout_definitions != NULL);
    BOXING_ASSERT(afs_toc_preview_layout_definitions_get_count(toc_preview_layout_definitions) == definitions_count);

    for (unsigned int i = 0; i < definitions_count; i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, i);
        
        char current_id[14];
        char current_name[16];

        sprintf(current_id, "id %u", i);
        sprintf(current_name, "name %u", i);

        afs_toc_preview_layout_definition * test_toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8(current_id, current_name, 5);
        BOXING_ASSERT(afs_toc_preview_layout_definition_equal(toc_preview_layout_definition, test_toc_preview_layout_definition) == DTRUE);
        afs_toc_preview_layout_definition_free(test_toc_preview_layout_definition);
    }
}


static void test_afs_toc_data(
    afs_toc_data * afs_toc_data,
    const char * version,
    const char * index_type,
    const char * job_id,
    const char * reel_id,
    int reels_count,
    int sources_count,
    int definitions_count)
{
    BOXING_ASSERT(afs_toc_data != NULL);
    BOXING_ASSERT(boxing_string_equal(afs_toc_data->version, version) == DTRUE);
    BOXING_ASSERT(boxing_string_equal(afs_toc_data->index_type, index_type) == DTRUE);
    BOXING_ASSERT(boxing_string_equal(afs_toc_data->job_id, job_id) == DTRUE);
    BOXING_ASSERT(boxing_string_equal(afs_toc_data->reel_id, reel_id) == DTRUE);

    if (reels_count == -1)
    {
        BOXING_ASSERT(afs_toc_data->reels == NULL);
    }
    else
    {
        test_afs_toc_data_reels(afs_toc_data->reels, reels_count);
    }

    if (sources_count == -1)
    {
        BOXING_ASSERT(afs_toc_data->metadata == NULL);
    }
    else
    {
        test_afs_toc_metadata(afs_toc_data->metadata, sources_count);
    }

    if (definitions_count == -1)
    {
        BOXING_ASSERT(afs_toc_data->preview_layout_definitions == NULL);
    }
    else
    {
        test_afs_toc_preview_layout_definitions(afs_toc_data->preview_layout_definitions, definitions_count);
    }
}


static char * read_xml_toc_data(const char * file_name)
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


static mxml_node_t * get_toc_data_xml_instance(const char * file_name)
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


static void test_equals_toc_data(afs_toc_data * toc_data1, afs_toc_data * toc_data2, DBOOL expected_result)
{
    DBOOL result = afs_toc_data_equal(toc_data2, toc_data1);
    BOXING_ASSERT(result == expected_result);
    result = afs_toc_data_equal(toc_data1, toc_data2);
    BOXING_ASSERT(result == expected_result);
}


// TEST TOC DATA FUNCTIONS
//

// Test function afs_toc_data_create
BOXING_START_TEST(afs_toc_data_create_test1)
{
    afs_toc_data * toc_data = afs_toc_data_create();

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_create2 with initial string pointers equal to NULL
BOXING_START_TEST(afs_toc_data_create2_test1)
{
    afs_toc_data * toc_data = afs_toc_data_create2(NULL, NULL, NULL, NULL);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_create2 with empty initial strings
BOXING_START_TEST(afs_toc_data_create2_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create2("", "", "", "");

    test_afs_toc_data(toc_data, "", "", "", "", -1, -1, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_create2 with not empty initial strings
BOXING_START_TEST(afs_toc_data_create2_test3)
{
    afs_toc_data * toc_data = afs_toc_data_create2("Some version", "Some index type", "Some job id", "Some reel id");

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", -1, -1, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_clone with input pointer equal to NULL
BOXING_START_TEST(afs_toc_data_clone_test1)
{
    afs_toc_data * toc_data1 = NULL;

    BOXING_ASSERT(toc_data1 == NULL);

    afs_toc_data * toc_data2 = afs_toc_data_clone(toc_data1);

    BOXING_ASSERT(toc_data2 == NULL);
}
END_TEST


// Test function afs_toc_data_clone with structure data pointers equal to NULL
BOXING_START_TEST(afs_toc_data_clone_test2)
{
    afs_toc_data * toc_data1 = afs_toc_data_create2(NULL, NULL, NULL, NULL);

    test_afs_toc_data(toc_data1, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data * toc_data2 = afs_toc_data_clone(toc_data1);

    test_afs_toc_data(toc_data2, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_clone with internal structure pointers equal to NULL and empty data strings
BOXING_START_TEST(afs_toc_data_clone_test3)
{
    afs_toc_data * toc_data1 = afs_toc_data_create2("", "", "", "");

    test_afs_toc_data(toc_data1, "", "", "", "", -1, -1, -1);

    afs_toc_data * toc_data2 = afs_toc_data_clone(toc_data1);

    test_afs_toc_data(toc_data2, "", "", "", "", -1, -1, -1);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_clone with internal structure pointers not equal to NULL and empty data strings
BOXING_START_TEST(afs_toc_data_clone_test4)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("", "", "", "", 3, 3, 3);

    test_afs_toc_data(toc_data1, "", "", "", "", 3, 3, 3);

    afs_toc_data * toc_data2 = afs_toc_data_clone(toc_data1);

    test_afs_toc_data(toc_data2, "", "", "", "", 3, 3, 3);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_clone with internal structure pointers equal to NULL and not empty data strings
BOXING_START_TEST(afs_toc_data_clone_test5)
{
    afs_toc_data * toc_data1 = afs_toc_data_create2("Sove version", "Some index type", "Some job id", "Some reel id");

    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", -1, -1, -1);

    afs_toc_data * toc_data2 = afs_toc_data_clone(toc_data1);

    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id", -1, -1, -1);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_clone with internal structure pointers not equal to NULL and not empty data strings
BOXING_START_TEST(afs_toc_data_clone_test6)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data * toc_data2 = afs_toc_data_clone(toc_data1);

    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_new_reference_test1)
{
    afs_toc_data * toc_data1 = NULL;

    BOXING_ASSERT(toc_data1 == NULL);

    afs_toc_data * toc_data2 = afs_toc_data_get_new_reference(toc_data1);

    BOXING_ASSERT(toc_data2 == NULL);

    toc_data1 = afs_toc_data_create();

    test_afs_toc_data(toc_data1, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_data1->reference_count == 1);
    BOXING_ASSERT(toc_data2 == NULL);

    afs_toc_data_free(toc_data1);
}
END_TEST


// Test function afs_toc_data_get_new_reference with structure data pointers equal to NULL
BOXING_START_TEST(afs_toc_data_get_new_reference_test2)
{
    afs_toc_data * toc_data1 = afs_toc_data_create2(NULL, NULL, NULL, NULL);

    test_afs_toc_data(toc_data1, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_data1->reference_count == 1);

    afs_toc_data * toc_data2 = afs_toc_data_get_new_reference(toc_data1);

    test_afs_toc_data(toc_data2, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_data1->reference_count == 2);
    BOXING_ASSERT(toc_data2->reference_count == 2);
    BOXING_ASSERT(toc_data1 == toc_data2);

    toc_data1->reel_id = boxing_string_clone("Some other reel id!");;

    BOXING_ASSERT(boxing_string_equal(toc_data2->reel_id, "Some other reel id!") == DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data1);
}
END_TEST


// Test function afs_toc_data_get_new_reference with internal structure pointers equal to NULL and empty data strings
BOXING_START_TEST(afs_toc_data_get_new_reference_test3)
{
    afs_toc_data * toc_data1 = afs_toc_data_create2("", "", "", "");

    test_afs_toc_data(toc_data1, "", "", "", "", -1, -1, -1);
    BOXING_ASSERT(toc_data1->reference_count == 1);

    afs_toc_data * toc_data2 = afs_toc_data_get_new_reference(toc_data1);

    test_afs_toc_data(toc_data2, "", "", "", "", -1, -1, -1);
    BOXING_ASSERT(toc_data1->reference_count == 2);
    BOXING_ASSERT(toc_data2->reference_count == 2);
    BOXING_ASSERT(toc_data1 == toc_data2);

    boxing_string_free(toc_data1->reel_id);
    toc_data1->reel_id = boxing_string_clone("Some other reel id!");

    BOXING_ASSERT(boxing_string_equal(toc_data2->reel_id, "Some other reel id!") == DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data1);
}
END_TEST


// Test function afs_toc_data_get_new_reference with internal structure pointers not equal to NULL and empty data strings
BOXING_START_TEST(afs_toc_data_get_new_reference_test4)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("", "", "", "", 3, 3, 3);

    test_afs_toc_data(toc_data1, "", "", "", "", 3, 3, 3);
    BOXING_ASSERT(toc_data1->reference_count == 1);

    afs_toc_data * toc_data2 = afs_toc_data_get_new_reference(toc_data1);

    test_afs_toc_data(toc_data2, "", "", "", "", 3, 3, 3);
    BOXING_ASSERT(toc_data1->reference_count == 2);
    BOXING_ASSERT(toc_data2->reference_count == 2);
    BOXING_ASSERT(toc_data1 == toc_data2);

    boxing_string_free(toc_data1->reel_id);
    toc_data1->reel_id = boxing_string_clone("Some other reel id!");

    BOXING_ASSERT(boxing_string_equal(toc_data2->reel_id, "Some other reel id!") == DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data1);
}
END_TEST


// Test function afs_toc_data_get_new_reference with internal structure pointers equal to NULL and not empty data strings
BOXING_START_TEST(afs_toc_data_get_new_reference_test5)
{
    afs_toc_data * toc_data1 = afs_toc_data_create2("Sove version", "Some index type", "Some job id", "Some reel id");

    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", -1, -1, -1);
    BOXING_ASSERT(toc_data1->reference_count == 1);

    afs_toc_data * toc_data2 = afs_toc_data_get_new_reference(toc_data1);

    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id", -1, -1, -1);
    BOXING_ASSERT(toc_data1->reference_count == 2);
    BOXING_ASSERT(toc_data2->reference_count == 2);
    BOXING_ASSERT(toc_data1 == toc_data2);

    boxing_string_free(toc_data1->reel_id);
    toc_data1->reel_id = boxing_string_clone("Some other reel id!");

    BOXING_ASSERT(boxing_string_equal(toc_data2->reel_id, "Some other reel id!") == DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data1);
}
END_TEST


// Test function afs_toc_data_get_new_reference with internal structure pointers not equal to NULL and not empty data strings
BOXING_START_TEST(afs_toc_data_get_new_reference_test6)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    BOXING_ASSERT(toc_data1->reference_count == 1);

    afs_toc_data * toc_data2 = afs_toc_data_get_new_reference(toc_data1);

    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    BOXING_ASSERT(toc_data1->reference_count == 2);
    BOXING_ASSERT(toc_data2->reference_count == 2);
    BOXING_ASSERT(toc_data1 == toc_data2);

    boxing_string_free(toc_data1->reel_id);
    toc_data1->reel_id = boxing_string_clone("Some other reel id!");

    BOXING_ASSERT(boxing_string_equal(toc_data2->reel_id, "Some other reel id!") == DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data1);
}
END_TEST


// Test function afs_toc_data_equal with input afs_toc_data pointers equal to NULL
BOXING_START_TEST(afs_toc_data_equal_test1)
{
    afs_toc_data * toc_data1 = NULL;
    afs_toc_data * toc_data2 = NULL;

    test_equals_toc_data(toc_data2, toc_data1, DTRUE);
}
END_TEST

// Test function afs_toc_data_equal with empty input afs_toc_data instances 
BOXING_START_TEST(afs_toc_data_equal_test2)
{
    afs_toc_data * toc_data1 = afs_toc_data_create();
    test_afs_toc_data(toc_data1, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data * toc_data2 = afs_toc_data_create();
    test_afs_toc_data(toc_data2, NULL, NULL, NULL, NULL, -1, -1, -1);

    test_equals_toc_data(toc_data2, toc_data1, DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with data strings equal to NULL and empty internal structure instances
BOXING_START_TEST(afs_toc_data_equal_test3)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8(NULL, NULL, NULL, NULL, 0, 0, 0);
    test_afs_toc_data(toc_data1, NULL, NULL, NULL, NULL, 0, 0, 0);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8(NULL, NULL, NULL, NULL, 0, 0, 0);
    test_afs_toc_data(toc_data2, NULL, NULL, NULL, NULL, 0, 0, 0);

    test_equals_toc_data(toc_data2, toc_data1, DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with data strings equal to NULL and not empty internal structure instances
BOXING_START_TEST(afs_toc_data_equal_test4)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8(NULL, NULL, NULL, NULL, 3, 3, 3);
    test_afs_toc_data(toc_data1, NULL, NULL, NULL, NULL, 3, 3, 3);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8(NULL, NULL, NULL, NULL, 3, 3, 3);
    test_afs_toc_data(toc_data2, NULL, NULL, NULL, NULL, 3, 3, 3);

    test_equals_toc_data(toc_data2, toc_data1, DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with empty data strings and internal structure pointers equal to NULL
BOXING_START_TEST(afs_toc_data_equal_test5)
{
    afs_toc_data * toc_data1 = afs_toc_data_create2("", "", "", "");
    test_afs_toc_data(toc_data1, "", "", "", "", -1, -1, -1);

    afs_toc_data * toc_data2 = afs_toc_data_create2("", "", "", "");
    test_afs_toc_data(toc_data2, "", "", "", "", -1, -1, -1);

    test_equals_toc_data(toc_data2, toc_data1, DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with empty data strings and empty internal structure instances
BOXING_START_TEST(afs_toc_data_equal_test6)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("", "", "", "", 0, 0, 0);
    test_afs_toc_data(toc_data1, "", "", "", "", 0, 0, 0);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("", "", "", "", 0, 0, 0);
    test_afs_toc_data(toc_data2, "", "", "", "", 0, 0, 0);

    test_equals_toc_data(toc_data2, toc_data1, DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with empty data strings and not empty internal structure instances
BOXING_START_TEST(afs_toc_data_equal_test7)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("", "", "", "", 3, 3, 3);
    test_afs_toc_data(toc_data1, "", "", "", "", 3, 3, 3);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("", "", "", "", 3, 3, 3);
    test_afs_toc_data(toc_data2, "", "", "", "", 3, 3, 3);

    test_equals_toc_data(toc_data2, toc_data1, DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with not empty data strings and internal structure pointers equal to NULL
BOXING_START_TEST(afs_toc_data_equal_test8)
{
    afs_toc_data * toc_data1 = afs_toc_data_create2("Sove version", "Some index type", "Some job id", "Some reel id");
    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", -1, -1, -1);

    afs_toc_data * toc_data2 = afs_toc_data_create2("Sove version", "Some index type", "Some job id", "Some reel id");
    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id", -1, -1, -1);

    test_equals_toc_data(toc_data2, toc_data1, DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with not empty data strings and empty internal structure instances
BOXING_START_TEST(afs_toc_data_equal_test9)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);
    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);
    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);

    test_equals_toc_data(toc_data2, toc_data1, DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with not empty data strings and not empty internal structure instances
BOXING_START_TEST(afs_toc_data_equal_test10)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    test_equals_toc_data(toc_data2, toc_data1, DTRUE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with empty input afs_toc_data instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_data_equal_test11)
{
    afs_toc_data * toc_data1 = afs_toc_data_create();
    test_afs_toc_data(toc_data1, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data * toc_data2 = NULL;

    test_equals_toc_data(toc_data2, toc_data1, DFALSE);

    afs_toc_data_free(toc_data1);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instance and pointer equal to NULL 
BOXING_START_TEST(afs_toc_data_equal_test12)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data * toc_data2 = NULL;

    test_equals_toc_data(toc_data2, toc_data1, DFALSE);

    afs_toc_data_free(toc_data1);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instance and empty input afs_toc_data instance 
BOXING_START_TEST(afs_toc_data_equal_test13)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data * toc_data2 = afs_toc_data_create();
    test_afs_toc_data(toc_data2, NULL, NULL, NULL, NULL, -1, -1, -1);

    test_equals_toc_data(toc_data2, toc_data1, DFALSE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with different version string
BOXING_START_TEST(afs_toc_data_equal_test14)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version 1", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data1, "Sove version 1", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("Sove version 2", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data2, "Sove version 2", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    test_equals_toc_data(toc_data2, toc_data1, DFALSE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST



// Test function afs_toc_data_equal with not empty input afs_toc_data instances with different index type string
BOXING_START_TEST(afs_toc_data_equal_test15)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type 1", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data1, "Sove version", "Some index type 1", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("Sove version", "Some index type 2", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data2, "Sove version", "Some index type 2", "Some job id", "Some reel id", 3, 3, 3);

    test_equals_toc_data(toc_data2, toc_data1, DFALSE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with different job id string
BOXING_START_TEST(afs_toc_data_equal_test16)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id 1", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id 1", "Some reel id", 3, 3, 3);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id 2", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id 2", "Some reel id", 3, 3, 3);

    test_equals_toc_data(toc_data2, toc_data1, DFALSE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with different reel id string
BOXING_START_TEST(afs_toc_data_equal_test17)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id 1", 3, 3, 3);
    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id 1", 3, 3, 3);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id 2", 3, 3, 3);
    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id 2", 3, 3, 3);

    test_equals_toc_data(toc_data2, toc_data1, DFALSE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with different reels instances
BOXING_START_TEST(afs_toc_data_equal_test18)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 2, 3, 3);
    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id", 2, 3, 3);

    test_equals_toc_data(toc_data2, toc_data1, DFALSE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with different metadata instances
BOXING_START_TEST(afs_toc_data_equal_test19)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 2, 3);
    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 2, 3);

    test_equals_toc_data(toc_data2, toc_data1, DFALSE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_equal with not empty input afs_toc_data instances with different layout definitions instances
BOXING_START_TEST(afs_toc_data_equal_test20)
{
    afs_toc_data * toc_data1 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    test_afs_toc_data(toc_data1, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data * toc_data2 = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 2);
    test_afs_toc_data(toc_data2, "Sove version", "Some index type", "Some job id", "Some reel id", 3, 3, 2);

    test_equals_toc_data(toc_data2, toc_data1, DFALSE);

    afs_toc_data_free(toc_data1);
    afs_toc_data_free(toc_data2);
}
END_TEST


// Test function afs_toc_data_get_reel_by_index with input afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_reel_by_index_test1)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, 0);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
}
END_TEST


// Test function afs_toc_data_get_reel_by_index with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_get_reel_by_index_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, 0);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_data_reel == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_reel_by_index with afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_get_reel_by_index_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);

    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, 0);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);
    BOXING_ASSERT(toc_data_reel == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_reel_by_index with not empty afs_toc_data instance and index out of range
BOXING_START_TEST(afs_toc_data_get_reel_by_index_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 1, 1, 1);

    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, 1);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 1, 1, 1);
    BOXING_ASSERT(toc_data_reel == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_reel_by_index with not empty afs_toc_data instance and index in range
BOXING_START_TEST(afs_toc_data_get_reel_by_index_test5)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);

    for (unsigned int i = 0; i < afs_toc_data_reel_count(toc_data); i++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, i);
        BOXING_ASSERT(afs_toc_data_reel_equal(GVECTORN(toc_data->reels->reels, afs_toc_data_reel *, i), toc_data_reel) == DTRUE);
    }

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_reel_by_id with input afs_toc_data pointer equal to NULL and id pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_reel_by_id_test1)
{
    afs_toc_data * toc_data = NULL;
    const char * id_string = NULL;
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_id(toc_data, id_string);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(id_string == NULL);
}
END_TEST


// Test function afs_toc_data_get_reel_by_id with input afs_toc_data pointer equal to NULL and empty id string
BOXING_START_TEST(afs_toc_data_get_reel_by_id_test2)
{
    afs_toc_data * toc_data = NULL;
    const char * id_string = "";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_id(toc_data, id_string);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(boxing_string_equal(id_string, "") == DTRUE);
}
END_TEST


// Test function afs_toc_data_get_reel_by_id with input afs_toc_data pointer equal to NULL and not empty id string
BOXING_START_TEST(afs_toc_data_get_reel_by_id_test3)
{
    afs_toc_data * toc_data = NULL;
    const char * id_string = "Reel id 0";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_id(toc_data, id_string);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(boxing_string_equal(id_string, "Reel id 0") == DTRUE);
}
END_TEST


// Test function afs_toc_data_get_reel_by_id with empty afs_toc_data instance and id pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_reel_by_id_test4)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    const char * id_string = NULL;
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_id(toc_data, id_string);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(id_string == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_reel_by_id with empty afs_toc_data instance and empty id string
BOXING_START_TEST(afs_toc_data_get_reel_by_id_test5)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    const char * id_string = "";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_id(toc_data, id_string);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(boxing_string_equal(id_string, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_reel_by_id with empty afs_toc_data instance and not empty id string
BOXING_START_TEST(afs_toc_data_get_reel_by_id_test6)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    const char * id_string = "Reel id 0";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_id(toc_data, id_string);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(boxing_string_equal(id_string, "Reel id 0") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_reel_by_id with not empty afs_toc_data instance and id pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_reel_by_id_test7)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    const char * id_string = NULL;
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_id(toc_data, id_string);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(id_string == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_reel_by_id with not empty afs_toc_data instance and empty id string
BOXING_START_TEST(afs_toc_data_get_reel_by_id_test8)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    const char * id_string = "";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_id(toc_data, id_string);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(boxing_string_equal(id_string, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_reel_by_id with not empty afs_toc_data instance and not exist id
BOXING_START_TEST(afs_toc_data_get_reel_by_id_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    const char * id_string = "Reel id 2";
    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_id(toc_data, id_string);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    BOXING_ASSERT(toc_data_reel == NULL);
    BOXING_ASSERT(boxing_string_equal(id_string, "Reel id 2") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_reel_by_id with not empty afs_toc_data instance and exist id
BOXING_START_TEST(afs_toc_data_get_reel_by_id_test10)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 6, 6, 6);

    for (unsigned int i = 0; i < afs_toc_data_reel_count(toc_data); i++)
    {
        char current_id[20];
        sprintf(current_id, "Reel id %u", i);

        afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_id(toc_data, current_id);
        BOXING_ASSERT(afs_toc_data_reel_equal(GVECTORN(toc_data->reels->reels, afs_toc_data_reel *, i), toc_data_reel) == DTRUE);
    }

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_metadata_source with input afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_metadata_source_test1)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_metadata_source * source = afs_toc_data_get_metadata_source(toc_data, 0);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(source == NULL);
}
END_TEST


// Test function afs_toc_data_get_metadata_source with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_get_metadata_source_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_metadata_source * source = afs_toc_data_get_metadata_source(toc_data, 0);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(source == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_metadata_source with afs_toc_data instance with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_data_get_metadata_source_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);

    afs_toc_metadata_source * source = afs_toc_data_get_metadata_source(toc_data, 0);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);
    BOXING_ASSERT(source == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_metadata_source with not empty afs_toc_data instance and index out of range
BOXING_START_TEST(afs_toc_data_get_metadata_source_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);

    afs_toc_metadata_source * source = afs_toc_data_get_metadata_source(toc_data, 2);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    BOXING_ASSERT(source == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_metadata_source with not empty afs_toc_data instance and index in range
BOXING_START_TEST(afs_toc_data_get_metadata_source_test5)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 6, 6, 6);

    for (unsigned int i = 0; i < afs_toc_data_metadata_source_count(toc_data); i++)
    {
        afs_toc_metadata_source * source = afs_toc_data_get_metadata_source(toc_data, i);
        BOXING_ASSERT(afs_toc_metadata_source_equal(GVECTORN(toc_data->metadata->sources, afs_toc_metadata_source *, i), source) == DTRUE);
    }

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_id with input afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_file_by_id_test1)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_file * toc_file = afs_toc_data_get_file_by_id(toc_data, 0);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(toc_file == NULL);
}
END_TEST


// Test function afs_toc_data_get_file_by_id with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_get_file_by_id_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_file * toc_file = afs_toc_data_get_file_by_id(toc_data, 0);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_file == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_id with afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_get_file_by_id_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);

    afs_toc_file * toc_file = afs_toc_data_get_file_by_id(toc_data, 0);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);
    BOXING_ASSERT(toc_file == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_id with not empty afs_toc_data instance and not exist id
BOXING_START_TEST(afs_toc_data_get_file_by_id_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 1, 1, 1);

    afs_toc_file * toc_file = afs_toc_data_get_file_by_id(toc_data, 1);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 1, 1, 1);
    BOXING_ASSERT(toc_file == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_id with not empty afs_toc_data instance and exist id
BOXING_START_TEST(afs_toc_data_get_file_by_id_test5)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);

    for (unsigned int i = 0; i < afs_toc_data_reel_count(toc_data); i++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, i);

        for (unsigned int j = 0; j < afs_toc_data_reel_file_count(toc_data_reel); j++)
        {
            GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, j)->id = i * 10 + j;
        }
    }

    for (unsigned int i = 0; i < afs_toc_data_reel_count(toc_data); i++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, i);

        for (unsigned int j = 0; j < afs_toc_data_reel_file_count(toc_data_reel); j++)
        {
            afs_toc_file * toc_file = afs_toc_data_get_file_by_id(toc_data, i * 10 + j);
            BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, j), toc_file) == DTRUE);
        }
    }

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_unique_id with input afs_toc_data pointer equal to NULL and unique id pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_file_by_unique_id_test1)
{
    afs_toc_data * toc_data = NULL;
    const char * unique_id_string = NULL;
    afs_toc_file * toc_file = afs_toc_data_get_file_by_unique_id(toc_data, unique_id_string);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(unique_id_string == NULL);
}
END_TEST


// Test function afs_toc_data_get_file_by_unique_id with input afs_toc_data pointer equal to NULL and empty unique id string
BOXING_START_TEST(afs_toc_data_get_file_by_unique_id_test2)
{
    afs_toc_data * toc_data = NULL;
    const char * unique_id_string = "";
    afs_toc_file * toc_file = afs_toc_data_get_file_by_unique_id(toc_data, unique_id_string);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(boxing_string_equal(unique_id_string, "") == DTRUE);
}
END_TEST


// Test function afs_toc_data_get_file_by_unique_id with input afs_toc_data pointer equal to NULL and not empty unique id string
BOXING_START_TEST(afs_toc_data_get_file_by_unique_id_test3)
{
    afs_toc_data * toc_data = NULL;
    const char * unique_id_string = "38A0AAAB-16B2-640C-5353-6DB8AE436700";
    afs_toc_file * toc_file = afs_toc_data_get_file_by_unique_id(toc_data, unique_id_string);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(boxing_string_equal(unique_id_string, "38A0AAAB-16B2-640C-5353-6DB8AE436700") == DTRUE);
}
END_TEST


// Test function afs_toc_data_get_file_by_unique_id with empty afs_toc_data instance and unique id pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_file_by_unique_id_test4)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    const char * unique_id_string = NULL;
    afs_toc_file * toc_file = afs_toc_data_get_file_by_unique_id(toc_data, unique_id_string);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(unique_id_string == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_unique_id with empty afs_toc_data instance and empty unique id string
BOXING_START_TEST(afs_toc_data_get_file_by_unique_id_test5)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    const char * unique_id_string = "";
    afs_toc_file * toc_file = afs_toc_data_get_file_by_unique_id(toc_data, unique_id_string);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(boxing_string_equal(unique_id_string, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_unique_id with empty afs_toc_data instance and not empty unique id string
BOXING_START_TEST(afs_toc_data_get_file_by_unique_id_test6)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    const char * unique_id_string = "38A0AAAB-16B2-640C-5353-6DB8AE436700";
    afs_toc_file * toc_file = afs_toc_data_get_file_by_unique_id(toc_data, unique_id_string);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(boxing_string_equal(unique_id_string, "38A0AAAB-16B2-640C-5353-6DB8AE436700") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_unique_id with not empty afs_toc_data instance and unique id pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_file_by_unique_id_test7)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    const char * unique_id_string = NULL;
    afs_toc_file * toc_file = afs_toc_data_get_file_by_unique_id(toc_data, unique_id_string);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(unique_id_string == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_unique_id with not empty afs_toc_data instance and empty unique id string
BOXING_START_TEST(afs_toc_data_get_file_by_unique_id_test8)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    const char * unique_id_string = "";
    afs_toc_file * toc_file = afs_toc_data_get_file_by_unique_id(toc_data, unique_id_string);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(boxing_string_equal(unique_id_string, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_unique_id with not empty afs_toc_data instance and not exist unique id
BOXING_START_TEST(afs_toc_data_get_file_by_unique_id_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    const char * unique_id_string = "38A0AAAB-16B2-640C-5353-6DB8AE436703";
    afs_toc_file * toc_file = afs_toc_data_get_file_by_unique_id(toc_data, unique_id_string);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 2, 2, 2);
    BOXING_ASSERT(toc_file == NULL);
    BOXING_ASSERT(boxing_string_equal(unique_id_string, "38A0AAAB-16B2-640C-5353-6DB8AE436703") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_file_by_unique_id with not empty afs_toc_data instance and exist unique id
BOXING_START_TEST(afs_toc_data_get_file_by_unique_id_test10)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 6, 6, 6);

    for (unsigned int i = 0; i < afs_toc_data_reel_count(toc_data); i++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, i);

        for (unsigned int j = 0; j < afs_toc_data_reel_file_count(toc_data_reel); j++)
        {
            char current_unique_id[100];
            sprintf(current_unique_id, "38A0AAAB-16B2-640C-5353-6DB8AE4367%02u", i * 10 + j);

            afs_toc_file * toc_file = afs_toc_data_get_file_by_unique_id(toc_data, current_unique_id);

            BOXING_ASSERT(afs_toc_file_equal(GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, j), toc_file) == DTRUE);
        }
    }

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_preview_layout_definition with input afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_preview_layout_definition_test1)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_data_get_preview_layout_definition(toc_data, 0);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);
}
END_TEST


// Test function afs_toc_data_get_preview_layout_definition with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_get_preview_layout_definition_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_data_get_preview_layout_definition(toc_data, 0);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_preview_layout_definition with afs_toc_data instance with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_get_preview_layout_definition_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_data_get_preview_layout_definition(toc_data, 0);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 0, 0, 0);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_preview_layout_definition with not empty afs_toc_data instance and index out of range
BOXING_START_TEST(afs_toc_data_get_preview_layout_definition_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 1, 1, 1);

    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_data_get_preview_layout_definition(toc_data, 1);

    test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 1, 1, 1);
    BOXING_ASSERT(toc_preview_layout_definition == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_preview_layout_definition with not empty afs_toc_data instance and index in range
BOXING_START_TEST(afs_toc_data_get_preview_layout_definition_test5)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);

    for (unsigned int i = 0; i < afs_toc_data_reel_count(toc_data); i++)
    {
        afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_data_get_preview_layout_definition(toc_data, i);
        BOXING_ASSERT(afs_toc_preview_layout_definition_equal(GVECTORN(toc_data->preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, i), toc_preview_layout_definition) == DTRUE);
    }

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with input afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test1)
{
	afs_toc_data * toc_data = NULL;

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	BOXING_ASSERT(toc_data == NULL);
	BOXING_ASSERT(layout_definitions == NULL);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with empty input afs_toc_data pointer
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test2)
{
	afs_toc_data * toc_data = afs_toc_data_create();

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
	BOXING_ASSERT(layout_definitions == NULL);

	afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but with afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test3)
{
	afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, -1);

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, -1);
	BOXING_ASSERT(layout_definitions == NULL);

	afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test4)
{
	afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 0);

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 0);
	BOXING_ASSERT(layout_definitions == NULL);

	afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but with number of reels equal to 0
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test5)
{
	afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 0, 5, 5);

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	test_afs_toc_data(toc_data, "Sove version", "Some index type", "Some job id", "Some reel id", 0, 5, 5);
	BOXING_ASSERT(layout_definitions == NULL);

	afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but reels don't have files
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test6)
{
	afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 0, 5, 5);
    afs_toc_data_reels_free(toc_data->reels);
	toc_data->reels = get_afs_toc_data_reels_instance8(3, 0);

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

    test_afs_toc_preview_layout_definitions(layout_definitions, 0);

	afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but reels contain files without visual frames
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test7)
{
	afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);

	unsigned int reels_count = afs_toc_data_reel_count(toc_data);

	for (unsigned int i = 0; i < reels_count; i++)
	{
		afs_toc_data_reel * current_reel = afs_toc_data_get_reel_by_index(toc_data, i);

		unsigned int files_count = afs_toc_data_reel_file_count(current_reel);

		for (unsigned int j = 0; j < files_count; j++)
		{
			afs_toc_file * current_file = afs_toc_data_reel_get_file_by_index(current_reel, j);
			current_file->types = AFS_TOC_FILE_TYPE_DIGITAL;
		}
	}

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	test_afs_toc_preview_layout_definitions(layout_definitions, 0);

	afs_toc_data_free(toc_data);
	afs_toc_preview_layout_definitions_free(layout_definitions);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but reels contain files with afs_toc_file_preview pointer equal to NULL
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test8)
{
	afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);

	unsigned int reels_count = afs_toc_data_reel_count(toc_data);

	for (unsigned int i = 0; i < reels_count; i++)
	{
		afs_toc_data_reel * current_reel = afs_toc_data_get_reel_by_index(toc_data, i);

		unsigned int files_count = afs_toc_data_reel_file_count(current_reel);

		for (unsigned int j = 0; j < files_count; j++)
		{
			afs_toc_file * current_file = afs_toc_data_reel_get_file_by_index(current_reel, j);
			afs_toc_file_preview_free(current_file->preview);
			current_file->preview = NULL;
		}
	}

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	test_afs_toc_preview_layout_definitions(layout_definitions, 0);

	afs_toc_data_free(toc_data);
	afs_toc_preview_layout_definitions_free(layout_definitions);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but reels contain files with empty afs_toc_file_preview instance
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test9)
{
	afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);

	unsigned int reels_count = afs_toc_data_reel_count(toc_data);

	for (unsigned int i = 0; i < reels_count; i++)
	{
		afs_toc_data_reel * current_reel = afs_toc_data_get_reel_by_index(toc_data, i);

		unsigned int files_count = afs_toc_data_reel_file_count(current_reel);

		for (unsigned int j = 0; j < files_count; j++)
		{
			afs_toc_file * current_file = afs_toc_data_reel_get_file_by_index(current_reel, j);
			afs_toc_file_preview_free(current_file->preview);
			current_file->preview = afs_toc_file_preview_create();
		}
	}

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	test_afs_toc_preview_layout_definitions(layout_definitions, 0);

	afs_toc_data_free(toc_data);
	afs_toc_preview_layout_definitions_free(layout_definitions);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but reels contain files with empty afs_toc_file_preview_page instances
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test10)
{
	afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);

	unsigned int reels_count = afs_toc_data_reel_count(toc_data);

	for (unsigned int i = 0; i < reels_count; i++)
	{
		afs_toc_data_reel * current_reel = afs_toc_data_get_reel_by_index(toc_data, i);

		unsigned int files_count = afs_toc_data_reel_file_count(current_reel);

		for (unsigned int j = 0; j < files_count; j++)
		{
			afs_toc_file * current_file = afs_toc_data_reel_get_file_by_index(current_reel, j);
			afs_toc_file_preview_free(current_file->preview);
			
			afs_toc_file_preview_pages * pages = get_filled_empty_pages_vector8(3);
			afs_toc_file_preview * toc_file_preview = afs_toc_file_preview_create2(pages);
			current_file->preview = toc_file_preview;
		}
	}

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	test_afs_toc_preview_layout_definitions(layout_definitions, 0);

	afs_toc_data_free(toc_data);
	afs_toc_preview_layout_definitions_free(layout_definitions);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but with layout definitions that do not match the layout definitions in the files
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test11)
{
	unsigned int layout_definitions_count = 5;

	afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, layout_definitions_count);

	for (unsigned int i = 0; i < layout_definitions_count; i++)
	{
		afs_toc_preview_layout_definition * current_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_data->preview_layout_definitions, i);
		boxing_string_free(current_layout_definition->id);
		current_layout_definition->id = boxing_string_clone("Do not match layout id");
	}

	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	test_afs_toc_preview_layout_definitions(layout_definitions, 0);

	afs_toc_data_free(toc_data);
	afs_toc_preview_layout_definitions_free(layout_definitions);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but with layout definitions that don't all match the layout definitions in the files
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test12)
{
	afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 10);

 	afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

	test_afs_toc_preview_layout_definitions(layout_definitions, 3);

	afs_toc_data_free(toc_data);
	afs_toc_preview_layout_definitions_free(layout_definitions);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but with layout definitions that all match the layout definitions in the files
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test13)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 3);

    afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

    test_afs_toc_preview_layout_definitions(layout_definitions, 3);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(layout_definitions);
}
END_TEST


// Test function afs_toc_data_get_used_preview_layout_definitions with not empty input afs_toc_data instance but when not all file layout definitions are present in afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_get_used_preview_layout_definitions_test14)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Sove version", "Some index type", "Some job id", "Some reel id", 5, 5, 1);

    afs_toc_preview_layout_definitions * layout_definitions = afs_toc_data_get_used_preview_layout_definitions(toc_data);

    test_afs_toc_preview_layout_definitions(layout_definitions, 1);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(layout_definitions);
}
END_TEST


// Test function afs_toc_data_set_version with input afs_toc_data pointer equal to NULL and version string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_version_test1)
{
    afs_toc_data * toc_data = NULL;
    const char * version = NULL;

    afs_toc_data_set_version(toc_data, version);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(version == NULL);
}
END_TEST


// Test function afs_toc_data_set_version with input afs_toc_data pointers equal to NULL and empty version string
BOXING_START_TEST(afs_toc_data_set_version_test2)
{
    afs_toc_data * toc_data = NULL;
    const char * version = "";

    afs_toc_data_set_version(toc_data, version);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(boxing_string_equal(version, "") == DTRUE);
}
END_TEST


// Test function afs_toc_data_set_version with input afs_toc_data pointers equal to NULL and not empty version string
BOXING_START_TEST(afs_toc_data_set_version_test3)
{
    afs_toc_data * toc_data = NULL;
    const char * version = "some version string";

    afs_toc_data_set_version(toc_data, version);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(boxing_string_equal(version, "some version string") == DTRUE);
}
END_TEST


// Test function afs_toc_data_set_version with empty input afs_toc_data instance and version string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_version_test4)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    const char * version = NULL;

    afs_toc_data_set_version(toc_data, version);

    BOXING_ASSERT(toc_data->version == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_version with empty input afs_toc_data instance and empty version string
BOXING_START_TEST(afs_toc_data_set_version_test5)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    BOXING_ASSERT(toc_data->version == NULL);

    const char * version = "";

    afs_toc_data_set_version(toc_data, version);

    BOXING_ASSERT(boxing_string_equal(toc_data->version, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_version with empty input afs_toc_data instance and not empty version string
BOXING_START_TEST(afs_toc_data_set_version_test6)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    BOXING_ASSERT(toc_data->version == NULL);

    const char * version = "some version string";

    afs_toc_data_set_version(toc_data, version);

    BOXING_ASSERT(boxing_string_equal(toc_data->version, "some version string") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_version with not empty input afs_toc_data instance and version string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_version_test7)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->version, "Some version") == DTRUE);

    const char * version = NULL;

    afs_toc_data_set_version(toc_data, version);

    BOXING_ASSERT(toc_data->version == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_version with not empty input afs_toc_data instance and empty version string
BOXING_START_TEST(afs_toc_data_set_version_test8)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->version, "Some version") == DTRUE);

    const char * version = "";

    afs_toc_data_set_version(toc_data, version);

    BOXING_ASSERT(boxing_string_equal(toc_data->version, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_version with not empty input afs_toc_data instance and not empty version string
BOXING_START_TEST(afs_toc_data_set_version_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->version, "Some version") == DTRUE);

    const char * version = "Some other version";

    afs_toc_data_set_version(toc_data, version);

    BOXING_ASSERT(boxing_string_equal(toc_data->version, "Some other version") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_index_type with input afs_toc_data pointer equal to NULL and index type string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_index_type_test1)
{
    afs_toc_data * toc_data = NULL;
    const char * index_type = NULL;

    afs_toc_data_set_index_type(toc_data, index_type);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(index_type == NULL);
}
END_TEST


// Test function afs_toc_data_set_index_type with input afs_toc_data pointers equal to NULL and empty index type string
BOXING_START_TEST(afs_toc_data_set_index_type_test2)
{
    afs_toc_data * toc_data = NULL;
    const char * index_type = "";

    afs_toc_data_set_index_type(toc_data, index_type);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(boxing_string_equal(index_type, "") == DTRUE);
}
END_TEST


// Test function afs_toc_data_set_index_type with input afs_toc_data pointers equal to NULL and not empty index type string
BOXING_START_TEST(afs_toc_data_set_index_type_test3)
{
    afs_toc_data * toc_data = NULL;
    const char * index_type = "some index type string";

    afs_toc_data_set_index_type(toc_data, index_type);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(boxing_string_equal(index_type, "some index type string") == DTRUE);
}
END_TEST


// Test function afs_toc_data_set_index_type with empty input afs_toc_data instance and index type string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_index_type_test4)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    const char * index_type = NULL;

    afs_toc_data_set_index_type(toc_data, index_type);

    BOXING_ASSERT(toc_data->index_type == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_index_type with empty input afs_toc_data instance and empty index type string
BOXING_START_TEST(afs_toc_data_set_index_type_test5)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    BOXING_ASSERT(toc_data->index_type == NULL);

    const char * index_type = "";

    afs_toc_data_set_index_type(toc_data, index_type);

    BOXING_ASSERT(boxing_string_equal(toc_data->index_type, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_index_type with empty input afs_toc_data instance and not empty index type string
BOXING_START_TEST(afs_toc_data_set_index_type_test6)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    BOXING_ASSERT(toc_data->index_type == NULL);

    const char * index_type = "some index type string";

    afs_toc_data_set_index_type(toc_data, index_type);

    BOXING_ASSERT(boxing_string_equal(toc_data->index_type, "some index type string") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_index_type with not empty input afs_toc_data instance and index type string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_index_type_test7)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->index_type, "Some index type") == DTRUE);

    const char * index_type = NULL;

    afs_toc_data_set_index_type(toc_data, index_type);

    BOXING_ASSERT(toc_data->index_type == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_index_type with not empty input afs_toc_data instance and empty index type string
BOXING_START_TEST(afs_toc_data_set_index_type_test8)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->index_type, "Some index type") == DTRUE);

    const char * index_type = "";

    afs_toc_data_set_index_type(toc_data, index_type);

    BOXING_ASSERT(boxing_string_equal(toc_data->index_type, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_index_type with not empty input afs_toc_data instance and not empty index type string
BOXING_START_TEST(afs_toc_data_set_index_type_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->index_type, "Some index type") == DTRUE);

    const char * index_type = "Some other index type";

    afs_toc_data_set_index_type(toc_data, index_type);

    BOXING_ASSERT(boxing_string_equal(toc_data->index_type, "Some other index type") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_job_id with input afs_toc_data pointer equal to NULL and job id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_job_id_test1)
{
    afs_toc_data * toc_data = NULL;
    const char * job_id = NULL;

    afs_toc_data_set_job_id(toc_data, job_id);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(job_id == NULL);
}
END_TEST


// Test function afs_toc_data_set_job_id with input afs_toc_data pointers equal to NULL and empty job id string
BOXING_START_TEST(afs_toc_data_set_job_id_test2)
{
    afs_toc_data * toc_data = NULL;
    const char * job_id = "";

    afs_toc_data_set_job_id(toc_data, job_id);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(boxing_string_equal(job_id, "") == DTRUE);
}
END_TEST


// Test function afs_toc_data_set_job_id with input afs_toc_data pointers equal to NULL and not empty job id string
BOXING_START_TEST(afs_toc_data_set_job_id_test3)
{
    afs_toc_data * toc_data = NULL;
    const char * job_id = "some job id string";

    afs_toc_data_set_job_id(toc_data, job_id);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(boxing_string_equal(job_id, "some job id string") == DTRUE);
}
END_TEST


// Test function afs_toc_data_set_job_id with empty input afs_toc_data instance and job id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_job_id_test4)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    const char * job_id = NULL;

    afs_toc_data_set_job_id(toc_data, job_id);

    BOXING_ASSERT(toc_data->job_id == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_job_id with empty input afs_toc_data instance and empty job id string
BOXING_START_TEST(afs_toc_data_set_job_id_test5)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    BOXING_ASSERT(toc_data->job_id == NULL);

    const char * job_id = "";

    afs_toc_data_set_job_id(toc_data, job_id);

    BOXING_ASSERT(boxing_string_equal(toc_data->job_id, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_job_id with empty input afs_toc_data instance and not empty job id string
BOXING_START_TEST(afs_toc_data_set_job_id_test6)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    BOXING_ASSERT(toc_data->job_id == NULL);

    const char * job_id = "some job id string";

    afs_toc_data_set_job_id(toc_data, job_id);

    BOXING_ASSERT(boxing_string_equal(toc_data->job_id, "some job id string") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_job_id with not empty input afs_toc_data instance and job id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_job_id_test7)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->job_id, "Some job id") == DTRUE);

    const char * job_id = NULL;

    afs_toc_data_set_job_id(toc_data, job_id);

    BOXING_ASSERT(toc_data->job_id == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_job_id with not empty input afs_toc_data instance and empty job id string
BOXING_START_TEST(afs_toc_data_set_job_id_test8)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->job_id, "Some job id") == DTRUE);

    const char * job_id = "";

    afs_toc_data_set_job_id(toc_data, job_id);

    BOXING_ASSERT(boxing_string_equal(toc_data->job_id, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_job_id with not empty input afs_toc_data instance and not empty job id string
BOXING_START_TEST(afs_toc_data_set_job_id_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->job_id, "Some job id") == DTRUE);

    const char * job_id = "Some other job id";

    afs_toc_data_set_job_id(toc_data, job_id);

    BOXING_ASSERT(boxing_string_equal(toc_data->job_id, "Some other job id") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_reel_id with input afs_toc_data pointer equal to NULL and reel id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_reel_id_test1)
{
    afs_toc_data * toc_data = NULL;
    const char * reel_id = NULL;

    afs_toc_data_set_reel_id(toc_data, reel_id);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(reel_id == NULL);
}
END_TEST


// Test function afs_toc_data_set_reel_id with input afs_toc_data pointers equal to NULL and empty reel id string
BOXING_START_TEST(afs_toc_data_set_reel_id_test2)
{
    afs_toc_data * toc_data = NULL;
    const char * reel_id = "";

    afs_toc_data_set_reel_id(toc_data, reel_id);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(boxing_string_equal(reel_id, "") == DTRUE);
}
END_TEST


// Test function afs_toc_data_set_reel_id with input afs_toc_data pointers equal to NULL and not empty reel id string
BOXING_START_TEST(afs_toc_data_set_reel_id_test3)
{
    afs_toc_data * toc_data = NULL;
    const char * reel_id = "some reel id string";

    afs_toc_data_set_reel_id(toc_data, reel_id);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(boxing_string_equal(reel_id, "some reel id string") == DTRUE);
}
END_TEST


// Test function afs_toc_data_set_reel_id with empty input afs_toc_data instance and reel id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_reel_id_test4)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    const char * reel_id = NULL;

    afs_toc_data_set_reel_id(toc_data, reel_id);

    BOXING_ASSERT(toc_data->reel_id == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_reel_id with empty input afs_toc_data instance and empty reel id string
BOXING_START_TEST(afs_toc_data_set_reel_id_test5)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    BOXING_ASSERT(toc_data->reel_id == NULL);

    const char * reel_id = "";

    afs_toc_data_set_reel_id(toc_data, reel_id);

    BOXING_ASSERT(boxing_string_equal(toc_data->reel_id, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_reel_id with empty input afs_toc_data instance and not empty reel id string
BOXING_START_TEST(afs_toc_data_set_reel_id_test6)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    BOXING_ASSERT(toc_data->reel_id == NULL);

    const char * reel_id = "some reel id string";

    afs_toc_data_set_reel_id(toc_data, reel_id);

    BOXING_ASSERT(boxing_string_equal(toc_data->reel_id, "some reel id string") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_reel_id with not empty input afs_toc_data instance and reel id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_set_reel_id_test7)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->reel_id, "Some reel id") == DTRUE);

    const char * reel_id = NULL;

    afs_toc_data_set_reel_id(toc_data, reel_id);

    BOXING_ASSERT(toc_data->reel_id == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_reel_id with not empty input afs_toc_data instance and empty reel id string
BOXING_START_TEST(afs_toc_data_set_reel_id_test8)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->reel_id, "Some reel id") == DTRUE);

    const char * reel_id = "";

    afs_toc_data_set_reel_id(toc_data, reel_id);

    BOXING_ASSERT(boxing_string_equal(toc_data->reel_id, "") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_set_reel_id with not empty input afs_toc_data instance and not empty reel id string
BOXING_START_TEST(afs_toc_data_set_reel_id_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 5, 5);
    BOXING_ASSERT(boxing_string_equal(toc_data->reel_id, "Some reel id") == DTRUE);

    const char * reel_id = "Some other reel id";

    afs_toc_data_set_reel_id(toc_data, reel_id);

    BOXING_ASSERT(boxing_string_equal(toc_data->reel_id, "Some other reel id") == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_reel with input afs_toc_data pointers equal to NULL and afs_toc_data_reel pointer equal to NULL
BOXING_START_TEST(afs_toc_data_add_reel_test1)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_add_reel with input afs_toc_data pointers equal to NULL and valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test2)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance8("Reel id 0", 0, 0);

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_add_reel with input afs_toc_data pointers equal to NULL and not valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test3)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance8("", 0, 0);

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_add_reel with empty input afs_toc_data instance and afs_toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_add_reel_test4)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_reel with empty input afs_toc_data instance and empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test5)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_add_reel with empty input afs_toc_data instance and not valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test6)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance8("", 0, 0);

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_add_reel with empty input afs_toc_data instance and valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test7)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance8("Reel id 0", 3, 0);

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, 1, -1, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_reel with input afs_toc_data instance with empty afs_toc_data_reels instance and afs_toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_add_reel_test8)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_reel with input afs_toc_data instance with empty afs_toc_data_reels instance and empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_add_reel with input afs_toc_data instance with empty afs_toc_data_reels instance and not valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test10)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance8("", 0, 0);

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_add_reel with input afs_toc_data instance with empty afs_toc_data_reels instance and valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test11)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance8("Reel id 0", 3, 0);

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 1, 3, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_reel with not empty input afs_toc_data instance and afs_toc_data_reel pointers equal to NULL
BOXING_START_TEST(afs_toc_data_add_reel_test12)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_data_reel * toc_data_reel = NULL;

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_reel with not empty input afs_toc_data instance and empty afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test13)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_data_reel * toc_data_reel = afs_toc_data_reel_create();

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_add_reel with not empty input afs_toc_data instance and not valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test14)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance8("", 3, 0);

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    afs_toc_data_reel_free(toc_data_reel);
}
END_TEST


// Test function afs_toc_data_add_reel with not empty input afs_toc_data instance and valid afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_add_reel_test15)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_data_reel * toc_data_reel = get_afs_toc_data_reel_instance8("Reel id 3", 3, 3);

    DBOOL result = afs_toc_data_add_reel(toc_data, toc_data_reel);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 4, 3, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with input afs_toc_data pointers equal to NULL and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_add_metadata_source_test1)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_metadata_source * source = NULL;

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with input afs_toc_data pointers equal to NULL and valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_data_add_metadata_source_test2)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance8(1);

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with input afs_toc_data pointers equal to NULL and not valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_data_add_metadata_source_test3)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance8(1);
    source->file_id = -3;

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with empty input afs_toc_data instance and afs_toc_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_data_add_metadata_source_test4)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_metadata_source * source = NULL;

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with empty input afs_toc_data instance and empty afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_data_add_metadata_source_test5)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_metadata_source * source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with empty input afs_toc_data instance and not valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_data_add_metadata_source_test6)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance8(1);
    source->file_id = -3;

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with empty input afs_toc_data instance and valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_data_add_metadata_source_test7)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance8(0);

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, 1, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with input afs_toc_data instance with empty afs_toc_metadata instance and afs_toc_metadata_source pointer equal to NULL
BOXING_START_TEST(afs_toc_data_add_metadata_source_test8)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 0, 3);
    afs_toc_metadata_source * source = NULL;

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with input afs_toc_data instance with empty afs_toc_metadata instance and empty afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_data_add_metadata_source_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 0, 3);
    afs_toc_metadata_source * source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with input afs_toc_data instance with empty afs_toc_metadata instance and not valid afs_toc_metadata_source
BOXING_START_TEST(afs_toc_data_add_metadata_source_test10)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 0, 3);
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance8(1);
    source->file_id = -3;

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with input afs_toc_data instance with empty afs_toc_metadata instance and valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_data_add_metadata_source_test11)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 0, 3);
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance8(0);

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 1, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with not empty input afs_toc_data instance and afs_toc_metadata_source pointers equal to NULL
BOXING_START_TEST(afs_toc_data_add_metadata_source_test12)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_metadata_source * source = NULL;

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with not empty input afs_toc_data instance and empty afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_data_add_metadata_source_test13)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_metadata_source * source = afs_toc_metadata_source_create();

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with not empty input afs_toc_data instance and not valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_data_add_metadata_source_test14)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance8(2);
    source->file_id = -3;

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    afs_toc_metadata_source_free(source);
}
END_TEST


// Test function afs_toc_data_add_metadata_source with not empty input afs_toc_data instance and valid afs_toc_metadata_source instance
BOXING_START_TEST(afs_toc_data_add_metadata_source_test15)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance8(3);

    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 4, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with input afs_toc_data pointers equal to NULL and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test1)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with input afs_toc_data pointers equal to NULL and valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test2)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8("id 0", "name 0", 5);

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with input afs_toc_data pointers equal to NULL and not valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test3)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8("", "", 5);

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with empty input afs_toc_data instance and afs_toc_preview_layout_definition pointers equal to NULL
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test4)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with empty input afs_toc_data instance and empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test5)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with empty input afs_toc_data instance and not valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test6)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8("", "", 5);

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with empty input afs_toc_data instance and valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test7)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8("id 0", "name 0", 5);

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, 1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with input afs_toc_data instance with empty afs_toc_preview_layout_definitions instance and afs_toc_preview_layout_definition pointer equal to NULL
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test8)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with input afs_toc_data instance with empty afs_toc_preview_layout_definitions instance and empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with input afs_toc_data instance with empty afs_toc_preview_layout_definitions instance and not valid afs_toc_preview_layout_definition
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test10)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8("", "", 5);

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with input afs_toc_data instance with empty afs_toc_preview_layout_definitions instance and valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test11)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8("id 0", "name 0", 5);

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with not empty input afs_toc_data instance and afs_toc_preview_layout_definition pointers equal to NULL
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test12)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with not empty input afs_toc_data instance and empty afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test13)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definition_create();

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with not empty input afs_toc_data instance and not valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test14)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8("", "", 5);

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definition_free(toc_preview_layout_definition);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definition with not empty input afs_toc_data instance and valid afs_toc_preview_layout_definition instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definition_test15)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8("id 3", "name 3", 5);

    DBOOL result = afs_toc_data_add_preview_layout_definition(toc_data, toc_preview_layout_definition);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 4);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data pointers equal to NULL and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test1)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data pointers equal to NULL and empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test2)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data pointers equal to NULL and afs_toc_preview_layout_definitions instance with one not valid item
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test3)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(1);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 0);
    boxing_string_free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data pointers equal to NULL and afs_toc_preview_layout_definitions instance with one valid item
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test4)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(1);

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data pointers equal to NULL and afs_toc_preview_layout_definitions instance with three items with one of the items not valid
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test5)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 1);
    boxing_string_free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data pointers equal to NULL and afs_toc_preview_layout_definitions instance with three valid items
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test6)
{
    afs_toc_data * toc_data = NULL;
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(3);

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with empty input afs_toc_data instance and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test7)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with with empty input afs_toc_data instance and empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test8)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with with empty input afs_toc_data instance and afs_toc_preview_layout_definitions instance with one not valid item
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test9)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(1);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 0);
    boxing_string_free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with with empty input afs_toc_data instance and afs_toc_preview_layout_definitions instance with one valid item
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test10)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(1);

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, 1);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with with empty input afs_toc_data instance and afs_toc_preview_layout_definitions instance with three items with one of the items not valid
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test11)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 1);
    boxing_string_free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with with empty input afs_toc_data instance and afs_toc_preview_layout_definitions instance with three valid items
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test12)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(3);

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, 3);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data instance with empty internal afs_toc_preview_layout_definitions instance and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test13)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data instance with empty internal afs_toc_preview_layout_definitions instance and empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test14)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data instance with empty internal afs_toc_preview_layout_definitions instance and afs_toc_preview_layout_definitions instance with one not valid item
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test15)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(1);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 0);
    boxing_string_free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data instance with empty internal afs_toc_preview_layout_definitions instance and afs_toc_preview_layout_definitions instance with one valid item
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test16)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(1);

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 1);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data instance with empty internal afs_toc_preview_layout_definitions instance and afs_toc_preview_layout_definitions instance with three items with one of the items not valid
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test17)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 1);
    boxing_string_free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with input afs_toc_data instance with empty internal afs_toc_preview_layout_definitions instance and afs_toc_preview_layout_definitions instance with three valid items
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test18)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(3);

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with not empty input afs_toc_data instance and afs_toc_preview_layout_definitions pointer equal to NULL
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test19)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with not empty input afs_toc_data instance and empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test20)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with not empty input afs_toc_data instance and afs_toc_preview_layout_definitions instance with one not valid item
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test21)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(1);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 0);
    boxing_string_free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with not empty input afs_toc_data instance and afs_toc_preview_layout_definitions instance with one valid item
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test22)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8("id 3", "name 3", 5);
    afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 4);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with not empty input afs_toc_data instance and afs_toc_preview_layout_definitions instance with three items with one of the items not valid
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test23)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = get_afs_toc_preview_layout_definitions_instance8(3);
    afs_toc_preview_layout_definition * toc_preview_layout_definition = afs_toc_preview_layout_definitions_get_layout_definition_by_index(toc_preview_layout_definitions, 1);
    boxing_string_free(toc_preview_layout_definition->id);
    toc_preview_layout_definition->id = NULL;

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_add_preview_layout_definitions with not empty input afs_toc_data instance and afs_toc_preview_layout_definitions instance with three valid items
BOXING_START_TEST(afs_toc_data_add_preview_layout_definitions_test24)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 3);
    
    afs_toc_preview_layout_definitions * toc_preview_layout_definitions = afs_toc_preview_layout_definitions_create();
    for (unsigned int i = 3; i < 6; i++)
    {
        char current_id[10];
        char current_name[10];

        sprintf(current_id, "id %u", i);
        sprintf(current_name, "name %u", i);

        afs_toc_preview_layout_definition * toc_preview_layout_definition = get_afs_toc_preview_layout_definition_instance8(current_id, current_name, 5);
        afs_toc_preview_layout_definitions_add_layout_definition(toc_preview_layout_definitions, toc_preview_layout_definition);
    }

    DBOOL result = afs_toc_data_add_preview_layout_definitions(toc_data, toc_preview_layout_definitions);

    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 6);

    afs_toc_data_free(toc_data);
    afs_toc_preview_layout_definitions_free(toc_preview_layout_definitions);
}
END_TEST


// Test function afs_toc_data_reel_count with input afs_toc_data pointers equal to NULL
BOXING_START_TEST(afs_toc_data_reel_count_test1)
{
    afs_toc_data * toc_data = NULL;
    unsigned int reels_count = afs_toc_data_reel_count(toc_data);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(reels_count == 0);
}
END_TEST


// Test function afs_toc_data_reel_count with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_reel_count_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    unsigned int reels_count = afs_toc_data_reel_count(toc_data);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(reels_count == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_reel_count with afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reel_count_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);

    unsigned int reels_count = afs_toc_data_reel_count(toc_data);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);
    BOXING_ASSERT(reels_count == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_reel_count with afs_toc_data instance with not empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_reel_count_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 5, 3, 3);

    unsigned int reels_count = afs_toc_data_reel_count(toc_data);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 5, 3, 3);
    BOXING_ASSERT(reels_count == 5);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_metadata_source_count with input afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_metadata_source_count_test1)
{
    afs_toc_data * toc_data = NULL;
    unsigned int sources_count = afs_toc_data_metadata_source_count(toc_data);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(sources_count == 0);
}
END_TEST


// Test function afs_toc_data_metadata_source_count with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_metadata_source_count_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    unsigned int sources_count = afs_toc_data_metadata_source_count(toc_data);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(sources_count == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_metadata_source_count with afs_toc_data instance with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_data_metadata_source_count_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 0, 3);

    unsigned int sources_count = afs_toc_data_metadata_source_count(toc_data);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 0, 3);
    BOXING_ASSERT(sources_count == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_metadata_source_count with afs_toc_data instance with not empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_data_metadata_source_count_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 5, 3);

    unsigned int sources_count = afs_toc_data_metadata_source_count(toc_data);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 5, 3);
    BOXING_ASSERT(sources_count == 5);

    afs_toc_metadata_source * source = get_afs_toc_metadata_source_instance8(5);
    DBOOL result = afs_toc_data_add_metadata_source(toc_data, source);

    BOXING_ASSERT(result == DTRUE);
    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 6, 3);

    sources_count = afs_toc_data_metadata_source_count(toc_data);

    BOXING_ASSERT(sources_count == 6);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_preview_layout_definition_count with input afs_toc_data pointers equal to NULL
BOXING_START_TEST(afs_toc_data_preview_layout_definition_count_test1)
{
    afs_toc_data * toc_data = NULL;
    unsigned int definitions_count = afs_toc_data_preview_layout_definition_count(toc_data);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(definitions_count == 0);
}
END_TEST


// Test function afs_toc_data_preview_layout_definition_count with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_preview_layout_definition_count_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    unsigned int definitions_count = afs_toc_data_preview_layout_definition_count(toc_data);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(definitions_count == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_preview_layout_definition_count with afs_toc_data instance with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_preview_layout_definition_count_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);

    unsigned int definitions_count = afs_toc_data_preview_layout_definition_count(toc_data);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 0);
    BOXING_ASSERT(definitions_count == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_preview_layout_definition_count with afs_toc_data instance with not empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_preview_layout_definition_count_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 5);

    unsigned int definitions_count = afs_toc_data_preview_layout_definition_count(toc_data);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 3, 3, 5);
    BOXING_ASSERT(definitions_count == 5);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_file_count with input afs_toc_data pointers equal to NULL
BOXING_START_TEST(afs_toc_data_file_count_test1)
{
    afs_toc_data * toc_data = NULL;
    unsigned int file_count = afs_toc_data_file_count(toc_data);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(file_count == 0);
}
END_TEST


// Test function afs_toc_data_file_count with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_file_count_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    unsigned int file_count = afs_toc_data_file_count(toc_data);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(file_count == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_file_count with afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_file_count_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);

    unsigned int file_count = afs_toc_data_file_count(toc_data);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);
    BOXING_ASSERT(file_count == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_file_count with not empty afs_toc_data instance with one afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_file_count_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 1, 3, 3);

    unsigned int file_count = afs_toc_data_file_count(toc_data);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 1, 3, 3);
    BOXING_ASSERT(file_count == 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_file_count with not empty afs_toc_data instance with six afs_toc_data_reel instances
BOXING_START_TEST(afs_toc_data_file_count_test5)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 6, 3, 3);

    unsigned int file_count = afs_toc_data_file_count(toc_data);

    test_afs_toc_data(toc_data, "Some version", "Some index type", "Some job id", "Some reel id", 6, 3, 3);
    BOXING_ASSERT(file_count == 18);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_frame_count with afs_toc_data structure pointer equal to NULL
BOXING_START_TEST(afs_toc_data_frame_count_test1)
{
    afs_toc_data * toc_data = NULL;

    unsigned int frame_count = afs_toc_data_frame_count(toc_data);

    BOXING_ASSERT(frame_count == 0);
}
END_TEST


// Test function afs_toc_data_frame_count with empty afs_toc_data structure instance
BOXING_START_TEST(afs_toc_data_frame_count_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();

    unsigned int frame_count = afs_toc_data_frame_count(toc_data);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_frame_count with afs_toc_data structure instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_frame_count_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);

    unsigned int frame_count = afs_toc_data_frame_count(toc_data);

    BOXING_ASSERT(frame_count == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_frame_count with afs_toc_data structure instance with one afs_toc_data_reel instance and different file type
BOXING_START_TEST(afs_toc_data_frame_count_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_different_reel_parent(1);

    unsigned int frame_count = afs_toc_data_frame_count(toc_data);

    BOXING_ASSERT(frame_count == 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_frame_count with afs_toc_data structure instance with six afs_toc_data_reel instances and different file type
BOXING_START_TEST(afs_toc_data_frame_count_test5)
{
    afs_toc_data * toc_data = get_afs_toc_data_different_reel_parent(6);

    unsigned int frame_count = afs_toc_data_frame_count(toc_data);

    BOXING_ASSERT(frame_count == 18);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_first_frame with input afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_first_frame_test1)
{
    afs_toc_data * toc_data = NULL;

    int start_frame = afs_toc_data_first_frame(toc_data);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(start_frame == -1);
}
END_TEST


// Test function afs_toc_data_first_frame with empty input afs_toc_data instance
BOXING_START_TEST(afs_toc_data_first_frame_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();

    int start_frame = afs_toc_data_first_frame(toc_data);

    BOXING_ASSERT(start_frame == -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_first_frame with input afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_first_frame_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);

    int start_frame = afs_toc_data_first_frame(toc_data);

    BOXING_ASSERT(start_frame == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_first_frame with not empty input afs_toc_data instance with one afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_first_frame_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_different_reel_parent(1);

    int start_frame = afs_toc_data_first_frame(toc_data);

    BOXING_ASSERT(start_frame == 2);

    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, 0);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->start_frame = 12;

    start_frame = afs_toc_data_first_frame(toc_data);
    BOXING_ASSERT(start_frame == 12);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_first_frame with not empty input afs_toc_data instance with six afs_toc_data_reel instances
BOXING_START_TEST(afs_toc_data_first_frame_test5)
{
    afs_toc_data * toc_data = get_afs_toc_data_different_reel_parent(6);

    int start_frame = afs_toc_data_first_frame(toc_data);

    BOXING_ASSERT(start_frame == 2);

    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, 0);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 0)->start_frame = 12;

    start_frame = afs_toc_data_first_frame(toc_data);
    BOXING_ASSERT(start_frame == 12);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_last_frame with input afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_last_frame_test1)
{
    afs_toc_data * toc_data = NULL;

    int last_frame = afs_toc_data_last_frame(toc_data);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(last_frame == -1);
}
END_TEST


// Test function afs_toc_data_last_frame with empty input afs_toc_data instance
BOXING_START_TEST(afs_toc_data_last_frame_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();

    int last_frame = afs_toc_data_last_frame(toc_data);

    BOXING_ASSERT(last_frame == -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_last_frame with input afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_last_frame_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some index type", "Some job id", "Some reel id", 0, 3, 3);

    int last_frame = afs_toc_data_last_frame(toc_data);

    BOXING_ASSERT(last_frame == 0);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_last_frame with not empty input afs_toc_data instance with one afs_toc_data_reel instance
BOXING_START_TEST(afs_toc_data_last_frame_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_different_reel_parent(1);

    int last_frame = afs_toc_data_last_frame(toc_data);

    BOXING_ASSERT(last_frame == 4);

    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, 0);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2)->end_frame = 12;

    last_frame = afs_toc_data_last_frame(toc_data);
    BOXING_ASSERT(last_frame == 12);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_last_frame with not empty input afs_toc_data instance with six afs_toc_data_reel instances
BOXING_START_TEST(afs_toc_data_last_frame_test5)
{
    afs_toc_data * toc_data = get_afs_toc_data_different_reel_parent(6);

    int last_frame = afs_toc_data_last_frame(toc_data);

    BOXING_ASSERT(last_frame == 4);

    afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, 1);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 1)->end_frame = 12;

    last_frame = afs_toc_data_last_frame(toc_data);
    BOXING_ASSERT(last_frame == 4);

    toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, 5);
    GVECTORN(toc_data_reel->files->tocs, afs_toc_file *, 2)->end_frame = 12;

    last_frame = afs_toc_data_last_frame(toc_data);
    BOXING_ASSERT(last_frame == 12);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data pointers equal to NULL
BOXING_START_TEST(afs_toc_data_is_valid_test1)
{
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_is_valid(toc_data);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_is_valid with empty input afs_toc_data instance
BOXING_START_TEST(afs_toc_data_is_valid_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with version string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_is_valid_test3)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8(NULL, "reel", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, NULL, "reel", "Some job id", "Some reel id", 3, 3, 3);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with empty version string
BOXING_START_TEST(afs_toc_data_is_valid_test4)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("", "reel", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "", "reel", "Some job id", "Some reel id", 3, 3, 3);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with index type string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_is_valid_test5)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", NULL, "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", NULL, "Some job id", "Some reel id", 3, 3, 3);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with empty index type string
BOXING_START_TEST(afs_toc_data_is_valid_test6)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", "", "Some job id", "Some reel id", 3, 3, 3);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with not correct index type string
BOXING_START_TEST(afs_toc_data_is_valid_test7)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "Some not correct string", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", "Some not correct string", "Some job id", "Some reel id", 3, 3, 3);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with job id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_is_valid_test8)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", NULL, "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", "reel", NULL, "Some reel id", 3, 3, 3);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with empty job id string
BOXING_START_TEST(afs_toc_data_is_valid_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", "reel", "", "Some reel id", 3, 3, 3);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with reel id string pointer equal to NULL
BOXING_START_TEST(afs_toc_data_is_valid_test10)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", NULL, 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", "reel", "Some job id", NULL, 3, 3, 3);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with empty reel id string
BOXING_START_TEST(afs_toc_data_is_valid_test11)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "", 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", "reel", "Some job id", "", 3, 3, 3);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with afs_toc_data_reels structure pointer equal to NULL
BOXING_START_TEST(afs_toc_data_is_valid_test12)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "Some reel id", 0, 3, 3);
    afs_toc_data_reels_free(toc_data->reels);
    toc_data->reels = NULL;

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", "reel", "Some job id", "Some reel id", -1, 3, 3);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with empty afs_toc_data_reels structure instance
BOXING_START_TEST(afs_toc_data_is_valid_test13)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "Some reel id", 0, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", "reel", "Some job id", "Some reel id", 0, 3, 3);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with not valid afs_toc_data_reels structure instance
BOXING_START_TEST(afs_toc_data_is_valid_test14)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "Some reel id", 3, 3, 3);
    afs_toc_data_reel_set_id(GVECTORN(toc_data->reels->reels, afs_toc_data_reel *, 1), "");

    DBOOL result = afs_toc_data_is_valid(toc_data);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with afs_toc_preview_layout_definitions structure pointer equal to NULL
BOXING_START_TEST(afs_toc_data_is_valid_test15)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "Some reel id", 3, 3, 0);
    afs_toc_preview_layout_definitions_free(toc_data->preview_layout_definitions);
    toc_data->preview_layout_definitions = NULL;

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", "reel", "Some job id", "Some reel id", 3, 3, -1);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with empty afs_toc_preview_layout_definitions structure instance
BOXING_START_TEST(afs_toc_data_is_valid_test16)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "Some reel id", 3, 3, 0);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with not valid afs_toc_preview_layout_definitions structure instance
BOXING_START_TEST(afs_toc_data_is_valid_test17)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "Some reel id", 3, 3, 3);
    boxing_string_free(GVECTORN(toc_data->preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, 1)->id);
    GVECTORN(toc_data->preview_layout_definitions->layout_definitions, afs_toc_preview_layout_definition *, 1)->id = NULL;

    DBOOL result = afs_toc_data_is_valid(toc_data);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with afs_toc_metadata structure pointer equal to NULL
BOXING_START_TEST(afs_toc_data_is_valid_test18)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "Some reel id", 3, 0, 3);
    afs_toc_metadata_free(toc_data->metadata);
    toc_data->metadata = NULL;

    DBOOL result = afs_toc_data_is_valid(toc_data);

    test_afs_toc_data(toc_data, "Some version", "reel", "Some job id", "Some reel id", 3, -1, 3);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with empty afs_toc_metadata structure instance
BOXING_START_TEST(afs_toc_data_is_valid_test19)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "Some reel id", 3, 0, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with not valid afs_toc_metadata structure instance
BOXING_START_TEST(afs_toc_data_is_valid_test20)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "Some reel id", 3, 3, 3);
    GVECTORN(toc_data->metadata->sources, afs_toc_metadata_source *, 1)->file_id = -3;

    DBOOL result = afs_toc_data_is_valid(toc_data);

    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with all valid data and index type string equal to "reel"
BOXING_START_TEST(afs_toc_data_is_valid_test21)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "reel", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_is_valid with input afs_toc_data instance with all valid data and index type string equal to "job"
BOXING_START_TEST(afs_toc_data_is_valid_test22)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_is_valid(toc_data);

    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with input afs_toc_data pointers equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_add_frame_offset_test1)
{
    afs_toc_data * toc_data = NULL;
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with input afs_toc_data pointers equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_add_frame_offset_test2)
{
    afs_toc_data * toc_data = NULL;
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with input afs_toc_data pointers equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_add_frame_offset_test3)
{
    afs_toc_data * toc_data = NULL;
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with input afs_toc_data pointers equal to NULL and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_add_frame_offset_test4)
{
    afs_toc_data * toc_data = NULL;
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_ALL);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(offset == 5);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with empty input afs_toc_data instance and section equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_add_frame_offset_test5)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with empty input afs_toc_data instance and section equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_add_frame_offset_test6)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with empty input afs_toc_data instance and section equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_add_frame_offset_test7)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with empty input afs_toc_data instance and section equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_add_frame_offset_test8)
{
    afs_toc_data * toc_data = afs_toc_data_create();
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_ALL);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with input afs_toc_data instance with empty afs_toc_data_reels instance and type equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_add_frame_offset_test9)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 0, 3, 3);
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 0, 3, 3);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with input afs_toc_data instance with empty afs_toc_data_reels instance and type equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_add_frame_offset_test10)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 0, 3, 3);
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 0, 3, 3);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with input afs_toc_data instance with empty afs_toc_data_reels instance and type equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_add_frame_offset_test11)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 0, 3, 3);
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 0, 3, 3);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with input afs_toc_data instance with empty afs_toc_data_reels instance and type equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_add_frame_offset_test12)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 0, 3, 3);
    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_ALL);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 0, 3, 3);
    BOXING_ASSERT(offset == 5);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with not empty input afs_toc_data instance and type equal to AFS_TOC_DATA_REEL_SECTION_DIGITAL
BOXING_START_TEST(afs_toc_data_add_frame_offset_test13)
{
    afs_toc_data * toc_data = get_afs_toc_data_different_reel_parent(3);

    unsigned int offset = 5;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_DIGITAL);

    BOXING_ASSERT(offset == 5);
    
    for (unsigned int r = 0; r < afs_toc_data_reel_count(toc_data); r++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, r);

        for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
        {
            afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, i);
            BOXING_ASSERT(toc_file->start_frame == (int)(2 + i + offset));
            BOXING_ASSERT(toc_file->end_frame == (int)(2 + i + offset));

            for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
            {
                afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
                BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j));
            }
        }
    }

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with not empty input afs_toc_data instance and type equal to AFS_TOC_DATA_REEL_SECTION_PREVIEW
BOXING_START_TEST(afs_toc_data_add_frame_offset_test14)
{
    afs_toc_data * toc_data = get_afs_toc_data_different_reel_parent(3);

    unsigned int offset = 6;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_PREVIEW);

    BOXING_ASSERT(offset == 6);

    for (unsigned int r = 0; r < afs_toc_data_reel_count(toc_data); r++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, r);

        for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
        {
            afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, i);
            BOXING_ASSERT(toc_file->start_frame == (int)(2 + i));
            BOXING_ASSERT(toc_file->end_frame == (int)(2 + i));

            for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
            {
                afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
                BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j + offset));
            }
        }
    }

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with not empty input afs_toc_data instance and type equal to AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
BOXING_START_TEST(afs_toc_data_add_frame_offset_test15)
{
    afs_toc_data * toc_data = get_afs_toc_data_different_reel_parent(3);

    unsigned int offset = 7;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS);

    BOXING_ASSERT(offset == 7);

    for (unsigned int r = 0; r < afs_toc_data_reel_count(toc_data); r++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, r);

        for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
        {
            afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, i);
            BOXING_ASSERT(toc_file->start_frame == (int)(2 + i));
            BOXING_ASSERT(toc_file->end_frame == (int)(2 + i));

            for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
            {
                afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
                BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j));
            }
        }
    }

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_add_frame_offset with not empty input afs_toc_data instance and type equal to AFS_TOC_DATA_REEL_SECTION_ALL
BOXING_START_TEST(afs_toc_data_add_frame_offset_test16)
{
    afs_toc_data * toc_data = get_afs_toc_data_different_reel_parent(3);

    unsigned int offset = 8;

    afs_toc_data_add_frame_offset(toc_data, offset, AFS_TOC_DATA_REEL_SECTION_ALL);

    BOXING_ASSERT(offset == 8);

    for (unsigned int r = 0; r < afs_toc_data_reel_count(toc_data); r++)
    {
        afs_toc_data_reel * toc_data_reel = afs_toc_data_get_reel_by_index(toc_data, r);

        for (unsigned int i = 0; i < afs_toc_data_reel_file_count(toc_data_reel); i++)
        {
            afs_toc_file * toc_file = afs_toc_data_reel_get_file_by_index(toc_data_reel, i);
            BOXING_ASSERT(toc_file->start_frame == (int)(2 + i + offset));
            BOXING_ASSERT(toc_file->end_frame == (int)(2 + i + offset));

            for (unsigned int j = 0; j < afs_toc_file_preview_get_page_count(toc_file->preview); j++)
            {
                afs_toc_file_preview_page * toc_file_preview_page = afs_toc_file_preview_get_page(toc_file->preview, j);
                BOXING_ASSERT(toc_file_preview_page->start_frame == (9 + j + offset));
            }
        }
    }

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_file with file_name pointer equal to NULL and afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_save_file_test1)
{
    const char * file_name = NULL;
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_save_file(toc_data, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_save_file with file_name pointer equal to NULL and afs_toc_data pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_save_file_test2)
{
    const char * file_name = NULL;

    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_save_file(toc_data, file_name, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_file with file_name pointer not equal to NULL and afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_save_file_test3)
{
    const char * file_name = "afs_toc_data_save_file_test.xml";
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_save_file(toc_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_save_file with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_save_file_test4)
{
    const char * file_name = "afs_toc_data_save_file_empty_test.xml";
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_save_file(toc_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_file with afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_save_file_test5)
{
    const char * file_name = "afs_toc_data_empty_reels_save_file_test.xml";
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 0, 3, 3);

    DBOOL result = afs_toc_data_save_file(toc_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_file with afs_toc_data instance with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_save_file_test6)
{
    const char * file_name = "afs_toc_data_empty_definitions_save_file_test.xml";
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 3, 0);

    DBOOL result = afs_toc_data_save_file(toc_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_file with afs_toc_data instance with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_data_save_file_test7)
{
    const char * file_name = "afs_toc_data_empty_metadata_save_file_test.xml";
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 0, 3);

    DBOOL result = afs_toc_data_save_file(toc_data, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_file with not valid afs_toc_data instance
BOXING_START_TEST(afs_toc_data_save_file_test8)
{
    const char * file_name = "afs_toc_data_save_file_not_valid_test.xml";
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_save_file(toc_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_file with afs_toc_data instance with valid data, in compact format
BOXING_START_TEST(afs_toc_data_save_file_test9)
{
    const char * file_name = "afs_toc_data_save_file_compact_test.xml";

    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_save_file(toc_data, file_name, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_file with afs_toc_data_reels instance with valid data, in not compact format
BOXING_START_TEST(afs_toc_data_save_file_test10)
{
    const char * file_name = "afs_toc_data_save_file_not_compact_test.xml";

    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_save_file(toc_data, file_name, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_string with afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_save_string_test1)
{
    afs_toc_data * toc_data = NULL;

    char * result = afs_toc_data_save_string(toc_data, DTRUE);

    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_toc_data_save_string with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_save_string_test2)
{
    afs_toc_data * toc_data = afs_toc_data_create();

    char * result = afs_toc_data_save_string(toc_data, DTRUE);

    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_string with afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_save_string_test3)
{
    char * test_string = read_xml_toc_data("afs_toc_data_empty_reels_save_file_test.xml");

    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 0, 3, 3);

    char * result = afs_toc_data_save_string(toc_data, DTRUE);

    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(boxing_string_equal(result, test_string) == DTRUE);

    afs_toc_data_free(toc_data);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_toc_data_save_string with afs_toc_data instance with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_data_save_string_test4)
{
    char * test_string = read_xml_toc_data("afs_toc_data_empty_metadata_save_file_test.xml");

    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 0, 3);

    char * result = afs_toc_data_save_string(toc_data, DFALSE);

    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(boxing_string_equal(result, test_string) == DTRUE);

    afs_toc_data_free(toc_data);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_toc_data_save_string with afs_toc_data instance with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_save_string_test5)
{
    char * test_string = read_xml_toc_data("afs_toc_data_empty_definitions_save_file_test.xml");

    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 3, 0);

    char * result = afs_toc_data_save_string(toc_data, DTRUE);

    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(boxing_string_equal(result, test_string) == DTRUE);

    afs_toc_data_free(toc_data);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_toc_data_save_string with not valid afs_toc_data instance
BOXING_START_TEST(afs_toc_data_save_string_test6)
{
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "", "Some job id", "Some reel id", 3, 3, 3);

    char * result = afs_toc_data_save_string(toc_data, DTRUE);

    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == NULL);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_string with valid afs_toc_data instance, in compact format
BOXING_START_TEST(afs_toc_data_save_string_test7)
{
    char * test_string = read_xml_toc_data("afs_toc_data_save_file_compact_test.xml");

    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    char * result = afs_toc_data_save_string(toc_data, DTRUE);

    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(boxing_string_equal(result, test_string) == DTRUE);

    afs_toc_data_free(toc_data);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_toc_data_save_string with valid afs_toc_data instance, in not compact format
BOXING_START_TEST(afs_toc_data_save_string_test8)
{
    char * test_string = read_xml_toc_data("afs_toc_data_save_file_not_compact_test.xml");

    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    char * result = afs_toc_data_save_string(toc_data, DFALSE);

    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(boxing_string_equal(result, test_string) == DTRUE);

    afs_toc_data_free(toc_data);
    boxing_string_free(test_string);
    boxing_string_free(result);
}
END_TEST


// Test function afs_toc_data_save_xml with mxml_node_t pointer equal to NULL and afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_save_xml(toc_data, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_save_xml with mxml_node_t pointer equal to NULL and afs_toc_data pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_save_xml(toc_data, out);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_save_xml with mxml_node_t pointer not equal to NULL and afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_save_xml_test3)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_save_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_save_xml with empty afs_toc_data instance
BOXING_START_TEST(afs_toc_data_save_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_save_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_save_xml with with afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_save_xml_test5)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 0, 3, 3);

    DBOOL result = afs_toc_data_save_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_save_xml with with afs_toc_data instance with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_data_save_xml_test6)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 0, 3);

    DBOOL result = afs_toc_data_save_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_save_xml with with afs_toc_data instance with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_save_xml_test7)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 0, 3);

    DBOOL result = afs_toc_data_save_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_save_xml with not valid afs_toc_data instance
BOXING_START_TEST(afs_toc_data_save_xml_test8)
{
    mxml_node_t *tree = mxmlNewXML("1.0");
    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_save_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_save_xml with valid afs_toc_data instance
BOXING_START_TEST(afs_toc_data_save_xml_test9)
{
    mxml_node_t *tree = mxmlNewXML("1.0");

    afs_toc_data * toc_data = get_afs_toc_data_instance8("Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    DBOOL result = afs_toc_data_save_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_load_file with file_name pointer equal to NULL and afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_load_file_test1)
{
    const char * file_name = NULL;
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_load_file(toc_data, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_load_file with file_name pointer equal to NULL and afs_toc_data pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_load_file_test2)
{
    const char * file_name = NULL;

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_file(toc_data, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(toc_data != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_load_file with file_name pointer not equal to NULL and afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_load_file_test3)
{
    const char * file_name = "afs_toc_data_save_file_compact_test.xml";
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_load_file(toc_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_load_file when XML file does not exist
BOXING_START_TEST(afs_toc_data_load_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_file(toc_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_load_file with initialized afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_load_file_test5)
{
    const char * file_name = "afs_toc_data_empty_reels_save_file_test.xml";

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_file(toc_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", -1, 3, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_load_file with initialized afs_toc_data instance with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_data_load_file_test6)
{
    const char * file_name = "afs_toc_data_empty_metadata_save_file_test.xml";

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_file(toc_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, -1, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_load_file with initialized afs_toc_data instance with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_load_file_test7)
{
    const char * file_name = "afs_toc_data_empty_definitions_save_file_test.xml";

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_file(toc_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, 3, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_load_file with initialized afs_toc_data instance, in compact format
BOXING_START_TEST(afs_toc_data_load_file_test8)
{
    const char * file_name = "afs_toc_data_save_file_compact_test.xml";

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_file(toc_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_load_file with initialized afs_toc_data instance, in not compact format
BOXING_START_TEST(afs_toc_data_load_file_test9)
{
    const char * file_name = "afs_toc_data_save_file_not_compact_test.xml";
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_file(toc_data, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_load_string with input string pointer equal to NULL and afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_load_string_test1)
{
    const char * input_string = NULL;
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_load_string(toc_data, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_load_string with input string pointer equal to NULL and afs_toc_data pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_load_string_test2)
{
    const char * input_string = NULL;

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_string(toc_data, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_load_string with input string pointer not equal to NULL and afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_load_string_test3)
{
    char * input_string = read_xml_toc_data("afs_toc_data_save_file_compact_test.xml");
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_load_string(toc_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);

    boxing_string_free(input_string);
}
END_TEST


// Test function afs_toc_data_load_string with empty input string
BOXING_START_TEST(afs_toc_data_load_string_test4)
{
    char * input_string = "";
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_string(toc_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_load_string with input string for initialized afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_load_string_test5)
{
    char * input_string = read_xml_toc_data("afs_toc_data_empty_reels_save_file_test.xml");

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_string(toc_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", -1, 3, 3);

    afs_toc_data_free(toc_data);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_toc_data_load_string with input string for initialized afs_toc_data instance with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_data_load_string_test6)
{
    char * input_string = read_xml_toc_data("afs_toc_data_empty_metadata_save_file_test.xml");

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_string(toc_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, -1, 3);

    afs_toc_data_free(toc_data);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_toc_data_load_string with input string for initialized afs_toc_data instance with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_load_string_test7)
{
    char * input_string = read_xml_toc_data("afs_toc_data_empty_definitions_save_file_test.xml");

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_string(toc_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, 3, -1);

    afs_toc_data_free(toc_data);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_toc_data_load_string with input string for initialized afs_toc_data instance, in compact format
BOXING_START_TEST(afs_toc_data_load_string_test8)
{
    char * input_string = read_xml_toc_data("afs_toc_data_save_file_compact_test.xml");

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_string(toc_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_toc_data_load_string with input string for initialized afs_toc_data instance, in not compact format
BOXING_START_TEST(afs_toc_data_load_string_test9)
{
    char * input_string = read_xml_toc_data("afs_toc_data_save_file_not_compact_test.xml");

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_string(toc_data, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    boxing_string_free(input_string);
}
END_TEST


// Test function afs_toc_data_load_xml with xml instance pointer equal to NULL and afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_load_xml_test1)
{
    mxml_node_t *tree = NULL;
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_load_xml(toc_data, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_toc_data_load_xml with xml instance pointer equal to NULL and afs_toc_data pointer not equal to NULL
BOXING_START_TEST(afs_toc_data_load_xml_test2)
{
    mxml_node_t * tree = NULL;

    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_xml(toc_data, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data_free(toc_data);
}
END_TEST


// Test function afs_toc_data_load_xml with xml instance pointer not equal to NULL and afs_toc_data pointer equal to NULL
BOXING_START_TEST(afs_toc_data_load_xml_test3)
{
    mxml_node_t * tree = get_toc_data_xml_instance("afs_toc_data_save_file_compact_test.xml");
    afs_toc_data * toc_data = NULL;

    DBOOL result = afs_toc_data_load_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(toc_data == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_load_xml with empty xml instance
BOXING_START_TEST(afs_toc_data_load_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DFALSE);

    test_afs_toc_data(toc_data, NULL, NULL, NULL, NULL, -1, -1, -1);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_load_xml with xml instance for initialized afs_toc_data instance with empty afs_toc_data_reels instance
BOXING_START_TEST(afs_toc_data_load_xml_test5)
{
    mxml_node_t * tree = get_toc_data_xml_instance("afs_toc_data_empty_reels_save_file_test.xml");
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", -1, 3, 3);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_load_xml with xml instance for initialized afs_toc_data instance with empty afs_toc_metadata instance
BOXING_START_TEST(afs_toc_data_load_xml_test6)
{
    mxml_node_t * tree = get_toc_data_xml_instance("afs_toc_data_empty_metadata_save_file_test.xml");
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, -1, 3);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_load_xml with xml instance for initialized afs_toc_data instance with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_toc_data_load_xml_test7)
{
    mxml_node_t * tree = get_toc_data_xml_instance("afs_toc_data_empty_definitions_save_file_test.xml");
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, 3, -1);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_load_xml with xml instance for initialized afs_toc_data instance, in compact format
BOXING_START_TEST(afs_toc_data_load_xml_test8)
{
    mxml_node_t * tree = get_toc_data_xml_instance("afs_toc_data_save_file_compact_test.xml");
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_toc_data_load_xml with xml instance for initialized afs_toc_data instance, in not compact format
BOXING_START_TEST(afs_toc_data_load_xml_test9)
{
    mxml_node_t * tree = get_toc_data_xml_instance("afs_toc_data_save_file_not_compact_test.xml");
    afs_toc_data * toc_data = afs_toc_data_create();

    DBOOL result = afs_toc_data_load_xml(toc_data, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_afs_toc_data(toc_data, "Some version", "job", "Some job id", "Some reel id", 3, 3, 3);

    afs_toc_data_free(toc_data);
    mxmlDelete(tree);
}
END_TEST


Suite * tocdata_tests(void)
{
    TCase * tc_tocdata_functions_tests = tcase_create("tocdata_functions_tests");

    // Test function afs_toc_data_reels_create
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_create_test1);
    // Test function afs_toc_data_reels_create2
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_create2_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_create2_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_create2_test3);
    // Test function afs_toc_data_clone
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_clone_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_clone_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_clone_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_clone_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_clone_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_clone_test6);
    // Test function afs_toc_data_get_new_reference
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_new_reference_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_new_reference_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_new_reference_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_new_reference_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_new_reference_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_new_reference_test6);
    // Test function afs_toc_data_equal
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test10);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test11);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test12);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test13);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test14);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test15);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test16);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test17);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test18);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test19);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_equal_test20);
    // Test function afs_toc_data_get_reel_by_index
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_index_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_index_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_index_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_index_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_index_test5);
    // Test function afs_toc_data_get_reel_by_id
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_id_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_id_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_id_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_id_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_id_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_id_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_id_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_id_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_id_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_reel_by_id_test10);
    // Test function afs_toc_data_get_metadata_source
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_metadata_source_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_metadata_source_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_metadata_source_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_metadata_source_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_metadata_source_test5);
    // Test function afs_toc_data_get_file_by_id
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_id_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_id_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_id_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_id_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_id_test5);
    // Test function afs_toc_data_get_file_by_unique_id
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_unique_id_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_unique_id_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_unique_id_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_unique_id_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_unique_id_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_unique_id_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_unique_id_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_unique_id_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_unique_id_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_file_by_unique_id_test10);
    // Test function afs_toc_data_get_preview_layout_definition
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_preview_layout_definition_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_preview_layout_definition_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_preview_layout_definition_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_preview_layout_definition_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_preview_layout_definition_test5);
    // Test function afs_toc_data_get_used_preview_layout_definitions
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test10);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test11);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test12);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test13);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_get_used_preview_layout_definitions_test14);
    // Test function afs_toc_data_set_version
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_version_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_version_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_version_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_version_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_version_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_version_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_version_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_version_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_version_test9);
    // Test function afs_toc_data_set_index_type
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_index_type_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_index_type_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_index_type_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_index_type_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_index_type_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_index_type_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_index_type_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_index_type_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_index_type_test9);
    // Test function afs_toc_data_set_job_id
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_job_id_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_job_id_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_job_id_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_job_id_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_job_id_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_job_id_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_job_id_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_job_id_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_job_id_test9);
    // Test function afs_toc_data_set_reel_id
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_reel_id_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_reel_id_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_reel_id_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_reel_id_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_reel_id_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_reel_id_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_reel_id_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_reel_id_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_set_reel_id_test9);
    // Test function afs_toc_data_add_reel
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test10);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test11);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test12);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test13);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test14);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_reel_test15);
    // Test function afs_toc_data_add_metadata_source
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test10);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test11);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test12);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test13);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test14);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_metadata_source_test15);
    // Test function afs_toc_data_add_preview_layout_definition
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test10);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test11);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test12);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test13);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test14);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definition_test15);
    // Test function afs_toc_data_add_preview_layout_definitions
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test10);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test11);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test12);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test13);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test14);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test15);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test16);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test17);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test18);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test19);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test20);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test21);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test22);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test23);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_preview_layout_definitions_test24);
    // Test function afs_toc_data_reel_count
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_reel_count_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_reel_count_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_reel_count_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_reel_count_test4);
    // Test function afs_toc_data_metadata_source_count
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_metadata_source_count_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_metadata_source_count_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_metadata_source_count_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_metadata_source_count_test4);
    // Test function afs_toc_data_preview_layout_definition_count
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_preview_layout_definition_count_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_preview_layout_definition_count_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_preview_layout_definition_count_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_preview_layout_definition_count_test4);
    // Test function afs_toc_data_file_count
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_file_count_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_file_count_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_file_count_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_file_count_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_file_count_test5);
    // Test function afs_toc_data_frame_count
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_frame_count_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_frame_count_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_frame_count_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_frame_count_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_frame_count_test5);
    // Test function afs_toc_data_first_frame
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_first_frame_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_first_frame_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_first_frame_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_first_frame_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_first_frame_test5);
    // Test function afs_toc_data_last_frame
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_last_frame_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_last_frame_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_last_frame_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_last_frame_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_last_frame_test5);
    // Test function afs_toc_data_is_valid
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test10);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test11);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test12);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test13);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test14);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test15);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test16);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test17);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test18);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test19);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test20);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test21);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_is_valid_test22);
    // Test function afs_toc_data_add_frame_offset
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test10);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test11);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test12);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test13);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test14);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test15);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_add_frame_offset_test16);
    // Test function afs_toc_data_save_file
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_file_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_file_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_file_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_file_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_file_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_file_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_file_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_file_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_file_test9);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_file_test10);
    // Test function afs_toc_data_save_string
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_string_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_string_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_string_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_string_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_string_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_string_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_string_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_string_test8);
    // Test function afs_toc_data_save_xml
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_xml_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_xml_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_xml_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_xml_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_xml_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_xml_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_xml_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_xml_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_save_xml_test9);
    // Test function afs_toc_data_load_file
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_file_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_file_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_file_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_file_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_file_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_file_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_file_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_file_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_file_test9);
    // Test function afs_toc_data_load_string
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_string_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_string_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_string_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_string_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_string_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_string_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_string_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_string_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_string_test9);
    // Test function afs_toc_data_load_xml
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_xml_test1);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_xml_test2);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_xml_test3);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_xml_test4);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_xml_test5);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_xml_test6);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_xml_test7);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_xml_test8);
    tcase_add_test(tc_tocdata_functions_tests, afs_toc_data_load_xml_test9);

    Suite * s = suite_create("tocdata_test_util");
    suite_add_tcase(s, tc_tocdata_functions_tests);

    return s;
}
