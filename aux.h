#pragma once
#include <sys/time.h>
#include <stddef.h>

// Gets elapsed time since start of program
double getExecTime(struct timeval start);

// Loads the struct info with the command line arguments
void loadArgv(&info, argv, argc);
