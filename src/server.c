#include "server.h"
#include <stdio.h>

#include <emergency_handler.h>
#include <critical_handler.h>
#include <server_req_handler.h>
#include <cjson_handler.h>

//#include <cJSON.h>

int main(int argc, char const *argv[])
{
    printf("Hello, World \n");




    printf("Calling shared lib from main program. I get '%d'.\n", shared_lib_function("Hello, OS II"));
    printf("Calling static lib from main program. I get '%d'.\n", static_lib_function("Bye, OS II"));
    printf("Calling static lib from main program. I get '%d'.\n", static_lib_function_("Bye2, OS II"));
    printf("Calling static lib from main program. I get '%d'.\n", cjson_handler_lib_function("Hello CJSON"));


    cJSON *jsonMessage = cJSON_CreateObject();
    char *string = NULL;

    cjson_add_key_value_to_object(jsonMessage, "MESSAGE", "buffer1");

    string = cJSON_Print(jsonMessage);
    printf("%s\n\n", string);
    cJSON_Delete(jsonMessage);
    if (string == NULL)
    {
        perror("Failed 2.");
    }

    return 0;
}


