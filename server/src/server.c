#include "server.h"

int main(int argc, char* argv[])
{
    set_signal_handlers();
    create_message_queue();
    int status;
    pid_t pid, pid_c;
    int child_process = 0;

    // Fork Modulo de alerta
    pid = fork();
    if (pid == 0)
    {
        run_alert_module(msg_id);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        child_process++;
    }
    else
    {
        fprintf(stderr, "Error creating process 2. Errno: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Fork listener alert temperature
    pid = fork();
    if (pid == 0)
    {
        alert_and_emergency_listener(msg_id);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        child_process++;
    }
    else
    {
        fprintf(stderr, "Error creating process 2. Errno: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    // Fork server connection ipv4 TCP
    pid = fork();
    if (pid == 0)
    {
        run_server_ipv4(SOCK_STREAM, AF_INET);
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

    // Fork server connection ipv4 UDP
    pid = fork();
    if (pid == 0)
    {
        run_server_ipv4(SOCK_DGRAM, AF_INET);
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

    // Fork server connection ipv6 TCP
    pid = fork();
    if (pid == 0)
    {
        run_server_ipv6(SOCK_STREAM, AF_INET6);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        child_process++;
    }
    else
    {
        fprintf(stderr, "Error creating process 2. Errno: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Fork server connection ipv6 UDP
    pid = fork();
    if (pid == 0)
    {
        run_server_ipv6(SOCK_DGRAM, AF_INET6);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        child_process++;
    }
    else
    {
        fprintf(stderr, "Error creating process 2. Errno: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    

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

int create_message_queue()
{
    // Create a unique key
    key_t key = ftok(K_MSG, 'm');
    if (key == -1) {
        error_handler("Error ftok", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    msg_id = msgget(key, IPC_CREAT | 0666);
    printf("QID = %d\n", msg_id);
    if (msg_id == -1)
    {
        error_handler("msgget error", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    return 0;
}

void alert_and_emergency_listener(int msg_id)
{
    char* message = NULL;
    mess_t send_buffer;

    int r = 0;
    while (TRUE)
    {
        if (msgrcv(msg_id, &send_buffer, sizeof(send_buffer), 1, 0) == -1)
        {
            perror("msgrcv error");
            exit(EXIT_FAILURE);
            continue;
        }
        printf("Mensaje recibido: %s\n", send_buffer.message);
    }
}

void run_server_ipv4(int type, int ipv)
{
    int clilen = 0, pid = 0;
    struct sockaddr_in cli_addr;

    if (connect_server_ipv4(&sockfd, type))
    {
        exit(EXIT_FAILURE);
    }
    switch (type)
    {
    case SOCK_STREAM:
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

                // Create pipe bethween parent and child
                if (pipe(pipe_fd) == -1)
                {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }

                // Create child process
                pid_t pid_ = fork();
                if (pid_ == -1)
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                else if (pid_ == 0)
                {
                    // emergency process
                    if (run_emergency_handler(pipe_fd))
                    {
                        error_handler("Error run_emergency_handler", __FILE__, __LINE__);
                        exit(EXIT_FAILURE);
                    }
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    // connection process
                    close(pipe_fd[1]);
                    run_admin_server(newsockfd);
                }
            }
            else
            {
                printf("SERVIDOR PID: %d, New client connected\n", pid);
                close(newsockfd);
            }
        }
        break;
    case SOCK_DGRAM:
        run_normal_user_server(sockfd, ipv);
        break;
    default:
        break;
    }
}

void run_server_ipv6(int type, int ipv)
{
    int clilen = 0, pid = 0;
    struct sockaddr_in6 cli_addr6;

    if (connect_server_ipv6(&sockfd, type))
    {
        exit(EXIT_FAILURE);
    }
    switch (type)
    {
    case SOCK_STREAM:
        while (TRUE)
        {
            newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr6, (socklen_t*)&clilen);
            if (newsockfd == -1)
            {
                error_handler("Error accept", __FILE__, __LINE__);
                exit(EXIT_FAILURE);
            }
            pid = fork();

            if (pid == 0)
            {
                close(sockfd);
                // Create pipe bethween parent and child
                if (pipe(pipe_fd) == -1)
                {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }

                // Create child process
                pid_t pid_ = fork();
                if (pid_ == -1)
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                else if (pid_ == 0)
                {

                    // emergency process
                    if (run_emergency_handler(pipe_fd))
                    {
                        error_handler("Error run_emergency_handler", __FILE__, __LINE__);
                        exit(EXIT_FAILURE);
                    }
                    exit(EXIT_SUCCESS);
                }
                else
                {

                    // connection process
                    close(pipe_fd[1]);
                    run_admin_server(newsockfd);
                }
            }
            else
            {
                printf("SERVIDOR PID: %d, New client connected\n", pid);
                close(newsockfd);
            }
        }
        break;
    case SOCK_DGRAM:
        run_normal_user_server(sockfd, ipv);
        break;
    default:
        break;
    }
}

void run_admin_server(int newsockfd)
{
    while (TRUE)
    {
        memset(recv_socket_buffer, 0, BUFFER_SIZE);
        if (recv_tcp_message(newsockfd, recv_socket_buffer))
        {
            error_handler("Error recv_tcp_message", __FILE__, __LINE__);
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

void run_normal_user_server(int newsockfd, int ipv)
{
    struct sockaddr_in cli_addr;

    while (TRUE)
    {
        // wait a moment to start to get the messages
        sleep(2);
        memset(recv_socket_buffer, 0, BUFFER_SIZE);
        if (recv_udp_message(newsockfd, recv_socket_buffer, &cli_addr))
        {
            error_handler("Error recv_udp_message", __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }
        printf("PROCESO %d. ", getpid());
        printf("Recibí: %s\n", recv_socket_buffer);

        switch (get_command())
        {
        case CLOSE_CONNECTION:
            end_conn(newsockfd);
            break;
        case OPTION1_:
            send_supply_udp(newsockfd, cli_addr);
            break;
        case OPTION2_:
            send_deneid_udp_message(newsockfd, cli_addr);
            break;
        default:
            fprintf(stdout, "Command error\n");
            break;
        };
    }
}

void send_supply_udp(int newsockfd, struct sockaddr_in cli_addr)
{   
    if (get_supply_status(&send_socket_buffer))
    {
        error_handler("Error get_supply_status", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    if (send_udp_message(newsockfd, send_socket_buffer, cli_addr))
    {
        error_handler("Error send_message", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    // Free memory
    free_ptr(&send_socket_buffer);
}

void send_deneid_udp_message(int newsockfd, struct sockaddr_in cli_addr)
{
    send_socket_buffer = (char*)malloc(BUFFER_SIZE);
    if (send_socket_buffer == NULL)
    {
        error_handler("Error allocating memory", __FILE__, __LINE__);
    }
    if (cjson_add_key_value_to_json_string(send_socket_buffer, K_ACC_DENEID, M_ACC_DENEID, OVERRIDE))
    {
        free_ptr(&send_socket_buffer);
        error_handler("Error json_add_key_value_to_json_string", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    if (send_udp_message(newsockfd, send_socket_buffer, cli_addr))
    {
        free_ptr(&send_socket_buffer);
        error_handler("Error send_message", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    free_ptr(&send_socket_buffer);
}

void end_conn(int newsockfd)
{
    // Configurar el extremo de lectura del pipe en modo no bloqueante
    int flags = fcntl(pipe_fd[0], F_GETFL);
    if (flags == -1)
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(pipe_fd[0], F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // Leer datos del pipe en el proceso padre
    ssize_t bytes_read;
    bytes_read = read(pipe_fd[0], buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("read");
        // Si no hay datos disponibles para leer, continuar sin bloquear
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            printf("No hay datos disponibles para leer en este momento.\n");
        }
        else
        {
            close(pipe_fd[0]);
            close(newsockfd);
            send_end_conn_message(newsockfd);
            exit(EXIT_FAILURE);
        }
    }
    else if (bytes_read > 0)
    {
        printf("Mensaje recibido en el proceso padre: %s\n", buffer);
    }

    close(pipe_fd[0]); // Cerrar el extremo de lectura del pipe en el padre

    fprintf(stdout, "Sending end connection, PID: %d.\n", getpid());
    send_end_conn_message(newsockfd);
    sleep(5);
    close(newsockfd);
    msgctl(msg_id, IPC_RMID, 0);
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
    if (cjson_add_key_value_to_json_string(send_socket_buffer, K_ACC_DENEID, TRUE_, OVERRIDE))
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
    if (cjson_add_key_value_to_json_string(send_socket_buffer, K_END, TRUE_, OVERRIDE))
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
    char* category = NULL;
    char* key = NULL;
    char* value = NULL;

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

static void sign_handler(int signal)
{   
    printf("Signal: %d\n", signal);
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
        end_conn(newsockfd);
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
