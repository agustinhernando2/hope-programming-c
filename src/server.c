#include "server.h"

int main(int argc, char const *argv[])
{
    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    printf("Hello, World \n");


    memset(socket_buffer, 0, BUFFER_SIZE);
    cjson_add_key_value_to_json_string(socket_buffer,"clave1","value");
    cjson_add_key_value_to_json_string(socket_buffer,"clave2","value");
    cjson_add_key_value_to_json_string(socket_buffer,"clave3","value");
    cjson_add_key_value_to_json_string(socket_buffer,"clave4","value");
    cjson_add_key_value_to_json_string(socket_buffer,"clave5","value");
    print_cjson(socket_buffer);

    return 0;
}


static void handler()
{
    if(flag_handler){
        close(sockfd);
        exit(EXIT_SUCCESS);
    }
    flag_handler = 1;
    printf("\
    The server is not accepting any more connections.\
    \nLas mensajes que reciba ahora retornan fin y cierran los clientes.\n");
}
