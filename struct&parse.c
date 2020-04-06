#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include "log.h"
#include <stdbool.h>

struct arg
{
    bool isPath;
    bool isA;
    bool isB;
    bool isBSize;
    bool isL;
    bool isS;
    bool isMax;
};


struct arg parser(int argc, char *argv[]){
    
    struct arg args;


    for(unsigned int i=2;i<=argc-1;i++){




    }


    return args;
}
