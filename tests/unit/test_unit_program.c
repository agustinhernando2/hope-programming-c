// #include <logic.h>
#include <unity.h>
#include <cjson_handler.h>

void test_cjson_add_key_value_to_json_string(){
    char key[] = "key";
    char value[] = "value";
    char cjson_buffer[50];

    TEST_ASSERT_EQUAL(0, cjson_add_key_value_to_json_string(cjson_buffer, key, value));
    TEST_ASSERT_EQUAL_STRING("{\n\t\"key\":\t\"value\"\n}", cjson_buffer);

}

void test_cjson_add_key_object_to_json_string(){
    char key[] = "key";
    char cjson_buffer[50];
    char cjson_buffer_value[] = "{\"key\":\"value\"}";
    
    TEST_ASSERT_EQUAL(0, cjson_add_key_object_to_json_string(cjson_buffer, key, cjson_buffer_value));
    TEST_ASSERT_EQUAL_STRING("{\n\t\"key\":\t{\n\t\t\"key\":\t\"value\"\n\t}\n}", cjson_buffer);
}


void test_cjson_add_key_object_to_json_string_2(){
    char key[] = "key_added";
    char cjson_buffer[500]="{\"key_root1\":\"value\",\"key_root2\":\"value\"}";
    char cjson_buffer_value[] = "{\"key1\":\"value\",\"key2\":\"value\"}";
    
    TEST_ASSERT_EQUAL(0, cjson_add_key_object_to_json_string(cjson_buffer, key, cjson_buffer_value));
    TEST_ASSERT_EQUAL_STRING("{\n\t\"key_root1\":\t\"value\",\n\t\"key_root2\":\t\"value\",\n\t\"key_added\":\t{\n\t\t\"key1\":\t\"value\",\n\t\t\"key2\":\t\"value\"\n\t}\n}", cjson_buffer);
}

/**
 * @brief run at the end of tests.
 */
void setUp(void)
{
}

/**
 * @brief run at the begining of tests.
 */
void tearDown(void)
{
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_cjson_add_key_object_to_json_string);
    RUN_TEST(test_cjson_add_key_value_to_json_string);
    RUN_TEST(test_cjson_add_key_object_to_json_string_2);
    return UNITY_END();
}
