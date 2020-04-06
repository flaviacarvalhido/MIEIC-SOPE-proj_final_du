#pragma once
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
#include <string.h>

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


struct arg parser(int argc, char *argv[]){

    struct arg args;
    char *temp;

    for(unsigned int i=2;i<=argc-1;i++){

        if(argv[i]=="-a"){
            args.isA=true;
            continue;
        }
        if(argv[i]=="-b"){
            args.isB=true;
            continue;
        }
        if(argv[i]=="-B" && argv[i+1]=="size"){
            i++;
            args.isBSize=true;
            args.size=atoi(argv[i]);
            continue;
        }
        if(argv[i]=="-L"){
            args.isL=true;
            continue;
        }
        if(argv[i]=="-S"){
            args.isS=true;
            continue;
        }
        if(strstr(argv[i], "--max-depth=") != NULL){
            args.isMax=true;
            strncpy(temp, argv[i]+12, length(argv[i])-12);
            args.depth=atoi(temp);
            continue;
        }

        args.isPath=true;
        args.path=argv[i];

    }


    return args;
}
