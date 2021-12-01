#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

//int count_semicols(char *line)
//Takes in a line and counts how many semicolons there are in it
int count_semicols(char *line){
	int count = 0;
	for (int i = 0; i < strlen(line); i++){
		if (line[i] == ';'){
			count++;
		}
	}
	return count;
}

//int countArgs(char * line)
//Takes in a line and counts how many spaces there are (and thereby
//counting how many arguments there are)
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

//char ** parse_args(char * line,int counter)
//This function takes in a string and breaks it apart based on where the spaces
//are; the counter is just to use only the necessary memory
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

//void execute(char ** good)
//Just a basic program that forks and execvps a parsed double pointer before exiting
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

//char ** split_at_redirection(char *line,char *symbol)
//Takes in a string with some redirection symbol (">",">>","<", or "|") and gets told
//what the redirection symbol is,and then splits the string in two parts
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

//void redirectionThing(char * line, int status)
//Takes in a string and which status, or what redirection type the string is,
//and performs the necessary operation(s) for the string (appends, overwrites, reads,etc)
void redirectionThing(char * line, int status){
	if (status == 1){
		printf("Append\n");
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
		//printf("Overwrite\n");
		char *symbol = ">";
		char ** good = split_at_redirection(line,symbol);
		int permissions = O_CREAT | O_WRONLY|	O_TRUNC;
		int fd = open(good[1], permissions, 0644);
		int backup_sdout = dup(STDOUT_FILENO); // save stdout for later
		int err = dup2(fd, STDOUT_FILENO);
		char ** executable = parse_args(good[0],countArgs(line));
		execute(executable);
		err = dup2(backup_sdout,STDOUT_FILENO);
		//printf("GOT UP TO HERE \n");
		free(good);
	}
	if (status == 4){
		char *symbol = "|";
		char ** good = split_at_redirection(line,symbol);
		FILE * first = popen(line,"r");
		int firstDescriptor = fileno(first);
		// //FILE * second = popen(good[1],"w");
		char *newTest = malloc(1000000);
		// int err = dup2(STDIN_FILENO, firstDescriptor);
		// FILE * second = popen(good[1],"w");
		read(firstDescriptor,newTest,1000000);
		printf("%s\n",newTest);
	}
}

//int check_piping(char *line)
//Takes in a line, scans through it, and just returns a number that tells what
//type of redirection to use; if no redirection symbol is found, returns 0
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

//void print_CurrentDir()
//Gets and prints the current working directory
void print_CurrentDir(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s$ ", cwd);
}

//char ** split_at_semicolons(char *line,int num)
//Takes in a line and splits it at all the semicolons, then
//returns all the splitted parts; the number is just there to
//minimize the necessary mallocs
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

//void chdirStuff(char ** input)
//This is just for the 2-3 functions you mentioned we couldn't fork; it
//checks if the person typed in cd, cd.., or exit and uses chdir to execute
//any of these if they did
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

		if (check_piping(test) == 3){
			//printf("The string is now %s",test);
			char ** good = split_at_redirection(test,"<");
			int permissions = O_RDONLY;
			//printf("Opening %s...\n",good[1]);
			int fd = open(good[1],permissions, 0644);
			//int backup_sdout = dup(STDOUT_FILENO); // save stdout for later
			//int err = dup2(fd, STDIN_FILENO);
			char *newTest = malloc(1000000);
			int testing = read(fd,newTest,1000000);
			// if (testing == -1){
			// 	printf("PROBLEM!\n");
			// }

			char *newPos;
			if ((newPos=strchr(newTest, '\n')) != NULL){
				*newPos = ';';
			}

			test = newTest;
			//printf("The string is now %s",test);
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
			if (check_piping(testing[i]) == 0 || check_piping(testing[i]) == 3) {
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
