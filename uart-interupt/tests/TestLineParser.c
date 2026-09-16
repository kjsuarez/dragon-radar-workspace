#include "unity.h"
#include <stdlib.h>
#include "lineParser.h"

// void test_substring_to_float(void){
//     // This is a placeholder test function
//     Position p;
//     char line[] = "xxx4523.4545xxx";
//     parseLine(line, &p);
//     printf("Minutes: %f\n", p.longitude);
//     // printf("Degrees: %d, Minutes: %f\n", degrees, minutes);
//     // TEST_ASSERT_EQUAL_UINT8(45, degrees);
//     TEST_ASSERT_FLOAT_WITHIN(0.00001, 23.4545, p.longitude);
// }

void test_invalid_position_line(void){
    Position p;
    char line[] = "$GPGLL,,,,,,V,N*64";
    parseLine(line, &p, 18);
    TEST_ASSERT_EQUAL_UINT8(0, p.latitude);
    TEST_ASSERT_EQUAL_UINT8(0, p.longitude);
    TEST_ASSERT_EQUAL_UINT8(0, p.status);
}

void test_partially_filled_line(void){

}

void test_valid_position_line(void){
    Position p;
    char line[] = "$GPGLL,4535.14779,N,12245.11480,W,200526.00,A,A*7F";
    parseLine(line, &p, 50);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 45.5857965, p.latitude);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -122.7519133, p.longitude);
    TEST_ASSERT_EQUAL_UINT8(192, p.status); // 11000000
}

void test_valid_course_over_ground_line(void){
    Position p;
    char line[] = "$GPVTG,77.52,T,,M,0.004,N,0.008,K,A*06";
    parseLine(line, &p, 38);
    
}

int main(void)
{
    UNITY_BEGIN();
    // RUN_TEST(test_invalid_position_line);
    RUN_TEST(test_valid_position_line);
    return UNITY_END();
}

void setUp(void)
{
    // Run before every test
}

void tearDown(void){}