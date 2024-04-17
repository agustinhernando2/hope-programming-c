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

#define ISVALID "isValid"
#define OPTION1 "opcion 1"
#define OPTION2 "opcion 2"
#define OPTION3 "opcion 3"
#define OPTION4 "opcion 4"
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define K_HOSTNAME "hostname"
#define K_PASSWORD "password"
#define K_COMMAND "command"

uint16_t puerto;
char socket_buffer[BUFFER_SIZE];
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

int recv_and_check_message();
void get_credentials();
void add_credentials();
void get_options();