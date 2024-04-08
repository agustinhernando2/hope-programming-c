#include <client.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Uso %s <host> <puerto>\n", argv[0]);
        exit(EXIT_SUCCESS);
    }
    puerto = (uint16_t)atoi(argv[2]);
    server = gethostbyname(argv[1]);

    if(try_connect_server())
    {
        fprintf(stderr, "%s:%d: Error to connect: %s\n", __FILE__, __LINE__,strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (TRUE)
    {
        memset(socket_buffer, '\0', BUFFER_SIZE);
        fgets(socket_buffer, BUFFER_SIZE - 1, stdin);
        socket_buffer[strlen(socket_buffer) - 1] = '\0';
 
        // Concatena la nueva cadena a la izquierda de la cadena original
        // if(!send_message(socket_buffer, sockfd,1)){
        //     fprintf(stderr, "%s:%d: Error send_message. errno: %s\n", __FILE__, __LINE__,strerror(errno));
        // }

        // Verificando si se escribió: fin
        // if (!strcmp(FIN, socket_buffer))
        // {
        //     printf("Finalizando ejecución\n");
        //     exit(EXIT_SUCCESS);
        // }
        // if (read_response(sockfd))
        // {
        //     fprintf(stderr, "%s:%d: Error read_response. errno: %s\n", __FILE__, __LINE__,strerror(errno));
        //     exit(EXIT_SUCCESS);
        // }
    }
    return 0;
}

int try_connect_server()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset((char *)&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    /*h_addr_list direccion IP a la que se va a conectar el servidor*/
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, (size_t) server->h_length);
    serv_addr.sin_port = htons((uint16_t)puerto);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        return 1;
    }
    return 0;

}

// int read_response(int sockfd){
//     if(print_message_from_socket(sockfd)){
//         fprintf(stderr, "%s:%d: Error print_message_from_socket: %s\n", __FILE__, __LINE__,strerror(errno));
//         return 1;
//     }
//     return 0;
// }
