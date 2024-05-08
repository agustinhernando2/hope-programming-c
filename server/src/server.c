#include "server.h"

int main(int argc, char* argv[])
{
    set_signal_handlers();
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
        error_handler("Error creating process 1", __FILE__, __LINE__); 
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
    int clilen, pid;
    struct sockaddr_in cli_addr;

    if (connect_server_ipv4(&sockfd, argv))
    {
        exit(EXIT_FAILURE);
    }

    while (TRUE)
    {
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, (socklen_t*)&clilen);
        if (newsockfd == -1)
        {
            error_handler("Error accept", __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }
        pid = fork();

        if (pid == 0)
        {   
            close(sockfd);

            // Create pipe
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            // Crate child process
            pid_t pid_ = fork();
            if (pid_ == -1) {
                // Error 
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid_ == 0) {
                // child
                if(run_emergency_handler(pipe_fd))
                {
                    error_handler("Error run_emergency_handler", __FILE__, __LINE__);
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            } else {
                // parent
                close(pipe_fd[1]);  // Close write end
                run_server(newsockfd);
            }
        }
        else
        {
            printf("SERVIDOR PID: %d, New client connected\n", pid);
            close(newsockfd);
        }
    }
}
void run_server(int newsockfd)
{   
    while (TRUE)
    {
        memset(recv_socket_buffer, 0, BUFFER_SIZE);
        if (recv_message(newsockfd, recv_socket_buffer))
        {
            error_handler("Error recv_message", __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }
        printf("PROCESO %d. ", getpid());
        printf("Recibí: %s\n", recv_socket_buffer);
        sleep(1);

        switch (get_command())
        {
        case CLOSE_CONNECTION:
            end_conn(newsockfd);
            break;
        case OPTION1_:
            send_supply_message(newsockfd);
            break;
        case OPTION2_:
            if (check_credentials())
            {
                send_deneid_message(newsockfd);
                break;
            }
            set_and_send_suply_status(newsockfd);
            break;

        default:
            fprintf(stdout, "Command error\n");
            break;
        };
    }
}

void end_conn(int newsockfd)
{
    // Configurar el extremo de lectura del pipe en modo no bloqueante
    int flags = fcntl(pipe_fd[0], F_GETFL);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(pipe_fd[0], F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // Leer datos del pipe en el proceso padre
    ssize_t bytes_read;
    bytes_read = read(pipe_fd[0], buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("read");
        // Si no hay datos disponibles para leer, continuar sin bloquear
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("No hay datos disponibles para leer en este momento.\n");
        } else {
            close(pipe_fd[0]); 
            close(newsockfd);
            send_end_conn_message(newsockfd);
            exit(EXIT_FAILURE);
        }
    } else if (bytes_read > 0) {
        printf("Mensaje recibido en el proceso padre: %s\n", buffer);
    }

    close(pipe_fd[0]); // Cerrar el extremo de lectura del pipe en el padre

    fprintf(stdout, "Sending end connection, PID: %d.\n", getpid());
    send_end_conn_message(newsockfd);
    sleep(5);
    close(newsockfd);
    // send end of connection to parent pid
    kill(getppid(), SIGTSTP);
    exit(EXIT_SUCCESS);
}

void set_and_send_suply_status(int newsockfd)
{
    if (set_supply())
    {
        error_handler("Error set_supply", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    if (send_message(send_socket_buffer, strlen(send_socket_buffer), newsockfd))
    {
        error_handler("Error send_message", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    // Free memory
    free_ptr(&send_socket_buffer);
}

void send_supply_message(int newsockfd)
{
    if (get_supply_status(&send_socket_buffer))
    {
        error_handler("Error get_supply_status", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    if (send_message(send_socket_buffer, strlen(send_socket_buffer), newsockfd))
    {
        error_handler("Error send_message", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    // Free memory
    free_ptr(&send_socket_buffer);
}

void send_deneid_message(int newsockfd)
{   
    send_socket_buffer = (char*)malloc(BUFFER_SIZE);
    if (send_socket_buffer == NULL)
    {
        error_handler("Error allocating memory", __FILE__, __LINE__);
    }
    if(cjson_add_key_value_to_json_string(send_socket_buffer, K_ACC_DENEID, TRUE_, OVERRIDE))
    {
        free_ptr(&send_socket_buffer);
        error_handler("Error json_add_key_value_to_json_string", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    if (send_message(send_socket_buffer, strlen(send_socket_buffer), newsockfd))
    {
        free_ptr(&send_socket_buffer);
        error_handler("Error send_message", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    free_ptr(&send_socket_buffer);
}

void send_end_conn_message(int newsockfd)
{   
    send_socket_buffer = (char*)malloc(BUFFER_SIZE);
    if (send_socket_buffer == NULL)
    {   
        error_handler("Error allocating memory", __FILE__, __LINE__);
    }
    if(cjson_add_key_value_to_json_string(send_socket_buffer, K_END, TRUE_, OVERRIDE))
    {
        free_ptr(&send_socket_buffer);
        error_handler("Error json_add_key_value_to_json_string", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    if (send_message(send_socket_buffer, strlen(send_socket_buffer), newsockfd))
    {
        free_ptr(&send_socket_buffer);
        error_handler("Error send_message", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    free_ptr(&send_socket_buffer);
}

int set_supply()
{
    char* category;
    char* key;
    char* value;

    if (get_value_of_key_from_json_string(recv_socket_buffer, K_SUP_COMMAND_EQ, &category))
    {
        free_ptr(&category);
        error_handler("Error get_value_of_key_from_json_string", __FILE__, __LINE__);
        return 1;
    }
    if (get_value_of_key_from_json_string(recv_socket_buffer, K_KEY, &key))
    {
        free_ptr(&key);
        error_handler("Error get_value_of_key_from_json_string", __FILE__, __LINE__);
        return 1;
    }
    if (get_value_of_key_from_json_string(recv_socket_buffer, K_VALUE, &value))
    {
        free_ptr(&value);
        error_handler("Error get_value_of_key_from_json_string", __FILE__, __LINE__);
        return 1;
    }
    if (set_supply_status(category, key, value, &send_socket_buffer))
    {
        error_handler("Error set_supply_status", __FILE__, __LINE__);
        return 1;
    }
    free_ptr(&category);
    free_ptr(&key);
    free_ptr(&value);
    return 0;
}
void make_deneid_message()
{
    fprintf(stdout, "Only the administrator can modify the data.\n");
    if (get_supply_status(&send_socket_buffer))
    {
        error_handler("Error get_supply_status", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    cjson_add_key_value_to_json_string(send_socket_buffer, K_ACC_DENEID, TRUE_, OVERRIDE);
}

int check_credentials()
{
    char* password;
    char* username;

    if (get_value_of_key_from_json_string(recv_socket_buffer, K_HOSTNAME, &username))
    {
        free_ptr(&username);
        error_handler("Error get_value_of_key_from_json_string", __FILE__, __LINE__);
        return 1;
    }
    if (get_value_of_key_from_json_string(recv_socket_buffer, K_PASSWORD, &password))
    {
        free_ptr(&password);
        error_handler("Error get_value_of_key_from_json_string", __FILE__, __LINE__);
        return 1;
    }
    if (strcmp(username, ADMIN) == 0 && strcmp(password, ADMIN) == 0)
    {
        free_ptr(&password);
        free_ptr(&username);
        return 0;
    }
    free_ptr(&password);
    free_ptr(&username);
    return 1;
}

int get_command()
{
    if (flag_handler)
    {   
        return 0;
    }
    char* c_buffer;
    if (get_value_of_key_from_json_string(recv_socket_buffer, K_COMMAND, &c_buffer))
    {
        free_ptr(&c_buffer);
        error_handler("Error get_value_of_key_from_json_string", __FILE__, __LINE__);
        return -1;
    }
    int r = atoi(c_buffer);
    free_ptr(&c_buffer);
    return r;
}

int connect_server_unix(int* sockfd, char* argv[])
{
    struct sockaddr_un serv_addr;
    uint16_t puerto;

    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error_handler("Error socket", __FILE__, __LINE__);
        return 1;
    }

    unlink(argv[2]);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, argv[2]);

    if (bind(*sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error_handler("Error bind", __FILE__, __LINE__);
        return 1;
    }
    printf("Proceso: %d - socket disponible: %s\n", getpid(), serv_addr.sun_path);

    if ((listen(*sockfd, 100)) == -1)
    {
        error_handler("Error listen", __FILE__, __LINE__);
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
        error_handler("Error socket", __FILE__, __LINE__);
        return 1;
    }

    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    puerto = (uint16_t)atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons((uint16_t)puerto);

    if (bind(*sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error_handler("Error bind", __FILE__, __LINE__);
        return 1;
    }

    printf("Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr.sin_port));

    if ((listen(*sockfd, 100)) == -1)
    {
        error_handler("Error listen", __FILE__, __LINE__);
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
        error_handler("Error socket", __FILE__, __LINE__);
        return 1;
    }

    memset((char*)&serv_addr6, 0, sizeof(serv_addr6));
    puerto = (uint16_t)atoi(argv[1]);
    serv_addr6.sin6_family = AF_INET6;
    serv_addr6.sin6_addr = in6addr_any;
    serv_addr6.sin6_port = htons((uint16_t)puerto);

    if (bind(*sockfd, (struct sockaddr*)&serv_addr6, sizeof(serv_addr6)) < 0)
    {
        error_handler("Error bind", __FILE__, __LINE__);
        return 1;
    }

    printf("Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr6.sin6_port));

    if (listen(*sockfd, 100) == -1)
    {
        error_handler("Error listen", __FILE__, __LINE__);
        return 1;
    }

    return 0;
}

static void sign_handler(int signal)
{
    switch (signal)
    {
    case SIGINT:
        /* out of the loop*/
        printf("SIGINT called\n");
        flag_handler = 1;
        printf("\
            The server is not accepting any more connections.\
            \nThe received messages will close the client connection.\n");
        end_conn(newsockfd);
        break;
    case SIGTSTP:
        /* send end to client */
        printf("SIGTSTP called\n");
        printf("\
            The server connection is close.\n");
        sleep(5);
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }
    return;
}

void set_signal_handlers()
{
    struct sigaction sa;
    sa.sa_handler = sign_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("Couldn't set SIGINT handler");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
    {
        perror("Couldn't set SIGTSTP handler");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        perror("Couldn't set SIGTERM handler");
        exit(EXIT_FAILURE);
    }
}
