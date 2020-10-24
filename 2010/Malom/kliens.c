// Nagyon fontos, hogy a linker be�ll�t�sokn�l meg kell adni a 'libws2_32.a' -t!
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

#define BUFFERLEN 25 //25 b�jtos bufferm�ret

char allapot[3][8]; //glob�lis v�ltoz�
// 3: a malomt�bl�n 3 n�gyzet van (k�ls�, k�z�ps�, bels�)
// 8: minden n�gyzeten van 8 pont ahova b�but lehet rakni

void mutat() // megmutatja a j�t�k�ll�st
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
    int i, j, rc, on = 1; //on: a socket be�ll�t�s�n�l lesz �rtelme
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
            allapot[i][j] = ' '; // felt�lt�s sz�k�z�kkel

    /**
        Protokoll magyar�zat:

        (kapott �zenetn�l)

        - az els� b�jt jelenti a csomag t�pus�t:
            - 0 = v�rakoz�s a m�sik j�t�kosra
            - 1 = a j�t�k elindult
            - 2 = b�but kell leraknunk
            - 3 = l�pn�nk kell
            - 4 = le kell �tni egy ellens�ges b�but
            - 5 = a szerver k�ldi az eg�sz �llapot t�mb�t (m�g 24 b�jt)
            - 6 = j�t�k v�ge, valaki nyert vagy feladta a j�t�kot (m�sodik b�jt: 0 = mi nyert�nk, 1 = a m�sik nyert)
            - 7 = valamilyen hiba t�rt�nt (m�sodik b�jt: 2, 3 vagy 4)

        (k�ld�tt �zenetn�l)

        - 0 = felad�s
        - 1 = lerak�s (a m�sodik b�jt a malom k�ls�(0), k�z�ps�(1) vagy bels�(2) n�gyzete, a harmadik b�jt a n�gyzet egy pontja (a bal fels� sarokt�l kezdve az �ramutat� j�r�s�val megyez� ir�nyban 0-t�l 7-ig))
        - 2 = l�p�s (honnan: m�sodik �s harmadik b�jt az el�z�h�z hasonl�an, hov�: negyedik �s �t�dik b�jt)
        - 3 = le�t�s (hasonl�an a lerak�shoz)
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
            switch (buffer[0]) // az els� b�jt
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
                    buffer[0] = 1; // lerak�s
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
                printf("  n es m: x �s y-hoz hasonloan\n");
                scanf("%s",inputb);
                if(inputb[0] == 'f') send(s,&nulla,1,0);
                else
                {
                    buffer[0] = 2; // l�p�s
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
                    buffer[0] = 3; // le�t�s
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
