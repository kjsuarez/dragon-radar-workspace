#include "unity.h"
#include "TestParser.h"
#include "parser.h"

void test_x(void){
    TEST_ASSERT_EQUAL_HEX8(0, 0);
}

void test_input_state_initialization(void){
    GtU7LineState state = initGtU7LineState();
    TEST_ASSERT_EQUAL_INT(-1, state.charIndex);
}

void test_basic_int_types(void){
    const int32_t SCALE = 1000000;
    int32_t start_lat = 45586066;   // 45.586066N
    int32_t start_lon = -122751580; // -122.751580W
    int32_t end_lat = 45590000;     // 45.59N
    int32_t end_lon = -122760000;

    TEST_ASSERT_EQUAL_INT32(10, 10);
}

void test_known_bearing_1(void){
    int32_t start_lat = 45586066; // 45.586066N
    int32_t start_lon = -122751580; // -122.751580W
    int32_t end_lat = 45590000; // 45.59N
    int32_t end_lon = -122760000; // -122.76W
    uint16_t expected_bearing = 30373; // 303.731667°
    int32_t actual_bearing = bearing(start_lat, start_lon, end_lat, end_lon);
    TEST_ASSERT_EQUAL_INT32(expected_bearing, actual_bearing);
}

void test_basic_fixed_point_assertions(void){
    int32_t start_lat = 45586066;   // 45.586066N
    int32_t start_lon = -122751580; // -122.751580W
    int32_t end_lat = 45590000;     // 45.59N
    int32_t end_lon = -122760000;
    const int32_t SCALE = 1000000;
    const int32_t PI = 3141592;
    const int32_t DEG_TO_RAD = PI / 180;
    TEST_ASSERT_EQUAL_INT32(((end_lon - start_lon) * DEG_TO_RAD), 0);
}

    int
    main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_x);
    RUN_TEST(test_input_state_initialization);
    RUN_TEST(test_basic_fixed_point_assertions);
    // RUN_TEST(test_known_bearing_1);
    return UNITY_END();
}

void setUp(void)
{
    // Run before every test
}

void tearDown(void)
{
    // Run after every test
}