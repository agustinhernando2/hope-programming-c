#include <unity.h>
#include <cjson_handler.h>
#include <lib_handler.h>

void test_cjson_add_key_value_to_json_string(){
    char cjson_buffer[50];
    char key[] = "key";
    char value[] = "value";

    TEST_ASSERT_EQUAL(0, cjson_add_key_value_to_json_string(cjson_buffer, key, value, OVERRIDE));
    TEST_ASSERT_EQUAL_STRING("{\n\t\"key\":\t\"value\"\n}", cjson_buffer);

}

void test_cjson_add_key_object_to_json_string(){
    char cjson_buffer[50];
    char key[] = "key";
    char cjson_buffer_value[] = "{\"key\":\"value\"}";
    
    TEST_ASSERT_EQUAL(0, cjson_add_key_value_to_json_string(cjson_buffer, key, cjson_buffer_value, OVERRIDE|OBJPARSE));
    TEST_ASSERT_EQUAL_STRING("{\n\t\"key\":\t{\n\t\t\"key\":\t\"value\"\n\t}\n}", cjson_buffer);
}


void test_cjson_add_key_object_to_json_string_2(){
    char key[] = "key_added";
    char cjson_buffer[500]="{\"key_root1\":\"value\",\"key_root2\":\"value\"}";
    char cjson_buffer_value[] = "{\"key1\":\"value\",\"key2\":\"value\"}";
    
    TEST_ASSERT_EQUAL(0, cjson_add_key_value_to_json_string(cjson_buffer, key, cjson_buffer_value, OVERRIDE|OBJPARSE));
    TEST_ASSERT_EQUAL_STRING("{\n\t\"key_root1\":\t\"value\",\n\t\"key_root2\":\t\"value\",\n\t\"key_added\":\t{\n\t\t\"key1\":\t\"value\",\n\t\t\"key2\":\t\"value\"\n\t}\n}", cjson_buffer);
}

void test_is_key_in_json_object()
{
    // Create a cJSON object
    cJSON* cjson_object = cJSON_CreateObject();
    cJSON* cjson_object_empty = cJSON_CreateObject();

    cJSON_AddStringToObject(cjson_object, "key1", "value1");
    cJSON_AddStringToObject(cjson_object, "key2", "value2");

    // Test if the key is in the JSON object
    TEST_ASSERT_EQUAL(1, is_key_in_json_object(cjson_object, "key1"));
    TEST_ASSERT_EQUAL(1, is_key_in_json_object(cjson_object, "key2"));

    // Test if the key is not in the JSON object
    TEST_ASSERT_EQUAL(0, is_key_in_json_object(cjson_object, "key3"));
    TEST_ASSERT_EQUAL(0, is_key_in_json_object(cjson_object_empty, "key3"));

    // Clean up the cJSON object
    cJSON_Delete(cjson_object);
}

void test_get_value_of_key_from_json_string()
{
    char cjson_buffer[] = "{\"key1\":\"value1\",\"key2\":{\"nested_key\":\"nested_value\"}}";
    char key1[] = "key1";
    char key2[] = "key2";
    char* ptr_buffer = NULL;

    // Test getting a value from the top-level key
    TEST_ASSERT_EQUAL(0, get_value_of_key_from_json_string(cjson_buffer, key1, &ptr_buffer));
    TEST_ASSERT_EQUAL_STRING("value1", ptr_buffer);
    free_ptr(&ptr_buffer);

    // Test getting a value from a nested key
    TEST_ASSERT_EQUAL(0, get_value_of_key_from_json_string(cjson_buffer, key2, &ptr_buffer));
    TEST_ASSERT_EQUAL_STRING("{\n\t\"nested_key\":\t\"nested_value\"\n}", ptr_buffer);
    free_ptr(&ptr_buffer);

    // Test getting a value from a non-existent key
    TEST_ASSERT_EQUAL(1, get_value_of_key_from_json_string(cjson_buffer, "nonexistent_key", &ptr_buffer));
    free_ptr(&ptr_buffer);

    // Test getting a value and save it in a pointer
    TEST_ASSERT_EQUAL(0, get_value_of_key_from_json_string(cjson_buffer, key1, &ptr_buffer));
    TEST_ASSERT_EQUAL_STRING("value1", ptr_buffer);
    free_ptr(&ptr_buffer);
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
    RUN_TEST(test_is_key_in_json_object);
    RUN_TEST(test_get_value_of_key_from_json_string);
    
    return UNITY_END();
}
