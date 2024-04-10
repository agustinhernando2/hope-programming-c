// #include <logic.h>
#include <unity.h>
#include <cjson_handler.h>

void test_cjson_add_key_value_to_json_object(){
    char key[10] = "key";
    char buffer[10] = "buffer";
    char cjson_buffer[50];

    cJSON* cjson_object = NULL;

    TEST_ASSERT_EQUAL(0,cjson_add_key_value_to_json_object(cjson_object, key, buffer, cjson_buffer));
}
void test_example(void)
{
    TEST_ASSERT_EQUAL(1, 1);
    TEST_ASSERT_EQUAL(1, 1);
}

/**
 * @brief funcion que se llama antes de cada test.
 *
 * podrias aqui setear variables.
 *
 * @param[in].
 * @param[out].
 * @return.
 */
void setUp(void)
{
}

/**
 * @brief funcion que se llama despues de cada test.
 *
 * .
 *
 * @param[in].
 * @param[out].
 * @return.
 */
void tearDown(void)
{
}

int main()
{
    UNITY_BEGIN();
    // RUN_TEST(test_div);
    RUN_TEST(test_example);
    RUN_TEST(test_cjson_add_key_value_to_json_object);
    return UNITY_END();
}
