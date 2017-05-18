#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 1024
#define TEST   printf( "HERE at line[%d]\n", __LINE__); // macro used for debugging only

void addToArgv(char *shellargv[], char *word);

#define MAX 1024
char start_dir[MAX];
char dir[MAX];
char *tok;
char sIn[MAX];


////////////////////////////////////////////////////////////////////////////////////
int spawn(char *shellargv[]) {
		pid_t status;
		int childexit;

		// create child process
		status = fork();

		// did fork fail?
		if (status < 0) {
				printf("fork failed\n");
				return 1;
		}

		// child process
		if (status == 0) {
				if (strcmp(shellargv[0], "calc") == 0 || strcmp(shellargv[0], "listf") == 0) {
					char src[MAX];
					char dest[MAX];
					strcpy(src, shellargv[0]);
					strcpy(dest, start_dir);
					strcat(dest,"/");
					strcat(dest,src);
					shellargv[0] = dest;
				}
				execvp(shellargv[0], shellargv);
				return 0;
		}

		// parent process
		else {
				wait(&childexit);
				return 0;
		}
} // EOspawn

////////////////////////////////////////////////////////////////////////////////////

// will take stdin as an argument eventuallY
int token(char *shellargv[]) {


	int state = 0 ; // 1 == left, 2 == between, 3==right
	int tokens = 0;
	getcwd(dir, MAX);
	strcat(dir, "/");

	int x = strlen(sIn);
	for (int i = 0 ; i < x ; i++) {  // while there are characters to read

		if (0 == state) {
			if (sIn[i] > ' ') { // ignore whitespace
				state = 1;
				addToArgv(shellargv, &sIn[i]);
				tokens++;
			}
			if(0 == state) {
				if (NULL != shellargv) {
					sIn[i] = '\0';
				}
			}
		}

		if (1 == state) { // make arg[i]
			if (sIn[i] <=' ') {
				state--;
				i--;
			}
			if(1 == state) {
			}
		}
	} // EOfor
	return(tokens) ;
} // EOtoken

////////////////////////////////////////////////////////////////////////////////////////////

 void execute(char **shellargv){
	char cwd[MAX];
	//printf("%s\n", shellargv[0]);
	if (strcmp(shellargv[0], "calc") == 0 || strcmp(shellargv[0], "listf") == 0) {
		spawn(shellargv);
	}
	else if (strcmp("cd", shellargv[0]) == 0 ) {
		getcwd(cwd, (size_t)sizeof(cwd));
		strcat(cwd, "/");
		if (chdir(strcat(cwd, shellargv[1])) < 0) {
			printf("no such directory: %s\n", shellargv[0]);
		}
		else {
			printf("cwd changed to %s\n", cwd );
		}
	}
	else if (strcmp("pwd", shellargv[0]) == 0) {
		getcwd(cwd, (size_t)sizeof(cwd));
		printf("%s ", cwd);
	}
	else if (strcmp("exit", shellargv[0]) == 0 ) {
		printf("exit encountered");
		exit(0);
	}
	else {
		spawn(shellargv);
	}
}

///////////////////////////////////////////////////////////////////////////////////

// return pointer to first thing in argv
void addToArgv(char *shellargv[], char *word){
  static int pos = 0; // indexing into shellargv
	if (NULL == shellargv) {
		pos = 0 ;
		return;
	}

	shellargv[pos] = word; // storing pointers to tokens into array shellargv[0] will be listf, calc
	pos++;
}

///////////////////////////////////////////////////////////////////////////////////

int main(void) {

  // keep for listf and calc
	getcwd(start_dir, MAX);

	printf("$> ");

	while(NULL != fgets(sIn, MAX, stdin)) { // get input from stdin and store it into sIn


		char **shellargv = NULL; // shellargv is just a pointer to NULL
		int args = token(shellargv); //shellargv is null therefore this 1st call to token() is only used to calculate num of tokens

		shellargv = (char**)malloc(sizeof(char*)*(args+1)); // shellargv is now an array of pointers to hold pointers to tokens +1
		shellargv[args] = NULL; // NULL terminate the array of pointers
		token(shellargv);
		execute(shellargv);
		free(shellargv);
		printf("$> ");
	} // EOwhile
	return 0;
}
