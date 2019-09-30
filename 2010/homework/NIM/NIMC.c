/* 
 * File:   main.c
 * Author: hgabi00
 *
 * Created on 2010. december 15., 11:40
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
 * 
 */
#define IP "127.0.0.1"
#define PORT 12345

#define FLAG 0

void rajzol(int s1, int s2, int s3, int s4)
{
    int i;
    //sor1
    printf("1.: ");
    for (i=0;i<s1;i++) printf("|");
    printf("\n");
    //sor2
    printf("2.: ");
    for (i=0;i<s2;i++) printf("|");
    printf("\n");
    //sor3
    printf("3.: ");
    for (i=0;i<s3;i++) printf("|");
    printf("\n");
    //sor4
    printf("4.: ");
    for (i=0;i<s4;i++) printf("|");
    printf("\n");
}

int main(int argc, char** argv) {

    int sock;
    int on = 1;
    int l;
    struct sockaddr_in addr;
    char buffer[256];

    addr.sin_family = AF_INET; /*AF_INET: milyen címcsalád, internet típusú*/
    addr.sin_port = htons(PORT); /* big endien-be konvertál _ host order to network order short int */
    addr.sin_addr.s_addr = inet_addr(IP); /*szerver ip-je = sztring to szám*/

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /*socket azon. (address family, TCP-t fog hasznalni, )*/
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on); /* port felszabadít */
    setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on); /*életben tartja a kapcsot*/
 
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) /*connect (aktív sock, szerver struktúrája, server size)  - 3 lépéses kézfogás túloldali párja  */
    {
        printf("Nem sikerult kapcsoldni\n");
        return 0;
    }

    while (1)
    {
        l = recv(sock, buffer, 256, FLAG); /*byteok hossza = recv(sock,buffer/1024max/, buffer mérete, flags) */
        if (l==0)
        {
            printf("Vege a kapcsolatnak\n");
            goto Vege;
        }
        if (l<0)
        {
            printf("Hiba");
            goto Vege;
        }

        if (strcmp(buffer, "allapot") == 0) 
        {
            rajzol(buffer[8],buffer[9],buffer[10],buffer[11]);
            printf("Lepj:\n");
            scanf("%s",buffer);
            l = send(sock, buffer, strlen(buffer)+1, FLAG); /*send(sock,buffer,bytes/puffer üzen hossz/,flags*/
            if (l <= 0)
            {
                printf("Hiba\n");
                goto Vege;
            }
	    continue;
        }
	
        else if (strcmp(buffer,"vege") == 0)
        {
            printf("A masik jatekos nem akart jatszani\n");
            goto Vege;
        }
        else if (strcmp(buffer, "gyoztel") == 0 || strcmp(buffer, "vesztettel") == 0)
        {
            if (strcmp(buffer, "gyoztel") == 0) printf("Gyoztel! Gratula\n");
            else printf("Vesztettel\n");

            printf("Akarsz uj jatekot? (ujra / vege)\n");
            do
            {
                scanf("%s", buffer);
            }
            while(strcmp(buffer,"ujra") && strcmp(buffer,"vege"));
            l = send(sock, buffer, strlen(buffer)+1, FLAG);
            if (l<=0)
            {
                printf("Hiba\n");
                goto Vege;
            }
            if (strcmp(buffer,"vege") == 0)
            {
                printf("Kileptel\n");
                goto Vege;
            }
	    continue;
        }
        else
        {
            printf("%s\n",buffer);
	    continue;
        }
    }

    Vege:
    close(sock);

    return (EXIT_SUCCESS);
}


