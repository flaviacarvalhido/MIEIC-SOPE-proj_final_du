#include "structparse.h"



struct arg parser(int argc, char *argv[]){

    struct arg args;
    char temp[100];
    args.error=false;

    args.isA=false;
    args.isL=false;
    args.isMax =false;
    args.isl=false;
    args.isS=false;
    args.isB=false;

    if(argc < 2){
        args.error=true;
        return args;
    }

    for(unsigned int i=1;i<=argc-1;i++){

        if(strcmp(argv[i],"-l") == 0 || strcmp(argv[i],"--count-links")== 0){
            args.isl=true;
            continue;
        }


        if(strcmp(argv[i],"-a")==0 || strcmp(argv[i],"--all")==0){
            args.isA=true;
            continue;
        }
        if(strcmp(argv[i],"-b")==0 || strcmp(argv[i],"--bytes")==0){
            args.isB=true;
            continue;
        }

        if(strcmp(argv[i],"-B") ==0){
            i++;
            args.isBSize=true;


            if(atoi(argv[i])){
                args.size=atoi(argv[i]);
            }else{
                args.error=true;
                return args;
            }

            continue;
        }
        //another argument for -B size
        if(strstr(argv[i], "--block-size=") != NULL){
            args.isBSize=true;
            strncpy(temp, argv[i]+13, sizeof(argv[i]));
            temp[sizeof(temp)]='\0';  //null character added

            if(atoi(temp)){
                args.size=atoi(temp);
            }else{
                args.error=true;
                return args;
            }
            continue;
        }

        if(strcmp(argv[i],"-L")==0 || strcmp(argv[i],"--dereference")==0){
            args.isL=true;
            continue;
        }
        if(strcmp(argv[i],"-S")==0 || strcmp(argv[i],"--separate-dirs")==0){
            args.isS=true;
            continue;
        }
        if(strstr(argv[i], "--max-depth=") != NULL){
            args.isMax=true;
            strncpy(temp, argv[i]+12, sizeof(argv[i]));
            temp[sizeof(temp)]='\0';  //null character added

            if(atoi(temp) || strcmp(temp,"0") == 0){
                args.depth=atoi(temp);
            }else{
                args.error=true;
                return args;
            }
            continue;
        }



        struct stat buf;
        stat(argv[i],&buf);
        if(S_ISDIR(buf.st_mode)){
            args.isPath=true;
            args.path=argv[i];
        }else{
            args.error=true;
            return args;
        }

    }

    if(!args.isPath){
        args.path=".";
    }

    if(!args.isBSize){
        args.size=1024;
    }

    if(!args.isMax){
        args.depth = 9999;
    }

    if(args.depth<0){
        args.error=true;
    }

    if(!args.isl){
        args.error=true;
    }

    if(args.isA && args.isS){
        args.error=true;
    }

    return args;
}
