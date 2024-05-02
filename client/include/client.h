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
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
int try_connect_server();

void send_message_to_server();

int recv_and_check_message();
void get_credentials();
void add_credentials();
void get_options();
void get_supplies_options();
void clear_screen();
static void sign_handler();
//void set_signal_handler(struct sigaction* sa);
