#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
char ** parse_args(char * line,int counter){
  char *sub;
  char *copyLine = malloc (strlen(line));
  strcpy(copyLine,line);
  char ** returnThis = malloc(8*(counter));
  int current = 0;
  while( (sub = strsep(&copyLine," ")) != NULL){
    returnThis[current] = sub;
    current++;
  }
  return returnThis;
}

int countArgs(char * line){
  char *sub;
  char *copyLine = malloc (strlen(line));
  strcpy(copyLine,line);
  int counter = 0;
  while( (sub = strsep(&copyLine," ")) != NULL){
    counter++;
  }
  return counter;
}

int main(){
  char *test = "ls -a -l";
  int length = countArgs(test);
  char ** good = parse_args(test,length);
  for (int i = 0; i < length; i++){
    printf("The thing at good is %s\n",good[i]);
  }
}
