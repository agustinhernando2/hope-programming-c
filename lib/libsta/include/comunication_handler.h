#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_K_V_SUP_LENGHT 50
#define ADMIN "u"

#define SUP_OPT1 "1"
#define SUP_OPT1_EQ "food"
#define SUP_OPT2 "2"
#define SUP_OPT2_EQ "medicine"
#define OPTION_END 3
#define OPTION_END_EQ "End client connection"
#define CLOSE_CONNECTION 0
#define OPTION1_ 1
#define OPTION1 "1"
#define OPTION1_EQ "Get server status"
#define OPTION2_ 2
#define OPTION2 "2"
#define OPTION2_EQ "Modify supplies"
#define K_COMMAND "command"
#define K_COMMAND_EQ "command_equivalent"
#define K_SUP_COMMAND "suply_type"
#define K_SUP_COMMAND_EQ "suply_type_equivalent"

#define K_ACC_DENEID "deneid_command"
#define K_HOSTNAME "hostname"
#define K_PASSWORD "password"
#define K_END "end_connection"
#define K_VALUE "value"
#define K_KEY "key"

#define C_SIZE 50
#define BUFFER_SIZE 512
#define MESSAGE "message"
#define END "end"
#define TRUE 1
#define TRUE_ "1"
#define FALSE 0
#define FALSE_ "0"


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
int send_message_to_socket(char *json_buffer, int b_size, int sockfd);

/**
 * @brief - send a message through sockets
 *
 * @param json_buffer: JSON character buffer.
 * @param sockfd: socket file descriptor.
 * @return 0 success, 1 if an error occurs.
 */
int recv_message(int sockfd, char *json_buffer);
