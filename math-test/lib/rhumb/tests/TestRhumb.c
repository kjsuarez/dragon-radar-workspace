#include "unity.h"
#include "rhumb.h"

void test_x(void){
    TEST_ASSERT_EQUAL_HEX8(0, 0);
}

void it_calculates_rhumb_in_NE_quadrant(void){
    float bearing;
    int distance;

    // Straight north
    calculateRhumb(45.0, 122.0, 45.6, 122.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(1, 6672, distance);
    // Straight south
    calculateRhumb(45.5, 122.0, 45.0, 122.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 180.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(1, 5560, distance);

    // Straight east
    calculateRhumb(45.0, 122.0, 45.0, 122.6, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(1, 4718, distance);
    // Straight west
    calculateRhumb(45.0, 122.0, 45.0, 121.6, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -90.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(1, 3145, distance);
}

void it_calculates_rhumb_crossing_NE_to_NW_across_antimeridian(void){
    float bearing;
    int distance;
    // East across the anti meridian
    calculateRhumb(45.0, 179.0, 45.0, -179.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(5, 15730, distance);
}

void it_calculates_rhumb_crossing_NE_to_NW_across_meridian(void){
    float bearing;
    int distance;

    // West across the prime meridian
    calculateRhumb(45.0, 1.0, 45.0, -1.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -90.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(5, 15730, distance);
}

void it_calculates_rhumb_crossing_NE_to_SE(void){
    float bearing;
    int distance;

    // South across equator
    calculateRhumb(1.0, 122.0, -1.0, 122.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 180.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(3, 22240, distance);
}

void it_calculates_rhumb_in_NW_quadrant(void){
    float bearing;
    int distance;
    // Straight west
    calculateRhumb(45.0, -122.0, 45.0, -122.6, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -90.0, bearing * (180.0 / M_PI));
    // Straight east
    calculateRhumb(45.0, -122.0, 45.0, -121.6, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, bearing * (180.0 / M_PI));
}

void it_calculates_rhumb_crossing_NW_to_NE_across_antimeridian(void){
    float bearing;
    int distance;
    // West across the antimeridian
    calculateRhumb(45.0, -179.0, 45.0, 179.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -90.0, bearing * (180.0 / M_PI));
}

void it_calculates_rhumb_crossing_NW_to_NE_across_meridian(void){
    float bearing;
    int distance;
    // East across the prime meridian
    calculateRhumb(45.0, -1.0, 45.0, 1.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, bearing * (180.0 / M_PI));
}

void it_calculates_rhumb_in_SW_quadrant(void){
    float bearing;
    int distance;
    // North across equator
    calculateRhumb(-1.0, 122.0, 1.0, 122.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, bearing * (180.0 / M_PI));
    
}

void it_calculates_rhumb_crossing_SW_to_SE_across_antimeridian(void){
    float bearing;
    int distance;
    // West across the antimeridian
    calculateRhumb(-45.0, -179.0, -45.0, 179.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -90.0, bearing * (180.0 / M_PI));
}

void it_calculates_rhumb_crossing_SW_to_SE_across_meridian(void){
    float bearing;
    int distance;
    // East across the prime meridian
    calculateRhumb(-45.0, -1.0, -45.0, 1.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, bearing * (180.0 / M_PI));
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_x);
    RUN_TEST(it_calculates_rhumb_in_NE_quadrant);
    RUN_TEST(it_calculates_rhumb_crossing_NE_to_NW_across_antimeridian);
    RUN_TEST(it_calculates_rhumb_crossing_NE_to_NW_across_meridian);
    RUN_TEST(it_calculates_rhumb_crossing_NE_to_SE);

    RUN_TEST(it_calculates_rhumb_in_NW_quadrant);
    RUN_TEST(it_calculates_rhumb_crossing_NW_to_NE_across_antimeridian);
    RUN_TEST(it_calculates_rhumb_crossing_NW_to_NE_across_meridian);

    RUN_TEST(it_calculates_rhumb_in_SW_quadrant);
    RUN_TEST(it_calculates_rhumb_crossing_SW_to_SE_across_antimeridian);
    RUN_TEST(it_calculates_rhumb_crossing_SW_to_SE_across_meridian);

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