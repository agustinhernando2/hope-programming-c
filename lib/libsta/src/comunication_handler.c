#include <comunication_handler.h>

int send_message(char* json_buffer, int b_size, int sockfd)
{
    if (b_size <= 0 || json_buffer == NULL)
    {
        error_handler("Invalid buffer size or buffer is NULL.", __FILE__, __LINE__);
        return 1;
    }

    size_t slice = 0;
    char socket_buffer[BUFFER_SIZE];

    while (slice < b_size)
    {
        size_t bytes_to_copy = (b_size - slice > BUFFER_SIZE) ? BUFFER_SIZE : (b_size - slice);
        memcpy(socket_buffer, json_buffer + slice, bytes_to_copy);

        if (send_message_to_socket(socket_buffer, bytes_to_copy, sockfd))
        {
            error_handler("Error sending message to socket.", __FILE__, __LINE__);
            return 1;
        }

        slice += bytes_to_copy;
    }

    return 0;
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
    ssize_t n;
    char temp_buffer[BUFFER_SIZE];
    size_t total_bytes_received = 0;

    while (1)
    {
        // reviving data from the socket
        n = recv(sockfd, temp_buffer, BUFFER_SIZE, 0);
        if (n == -1)
        {
            error_handler("Error receiving data.", __FILE__, __LINE__);
            return 1;
        }
        else if (n == 0)
        {
            return 1;
            break;
        }

        // copy the received data to the buffer
        memcpy(socket_buffer + total_bytes_received, temp_buffer, n);
        total_bytes_received += n;

        // check if the message is complete
        if (memchr(temp_buffer, DELIMITER, n) != NULL)
        {
            break;
        }
    }

    // add the null terminator
    socket_buffer[total_bytes_received] = '\0';

    return 0;
}
