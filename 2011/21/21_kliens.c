#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345
#define SERVER_ADDR "127.0.0.1"

#define BUFFER_LENGTH 256
#define FLAGS 0

#define JATEKBAN 0
#define RENONSZ 1
#define FUCCS 2
#define FELADTA 3
#define TARTJA 4

int main(void)
{
	// az összes deklaráció a program elején van
	// először a hálózati résszel kapcsolatos változók
	int sock;
	int ip, err;
	int on = 1;
	struct sockaddr_in server;

	// játékkal kapcsolatos dolgok
	// egy BUFFER_LENGTH hosszúságú tömb, itt tárolom a szervertől kapott üzeneteket, meg azt is amit a felhasználótól bekérek vagy elküldök
	char buffer[BUFFER_LENGTH];

	char* kartyak[] =
	{
		"Alsó",
		"Felső",
		"Király",
		"VII",
		"VIII",
		"IX",
		"X",
		"Ász",
		"--"
	}; // itt csak a nevekre van szükség, az értékekkel majd a szerver foglalkozik

	char* jatekallas[] =
	{
		"Játékban",
		"Renonsz",
		"Fuccs",
		"Feladta",
		"Tartja"
	};

	ip = inet_addr(SERVER_ADDR);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = ip;
	server.sin_port = htons(PORT);

	sock = socket(AF_INET, SOCK_STREAM, FLAGS);
	if (sock < 0)
	{
		printf("Váratlan hiba (socket). A kliens kilép.\n");
		return 1;
	}

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof on); // hibás kilépés esetén a lefoglalt port legyen újra felhasználható
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char *) &on, sizeof on); // keepalive kapcsolat, ne szakadjon meg tétlenségnél
	err = connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_in));
	if (err < 0)
	{
		printf("Váratlan hiba (connect). A kliens kilép.\n");
		return 1;
	}

    while (1)
	{
    	err = recv(sock, buffer, BUFFER_LENGTH, FLAGS);
		if (err <= 0)
		{
			printf("Váratlan hiba (fogadás). A kliens kilép.\n");
			return 1;
		}

		switch (buffer[0]) // a szerver és a kliens szöveges üzenetek helyett számok küldésével 'beszélget'
		{ // minden kapott üzenetnek megnézem az első karakterét, innen tudom mit üzent a szerver (számként kezelem, nem karakterként, ez ne zavarjon meg)
			case 0:
				printf("\nTe vagy az első játékos. Kérlek várj, amíg a másik játékos csatlakozik..\n");
				break;

			case 1:
				printf("\n");
				printf("   A másik játékos van soron\n");
				printf("   '%s' lapot kapott és jelenleg ennyinél tart: %d\n", kartyak[ buffer[1] ], buffer[2]);
				// a buffer[1] a kapott üzenet második bájtja, ilyen kártyát húzott a másik játékos
				// a buffer[2] a 3. bájt, ennyinél tart az ellenfér (pl ha ez az első lapja és Ászt kapott akkor az érték 11)
				printf("   Játékállása: %s\n", jatekallas[ buffer[3] ]);
				// a buffer[3] a 4. bájt és a játékállást tartalmazza (pl 0 esetén: Játékban)
				break;

			case 2:
				printf("\n______________________\n");
				printf("Te vagy soron\n");
				printf("'%s' lapot kaptál és ennyinél tartasz: %d\n", kartyak[ buffer[1] ], buffer[2]);
				printf("Játékállásod: %s\n", jatekallas[ buffer[3] ]);
				
				if (buffer[3] == JATEKBAN) // ha a játékállásom != Játékban, akkor felesleges bármit is beírnom már
				{
					printf("\nA következő parancsok használhatóak: lapot, tartom, feladom\n");
        			while (1)
					{
						scanf("%s", buffer);
        				if (strcmp(buffer, "lapot") == 0) break;
        				if (strcmp(buffer, "tartom") == 0) break;
        				if (strcmp(buffer, "feladom") == 0) break;
						// végtelen ciklus, ha a beírt szó egyiknek se felel meg, akkor megint bekéri a választást
						printf("Hibás parancs. Kérlek írd be újra:\n");
					}
					// elküldöm a szervernek azt amit beírt a felhasználó (hossz+1, mert minden stringet lezár egy \0 karakter)
        			err = send(sock, buffer, strlen(buffer)+1, FLAGS);
        			if (err <= 0)
					{
        				printf("Váratlan hiba. A kliens kilép.\n");
        				return 1;
        			}
				}
        		break;

			case 3:
				printf("\nA játék befejeződött\n");
				switch (buffer[1]) // a második bájt függvényében történik vmi
				{
					case 0:
						printf("Te viszed a kört. Kapsz egy nyalókát.\n");
						break;
					case 1:
						printf("A másik játékos viszi a kört. Nem kapsz nyalókát.\n");
						break;
					case 2:
						printf("Döntetlen állás alakult ki.\n");
						break;
					case 3:
						printf("Nem akartál új kört kezdeni, így a játék véget ér.\n");
						return 0;
						break;
					case 4:
						printf("A másik játékos nem akart új kört kezdeni, így a játék véget ér.\n");
						return 0;
						break;
				}
				break;

			case 4:
				printf("\nAkarsz új kört kezdeni?\nLehetőségek: ujra, vege\n");
        		while (1)
				{
					scanf("%s", buffer);
        			if (strcmp(buffer, "ujra") == 0) break;
        			if (strcmp(buffer, "vege") == 0) break;
					printf("Hibás parancs. Kérlek írd be újra:\n");
				}
        		err = send(sock, buffer, strlen(buffer)+1, FLAGS);
        		if (err <= 0)
				{
        			printf("Váratlan hiba. A kliens kilép.\n");
        			return 1;
        		}
				break;
		}
	}

	return 0;
}
