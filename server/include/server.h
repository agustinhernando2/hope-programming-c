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
#include <r_w_handler.h>
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
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
void run_server_ipv4(char* argv[]);

/**
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
void run_server_ipv6(char* argv[]);

/**
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
int connect_server_ipv4(int *sockfd, char* argv[]);

/**
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
int connect_server_ipv6(int *sockfd, char* argv[]);

/**
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
void run_server(int newsockfd);

/**
 * @brief get command
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
 * @brief do message with deneid operation.
 *
 */
void make_deneid_message();

/**
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
static void sign_handler(int signal);

/**
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
void set_signal_handlers();

/**
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
int check_server_status();
