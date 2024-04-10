#include "server.h"

int main(int argc, char const* argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Uso: %s <puerto> <1/0 ipv4/ipv6 respectively>\n", argv[0]);
        return 0;
    }
    // INTERNET SERVER
    puerto = (uint16_t)atoi(argv[1]);
    uint8_t ipv4 = (uint8_t)atoi(argv[2]);
    run_server(ipv4);
    // END
    exit(EXIT_SUCCESS);
}

void run_server(uint8_t ipv4)
{
    int sockfd;
    int newsockfd, clilen, pid;
    struct sockaddr_in cli_addr;

    if (connect_server(sockfd, ipv4))
    {
        exit(EXIT_FAILURE);
    }

    while (TRUE)
    {
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, (socklen_t*)&clilen);

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

int connect_server(int sockfd, uint8_t ipv4)
{
    struct sockaddr_in serv_addr;
    struct sockaddr_in6 serv_addr6;

    if (ipv4)
    {
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            fprintf(stderr, "%s:%d: Error socket.\n", __FILE__, __LINE__);
            return 1;
        }

        memset((char*)&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons((uint16_t)puerto);

        if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        {
            fprintf(stderr, "%s:%d: Bind error server unix. %s\n", __FILE__, __LINE__, strerror(errno));
            return 1;
        }

        printf("Proceso: %d - socket available: %d\n", getpid(), ntohs(serv_addr.sin_port));

        if ((listen(sockfd, SOMAXCONN)) == -1)
        {
            fprintf(stderr, "%s:%d: Error LISTEN: %s\n", __FILE__, __LINE__, strerror(errno));
            return 1;
        }
        return 0;
    }
    else
    {
        if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
        {
            fprintf(stderr, "%s:%d: Error socket.\n", __FILE__, __LINE__);
            return 1;
        }

        memset((char*)&serv_addr6, 0, sizeof(serv_addr6));
        serv_addr6.sin6_family = AF_INET6;
        serv_addr6.sin6_addr = in6addr_any;
        serv_addr6.sin6_port = htons((uint16_t)puerto);

        if (bind(sockfd, (struct sockaddr*)&serv_addr6, sizeof(serv_addr6)) < 0)
        {
            fprintf(stderr, "%s:%d: Bind error server ipv6. %s\n", __FILE__, __LINE__, strerror(errno));
            return 1;
        }

        printf("Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr6.sin6_port));

        if (listen(sockfd, SOMAXCONN) == -1)
        {
            fprintf(stderr, "%s:%d: Error LISTEN: %s\n", __FILE__, __LINE__, strerror(errno));
            return 1;
        }
        return 0;
    }
}

static void sign_handler()
{
    if (flag_handler)
    {
        // close(sockfd);
        exit(EXIT_SUCCESS);
    }
    flag_handler = 1;
    printf("\
    The server is not accepting any more connections.\
    \nLas mensajes que reciba ahora retornan fin y cierran los clientes.\n");
}
