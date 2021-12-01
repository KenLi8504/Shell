# Shell
Function Headers
//int count_semicols(char *line)
//Takes in a line and counts how many semicolons there are in it

//char ** split_at_semicolons(char *line,int num)
//Takes in a line and splits it at all the semicolons, then
//returns all the splitted parts; the number is just there to
//minimize the necessary mallocs

//int countArgs(char * line)
//Takes in a line and counts how many spaces there are (and thereby
//counting how many arguments there are)

//char ** parse_args(char * line,int counter)
//This function takes in a string and breaks it apart based on where the spaces
//are; the counter is just to use only the necessary memory

//void execute(char ** good)
//Just a basic program that forks and execvps a parsed double pointer before exiting

//char ** split_at_redirection(char *line,char *symbol)
//Takes in a string with some redirection symbol (">",">>","<", or "|") and gets told
//what the redirection symbol is,and then splits the string in two parts

//void redirectionThing(char * line, int status)
//Takes in a string and which status, or what redirection type the string is,
//and performs the necessary operation(s) for the string (appends, overwrites, reads,etc)

//int check_piping(char *line)
//Takes in a line, scans through it, and just returns a number that tells what
//type of redirection to use; if no redirection symbol is found, returns 0

//void print_CurrentDir()
//Gets and prints the current working directory

//void chdirStuff(char ** input)
//This is just for the 2-3 functions you mentioned we couldn't fork; it
//checks if the person typed in cd, cd.., or exit and uses chdir to execute
//any of these if they did
