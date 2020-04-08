#include "aux.h"

double getExecTime(struct timeval start){
    struct timeval stop;

    gettimeofday(&stop, NULL);
    return (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
}

void loadArgv(struct info *info, char* argv[], int argc){
    for (size_t i = 0; i < argc; i++) {
        info->argv[i] = argv[i];
    }

    info->argv_size = argc;
}
