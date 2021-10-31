/*
 * Takes file path to execute and print the  result of the operation into it (both stdout and stderr)
 * Path should exists and be an excutable file otherwise the program will print an error
 * It takes another file to print the result in it should create the file if not existed otherwise overwrite it
 * if the output file path was wrong the program will end it with an error
 * Then it will take the required parameter and argument from user to pass to the program
 * Then print the exit code produced from running the program
 * this program will be compiled using a Makefile
 * Usage : ./redirect <program> <file> <program argument>
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "redirect.h"

#define BUFFER_SIZE 256

int main(int agrc, char *argv[]){
	char programName[BUFFER_SIZE];
	char fileName[BUFFER_SIZE];
	char programArgumets[BUFFER_SIZE]; // TODO: a list of arguments
	char tempLine[BUFFER_SIZE];
	char command[BUFFER_SIZE];

	getLine(tempLine);
	assignSeparated(tempLine," ",&programName,&fileName);

	int rc = fork();
	
	if(rc<0){ // fork failed
		fprintf(stderr,"can not instantiate a process");
		exit(1);
	}else if(rc==0){ // child process
		printf("hello, I am child (pid:%d)\n" ,(int) getpid());
		// now exec
		if(!isFileExecutable){
			printf("given program is not executable");
			return 1;
		}
		if(!isFileWritable(fileName)){
			printf("give file is not writable");
			return 1;
		}
		sprintf(command , "%s %s > %s",programName, tempLine, fileName);
		int status = system(command);
	}else{ // parent process
		int rc_wait = wait(NULL);
		printf("parent done");

	}	

	


	return 0;
}
