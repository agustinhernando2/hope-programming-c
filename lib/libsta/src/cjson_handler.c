#include <cjson_handler.h>

void print_cjson(char* cjson_buffer)
{
    char* string = NULL;
    cJSON* cjson_object = cJSON_Parse(cjson_buffer);
    string = cJSON_Print(cjson_object);
    printf("%s\n\n", string);
    cJSON_Delete(cjson_object);
    if (string == NULL)
    {
        fprintf(stderr, "%s:%d: Error printing json.\n", 
        __FILE__, __LINE__);
    }
}

char* get_value_of_key_from_json_string(char* cjson_buffer, char* key)
{
    if (cjson_buffer == NULL){
        fprintf(stderr, "%s:%d: Error null char*.\n", 
            __FILE__, __LINE__);
        return NULL;
    }
    cJSON* cjson_object = cJSON_Parse(cjson_buffer);
    return get_value_of_key_from_json_object(cjson_object, key);
}

char* get_value_of_key_from_json_object(cJSON* cjson_object, char* key)
{
    cJSON* json_value = cJSON_GetObjectItemCaseSensitive(cjson_object, key);
    if (cJSON_IsString(json_value) && (json_value->valuestring != NULL))
    {
        char* result = malloc(strlen(json_value->valuestring) + 1);
        if (result == NULL)
        {
            fprintf(stderr, "%s:%d: Error allocating memory using malloc.\n", 
            __FILE__, __LINE__);
            return NULL;
        }
        strcpy(result, json_value->valuestring);
        cJSON_Delete(json_value);
        return result;
    }
    cJSON_Delete(json_value);
    // sleep(10);
    return NULL;
}

int cjson_add_key_value_to_json_string(char* cjson_buffer, char* key, char* buffer)
{
    cJSON* cjson_object = NULL;
    
    if (buffer == NULL || key == NULL){
        fprintf(stderr, "%s:%d: Error params.\n", 
            __FILE__, __LINE__);
        return 1;
    }
    
    cjson_object = cJSON_Parse(cjson_buffer);
    if (cjson_object == NULL){
        cjson_object = cJSON_CreateObject();
    }
    return cjson_add_key_value_to_json_object(cjson_object, key, buffer, cjson_buffer);
}

int cjson_add_key_value_to_json_object(cJSON* cjson_object, char* key, char* buffer, char* cjson_buffer)
{
    cJSON* value = cJSON_CreateString(buffer);
    if (value == NULL)
    {
        fprintf(stderr, "%s:%d: Error adding a new key-value pair to JSON object. Unable to allocate memory.\n",
                __FILE__, __LINE__);
        return 1;
    }
    cJSON_AddItemToObject(cjson_object, key, value);

    json_object_to_json_string(cjson_object, cjson_buffer);
    return 0;
}

int json_object_to_json_string(cJSON* cjson_object, char* buffer)
{
    char* result = NULL;
    result = cJSON_Print(cjson_object);
    if (result == NULL)
    {
        fprintf(stderr, "%s:%d: Error getting char buffer json.\n", __FILE__, __LINE__);
        return 1;
    }
    strcpy(buffer, result);
    return 0;
}
