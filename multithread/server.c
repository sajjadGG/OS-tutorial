
#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>
#define PORT 8090
#define BUFFER_SIZE 30000
#define MAX_THREAD 200

typedef struct __myarg_t {
	int socket;


} myarg_t;


void *printMessage(void *arg){
	int soc = *((int *) arg);
	//write(m->socket,m->message,strlen(m->message));
	sleep(5);
	write(soc,"HTTP:/1.1 200 OK\nContet-Type: text/plain\nContent-Length:20\n\nhello form thread",100);
	printf("---------------Hello message sent-------------\n");
	close(soc);
	pthread_exit(NULL);
}

int checkServerLoad(){
	return 1;
}
//TODO:handle requests in a queue
int handleRequest(const char* valread, const char* message, int* socket, int (*checkServerLoad)(), void* (*taskFunc)(void * )){
	if(checkServerLoad()){
		pthread_t p;
		pthread_attr_t attr;
		int rc;
		uint64_t ns = 4;
		printf("WHYYYYYY!!!!!!! %d\n",socket);
		printf("before");
		printf("\n");
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		rc = pthread_create(&p,&attr,printMessage,socket);
	//	pthread_join(p,NULL);
	}else{
		printf("HTTP:/1.1 429 Too Many Requests\nContent-Type: text/plain\nContent-Length: 4\n\nwait");
		return 0;
	}

}

int main(int argc, char const *argv[]){
	printf("parent process id : %d" , (int)getpid());
	int server_fd,new_socket;
	long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char *hello = "HTTP:/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

	if((server_fd = socket(AF_INET,SOCK_STREAM,0))==0){
		perror("In socket");
		exit(EXIT_FAILURE);
		
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	memset(address.sin_zero,'\0',sizeof address.sin_zero);

	if(bind(server_fd,(struct sockaddr *)&address,sizeof(address))<0){
		perror("In bind");
		exit(EXIT_FAILURE);
	}

	if(listen(server_fd,10)<0){
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	int sockets[MAX_THREAD];
	int num=0;
	while(1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if((new_socket = accept(server_fd,(struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
			perror("In accept");
			exit(EXIT_FAILURE);

		}

		char buffer[BUFFER_SIZE] = {0};
		valread = read(new_socket,buffer,BUFFER_SIZE);
		printf("%s\n",buffer);
		sockets[num] = new_socket;
		handleRequest(valread,hello,&sockets[num],checkServerLoad,printMessage);
		num++;
	}

}
