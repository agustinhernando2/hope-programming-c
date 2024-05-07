#include <client.h>

int main(int argc, char* argv[])
{
    set_signal_handlers();

    if (argc < 3)
    {
        fprintf(stderr, "Uso %s <host> <puerto>\n", argv[0]);
        exit(EXIT_SUCCESS);
    }
    puerto = (uint16_t)atoi(argv[2]);
    server = gethostbyname(argv[1]);

    if (try_connect_server())
    {
        fprintf(stderr, "%s:%d: Error to connect: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    get_credentials();

    while (TRUE)
    {
        get_options();
        add_credentials();
        send_message_to_server();
        recv_and_check_message();
    }
    return 0;
}

void send_message_to_server()
{
    if (send_message(send_socket_buffer, BUFFER_SIZE, sockfd))
    {
        fprintf(stderr, "%s:%d: Error send_message. errno: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int try_connect_server()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset((char*)&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    /*h_addr_list direccion IP a la que se va a conectar el servidor*/
    bcopy((char*)server->h_addr_list[0], (char*)&serv_addr.sin_addr.s_addr, (size_t)server->h_length);
    serv_addr.sin_port = htons((uint16_t)puerto);
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        return 1;
    }
    return 0;
}
int recv_and_check_message()
{
    memset(recv_socket_buffer, 0, BUFFER_SIZE);
    recv_message(sockfd, recv_socket_buffer);

    if (is_key_in_json_buffer(recv_socket_buffer, K_ACC_DENEID))
    {
        clear_screen();
        // print message
        fprintf(stdout, "Access denied\n");
        sleep(2);
    }
    if (is_key_in_json_buffer(recv_socket_buffer, K_ACC_DENEID))
    {
        end_client_conn();
    }
    // show message
    clear_screen();
    print_cjson(recv_socket_buffer);

    while(flag_get_supply == 1)
    {
        // in loop to get supply until SIGINT is ejecuted
        fprintf(stdout, "\n\tCTRL + C to back\n");
        send_message_to_server();
        recv_and_check_message();
    }
    
    return 0;
}
void end_client_conn()
{
    close(sockfd);
    clear_screen();
    // print message
    fprintf(stdout, "\n***Connection closed, terminating execution...***\n");
    sleep(1);
    exit(EXIT_SUCCESS);
}
void get_credentials()
{
    printf("Enter your username: ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    username[strlen(username) - 1] = '\0';

    printf("Enter your password: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strlen(password) - 1] = '\0';
}
void add_credentials()
{
    cjson_add_key_value_to_json_string(send_socket_buffer, K_HOSTNAME, username, OVERRIDE);
    cjson_add_key_value_to_json_string(send_socket_buffer, K_PASSWORD, password, OVERRIDE);
}

void get_options()
{
    int option;
    memset(send_socket_buffer, 0, BUFFER_SIZE);
    int flag;
    do
    {
        clear_screen();
        flag = 0;
        printf("Please enter an option (1 or 2):\n");
        fprintf(stdout, "1. %s\n", OPTION1_EQ);
        fprintf(stdout, "2. %s\n", OPTION2_EQ);
        fprintf(stdout, "3. %s\n", OPTION_END_EQ);
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            fprintf(stdout, "You have selected: %s.\n", OPTION1_EQ);
            flag_get_supply = 1;
            cjson_add_key_value_to_json_string(send_socket_buffer, K_COMMAND, OPTION1, OVERRIDE);
            cjson_add_key_value_to_json_string(send_socket_buffer, K_COMMAND_EQ, OPTION1_EQ, OVERRIDE);
            break;
        case 2:
            fprintf(stdout, "You have selected: %s.\n", OPTION2_EQ);
            cjson_add_key_value_to_json_string(send_socket_buffer, K_COMMAND, OPTION2, OVERRIDE);
            cjson_add_key_value_to_json_string(send_socket_buffer, K_COMMAND_EQ, OPTION2_EQ, OVERRIDE);
            get_supplies_options();
            break;
        case OPTION_END:
            fprintf(stdout, "You have selected: %s.\n", OPTION_END_EQ);
            end_client_conn();
            break;
        default:
            printf("Invalid option. Please enter a valid number.\n");
            flag=1;
            break;
        }
    }while (flag);
}

void get_supplies_options()
{
    int option;
    int value;
    char key[MAX_K_V_SUP_LENGHT];

    int flag;
    do
    {
        clear_screen();
        flag = 0;
        printf("Please enter an option (1 or 2):\n");
        fprintf(stdout, "1. %s\n", SUP_OPT1_EQ);
        fprintf(stdout, "2. %s\n", SUP_OPT2_EQ);
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            fprintf(stdout, "You have selected: %s.\n", SUP_OPT1_EQ);
            cjson_add_key_value_to_json_string(send_socket_buffer, K_SUP_COMMAND, SUP_OPT1, OVERRIDE);
            cjson_add_key_value_to_json_string(send_socket_buffer, K_SUP_COMMAND_EQ, SUP_OPT1_EQ, OVERRIDE);
            break;
        case 2:
            fprintf(stdout, "You have selected: %s.\n", SUP_OPT2_EQ);
            cjson_add_key_value_to_json_string(send_socket_buffer, K_SUP_COMMAND, SUP_OPT2, OVERRIDE);
            cjson_add_key_value_to_json_string(send_socket_buffer, K_SUP_COMMAND_EQ, SUP_OPT2_EQ, OVERRIDE);
            break;
        default:
            printf("Invalid option. Please enter a valid number.\n");
            flag=1;
            break;
        }
    } while (flag);
    
    printf("Write the suply:\n");
    scanf("%s", key);
    cjson_add_key_value_to_json_string(send_socket_buffer, K_KEY, key, OVERRIDE);

    printf("Write the amount:\n");
    scanf("%d", &value);
    char value_str[10];
    sprintf(value_str, "%d", value);
    cjson_add_key_value_to_json_string(send_socket_buffer, K_VALUE, value_str, OVERRIDE);
}

void clear_screen() {
    fprintf(stdout, "...\n");
    sleep(1);
    // ANSI secuence to clean the screen
    // printf("\033[2J\033[H");
    printf("\033[2J");
}

static void sign_handler(int signal)
{
    switch (signal) {
        case SIGINT:
            /* out of the loop*/
            printf("SIGINT called\n");
            flag_get_supply = 0;
            break;
        case SIGTSTP:
            printf("SIGTSTP called\n");
            // end connection
            end_client_conn();
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
