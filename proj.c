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
#include "structparse.h"

void sigint_handler(int sigint);

int countSubDirectories(char* directory);
int countSubDirectoriesRecursive(char* directory);
double getExecTime(clock_t begin);
void resetSIGINT();
int calculateBlocks(int size, int block_size);
void simpleduPrototype(char* directory);
int getDirSize(char* directory);

int receivedSIGINT;

int main(int argc, char *argv[], char *envp[]){

    char directory[50] = "./Test";

    //simpleduPrototype(directory);
    parser(argc,argv);


    pid_t pid = getppid();
    action_type action = CREATE;


    // Falta dar cap às decimais e meter a escrever direito
    writeLog(10, pid, action);



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


int getDirSize(char* directory){

    int directory_size = 0;
    DIR *pDir;

    if ((pDir = opendir(directory)) != NULL)
    {
        struct dirent *pDirent;

        while ( (pDirent = readdir(pDir)) != NULL)
        {
            char buffer[PATH_MAX + 1];
            strcat(strcat(strcpy(buffer, directory), "/"), pDirent->d_name);
            struct stat file_stat;
            if (stat(buffer, &file_stat) == 0){
                directory_size += file_stat.st_size;
                printf("DIRECTORY: %s\n", pDirent->d_name);
                printf("DIRECTORY SIZE: %d\n", directory_size);
            }

            if (pDirent->d_type == DT_DIR)
            {
                if ( strcmp(pDirent->d_name, ".") && strcmp(pDirent->d_name, "..") )
                {
                    directory_size += getDirSize(buffer);
                }
            }
        }

        closedir(pDir);
    }

    return directory_size;
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
