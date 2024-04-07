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
#include <emergency_handler.h>
#include <critical_handler.h>
#include <server_req_handler.h>
#include <comunication_handler.h>
#include <cjson_handler.h>
#include <cJSON.h>

#define STYPE_IPV4 1
#define STYPE_IPV6 2
#define BUFFER_SIZE 512
/* An integral type that can be modified atomically, without the
   possibility of a signal arriving in the middle of the operation.  */
volatile __uint8_t flag_handler = 0;
int sockfd;

char socket_buffer[BUFFER_SIZE];

static void handler();

/**
 * @brief Function to run the IPv4 or IPv6 server.
 *
 * This function runs the IPv4 or IPv6 server with the provided arguments.
 *
 * @param[in] argv Command-line arguments.
 */
// void run_server_ipvx(char *argv[]);


// int connect_server(int *sockfd, char *argv[], sa_family_t domain);


