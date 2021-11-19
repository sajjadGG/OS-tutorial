// test user program

#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
	int pid = fork();
	if(pid==0){
		char *str;
		str = (char*)malloc(150000);
		str[0] = 'a';
		printf(1,"hi from %d\n",getpid());
		printf(1,"pid : memsize\n");
		printf(1,"%d\n",proc_dump());
	}
	else
		while(1){

		}
	exit();
}

