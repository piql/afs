#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "zeroreferenceframe.h"
#include "boxing/utils.h"


static void test_zero_reference_mark(afs_zero_reference_mark* zero_reference_mark)
{
    BOXING_ASSERT(zero_reference_mark != NULL);
    BOXING_ASSERT(zero_reference_mark->width == 100);
    BOXING_ASSERT(zero_reference_mark->height == 14);
    BOXING_ASSERT(zero_reference_mark->top_left_x == 100);
    BOXING_ASSERT(zero_reference_mark->top_left_y == 8);
}


static void test_zero_reference_mark2(afs_zero_reference_mark* zero_reference_mark)
{
    BOXING_ASSERT(zero_reference_mark != NULL);
    BOXING_ASSERT(zero_reference_mark->width == 1000);
    BOXING_ASSERT(zero_reference_mark->height == 142);
    BOXING_ASSERT(zero_reference_mark->top_left_x == 1000);
    BOXING_ASSERT(zero_reference_mark->top_left_y == 88);
}


static void test_zero_reference_frame(afs_zero_reference_frame* zero_reference_frame)
{
    BOXING_ASSERT(zero_reference_frame != NULL);
    BOXING_ASSERT(zero_reference_frame->width == 200);
    BOXING_ASSERT(zero_reference_frame->height == 100);

    test_zero_reference_mark(zero_reference_frame->zero_reference_mark);
}


static void test_zero_reference_frame2(afs_zero_reference_frame* zero_reference_frame)
{
    BOXING_ASSERT(zero_reference_frame != NULL);
    BOXING_ASSERT(zero_reference_frame->width == 2000);
    BOXING_ASSERT(zero_reference_frame->height == 1000);

    test_zero_reference_mark2(zero_reference_frame->zero_reference_mark);
}


// TEST ZERO REFERENCE MARK FUNCTIONS
//


// Test function afs_zero_reference_mark_create test1 with input width and height equal to zero
BOXING_START_TEST(afs_zero_reference_mark_create_test1)
{
    afs_zero_reference_mark* zero_reference_mark = afs_zero_reference_mark_create(0, 0);

    BOXING_ASSERT(zero_reference_mark == NULL);
}
END_TEST


// Test function afs_zero_reference_mark_create test1 with input width equal to zero
BOXING_START_TEST(afs_zero_reference_mark_create_test2)
{
    afs_zero_reference_mark* zero_reference_mark = afs_zero_reference_mark_create(0, 100);

    BOXING_ASSERT(zero_reference_mark == NULL);
}
END_TEST


// Test function afs_zero_reference_mark_create test1 with input height equal to zero
BOXING_START_TEST(afs_zero_reference_mark_create_test3)
{
    afs_zero_reference_mark* zero_reference_mark = afs_zero_reference_mark_create(200, 0);

    BOXING_ASSERT(zero_reference_mark == NULL);
}
END_TEST

// Test function afs_zero_reference_mark_create test1 with too small input sizes
BOXING_START_TEST(afs_zero_reference_mark_create_test4)
{
    afs_zero_reference_mark* zero_reference_mark = afs_zero_reference_mark_create(1, 1);

    BOXING_ASSERT(zero_reference_mark == NULL);
}
END_TEST


// Test function afs_zero_reference_mark_create test1 with input width and height not equal to zero
BOXING_START_TEST(afs_zero_reference_mark_create_test5)
{
    afs_zero_reference_mark* zero_reference_mark = afs_zero_reference_mark_create(200, 100);

    test_zero_reference_mark(zero_reference_mark);

    afs_zero_reference_mark_free(zero_reference_mark);
}
END_TEST


// Test function afs_zero_reference_mark_init test1 with afs_zero_reference_mark pointer equal to NULL
BOXING_START_TEST(afs_zero_reference_mark_init_test1)
{
    afs_zero_reference_mark* zero_reference_mark = NULL;

    DBOOL result = afs_zero_reference_mark_init(zero_reference_mark, 200, 100);

    BOXING_ASSERT(zero_reference_mark == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_zero_reference_mark_init test1 with afs_zero_reference_mark pointer not equal to NULL and input width and height equal to zero
BOXING_START_TEST(afs_zero_reference_mark_init_test2)
{
    afs_zero_reference_mark* zero_reference_mark = afs_zero_reference_mark_create(200, 100);

    test_zero_reference_mark(zero_reference_mark);

    DBOOL result = afs_zero_reference_mark_init(zero_reference_mark, 0, 0);

    BOXING_ASSERT(result == DFALSE);
    test_zero_reference_mark(zero_reference_mark);

    afs_zero_reference_mark_free(zero_reference_mark);
}
END_TEST


// Test function afs_zero_reference_mark_init test1 with afs_zero_reference_mark pointer not equal to NULL and input width equal to zero
BOXING_START_TEST(afs_zero_reference_mark_init_test3)
{
    afs_zero_reference_mark* zero_reference_mark = afs_zero_reference_mark_create(200, 100);

    test_zero_reference_mark(zero_reference_mark);

    DBOOL result = afs_zero_reference_mark_init(zero_reference_mark, 0, 100);

    BOXING_ASSERT(result == DFALSE);
    test_zero_reference_mark(zero_reference_mark);

    afs_zero_reference_mark_free(zero_reference_mark);
}
END_TEST


// Test function afs_zero_reference_mark_init test1 with afs_zero_reference_mark pointer not equal to NULL and input height equal to zero
BOXING_START_TEST(afs_zero_reference_mark_init_test4)
{
    afs_zero_reference_mark* zero_reference_mark = afs_zero_reference_mark_create(200, 100);

    test_zero_reference_mark(zero_reference_mark);

    DBOOL result = afs_zero_reference_mark_init(zero_reference_mark, 200, 0);

    BOXING_ASSERT(result == DFALSE);
    test_zero_reference_mark(zero_reference_mark);

    afs_zero_reference_mark_free(zero_reference_mark);
}
END_TEST


// Test function afs_zero_reference_mark_init test1 with afs_zero_reference_mark pointer not equal to NULL and too small sizes
BOXING_START_TEST(afs_zero_reference_mark_init_test5)
{
    afs_zero_reference_mark* zero_reference_mark = afs_zero_reference_mark_create(200, 100);

    test_zero_reference_mark(zero_reference_mark);

    DBOOL result = afs_zero_reference_mark_init(zero_reference_mark, 1, 1);

    BOXING_ASSERT(result == DFALSE);
    test_zero_reference_mark(zero_reference_mark);

    afs_zero_reference_mark_free(zero_reference_mark);
}
END_TEST


// Test function afs_zero_reference_mark_init test1 with afs_zero_reference_mark pointer not equal to NULL and input width and height not equal to zero
BOXING_START_TEST(afs_zero_reference_mark_init_test6)
{
    afs_zero_reference_mark* zero_reference_mark = afs_zero_reference_mark_create(200, 100);

    test_zero_reference_mark(zero_reference_mark);

    DBOOL result = afs_zero_reference_mark_init(zero_reference_mark, 2000, 1000);

    BOXING_ASSERT(result == DTRUE);
    test_zero_reference_mark2(zero_reference_mark);

    afs_zero_reference_mark_free(zero_reference_mark);
}
END_TEST


// TEST ZERO REFERENCE FRAME FUNCTIONS
//


// Test function afs_zero_reference_frame_create test1 with input width and height equal to zero
BOXING_START_TEST(afs_zero_reference_frame_create_test1)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(0, 0);

    BOXING_ASSERT(zero_reference_frame == NULL);
}
END_TEST


// Test function afs_zero_reference_frame_create test1 with input width equal to zero
BOXING_START_TEST(afs_zero_reference_frame_create_test2)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(0, 100);

    BOXING_ASSERT(zero_reference_frame == NULL);
}
END_TEST


// Test function afs_zero_reference_frame_create test1 with input height equal to zero
BOXING_START_TEST(afs_zero_reference_frame_create_test3)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(200, 0);

    BOXING_ASSERT(zero_reference_frame == NULL);
}
END_TEST

// Test function afs_zero_reference_frame_create test1 with too small input sizes
BOXING_START_TEST(afs_zero_reference_frame_create_test4)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(1, 1);

    BOXING_ASSERT(zero_reference_frame == NULL);
}
END_TEST


// Test function afs_zero_reference_frame_create test1 with input width and height not equal to zero
BOXING_START_TEST(afs_zero_reference_frame_create_test5)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(200, 100);

    test_zero_reference_frame(zero_reference_frame);

    afs_zero_reference_frame_free(zero_reference_frame);
}
END_TEST


// Test function afs_zero_reference_frame_init test1 with afs_zero_reference_frame pointer equal to NULL
BOXING_START_TEST(afs_zero_reference_frame_init_test1)
{
    afs_zero_reference_frame* zero_reference_frame = NULL;

    DBOOL result = afs_zero_reference_frame_init(zero_reference_frame, 200, 100);

    BOXING_ASSERT(zero_reference_frame == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_zero_reference_frame_init test1 with afs_zero_reference_mframe pointer not equal to NULL and input width and height equal to zero
BOXING_START_TEST(afs_zero_reference_frame_init_test2)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(200, 100);

    test_zero_reference_frame(zero_reference_frame);

    DBOOL result = afs_zero_reference_frame_init(zero_reference_frame, 0, 0);

    BOXING_ASSERT(result == DFALSE);
    test_zero_reference_frame(zero_reference_frame);

    afs_zero_reference_frame_free(zero_reference_frame);
}
END_TEST


// Test function afs_zero_reference_frame_init test1 with afs_zero_reference_frame pointer not equal to NULL and input width equal to zero
BOXING_START_TEST(afs_zero_reference_frame_init_test3)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(200, 100);

    test_zero_reference_frame(zero_reference_frame);

    DBOOL result = afs_zero_reference_frame_init(zero_reference_frame, 0, 100);

    BOXING_ASSERT(result == DFALSE);
    test_zero_reference_frame(zero_reference_frame);

    afs_zero_reference_frame_free(zero_reference_frame);
}
END_TEST


// Test function afs_zero_reference_frame_init test1 with afs_zero_reference_frame pointer not equal to NULL and input height equal to zero
BOXING_START_TEST(afs_zero_reference_frame_init_test4)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(200, 100);

    test_zero_reference_frame(zero_reference_frame);

    DBOOL result = afs_zero_reference_frame_init(zero_reference_frame, 200, 0);

    BOXING_ASSERT(result == DFALSE);
    test_zero_reference_frame(zero_reference_frame);

    afs_zero_reference_frame_free(zero_reference_frame);
}
END_TEST


// Test function afs_zero_reference_frame_init test1 with afs_zero_reference_frame pointer not equal to NULL and too small sizes
BOXING_START_TEST(afs_zero_reference_frame_init_test5)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(200, 100);

    test_zero_reference_frame(zero_reference_frame);

    DBOOL result = afs_zero_reference_frame_init(zero_reference_frame, 1, 1);

    BOXING_ASSERT(result == DFALSE);
    test_zero_reference_frame(zero_reference_frame);

    afs_zero_reference_frame_free(zero_reference_frame);
}
END_TEST


// Test function afs_zero_reference_frame_init test1 with afs_zero_reference_frame pointer not equal to NULL and input width and height not equal to zero
BOXING_START_TEST(afs_zero_reference_frame_init_test6)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(200, 100);

    test_zero_reference_frame(zero_reference_frame);

    afs_zero_reference_mark_free(zero_reference_frame->zero_reference_mark);
    DBOOL result = afs_zero_reference_frame_init(zero_reference_frame, 2000, 1000);

    BOXING_ASSERT(result == DTRUE);
    test_zero_reference_frame2(zero_reference_frame);

    afs_zero_reference_frame_free(zero_reference_frame);
}
END_TEST


// Test function afs_zero_reference_frame_create_image test1 with afs_zero_reference_frame pointer equal to NULL
BOXING_START_TEST(afs_zero_reference_frame_create_image_test1)
{
    afs_zero_reference_frame* zero_reference_frame = NULL;

    boxing_image8 * image = afs_zero_reference_frame_create_image(zero_reference_frame);

    BOXING_ASSERT(zero_reference_frame == NULL);
    BOXING_ASSERT(image == NULL);
}
END_TEST


// Test function afs_zero_reference_frame_create_image test2 with afs_zero_reference_frame pointer not equal to NULL
BOXING_START_TEST(afs_zero_reference_frame_create_image_test2)
{
    afs_zero_reference_frame* zero_reference_frame = afs_zero_reference_frame_create(200, 100);

    //test_zero_reference_frame(zero_reference_frame);

    boxing_image8 * image = afs_zero_reference_frame_create_image(zero_reference_frame);

    BOXING_ASSERT(zero_reference_frame != NULL);
    BOXING_ASSERT(image != NULL);
    BOXING_ASSERT(image->width == zero_reference_frame->width);
    BOXING_ASSERT(image->height == zero_reference_frame->height);
    
    afs_zero_reference_mark * zero_reference_mark = zero_reference_frame->zero_reference_mark;

    BOXING_ASSERT(zero_reference_mark != NULL);

    for (unsigned int i = 0; i < zero_reference_frame->width; i++)
    {
        for (unsigned int j = 0; j < zero_reference_frame->height; j++)
        {
            if (i >= zero_reference_mark->top_left_x &&
                j >= zero_reference_mark->top_left_y &&
                j < (zero_reference_mark->top_left_y + zero_reference_mark->height))
            {
                BOXING_ASSERT(IMAGE8_PIXEL(image, i, j) == BOXING_PIXEL_MIN);
            }
            else
            {
                BOXING_ASSERT(IMAGE8_PIXEL(image, i, j) == BOXING_PIXEL_MAX);
            }
        }
    }

    afs_zero_reference_frame_free(zero_reference_frame);
    boxing_image8_free(image);
}
END_TEST



Suite * zeroreferencemark_tests(void)
{
    TCase * tc_zeroreferencemark_functions_tests = tcase_create("zeroreferencemark_functions_tests");

    // Test function afs_zero_reference_mark_create
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_create_test1);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_create_test2);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_create_test3);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_create_test4);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_create_test5);
    // Test function afs_zero_reference_mark_init
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_init_test1);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_init_test2);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_init_test3);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_init_test4);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_init_test5);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_mark_init_test6);
    // Test function afs_zero_reference_frame_create
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_create_test1);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_create_test2);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_create_test3);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_create_test4);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_create_test5);
    // Test function afs_zero_reference_frame_init
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_init_test1);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_init_test2);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_init_test3);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_init_test4);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_init_test5);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_init_test6);
    // Test function afs_zero_reference_frame_create_image
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_create_image_test1);
    tcase_add_test(tc_zeroreferencemark_functions_tests, afs_zero_reference_frame_create_image_test2);


    Suite * s = suite_create("zeroreferencemark_test_util");
    suite_add_tcase(s, tc_zeroreferencemark_functions_tests);

    return s;
}

