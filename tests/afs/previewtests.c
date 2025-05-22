#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "toc/previewimageutils.h"
#include "boxing/utils.h"
#include <limits.h>

/*Sections functions tests*/
BOXING_START_TEST(boxing_preview_sections_create_test0)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();

    BOXING_ASSERT(NULL != sections);
    
    afs_toc_preview_sections_free(sections);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_create_test1)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();

    BOXING_ASSERT(NULL != sections && sections->sections == NULL);
    
    afs_toc_preview_sections_free(sections);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_free_test0)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
    
    afs_toc_preview_sections_free(sections);

    /*Don't know how to check correctly*/
    BOXING_ASSERT(DTRUE);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_size_test0)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
    
    const unsigned int sections_size = afs_toc_preview_sections_size(sections);

    BOXING_ASSERT(0 == sections_size);
    
    afs_toc_preview_sections_free(sections);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_size_test1)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
    afs_toc_preview_section * section = afs_toc_preview_section_create2(5, 100, 50, 400, 300, 0);
    
    afs_toc_preview_sections_append(sections, section);
    
    const unsigned int sections_size = afs_toc_preview_sections_size(sections);

    BOXING_ASSERT(1 == sections_size);
    
    afs_toc_preview_sections_free(sections);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_size_test2)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();

    afs_toc_preview_section * sections_array[] = {
        afs_toc_preview_section_create2(0, 100, 50, 400, 300, 0),
        afs_toc_preview_section_create2(1, 550, 50, 400, 300, 0),
        afs_toc_preview_section_create2(2, 1000, 50, 400, 300, 0),
        afs_toc_preview_section_create2(3, 100, 400, 400, 300, 0),
        afs_toc_preview_section_create2(4, 550, 400, 400, 300, 0),
        afs_toc_preview_section_create2(5, 1000, 400, 400, 300, 0)
    };

    afs_toc_preview_sections_append(sections, sections_array[0]);
    afs_toc_preview_sections_append(sections, sections_array[1]);
    afs_toc_preview_sections_append(sections, sections_array[2]);
    afs_toc_preview_sections_append(sections, sections_array[3]);
    afs_toc_preview_sections_append(sections, sections_array[4]);
    afs_toc_preview_sections_append(sections, sections_array[5]);

    unsigned int sections_size = afs_toc_preview_sections_size(sections);

    BOXING_ASSERT(6 == sections_size);
    
    afs_toc_preview_sections_free(sections);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_append_test0)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
   
    afs_toc_preview_sections_append(sections, NULL);
    
    const unsigned int sections_size = afs_toc_preview_sections_size(sections);

    BOXING_ASSERT(0 == sections_size);
    
    afs_toc_preview_sections_free(sections);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_append_test1)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
    
    afs_toc_preview_section * section = afs_toc_preview_section_create2(5, 100, 50, 400, 300, 0);
    
    const unsigned int appended = afs_toc_preview_sections_append(sections, section);
    
    const unsigned int sections_size = afs_toc_preview_sections_size(sections);

    BOXING_ASSERT(0 != appended && 1 == sections_size);
    
    afs_toc_preview_sections_free(sections);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_get_test0)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
    
    afs_toc_preview_section * section = afs_toc_preview_section_create2(5, 100, 50, 400, 300, 0);
    
    afs_toc_preview_sections_append(sections, section);
    
    afs_toc_preview_section * returned_section = afs_toc_preview_sections_get(sections, 0);

    BOXING_ASSERT(afs_toc_preview_section_equal(section, returned_section));
    
    afs_toc_preview_sections_free(sections);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_get_test1)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
   
    afs_toc_preview_section * returned_section = afs_toc_preview_sections_get(sections, 0);

    BOXING_ASSERT(NULL == returned_section);
    
    afs_toc_preview_sections_free(sections);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_get_test2)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
    
    afs_toc_preview_section * section = afs_toc_preview_section_create2(5, 100, 50, 400, 300, 0);
    
    afs_toc_preview_sections_append(sections, section);
    
    afs_toc_preview_section * returned_section = afs_toc_preview_sections_get(sections, 1);

    BOXING_ASSERT(NULL == returned_section);
    
    afs_toc_preview_sections_free(sections);
}
END_TEST

BOXING_START_TEST(boxing_preview_sections_get_test3)
{
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
    
    afs_toc_preview_section * section = afs_toc_preview_section_create2(5, 100, 50, 400, 300, 0);
    
    afs_toc_preview_sections_append(sections, section);
    
    afs_toc_preview_section * returned_section = afs_toc_preview_sections_get(sections, UINT_MAX);

    BOXING_ASSERT(NULL == returned_section);
    
    afs_toc_preview_sections_free(sections);
}
END_TEST


/*Preview image utils*/
BOXING_START_TEST(boxing_preview_image_util_extract_test0)
{
    boxing_image8** result_images = NULL;
    
    afs_toc_preview_layout_definition * layout_definition = afs_toc_preview_layout_definition_create2("someId", "someName");
    
    boxing_image8 * source_image = boxing_image8_create(12288, 6878);
    
    const int extracted_images = toc_preview_image_util_extract(&result_images, layout_definition, source_image);
    
    BOXING_ASSERT(0 == extracted_images);
    
    afs_toc_preview_layout_definition_free(layout_definition);
    boxing_image8_free(source_image);
}
END_TEST

BOXING_START_TEST(boxing_preview_image_util_extract_test1)
{
    boxing_image8** result_images = NULL;
    
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
    afs_toc_preview_sections_append(sections, afs_toc_preview_section_create2(5, 100, 50, 400, 300, 0));
    afs_toc_preview_sections_append(sections, afs_toc_preview_section_create2(6, 550, 50, 400, 300, 0));
    afs_toc_preview_layout_definition * layout_definition = afs_toc_preview_layout_definition_create3("someId", "someName", sections);
    
    boxing_image8 * source_image = boxing_image8_create(100, 100);
    
    const int extracted_images = toc_preview_image_util_extract(&result_images, layout_definition, source_image);
    
    BOXING_ASSERT(0 == extracted_images);
    
    afs_toc_preview_layout_definition_free(layout_definition);
    boxing_image8_free(source_image);
    free(result_images);
}
END_TEST

BOXING_START_TEST(boxing_preview_image_util_extract_test2)
{
    boxing_image8** result_images = NULL;
    
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
    afs_toc_preview_sections_append(sections, afs_toc_preview_section_create2(0, 50, 50, 400, 300, 0));
    afs_toc_preview_sections_append(sections, afs_toc_preview_section_create2(1, 500, 50, 400, 300, 0));
    afs_toc_preview_layout_definition * layout_definition = afs_toc_preview_layout_definition_create3("someId", "someName", sections);
    
    boxing_image8 * source_image = boxing_image8_create(600, 500);
    
    const int extracted_images = toc_preview_image_util_extract(&result_images, layout_definition, source_image);
    
    BOXING_ASSERT(1 == extracted_images && NULL != result_images);
    
    afs_toc_preview_layout_definition_free(layout_definition);
    boxing_image8_free(source_image);
    boxing_image8_free(*result_images);
    free(result_images);
}
END_TEST

BOXING_START_TEST(boxing_preview_image_util_extract_test3)
{
    boxing_image8** result_images = NULL;
    
    afs_toc_preview_sections * sections = afs_toc_preview_sections_create();
    afs_toc_preview_sections_append(sections, afs_toc_preview_section_create2(5, 100, 50, 400, 300, 0));
    afs_toc_preview_sections_append(sections, afs_toc_preview_section_create2(6, 550, 50, 400, 300, 0));
    afs_toc_preview_sections_append(sections, afs_toc_preview_section_create2(7, 100, 400, 400, 300, 0));
    afs_toc_preview_layout_definition * layout_definition = afs_toc_preview_layout_definition_create3("someId", "someName", sections);
    
    boxing_image8 * source_image = boxing_image8_create(12288, 6878);
    
    const int extracted_images = toc_preview_image_util_extract(&result_images, layout_definition, source_image);
    
    BOXING_ASSERT(3 == extracted_images && NULL != result_images);
    
    afs_toc_preview_layout_definition_free(layout_definition);
    boxing_image8_free(source_image);
    
    for (int i = 0; i < extracted_images; i++)
    {
        boxing_image8_free(result_images[i]);
    }
    free(result_images);
}
END_TEST


Suite * preview_tests(void)
{
    TCase * tc_preview_sections_functions_tests = tcase_create("preview_sections_functions_tests");
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_create_test0);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_create_test1);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_free_test0);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_size_test0);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_size_test1);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_size_test2);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_append_test0);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_append_test1);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_get_test0);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_get_test1);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_get_test2);
    tcase_add_test(tc_preview_sections_functions_tests, boxing_preview_sections_get_test3);
    
    TCase * tc_preview_image_utils_tests = tcase_create("tc_preview_image_utils_tests");
    tcase_add_test(tc_preview_image_utils_tests, boxing_preview_image_util_extract_test0);
    tcase_add_test(tc_preview_image_utils_tests, boxing_preview_image_util_extract_test1);
    tcase_add_test(tc_preview_image_utils_tests, boxing_preview_image_util_extract_test2);
    tcase_add_test(tc_preview_image_utils_tests, boxing_preview_image_util_extract_test3);

    Suite * s = suite_create("previews_test_util");
    suite_add_tcase(s, tc_preview_sections_functions_tests);
    suite_add_tcase(s, tc_preview_image_utils_tests);

    return s;
}
