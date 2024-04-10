#include <comunication_handler.h>


int send_message(char *json_buffer, int b_size,int sockfd)
{
    size_t slice = 0;
    char socket_buffer[BUFFER_SIZE];
    memset(socket_buffer, 0, BUFFER_SIZE);

    if (strlen(json_buffer) < BUFFER_SIZE)
    {
        strncpy(socket_buffer, json_buffer, BUFFER_SIZE);
        return (send_message_to_socket(socket_buffer, sockfd));
    }
    while (TRUE)
    {
        if ((slice + BUFFER_SIZE) > strlen(json_buffer))
            break;
        strncpy(socket_buffer, json_buffer + slice, BUFFER_SIZE);
        if (!(send_message_to_socket(socket_buffer, sockfd)))
        {
            return 1;
        }
        slice += BUFFER_SIZE;
    }
    strncpy(socket_buffer, json_buffer + slice, BUFFER_SIZE);
    return (send_message_to_socket(socket_buffer, sockfd));
}

int send_message_to_socket(char *socket_buffer, int sockfd)
{
    if (send(sockfd, socket_buffer, BUFFER_SIZE, 0) == -1)
    {
        fprintf(stderr, "%s:%d: Error sending the socket_buffer content.\n",
        __FILE__, __LINE__);
        return 1;
    }
    return 0;
}

// int recv_message(int sockfd, char *buffer)
// {
//     memset(buffer, 0, BUFFER_SIZE);
//     ssize_t n;
//     n = recv(sockfd, buffer, BUFFER_SIZE, 0);
//     if ((n == -1) || (strlen(buffer) == 0))
//     {
//         fprintf(stderr, "%s:%d: Error reciving the data.\n", 
//         __FILE__, __LINE__);
//         return 1;
//     }
//     return 0;
// }
