#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <limits.h>
#include "log.h"
#include "structparse.h"
#include "aux.h"

void sigint_handler(int sigint);

int countSubDirectories(char* directory);
int countSubDirectoriesRecursive(char* directory);
void resetSIGINT();
int calculateBlocks(int size, int block_size);
void simpleduPrototype(char* directory);
int getDirSize(char* directory);

int receivedSIGINT;

int main(int argc, char *argv[], char *envp[]){
    struct timeval start;
    gettimeofday(&start, NULL);

    char directory[50] = "./Test";

    //simpleduPrototype(directory);
    parser(argc,argv);


    pid_t pid = getppid();
    action_type action = CREATE;

    writeLog(getExecTime(start), pid, action);

    //printf("SIZE: %d\n", getDirSize("./Test")/1024);

    return 0;
}

// Auxiliary Functions to be placed elsewhere

void simpleduPrototype(char* directory){
    struct stat statbuf;
    int fd;

    struct dirent* dentry;

    DIR* source_dir = opendir(directory);


    if (source_dir == NULL)  // opendir returns NULL if couldn't open directory
    return;

    while ((dentry = readdir(source_dir)) != NULL){

        if(strcmp(dentry->d_name, "..") == 0)
        continue;

        fd = dirfd(source_dir); // Transforms directory into a file descriptor

        // Lê informações para o statbuf
        if(fstatat(fd, dentry->d_name, &statbuf, 0) < 0){
            perror("Could not read info");
            continue;
        }

        if(S_ISDIR(statbuf.st_mode)){
            int temp = getDirSize(dentry->d_name);
            printf("TEMP: %d\n", temp);
            printf("%d\t%s\n", calculateBlocks(temp, 1024), dentry->d_name);
        }
    }
}

/*
int getDirSize(char* directory)
{
    char new_entry[400];
    new_entry[0] = '\0';
    long int total = 0;
    long int size=0;
    struct stat statbuf;
    struct dirent* dentry;
    DIR *source_dir;

    source_dir = opendir(directory);

    while( (dentry = readdir(source_dir)) != NULL)
    {
        new_entry[0]='\0';

        // Ignora o diretório atual e o diretório pai
        if(strcmp(dentry->d_name, ".") == 0)
            continue;
        if(strcmp(dentry->d_name, "..") == 0)
            continue;

        //printf("%s",dentry->d_name);

        if(S_ISDIR(statbuf.st_mode)!=0)
        {
            strcat(new_entry, directory);
            strcat(new_entry,"/");
            strcat(new_entry, dentry->d_name);
            //printf("im here\n");

            printf("%s\n", new_entry);

            int temp = getDirSize(new_entry);

            printf("1\n");
            total += 4096 + temp;
            printf("2\n");
        }
        else
        {
            printf("%s\n",new_entry);
            stat(new_entry, &statbuf);
            size = statbuf.st_size;
            total += size;
        }

    }
    return total;
}
*/


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
                size+=statbuf.st_size+4096*(countSubDirectories(str)+1);
                getDirSize(str);
                printf("size=%d\n",size);
                printf("%s\n",dentry->d_name);
            }
        }
        else
        {
            strcpy(str,directory);
            strcat(str,"/");
            strcat(str,dentry->d_name);
            lstat(str,&statbuf);
            size+=statbuf.st_size;
            printf("size=%d\n",size);
            printf("%s\n",dentry->d_name);
        }
    }

    return size;

}


// Gets size in blocks of 1024
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

void sigint_handler(int sigint)
{
    if (sigint == SIGINT)
    receivedSIGINT = 1;
}

void resetSIGINT(){
    receivedSIGINT = 0;
}
