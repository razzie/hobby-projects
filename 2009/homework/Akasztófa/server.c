#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

#include "common.h"

char myword[MAX_WORDLEN];
char rec_buffer, buffer[1 + MAX_WORDLEN];
int myword_len;

void select_word() {
	int i = rand()%5;
	switch ( i ) {
		case 0: strcpy(myword, "kutya");
			myword_len = 5;
			break;
		case 1: strcpy(myword, "macska");
			myword_len = 6;
			break;
		case 2: strcpy(myword, "asztal");
			myword_len = 6;
			break;
		case 3: strcpy(myword, "ablak");
			myword_len = 5;
			break;
		case 4: strcpy(myword, "szoba");
			myword_len = 5;
			break;
	}
	for (i=0;i<myword_len;i++) buffer[i+2] = '_';
}

void quit(int code) {
	switch ( code ) {
		case 1: printf("Socket creation error\n"); break;
		case 2: printf("Cannot bind to the socket\n"); break;
		case 3: printf("Cannot listen to the socket\n"); break;
		case 4: printf("Cannot accept on socket\n"); break;
		case 5: printf("Cannot receive from the socket\n"); break;
		case 6: printf("Cannot send data to client\n"); break;
		default: printf("Unknown error\n");
	}
	exit( code );
}

main( int argc, char *argv[] ) {
	char words[256];

	int i, counter, tries;
	int fd, fdc, on = 1;

	struct sockaddr_in server;
	struct sockaddr_in client;
	int server_size = sizeof server;
	int client_size = sizeof client;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT_NO);

	fd = socket(AF_INET, SOCK_STREAM, 0 );
	if ( fd < 0 ) quit(1);
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
		setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

	if ( bind( fd, (struct sockaddr *) &server, server_size) < 0 ) quit(2);

	if ( listen( fd, 10) < 0 ) quit(3);

	fdc = accept(fd, (struct sockaddr *) &client, &client_size);
	if ( fdc < 0 ) quit(4);


	/*
	 1. bájt: hibás próbálkozások
	  - 0: ez a betû már volt
	  - 1: semmi sincs még
	  - 2: akasztófa lába
	  - 3: láb + talapzat
	  - 4: oszlop
	  - 5: oszlop folytatás
	  - 6: kötél
	  - 7: game over
	 többi bájt: a szó állapota
	*/

	select_word();

	while ( 1 ) {
		err = recv( fdc, &rec_buffer, 1, 0 );
		if ( err < 0 ) quit(5);

		if (words[rec_buffer] > 0) buffer[0] = 0;
		else {
			counter = 0;
			for(i=0;i<myword_len;i++) {
				if(myword[i] == rec_buffer) {
					buffer[i+2] == myword[i];
					counter++;
				}
			}
			if (counter == 0) tries++;
			buffer[0] = tries;
		}

		err = send( fdc, buffer, 1+myword_len, 0 );
		if ( err < 0 ) quit(6);
	}


	close(fd);
	close(fdc);
	exit(0);
}
