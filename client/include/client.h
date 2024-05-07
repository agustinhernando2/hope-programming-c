/*preventing multiple inclusions of the same header file*/
#pragma once

/* Unix libraries */
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/* Libraries */
#include <comunication_handler.h>
#include <cjson_handler.h>
#include <cJSON.h>
#include <lib_handler.h>

/* An integral type that can be modified atomically, without the
   possibility of a signal arriving in the middle of the operation.  */
volatile sig_atomic_t flag_get_supply = 0;

uint16_t puerto;
char send_socket_buffer[BUFFER_SIZE];
char recv_socket_buffer[BUFFER_SIZE];
struct sockaddr_in serv_addr;
struct hostent *server;
int sockfd;


char username[MAX_USERNAME_LENGTH];
char password[MAX_PASSWORD_LENGTH];

/**
 * @brief Attempts to connect to the server.
 *
 * Tries to establish a connection to the server. Returns 0 on success, 1 on error.
 *
 * @return 0 on success, 1 on error.
 */
int try_connect_server();

/**
 * @brief Sends a message to the server.
 *
 * Sends the contents of the send_socket_buffer to the sockfd.
 */
void send_message_to_server();

/**
 * @brief Receives a message from the server and checks for access denied or end of connection.
 *
 * If the command is to obtain the status of supplies, it enters a loop until a SIGINT signal is received.
 *
 * @return 0 on success, 1 if access is denied, -1 on error.
 */
int recv_and_check_message();

/**
 * @brief Closes the connection with the server.
 *
 * Closes the file descriptor and sends a notification message.
 */
void end_client_conn();


/**
 * @brief Requests user credentials.
 *
 * Requests the user to input their username and password.
 */
void get_credentials();

/**
 * @brief Adds the user credentials to the message.
 *
 * Adds the username and password to the message to be sent to the server.
 */
void add_credentials();

/**
 * @brief Requests user information.
 *
 * Requests the user to input information.
 */
void get_options();

/**
 * @brief Requests information to modify supplies.
 *
 * Requests the user to input information to modify supplies.
 */
void get_supplies_options();

/**
 * @brief Clears the screen.
 *
 * Clears the terminal screen.
 */
void clear_screen();

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
