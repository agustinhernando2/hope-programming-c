#include "server.h"

int main(int argc, char *argv[])
{
    signal(SIGINT, sign_handler);
    signal(SIGTERM, sign_handler);
    int status;
    pid_t pid, pid_c;
    int child_process = 0;

    if (argc < 3)
    {
        fprintf(stderr, "Uso: %s <puerto> <unix_name>\n", argv[0]);
        return 0;
    }

    pid = fork();
    if (pid == 0)
    {
        run_server_ipv4(argv);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        child_process++;
    }
    else
    {
        fprintf(stderr, "Error creating process 1. Errno: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // pid = fork();
    // if (pid == 0)
    // {
    //     run_server_ipv6(argv);
    //     exit(EXIT_SUCCESS);
    // }
    // else if (pid > 0)
    // {
    //     child_process++;
    // }
    // else
    // {
    //     fprintf(stderr,"Error creating process 2. Errno: %s\n", strerror(errno));
    //     exit(EXIT_FAILURE);
    // }

    // pid = fork();
    // if (pid == 0)
    // {
    //     run_server_bluetooth();
    //     exit(EXIT_SUCCESS);
    // }
    // else if (pid > 0)
    // {
    //     child_process++;
    // }
    // else
    // {
    //     fprintf(stderr,"Error creating process 3. Errno: %s\n", strerror(errno));
    //     exit(EXIT_FAILURE);
    // }

    while (pid && child_process)
    {
        pid_c = wait(&status);
        child_process--;
        if (WIFEXITED(status))
        {
            printf("Child %i, status: %i\n", pid_c, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Finished child %i, status:%i\n", pid_c, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("Error child %i, status %i\n", pid_c, WSTOPSIG(status));
        }
    }

    exit(EXIT_SUCCESS);
}

void run_server_ipv4(char* argv[])
{
    char socket_buffer[BUFFER_SIZE];
    int sockfd;
    int newsockfd, clilen, pid;
    struct sockaddr_in cli_addr;

    if (connect_server_ipv4(&sockfd, argv))
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

            while (TRUE)
            {
                memset(socket_buffer, 0, BUFFER_SIZE);
                if (recv_message(newsockfd, socket_buffer))
                {
                    fprintf(stderr, "NULL socket_buffer, errno: %s", strerror(errno));
                }
                printf("PROCESO %d. ", getpid());
                printf("Recibí: %s\n", socket_buffer);
                sleep(1);
                if (send_message(socket_buffer, BUFFER_SIZE, newsockfd)){

                    exit(EXIT_FAILURE);
                }

                if (flag_handler)
                {
                    exit(EXIT_SUCCESS);
                }
            }
        }
        else
        {
            printf("SERVIDOR PID: %d, New client connected\n", pid);
            close(newsockfd);
        }
    }
}
int connect_server_unix(int* sockfd, char* argv[])
{
    struct sockaddr_un serv_addr;
    uint16_t puerto;

    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "%s:%d: Error socket.\n", __FILE__, __LINE__);
        return 1;
    }

    unlink(argv[2]);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, argv[2]);

    if (bind(*sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "%s:%d: Bind error server unix. errno: %s\n", __FILE__, __LINE__, strerror(errno));
        return 1;
    }
    printf("Proceso: %d - socket disponible: %s\n", getpid(), serv_addr.sun_path);

    if ((listen(*sockfd, 100)) == -1)
    {
        fprintf(stderr, "%s:%d: Error LISTEN: %s\n", __FILE__, __LINE__, strerror(errno));
        return 1;
    }
    return 0;
}
int connect_server_ipv4(int* sockfd, char* argv[])
{
    struct sockaddr_in serv_addr;
    uint16_t puerto;

    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "%s:%d: Error socket.\n", __FILE__, __LINE__);
        return 1;
    }

    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    puerto = (uint16_t)atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons((uint16_t)puerto);

    if (bind(*sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "%s:%d: Bind error server unix. IPv4: %s\n", __FILE__, __LINE__, strerror(errno));
        return 1;
    }

    printf("Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr.sin_port));

    if ((listen(*sockfd, 100)) == -1)
    {
        fprintf(stderr, "%s:%d: Error LISTEN: %s\n", __FILE__, __LINE__, strerror(errno));
        return 1;
    }
    return 0;
}
int connect_server_ipv6(int* sockfd, char* argv[])
{
    struct sockaddr_in6 serv_addr6;
    uint16_t puerto;

    if ((*sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "%s:%d: Error socket.\n", __FILE__, __LINE__);
        return 1;
    }

    memset((char*)&serv_addr6, 0, sizeof(serv_addr6));
    puerto = (uint16_t)atoi(argv[1]);
    serv_addr6.sin6_family = AF_INET6;
    serv_addr6.sin6_addr = in6addr_any;
    serv_addr6.sin6_port = htons((uint16_t)puerto);

    if (bind(*sockfd, (struct sockaddr*)&serv_addr6, sizeof(serv_addr6)) < 0)
    {
        fprintf(stderr, "%s:%d: Bind error server ipv6. %s\n", __FILE__, __LINE__, strerror(errno));
        return 1;
    }

    printf("Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr6.sin6_port));

    if (listen(*sockfd, 100) == -1)
    {
        fprintf(stderr, "%s:%d: Error LISTEN: %s\n", __FILE__, __LINE__, strerror(errno));
        return 1;
    }
    return 0;
}

static void sign_handler()
{
    if (flag_handler)
    {
        close(sockfd);
        exit(EXIT_SUCCESS);
    }
    flag_handler = 1;
    printf("\
    The server is not accepting any more connections.\
    \nThe received messages will close the client connection.\n");
}
