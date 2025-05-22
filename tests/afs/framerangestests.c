#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "frameranges.h"
#include "boxing/utils.h"


static void test_afs_frame_range(afs_frame_range * frame_range, afs_frame_range_type start, afs_frame_range_type count)
{
    BOXING_ASSERT(frame_range != NULL);
    BOXING_ASSERT(frame_range->start == start);
    BOXING_ASSERT(frame_range->count == count);
}


static void test_afs_frame_ranges(afs_frame_ranges * frame_ranges, unsigned int frame_ranges_count)
{
    BOXING_ASSERT(frame_ranges != NULL);

    if (frame_ranges_count == 0)
    {
        BOXING_ASSERT(frame_ranges->frame_ranges == NULL);
        return;
    }

    BOXING_ASSERT(frame_ranges->frame_ranges != NULL);

    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == frame_ranges_count);

    for (unsigned int i = 0; i < frame_ranges_count; i++)
    {
        afs_frame_range * frame_range = afs_frame_ranges_get_frame_range(frame_ranges, i);

        test_afs_frame_range(frame_range, i, i + 3);
    }
}


afs_frame_ranges * get_afs_frame_ranges_instance(unsigned int frame_ranges_count)
{
    afs_frame_ranges * frame_ranges = afs_frame_ranges_create();

    for (unsigned int i = 0; i < frame_ranges_count; i++)
    {
        afs_frame_range * frame_range = afs_frame_range_create2(i, i + 3);
        afs_frame_ranges_add_frame_range(frame_ranges, frame_range);
    }

    return frame_ranges;
}


static void test_equals_frame_ranges(afs_frame_ranges * frame_ranges1, afs_frame_ranges * frame_ranges2, DBOOL expected_result)
{
    DBOOL result = afs_frame_ranges_equal(frame_ranges2, frame_ranges1);
    BOXING_ASSERT(result == expected_result);
    result = afs_frame_ranges_equal(frame_ranges1, frame_ranges2);
    BOXING_ASSERT(result == expected_result);
}


// TEST FRAME RANGES FUNCTIONS
//

// Test function afs_frame_ranges_create
BOXING_START_TEST(afs_frame_ranges_create_test1)
{
    afs_frame_ranges * frame_ranges = afs_frame_ranges_create();

    test_afs_frame_ranges(frame_ranges, 0);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_clone with input pointer equal to NULL
BOXING_START_TEST(afs_frame_ranges_clone_test1)
{
    afs_frame_ranges * frame_ranges1 = NULL;

    BOXING_ASSERT(frame_ranges1 == NULL);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_clone(frame_ranges1);

    BOXING_ASSERT(frame_ranges2 == NULL);
}
END_TEST


// Test function afs_frame_ranges_clone with empty input afs_frame_ranges instance
BOXING_START_TEST(afs_frame_ranges_clone_test2)
{
    afs_frame_ranges * frame_ranges1 = afs_frame_ranges_create();

    test_afs_frame_ranges(frame_ranges1, 0);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_clone(frame_ranges1);

    test_afs_frame_ranges(frame_ranges2, 0);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges2);
}
END_TEST


// Test function afs_frame_ranges_clone with not empty input afs_frame_ranges instance with one frame range
BOXING_START_TEST(afs_frame_ranges_clone_test3)
{
    afs_frame_ranges * frame_ranges1 = get_afs_frame_ranges_instance(1);

    test_afs_frame_ranges(frame_ranges1, 1);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_clone(frame_ranges1);

    test_afs_frame_ranges(frame_ranges2, 1);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges2);
}
END_TEST


// Test function afs_frame_ranges_clone with not empty input afs_frame_ranges instance with three frame ranges
BOXING_START_TEST(afs_frame_ranges_clone_test4)
{
    afs_frame_ranges * frame_ranges1 = get_afs_frame_ranges_instance(3);

    test_afs_frame_ranges(frame_ranges1, 3);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_clone(frame_ranges1);

    test_afs_frame_ranges(frame_ranges2, 3);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges2);
}
END_TEST


// Test function afs_frame_ranges_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_frame_ranges_get_new_reference_test1)
{
    afs_frame_ranges * frame_ranges1 = NULL;

    BOXING_ASSERT(frame_ranges1 == NULL);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_get_new_reference(frame_ranges1);

    BOXING_ASSERT(frame_ranges2 == NULL);

    frame_ranges1 = afs_frame_ranges_create();

    test_afs_frame_ranges(frame_ranges1, 0);
    BOXING_ASSERT(frame_ranges1->reference_count == 1);
    BOXING_ASSERT(frame_ranges2 == NULL);

    afs_frame_ranges_free(frame_ranges1);
}
END_TEST


// Test function afs_frame_ranges_get_new_reference with empty input afs_frame_ranges instance
BOXING_START_TEST(afs_frame_ranges_get_new_reference_test2)
{
    afs_frame_ranges * frame_ranges1 = afs_frame_ranges_create();

    test_afs_frame_ranges(frame_ranges1, 0);
    BOXING_ASSERT(frame_ranges1->reference_count == 1);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_get_new_reference(frame_ranges1);

    test_afs_frame_ranges(frame_ranges2, 0);
    BOXING_ASSERT(frame_ranges1->reference_count == 2);
    BOXING_ASSERT(frame_ranges2->reference_count == 2);
    BOXING_ASSERT(frame_ranges1 == frame_ranges2);

    afs_frame_range * frame_range = afs_frame_range_create2(0, 3);
    afs_frame_ranges_add_frame_range(frame_ranges1, frame_range);

    test_afs_frame_ranges(frame_ranges2, 1);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges1);
}
END_TEST


// Test function afs_frame_ranges_get_new_reference with not empty input afs_frame_ranges instance with one frame range
BOXING_START_TEST(afs_frame_ranges_get_new_reference_test3)
{
    afs_frame_ranges * frame_ranges1 = get_afs_frame_ranges_instance(1);

    test_afs_frame_ranges(frame_ranges1, 1);
    BOXING_ASSERT(frame_ranges1->reference_count == 1);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_get_new_reference(frame_ranges1);

    test_afs_frame_ranges(frame_ranges2, 1);
    BOXING_ASSERT(frame_ranges1->reference_count == 2);
    BOXING_ASSERT(frame_ranges2->reference_count == 2);
    BOXING_ASSERT(frame_ranges1 == frame_ranges2);

    afs_frame_range * frame_range = afs_frame_range_create2(1, 4);
    afs_frame_ranges_add_frame_range(frame_ranges1, frame_range);

    test_afs_frame_ranges(frame_ranges2, 2);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges1);
}
END_TEST


// Test function afs_frame_ranges_get_new_reference with not empty input afs_frame_ranges instance with three frame ranges
BOXING_START_TEST(afs_frame_ranges_get_new_reference_test4)
{
    afs_frame_ranges * frame_ranges1 = get_afs_frame_ranges_instance(3);

    test_afs_frame_ranges(frame_ranges1, 3);
    BOXING_ASSERT(frame_ranges1->reference_count == 1);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_get_new_reference(frame_ranges1);

    test_afs_frame_ranges(frame_ranges2, 3);
    BOXING_ASSERT(frame_ranges1->reference_count == 2);
    BOXING_ASSERT(frame_ranges2->reference_count == 2);
    BOXING_ASSERT(frame_ranges1 == frame_ranges2);

    afs_frame_range * frame_range = afs_frame_range_create2(3, 6);
    afs_frame_ranges_add_frame_range(frame_ranges1, frame_range);

    test_afs_frame_ranges(frame_ranges2, 4);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges1);
}
END_TEST


// Test function afs_frame_ranges_equal with input afs_frame_ranges pointers equal to NULL
BOXING_START_TEST(afs_frame_ranges_equal_test1)
{
    afs_frame_ranges * frame_ranges1 = NULL;
    afs_frame_ranges * frame_ranges2 = NULL;

    test_equals_frame_ranges(frame_ranges1, frame_ranges2, DTRUE);
}
END_TEST

// Test function afs_frame_ranges_equal with empty input afs_frame_ranges instances 
BOXING_START_TEST(afs_frame_ranges_equal_test2)
{
    afs_frame_ranges * frame_ranges1 = afs_frame_ranges_create();
    test_afs_frame_ranges(frame_ranges1, 0);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_create();
    test_afs_frame_ranges(frame_ranges2, 0);

    test_equals_frame_ranges(frame_ranges1, frame_ranges2, DTRUE);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges2);
}
END_TEST


// Test function afs_frame_ranges_equal with not empty input afs_frame_ranges instances with one frame range
BOXING_START_TEST(afs_frame_ranges_equal_test3)
{
    afs_frame_ranges * frame_ranges1 = get_afs_frame_ranges_instance(1);
    test_afs_frame_ranges(frame_ranges1, 1);

    afs_frame_ranges * frame_ranges2 = get_afs_frame_ranges_instance(1);
    test_afs_frame_ranges(frame_ranges2, 1);

    test_equals_frame_ranges(frame_ranges1, frame_ranges2, DTRUE);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges2);
}
END_TEST


// Test function afs_frame_ranges_equal with not empty input afs_frame_ranges instances with three frame ranges
BOXING_START_TEST(afs_frame_ranges_equal_test4)
{
    afs_frame_ranges * frame_ranges1 = get_afs_frame_ranges_instance(3);
    test_afs_frame_ranges(frame_ranges1, 3);

    afs_frame_ranges * frame_ranges2 = get_afs_frame_ranges_instance(3);
    test_afs_frame_ranges(frame_ranges2, 3);

    test_equals_frame_ranges(frame_ranges1, frame_ranges2, DTRUE);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges2);
}
END_TEST


// Test function afs_frame_ranges_equal with empty input afs_frame_ranges instance and pointer equal to NULL 
BOXING_START_TEST(afs_frame_ranges_equal_test5)
{
    afs_frame_ranges * frame_ranges1 = afs_frame_ranges_create();
    test_afs_frame_ranges(frame_ranges1, 0);

    afs_frame_ranges * frame_ranges2 = NULL;

    test_equals_frame_ranges(frame_ranges1, frame_ranges2, DFALSE);

    afs_frame_ranges_free(frame_ranges1);
}
END_TEST


// Test function afs_frame_ranges_equal with input afs_frame_ranges pointer equal to NULL and instance with filled frame ranges vector 
BOXING_START_TEST(afs_frame_ranges_equal_test6)
{
    afs_frame_ranges * frame_ranges1 = get_afs_frame_ranges_instance(3);
    test_afs_frame_ranges(frame_ranges1, 3);

    afs_frame_ranges * frame_ranges2 = NULL;

    test_equals_frame_ranges(frame_ranges1, frame_ranges2, DFALSE);

    afs_frame_ranges_free(frame_ranges1);
}
END_TEST


// Test function afs_frame_ranges_equal with empty input afs_frame_ranges instance and instance with filled frame ranges vector 
BOXING_START_TEST(afs_frame_ranges_equal_test7)
{
    afs_frame_ranges * frame_ranges1 = get_afs_frame_ranges_instance(3);
    test_afs_frame_ranges(frame_ranges1, 3);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_create();
    test_afs_frame_ranges(frame_ranges2, 0);

    test_equals_frame_ranges(frame_ranges1, frame_ranges2, DFALSE);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges2);
}
END_TEST


// Test function afs_frame_ranges_equal with input afs_frame_ranges instances with different frame ranges vector size 
BOXING_START_TEST(afs_frame_ranges_equal_test8)
{
    afs_frame_ranges * frame_ranges1 = get_afs_frame_ranges_instance(3);
    test_afs_frame_ranges(frame_ranges1, 3);

    afs_frame_ranges * frame_ranges2 = get_afs_frame_ranges_instance(4);
    test_afs_frame_ranges(frame_ranges2, 4);

    test_equals_frame_ranges(frame_ranges1, frame_ranges2, DFALSE);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges2);
}
END_TEST


// Test function afs_frame_ranges_equal with input afs_frame_ranges instances with different frame range data 
BOXING_START_TEST(afs_frame_ranges_equal_test9)
{
    afs_frame_ranges * frame_ranges1 = afs_frame_ranges_create();
    afs_frame_range * frame_range1 = afs_frame_range_create2(1, 2);
    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges1, frame_range1);

    BOXING_ASSERT(result == DTRUE);

    afs_frame_ranges * frame_ranges2 = afs_frame_ranges_create();
    afs_frame_range * frame_range2 = afs_frame_range_create2(3, 4);
    result = afs_frame_ranges_add_frame_range(frame_ranges2, frame_range2);

    BOXING_ASSERT(result == DTRUE);

    test_equals_frame_ranges(frame_ranges1, frame_ranges2, DFALSE);

    afs_frame_ranges_free(frame_ranges1);
    afs_frame_ranges_free(frame_ranges2);
}
END_TEST


// Test function afs_frame_ranges_get_count with input afs_frame_ranges pointer equal to NULL
BOXING_START_TEST(afs_frame_ranges_get_count_test1)
{
    afs_frame_ranges * frame_ranges = NULL;
    unsigned int frame_ranges_count = afs_frame_ranges_get_count(frame_ranges);

    BOXING_ASSERT(frame_ranges == NULL);
    BOXING_ASSERT(frame_ranges_count == 0);
}
END_TEST


// Test function afs_frame_ranges_get_count with empty afs_frame_ranges instance
BOXING_START_TEST(afs_frame_ranges_get_count_test2)
{
    afs_frame_ranges * frame_ranges = afs_frame_ranges_create();
    unsigned int frame_ranges_count = afs_frame_ranges_get_count(frame_ranges);

    test_afs_frame_ranges(frame_ranges, 0);
    BOXING_ASSERT(frame_ranges_count == 0);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_get_count with afs_frame_ranges instance with filled frame ranges vector
BOXING_START_TEST(afs_frame_ranges_get_count_test3)
{
    afs_frame_ranges * frame_ranges = get_afs_frame_ranges_instance(3);

    unsigned int frame_ranges_count = afs_frame_ranges_get_count(frame_ranges);

    test_afs_frame_ranges(frame_ranges, 3);
    BOXING_ASSERT(frame_ranges_count == 3);

    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, afs_frame_range_create2(3, 6));

    BOXING_ASSERT(result == DTRUE);
    test_afs_frame_ranges(frame_ranges, 4);

    frame_ranges_count = afs_frame_ranges_get_count(frame_ranges);

    BOXING_ASSERT(frame_ranges_count == 4);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_get_frame_range with input afs_frame_ranges pointer equal to NULL
BOXING_START_TEST(afs_frame_ranges_get_frame_range_test1)
{
    afs_frame_ranges * frame_ranges = NULL;
    afs_frame_range * frame_range = afs_frame_ranges_get_frame_range(frame_ranges, 0);

    BOXING_ASSERT(frame_ranges == NULL);
    BOXING_ASSERT(frame_range == NULL);
}
END_TEST


// Test function afs_toc_preview_layout_definitions_get_layout_definition_by_index with empty afs_toc_preview_layout_definitions instance
BOXING_START_TEST(afs_frame_ranges_get_frame_range_test2)
{
    afs_frame_ranges * frame_ranges = afs_frame_ranges_create();
    afs_frame_range * frame_range = afs_frame_ranges_get_frame_range(frame_ranges, 0);

    test_afs_frame_ranges(frame_ranges, 0);
    BOXING_ASSERT(frame_range == NULL);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_get_frame_range with not empty afs_frame_ranges instance and index out of range
BOXING_START_TEST(afs_frame_ranges_get_frame_range_test3)
{
    afs_frame_ranges * frame_ranges = get_afs_frame_ranges_instance(2);

    afs_frame_range * frame_range = afs_frame_ranges_get_frame_range(frame_ranges, 2);

    test_afs_frame_ranges(frame_ranges, 2);
    BOXING_ASSERT(frame_range == NULL);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_get_frame_range with not empty afs_frame_ranges instance and index in range
BOXING_START_TEST(afs_frame_ranges_get_frame_range_test4)
{
    afs_frame_ranges * frame_ranges = get_afs_frame_ranges_instance(6);

    for (unsigned int i = 0; i < afs_frame_ranges_get_count(frame_ranges); i++)
    {
        afs_frame_range * frame_range = afs_frame_ranges_get_frame_range(frame_ranges, i);
        test_afs_frame_range(frame_range, i, i + 3);
    }

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_add_frame_range with input afs_frame_ranges pointers equal to NULL and afs_frame_range pointer equal to NULL
BOXING_START_TEST(afs_frame_ranges_add_frame_range_test1)
{
    afs_frame_ranges * frame_ranges = NULL;
    afs_frame_range * frame_range = NULL;

    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, frame_range);

    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_frame_ranges_add_frame_range with input afs_frame_ranges pointers equal to NULL and empty afs_frame_range instance
BOXING_START_TEST(afs_frame_ranges_add_frame_range_test2)
{
    afs_frame_ranges * frame_ranges = NULL;
    afs_frame_range * frame_range = afs_frame_range_create();

    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, frame_range);

    BOXING_ASSERT(result == DFALSE);

    afs_frame_range_free(frame_range);
}
END_TEST


// Test function afs_frame_ranges_add_frame_range with input afs_frame_ranges pointers equal to NULL and not empty afs_frame_range instance
BOXING_START_TEST(afs_frame_ranges_add_frame_range_test3)
{
    afs_frame_ranges * frame_ranges = NULL;
    afs_frame_range * frame_range = afs_frame_range_create2(3, 6);

    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, frame_range);

    BOXING_ASSERT(result == DFALSE);

    afs_frame_range_free(frame_range);
}
END_TEST


// Test function afs_frame_ranges_add_frame_range with empty input afs_frame_ranges instance and afs_frame_range pointer equal to NULL
BOXING_START_TEST(afs_frame_ranges_add_frame_range_test4)
{
    afs_frame_ranges * frame_ranges = afs_frame_ranges_create();
    afs_frame_range * frame_range = NULL;

    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, frame_range);

    BOXING_ASSERT(result == DFALSE);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_add_frame_range with empty input afs_frame_ranges instance and empty afs_frame_range instance
BOXING_START_TEST(afs_frame_ranges_add_frame_range_test5)
{
    afs_frame_ranges * frame_ranges = afs_frame_ranges_create();
    afs_frame_range * frame_range = afs_frame_range_create();

    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, frame_range);

    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == 1);

    test_afs_frame_range(afs_frame_ranges_get_frame_range(frame_ranges, 0), 0, 0);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_add_frame_range with empty input afs_frame_ranges instance and not empty afs_frame_range instance
BOXING_START_TEST(afs_frame_ranges_add_frame_range_test6)
{
    afs_frame_ranges * frame_ranges = afs_frame_ranges_create();
    afs_frame_range * frame_range = afs_frame_range_create2(4, 5);

    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, frame_range);

    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == 1);

    test_afs_frame_range(afs_frame_ranges_get_frame_range(frame_ranges, 0), 4, 5);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_add_frame_range with not empty input afs_frame_ranges instance and afs_frame_range pointers equal to NULL
BOXING_START_TEST(afs_frame_ranges_add_frame_range_test7)
{
    afs_frame_ranges * frame_ranges = get_afs_frame_ranges_instance(2);
    afs_frame_range * frame_range = NULL;

    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, frame_range);

    BOXING_ASSERT(result == DFALSE);

    test_afs_frame_ranges(frame_ranges, 2);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_add_frame_range with not empty input afs_frame_ranges instance and empty afs_frame_range instance
BOXING_START_TEST(afs_frame_ranges_add_frame_range_test8)
{
    afs_frame_ranges * frame_ranges = get_afs_frame_ranges_instance(2);
    afs_frame_range * frame_range = afs_frame_range_create();

    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, frame_range);

    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == 3);

    test_afs_frame_range(afs_frame_ranges_get_frame_range(frame_ranges, 2), 0, 0);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


// Test function afs_frame_ranges_add_frame_range with not empty input afs_frame_ranges instance and not empty afs_frame_range instance
BOXING_START_TEST(afs_frame_ranges_add_frame_range_test9)
{
    afs_frame_ranges * frame_ranges = get_afs_frame_ranges_instance(2);
    afs_frame_range * frame_range = afs_frame_range_create2(2, 5);

    DBOOL result = afs_frame_ranges_add_frame_range(frame_ranges, frame_range);

    BOXING_ASSERT(result == DTRUE);
    BOXING_ASSERT(afs_frame_ranges_get_count(frame_ranges) == 3);

    test_afs_frame_range(afs_frame_ranges_get_frame_range(frame_ranges, 2), 2, 5);

    afs_frame_ranges_free(frame_ranges);
}
END_TEST


Suite * frameranges_tests(void)
{
    TCase * tc_frameranges_functions_tests = tcase_create("frameranges_functions_tests");

    // Test function afs_frame_ranges_create
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_create_test1);
    // Test function afs_frame_ranges_clone
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_clone_test1);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_clone_test2);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_clone_test3);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_clone_test4);
    // Test function afs_frame_ranges_get_new_reference
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_new_reference_test1);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_new_reference_test2);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_new_reference_test3);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_new_reference_test4);
    // Test function afs_frame_ranges_equal
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_equal_test1);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_equal_test2);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_equal_test3);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_equal_test4);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_equal_test5);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_equal_test6);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_equal_test7);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_equal_test8);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_equal_test9);
    // Test function afs_frame_ranges_get_count
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_count_test1);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_count_test2);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_count_test3);
    // Test function afs_frame_ranges_get_frame_range
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_frame_range_test1);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_frame_range_test2);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_frame_range_test3);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_get_frame_range_test4);
    // Test function afs_frame_ranges_add_frame_range
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_add_frame_range_test1);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_add_frame_range_test2);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_add_frame_range_test3);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_add_frame_range_test4);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_add_frame_range_test5);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_add_frame_range_test6);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_add_frame_range_test7);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_add_frame_range_test8);
    tcase_add_test(tc_frameranges_functions_tests, afs_frame_ranges_add_frame_range_test9);

    Suite * s = suite_create("frameranges_test_util");
    suite_add_tcase(s, tc_frameranges_functions_tests);

    return s;
}
