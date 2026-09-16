#include "unity.h"
#include "floatUtility.h"
#include "trigUtility.h"
#include <string.h>

void test_positive_greater_than_one(void)
{
    char output[50];

    floatToString(1.23456, output, 1000, 3);
    TEST_ASSERT_EQUAL_STRING("1.234", output);
    
    output[0] = '\0';
    floatToString(11.23456, output, 1000, 3);
    TEST_ASSERT_EQUAL_STRING("11.234", output);

    output[0] = '\0';
    floatToString(78.53981, output, 1000, 3);
    TEST_ASSERT_EQUAL_STRING("78.539", output);
}

void test_positive_less_than_one(void){
    char output[50];

    output[0] = '\0';
    floatToString(0.23456, output, 1000, 3);
    TEST_ASSERT_EQUAL_STRING("0.234", output);

    output[0] = '\0';
    floatToString(0.0123, output, 1000, 3);
    TEST_ASSERT_EQUAL_STRING("0.012", output);

    output[0] = '\0';
    floatToString(0.45678, output, 1000, 3);
    TEST_ASSERT_EQUAL_STRING("0.456", output);
}

void test_negative_less_than_one(void){
    char output[50];

    output[0] = '\0';
    floatToString(-0.456, output, 1000, 3);
    TEST_ASSERT_EQUAL_STRING("-0.456", output);
}

void test_negative_more_than_one(void){
    char output[50];

    output[0] = '\0';
    floatToString(-123.456, output, 1000, 3);
    TEST_ASSERT_EQUAL_STRING("-123.456", output);
}

void test_rhumb_bearing_NE_hemisphere(void){
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

    // South across equator
    calculateRhumb(1.0, 122.0, -1.0, 122.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 180.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(3, 22240, distance);

    // Straight east
    calculateRhumb(45.0, 122.0, 45.0, 122.6, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(1, 4718, distance);
    // Straight west
    calculateRhumb(45.0, 122.0, 45.0, 121.6, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -90.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(1, 3145, distance);

    // West across the prime meridian
    calculateRhumb(45.0, 1.0, 45.0, -1.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -90.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(5, 15730, distance);
    // East across the anti meridian
    calculateRhumb(45.0, 179.0, 45.0, -179.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, bearing * (180.0 / M_PI));
    TEST_ASSERT_INT_WITHIN(5, 15730, distance);
}

void test_rhumb_bearing_NW_hemisphere(void){
    float bearing;
    int distance;
    // Straight west
    calculateRhumb(45.0, -122.0, 45.0, -122.6, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -90.0, bearing * (180.0 / M_PI));
    // Straight east
    calculateRhumb(45.0, -122.0, 45.0, -121.6, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, bearing * (180.0 / M_PI));

    // West across the antimeridian
    calculateRhumb(45.0, -179.0, 45.0, 179.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -90.0, bearing * (180.0 / M_PI));
    // East across the prime meridian
    calculateRhumb(45.0, -1.0, 45.0, 1.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, bearing * (180.0 / M_PI));
}

void test_rhumb_bearing_SW_hemisphere(void){
    float bearing;
    int distance;
    // North across equator
    calculateRhumb(-1.0, 122.0, 1.0, 122.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, bearing * (180.0 / M_PI));

    // West across the antimeridian
    calculateRhumb(-45.0, -179.0, -45.0, 179.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -90.0, bearing * (180.0 / M_PI));
    // East across the prime meridian
    calculateRhumb(-45.0, -1.0, -45.0, 1.0, &bearing, &distance);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 90.0, bearing * (180.0 / M_PI));
}

int main(void){
    UNITY_BEGIN(); 
    RUN_TEST(test_positive_greater_than_one);
    RUN_TEST(test_positive_less_than_one);
    RUN_TEST(test_negative_more_than_one);
    RUN_TEST(test_negative_less_than_one);
    RUN_TEST(test_rhumb_bearing_NW_hemisphere);
    RUN_TEST(test_rhumb_bearing_NE_hemisphere);
    RUN_TEST(test_rhumb_bearing_SW_hemisphere);
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