#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "controlframe/boxingformat.h"
#include "boxing_config.h"
#include "boxing/utils.h"
#include "mxml.h"

#include <string.h>

#if defined ( D_OS_WIN32 )
#define DFSEEK _fseeki64
#define DFTELL _ftelli64
#else
#define DFSEEK fseeko
#define DFTELL ftello
#endif


static void test_empty_afs_boxing_format(afs_boxing_format * boxing_format)
{
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(boxing_format->config == NULL);
    BOXING_ASSERT(boxing_format->name == NULL);
    BOXING_ASSERT(boxing_format->bytes_per_frame == 0);
    BOXING_ASSERT(boxing_format->data_bytes_per_frame == 0);
    BOXING_ASSERT(boxing_format->data_stripe_size == 0);
    BOXING_ASSERT(boxing_format->scaling_factor == 0);
    BOXING_ASSERT(boxing_format->width == 0);
    BOXING_ASSERT(boxing_format->height == 0);
    BOXING_ASSERT(boxing_format->data_frame_width == 0);
    BOXING_ASSERT(boxing_format->data_frame_height == 0);
    BOXING_ASSERT(boxing_format->bits_per_pixel == 0);
    BOXING_ASSERT(boxing_format->symbol_per_pixel == 0);
}


static unsigned int get_configuration_number(const char * config_name)
{
    if (config_name == NULL)
    {
        return 0;
    }

    const char * configurations[] = { "4kv6", "4kv7", "4kv8", "4kv9", "4kv10" };
    unsigned int configurations_count = 5;

    for (unsigned int i = 1; i <= configurations_count; i++)
    {
        if (strcmp(configurations[i - 1], config_name) == 0)
        {
            return i;
        }
    }

    return 0;
}


static void test_not_empty_afs_boxing_format(afs_boxing_format * boxing_format, const char * config_name)
{
    unsigned int bytes_per_frame_array[] = { 0, 2106644, 2106644, 2106644, 2106644, 2106644 };
    unsigned int data_bytes_per_frame_array[] = { 0, 1619143, 1619143, 1619143, 1619143, 1849259 };
    unsigned int data_stripe_size_array[] = { 0, 1, 1, 1, 1, 200 };
    unsigned int scaling_factor_array[] = { 0, 1, 1, 1, 1, 1 };
    unsigned int width_array[] = { 0, 4096, 4096, 4096, 4096, 4096 };
    unsigned int height_array[] = { 0, 2160, 2160, 2160, 2160, 2160 };
    unsigned int data_frame_width_array[] = { 0, 4028, 4028, 4028, 4028, 4028 };
    unsigned int data_frame_height_array[] = { 0, 2092, 2092, 2092, 2092, 2092 };
    unsigned int bits_per_pixel_array[] = { 0, 2, 2, 2, 2, 2 };
    unsigned int symbol_per_pixel_array[] = { 0, 4, 4, 4, 4, 4 };

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(boxing_format->config != NULL);
    BOXING_ASSERT(strcmp(boxing_format->name, config_name) == 0);

    unsigned int configuration_number = get_configuration_number(config_name);

    BOXING_ASSERT(boxing_format->bytes_per_frame == bytes_per_frame_array[configuration_number]);
    BOXING_ASSERT(boxing_format->data_bytes_per_frame == data_bytes_per_frame_array[configuration_number]);
    BOXING_ASSERT(boxing_format->data_stripe_size == data_stripe_size_array[configuration_number]);
    BOXING_ASSERT(boxing_format->scaling_factor == scaling_factor_array[configuration_number]);
    BOXING_ASSERT(boxing_format->width == width_array[configuration_number]);
    BOXING_ASSERT(boxing_format->height == height_array[configuration_number]);
    BOXING_ASSERT(boxing_format->data_frame_width == data_frame_width_array[configuration_number]);
    BOXING_ASSERT(boxing_format->data_frame_height == data_frame_height_array[configuration_number]);
    BOXING_ASSERT(boxing_format->bits_per_pixel == bits_per_pixel_array[configuration_number]);
    BOXING_ASSERT(boxing_format->symbol_per_pixel == symbol_per_pixel_array[configuration_number]);
}


static void test_equals_boxing_format(afs_boxing_format * boxing_format1, afs_boxing_format * boxing_format2, DBOOL expected_result)
{
    DBOOL result = afs_boxing_format_equal(boxing_format2, boxing_format1);
    BOXING_ASSERT(result == expected_result);
    result = afs_boxing_format_equal(boxing_format1, boxing_format2);
    BOXING_ASSERT(result == expected_result);
}


static afs_boxing_format * get_boxing_format_instance(const char * format_name)
{
    if (format_name == NULL)
    {
        return NULL;
    }

    boxing_config * config = boxing_create_boxing_config(format_name);

    if (config == NULL)
    {
        config = boxing_config_create();
    }

    afs_boxing_format * boxing_format = afs_boxing_format_create2(config);

    boxing_config_free(config);

    return boxing_format;
}


static mxml_node_t * get_xml_instance(const char * file_name)
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


static char * read_xml_config_file(const char* file_name)
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


// Test function afs_boxing_format_create
BOXING_START_TEST(afs_boxing_format_create_test1)
{
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    test_empty_afs_boxing_format(boxing_format);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_create2 with boxing_config pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_create2_test1)
{
    boxing_config * config = NULL;

    afs_boxing_format * boxing_format = afs_boxing_format_create2(config);

    test_empty_afs_boxing_format(boxing_format);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_create2 with empty boxing_config structure
BOXING_START_TEST(afs_boxing_format_create2_test2)
{
    boxing_config * config = boxing_config_create();

    afs_boxing_format * boxing_format = afs_boxing_format_create2(config);

    test_not_empty_afs_boxing_format(boxing_format, NULL);

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_create2 with "4kv6" boxing_config structure
BOXING_START_TEST(afs_boxing_format_create2_test3)
{
    const char * config_name = "4kv6";

    boxing_config * config = boxing_create_boxing_config(config_name);
    afs_boxing_format * boxing_format = afs_boxing_format_create2(config);

    test_not_empty_afs_boxing_format(boxing_format, config_name);

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_create2 with "4kv7" boxing_config structure
BOXING_START_TEST(afs_boxing_format_create2_test4)
{
    const char * config_name = "4kv7";

    boxing_config * config = boxing_create_boxing_config(config_name);
    afs_boxing_format * boxing_format = afs_boxing_format_create2(config);

    test_not_empty_afs_boxing_format(boxing_format, config_name);

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_create2 with "4kv8" boxing_config structure
BOXING_START_TEST(afs_boxing_format_create2_test5)
{
    const char * config_name = "4kv8";

    boxing_config * config = boxing_create_boxing_config(config_name);
    afs_boxing_format * boxing_format = afs_boxing_format_create2(config);

    test_not_empty_afs_boxing_format(boxing_format, config_name);

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_create2 with "4kv9" boxing_config structure
BOXING_START_TEST(afs_boxing_format_create2_test6)
{
    const char * config_name = "4kv9";

    boxing_config * config = boxing_create_boxing_config(config_name);
    afs_boxing_format * boxing_format = afs_boxing_format_create2(config);

    test_not_empty_afs_boxing_format(boxing_format, config_name);

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_create2 with "4kv10" boxing_config structure
BOXING_START_TEST(afs_boxing_format_create2_test7)
{
    const char * config_name = "4kv10";

    boxing_config * config = boxing_create_boxing_config(config_name);
    afs_boxing_format * boxing_format = afs_boxing_format_create2(config);

    test_not_empty_afs_boxing_format(boxing_format, config_name);

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_clone with input pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_clone_test1)
{
    afs_boxing_format * boxing_format1 = NULL;

    BOXING_ASSERT(boxing_format1 == NULL);

    afs_boxing_format * boxing_format2 = afs_boxing_format_clone(boxing_format1);

    BOXING_ASSERT(boxing_format2 == NULL);
}
END_TEST


// Test function afs_boxing_format_clone with empty input afs_boxing_format instance
BOXING_START_TEST(afs_boxing_format_clone_test2)
{
    afs_boxing_format * boxing_format1 = afs_boxing_format_create();

    test_empty_afs_boxing_format(boxing_format1);

    afs_boxing_format * boxing_format2 = afs_boxing_format_clone(boxing_format1);

    test_empty_afs_boxing_format(boxing_format2);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_clone with not empty input afs_boxing_format instance with empty config
BOXING_START_TEST(afs_boxing_format_clone_test3)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("");

    test_not_empty_afs_boxing_format(boxing_format1, NULL);

    afs_boxing_format * boxing_format2 = afs_boxing_format_clone(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, NULL);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_clone with not empty input afs_boxing_format instance with config "4kv6"
BOXING_START_TEST(afs_boxing_format_clone_test4)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv6");

    test_not_empty_afs_boxing_format(boxing_format1, "4kv6");

    afs_boxing_format * boxing_format2 = afs_boxing_format_clone(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, "4kv6");

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_clone with not empty input afs_boxing_format instance with config "4kv7"
BOXING_START_TEST(afs_boxing_format_clone_test5)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv7");

    test_not_empty_afs_boxing_format(boxing_format1, "4kv7");

    afs_boxing_format * boxing_format2 = afs_boxing_format_clone(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, "4kv7");

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_clone with not empty input afs_boxing_format instance with config "4kv8"
BOXING_START_TEST(afs_boxing_format_clone_test6)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv8");

    test_not_empty_afs_boxing_format(boxing_format1, "4kv8");

    afs_boxing_format * boxing_format2 = afs_boxing_format_clone(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, "4kv8");

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_clone with not empty input afs_boxing_format instance with config "4kv9"
BOXING_START_TEST(afs_boxing_format_clone_test7)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");

    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = afs_boxing_format_clone(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_clone with not empty input afs_boxing_format instance with config "4kv10"
BOXING_START_TEST(afs_boxing_format_clone_test8)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv10");

    test_not_empty_afs_boxing_format(boxing_format1, "4kv10");

    afs_boxing_format * boxing_format2 = afs_boxing_format_clone(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, "4kv10");

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_get_new_reference with input pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_get_new_reference_test1)
{
    afs_boxing_format * boxing_format1 = NULL;

    BOXING_ASSERT(boxing_format1 == NULL);

    afs_boxing_format * boxing_format2 = afs_boxing_format_get_new_reference(boxing_format1);

    BOXING_ASSERT(boxing_format2 == NULL);

    boxing_format1 = afs_boxing_format_create();

    test_empty_afs_boxing_format(boxing_format1);
    BOXING_ASSERT(boxing_format1->reference_count == 1);
    BOXING_ASSERT(boxing_format2 == NULL);

    afs_boxing_format_free(boxing_format1);
}
END_TEST


// Test function afs_boxing_format_get_new_reference with empty input afs_boxing_format instance
BOXING_START_TEST(afs_boxing_format_get_new_reference_test2)
{
    afs_boxing_format * boxing_format1 = afs_boxing_format_create();

    test_empty_afs_boxing_format(boxing_format1);
    BOXING_ASSERT(boxing_format1->reference_count == 1);

    afs_boxing_format * boxing_format2 = afs_boxing_format_get_new_reference(boxing_format1);

    test_empty_afs_boxing_format(boxing_format2);
    BOXING_ASSERT(boxing_format1->reference_count == 2);
    BOXING_ASSERT(boxing_format2->reference_count == 2);
    BOXING_ASSERT(boxing_format1 == boxing_format2);

    boxing_format1->bytes_per_frame = 99;

    BOXING_ASSERT(boxing_format2->bytes_per_frame == 99);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_get_new_reference with not empty input afs_boxing_format instance with empty config
BOXING_START_TEST(afs_boxing_format_get_new_reference_test3)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("");

    test_not_empty_afs_boxing_format(boxing_format1, NULL);
    BOXING_ASSERT(boxing_format1->reference_count == 1);

    afs_boxing_format * boxing_format2 = afs_boxing_format_get_new_reference(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, NULL);
    BOXING_ASSERT(boxing_format1->reference_count == 2);
    BOXING_ASSERT(boxing_format2->reference_count == 2);
    BOXING_ASSERT(boxing_format1 == boxing_format2);

    boxing_format1->bytes_per_frame = 99;

    BOXING_ASSERT(boxing_format2->bytes_per_frame == 99);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_get_new_reference with not empty input afs_boxing_format instance with config "4kv6"
BOXING_START_TEST(afs_boxing_format_get_new_reference_test4)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv6");

    test_not_empty_afs_boxing_format(boxing_format1, "4kv6");
    BOXING_ASSERT(boxing_format1->reference_count == 1);

    afs_boxing_format * boxing_format2 = afs_boxing_format_get_new_reference(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, "4kv6");
    BOXING_ASSERT(boxing_format1->reference_count == 2);
    BOXING_ASSERT(boxing_format2->reference_count == 2);
    BOXING_ASSERT(boxing_format1 == boxing_format2);

    boxing_format1->bytes_per_frame = 99;

    BOXING_ASSERT(boxing_format2->bytes_per_frame == 99);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_get_new_reference with not empty input afs_boxing_format instance with config "4kv7"
BOXING_START_TEST(afs_boxing_format_get_new_reference_test5)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv7");

    test_not_empty_afs_boxing_format(boxing_format1, "4kv7");
    BOXING_ASSERT(boxing_format1->reference_count == 1);

    afs_boxing_format * boxing_format2 = afs_boxing_format_get_new_reference(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, "4kv7");
    BOXING_ASSERT(boxing_format1->reference_count == 2);
    BOXING_ASSERT(boxing_format2->reference_count == 2);
    BOXING_ASSERT(boxing_format1 == boxing_format2);

    boxing_format1->bytes_per_frame = 99;

    BOXING_ASSERT(boxing_format2->bytes_per_frame == 99);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_get_new_reference with not empty input afs_boxing_format instance with config "4kv8"
BOXING_START_TEST(afs_boxing_format_get_new_reference_test6)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv8");

    test_not_empty_afs_boxing_format(boxing_format1, "4kv8");
    BOXING_ASSERT(boxing_format1->reference_count == 1);

    afs_boxing_format * boxing_format2 = afs_boxing_format_get_new_reference(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, "4kv8");
    BOXING_ASSERT(boxing_format1->reference_count == 2);
    BOXING_ASSERT(boxing_format2->reference_count == 2);
    BOXING_ASSERT(boxing_format1 == boxing_format2);

    boxing_format1->bytes_per_frame = 99;

    BOXING_ASSERT(boxing_format2->bytes_per_frame == 99);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_get_new_reference with not empty input afs_boxing_format instance with config "4kv9"
BOXING_START_TEST(afs_boxing_format_get_new_reference_test7)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");

    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");
    BOXING_ASSERT(boxing_format1->reference_count == 1);

    afs_boxing_format * boxing_format2 = afs_boxing_format_get_new_reference(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    BOXING_ASSERT(boxing_format1->reference_count == 2);
    BOXING_ASSERT(boxing_format2->reference_count == 2);
    BOXING_ASSERT(boxing_format1 == boxing_format2);

    boxing_format1->bytes_per_frame = 99;

    BOXING_ASSERT(boxing_format2->bytes_per_frame == 99);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_get_new_reference with not empty input afs_boxing_format instance with config "4kv10"
BOXING_START_TEST(afs_boxing_format_get_new_reference_test8)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv10");

    test_not_empty_afs_boxing_format(boxing_format1, "4kv10");
    BOXING_ASSERT(boxing_format1->reference_count == 1);

    afs_boxing_format * boxing_format2 = afs_boxing_format_get_new_reference(boxing_format1);

    test_not_empty_afs_boxing_format(boxing_format2, "4kv10");
    BOXING_ASSERT(boxing_format1->reference_count == 2);
    BOXING_ASSERT(boxing_format2->reference_count == 2);
    BOXING_ASSERT(boxing_format1 == boxing_format2);

    boxing_format1->bytes_per_frame = 99;

    BOXING_ASSERT(boxing_format2->bytes_per_frame == 99);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format pointers equal to NULL
BOXING_START_TEST(afs_boxing_format_equal_test1)
{
    afs_boxing_format * boxing_format1 = NULL;
    afs_boxing_format * boxing_format2 = NULL;

    test_equals_boxing_format(boxing_format2, boxing_format1, DTRUE);
}
END_TEST

// Test function afs_boxing_format_equal with empty input afs_boxing_format instances 
BOXING_START_TEST(afs_boxing_format_equal_test2)
{
    afs_boxing_format * boxing_format1 = afs_boxing_format_create();
    test_empty_afs_boxing_format(boxing_format1);

    afs_boxing_format * boxing_format2 = afs_boxing_format_create();
    test_empty_afs_boxing_format(boxing_format2);

    test_equals_boxing_format(boxing_format2, boxing_format1, DTRUE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with not empty input afs_boxing_format instances with empty config
BOXING_START_TEST(afs_boxing_format_equal_test3)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("");
    test_not_empty_afs_boxing_format(boxing_format1, NULL);

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("");
    test_not_empty_afs_boxing_format(boxing_format2, NULL);

    test_equals_boxing_format(boxing_format2, boxing_format1, DTRUE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with not empty input afs_boxing_format instances with config "4kv6"
BOXING_START_TEST(afs_boxing_format_equal_test4)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv6");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv6");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv6");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv6");

    test_equals_boxing_format(boxing_format2, boxing_format1, DTRUE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with not empty input afs_boxing_format instances with config "4kv7"
BOXING_START_TEST(afs_boxing_format_equal_test5)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv7");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv7");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv7");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv7");

    test_equals_boxing_format(boxing_format2, boxing_format1, DTRUE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with not empty input afs_boxing_format instances with config "4kv8"
BOXING_START_TEST(afs_boxing_format_equal_test6)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv8");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv8");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv8");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv8");

    test_equals_boxing_format(boxing_format2, boxing_format1, DTRUE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with not empty input afs_boxing_format instances with config "4kv9"
BOXING_START_TEST(afs_boxing_format_equal_test7)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");

    test_equals_boxing_format(boxing_format2, boxing_format1, DTRUE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with not empty input afs_boxing_format instances with config "4kv10"
BOXING_START_TEST(afs_boxing_format_equal_test8)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv10");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv10");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv10");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv10");

    test_equals_boxing_format(boxing_format2, boxing_format1, DTRUE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with empty input afs_boxing_format instance and pointer equal to NULL 
BOXING_START_TEST(afs_boxing_format_equal_test9)
{
    afs_boxing_format * boxing_format1 = afs_boxing_format_create();
    test_empty_afs_boxing_format(boxing_format1);

    afs_boxing_format * boxing_format2 = NULL;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instance with empty config and pointer equal to NULL 
BOXING_START_TEST(afs_boxing_format_equal_test10)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("");
    test_not_empty_afs_boxing_format(boxing_format1, NULL);

    afs_boxing_format * boxing_format2 = NULL;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instance with not empty config and pointer equal to NULL 
BOXING_START_TEST(afs_boxing_format_equal_test11)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = NULL;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
}
END_TEST


// Test function afs_boxing_format_equal with empty input afs_boxing_format instance and instance with empty config 
BOXING_START_TEST(afs_boxing_format_equal_test12)
{
    afs_boxing_format * boxing_format1 = afs_boxing_format_create();
    test_empty_afs_boxing_format(boxing_format1);

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("");
    test_not_empty_afs_boxing_format(boxing_format2, NULL);

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with empty input afs_boxing_format instance and instance with not empty config 
BOXING_START_TEST(afs_boxing_format_equal_test13)
{
    afs_boxing_format * boxing_format1 = afs_boxing_format_create();
    test_empty_afs_boxing_format(boxing_format1);

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv8");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv8");

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instance with empty config and instance with not empty config 
BOXING_START_TEST(afs_boxing_format_equal_test14)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("");
    test_not_empty_afs_boxing_format(boxing_format1, NULL);

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv8");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv8");

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different config 
BOXING_START_TEST(afs_boxing_format_equal_test15)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv8");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv8");

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different name
BOXING_START_TEST(afs_boxing_format_equal_test16)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->name = NULL;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different bytes per frame values
BOXING_START_TEST(afs_boxing_format_equal_test17)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->bytes_per_frame = 12345;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different data bytes per frame values
BOXING_START_TEST(afs_boxing_format_equal_test18)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->data_bytes_per_frame = 12345;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different data stripe size values
BOXING_START_TEST(afs_boxing_format_equal_test19)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->data_stripe_size = 12345;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different scaling factor values
BOXING_START_TEST(afs_boxing_format_equal_test20)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->scaling_factor = 12345;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different width values
BOXING_START_TEST(afs_boxing_format_equal_test21)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->width = 12345;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different height values
BOXING_START_TEST(afs_boxing_format_equal_test22)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->height = 12345;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different data frame width values
BOXING_START_TEST(afs_boxing_format_equal_test23)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->data_frame_width = 12345;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different data frame height values
BOXING_START_TEST(afs_boxing_format_equal_test24)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->data_frame_height = 12345;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different bits per pixel values
BOXING_START_TEST(afs_boxing_format_equal_test25)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->bits_per_pixel = 12345;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_equal with input afs_boxing_format instances with different symbol per pixel values
BOXING_START_TEST(afs_boxing_format_equal_test26)
{
    afs_boxing_format * boxing_format1 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format1, "4kv9");

    afs_boxing_format * boxing_format2 = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format2, "4kv9");
    boxing_format2->symbol_per_pixel = 12345;

    test_equals_boxing_format(boxing_format2, boxing_format1, DFALSE);

    afs_boxing_format_free(boxing_format1);
    afs_boxing_format_free(boxing_format2);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with input afs_boxing_format pointer equal to NULL and input file size equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test1)
{
    afs_boxing_format * boxing_format = NULL;

    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, 0);

    BOXING_ASSERT(data_frames == 0);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with input afs_boxing_format pointer equal to NULL and input file size not equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test2)
{
    afs_boxing_format * boxing_format = NULL;

    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, 10000);

    BOXING_ASSERT(data_frames == 0);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with empty input afs_boxing_format instance and input file size equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test3)
{
    afs_boxing_format * boxing_format = afs_boxing_format_create();
    test_empty_afs_boxing_format(boxing_format);

    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, 0);

    BOXING_ASSERT(data_frames == 0);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with empty input afs_boxing_format instance and input file size not equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test4)
{
    afs_boxing_format * boxing_format = afs_boxing_format_create();
    test_empty_afs_boxing_format(boxing_format);

    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, 10000);

    BOXING_ASSERT(data_frames == 0);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with afs_boxing_format instance with empty config and input file size equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test5)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("");
    test_not_empty_afs_boxing_format(boxing_format, NULL);

    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, 0);

    BOXING_ASSERT(data_frames == 0);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with afs_boxing_format instance with empty config and input file size not equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test6)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("");
    test_not_empty_afs_boxing_format(boxing_format, NULL);

    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, 10000);

    BOXING_ASSERT(data_frames == 0);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with afs_boxing_format instance with config "4kv6" and input file size equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test7)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv6");
    test_not_empty_afs_boxing_format(boxing_format, "4kv6");

    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, 0);

    BOXING_ASSERT(data_frames == 0);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with afs_boxing_format instance with config "4kv6" and input file size not equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test8)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv6");
    test_not_empty_afs_boxing_format(boxing_format, "4kv6");

    size_t file_size = 1;
    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 1);

    file_size = (size_t)boxing_format->bytes_per_frame;
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 1);

    file_size = (size_t)(boxing_format->bytes_per_frame * 1.5);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 2);

    file_size = (size_t)(boxing_format->bytes_per_frame * 2);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 2);

    file_size = (size_t)(boxing_format->bytes_per_frame * 2 + 1);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 3);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with afs_boxing_format instance with config "4kv7" and input file size not equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test9)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv7");
    test_not_empty_afs_boxing_format(boxing_format, "4kv7");

    size_t file_size = 1;
    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 1);

    file_size = (size_t)boxing_format->bytes_per_frame;
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 1);

    file_size = (size_t)(boxing_format->bytes_per_frame * 1.5);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 2);

    file_size = (size_t)(boxing_format->bytes_per_frame * 2);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 2);

    file_size = (size_t)(boxing_format->bytes_per_frame * 2 + 1);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 3);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with afs_boxing_format instance with config "4kv8" and input file size not equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test10)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv8");
    test_not_empty_afs_boxing_format(boxing_format, "4kv8");

    size_t file_size = 1;
    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 1);

    file_size = (size_t)boxing_format->bytes_per_frame;
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 1);

    file_size = (size_t)(boxing_format->bytes_per_frame * 1.5);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 2);

    file_size = (size_t)(boxing_format->bytes_per_frame * 2);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 2);

    file_size = (size_t)(boxing_format->bytes_per_frame * 2 + 1);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 3);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with afs_boxing_format instance with config "4kv9" and input file size not equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test11)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv9");
    test_not_empty_afs_boxing_format(boxing_format, "4kv9");

    size_t file_size = 1;
    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 1);

    file_size = (size_t)boxing_format->bytes_per_frame;
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 1);

    file_size = (size_t)(boxing_format->bytes_per_frame * 1.5);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 2);

    file_size = (size_t)(boxing_format->bytes_per_frame * 2);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 2);

    file_size = (size_t)(boxing_format->bytes_per_frame * 2 + 1);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 3);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_get_data_frames with afs_boxing_format instance with config "4kv10" and input file size not equal to zero
BOXING_START_TEST(afs_boxing_format_get_data_frames_test12)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv10");
    test_not_empty_afs_boxing_format(boxing_format, "4kv10");

    size_t file_size = 1;
    unsigned int data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 1);

    file_size = (size_t)boxing_format->bytes_per_frame;
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 1);

    file_size = (size_t)(boxing_format->bytes_per_frame * 1.5);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 2);

    file_size = (size_t)(boxing_format->bytes_per_frame * 2);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 2);

    file_size = (size_t)(boxing_format->bytes_per_frame * 2 + 1);
    data_frames = afs_boxing_format_get_data_frames(boxing_format, file_size);

    BOXING_ASSERT(data_frames == 3);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_set_config with afs_boxing_format pointer equal to NULL and boxing_config pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_set_config_test1)
{
    boxing_config * config = NULL;
    afs_boxing_format * boxing_format = NULL;

    afs_boxing_format_set_config(boxing_format, config);

    BOXING_ASSERT(config == NULL);
    BOXING_ASSERT(boxing_format == NULL);
}
END_TEST


// Test function afs_boxing_format_set_config with afs_boxing_format pointer equal to NULL and empty boxing_config instance
BOXING_START_TEST(afs_boxing_format_set_config_test2)
{
    boxing_config * config = boxing_config_create();
    afs_boxing_format * boxing_format = NULL;

    afs_boxing_format_set_config(boxing_format, config);

    BOXING_ASSERT(boxing_format == NULL);

    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_set_config with afs_boxing_format pointer equal to NULL and not empty boxing_config instance
BOXING_START_TEST(afs_boxing_format_set_config_test3)
{
    boxing_config * config = boxing_create_boxing_config("4kv10");
    afs_boxing_format * boxing_format = NULL;

    afs_boxing_format_set_config(boxing_format, config);

    BOXING_ASSERT(boxing_format == NULL);

    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_set_config with empty afs_boxing_format instance and boxing_config pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_set_config_test4)
{
    boxing_config * config = NULL;
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    test_empty_afs_boxing_format(boxing_format);

    afs_boxing_format_set_config(boxing_format, config);

    test_empty_afs_boxing_format(boxing_format);
    BOXING_ASSERT(config == NULL);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_set_config with empty afs_boxing_format instance and empty boxing_config instance
BOXING_START_TEST(afs_boxing_format_set_config_test5)
{
    boxing_config * config = boxing_config_create();
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    test_empty_afs_boxing_format(boxing_format);

    afs_boxing_format_set_config(boxing_format, config);

    test_not_empty_afs_boxing_format(boxing_format, NULL);

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_set_config with empty afs_boxing_format instance and not empty boxing_config instance
BOXING_START_TEST(afs_boxing_format_set_config_test6)
{
    boxing_config * config = boxing_create_boxing_config("4kv9");
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    test_empty_afs_boxing_format(boxing_format);

    afs_boxing_format_set_config(boxing_format, config);

    test_not_empty_afs_boxing_format(boxing_format, "4kv9");

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_set_config with afs_boxing_format instance with empty config and boxing_config pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_set_config_test7)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("");

    test_not_empty_afs_boxing_format(boxing_format, NULL);

    boxing_config * config = NULL;
    afs_boxing_format_set_config(boxing_format, config);

    test_empty_afs_boxing_format(boxing_format);
    BOXING_ASSERT(config == NULL);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_set_config with afs_boxing_format instance with empty config and empty boxing_config instance
BOXING_START_TEST(afs_boxing_format_set_config_test8)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("");

    test_not_empty_afs_boxing_format(boxing_format, NULL);

    boxing_config * config = boxing_config_create();
    afs_boxing_format_set_config(boxing_format, config);

    test_not_empty_afs_boxing_format(boxing_format, NULL);

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_set_config with afs_boxing_format instance with empty config and not empty boxing_config instance
BOXING_START_TEST(afs_boxing_format_set_config_test9)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("");

    test_not_empty_afs_boxing_format(boxing_format, NULL);

    boxing_config * config = boxing_create_boxing_config("4kv8");
    afs_boxing_format_set_config(boxing_format, config);

    test_not_empty_afs_boxing_format(boxing_format, "4kv8");

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_set_config with afs_boxing_format instance with not empty config and boxing_config pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_set_config_test10)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv6");

    test_not_empty_afs_boxing_format(boxing_format, "4kv6");

    boxing_config * config = NULL;
    afs_boxing_format_set_config(boxing_format, config);

    test_empty_afs_boxing_format(boxing_format);
    BOXING_ASSERT(config == NULL);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_set_config with afs_boxing_format instance with not empty config and empty boxing_config instance
BOXING_START_TEST(afs_boxing_format_set_config_test11)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv7");

    test_not_empty_afs_boxing_format(boxing_format, "4kv7");

    boxing_config * config = boxing_config_create();
    afs_boxing_format_set_config(boxing_format, config);

    test_not_empty_afs_boxing_format(boxing_format, NULL);

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config);
}
END_TEST


// Test function afs_boxing_format_set_config with afs_boxing_format instance with not empty config and not empty boxing_config instance
BOXING_START_TEST(afs_boxing_format_set_config_test12)
{
    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv8");

    test_not_empty_afs_boxing_format(boxing_format, "4kv8");

    boxing_config * config1 = boxing_create_boxing_config("4kv9");
    afs_boxing_format_set_config(boxing_format, config1);

    test_not_empty_afs_boxing_format(boxing_format, "4kv9");

    boxing_config * config2 = boxing_create_boxing_config("4kv10");
    afs_boxing_format_set_config(boxing_format, config2);

    test_not_empty_afs_boxing_format(boxing_format, "4kv10");

    boxing_config * config3 = boxing_create_boxing_config("4kv6");
    afs_boxing_format_set_config(boxing_format, config3);

    test_not_empty_afs_boxing_format(boxing_format, "4kv6");

    boxing_config * config4 = boxing_create_boxing_config("4kv7");
    afs_boxing_format_set_config(boxing_format, config4);

    test_not_empty_afs_boxing_format(boxing_format, "4kv7");

    afs_boxing_format_free(boxing_format);
    boxing_config_free(config1);
    boxing_config_free(config2);
    boxing_config_free(config3);
    boxing_config_free(config4);
}
END_TEST


// Test function afs_boxing_format_save_config_file with file_name pointer equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_save_config_file_test1)
{
    const char * file_name = NULL;
    afs_boxing_format * boxing_format = NULL;

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_boxing_format_save_config_file with file_name pointer equal to NULL and afs_boxing_format pointer not equal to NULL
BOXING_START_TEST(afs_boxing_format_save_config_file_test2)
{
    const char * file_name = NULL;

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv6");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DTRUE);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with file_name pointer not equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_save_config_file_test3)
{
    const char * file_name = "afs_boxing_format_save_config_file_test.xml";
    afs_boxing_format * boxing_format = NULL;

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_boxing_format_save_config_file with empty boxing_format instance
BOXING_START_TEST(afs_boxing_format_save_config_file_test4)
{
    const char * file_name = "afs_boxing_format_save_without_config_test.xml";
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with empty boxing_config instance in the boxing_format
BOXING_START_TEST(afs_boxing_format_save_config_file_test5)
{
    const char * file_name = "afs_boxing_format_save_empty_config_file_test.xml";
    afs_boxing_format * boxing_format = get_boxing_format_instance("");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with boxing_format instance with format "4kv6" in compact format
BOXING_START_TEST(afs_boxing_format_save_config_file_test6)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv6_compact.xml";

    afs_boxing_format* boxing_format = get_boxing_format_instance("4kv6");
    
    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with boxing_format instance with format "4kv6" and formatted text
BOXING_START_TEST(afs_boxing_format_save_config_file_test7)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv6_not_compact.xml";

    afs_boxing_format* boxing_format = get_boxing_format_instance("4kv6");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with boxing_format instance with format "4kv7" in compact format
BOXING_START_TEST(afs_boxing_format_save_config_file_test8)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv7_compact.xml";

    afs_boxing_format* boxing_format = get_boxing_format_instance("4kv7");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with boxing_format instance with format "4kv7" and formatted text
BOXING_START_TEST(afs_boxing_format_save_config_file_test9)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv7_not_compact.xml";

    afs_boxing_format* boxing_format = get_boxing_format_instance("4kv7");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with boxing_format instance with format "4kv8" in compact format
BOXING_START_TEST(afs_boxing_format_save_config_file_test10)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv8_compact.xml";

    afs_boxing_format* boxing_format = get_boxing_format_instance("4kv8");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with boxing_format instance with format "4kv8" and formatted text
BOXING_START_TEST(afs_boxing_format_save_config_file_test11)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv8_not_compact.xml";

    afs_boxing_format* boxing_format = get_boxing_format_instance("4kv8");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with boxing_format instance with format "4kv9" in compact format
BOXING_START_TEST(afs_boxing_format_save_config_file_test12)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv9_compact.xml";

    afs_boxing_format* boxing_format = get_boxing_format_instance("4kv9");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with boxing_format instance with format "4kv9" and formatted text
BOXING_START_TEST(afs_boxing_format_save_config_file_test13)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv9_not_compact.xml";

    afs_boxing_format* boxing_format = get_boxing_format_instance("4kv9");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with boxing_format instance with format "4kv10" in compact format
BOXING_START_TEST(afs_boxing_format_save_config_file_test14)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv10_compact.xml";

    afs_boxing_format* boxing_format = get_boxing_format_instance("4kv10");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DTRUE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_config_file with boxing_format instance with format "4kv10" and formatted text
BOXING_START_TEST(afs_boxing_format_save_config_file_test15)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv10_not_compact.xml";

    afs_boxing_format* boxing_format = get_boxing_format_instance("4kv10");

    DBOOL result = afs_boxing_format_save_config_file(file_name, boxing_format, DFALSE);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file with file_name pointer equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_load_config_file_test1)
{
    const char * file_name = NULL;
    afs_boxing_format * boxing_format = NULL;

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_boxing_format_load_config_file with file_name pointer equal to NULL and afs_boxing_format pointer not equal to NULL
BOXING_START_TEST(afs_boxing_format_load_config_file_test2)
{
    const char * file_name = NULL;

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name == NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file with file_name pointer not equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_load_config_file_test3)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv6_compact.xml";
    afs_boxing_format * boxing_format = NULL;

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_boxing_format_load_config_file when XML file does not exist
BOXING_START_TEST(afs_boxing_format_load_config_file_test4)
{
    const char * file_name = "file_does_not_exist.xml";
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file with empty boxing_config instance
BOXING_START_TEST(afs_boxing_format_load_config_file_test5)
{
    const char * file_name = "afs_boxing_format_save_without_config_test.xml";
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(result == DTRUE);
    test_empty_afs_boxing_format(boxing_format);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file with afs_boxing_format instance with empty boxing_config instance
BOXING_START_TEST(afs_boxing_format_load_config_file_test6)
{
    const char * file_name = "afs_boxing_format_save_empty_config_file_test.xml";
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);
    
    test_not_empty_afs_boxing_format(boxing_format, NULL);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file from the XML file with "4kv6" config in compact mode
BOXING_START_TEST(afs_boxing_format_load_config_file_test7)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv6_compact.xml";

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);
    
    test_not_empty_afs_boxing_format(boxing_format, "4kv6");

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file from the XML file with "4kv6" config in not compact mode
BOXING_START_TEST(afs_boxing_format_load_config_file_test8)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv6_not_compact.xml";

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv6");

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file from the XML file with "4kv7" config in compact mode
BOXING_START_TEST(afs_boxing_format_load_config_file_test9)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv7_compact.xml";

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv7");

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file from the XML file with "4kv7" config in not compact mode
BOXING_START_TEST(afs_boxing_format_load_config_file_test10)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv7_not_compact.xml";

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv7");

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file from the XML file with "4kv8" config in compact mode
BOXING_START_TEST(afs_boxing_format_load_config_file_test11)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv8_compact.xml";

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv8");

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file from the XML file with "4kv8" config in not compact mode
BOXING_START_TEST(afs_boxing_format_load_config_file_test12)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv8_not_compact.xml";

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv8");

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file from the XML file with "4kv9" config in compact mode
BOXING_START_TEST(afs_boxing_format_load_config_file_test13)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv9_compact.xml";

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv9");

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file from the XML file with "4kv9" config in not compact mode
BOXING_START_TEST(afs_boxing_format_load_config_file_test14)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv9_not_compact.xml";

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv9");

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file from the XML file with "4kv10" config in compact mode
BOXING_START_TEST(afs_boxing_format_load_config_file_test15)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv10_compact.xml";

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv10");

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_config_file from the XML file with "4kv10" config in not compact mode
BOXING_START_TEST(afs_boxing_format_load_config_file_test16)
{
    const char * file_name = "afs_boxing_format_save_config_file_4kv10_not_compact.xml";

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, file_name);

    BOXING_ASSERT(file_name != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv10");

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_xml with mxml_node_t pointer equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_save_xml_test1)
{
    mxml_node_t * out = NULL;
    afs_boxing_format * boxing_format = NULL;

    DBOOL result = afs_boxing_format_save_xml(out, boxing_format);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_boxing_format_save_xml with mxml_node_t pointer equal to NULL and afs_boxing_format pointer not equal to NULL
BOXING_START_TEST(afs_boxing_format_save_xml_test2)
{
    mxml_node_t * out = NULL;

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv10");

    DBOOL result = afs_boxing_format_save_xml(out, boxing_format);

    BOXING_ASSERT(out == NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_save_xml with mxml_node_t pointer not equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_save_xml_test3)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_boxing_format * boxing_format = NULL;

    DBOOL result = afs_boxing_format_save_xml(tree, boxing_format);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_save_xml with empty boxing_format instance
BOXING_START_TEST(afs_boxing_format_save_xml_test4)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_save_xml(tree, boxing_format);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_save_xml with empty boxing_config instance in the boxing_format
BOXING_START_TEST(afs_boxing_format_save_xml_test5)
{
    mxml_node_t * tree = mxmlNewXML("1.0");
    afs_boxing_format * boxing_format = get_boxing_format_instance("");

    DBOOL result = afs_boxing_format_save_xml(tree, boxing_format);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_save_xml with boxing_format instance with config "4kv6"
BOXING_START_TEST(afs_boxing_format_save_xml_test6)
{
    mxml_node_t * tree = mxmlNewXML("1.0");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv6");

    DBOOL result = afs_boxing_format_save_xml(tree, boxing_format);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_save_xml with boxing_format instance with config "4kv7"
BOXING_START_TEST(afs_boxing_format_save_xml_test7)
{
    mxml_node_t * tree = mxmlNewXML("1.0");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv7");

    DBOOL result = afs_boxing_format_save_xml(tree, boxing_format);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_save_xml with boxing_format instance with config "4kv8"
BOXING_START_TEST(afs_boxing_format_save_xml_test8)
{
    mxml_node_t * tree = mxmlNewXML("1.0");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv8");

    DBOOL result = afs_boxing_format_save_xml(tree, boxing_format);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_save_xml with boxing_format instance with config "4kv9"
BOXING_START_TEST(afs_boxing_format_save_xml_test9)
{
    mxml_node_t * tree = mxmlNewXML("1.0");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv9");

    DBOOL result = afs_boxing_format_save_xml(tree, boxing_format);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_save_xml with boxing_format instance with config "4kv10"
BOXING_START_TEST(afs_boxing_format_save_xml_test10)
{
    mxml_node_t * tree = mxmlNewXML("1.0");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv10");

    DBOOL result = afs_boxing_format_save_xml(tree, boxing_format);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_load_xml with mxml_node_t pointer equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_load_xml_test1)
{
    mxml_node_t * tree = NULL;
    afs_boxing_format * boxing_format = NULL;

    DBOOL result = afs_boxing_format_load_xml(boxing_format, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_boxing_format_load_xml with mxml_node_t pointer equal to NULL and afs_boxing_format pointer not equal to NULL
BOXING_START_TEST(afs_boxing_format_load_xml_test2)
{
    mxml_node_t * tree = NULL;

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_xml(boxing_format, tree);

    BOXING_ASSERT(tree == NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_xml with mxml_node_t pointer not equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_load_xml_test3)
{
    mxml_node_t * tree = get_xml_instance("afs_boxing_format_save_config_file_4kv6_compact.xml");
    afs_boxing_format * boxing_format = NULL;

    DBOOL result = afs_boxing_format_load_xml(boxing_format, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == DFALSE);

    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_load_xml with the xml data for empty afs_boxing_format instance
BOXING_START_TEST(afs_boxing_format_load_xml_test4)
{
    mxml_node_t * tree = get_xml_instance("afs_boxing_format_save_without_config_test.xml");

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_xml(boxing_format, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_boxing_format(boxing_format);

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_load_xml with the xml data for afs_boxing_format instance with empty config
BOXING_START_TEST(afs_boxing_format_load_xml_test5)
{
    mxml_node_t * tree = get_xml_instance("afs_boxing_format_save_empty_config_file_test.xml");

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_xml(boxing_format, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, NULL);

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_load_xml with the xml data for afs_boxing_format instance with config "4kv6"
BOXING_START_TEST(afs_boxing_format_load_xml_test6)
{
    mxml_node_t * tree = get_xml_instance("afs_boxing_format_save_config_file_4kv6_not_compact.xml");

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_xml(boxing_format, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv6");

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_load_xml with the xml data for afs_boxing_format instance with config "4kv7"
BOXING_START_TEST(afs_boxing_format_load_xml_test7)
{
    mxml_node_t * tree = get_xml_instance("afs_boxing_format_save_config_file_4kv7_not_compact.xml");

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_xml(boxing_format, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv7");

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_load_xml with the xml data for afs_boxing_format instance with config "4kv8"
BOXING_START_TEST(afs_boxing_format_load_xml_test8)
{
    mxml_node_t * tree = get_xml_instance("afs_boxing_format_save_config_file_4kv8_not_compact.xml");

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_xml(boxing_format, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv8");

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_load_xml with the xml data for afs_boxing_format instance with config "4kv9"
BOXING_START_TEST(afs_boxing_format_load_xml_test9)
{
    mxml_node_t * tree = get_xml_instance("afs_boxing_format_save_config_file_4kv9_not_compact.xml");

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_xml(boxing_format, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv9");

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_load_xml with the xml data for afs_boxing_format instance with config "4kv10"
BOXING_START_TEST(afs_boxing_format_load_xml_test10)
{
    mxml_node_t * tree = get_xml_instance("afs_boxing_format_save_config_file_4kv10_not_compact.xml");

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_xml(boxing_format, tree);

    BOXING_ASSERT(tree != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv10");

    afs_boxing_format_free(boxing_format);
    mxmlDelete(tree);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_save_string_test1)
{
    afs_boxing_format * boxing_format = NULL;

    char * result = afs_boxing_format_save_string(boxing_format, DTRUE);

    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == NULL);
}
END_TEST


// Test function afs_boxing_format_save_string with empty boxing_format instance
BOXING_START_TEST(afs_boxing_format_save_string_test2)
{
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    char * result = afs_boxing_format_save_string(boxing_format, DTRUE);
    
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, "<?xml version=\"1.0\" encoding=\"utf-8\"?><NULL />\n") == 0);

    afs_boxing_format_free(boxing_format);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with empty boxing_config instance in the boxing_format
BOXING_START_TEST(afs_boxing_format_save_string_test3)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_empty_config_file_test.xml");
    
    BOXING_ASSERT(test_string != NULL);

    afs_boxing_format * boxing_format = get_boxing_format_instance("");

    char * result = afs_boxing_format_save_string(boxing_format, DTRUE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format instance with config "4kv6" in compact format
BOXING_START_TEST(afs_boxing_format_save_string_test4)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv6_compact.xml");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv6");

    char * result = afs_boxing_format_save_string(boxing_format, DTRUE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format instance with config "4kv6" in not compact format
BOXING_START_TEST(afs_boxing_format_save_string_test5)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv6_not_compact.xml");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv6");

    char * result = afs_boxing_format_save_string(boxing_format, DFALSE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format instance with config "4kv7" in compact format
BOXING_START_TEST(afs_boxing_format_save_string_test6)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv7_compact.xml");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv7");

    char * result = afs_boxing_format_save_string(boxing_format, DTRUE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format instance with config "4kv7" in not compact format
BOXING_START_TEST(afs_boxing_format_save_string_test7)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv7_not_compact.xml");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv7");

    char * result = afs_boxing_format_save_string(boxing_format, DFALSE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format instance with config "4kv8" in compact format
BOXING_START_TEST(afs_boxing_format_save_string_test8)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv8_compact.xml");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv8");

    char * result = afs_boxing_format_save_string(boxing_format, DTRUE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format instance with config "4kv8" in not compact format
BOXING_START_TEST(afs_boxing_format_save_string_test9)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv8_not_compact.xml");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv8");

    char * result = afs_boxing_format_save_string(boxing_format, DFALSE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format instance with config "4kv9" in compact format
BOXING_START_TEST(afs_boxing_format_save_string_test10)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv9_compact.xml");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv9");

    char * result = afs_boxing_format_save_string(boxing_format, DTRUE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format instance with config "4kv9" in not compact format
BOXING_START_TEST(afs_boxing_format_save_string_test11)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv9_not_compact.xml");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv9");

    char * result = afs_boxing_format_save_string(boxing_format, DFALSE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format instance with config "4kv10" in compact format
BOXING_START_TEST(afs_boxing_format_save_string_test12)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv10_compact.xml");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv10");

    char * result = afs_boxing_format_save_string(boxing_format, DTRUE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_save_string with afs_boxing_format instance with config "4kv10" in not compact format
BOXING_START_TEST(afs_boxing_format_save_string_test13)
{
    char * test_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv10_not_compact.xml");

    afs_boxing_format * boxing_format = get_boxing_format_instance("4kv10");

    char * result = afs_boxing_format_save_string(boxing_format, DFALSE);

    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result != NULL);
    BOXING_ASSERT(strcmp(result, test_string) == 0);

    afs_boxing_format_free(boxing_format);
    free(test_string);
    free(result);
}
END_TEST


// Test function afs_boxing_format_load_string with string pointer equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_load_string_test1)
{
    char * input_string = NULL;
    afs_boxing_format * boxing_format = NULL;

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == DFALSE);
}
END_TEST


// Test function afs_boxing_format_load_string with string pointer equal to NULL and afs_boxing_format pointer not equal to NULL
BOXING_START_TEST(afs_boxing_format_load_string_test2)
{
    char * input_string = NULL;

    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_config_file(boxing_format, input_string);

    BOXING_ASSERT(input_string == NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_string with string pointer not equal to NULL and afs_boxing_format pointer equal to NULL
BOXING_START_TEST(afs_boxing_format_load_string_test3)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv10_not_compact.xml");
    afs_boxing_format * boxing_format = NULL;

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format == NULL);
    BOXING_ASSERT(result == DFALSE);

    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string with empty input string
BOXING_START_TEST(afs_boxing_format_load_string_test4)
{
    char * input_string = "";
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DFALSE);

    afs_boxing_format_free(boxing_format);
}
END_TEST


// Test function afs_boxing_format_load_string with input string from empty afs_boxing_format instance
BOXING_START_TEST(afs_boxing_format_load_string_test5)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_without_config_test.xml");
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_empty_afs_boxing_format(boxing_format);

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string with input string from afs_boxing_format instance with empty boxing_config
BOXING_START_TEST(afs_boxing_format_load_string_test6)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_empty_config_file_test.xml");
    afs_boxing_format * boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, NULL);

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string from the XML string from afs_boxing_format instance with config "4kv6" in compact format
BOXING_START_TEST(afs_boxing_format_load_string_test7)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv6_compact.xml");

    afs_boxing_format* boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv6");

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string from the XML string from afs_boxing_format instance with config "4kv6" in not compact format
BOXING_START_TEST(afs_boxing_format_load_string_test8)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv6_not_compact.xml");

    afs_boxing_format* boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv6");

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string from the XML string from afs_boxing_format instance with config "4kv7" in compact format
BOXING_START_TEST(afs_boxing_format_load_string_test9)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv7_compact.xml");

    afs_boxing_format* boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv7");

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string from the XML string from afs_boxing_format instance with config "4kv7" in not compact format
BOXING_START_TEST(afs_boxing_format_load_string_test10)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv7_not_compact.xml");

    afs_boxing_format* boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv7");

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string from the XML string from afs_boxing_format instance with config "4kv8" in compact format
BOXING_START_TEST(afs_boxing_format_load_string_test11)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv8_compact.xml");

    afs_boxing_format* boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv8");

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string from the XML string from afs_boxing_format instance with config "4kv8" in not compact format
BOXING_START_TEST(afs_boxing_format_load_string_test12)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv8_not_compact.xml");

    afs_boxing_format* boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv8");

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string from the XML string from afs_boxing_format instance with config "4kv9" in compact format
BOXING_START_TEST(afs_boxing_format_load_string_test13)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv9_compact.xml");

    afs_boxing_format* boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv9");

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string from the XML string from afs_boxing_format instance with config "4kv9" in not compact format
BOXING_START_TEST(afs_boxing_format_load_string_test14)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv9_not_compact.xml");

    afs_boxing_format* boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv9");

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string from the XML string from afs_boxing_format instance with config "4kv10" in compact format
BOXING_START_TEST(afs_boxing_format_load_string_test15)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv10_compact.xml");

    afs_boxing_format* boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv10");

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


// Test function afs_boxing_format_load_string from the XML string from afs_boxing_format instance with config "4kv10" in not compact format
BOXING_START_TEST(afs_boxing_format_load_string_test16)
{
    char * input_string = read_xml_config_file("afs_boxing_format_save_config_file_4kv10_not_compact.xml");

    afs_boxing_format* boxing_format = afs_boxing_format_create();

    DBOOL result = afs_boxing_format_load_string(boxing_format, input_string);

    BOXING_ASSERT(input_string != NULL);
    BOXING_ASSERT(boxing_format != NULL);
    BOXING_ASSERT(result == DTRUE);

    test_not_empty_afs_boxing_format(boxing_format, "4kv10");

    afs_boxing_format_free(boxing_format);
    free(input_string);
}
END_TEST


Suite * controlframe_tests(void)
{
    TCase * tc_controlframe_functions_tests = tcase_create("controlframe_functions_tests");

    // Test function afs_control_data_create
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_create_test1);
    // Test function afs_control_data_create2
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_create2_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_create2_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_create2_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_create2_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_create2_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_create2_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_create2_test7);
    // Test function afs_boxing_format_clone
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_clone_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_clone_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_clone_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_clone_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_clone_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_clone_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_clone_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_clone_test8);
    // Test function afs_boxing_format_get_new_reference
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_new_reference_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_new_reference_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_new_reference_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_new_reference_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_new_reference_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_new_reference_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_new_reference_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_new_reference_test8);
    // Test function afs_boxing_format_equal
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test8);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test9);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test10);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test11);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test12);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test13);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test14);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test15);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test16);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test17);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test18);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test19);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test20);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test21);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test22);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test23);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test24);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test25);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_equal_test26);
    // Test function afs_boxing_format_get_data_frames
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test8);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test9);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test10);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test11);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_get_data_frames_test12);
    // Test function afs_boxing_format_set_config
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test8);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test9);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test10);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test11);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_set_config_test12);
    // Test function afs_boxing_format_save_config_file
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test8);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test9);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test10);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test11);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test12);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test13);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test14);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_config_file_test15);
    // Test function afs_boxing_format_load_config_file
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test8);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test9);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test10);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test11);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test12);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test13);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test14);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test15);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_config_file_test16);
    // Test function afs_boxing_format_save_xml
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_xml_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_xml_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_xml_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_xml_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_xml_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_xml_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_xml_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_xml_test8);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_xml_test9);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_xml_test10);
    // Test function afs_boxing_format_load_xml
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_xml_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_xml_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_xml_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_xml_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_xml_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_xml_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_xml_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_xml_test8);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_xml_test9);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_xml_test10);
    // Test function afs_boxing_format_save_string
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test8);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test9);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test10);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test11);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test12);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_save_string_test13);
    // Test function afs_boxing_format_load_string
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test1);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test2);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test3);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test4);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test5);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test6);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test7);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test8);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test9);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test10);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test11);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test12);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test13);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test14);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test15);
    tcase_add_test(tc_controlframe_functions_tests, afs_boxing_format_load_string_test16);

    Suite * s = suite_create("controlframe_test_util");
    suite_add_tcase(s, tc_controlframe_functions_tests);

    return s;
}

