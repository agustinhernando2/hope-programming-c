#include <supplies_data_handler.h>

int get_supply_status(char** supplies_buffer){
    return read_file(JSON_FILE_PATH, supplies_buffer);
}

int set_supply_status(char* json_buffer, char* supplies_buffer){
    return write_file(JSON_FILE_PATH, supplies_buffer);
}