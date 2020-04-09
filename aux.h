#pragma once
#include <sys/time.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Gets elapsed time since start of program
double getExecTime(struct timeval start);

int confirmExit();
