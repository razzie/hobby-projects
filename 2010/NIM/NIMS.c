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
#define PORT 12345

#define FLAG 0

int allapotkuldes(int* state, int socket)
{
    char tmp[12];
    memcpy(tmp, "allapot", 8);
    tmp[8] = state[0];
    tmp[9] = state[1];
    tmp[10] = state[2];
    tmp[11] = state[3];
    return send(socket, tmp, 12, FLAG);
}

int palcikak(int* state)
{
    return state[0]+state[1]+state[2]+state[3];
}

int main(int argc, char** argv) {

    int serv, client1, client2, egyik, masik;
    int state[4];
    int current_client, ujra, vege, jatekvege;
    int on = 1;
    int l, sor;
    struct sockaddr_in serv_addr;
    char buffer[256];

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    serv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(serv, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(serv, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    if (bind(serv, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("Nem indult el a szerver (foglalt a port?)\n");
        return 0;
    }
    printf("A szerver elindult\n");

    // első kliens fogadása
    listen(serv, 2);
    client1 = accept(serv, (struct sockaddr*)NULL, NULL);
    printf("Az elso jatekos csatlakozott\n");
    send(client1, "Varj a masikra", 15, FLAG);

    // második kliens fogadása
    listen(serv, 2);
    client2 = accept(serv, (struct sockaddr*)NULL, NULL);
    printf("A masodik jatekos csatlakozott\n");

    Kezdes:
    //kezdődik a játék
    current_client = 0;
    ujra = vege = jatekvege = 0;
    state[0] = 1;
    state[1] = 3;
    state[2] = 5;
    state[3] = 7;
    send(client1, "Kezdodik a jatek", 17, FLAG);
    send(client2, "Kezdodik a jatek", 17, FLAG);
    printf("A jatek elkezdodott\n");

    while (1)
    {
        if (current_client == 0)
        {
            egyik = client1;
            masik = client2;
        }
        else
        {
            egyik = client2;
            masik = client1;
        }

	if (ujra == 2)
	{
	    printf("A jatek ujrakezdodik\n");
	    goto Kezdes;
	}
	if (vege > 0)
	{
	    send(egyik, "vege", 5, FLAG);
	    send(masik, "vege", 5, FLAG);
	    printf("Valaki nem akart ujat kezdeni, a jatek veget ert\n");
	    goto Vege;
	}

	if (jatekvege == 0)
	{
            l = allapotkuldes(state, egyik);
	    printf("Allapot elkuldese a kliensnek: %d\n",current_client);
            if (l <= 0)
            {
            	printf("Hiba\n");
            	goto Vege;
            }
	}

        l = recv(egyik, buffer, 256, FLAG);
        if (l==0)
        {
            printf("Vege a kapcsolatnak (kliens: %d)\n", current_client);
            goto Vege;
        }
        if (l<0)
        {
            printf("Hiba\n");
            goto Vege;
        }

        if (strcmp(buffer,"feladom") == 0)
        {
            send(egyik, "vesztettel", 11, FLAG);
            send(masik, "gyoztel", 8, FLAG);
	    printf("Jatekos%d feladta\n",current_client);
	    jatekvege++;
        }
        else if (strcmp(buffer,"vege") == 0)
        {
            printf("Jatekos%d nem akar ujat kezdeni\n",current_client);
	    vege++;
        }
        else if (strcmp(buffer,"ujra") == 0)
        {
	    printf("Jatekos%d ujat akar kezdeni\n",current_client);
	    ujra++;
        }
        else
        {
            sor = buffer[0]-'1';
            if (sor >= 0 && sor <= 3 && state[sor] > 0)
            {
                state[sor]--;
		printf("Jatekos%d elvett egy palcikat\n",current_client);
                if (palcikak(state) == 1)
                {
                    send(egyik, "gyoztel", 8, FLAG);
		    send(masik, "vesztettel", 11, FLAG);
		    printf("Jatekos%d gyozott\n",current_client);
		    jatekvege++;
                }
            }
            else
            {
		printf("Jatekos%d hibas lepest adott meg, ezert ujra lep\n",current_client);
                
	    	continue;
            }
        }

        current_client++;
        if (current_client == 2) current_client = 0;
    }

    Vege:
    
    close(client1);
    close(client2);
    close(serv);	
    printf("A szerver leall\n");

    return EXIT_SUCCESS;
}
