#pragma once
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

typedef enum {CREATE, EXIT, RECV_SIGNAL, SEND_SIGNAL, RECV_PIPE, SEND_PIPE, ENTRY} action_type;
static char* action_type_string[] = {"CREATE", "EXIT", "RECV_SIGNAL", "SEND_SIGNAL", "RECV_PIPE", "SEND_PIPE", "ENTRY"};

int writeLog(double instant, pid_t pid, action_type action);
void resetLog();
char* getDirectoryEnv();
