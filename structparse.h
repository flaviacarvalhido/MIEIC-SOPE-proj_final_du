#pragma once
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include "log.h"
#include <stdbool.h>
#include <stdlib.h>


struct arg
{
    bool isPath;
    char * path;
    bool isA;
    bool isB;
    bool isBSize;
    int size;
    bool isL;
    bool isS;
    bool isMax;
    int depth;
};

struct arg parser(int argc, char *argv[]);


