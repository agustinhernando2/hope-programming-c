#include <comunication_handler.h>

int send_message(char* json_buffer, int b_size, int sockfd)
{
    size_t slice = 0;
    char socket_buffer[BUFFER_SIZE];
    memset(socket_buffer, 0, BUFFER_SIZE);

    if (b_size <= BUFFER_SIZE)
    {
        strncpy(socket_buffer, json_buffer, b_size);
        return (send_message_to_socket(socket_buffer,b_size, sockfd));
    }
    while (TRUE)
    {
        if ((slice + BUFFER_SIZE) > b_size)
        {
            break;
        }
        strncpy(socket_buffer, json_buffer + slice, BUFFER_SIZE);
        if (send_message_to_socket(socket_buffer,BUFFER_SIZE, sockfd))
        {
            fprintf(stderr, "%s:%d: Error send_message_to_socket.\n", __FILE__, __LINE__);
            return 1;
        }
        slice += BUFFER_SIZE;
    }
    strncpy(socket_buffer, json_buffer + slice, BUFFER_SIZE);
    return send_message_to_socket(socket_buffer,strlen(json_buffer), sockfd);
}

int send_message_to_socket(char* json_buffer, int b_size, int sockfd)
{
    if((b_size == 0)||(strlen(json_buffer) == 0))
    {
        return 0;
    }
    if (send(sockfd, json_buffer, b_size, 0) == -1)
    {
        fprintf(stderr, "%s:%d: Error sending the json_buffer content.\n", __FILE__, __LINE__);
        return 1;
    }
    return 0;
}

int recv_message(int sockfd, char* socket_buffer)
{
    memset(socket_buffer, 0, BUFFER_SIZE);
    ssize_t n;
    n = recv(sockfd, socket_buffer, BUFFER_SIZE, 0);
    if ((n == -1) || (strlen(socket_buffer) == 0))
    {
        fprintf(stderr, "%s:%d: Error reciving the data.\n", __FILE__, __LINE__);
        return 1;
    }
    return 0;
}
