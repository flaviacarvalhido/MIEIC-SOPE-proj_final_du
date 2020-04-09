#pragma once
#include <sys/time.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

extern struct timeval start;

// Gets elapsed time since start of program
double getExecTime();

int confirmExit();
