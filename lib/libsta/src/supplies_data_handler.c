#include <supplies_data_handler.h>

int get_supply_status(char** supplies_buffer){
    return read_file(JSON_FILE_PATH, supplies_buffer);
}

int set_supply_status(char* category, char* key, char* value, char** supplies_buffer)
{
    char* value_of_supply_key = NULL ;
    char* value_of_category = NULL ;

    if (read_file(JSON_FILE_PATH, supplies_buffer)){
        return 1;
    }
    
    get_value_of_key_from_json_string(*supplies_buffer, SUPPLIES, &value_of_supply_key);
    get_value_of_key_from_json_string(value_of_supply_key, category, &value_of_category);
    cjson_add_key_value_to_json_string(value_of_category, key, value);
    printf("\nset_supply_status: %s\n", *supplies_buffer);
    printf("\nvalue_of_category: %s\n", value_of_category);
    // return write_file(JSON_FILE_PATH, supplies_buffer);
    free(value_of_category);
    return 0;
}
