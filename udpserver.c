#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include<stdio.h>
#include<string.h>


#define BUFSIZE 2048

char *message_from_server = "thank you for your message.";
int main(int argc, char* argv[])
{

	int serverfd, indexofACK; /* our socket fd*/
	struct sockaddr_in myaddr, remaddr; /*host and remote sockaddresses */
	int recvlen; /* bytes received */
	unsigned char readbuf[BUFSIZE]; /* receive buffer */
	unsigned char writebuf[BUFSIZE]; /* send buffer */
	socklen_t addrlen = sizeof(remaddr);

	if ( (serverfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");
		return -1;
	}

	memset((char*)&myaddr,0,sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(atoi(argv[1]));

	if ( (bind(serverfd, (struct sockaddr*)&myaddr, sizeof(myaddr))) < 0 ){
		perror("bind failed");
		return -1;
	}


	printf("Waiting on port %d\n",atoi(argv[1]));
	for(;;){
		sleep(1);
		recvlen = recvfrom( serverfd, readbuf, BUFSIZE, 0, (struct sockaddr*)&remaddr, &addrlen) ;
		printf("received %d bytes\n", recvlen);
		if (recvlen > 0) {

			readbuf[recvlen] = 0;
			sscanf(readbuf, "This is packet %d",&indexofACK);
			printf("received message: \"%s\"\n", &readbuf[0]);
		}
		sprintf(writebuf, "ack %d", indexofACK);
		sendto(serverfd, writebuf, strlen(writebuf), 0, (struct sockaddr*)&remaddr,addrlen);
	}

}
