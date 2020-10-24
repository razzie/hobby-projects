#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
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

#define NINCS_KARTYA 8

#define NYERT 0
#define VESZTETT 1
#define DONTETLEN 2

void kuldes(int sock, char* buffer, int hossz)
{
	int err = send(sock, buffer, hossz, FLAGS); // valójában az err ha pozitív, akkor azt mutatja hány bájtot küldtünk, de ebben a programban nem fontos
	if (err <= 0)
	{
		printf("Váratlan hiba (küldés). A szerver kilép.\n");
		exit(1);
	}
}

void fogadas(int sock, char* buffer, int hossz)
{
	int err = recv(sock, buffer, hossz, FLAGS); // itt pedig az err a kapott bájtok számát mutatja, de erre sincs szükség
	if (err <= 0)
	{
		printf("Váratlan hiba (fogadás). A szerver kilép.\n");
		exit(1);
	}
}

// a kuldes és fogadas függvények nem adnak vissza semmilyen értéket, mert nincs rá szükség (már ha a tanár kérdezné)

int main(void)
{
	// az összes deklaráció a program elején van
	// először a hálózati résszel kapcsolatos változók
	int sock;
	int ip, err;
	int on = 1;
	struct sockaddr_in server, client;
	int client_size = sizeof client;

	// játékkal kapcsolatos dolgok
	int i;
	int kartya;
	char buffer[BUFFER_LENGTH];

	char* jatekallas[] =
	{
		"Játékban",
		"Renonsz",
		"Fuccs",
		"Feladta",
		"Tartja"
	};

	typedef struct
	{
		int ertek;
		char* megnevezes;
	} kartyalap;

	kartyalap kartyak[] =
	{
		{ 2, "Alsó" },
		{ 3, "Felső" },
		{ 4, "Király" },
		{ 7, "VII" },
		{ 8, "VIII" },
		{ 9, "IX" },
		{ 10, "X" },
		{ 11, "Ász" },
		{ 0, "--" }
	}; // erre azért van szükség mert ezentúl tömbelemként hivatkozok a kártyákra (pl az Alsó az a kartyak[0] lesz, vagy a 0-ás számú lap)

	char referencia_pakli[32] = // ezt keverem össze és másolom a lapjait össze-vissza a másik pakliba (ami ténylegesen használva lesz)
	{
		0, 1, 2, 3, 4, 5, 6, 7,
		0, 1, 2, 3, 4, 5, 6, 7,
		0, 1, 2, 3, 4, 5, 6, 7,
		0, 1, 2, 3, 4, 5, 6, 7
	};
	char pakli[32]; // 32 lapos pakli, lehetne int típusú is, de a char csak 1 bájtos, és bőven elfér benne a 8 féle lap
	int hely, lap; // hely: keverésnél használom, lap: ez számolja, hogy hanyadik lapot kell majd húzni a pakliból legközelebb
	bool ures_hely[32]; // pakli keveréskor van jelentősége, lentebb elmagyarázom

	typedef struct
	{
		int socket;
		char lapok_erteke;
		char jatekallas; // 0: Játékban, 1: Renonsz, 2: Fuccs, 3: Feladta
		int eredmeny;
	} adat;

	adat jatekos[2]; // 2 játékos lesz, akik a fenti adatokkal rendelkeznek
	adat *soron_van, *masik;


	ip = inet_addr(SERVER_ADDR);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = ip;
	server.sin_port = htons(PORT);

	sock = socket(AF_INET, SOCK_STREAM, FLAGS);
	if (sock < 0)
	{
		printf("Váratlan hiba (socket). A szerver kilép.\n");
		return 1;
	}

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof on); // hibás kilépés esetén a lefoglalt port legyen újra felhasználható
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char *) &on, sizeof on); // keepalive kapcsolat, ne szakadjon meg tétlenségnél
	if ( bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) < 0 || listen(sock,10) < 0 )
	{
		printf("Váratlan hiba (bind / listen). A szerver kilép.\n");
		return 1;
	}

	// első játékos fogadása, majd értesítése arról, hogy várunk a másikra
	jatekos[0].socket = accept(sock, (struct sockaddr *) &server, &client_size);
	printf("Csatlakozott az első játékos, várunk a másikra.\n");

	buffer[0] = 0; // a buffer első bájtja 0 lesz, ez jelzi a kliensnek hogy várjon
	kuldes(jatekos[0].socket, buffer, 1); // csak 1 bájtnyi hosszú üzenetet küld most a szerver

	// a második játékos fogadása
	jatekos[1].socket = accept(sock, (struct sockaddr *) &server, &client_size);
	printf("Csatlakozott a masik jatekos.\n");

	Kezdes:
	printf("Új játék kezdődik.\n");

	// játékosok alapértékeinek beállítása
	for (i=0;i<=1;i++)
	{
		jatekos[i].lapok_erteke = 0;
		jatekos[i].jatekallas = JATEKBAN;
	}

	// pakli keverése rész jön
	// a ures_hely tömböt arra használom, hogy 'emlékezzek' az adott helyre tettem-e már lapot a pakliban
	for (i=0;i<32;i++)
		ures_hely[i] = true;

	// az egész keverés lényege, hogy megpróbálok a referenci_pakli lapjait megpróbálom a 24 üres hely közül valahová betenni (de csak ha a hely nem üres)
	srand( time(NULL) );
	for (i=0;i<32;i++)
	{
		while(1)
		{
			hely = rand()%32;
			if ( ures_hely[hely] )
			{
				pakli[hely] = referencia_pakli[i];
				ures_hely[hely] = false;
				break;
			}
		}
	}
	printf("Pakli megkeverve.\n");

	i = -1;
	lap = 0;

    while (1)
	{
		i++;
		printf("Játékosléptetés. (jatekos[%d])\n",i%2);
		// az i%2 mindig 0 és 1 értéket vehet fel, így a jatekos[0] és jatekos[1] váltogatják egymást
		soron_van = &jatekos[ i%2 ];
		masik     = &jatekos[ (i+1)%2 ];

		// lap osztása a soron lévőnek
		if (soron_van->jatekallas == JATEKBAN)
		{
			// ez kicsit összetett, a pakliban csak kártyaazonosítók vannak, a nevük és értékük a kartyak tömbben van
			kartya = pakli[lap];
			lap++;
			soron_van->lapok_erteke += kartyak[kartya].ertek;

			if (soron_van->lapok_erteke > 21) soron_van->jatekallas = FUCCS;
		}
		else // nyilván ha a soron lévő játékos nincs már játékban akkor nem kap kártyát, de a menete azért még lezajlik
			kartya = NINCS_KARTYA;

		printf("A soron lévő a %s kártyát húzta, ennyinél tart: %d, és játékállása: %s.\n",
					kartyak[kartya].megnevezes, soron_van->lapok_erteke, jatekallas[ soron_van->jatekallas ]);

		// most elküldöm a másik játékosnak, hogy a soron lévő mit kapott
		buffer[0] = 1; // 1-gyel kezdődő üzenet (lásd: kliens)
		buffer[1] = kartya; // ezt kapta
		buffer[2] = soron_van->lapok_erteke;
		buffer[3] = soron_van->jatekallas;
		kuldes(masik->socket, buffer, 4);

		// most a soron lévőnek is elküdöm mit kapott
		buffer[0] = 2; // 2-essel kezdődő üzenet, ezután valószínű, hogy a kliens vmit lépni fog még
		// a buffer[1 - 3] -at nem kell módosítani, marad az előző
		kuldes(soron_van->socket, buffer, 4);
		
		// most várunk a soron lévő játékos lépésére, de csak ha még játékban van
		if (soron_van->jatekallas == JATEKBAN)
		{
    		fogadas(soron_van->socket, buffer, BUFFER_LENGTH); // a játékos socket-éről várom, hogy jöjjön üzenet
			printf("A játékos üzenete: %s\n",buffer);

        	if (strcmp(buffer, "tartom") == 0)
			{
				soron_van->jatekallas = TARTJA;
				if (soron_van->lapok_erteke < 15) soron_van->jatekallas = RENONSZ;
			}
        	if (strcmp(buffer, "feladom") == 0) soron_van->jatekallas = FELADTA;
		}

		if (soron_van->jatekallas != JATEKBAN && masik->jatekallas != JATEKBAN)
		{
			// kiértékelés és eredményküldés
			if (soron_van->jatekallas == TARTJA && masik->jatekallas != TARTJA) // ha a soron lévő egy pontnál megállt és a másik nem, akkor ő csak renonsz,fuccs vagy feladta állapotú lehet (mivel egyik se túl szerencsés, a soron lévő nyert)
			{
				printf("A soron lévő játékos nyert.\n");
				soron_van->eredmeny = NYERT;
				masik->eredmeny = VESZTETT;
			}
			else if (soron_van->jatekallas != TARTJA && masik->jatekallas == TARTJA) // ellenkező esetben
			{
				printf("A másik játékos nyert.\n");
				soron_van->eredmeny = VESZTETT;
				masik->eredmeny = NYERT;
			}
			else // ez már csak döntetlen lehet
			{
				printf("Döntetlen eredmény alakult ki.\n");
				soron_van->eredmeny = DONTETLEN;
				masik->eredmeny = DONTETLEN;
			}

			sleep(1); // egy kis várakozás, hogy nehogy egybe küldje az összes üzenetet

			// szólunk a soron lévőnek arról, hogy mi az eredmény
			buffer[0] = 3; // 3-as típusú üzenet
			buffer[1] = soron_van->eredmeny;
			kuldes(soron_van->socket, buffer, 2);
			// és most üzenünk a másiknak (a buffer[0] = 3 marad)
			buffer[1] = masik->eredmeny;
			kuldes(masik->socket, buffer, 2);


			// megkérdezzük a soron lévőt, akar-e játszani még
			buffer[0] = 4;
			kuldes(soron_van->socket, buffer, 1);
			fogadas(soron_van->socket, buffer, BUFFER_LENGTH); // és várjuk a válaszát
			if (strcmp(buffer,"vege") == 0)
			{
				printf("A jelenlegi játékos nem szeretne új játékot kezdeni.\n");
				// éretesítések küldése, hogy vége
				buffer[0] = 3;
				buffer[1] = 3;
				kuldes(soron_van->socket, buffer, 2);
				buffer[1] = 4;
				kuldes(masik->socket, buffer, 2);

				printf("Játék vége. A szerver kilép.\n");
				return 0;
			}
			printf("A jelenlegi játékos szeretne mégegyet játszani.\n");

			// megkérdezzük a másik játékost, hogy akar-e játszani
			buffer[0] = 4;
			kuldes(masik->socket, buffer, 1);
			fogadas(masik->socket, buffer, BUFFER_LENGTH); // válasz fogadása
			if (strcmp(buffer,"vege") == 0)
			{
				printf("A másik játékos sajnos nem akar új játékot kezdeni.\n");
				// éretesítések küldése, hogy vége
				buffer[0] = 3;
				buffer[1] = 3;
				kuldes(masik->socket, buffer, 2);
				buffer[1] = 4;
				kuldes(soron_van->socket, buffer, 2);

				printf("Játék vége. A szerver kilép.\n");
				return 0;
			}

			goto Kezdes;
		}
	}

	return 0;
}
