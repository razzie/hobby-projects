#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NO  55225
#define SERVER_ADDR "127.0.0.1"


int main(int argc, char argv[])
{

/************************** Declarations ***********************/
    int fd, client[2];		/* socket endpt, rcv flags      */// létrehozunk egy két elemű tömböt a klienshez a váltogatás miatt

    struct sockaddr_in server;	/* socket name (addr) of server */
    int server_size = sizeof server;	/* length of the socket addr. server */

    struct sockaddr_in clt;	/* socket name of client */
    int clt_size = sizeof clt;	/* length of the socket addr. client */

    int ip, err;
    char buffer[255];		//milyen választ kap a clienstől
    int on = 1;

// JÁTÉK DEKLARÁCIÓ
    int i;			// játékos váltogatásához
    int egyik, masik;
    int meg1;
    char kuldbuffer[10];
    char menet;
    char mittipp[2];
    int eredmeny;


    char jatekos[2];		//játékos pontszáma
    char dobas;

// SERVER beállítása
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NO);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
	error("%s: Socket creation error\n", argv[0]);
	exit(1);
    }

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof on);
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *) &on, sizeof on);

/* elso socket = fd | sockaddr a szerver beallitasai | sockaddr merete bajtban */
    err = bind(fd, (struct sockaddr *) &server, server_size);
    if (err < 0) {
	error("%s: Cannot bind to the socket\n", argv[0]);
	exit(2);
    }

    err = listen(fd, 10);
    if (err < 0) {
	error("%s: Cannot listen to the socket\n", argv[0]);
	exit(3);
    }

    client[0] = accept(fd, (struct sockaddr *) &clt, &clt_size);
    printf("Első játékos csatlakozott\n");
// szólunk az első játékosnak hogy várjunk
    kuldbuffer[0] = 0;		// a clientben megvolt a switchbe hogy 0 akkor várjon és most adjuk meg a serverből
    send(client[0], kuldbuffer, 1, 0);

    client[1] = accept(fd, (struct sockaddr *) &clt, &clt_size);
// buffer tartalma [1-----] mindenkinek el kell küldeni ez lesz a játék kezdődik
    printf("MÁsodik játékos csatlakozott\n");
  ujra:
    printf("új játék kezdődött\n");
    i = -1;
    meg1 = 0;
    menet = 1;
    jatekos[0] = 0;
    jatekos[1] = 0;

    kuldbuffer[0] = 1;
    send(client[0], kuldbuffer, 1, 0);
    send(client[1], kuldbuffer, 1, 0);

    while (1) {
	sleep(1);
	i++;
	egyik = i % 2;		//nem kezeljük külön a két játékost hogy első második váltogatódik
	masik = (i + 1) % 2;	// ez jelöli a második játékost
	printf("jatekos[%d] van soron\n", egyik);

	printf("Elküldjük hogy tippeljen\n");
	kuldbuffer[0] = 2;	//tippelni kell majd
	kuldbuffer[1] = menet;	//menetszám  [2"menettartalma"-------]
	send(client[egyik], kuldbuffer, 2, 0);

	err = recv(client[egyik], buffer, 255, 0);
	if (err <= 0)
	    printf("HIBa");

	printf("DEBUG: jatekos[%d]: %s\n", egyik, buffer);
	if (strcmp(buffer, "fej") == 0) {
	    mittipp[egyik] = 'f';
	    printf("A tippje fej volt\n");
	}
	if (strcmp(buffer, "iras") == 0) {
	    mittipp[egyik] = 'i';
	    printf("A tippje iras volt\n");
	}

	if (strcmp(buffer, "feladom") == 0) {
	    printf("Feladta\n");
	    kuldbuffer[0] = 5;

	    printf("Elküldjük a jelenlegi játékosnak hogy feladta\n");
	    kuldbuffer[1] = jatekos[egyik];
	    kuldbuffer[2] = jatekos[masik];
	    kuldbuffer[3] = 3;
	    send(client[egyik], kuldbuffer, 4, 0);

	    printf
		("Elküldjük a másik játékosnak hogy a jelenlegi feladta\n");
	    kuldbuffer[1] = jatekos[masik];
	    kuldbuffer[2] = jatekos[egyik];
	    kuldbuffer[3] = 4;
	    send(client[masik], kuldbuffer, 4, 0);

	    err = recv(client[egyik], buffer, 255, 0);
	    if (strcmp(buffer, "ujra") == 0)
		meg1++;

	    err = recv(client[masik], buffer, 255, 0);
	    if (strcmp(buffer, "ujra") == 0)
		meg1++;

	    if (meg1 == 2) {
		printf("Mindketten újat akarnak játszani\n");
		goto ujra;
	    } else {
		kuldbuffer[0] = 6;
		send(client[egyik], kuldbuffer, 1, 0);
		send(client[masik], kuldbuffer, 1, 0);
		printf("Valaki nem akart újat kezdeni, játék vége\n");
		return 0;
	    }
	}
	//SZÓLUNK A MÁSIK JÁTÉKOSNAK HOGY MIT TIPPELTÜNK
	kuldbuffer[0] = 3;
	kuldbuffer[1] = mittipp[egyik];
	send(client[masik], kuldbuffer, 2, 0);
	sleep(1);
	printf
	    ("A jelenlegi játékos megmondja a másiknak hogy mit tippelt\n");


	if (egyik == 0) {	// értesítem hogy várjon mivel én még az első játékos vagyok , a másodikra várunk
	    printf
		("Jelenleg még csak az egyik játékos tippelt, szólunk neki hogy várjon a másikra\n");
	    kuldbuffer[0] = 0;
	    send(client[egyik], kuldbuffer, 1, 0);
	} else {		// MIND A Ketten tippeltünk
	    printf("Mostmár mindketten tippeltek, jöhet a dobás\n");
	    //dobás

	    srand((unsigned) time(NULL));
	    dobas = (rand() % 2 == 0) ? 'f' : 'i';
	    if (mittipp[egyik] == dobas) {
		jatekos[egyik]++;
		printf("A jelenlegi játékos pontot kapott\n");
	    }
	    if (mittipp[masik] == dobas) {
		jatekos[masik]++;
		printf("A másik játékos pontott kapott\n");
	    }
	    //ÉRTESÍTÉS
	    kuldbuffer[0] = 4;
	    kuldbuffer[3] = menet;
	    kuldbuffer[4] = dobas;
	    menet++;

	    printf("Jelezzük a jelenlegi játékosnak a menet végét\n");
	    kuldbuffer[1] = jatekos[egyik];
	    kuldbuffer[2] = jatekos[masik];
	    send(client[egyik], kuldbuffer, 5, 0);

	    printf("Jelezzük a másik játékosnak a menet végét\n");
	    kuldbuffer[1] = jatekos[masik];
	    kuldbuffer[2] = jatekos[egyik];
	    send(client[masik], kuldbuffer, 5, 0);
	}
	if (menet == 11) {
	    sleep(1);
	    printf("A 10. menet is megvolt\n");
	    kuldbuffer[0] = 5;

	    printf
		("A jelenlegi játékos értékelése, és eredmény küldése\n");
	    if (jatekos[egyik] > jatekos[masik])
		eredmeny = 0;
	    else if (jatekos[egyik] < jatekos[masik])
		eredmeny = 1;
	    else
		eredmeny = 2;
	    kuldbuffer[1] = jatekos[egyik];
	    kuldbuffer[2] = jatekos[masik];
	    kuldbuffer[3] = eredmeny;
	    send(client[egyik], kuldbuffer, 4, 0);

	    printf
		("A másik játékos értékelése, és eredmény küldése\n");
	    if (jatekos[masik] > jatekos[egyik])
		eredmeny = 0;
	    else if (jatekos[masik] < jatekos[egyik])
		eredmeny = 1;
	    else
		eredmeny = 2;
	    kuldbuffer[1] = jatekos[masik];
	    kuldbuffer[2] = jatekos[egyik];
	    kuldbuffer[3] = eredmeny;
	    send(client[masik], kuldbuffer, 4, 0);

	    err = recv(client[egyik], buffer, 255, 0);
	    if (strcmp(buffer, "ujra") == 0)
		meg1++;

	    err = recv(client[masik], buffer, 255, 0);
	    if (strcmp(buffer, "ujra") == 0)
		meg1++;

	    if (meg1 == 2) {
		printf("Mindketten újat akarnak játszani\n");
		goto ujra;
	    } else {
		kuldbuffer[0] = 6;
		send(client[egyik], kuldbuffer, 1, 0);
		send(client[masik], kuldbuffer, 1, 0);
		printf("Valaki nem akart újat kezdeni, játék vége\n");
		return 0;
	    }
	}
    }
    return 0;
}
