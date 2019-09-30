#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

#include "common.h"
#define SERVER_ADDR "127.0.0.1"

void quit(int code) {
	switch ( code ) {
		case 1: printf("Socket creation error\n"); break;
		case 2: printf("Cannot connect to server\n"); break;
		case 3: printf("Cannot send to server\n"); break;
		case 5: printf("Cannot receive from the socket\n"); break;
		default: printf("Unknown error\n");
	}
	exit( code );
}

void print_status(char status, char *word, int wordlen) {
	int i;
	printf("\n\nSzerver valasza:\n\n");
	switch ( status ) {
		case 0: printf("Ez a betu mar volt!\n"); break;
		case 1: printf(": \n: \n: \n: \n: \n:  |  |\n");
		case 2: printf(": \n: \n: \n: \n: ======\n:  |  |\n");
		case 3: printf(": \n:     |\n:     |\n:     |\n: ======\n:  |  |\n");
		case 4: printf(":   __\n:     |\n:     |\n:     |\n: ======\n:  |  |\n");
		case 5: printf(":   __\n:     |\n:     |\n:     |\n: ======\n:  |  |\n");
		case 6: printf(":   __\n:  '  |\n:     |\n:     |\n: ======\n:  |  |\n");
		case 7: printf(":   __\n:   o |\n:  -+-|\n:   ^ |\n: ======\n:  |  |\n\n: Game Over!\n");

	}
	printf("\n");
	for(i=0;i<wordlen;i++) printf("%c",word[i]);
	printf("\n");
}

main( int argc, char *argv[] ) {
	int fd, fdc;
	int ip, err, on = 1;

	char buffer[1+MAX_WORDLEN], c;

	struct sockaddr_in server;
	struct sockaddr_in client;
	int server_size = sizeof server;
	int client_size = sizeof client;

	ip = inet_addr(SERVER_ADDR);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = ip;
	server.sin_port = htons(PORT_NO);

	fd = socket(AF_INET, SOCK_STREAM, 0 );
	if ( fd < 0 ) quit(1);
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
		setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

	if ( connect( fd, (struct sockaddr *) &server, server_size) < 0 ) quit(2);


	buffer[0] = 0;

	while ( 1 ) {
		c = getch();
		if (c == 27) break;

		err = send( fd, &c, 1, 0 );
		if ( err < 0 ) quit(3);

		err = recv( fd, buffer, 1+MAX_WORDLEN, 0 );
		if ( err < 0 ) quit(5);

		print_status(buffer[0], &buffer[1], err-1);
	}

	close(fd);
	exit(0);
}
