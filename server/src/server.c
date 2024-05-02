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
        { 
            close(sockfd);

            run_server(newsockfd);
        }
        else
        {
            printf("SERVIDOR PID: %d, New client connected\n", pid);
            close(newsockfd);
        }
    }
}
void run_server(int newsockfd){
    char socket_buffer[BUFFER_SIZE];
    char* supplies_buffer = NULL;
    while (TRUE)
    {
        memset(socket_buffer, 0, BUFFER_SIZE);
        if (recv_message(newsockfd, socket_buffer))
        {
            fprintf(stderr, "NULL socket_buffer, errno: %s", strerror(errno));
        }
        printf("PROCESO %d. ", getpid());
        printf("Recibí: %s\n", socket_buffer);
        // sleep(1);
        // if (send_message(socket_buffer, BUFFER_SIZE, newsockfd)){
        //     exit(EXIT_FAILURE);
        // }

        switch (get_command(socket_buffer))
        {
        case OPTION1_:
            if(get_supply_status(&supplies_buffer)){
                fprintf(stderr, "%s:%d: Error get_supply_status.\n", __FILE__, __LINE__);
                exit(EXIT_FAILURE);
            }
            if (send_message(supplies_buffer, strlen(supplies_buffer), newsockfd)){
                fprintf(stderr, "%s:%d: Error send_message.\n", __FILE__, __LINE__);
                exit(EXIT_FAILURE);
            }
            // Free memory
            free_ptr(&supplies_buffer);
            break;
        case OPTION2_:
            if (check_credentials(socket_buffer))
            {
                make_deneid_message(supplies_buffer);
                if (send_message(supplies_buffer, strlen(supplies_buffer), newsockfd)){
                    fprintf(stderr, "%s:%d: Error send_message.\n", __FILE__, __LINE__);
                    exit(EXIT_FAILURE);
                }
                break;
            }
            if(set_supply_status(socket_buffer,supplies_buffer)){
                fprintf(stderr, "%s:%d: Error get_supply_status.\n", __FILE__, __LINE__);
                exit(EXIT_FAILURE);
            }
            if (send_message(supplies_buffer, strlen(supplies_buffer), newsockfd)){
                fprintf(stderr, "%s:%d: Error send_message.\n", __FILE__, __LINE__);
                exit(EXIT_FAILURE);
            }
            // Free memory
            free_ptr(&supplies_buffer);
            break;
        
        default:
            fprintf(stdout, "Command error\n");
            break;
        };
        
        if (flag_handler)
        {
            exit(EXIT_SUCCESS);
        }
    }
}

void make_deneid_message(char* supplies_buffer)
{
    fprintf(stdout, "Only the administrator can modify the data.\n");
    if(get_supply_status(&supplies_buffer)){
        fprintf(stderr, "%s:%d: Error get_supply_status.\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    cjson_add_key_value_to_json_string(supplies_buffer, K_ACC_DENEID, TRUE_);
}

int check_credentials(char* socket_buffer)
{
    char password[MAX_PASSWORD_LENGTH];
    char username[MAX_USERNAME_LENGTH];

    if(get_value_of_key_from_json_string(socket_buffer, K_HOSTNAME, username))
    {
        fprintf(stderr, "%s:%d: Error get_value_of_key_from_json_string.\n", __FILE__, __LINE__);
        return 1;
    }
    if(get_value_of_key_from_json_string(socket_buffer, K_PASSWORD, password))
    {
        fprintf(stderr, "%s:%d: Error get_value_of_key_from_json_string.\n", __FILE__, __LINE__);
        return 1;
    }
    if (strcmp(username, ADMIN) == 0 && strcmp(password, ADMIN) == 0)
    {
        return 0;
    }
    return 1;
}

int get_command(char* socket_buffer)
{
    char c_buffer[C_SIZE];
    if(get_value_of_key_from_json_string(socket_buffer, K_COMMAND, c_buffer))
    {
        fprintf(stderr, "%s:%d: Error get_value_of_key_from_json_string.\n", __FILE__, __LINE__);
        return 0;
    }
    return atoi(c_buffer);
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
