
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdarg.h>
#include<string.h>

#define INPUT_BUFFER_MAX 256

// File
int isFileWritable(const char* path){
	return access(path, F_OK | W_OK);

}


int isFileExecutable(const char* path){
	return access(path,F_OK | X_OK);
}


// Reading Input

char* rtrim(char *s){

	char *back = s + strlen(s);
	while(isspace(*--back));
		*(back+1)='\0';
	return s;
}
void getLine(char* line, int max){
	
	if(fgets(line,INPUT_BUFFER_MAX,stdin)==NULL){
		
	}
	rtrim(line);
}
//TODO: make it generic and pass a convert function
void assignSeparated(char *line, const char* delim, ...){

	char* token_ptr = strtok(line,delim);
	va_list ap;
	int j;
	va_start(ap,delim);
	while(token_ptr!=NULL){
		strcpy(va_arg(ap,char *),token_ptr);
		token_ptr = strtok(NULL,delim);
		j+=1;

	}
	va_end(ap);
}

void removeWord(char* line,const char* delim, char* res , int num){
	char* token_ptr = strtok(line,delim);
	int j=0;
	while(token_ptr!=NULL){
		if(j!=num)
			strcat(res,token_ptr);
	}

}
