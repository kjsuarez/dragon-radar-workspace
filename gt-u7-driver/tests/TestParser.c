#include "unity.h"
#include "TestParser.h"
#include "parser.h"

void test_x(void){
    TEST_ASSERT_EQUAL_HEX8(0, 0);
}

void it_inits_line_state(void){
    GtU7LineState state = initGtU7LineState();
    TEST_ASSERT_EQUAL_INT(0, state.attrIndex);
}

void test_basic_int_types(void){
    const int32_t SCALE = 1000000;
    int32_t start_lat = 45586066;   // 45.586066N
    int32_t start_lon = -122751580; // -122.751580W
    int32_t end_lat = 45590000;     // 45.59N
    int32_t end_lon = -122760000;

    TEST_ASSERT_EQUAL_INT32(10, 10);
}

void it_parses_line_start(void){
    GtU7LineState state = initGtU7LineState();
    processGtU7Char(&state, '$');
    TEST_ASSERT_EQUAL_INT(1, state.online);
    TEST_ASSERT_EQUAL_INT(0, state.attrIndex);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("", state.attrBuffer, 1);
}

void it_parses_chars_to_buffer(void){
    GtU7LineState state = initGtU7LineState();
    char id[] = "$GPGLL";
    for (size_t i = 0; i < 6; i++){
        processGtU7Char(&state, id[i]);
    }
    TEST_ASSERT_EQUAL_INT(1, state.online);
    TEST_ASSERT_EQUAL_INT(0, state.attrIndex);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("GPGLL", state.attrBuffer, 5);
}

void it_parses_id(void){
    GtU7LineState state = initGtU7LineState();
    char id[] = "$GPGLL,";
    for (size_t i = 0; i < 7; i++){
        processGtU7Char(&state, id[i]);
    }
    TEST_ASSERT_EQUAL_INT(1, state.online);
    TEST_ASSERT_EQUAL_INT(1, state.attrIndex);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("", state.attrBuffer, 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("GPGLL", state.id, 5);
}

void it_parses_gpgll_lat(void){
    GtU7LineState state = initGtU7LineState();
    char id[] = "$GPGLL,4535.14272,";
    for (size_t i = 0; i < 18; i++){
        processGtU7Char(&state, id[i]);
    }
    TEST_ASSERT_EQUAL_INT(1, state.online);
    TEST_ASSERT_EQUAL_INT(2, state.attrIndex);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("", state.attrBuffer, 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("4535.14272", state.latBuffer, 10);
}

void it_parses_gpgll_lat_hemisphere(void){
    GtU7LineState state = initGtU7LineState();
    char id[] = "$GPGLL,4535.14272,N,";
    for (size_t i = 0; i < 20; i++){
        processGtU7Char(&state, id[i]);
    }
    // printf("5. attribute index: %hhu\n", state.attrIndex);
    // printf("hemisphere: %s\n", state.latHemi);
    TEST_ASSERT_EQUAL_INT(1, state.online);
    TEST_ASSERT_EQUAL_INT(3, state.attrIndex);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("", state.attrBuffer, 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("N", state.latHemi, 1);
}

void it_parses_gpgll_lon(void){
    GtU7LineState state = initGtU7LineState();
    char id[] = "$GPGLL,4535.14272,N,12245.10958,";
    for (size_t i = 0; i < 32; i++){
        processGtU7Char(&state, id[i]);
    }
    TEST_ASSERT_EQUAL_INT(1, state.online);
    TEST_ASSERT_EQUAL_INT(4, state.attrIndex);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("", state.attrBuffer, 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("12245.10958", state.lonBuffer, 11);
}

void it_parses_gpgll_lon_hemisphere(void){
    GtU7LineState state = initGtU7LineState();
    char id[] = "$GPGLL,4535.14272,N,12245.10958,W,";
    for (size_t i = 0; i < 34; i++){
        processGtU7Char(&state, id[i]);
    }
    TEST_ASSERT_EQUAL_INT(1, state.online);
    TEST_ASSERT_EQUAL_INT(5, state.attrIndex);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("", state.attrBuffer, 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("W", state.lonHemi, 1);
}

void it_parses_gpgll_line(void){
    GtU7LineState state = initGtU7LineState();
    char id[] = "$GPGLL,4535.14272,N,12245.10958,W,195237.00,A,A*70\n";
    for (size_t i = 0; i < 51; i++){
        processGtU7Char(&state, id[i]);
    }
    TEST_ASSERT_EQUAL_INT(0, state.online);
    TEST_ASSERT_EQUAL_INT(0, state.attrIndex);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("", state.attrBuffer, 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("GPGLL", state.id, 5);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("4535.14272", state.latBuffer, 10);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("N", state.latHemi, 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("12245.10958", state.lonBuffer, 11);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("W", state.lonHemi, 1);
}

void it_parses_gpgll_from_midline(void){
    GtU7LineState state = initGtU7LineState();
    char id[] = "7,53,265,32*4F\n$GPGLL,4535.14272,N,12245.10958,W,195237.00,A,A*70\n";
    for (size_t i = 0; i < 66; i++){
        processGtU7Char(&state, id[i]);
    }
    TEST_ASSERT_EQUAL_INT(0, state.online);
    TEST_ASSERT_EQUAL_INT(0, state.attrIndex);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("", state.attrBuffer, 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("GPGLL", state.id, 5);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("4535.14272", state.latBuffer, 10);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("N", state.latHemi, 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("12245.10958", state.lonBuffer, 11);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("W", state.lonHemi, 1);
}

void it_ignores_chars_when_off_line(void){
    GtU7LineState state = initGtU7LineState();
    char id[] = "7,53,265,32*4F";
    for (size_t i = 0; i < 14; i++){
        processGtU7Char(&state, id[i]);
    }
    TEST_ASSERT_EQUAL_INT(0, state.online);
    TEST_ASSERT_EQUAL_INT(0, state.attrIndex);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("", state.attrBuffer, 1);
}

void it_builds_position_from_gpgll_line(void){
    GtU7LineState state = initGtU7LineState();
    char id[] = "$GPGLL,4535.14272,N,12245.10958,W,195237.00,A,A*70\n";
    for (size_t i = 0; i < 51; i++){
        processGtU7Char(&state, id[i]);
    }
    Position pos = lineToPosition(&state);
    printf("floating latitude: %f\n", pos.lat_dec_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 45.585712, pos.lat_dec_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -122.75182, pos.lon_dec_deg);
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_x);
    RUN_TEST(it_inits_line_state);
    RUN_TEST(it_parses_line_start);
    RUN_TEST(it_parses_chars_to_buffer);
    RUN_TEST(it_parses_id);
    RUN_TEST(it_ignores_chars_when_off_line);
    // RUN_TEST(it_parses_gpgll_lat);
    // RUN_TEST(it_parses_gpgll_lat_hemisphere);
    // RUN_TEST(it_parses_gpgll_lon);
    // RUN_TEST(it_parses_gpgll_lon_hemisphere);
    RUN_TEST(it_parses_gpgll_line);
    RUN_TEST(it_parses_gpgll_from_midline);
    RUN_TEST(it_builds_position_from_gpgll_line);

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