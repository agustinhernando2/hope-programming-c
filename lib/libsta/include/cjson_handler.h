/*preventing multiple inclusions of the same header file*/
#pragma once

/* Unix libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Libraries*/
#include <cJSON.h>

/**
 * @brief print a JSON character buffer
 *
 * @param [in] buffer JSON character buffer to print.
 */
void print_cjson(char* buffer);

/**
 * @brief Add key-value pair to a JSON object.
 *
 * This function adds a key-value pair to a JSON object.
 *
 * @param cjson_object Pointer to the JSON object to which the key-value pair will be added.
 * @param key Content of the key.
 * @param buffer Content of the value.
 * @return 0 if successful, 1 in case of an error.
 */
int cjson_add_key_value_to_json_object(cJSON* cjson_object, char* key, char* buffer, char* cjson_buffer);

/**
 * @brief Add key-value pair to a JSON character buffer.
 *
 * This function adds a key-value pair to a JSON character buffer.
 *
 * @param cjson_buffer Pointer to the JSON character buffer to which the key-value pair will be added.
 * @param key Content of the key.
 * @param buffer Content of the value.
 * @return 0 if successful, 1 in case of an error.
 */
int cjson_add_key_value_to_json_string(char* cjson_buffer, char* key, char* buffer);

/**
 * @brief - get value of a JSON object key
 *
 * @param cjson_object JSON object.
 * @param key Key to search for in the JSON object.
 * @return Value associated with the specified key. NULL if an error occurs.
 */
char* get_value_of_key_from_json_object(cJSON* cjson_object, char* key);

/**
 * @brief - get value of a JSON character buffer
 *
 * @param cjson_buffer JSON character buffer.
 * @param key Key to search for in the JSON.
 * @return Value associated with the specified key. NULL if an error occurs.
 */
char* get_value_of_key_from_json_string(char* cjson_buffer, char* key);

/**
 * @brief - parse JSON object to JSON character buffer
 *
 * @param cjson_object JSON object.
 * @param buffer receive an JSON character buffer.
 * @return 1 success. 1 if an error occurs.
 */
int json_object_to_json_string(cJSON* cjson_object, char* buffer);