#pragma once
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <cjson_handler.h>

#define LAST_KEEP_ALIVED "last_keepalived"
#define LAST_EVENT "last_event"
#define SIZE_TIME 20
#define K_EMERGENCY "emergency"
#define LOG_E_FILE_PATH "data/emergency.log"

int run_emergency_handler(int* pipe_fd);

int add_event_to_json(char* key, char* value);
