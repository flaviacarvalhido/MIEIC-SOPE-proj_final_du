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
#include <limits.h>
#include "log.h"

void sigint_handler(int sigint);

int countSubDirectories(char* directory);
int countSubDirectoriesRecursive(char* directory);
double getExecTime(clock_t begin);
void resetSIGINT();
int calculateBlocks(int size, int block_size);
void simpleduPrototype(char* directory);

int receivedSIGINT;

int main(int argc, char *argv[], char *envp[]){

    // Du function itself
    //a

    char directory[50] = ".";

    simpleduPrototype(directory);

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
            simpleduPrototype(dentry->d_name);
            printf("%d\t%s\n", calculateBlocks(statbuf.st_size, 1024), dentry->d_name);
        }
    }
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

// Gets elapsed time since start of program
double getExecTime(clock_t begin){
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
}


void sigint_handler(int sigint)
{
    if (sigint == SIGINT)
    receivedSIGINT = 1;
}

void resetSIGINT(){
    receivedSIGINT = 0;
}
