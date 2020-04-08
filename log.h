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

struct info {
   char* argv[50];
   int argv_size;

   int exit_code;
   char* received_signal;

   char* sent_signal;
   pid_t pid;

   char* received_from_pipe;
   char* sent_from_pipe;

   int entry; // Number of bytes
};

int writeLog(double instant, pid_t pid, action_type action, struct info info);
void resetLog();
char* getDirectoryEnv();

// Loads the struct info with the command line arguments
void loadArgv(struct info *info, char* argv[], int argc);
