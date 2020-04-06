#include "structparse.h"



struct arg parser(int argc, char *argv[]){

    struct arg args;
    char *temp;

    for(unsigned int i=2;i<=argc-1;i++){

        if(argv[i]=="-a"){
            args.isA=true;
            printf("yes I work");
            continue;
        }
        if(argv[i]=="-b"){
            args.isB=true;
            printf("yes I work");
            continue;
        }
        if(argv[i]=="-B" && argv[i+1]=="size"){
            i++;
            args.isBSize=true;
            args.size=atoi(argv[i]);
            printf("yes I work");
            continue;
        }
        if(argv[i]=="-L"){
            args.isL=true;
            printf("yes I work");
            continue;
        }
        if(argv[i]=="-S"){
            args.isS=true;
            printf("yes I work");
            continue;
        }
        if(strstr(argv[i], "--max-depth=") != NULL){
            args.isMax=true;
            strncpy(temp, argv[i]+12, strlen(argv[i])-12);
            args.depth=atoi(temp);
            printf("yes I work");
            continue;
        }

        args.isPath=true;
        args.path=argv[i];

    }


    return args;
}
