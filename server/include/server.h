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


void run_server_ipv4(char* argv[]);

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

static void sign_handler();

void run_server(int newsockfd);

int get_command(char* socket_buffer);

int check_credentials(char* socket_buffer);

void make_deneid_message(char* supplies_buffer);
