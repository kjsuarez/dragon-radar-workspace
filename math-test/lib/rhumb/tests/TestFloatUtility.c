#include "unity.h"
#include "floatUtility.h"
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




int main(void){
    UNITY_BEGIN(); 
    RUN_TEST(test_positive_greater_than_one);
    RUN_TEST(test_positive_less_than_one);
    RUN_TEST(test_negative_more_than_one);
    RUN_TEST(test_negative_less_than_one);
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