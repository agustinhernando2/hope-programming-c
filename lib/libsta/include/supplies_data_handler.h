#pragma once

#include <r_w_handler.h>

#define JSON_FILE_PATH "data/state_summary.json"

/**
 * @brief - get supplies json
 *      Load the supplies json file into the supplies_buffer.
 * @param supplies_buffer Empty supplies json pointer.
 * @return 0 if successful, 1 in case of an error.
 */
int get_supply_status(char** supplies_buffer);

/**
 * @brief - update supplies json
 *      Load the updated supplies json file into the supplies_buffer.
 * @param json_buffer Cointains the instructions to update the supplies.
 * @param supplies_buffer Empty supplies json pointer.
 * @return 0 if successful, 1 in case of an error.
 */
int set_supply_status(char* json_buffer, char* supplies_buffer);
