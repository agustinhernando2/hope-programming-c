#include <cjson_handler.h>
#include <stdio.h>
#include <string.h>

int cjson_handler_lib_function(char* msg)
{
    printf("Hi, I'm a cjson_handler lib that receives '%s', and return '%ld'.\n", msg, strlen(msg));
    return strlen(msg);
}
