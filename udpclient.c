#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include<stdio.h>
#include<string.h>

#define BUFSIZE 2048
char *my_message = "this is a test message";
int main(int argc, char* argv[]){

	int clientfd, recvlen, index;
	char writebuf[BUFSIZE];
	struct addrinfo hints, *local, *peer;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	memset(&hints,0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	char readbuf[BUFSIZE];
	
	if ( 0 != getaddrinfo(NULL,"9000",&hints,&local)){
		perror("couldn't getaddrinfo on local\n");
		return -1;
	}

	if ( (clientfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");
		return -1;
	}

 	freeaddrinfo(local);

        if ( 0 != getaddrinfo(argv[1],argv[2],&hints,&peer)) {
                perror("couldn't getaddrinfo of peer\n");
                return -1;
        }

	index = 0;
	for(;;){
		sleep(1);
		printf("Sending packet %d\n",index);		
		sprintf(writebuf, "This is packet %d",index++);
	
		if ( sendto(clientfd, writebuf,strlen(writebuf),0, (struct sockaddr*)peer->ai_addr,sizeof(struct sockaddr)) < 0 )
		{
			perror("sendto failed! :( ");
			return -1;
		}

		recvlen = recvfrom(clientfd, readbuf, BUFSIZE, 0, (struct sockaddr*)peer->ai_addr, &addrlen);
		if(recvlen > 0){
			readbuf[recvlen] = 0;
//			printf("received bytes %d \n",recvlen);
			printf("received message: \"%s\"\n",readbuf);
	
		}
	}
	
}
