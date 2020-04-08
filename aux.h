#pragma once
#include <sys/time.h>
#include <stddef.h>

// Gets elapsed time since start of program
double getExecTime(struct timeval start);
