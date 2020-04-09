#include "aux.h"

double getExecTime(){
    struct timeval stop;

    gettimeofday(&stop, NULL);
    return (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
}

int confirmExit(){
    char response[50];

    do {
        printf("Do you wish to terminate the program? (Yes/No)\n");
        scanf("%s", response);
    } while( (strcmp(response,"Yes") != 0) && (strcmp(response,"yes") != 0) &&
                (strcmp(response,"Y") != 0) && (strcmp(response,"y") != 0) &&
                (strcmp(response,"No") != 0) && (strcmp(response,"no") != 0) &&
                (strcmp(response,"N") != 0) && (strcmp(response,"n") != 0) );

    if ( (strcmp(response,"Yes")==0) || (strcmp(response, "Y") ==0) || (strcmp(response,"yes")==0) || (strcmp(response,"y")==0))
        return 1;
    else
        return 0;
}
