#include <client.h>

int main(int argc, char* argv[])
{
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
        memset(socket_buffer, 0, BUFFER_SIZE);
        get_options();
        add_credentials(); 
        if (send_message(socket_buffer, BUFFER_SIZE, sockfd))
        {
            fprintf(stderr, "%s:%d: Error send_message. errno: %s\n", 
            __FILE__, __LINE__, strerror(errno));
            exit(EXIT_FAILURE);
        }
        memset(socket_buffer, 0, BUFFER_SIZE);
        if (recv_and_check_message())
        {
            // fprintf(stderr, "%s:%d: Error user not authorized. errno: %s\n", 
            // __FILE__, __LINE__, strerror(errno));
        }
        print_cjson(socket_buffer);
    }
    return 0;
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
    char value[10];
    recv_message(sockfd, socket_buffer);
    return get_value_of_key_from_json_string(socket_buffer, ISVALID, value);
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
    cjson_add_key_value_to_json_string(socket_buffer, K_HOSTNAME, username);
    cjson_add_key_value_to_json_string(socket_buffer, K_PASSWORD, password);
}

void get_options()
{
    int option;

    printf("Please enter an option from 1 to 4:\n");
    fprintf(stdout, "1. %s\n", OPTION1);
    fprintf(stdout, "2. %s\n", OPTION2);
    fprintf(stdout, "3. %s\n", OPTION3);
    fprintf(stdout, "4. %s\n", OPTION4);
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        fprintf(stdout, "You have selected the %s.\n", OPTION1);
        cjson_add_key_value_to_json_string(socket_buffer, K_COMMAND, OPTION1);
        break;
    case 2:
        fprintf(stdout, "You have selected the %s.\n", OPTION2);
        cjson_add_key_value_to_json_string(socket_buffer, K_COMMAND, OPTION2);
        break;
    case 3:
        fprintf(stdout, "You have selected the %s.\n", OPTION3);
        cjson_add_key_value_to_json_string(socket_buffer, K_COMMAND, OPTION3);
        break;
    case 4:
        fprintf(stdout, "You have selected the %s.\n", OPTION4);
        cjson_add_key_value_to_json_string(socket_buffer, K_COMMAND, OPTION4);
        break;
    default:
        printf("Invalid option. Please enter a number from 1 to 4.\n");
        break;
    }
}

// int read_response(int sockfd){
//     if(print_message_from_socket(sockfd)){
//         fprintf(stderr, "%s:%d: Error print_message_from_socket: %s\n", __FILE__, __LINE__,strerror(errno));
//         return 1;
//     }
//     return 0;
// }
