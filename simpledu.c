#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <limits.h>
#include "log.h"
#include "structparse.h"
#include <math.h>
#include "aux.h"
#include <sys/wait.h>

void sigint_handler(int sigint);

int countSubDirectories(char* directory);
int countSubDirectoriesRecursive(char* directory);
void resetSIGINT();
int calculateBlocks(int size, int block_size);
void simpleduPrototype(char* directory);
int getDirSize(char* directory);
int du(char * dir,int d);
char ** readSubDirs(char*directory);

int receivedSIGINT;
struct arg args;
struct info i;

int main(int argc, char *argv[], char *envp[]){
    struct timeval start;
    gettimeofday(&start, NULL);
    char directory[50] = "./Test";

    resetLog();

    loadArgv(&i,argv,argc);
    writeLog(getExecTime(start),getpid(),CREATE,i);

    args=parser(argc,argv);

    if(args.error){
        printf("Error: wrong arguments provided. ./simpledu [-l or --count-links] <args>\n");
        i.exit_code=-1;
        writeLog(getExecTime(start),getpid(),EXIT,i);
        exit(-1);
    }


    //int size=getDirSize("./Test")+4;

    //du(args.path);
    int depth=args.depth;

    du("./Test",depth);

    //prints info of path provided in args
    //printf("%s\n",args.path);
    int size_parent=getDirSize(args.path)+4;
    //printf("%d\n",size_parent);
    printf("%d\t%s\n", size_parent, args.path);

    return 0;
}

int du(char * dir,int d){

    int subdir=countSubDirectories(dir);
    char ** subdirectories=readSubDirs(dir);
    pid_t pids[subdir];
    int status;
    struct stat buf;
    d--;


    for(unsigned int i=0;i<subdir;i++){

        pids[i] = fork();

        if(pids[i]==0){ //child

            char str[3000];
            str[0]='\0';

            char * mydir= subdirectories[i];
            //printf("%s\n",subdirectories[i]);

            strcat(str, dir);
            strcat(str, "/");
            strcat(str, mydir);

            //printf("str=%s\n",str);

            lstat(str,&buf);
            if(!args.isL && S_ISLNK(buf.st_mode)){
                exit(2);
            }

            int mysize = getDirSize(str)+4;

            //printf("size=%d\n",mysize);

            printf("%d\t%s\n", mysize, str);


            //printf("depthfilho:%d\n",d);
            if(countSubDirectories(str)!=0 && d>0){
                du(str,d);
            }

            exit(99);

        }else{  //parent
            //args.depth--;
            //printf("depthpai:%d\n",args.depth);
            pid_t wpid;
            while ((wpid = wait(&status)) > 0);
        }
    }


}

// Auxiliary Functions to be placed elsewhere

//always add +4 when called to account for current directory
int getDirSize(char* directory)
{
    struct stat statbuf;
    DIR *source_dir;
    struct dirent *dentry;
    int size=0;
    char str[100];

    source_dir = opendir(directory);
    while(dentry = readdir(source_dir))
    {
        if(dentry->d_type == 4)
        {
            if(dentry->d_name[0]!='.')
            {
                strcpy(str, directory);
                strcat(str, "/");
                strcat(str, dentry->d_name);
                lstat(str,&statbuf);
                size+=statbuf.st_blocks * 512/args.size+getDirSize(str);
                //printf("%s\n",dentry->d_name);
                //printf("size=%d\n",size);

            }
        }
        else
        {
            strcpy(str,directory);
            strcat(str,"/");
            strcat(str,dentry->d_name);
            lstat(str,&statbuf);
            if(!S_ISLNK(statbuf.st_mode))
                size+=statbuf.st_blocks*512/args.size;
            //printf("%s\n",dentry->d_name);
            //printf("size=%d\n",size);

        }
    }

    return size;

}

// Gets size in blocks specified
int calculateBlocks(int size, int block_size){
    return size / block_size;
}

// Counts subdirectories on the first level
int countSubDirectories(char* directory){
    int counter = 0;
    struct stat statbuf;
    int fd;

    struct dirent* dentry;

    DIR* source_dir = opendir(directory);

    if (source_dir == NULL)  // opendir returns NULL if couldn't open directory
    {
        return 0;
    }

    while ((dentry = readdir(source_dir)) != NULL){

        // Ignora o diretório atual e o diretório pai
        if(strcmp(dentry->d_name, ".") == 0)
        continue;
        if(strcmp(dentry->d_name, "..") == 0)
        continue;

        fd = dirfd(source_dir); // Transforms directory into a file descriptor

        // Lê informações para o statbuf
        if(fstatat(fd, dentry->d_name, &statbuf, 0) < 0){
            perror("Could not read info");
            continue;
        }

        if(S_ISDIR(statbuf.st_mode))
        counter++;
    }

    closedir(source_dir);
    return counter;
}

// Counts subdirectories recursively
int countSubDirectoriesRecursive(char* directory){
    int counter = 0;
    struct stat statbuf;
    int fd;

    struct dirent* dentry;

    DIR* source_dir = opendir(directory);

    if (source_dir == NULL)  // opendir returns NULL if couldn't open directory
    {
        return 0;
    }

    while ((dentry = readdir(source_dir)) != NULL){

        // Ignora o diretório atual e o diretório pai
        if(strcmp(dentry->d_name, ".") == 0)
        continue;
        if(strcmp(dentry->d_name, "..") == 0)
        continue;
        // Testing purposes
        if(strcmp(dentry->d_name, ".git") == 0)
        continue;

        fd = dirfd(source_dir); // Transforms directory into a file descriptor

        // Lê informações para o statbuf
        if(fstatat(fd, dentry->d_name, &statbuf, 0) < 0){
            perror("Could not read info\n");
            continue;
        }

        if(S_ISDIR(statbuf.st_mode)){
            counter++;
            counter+=countSubDirectoriesRecursive(dentry->d_name);
        }

    }

    closedir(source_dir);
    return counter;
}

char ** readSubDirs(char*directory){

    static char * vector[3000];
    DIR *source_dir;
    struct dirent *dentry;
    int index=0;
    int fd;
    struct stat statbuf;

    source_dir = opendir(directory);
    while ((dentry = readdir(source_dir)) != NULL){

        // Ignora o diretório atual e o diretório pai
        if(strcmp(dentry->d_name, ".") == 0)
        continue;
        if(strcmp(dentry->d_name, "..") == 0)
        continue;

        fd = dirfd(source_dir); // Transforms directory into a file descriptor

        // Lê informações para o statbuf
        if(fstatat(fd, dentry->d_name, &statbuf, 0) < 0){
            perror("Could not read info");
            continue;
        }

        if(S_ISDIR(statbuf.st_mode)){
            vector[index]=dentry->d_name;
            index++;
        }
    }
    return vector;
}

void sigint_handler(int sigint)
{
    if (sigint == SIGINT)
    receivedSIGINT = 1;
}

void resetSIGINT(){
    receivedSIGINT = 0;
}
