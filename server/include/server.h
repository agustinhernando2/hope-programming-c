/*preventing multiple inclusions of the same header file*/
#pragma once

/* Unix libraries */
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/mman.h>

/* Libraries */
#include <emergency_handler.h>
#include <critical_handler.h>
#include <comunication_handler.h>
#include <supplies_data_handler.h>
#include <lib_handler.h>
#include <cjson_handler.h>
#include <cJSON.h>

#define STYPE_IPV4 1
#define STYPE_IPV6 2
#define N_CONNECTIONS 5
#define JSON_FILE "data.json"


/* An integral type that can be modified atomically, without the
   possibility of a signal arriving in the middle of the operation.  */
volatile sig_atomic_t flag_handler = 0;
int sockfd;
char* send_socket_buffer = NULL;
char recv_socket_buffer[BUFFER_SIZE];

/**
 * @brief Runs the server using IPv4.
 *
 * @param[in] argv Command-line arguments.
 */
void run_server_ipv4(char* argv[]);

/**
 * @brief Runs the server using IPv6.
 *
 * @param[in] argv Command-line arguments.
 */
void run_server_ipv6(char* argv[]);

/**
 * @brief Connects to the server using IPv4.
 *
 * @param[out] sockfd Pointer to the socket file descriptor.
 * @param[in] argv Command-line arguments.
 * @return 0 on success, -1 on error.
 */
int connect_server_ipv4(int *sockfd, char* argv[]);

/**
 * @brief Connects to the server using IPv6.
 *
 * @param[out] sockfd Pointer to the socket file descriptor.
 * @param[in] argv Command-line arguments.
 * @return 0 on success, -1 on error.
 */
int connect_server_ipv6(int *sockfd, char* argv[]);

/**
 * @brief Run the server.
 *
 * @param[in] newsockfd The socket file descriptor for the connection.
 */
void run_server(int newsockfd);

/**
 * @brief Modify and send the supply status to the client.
 * only admin can do run this function.
 * @param[in] newsockfd The socket file descriptor for the connection.
 */
void set_and_send_suply_status(int newsockfd);

/**
 * @brief Sends a supply status message to the client.
 *
 * @param[in] newsockfd The socket file descriptor for the connection.
 */
void send_supply_message(int newsockfd);

/**
 * @brief Send an end connection.
 *
 * @param[in] newsockfd The socket file descriptor for the connection.
 */
void send_end_conn_message(int newsockfd);

/**
 * @brief Send a denied message to the client..
 *
 * @param[in] newsockfd The socket file descriptor for the connection.
 */
void send_deneid_message(int newsockfd);

/**
 * @brief Gets the command from the user.
 *
 * @return 0 on error command, positive number on success.
 */
int get_command();

/**
 * @brief check credentials
 *
 * @return 0 on success, 1 deneid.
 */
int check_credentials();


/**
 * @brief Signal handler function.
 *
 * @param[in] signal The signal received.
 */
static void sign_handler(int signal);

/**
 * @brief Sets signal handlers for interrupt signals.
 */
void set_signal_handlers();


/**
 * @brief Set the supply.
 *
 * @return 0 on success, -1 on error.
 */
int set_supply();
