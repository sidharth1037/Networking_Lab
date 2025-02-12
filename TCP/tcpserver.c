#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int connfd)
{
	char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(connfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\t To client : ", buff);
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n')
			;

		// and send that buffer to client
		write(connfd, buff, sizeof(buff));

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0); //parameters (internet/localhost, tcp/udp, protocol value for internet which is 0)
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr)); // write zeros at location servaddr
	

	// assign IP, PORT
	servaddr.sin_family = AF_INET; // decides whether tcp or udp
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //tells socket to listen on all available interfaces.
	servaddr.sin_port = htons(PORT);
	//Network order is big endian. Host order can be big or little­ endian
	// htonl converts unsigned integer hostlong from host byte order to network byte order. 
	// htons converts unsigned short integer hostshort from host byte order to network byte order. 
	

	// Binding newly created socket to given IP and verification
	// (SA*) means (struct sockaddr *) 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	/* int listen(int sockfd, int backlog);
	   The backlog, defines the maximum length to which the queue of pending connections for sockfd may grow.
	   If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED.*/
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	/*int new_socket= accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	  It extracts the first connection request on the queue of pending connections for
	  the listening socket, sockfd, creates a new connected socket,
	  and returns a new file descriptor referring to that socket. At this point,
	  the connection is established between client and server, and they are ready to transfer data.*/
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}
