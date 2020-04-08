#include "aux.h"

double getExecTime(struct timeval start){
    struct timeval stop;

    gettimeofday(&stop, NULL);
    return (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
}
