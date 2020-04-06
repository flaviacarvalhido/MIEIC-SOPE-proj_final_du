#include "structparse.h"



struct arg parser(int argc, char *argv[]){

    struct arg args;
    char temp[100];

    for(unsigned int i=2;i<=argc-1;i++){


        if(strcmp(argv[i],"-a")==0){
            args.isA=true;
            //printf("yes I work -a\n");
            continue;
        }
        if(strcmp(argv[i],"-b")==0){
            args.isB=true;
            //printf("yes I work -b\n");
            continue;
        }
        if(strcmp(argv[i],"-B") ==0){
            i++;
            args.isBSize=true;


            if(atoi(argv[i])){
                args.size=atoi(argv[i]);
                //printf("i worked\n");
            }else{
                args.error=true;
                //printf("error\n");
                return args;
            }

            
            //printf("yes I work -B\n");
            //printf("size: %d",args.size);
            continue;
        }
        if(strcmp(argv[i],"-L")==0){
            args.isL=true;
            //printf("yes I work -L\n");
            continue;
        }
        if(strcmp(argv[i],"-S")==0){
            args.isS=true;
            //printf("yes I work -S\n");
            continue;
        }
        if(strstr(argv[i], "--max-depth=") != NULL){
            args.isMax=true;
            strncpy(temp, argv[i]+12, sizeof(argv[i]));
            temp[sizeof(temp)]='\0';  //null character added

            if(atoi(temp)){
                args.depth=atoi(temp);
                //printf("i worked\n");
            }else{
                args.error=true;
                //printf("error\n");
                return args;
            }
            //printf("yes I work --max\n");
            //printf("depth: %d\n",args.depth);
            continue;
        }

        args.isPath=true;
        args.path=argv[i];
        //printf("path is : %s",args.path);



    }

    if(!args.isPath){
        args.path=".";
        //printf("path is current\n");
    }

    return args;
}
