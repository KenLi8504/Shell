#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

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
	free(copyLine);
  return returnThis;
}

void execute(char ** good){
	//printf("GOT TO HERE POG\n");
  int fork1 = fork();
  if (fork1){
    wait(NULL);
    return;
  }
  else{
		//printf("Started EXING\n");
    execvp(good[0],good);
		//printf("FINISHED EXING\n");
    exit(0);
  }
}

int count_semicols(char *line){
	int count = 0;
	for (int i = 0; i < strlen(line); i++){
		if (line[i] == ';'){
			count++;
		}
	}
	return count;
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

char ** split_at_redirection(char *line,char *symbol){
	char *copyLine = malloc (strlen(line));
	strcpy(copyLine,line);

  char *sub;
  int current = 0;
  char ** returnThis = malloc(16);
  while( (sub = strsep(&copyLine,symbol)) != NULL){
    int value = *sub;
    if (value != 0){
      //printf("The value is %d\n",value);
      //printf("The string is %s\n",sub);
      returnThis[current] = sub;
      current++;
    }
  }
	free(copyLine);
  return returnThis;
}

void redirectionThing(char * line, int status){

	if (status == 1){
		char *symbol = ">>";
		char ** good = split_at_redirection(line,symbol);
		int permissions = O_CREAT | O_WRONLY|	O_APPEND;
		int fd = open(good[1], permissions, 0644);
		int backup_sdout = dup(STDOUT_FILENO); // save stdout for later
		int err = dup2(fd, STDOUT_FILENO);
		char ** executable = parse_args(good[0],countArgs(line));
		execute(executable);
		err = dup2(backup_sdout,STDOUT_FILENO);
		//printf("GOT UP TO HERE \n");
		free(good);
	}

	if (status == 2){
		printf("Overwrite\n");
	}
	if (status == 3){
		printf("Input\n");
	}
	if (status == 4){
		printf("pipe\n");
	}
}

int check_piping(char *line){
	if (strstr(line,">>") != NULL){
		return 1;
	}
	else if (strstr(line,">") != NULL){
		return 2;
	}
	else if (strstr(line,"<") != NULL){
		return 3;
	}
	else if (strstr(line,"|") != NULL){
		return 4;
	}
	else{
		return 0;
	}
}

void print_CurrentDir(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s$ ", cwd);
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
	free(copyLine);
  return returnThis;
}

void chdirStuff(char ** input){
	if (strcmp(input[0],"cd ..") == 0 || strcmp(input[0],"cd..") == 0){;
		chdir("..");
	}
	if (strcmp(input[0],"cd") == 0){
		chdir(input[1]);
	}
	if (strcmp(input[0],"exit") == 0){
		exit(0);
	}
}

int main(int argc, char ** argv){
  while (1){

		print_CurrentDir();
    char *test = malloc(1000000);
		fgets(test,1000000,stdin);

		char *pos;
		if ((pos=strchr(test, '\n')) != NULL){
			*pos = '\0';
		}

    int semicolons = count_semicols(test);

    char ** testing = malloc(8*semicolons);
    testing = split_at_semicolons(test,semicolons);
    for (int i = 0; i <= semicolons; i++){
      int length = countArgs(testing[i]);
			//printf("The length is %d\n",length);
      char ** good = parse_args(testing[i],length);
			if (strcmp(good[0],"cd ..") == 0 || strcmp(good[0],"cd..") == 0 || strcmp(good[0],"cd") == 0 || strcmp(good[0],"exit") == 0) {
				chdirStuff(good);
			}
			if (check_piping(testing[i]) == 0){
				execute(good);
			}
			else{
				//printf("OH YEAH IT'S PIPING TIME\n");
				redirectionThing(testing[i],check_piping(testing[i]));
			}
			//printf("DONE ONCE\n");
			free(good);
    }
    // for (int i = 0; i < length-1; i++){
    //   printf("The thing at good is %s\n",good[i]);
    // }
		free(test);
  }
}
