// test user program

#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
	printf(1,"pid : memsize\n");
	int pid = fork();
	if(pid==0){
		printf(1,"hi from %d",getpid());
	
	}
	else
		printf(1,"My first userprogram: %d\n",proc_dump());
	exit();
}

