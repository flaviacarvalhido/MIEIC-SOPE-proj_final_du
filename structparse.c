#include "structparse.h"



struct arg parser(int argc, char *argv[]){

    struct arg args;
    char *temp;

    for(unsigned int i=2;i<=argc-1;i++){

        printf("entered for \n");

        if(strcmp(argv[i],"-a")==0){
            args.isA=true;
            printf("yes I work\n");
            continue;
        }
        if(strcmp(argv[i],"-b")==0){
            args.isB=true;
            printf("yes I work\n");
            continue;
        }
        if(strcmp(argv[i],"-B") ==0){
            i++;
            args.isBSize=true;


            if(atoi(argv[i])){
                args.size=atoi(argv[i]);
                printf("i worked\n");
            }else{
                args.error=true;
                printf("error\n");
                return args;
            }

            
            printf("yes I work\n");
            continue;
        }
        if(strcmp(argv[i],"-L")==0){
            args.isL=true;
            printf("yes I work\n");
            continue;
        }
        if(strcmp(argv[i],"-S")==0){
            args.isS=true;
            printf("yes I work\n");
            continue;
        }
        if(strstr(argv[i], "--max-depth=") != NULL){
            args.isMax=true;
            strncpy(temp, argv[i]+12, strlen(argv[i])-12);
            args.depth=atoi(temp);
            printf("yes I work\n");
            continue;
        }

        args.isPath=true;
        args.path=argv[i];

    }

    if(args.path == NULL){
        args.path=".";
    }

    return args;
}
