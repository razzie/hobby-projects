// Nagyon fontos, hogy a linker beállításoknál meg kell adni a 'libws2_32.a' -t!
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define KULSO 0
#define KOZEPSO 1
#define BELSO 2

#define BAL_FELSO 0
#define FELSO 1
#define JOBB_FELSO 2
#define JOBB 3
#define JOBB_ALSO 4
#define ALSO 5
#define BAL_ALSO 6
#define BAL 7

#define PORT 500
#define IP "127.0.0.1"

#define BUFFERLEN 25 //25 bájtos bufferméret

char allapot[3][8]; //globális változó
// 3: a malomtáblán 3 négyzet van (külsõ, középsõ, belsõ)
// 8: minden négyzeten van 8 pont ahova bábut lehet rakni

void mutat() // megmutatja a játékállást
{
    printf("\n");
    printf("%c-----%c-----%c\n",allapot[KULSO][BAL_FELSO],allapot[KULSO][FELSO],allapot[KULSO][JOBB_FELSO]);
    printf("| %c---%c---%c |\n",allapot[KOZEPSO][BAL_FELSO],allapot[KOZEPSO][FELSO],allapot[KOZEPSO][JOBB_FELSO]);
    printf("| | %c-%c-%c | |\n",allapot[BELSO][BAL_FELSO],allapot[BELSO][FELSO],allapot[BELSO][JOBB_FELSO]);
    printf("%c-%c-%c   %c-%c-%c\n",allapot[KULSO][BAL],allapot[KOZEPSO][BAL],allapot[BELSO][BAL], allapot[KULSO][JOBB],allapot[KOZEPSO][JOBB],allapot[BELSO][JOBB]);
    printf("| | %c-%c-%c | |\n",allapot[BELSO][BAL_ALSO],allapot[BELSO][ALSO],allapot[BELSO][JOBB_ALSO]);
    printf("| %c---%c---%c |\n",allapot[KOZEPSO][BAL_ALSO],allapot[KOZEPSO][ALSO],allapot[KOZEPSO][JOBB_ALSO]);
    printf("%c-----%c-----%c\n",allapot[KULSO][BAL_ALSO],allapot[KULSO][ALSO],allapot[KULSO][JOBB_ALSO]);
    printf("\n");
}

int main()
{
    WSADATA wsadata;
    SOCKET s;
    SOCKADDR_IN addr;
    int i, j, rc, on = 1; //on: a socket beállításánál lesz értelme
    char buffer[BUFFERLEN], inputb[100];
    char nulla = 0;

    //a WSAStartup egy Windows specifikus dolog
    int error = WSAStartup(0x0202, &wsadata);
    if (error || wsadata.wVersion != 0x0202)
    {
        printf("Nem sikerult az inicializalas\n");
        return 0;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    if (connect(s, (SOCKADDR *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        printf("Kapcsolodas sikertelen\n");
        return 0;
    }

    for (i=0;i<3;++i)
        for(j=0;j<7;++j)
            allapot[i][j] = ' '; // feltöltés szóközökkel

    /**
        Protokoll magyarázat:

        (kapott üzenetnél)

        - az elsõ bájt jelenti a csomag típusát:
            - 0 = várakozás a másik játékosra
            - 1 = a játék elindult
            - 2 = bábut kell leraknunk
            - 3 = lépnünk kell
            - 4 = le kell ütni egy ellenséges bábut
            - 5 = a szerver küldi az egész állapot tömböt (még 24 bájt)
            - 6 = játék vége, valaki nyert vagy feladta a játékot (második bájt: 0 = mi nyertünk, 1 = a másik nyert)
            - 7 = valamilyen hiba történt (második bájt: 2, 3 vagy 4)

        (küldött üzenetnél)

        - 0 = feladás
        - 1 = lerakás (a második bájt a malom külsõ(0), középsõ(1) vagy belsõ(2) négyzete, a harmadik bájt a négyzet egy pontja (a bal felsõ saroktól kezdve az óramutató járásával megyezõ irányban 0-tól 7-ig))
        - 2 = lépés (honnan: második és harmadik bájt az elõzõhöz hasonlóan, hová: negyedik és ötödik bájt)
        - 3 = leütés (hasonlóan a lerakáshoz)
    **/

    while (1)
    {
        rc = recv(s, buffer, BUFFERLEN, 0);
        if (rc == 0)
        {
            printf("A kapcsolatnak vege szakadt\n");
            return 0;
        }
        if (rc == SOCKET_ERROR)
        {
            printf("Valamilyen hiba tortent\n");
            return 0;
        }
        Ujra:
        {
            switch (buffer[0]) // az elsõ bájt
            {
            case 0:
                printf("Varakozas a masik jatekosra\n");
                break;
            case 1:
                printf("A jatek elindult\n");
                break;
            case 2:
                printf("Rakj le egy babut!\n");
                printf("Hasznalat: xy  vagy  'feladom'\n");
                printf("  x: a malom kulso(0), kozepso(1) vagy belso(2) negyzete\n");
                printf("  y: az adott negyzeten levo pont\n");
                printf("     a bal felso saroktol kezdve a szamozast oramutato iranyaval megegyezo iranyban (0-7)\n");
                scanf("%s",inputb);
                if(inputb[0] == 'f') send(s,&nulla,1,0);
                else
                {
                    buffer[0] = 1; // lerakás
                    buffer[1] = inputb[0]-'0';
                    buffer[2] = inputb[1]-'0';
                    send(s,buffer,3,0);
                }
                break;
            case 3:
                printf("Lepj!\n");
                printf("Hasznalat: xynm  vagy  'feladom'\n");
                printf(" Honnan: (xy)\n");
                printf("  x: a malom kulso(0), kozepso(1) vagy belso(2) negyzete\n");
                printf("  y: az adott negyzeten levo pont\n");
                printf("     a bal felso saroktol kezdve a szamozast oramutato iranyaval megegyezo iranyban (0-7)\n");
                printf(" Hova: (nm)\n");
                printf("  n es m: x és y-hoz hasonloan\n");
                scanf("%s",inputb);
                if(inputb[0] == 'f') send(s,&nulla,1,0);
                else
                {
                    buffer[0] = 2; // lépés
                    buffer[1] = inputb[0]-'0';
                    buffer[2] = inputb[1]-'0';
                    buffer[3] = inputb[2]-'0';
                    buffer[4] = inputb[3]-'0';
                    send(s,buffer,5,0);
                }
                break;
            case 4:
                printf("Malmod van. Uss le egy babut!\n");
                printf("Hasznalat: xy  vagy  'feladom'\n");
                printf("  x: a malom kulso(0), kozepso(1) vagy belso(2) negyzete\n");
                printf("  y: az adott negyzeten levo pont\n");
                printf("     a bal felso saroktol kezdve a szamozast oramutato iranyaval megegyezo iranyban (0-7)\n");
                scanf("%s",inputb);
                if(inputb[0] == 'f') send(s,&nulla,1,0);
                else
                {
                    buffer[0] = 3; // leütés
                    buffer[1] = inputb[0]-'0';
                    buffer[2] = inputb[1]-'0';
                    send(s,buffer,3,0);
                }
                break;
            case 5:
                for(i=0;i<3;++i)
                    for(j=0;j<8;++j)
                        allapot[i][j] = buffer[1+i*8+j];
                mutat();
                break;
            case 6:
                if (buffer[1] == 0)
                    printf("Gratulatok! Nyertel!\n");
                else
                    printf("Vesztettel!\n");
                return 0;
                break;
            case 7:
                printf("Hiba tortent!\n");
                buffer[0] = buffer[1];
                goto Ujra;
                break;
            }
        }
    }

    WSACleanup();

    return 0;
}
