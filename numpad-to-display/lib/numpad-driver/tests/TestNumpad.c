#include "unity.h"
#include "numpad.h"

void it_knows_when_debounce_needed(void){
    uint8_t needed;
    // off before off now
    needed = debounceNeeded(0, 0);
    TEST_ASSERT_EQUAL_UINT8(needed, 0);
    // on before off now
    needed = debounceNeeded(0, '1');
    TEST_ASSERT_NOT_EQUAL_UINT8(needed, 0);
    // off before on now
    needed = debounceNeeded('2', 0);
    TEST_ASSERT_NOT_EQUAL_UINT8(needed, 0);
    // '3' before '3' now
    needed = debounceNeeded('3', '3');
    TEST_ASSERT_EQUAL_UINT8(needed, 0);
    // two distinct non-zero chars
    needed = debounceNeeded('1', '3');
    TEST_ASSERT_NOT_EQUAL_UINT8(needed, 0);
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(it_knows_when_debounce_needed);
}

void setUp(void)
{
    // Run before every test
}

void tearDown(void)
{
    // Run after every test
}