#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


int count_semicols(char *line){
	int count = 0;
	for (int i = 0; i < strlen(line); i++){
		if (line[i] == ';'){
			count++;
		}
	}
	return count;
}

char ** split_at_semicolons(char *line,int num){
	char *copyLine = malloc (strlen(line));
	strcpy(copyLine,line);

  char *sub;
  int current = 0;
  char ** returnThis = malloc(8*num);
  while( (sub = strsep(&copyLine,";")) != NULL){
    int value = *sub;
    if (value != 0){
      //printf("The value is %d\n",value);
      //printf("The string is %s\n",sub);
      returnThis[current] = sub;
      current++;
    }
  }
  return returnThis;
}

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
      //printf("The string is %s\n",sub);
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

void execute(char ** good){
  int fork1 = fork();
  if (fork1){
    wait(NULL);
    return;
  }
  else{
    execvp(good[0],good);
    exit(0);
  }
}

int main(int argc, char ** argv){
  while (1){
    // char *blank = malloc(10000);
    // fgets(blank,10000,stdin);

    char *test = malloc(1000000);
    //test = "echo hello;ls -al;echo goodbye";
		fgets(test,1000000,stdin);

		char *pos;
		if ((pos=strchr(test, '\n')) != NULL){
			*pos = '\0';
		}

		//printf("The strings are %d",strcmp(test,"echo hello"));
		//printf("GOOD\n");
    int semicolons = count_semicols(test);
    //printf("There are %d semicolons in this line\n",semicolons);
    char ** testing = malloc(8*semicolons);
    testing = split_at_semicolons(test,semicolons);
    for (int i = 0; i <= semicolons; i++){
      int length = countArgs(testing[i]);
			//printf("The length is %d\n",length);
      char ** good = parse_args(testing[i],length);
      execute(good);
			//printf("DONE ONCE\n");
    }
    // for (int i = 0; i < length-1; i++){
    //   printf("The thing at good is %s\n",good[i]);
    // }
  }
}
