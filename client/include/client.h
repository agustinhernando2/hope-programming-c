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

uint16_t puerto;
char socket_buffer[BUFFER_SIZE];
struct sockaddr_in serv_addr;
struct hostent *server;
int sockfd;

/**
 * @brief .
 *
 * .
 *
 * @param[in] argv Command-line arguments.
 */
int try_connect_server();
