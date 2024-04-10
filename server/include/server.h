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
#define N_CONNECTIONS 5

struct sockaddr_in serv_addr;
uint16_t puerto;
int status;

pid_t pid;
int active_childs = 0;
uint8_t selected_ip_conn = 1;

char socket_buffer[BUFFER_SIZE];

/* An integral type that can be modified atomically, without the
   possibility of a signal arriving in the middle of the operation.  */
volatile __uint8_t flag_handler = 0;

int sockfd;

char socket_buffer[BUFFER_SIZE];


void run_server();

/**
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
int connect_server();

// void get_interface_ipv6(char* ip, char* iface)

static void sign_handler();


