#include <stdio.h>
#include "log.h"

int main(void){

  char cenas[50];
  printf("Write a string to be output\n");
  scanf("%s", cenas);

  writeLog(cenas);

return 0;
}
