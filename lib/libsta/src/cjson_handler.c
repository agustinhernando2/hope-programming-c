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
        fprintf(stderr, "%s:%d: Error printing json.\n", __FILE__, __LINE__);
    }
}

int get_value_of_key_from_json_string(char* cjson_buffer, char* key, char** ptr_buffer)
{
    if (cjson_buffer == NULL)
    {
        fprintf(stderr, "%s:%d: Error null char*.\n", __FILE__, __LINE__);
        return 1;
    }
    cJSON* cjson_object = cJSON_Parse(cjson_buffer);
    if (cjson_object == NULL)
    {
        fprintf(stderr, "%s:%d: Error null cjson*.\n", __FILE__, __LINE__);
        return 1;
    }
    return get_value_of_key_from_json_object(cjson_object, key, ptr_buffer);
}

int get_value_of_key_from_json_object(cJSON* cjson_object, char* key, char** ptr_buffer)
{   
    char* buffer = NULL;
    cJSON* json_value = cJSON_GetObjectItemCaseSensitive(cjson_object, key);
    if (json_value == NULL)
    {
        return 1;
    }
    // if the value is a string
    if (cJSON_IsString(json_value) && (json_value->valuestring != NULL))
    {
        buffer = (char*)malloc(strlen(json_value->valuestring) + 1);
        if (buffer == NULL)
        {
            fprintf(stderr, "%s:%d: Error allocating memory.\n", __FILE__, __LINE__);
            return 1;
        }
        // copy value to buffer
        strcpy(buffer, json_value->valuestring);
        *ptr_buffer = buffer;
        cJSON_Delete(json_value);
        return 0;
    }
    // if the value is an object
    if(cJSON_IsObject(json_value)){
    // create buffer
        buffer = (char*)malloc(strlen(cJSON_Print(json_value)) + 1);
        if (buffer == NULL)
        {
            fprintf(stderr, "%s:%d: Error allocating memory.\n", __FILE__, __LINE__);
            return 1;
        }
        // copy value to buffer
        strcpy(buffer, cJSON_Print(json_value));
        *ptr_buffer = buffer;
        cJSON_Delete(json_value);
        return 0;
    }
    return 0;
}

int cjson_add_key_value_to_json_string(char* cjson_buffer, char* key, char* buffer)
{
    cJSON* cjson_object;
    cJSON* value;

    if (buffer == NULL || key == NULL)
    {
        fprintf(stderr, "%s:%d: Error params.\n", __FILE__, __LINE__);
        return 1;
    }

    // parse json string or create a new object
    cjson_object = cJSON_Parse(cjson_buffer);
    if (cjson_object == NULL)
    {
        cjson_object = cJSON_CreateObject();
        if (cjson_object == NULL)
        {
            fprintf(stderr, "%s:%d: Error: Unable to create JSON object.\n", __FILE__, __LINE__);
            return 1;
        }
    }

    // create a new json value
    value = cJSON_CreateString(buffer);
    if (value == NULL)
    {
        cJSON_Delete(cjson_object); 
        fprintf(stderr, "%s:%d: Error: Unable to create JSON string value.\n", __FILE__, __LINE__);
        return 1;
    }

    if (cjson_add_key_item_to_json_object(cjson_object, key, value, cjson_buffer))
    {
        cJSON_Delete(cjson_object); 
        return 1;
    }

    // value is already in the object, the free is not necessary
    cJSON_Delete(cjson_object);
    return 0;
}



int cjson_add_key_object_to_json_string(char* cjson_buffer, char* key, char* cjson_buffer_value)
{
    cJSON* cjson_object = NULL;

    if (cjson_buffer_value == NULL || key == NULL)
    {
        fprintf(stderr, "%s:%d: Error params.\n", __FILE__, __LINE__);
        return 1;
    }

    cjson_object = cJSON_Parse(cjson_buffer);
    if (cjson_object == NULL)
    {
        cjson_object = cJSON_CreateObject();
    }
    cJSON* value = cJSON_Parse(cjson_buffer_value);
    if(cjson_add_key_item_to_json_object(cjson_object, key, value, cjson_buffer))
    {
        return 1;
    }

    // value is already in the object, the free is not necessary
    cJSON_Delete(cjson_object);
    return 0;
}

int cjson_add_key_item_to_json_object(cJSON* cjson_object, char* key, cJSON* value, char* cjson_buffer)
{
    if (value == NULL)
    {
        fprintf(stderr, "%s:%d: Error adding a new key-value pair to JSON object. Unable to allocate memory.\n",
                __FILE__, __LINE__);
        return 1;
    }
    
    if(is_key_in_json_object(cjson_object, key)){
        fprintf(stderr, "%s:%d: Error the key is already exists.\n",
                __FILE__, __LINE__);
        return 0;
    }
    cJSON_AddItemToObject(cjson_object, key, value);

    json_object_to_json_string(cjson_object, cjson_buffer);
    return 0;
}

int json_object_to_json_string(cJSON* cjson_object, char* buffer)
{
    if (buffer == NULL) {
        fprintf(stderr, "Error: buffer is NULL\n");
        return 1;
    }

    char* result = cJSON_Print(cjson_object);
    if (result == NULL)
    {
        fprintf(stderr, "%s:%d: Error getting char buffer json.\n", __FILE__, __LINE__);
        return 1;
    }
    strcpy(buffer, result);
    free(result);
    return 0;
}

int is_key_in_json_object(cJSON* cjson_object, char* key)
{
    cJSON* json_value = cJSON_GetObjectItemCaseSensitive(cjson_object, key);
    if (json_value == NULL)
    {
        return 0;
    }
    return 1;
}

int is_key_in_json_buffer(char* cjson_buffer, char* key)
{   
    cJSON* cjson_object;

    if (cjson_buffer == NULL || key == NULL)
    {
        fprintf(stderr, "%s:%d: Error params.\n", __FILE__, __LINE__);
        return 0;
    }

    // parse json string or create a new object
    cjson_object = cJSON_Parse(cjson_buffer);
    if (cjson_object == NULL)
    {
        fprintf(stderr, "%s:%d: Error: cJSON_Parse.\n", __FILE__, __LINE__);
        return 0;
    }
    // check if the key is in the object
    int r = is_key_in_json_object(cjson_object, key);

    cJSON_Delete(cjson_object);

    return r;
}

int merge_json_strings(char* json_string1, char* json_string2, char* merged_json)
{
    if (json_string1 == NULL || json_string2 == NULL || merged_json == NULL)
    {
        fprintf(stderr, "%s:%d: Error null char*.\n", __FILE__, __LINE__);
        return 1;
    }
    
    cJSON* json_object1 = cJSON_Parse(json_string1);
    cJSON* json_object2 = cJSON_Parse(json_string2);
    
    if (json_object1 == NULL || json_object2 == NULL)
    {
        fprintf(stderr, "%s:%d: Error parsing json objects.\n", __FILE__, __LINE__);
        return 1;
    }
    
    cJSON* merged_object = cJSON_Duplicate(json_object1, 1);
    
    if (merged_object == NULL)
    {
        fprintf(stderr, "%s:%d: Error duplicating json object.\n", __FILE__, __LINE__);
        return 1;
    }
    
    cJSON* current_item = NULL;
    /* Macro for iterating over an array or object */
    cJSON_ArrayForEach(current_item, json_object2)
    {   
        // check if the key is already in the merged object
        if(is_key_in_json_object(merged_object, current_item->string))
        {
            cJSON_DeleteItemFromObject(merged_object, current_item->string);
        }
        cJSON_AddItemToObject(merged_object, current_item->string, cJSON_Duplicate(current_item, 1));
    }
    
    char* merged_string = cJSON_Print(merged_object);
    
    if (merged_string == NULL)
    {
        fprintf(stderr, "%s:%d: Error printing merged json.\n", __FILE__, __LINE__);
        cJSON_Delete(merged_object);
        return 1;
    }
    
    strcpy(merged_json, merged_string);
    
    cJSON_Delete(merged_object);
    free(merged_string);
    
    return 0;
}