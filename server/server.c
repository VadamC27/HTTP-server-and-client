#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "HTTPprocessing.h"

#define BUFFER_SIZE 8192 //how long can a http request get, acording to http specyfication it is recomened to have 8000 octets

struct cln{
	int cfd;
	struct sockaddr_in caddr;
};

void* cthread(void * arg){
	struct cln* c =(struct cln*) arg;
	printf("[%lu] new connection from: %s:%d\n", (unsigned long int)pthread_self(),
			inet_ntoa((struct in_addr)c->caddr.sin_addr),
			ntohs(c->caddr.sin_port));

	//alocate memory for response
	char * request_buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
	if (request_buffer == NULL) {
        perror("Memory allocation error");
        goto end;
    }


	//read request
	size_t received_size = read(c->cfd, request_buffer, BUFFER_SIZE);
	if (received_size == -1) {
        perror("Read error");
		goto end;
    }
	request_buffer[received_size] = '\0';

	//process request 
	char * response = prepare_response(request_buffer, received_size);
	if (response == NULL){
		perror("Error preparing response");
		goto end;
	}

	size_t response_size = strlen(response);
	size_t bytes_written = 0;

	while (bytes_written < response_size) {
		ssize_t write_result = write(c->cfd, response + bytes_written, response_size - bytes_written);

		if (write_result == -1) {
			perror("Write error");
			goto end;
		}
		bytes_written += write_result;
	}
		

end:
	free(request_buffer);	
	close(c->cfd);
	free(c);
	return NULL;
}


int main(int argc, char * argv[]){
    socklen_t sl;
	int sfd, on;
	struct sockaddr_in saddr;
	pthread_t tid;
	
	//signal(SIGCHLD, childend);

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(8080);
	sfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(char*)&on,sizeof(on));
	bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	listen(sfd,100);

	while(1){
		struct cln * c = malloc(sizeof(struct cln));
		sl =sizeof(c->caddr);

		c->cfd = accept(sfd, (struct sockaddr*)&c->caddr, &sl);
		
		pthread_create(&tid, NULL, cthread, c);
		pthread_detach(tid);
		
	}

    return 0;
}