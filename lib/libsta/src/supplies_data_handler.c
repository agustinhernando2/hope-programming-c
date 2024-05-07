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
    // get the value of the key supplies, ex {"supplies":{...}}
    get_value_of_key_from_json_string(*supplies_buffer, SUPPLIES, &value_of_supply_key);
    // get the value of the key category, ex {"food":{...}}
    get_value_of_key_from_json_string(value_of_supply_key, category, &value_of_category);
    // add the key-value pair to the category object, ex {"food":{"water":120 ...}} and override if already exist
    cjson_add_key_value_to_json_string(value_of_category, key, value, OVERRIDE|INTPARSE);
    // update the category object
    cjson_add_key_value_to_json_string(value_of_supply_key, category, value_of_category, OVERRIDE|OBJPARSE);
    // update the supplies object
    cjson_add_key_value_to_json_string(*supplies_buffer, SUPPLIES, value_of_supply_key, OVERRIDE|OBJPARSE);

    // free the memory
    // free_ptr(&value_of_category);
    free_ptr(&value_of_supply_key);

    // write the updated supplies object to the file
    return write_file(JSON_FILE_PATH, *supplies_buffer);
}
