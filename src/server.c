#include "server.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        return 0;
    }
    // INTERNET SERVER
    puerto = (uint16_t)atoi(argv[1]);
    run_server();
    // END
    exit(EXIT_SUCCESS);
}

void run_server()
{
    int newsockfd, clilen, pid;
    struct sockaddr_in cli_addr;

    if (connect_server())
    {
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

        pid = fork();

        if (pid == 0)
        { // Proceso hijo
            close(sockfd);

            // while (1)
            // {
            //     recv_command(newsockfd, socket_buffer);
            //     if (socket_buffer == NULL)
            //     {
            //         fprintf(stderr,"NULL socket_buffer, errno: %s",strerror(errno));
            //     }
            //     printf("PROCESO %d. ", getpid());
            //     printf("Recibí: %s", socket_buffer);
            //     printf("\n");
            //     // Verificación de si hay que terminar
            //     if (!strcmp("journalctl fin", socket_buffer))
            //     {
            //         printf("PROCESO %d. Como recibí 'fin', termino la ejecución.\n\n", getpid());
            //         exit(0);
            //     }
            //     if (!(send_response(socket_buffer, newsockfd)))
            //         exit(EXIT_FAILURE);

            //     if(flag_handler){
            //         exit(EXIT_SUCCESS);
            //     }
            // }
            printf("HelloW\n");
        }
        else
        {
            printf("SERVIDOR PID: %d, New client connected\n", pid);
            close(newsockfd);
        }
    }
}

int connect_server()
{
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "%s:%d: Error socket.\n", __FILE__, __LINE__);
        return 1;
    }

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons((uint16_t)puerto);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "%s:%d: Bind error server unix. %s\n", __FILE__, __LINE__, strerror(errno));
        return 1;
    }

    printf("Proceso: %d - socket available: %d\n", getpid(), ntohs(serv_addr.sin_port));

    if ((listen(sockfd, N_CONNECTIONS)) == -1)
    {
        fprintf(stderr, "%s:%d: Error LISTEN: %s\n", __FILE__, __LINE__, strerror(errno));
        return 1;
    }
    return 0;
}



//copia el argumento a ip para anexar la interfaz de red activa a ipv6
// strcpy(ip, argv[1]);
// if(strchr(ip, ':') != NULL) {
//     set_interface_ipv6(ip, iface);
// }

// void get_interface_ipv6(char* ip, char* iface){


//     //obtengo la lista de interfaz de red
//     if (getifaddrs(&ifaddr) == -1) {
//         perror("getifaddrs");
//         exit(EXIT_FAILURE);
//     }

//     // Recorre la lista de interfaces y busca una que esté activa y tenga una dirección IPv6
//     for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
//         if (ifa->ifa_addr == NULL || strcmp(ifa->ifa_name, "lo") == 0)
//             continue;
//         if(ifa->ifa_addr->sa_family==AF_INET6){
//             if (s != 0){
//                 printf("getnameinfo() failed: %s\n", gai_strerror(s));
//                 exit(EXIT_FAILURE);
//             }
//             iface = ifa->ifa_name;
//             break;
//         }
//     }

//     if(iface != NULL) {
//         //verifica si se trata de una IPv6 de enlace local
//         if(strncmp(ip, "fe80::",6) == 0){
//             //concateno la interfaz de red de enlace local
//             strcat(ip, "%");
//             strcat(ip, iface);
//         }
//     }
// }

static void sign_handler()
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
