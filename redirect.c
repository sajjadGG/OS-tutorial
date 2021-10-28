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

int main(){

	int rc = fork();
	
	if(rc<0){ // fork failed
		printf("can not instantiate a process");
	}else if(rc==0){ // child process


	}else{ // parent process


	}	

	


	return 0;
}
