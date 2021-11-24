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
    int value = *sub;
    if (value != 0){
      // printf("The value is %d\n",value);
      // printf("The string is %s\n",sub);
      returnThis[current] = sub;
      current++;
    }
  }
  returnThis[current] = NULL;
  return returnThis;
}

int countArgs(char * line){
  char *sub;
  char *copyLine = malloc (strlen(line));
  strcpy(copyLine,line);
  int counter = 0;
  while( (sub = strsep(&copyLine," ")) != NULL){
    int value = *sub;
    if (value){
      // printf("Good!\n");
      counter++;
    }
  }
  return counter+1;
}

int main(int argc, char ** argv){
  char *test = "echo hello";
  int length = countArgs(test);
  printf("%d\n",length);
  // printf("%d",strcmp(" ", strsep(&test," ")));
  char ** good = parse_args(test,length);
  // for (int i = 0; i < length-1; i++){
  //   printf("The thing at good is %s\n",good[i]);
  // }
  execvp(good[0],good);
}
