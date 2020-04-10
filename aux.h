#pragma once
#include <sys/time.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern struct timeval start;

// Gets elapsed time since start of program
double getExecTime();

// Prompts the user for an exit confirmation
int confirmExit();
