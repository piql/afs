#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "framerange.h"
#include "boxing/utils.h"


static void test_afs_frame_range(afs_frame_range * frame_range, afs_frame_range_type start, afs_frame_range_type count)
{
    BOXING_ASSERT(frame_range != NULL);
    BOXING_ASSERT(frame_range->start == start);
    BOXING_ASSERT(frame_range->count == count);
}


static void test_equals_frame_range(afs_frame_range * frame_range1, afs_frame_range * frame_range2, DBOOL expected_result)
{
    DBOOL result = afs_frame_range_equal(frame_range2, frame_range1);
    BOXING_ASSERT(result == expected_result);
    result = afs_frame_range_equal(frame_range1, frame_range2);
    BOXING_ASSERT(result == expected_result);
}


// TEST FRAME RANGE FUNCTIONS
//

// Test function afs_frame_range_create test
BOXING_START_TEST(afs_frame_range_create_test1)
{
    afs_frame_range * frame_range = afs_frame_range_create();

    test_afs_frame_range(frame_range, 0, 0);

    afs_frame_range_free(frame_range);
}
END_TEST


// Test function afs_frame_range_create2 with zero initial values
BOXING_START_TEST(afs_frame_range_create2_test1)
{
    afs_frame_range * frame_range = afs_frame_range_create2(0, 0);

    test_afs_frame_range(frame_range, 0, 0);

    afs_frame_range_free(frame_range);
}
END_TEST


// Test function afs_frame_range_create2 with normal initial values
BOXING_START_TEST(afs_frame_range_create2_test2)
{
    afs_frame_range * frame_range = afs_frame_range_create2(9, 8);

    test_afs_frame_range(frame_range, 9, 8);

    afs_frame_range_free(frame_range);
}
END_TEST


// Test function afs_frame_range_clone with input pointer equal to NULL
BOXING_START_TEST(afs_frame_range_clone_test1)
{
    afs_frame_range * frame_range1 = NULL;

    BOXING_ASSERT(frame_range1 == NULL);

    afs_frame_range * frame_range2 = afs_frame_range_clone(frame_range1);

    BOXING_ASSERT(frame_range2 == NULL);
}
END_TEST


// Test function afs_frame_range_clone with empty input afs_frame_range instance
BOXING_START_TEST(afs_frame_range_clone_test2)
{
    afs_frame_range * frame_range1 = afs_frame_range_create();

    test_afs_frame_range(frame_range1, 0, 0);

    afs_frame_range * frame_range2 = afs_frame_range_clone(frame_range1);

    test_afs_frame_range(frame_range2, 0, 0);

    afs_frame_range_free(frame_range1);
    afs_frame_range_free(frame_range2);
}
END_TEST


// Test function afs_frame_range_clone with normal input afs_frame_range instance
BOXING_START_TEST(afs_frame_range_clone_test3)
{
    afs_frame_range * frame_range1 = afs_frame_range_create2(4, 3);

    test_afs_frame_range(frame_range1, 4, 3);

    afs_frame_range * frame_range2 = afs_frame_range_clone(frame_range1);

    test_afs_frame_range(frame_range2, 4, 3);

    afs_frame_range_free(frame_range1);
    afs_frame_range_free(frame_range2);
}
END_TEST


// Test function afs_frame_range_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_frame_range_get_new_reference_test1)
{
    afs_frame_range * frame_range1 = NULL;

    BOXING_ASSERT(frame_range1 == NULL);

    afs_frame_range * frame_range2 = afs_frame_range_get_new_reference(frame_range1);

    BOXING_ASSERT(frame_range2 == NULL);

    frame_range1 = afs_frame_range_create();

    test_afs_frame_range(frame_range1, 0, 0);
    BOXING_ASSERT(frame_range1->reference_count == 1);
    BOXING_ASSERT(frame_range2 == NULL);

    afs_frame_range_free(frame_range1);
}
END_TEST


// Test function afs_frame_range_get_new_reference with empty input afs_frame_range instance
BOXING_START_TEST(afs_frame_range_get_new_reference_test2)
{
    afs_frame_range * frame_range1 = afs_frame_range_create();

    test_afs_frame_range(frame_range1, 0, 0);
    BOXING_ASSERT(frame_range1->reference_count == 1);

    afs_frame_range * frame_range2 = afs_frame_range_get_new_reference(frame_range1);

    test_afs_frame_range(frame_range2, 0, 0);
    BOXING_ASSERT(frame_range1->reference_count == 2);
    BOXING_ASSERT(frame_range2->reference_count == 2);
    BOXING_ASSERT(frame_range1 == frame_range2);

    frame_range1->start = 99;

    BOXING_ASSERT(frame_range2->start == 99);

    afs_frame_range_free(frame_range1);
    afs_frame_range_free(frame_range1);
}
END_TEST


// Test function afs_frame_range_get_new_reference with normal input afs_frame_range instance
BOXING_START_TEST(afs_frame_range_get_new_reference_test3)
{
    afs_frame_range * frame_range1 = afs_frame_range_create2(4, 3);

    test_afs_frame_range(frame_range1, 4, 3);
    BOXING_ASSERT(frame_range1->reference_count == 1);

    afs_frame_range * frame_range2 = afs_frame_range_get_new_reference(frame_range1);

    test_afs_frame_range(frame_range2, 4, 3);
    BOXING_ASSERT(frame_range1->reference_count == 2);
    BOXING_ASSERT(frame_range2->reference_count == 2);
    BOXING_ASSERT(frame_range1 == frame_range2);

    frame_range1->start = 99;

    BOXING_ASSERT(frame_range2->start == 99);

    afs_frame_range_free(frame_range1);
    afs_frame_range_free(frame_range2);
}
END_TEST


// Test function afs_frame_range_equal with input afs_frame_range pointers equal to NULL
BOXING_START_TEST(afs_frame_range_equal_test1)
{
    afs_frame_range * frame_range1 = NULL;
    afs_frame_range * frame_range2 = NULL;

    test_equals_frame_range(frame_range1, frame_range2, DTRUE);
}
END_TEST

// Test function afs_frame_range_equal with empty input afs_frame_range instances 
BOXING_START_TEST(afs_frame_range_equal_test2)
{
    afs_frame_range * frame_range1 = afs_frame_range_create();
    test_afs_frame_range(frame_range1, 0, 0);

    afs_frame_range * frame_range2 = afs_frame_range_create();
    test_afs_frame_range(frame_range2, 0, 0);

    test_equals_frame_range(frame_range1, frame_range2, DTRUE);

    afs_frame_range_free(frame_range1);
    afs_frame_range_free(frame_range2);
}
END_TEST


// Test function afs_frame_range_equal with normal input afs_frame_range pointers 
BOXING_START_TEST(afs_frame_range_equal_test3)
{
    afs_frame_range * frame_range1 = afs_frame_range_create2(4, 3);
    test_afs_frame_range(frame_range1, 4, 3);

    afs_frame_range * frame_range2 = afs_frame_range_create2(4, 3);
    test_afs_frame_range(frame_range2, 4, 3);

    test_equals_frame_range(frame_range1, frame_range2, DTRUE);

    afs_frame_range_free(frame_range1);
    afs_frame_range_free(frame_range2);
}
END_TEST


// Test function afs_frame_range_equal with empty input afs_frame_range instance and pointer equal to NULL 
BOXING_START_TEST(afs_frame_range_equal_test4)
{
    afs_frame_range * frame_range1 = afs_frame_range_create();
    test_afs_frame_range(frame_range1, 0, 0);

    afs_frame_range * frame_range2 = NULL;

    test_equals_frame_range(frame_range1, frame_range2, DFALSE);

    afs_frame_range_free(frame_range1);
}
END_TEST


// Test function afs_frame_range_equal with normal input afs_frame_range_page instance and pointer equal to NULL
BOXING_START_TEST(afs_frame_range_equal_test5)
{
    afs_frame_range * frame_range1 = afs_frame_range_create2(4, 3);
    test_afs_frame_range(frame_range1, 4, 3);

    afs_frame_range * frame_range2 = NULL;

    test_equals_frame_range(frame_range1, frame_range2, DFALSE);

    afs_frame_range_free(frame_range1);
}
END_TEST


// Test function afs_frame_range_equal with normal afs_frame_range instance and empty afs_frame_range instance
BOXING_START_TEST(afs_frame_range_equal_test6)
{
    afs_frame_range * frame_range1 = afs_frame_range_create2(4, 3);
    test_afs_frame_range(frame_range1, 4, 3);

    afs_frame_range * frame_range2 = afs_frame_range_create();
    test_afs_frame_range(frame_range2, 0, 0);

    test_equals_frame_range(frame_range1, frame_range2, DFALSE);

    afs_frame_range_free(frame_range1);
    afs_frame_range_free(frame_range2);
}
END_TEST


// Test function afs_frame_range_equal to afs_frame_range instances with different start value
BOXING_START_TEST(afs_frame_range_equal_test7)
{
    afs_frame_range * frame_range1 = afs_frame_range_create2(0, 3);

    afs_frame_range * frame_range2 = afs_frame_range_create2(4, 3);

    test_equals_frame_range(frame_range1, frame_range2, DFALSE);

    afs_frame_range_free(frame_range1);
    afs_frame_range_free(frame_range2);
}
END_TEST


// Test function afs_frame_range_equal to afs_frame_range instances with different count value
BOXING_START_TEST(afs_frame_range_equal_test8)
{
    afs_frame_range * frame_range1 = afs_frame_range_create2(4, 0);

    afs_frame_range * frame_range2 = afs_frame_range_create2(4, 3);

    test_equals_frame_range(frame_range1, frame_range2, DFALSE);

    afs_frame_range_free(frame_range1);
    afs_frame_range_free(frame_range2);
}
END_TEST


// Test function afs_frame_range_get_end with input afs_frame_range pointers equal to NULL
BOXING_START_TEST(afs_frame_range_get_end_test1)
{
    afs_frame_range * frame_range = NULL;

    afs_frame_range_type result = afs_frame_range_get_end(frame_range);

    BOXING_ASSERT(frame_range == NULL);
    BOXING_ASSERT(result == 0);
}
END_TEST


// Test function afs_frame_range_get_end with empty input afs_frame_range instance
BOXING_START_TEST(afs_frame_range_get_end_test2)
{
    afs_frame_range * frame_range = afs_frame_range_create();
    test_afs_frame_range(frame_range, 0, 0);

    afs_frame_range_type result = afs_frame_range_get_end(frame_range);

    BOXING_ASSERT(result == 0);

    afs_frame_range_free(frame_range);
}
END_TEST


// Test function afs_frame_range_get_end with count value equal to zero
BOXING_START_TEST(afs_frame_range_get_end_test3)
{
    afs_frame_range * frame_range = afs_frame_range_create2(5, 0);
    test_afs_frame_range(frame_range, 5, 0);

    afs_frame_range_type result = afs_frame_range_get_end(frame_range);

    BOXING_ASSERT(result == 0);

    afs_frame_range_free(frame_range);
}
END_TEST


// Test function afs_frame_range_get_end with count value equal to zero
BOXING_START_TEST(afs_frame_range_get_end_test4)
{
    afs_frame_range * frame_range = afs_frame_range_create2(0, 5);
    test_afs_frame_range(frame_range, 0, 5);

    afs_frame_range_type result = afs_frame_range_get_end(frame_range);

    BOXING_ASSERT(result == 4);

    afs_frame_range_free(frame_range);
}
END_TEST


// Test function afs_frame_range_get_end with start and count values not equal to zero
BOXING_START_TEST(afs_frame_range_get_end_test5)
{
    afs_frame_range * frame_range = afs_frame_range_create2(12, 5);
    test_afs_frame_range(frame_range, 12, 5);

    afs_frame_range_type result = afs_frame_range_get_end(frame_range);

    BOXING_ASSERT(result == 16);

    afs_frame_range_free(frame_range);
}
END_TEST


// Test function afs_frame_range_get_end with start equal to 0 and count equal to 1
BOXING_START_TEST(afs_frame_range_get_end_test6)
{
    afs_frame_range * frame_range = afs_frame_range_create2(0, 1);
    test_afs_frame_range(frame_range, 0, 1);

    afs_frame_range_type result = afs_frame_range_get_end(frame_range);

    BOXING_ASSERT(result == 0);

    afs_frame_range_free(frame_range);
}
END_TEST


Suite * framerange_tests(void)
{
    TCase * tc_framerange_functions_tests = tcase_create("framerange_functions_tests");

    // Test function afs_frame_range_create
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_create_test1);
    // Test function afs_toc_file_preview_page_create2
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_create2_test1);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_create2_test2);
    // Test function afs_frame_range_clone
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_clone_test1);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_clone_test2);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_clone_test3);
    // Test function afs_frame_range_get_new_reference
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_get_new_reference_test1);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_get_new_reference_test2);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_get_new_reference_test3);
    // Test function afs_toc_file_preview_page_equal
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_equal_test1);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_equal_test2);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_equal_test3);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_equal_test4);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_equal_test5);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_equal_test6);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_equal_test7);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_equal_test8);
    // Test function afs_frame_range_get_end
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_get_end_test1);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_get_end_test2);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_get_end_test3);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_get_end_test4);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_get_end_test5);
    tcase_add_test(tc_framerange_functions_tests, afs_frame_range_get_end_test6);

    Suite * s = suite_create("framerange_test_util");
    suite_add_tcase(s, tc_framerange_functions_tests);

    return s;
}
