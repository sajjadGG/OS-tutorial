
#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>

#define PORT 8090
#define BUFFER_SIZE 30000
#define MAX_PROCESS 20




int printMessage(const char* inp,const char* message,int socket){

	write(socket,message,strlen(message));
	return 0;
}

int checkServerLoad(){
	return 1;
}
//TODO:handle requests in a queue
int handleRequest(const char* valread, const char* message, int socket, int (*checkServerLoad)(), int (*taskFunc)(const char* inp,const char* message, int socket)){
	if(checkServerLoad()){
		int rc = fork();
		if(rc<0){
			return 1;
		}else if(rc==0){
			int status = taskFunc(valread,message,socket);
			_exit(status);

		}else{
			return 0;
		}
	}else{
		printf("HTTP:/1.1 429 Too Many Requests\nContent-Type: text/plain\nContent-Length: 4\n\nwait");
		return 0;
	}

}

int main(int argc, char const *argv[]){

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
		//uncomment following line and connect many clients
		//sleep(5)
		handleRequest(valread,hello,new_socket,checkServerLoad,printMessage);
		printf("---------------Hello message sent-------------");
		close(new_socket);
	}

}
