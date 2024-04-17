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

/* Libraries */
#include <emergency_handler.h>
#include <critical_handler.h>
#include <server_req_handler.h>
#include <comunication_handler.h>
#include <cjson_handler.h>
#include <cJSON.h>

#define STYPE_IPV4 1
#define STYPE_IPV6 2
#define N_CONNECTIONS 5

int sockfd;
/* An integral type that can be modified atomically, without the
   possibility of a signal arriving in the middle of the operation.  */
volatile __uint8_t flag_handler = 0;

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


