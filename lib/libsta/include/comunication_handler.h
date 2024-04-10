#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>


#define BUFFER_SIZE 512
#define MESSAGE "message"
#define END "end"
#define TRUE 1

/**
 * @brief - send a message
 * If the message is too long so It'll be sending by parts
 * @param json_buffer: JSON character buffer.
 * @param sockfd: socket file descriptor.
 * @return 0 success, 1 if an error occurs.
 */
int send_message(char *json_buffer, int b_size,int sockfd);

/**
 * @brief - send a message through sockets
 *
 * @param json_buffer: JSON character buffer.
 * @param sockfd: socket file descriptor.
 * @return 0 success, 1 if an error occurs.
 */
int send_message_to_socket(char *socket_buffer, int sockfd);

// /**
//  * @brief - send a message through sockets
//  *
//  * @param json_buffer: JSON character buffer.
//  * @param sockfd: socket file descriptor.
//  * @return 0 success, 1 if an error occurs.
//  */
// int recv_message(int sockfd, char *buffer)
