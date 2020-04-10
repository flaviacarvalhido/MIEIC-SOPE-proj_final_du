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

#define READ 0
#define WRITE 1

void sigint_handler(int sigint);
void sigterm_handler(int sigterm);
void sigcont_handler(int sigcont);
void sigstop_handler(int sigstop);

int countSubDirectories(char* directory);
int countSubDirectoriesRecursive(char* directory);
void resetSIGINT();
int calculateBlocks(int size, int block_size);
void simpleduPrototype(char* directory);
int getDirSize(char* directory);
int du(char * dir, int d, int argc, char *argv[]);
char ** readSubDirs(char*directory);
int getFileSize(char* file);

int receivedSIGINT;
struct arg args;
struct info i;
struct timeval start;
int n;
pid_t parent_pid, child_pid = 0;

int main(int argc, char *argv[], char *envp[]){
    gettimeofday(&start, NULL);
    char directory[50] = ".";

    resetLog();

    loadArgv(&i,argv,argc);
    writeLog(getExecTime(),getpid(),CREATE,i);

    args=parser(argc,argv);

    if(args.error){
        printf("Error: wrong arguments provided. ./simpledu [-l or --count-links] <args>\n");
        i.exit_code=-1;
        writeLog(getExecTime(),getpid(),EXIT,i);
        exit(-1);
    }


    //int size=getDirSize("./Test")+4;

    //du(args.path);
    int depth=args.depth;

    //signal(SIGINT, sigint_handler);

    parent_pid = getpgrp();
    signal(SIGINT, sigint_handler);

    if(depth!=0){
        du(args.path,depth, argc, argv);
    }

    int size_parent;
    if(args.isB){
        size_parent=getDirSize(args.path)+4096;
    }
    else{
        size_parent=getDirSize(args.path)+ceil(4096/args.size);
    }
    //printf("%d\n",size_parent);

    char string_to_log[100];
    snprintf(string_to_log, sizeof(string_to_log), "%d %s\n", size_parent, args.path);
    i.entry = string_to_log;
    writeLog(getExecTime(), getpid(), ENTRY, i);
    printf("%d\t%s\n", size_parent, args.path);

    return 0;
}

int du(char * dir, int d, int argc, char *argv[]){

    int subdir=countSubDirectories(dir);
    char ** subdirectories=readSubDirs(dir);
    pid_t pid;
    int status;
    struct stat buf;
    char output[300];
    int fd[2];
    d--;

    for(unsigned int i=0;i<subdir;i++){

        if(pipe(fd) == -1) {
            perror("Pipe opening failed\n");
            exit(5);
        }

        pid = fork();

        if(pid==0){ //child
            
            
            if (getppid() == parent_pid)
            {
                child_pid = getpid();
            }

            signal(SIGTERM, sigterm_handler);
            signal(SIGCONT, sigcont_handler);
            signal(SIGSTOP, sigstop_handler);

            close(fd[READ]);

            char str[3000];
            str[0]='\0';

            char * mydir= subdirectories[i];

            strcat(str, dir);
            strcat(str, "/");
            strcat(str, mydir);

            struct info info;
            char new_path[300];
            new_path[0] = '\0';
            strcat(new_path, str);

            info.path = new_path;
            loadArgv(&info,argv, argc);

            writeLog(getExecTime(), getpid(), CREATE_FORK, info);

            //printf("str=%s\n",str);

            lstat(str,&buf);


            if(!args.isL && S_ISLNK(buf.st_mode) && args.isA){
                if(args.isB){
                    struct info info;
                    snprintf(output, sizeof(output), "%ld\t%s\n", buf.st_size, str);
                    info.sent_from_pipe = output;
                    writeLog(getExecTime(), getpid(), SEND_PIPE, info);

                    n=write(fd[WRITE], output, sizeof(output)+1);
                    //printf("%ld\t%s\n", buf.st_size, str);
                }
                else{
                    struct info info;
                    snprintf(output, sizeof(output), "%fd\t%s\n", ceil(buf.st_blocks*512/args.size), str);
                    info.sent_from_pipe = output;
                    writeLog(getExecTime(), getpid(), SEND_PIPE, info);
                    n=write(fd[WRITE], output, sizeof(output)+1);
                    //printf("%ld\t%s\n", buf.st_size/args.size, str);
                }
            }

            if(!args.isL && S_ISLNK(buf.st_mode)){
                exit(2);
            }

            int mysize;
            if(args.isB){
                if(args.isA && S_ISREG(buf.st_mode) )
                    mysize = getFileSize(str);
                else{
                    stat(str,&buf);
                    if(S_ISREG(buf.st_mode))
                       mysize=getFileSize(str);
                    else
                    {
                        mysize = getDirSize(str)+4096;
                    }
                }
            }
            else{
                if(args.isA && S_ISREG(buf.st_mode) ){
                    mysize = getFileSize(str);
                }
                else{

                    stat(str,&buf);
                    if(S_ISREG(buf.st_mode))
                       mysize=getFileSize(str);
                    else
                    {
                        mysize = getDirSize(str)+ceil(4096/args.size);
                    }
                }
            }

            snprintf(output, sizeof(output), "%d\t%s\n", mysize, str);

            info.sent_from_pipe = output;
            writeLog(getExecTime(), getpid(), SEND_PIPE, info);
            //printf("output: %s\n", output);
            n= write(fd[WRITE], output, sizeof(output)+1);
            //printf("%d\n",n);

            //printf("%d\t%s\n", mysize, str);

            close(fd[WRITE]);


            //printf("depthfilho:%d\n",d);
            if(countSubDirectories(str)!=0 && d>0){
                du(str, d, argc, argv);
            }

            exit(99);

        }
        else{  //parent

            if (getpgrp() == parent_pid)
            {
                child_pid = pid;
            }
            pid_t wpid;
            /*struct sigaction action;
            action.sa_handler = sigint_handler;
            sigemptyset(&action.sa_mask);
            action.sa_flags = 0;*/

            //sigaction(SIGINT,&action,NULL);

            while ((wpid = wait(&status)) > 0);
            

            close(fd[WRITE]);
            char received_data[1000];

            int r;
            r = read(fd[READ], received_data, 1000);

            struct info info;
            info.received_from_pipe = received_data;
            info.entry = received_data;
            writeLog(getExecTime(), getpid(), RECV_PIPE, info);
            writeLog(getExecTime(), getpid(), ENTRY, info);


            //printf("n is %d\n",n);
            //printf("read returns %d\n",r);
            printf("%s",received_data);

            close(fd[READ]);
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
        if(!(dentry->d_type == 4))
        {
            strcpy(str,directory);
            strcat(str,"/");
            strcat(str,dentry->d_name);

            int temp = 0;

            if(args.isL){
                lstat(str,&statbuf);

                if(S_ISLNK(statbuf.st_mode)){
                    stat(str,&statbuf);
                    if(S_ISDIR(statbuf.st_mode)){
                        lstat(str,&statbuf);
                        if(args.isB){
                            temp=/*statbuf.st_size + */ getDirSize(str)+4096;
                            size+=temp;
                        }
                        else{
                            temp=ceil(statbuf.st_blocks*512/args.size) + getDirSize(str)+ceil(4096/args.size);
                            size+=temp;
                        }
                    }else{
                        if(args.isB){
                            temp=statbuf.st_size /*+  getFileSize(str)+4096*/;
                            size+=temp;
                        }
                        else{
                            temp=ceil(statbuf.st_blocks*512/args.size) /*getFileSize(str)+4*/;
                            size+=temp;
                        }
                    }
                }
                else {
                    if(args.isB){
                        temp=statbuf.st_size;
                        size+=temp;
                    }
                    else{
                        temp=ceil(statbuf.st_blocks*512/args.size);
                        size+=temp;
                    }
                }
            }

            else{
                lstat(str,&statbuf);
                if(args.isB){
                    temp=statbuf.st_size;
                    size+=temp;
                }
                else{
                    temp=ceil(statbuf.st_blocks*512/args.size);
                    size+=temp;
                }
            }
        }
        else
        {


            if( (strcmp(dentry->d_name,"..")!=0)  && !args.isS && (strlen(dentry->d_name)>1 && !args.isS) )
            {

                strcpy(str, directory);
                strcat(str, "/");
                strcat(str, dentry->d_name);
                stat(str,&statbuf);

                int temp = 0;
                if(args.isB){
                    temp=statbuf.st_size + getDirSize(str);
                    size+=temp;
                }else{
                    temp=ceil(statbuf.st_blocks*512/args.size) + getDirSize(str);
                    size+=temp;
                }

                //printf("%s\n",dentry->d_name);
                //printf("size=%d\n",size);

            }


        }



        //printf("%s\n",dentry->d_name);
        //printf("size=%d\n",size);
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

        if(args.isA && S_ISREG(statbuf.st_mode))
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

        if(S_ISREG(statbuf.st_mode) && args.isA){
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
    killpg(child_pid, SIGSTOP);
    if (confirmExit())
    {
        kill(0, SIGTERM);
    }
    else
    {
        kill(0, SIGCONT);
    }
}

void sigterm_handler(int sigterm)
{
    exit(15);
}

void sigcont_handler(int sigcont)
{}

void sigstop_handler(int sigstop)
{
    pause();
}

void resetSIGINT(){
    receivedSIGINT = 0;
}

int getFileSize(char* file){
    struct stat statbuf;
    stat(file, &statbuf);

    if(args.isB)
    return statbuf.st_size;
    else
    return ceil(statbuf.st_blocks*512/args.size);
}
