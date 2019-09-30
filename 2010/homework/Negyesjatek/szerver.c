#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

int allapotkuldes(int socket, char tabla[6][7]);
int ellenorzes(char kliens, char tabla[6][7]);
int rak(char kliens, char tabla[6][7], int oszlop, int* korongszam);

int main()
{
    WSADATA wsadata;
    WSAStartup(0x0202, &wsadata);

    int s;
    int player1, player2;
    struct sockaddr_in sa, p1a, p2a;
    int i, j, rcv, on = 1;
    int oszlop, korongszam = 0;
    char buffer[100];
    char tabla[6][7]; //6 sor, 7 oszlop
    int playersock; // aktív játékos sockete
    char player; // aktív játékos ('x' vagy 'o')

    sa.sin_family = AF_INET;
    sa.sin_port = htons(4000);
    sa.sin_addr.s_addr = INADDR_ANY;

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        printf("Sikertelen inditas\n");
        close(s);
        WSACleanup();
        return 0;
    }
    printf("A szerver elindult. Varakozas jatekosokra..\n");

    listen(s, 2);
    player1 = accept(s, (struct sockaddr *)&p1a, NULL);
    send(player1, "Meg varunk a masik jatekosra..", 31, 0);
    printf("Elso jatekos belepett\n");

    listen(s, 2);
    player2 = accept(s, (struct sockaddr *)&p2a, NULL);
    printf("Masodik jatekos belepett\n");

    send(player1, "Elindult a jatek", 17, 0);
    send(player2, "Elindult a jatek", 17, 0);
    printf("Elindult a jatek\n");

    for (i=0;i<6;i++)
        for (j=0;j<7;j++)
            tabla[i][j] = ' '; // tábla feltöltése üres mezõkkel

    playersock = player1;
    player = 'o';

    while ( 1 == 1 )
    {
        send(playersock, "lepj", 5, 0); // azért 5 a hossz mert a lezáró '\0'-t is beleveszem (az kell)

        rcv = recv(playersock, buffer, 100, 0);
        if (rcv <= 0)
        {
            printf("Hiba a kapcsolatban\n");
            break;
        }

        if (strcmp(buffer,"feladom") == 0)
        {
            if (player == 'o')
            {
                printf("Az elso jatekos feladta\n");
                send(player1, "Feladtad a jatekot\n", 21, 0);
                send(player2, "A masik fel feladta. Gratulalok, nyertel!\n", 44, 0);
            }
            else
            {
                printf("A masodik jatekos feladta\n");
                send(player1, "A masik fel feladta. Gratulalok, nyertel!\n", 44, 0);
                send(player2, "Feladtad a jatekot\n", 21, 0);
            }
            send(player1, "kilep", 6, 0);
            send(player2, "kilep", 6, 0);
            break;
        }
        else
        {
            oszlop = buffer[0] - '0'; // ezzel az 1. karaktert számmá alakítom
            if (rak(player, tabla, oszlop, &korongszam))
            {
                allapotkuldes( (player=='o'?player2:player1) , tabla ); // ha én 'o' vagyok, akkor 'x'-nak küldöm a táblát (hogy lássa mit léptem)
                allapotkuldes( playersock , tabla ); // a jelen játékosnak is elküldöm
                printf("Jatekos '%c' korongot rakott le\n",player);

                // ellenõrzések
                if (ellenorzes(player,tabla)) // player nyert-e
                {
                    if (player=='o')
                    {
                        send(player1, "Gratulalok, gyoztel!\n", 23, 0);
                        send(player2, "Vesztettel!\n", 14, 0);
                        printf("Az elso jatekos nyert\n");
                    }
                    else
                    {
                        send(player1, "Vesztettel!\n", 14, 0);
                        send(player2, "Gratulalok, gyoztel!\n", 23, 0);
                        printf("A masodik jatekos nyert\n");
                    }
                    send(player1, "kilep", 6, 0);
                    send(player2, "kilep", 6, 0);
                    break;
                }
                else if (korongszam == 6*7)
                {
                    send(player1, "Dontetlen jatek\n", 18, 0);
                    send(player2, "Dontetlen jatek\n", 18, 0);
                    printf("Nincs tobb lepesi lehetoseg, dontetlen jatek\n");
                    send(player1, "kilep", 6, 0);
                    send(player2, "kilep", 6, 0);
                    break;
                }
            }
            else
            {
                send(playersock, "Hibas lepes\n", 14, 0);
                printf("Jatekos '%c' hibasan lepett\n",player);
                continue; //ezzel visszaugrunk a while ciklus elejére, így meggátolva a ciklus alján a kliensváltást
            }
        }

        // játékosváltás
        printf("Jatekoscsere..\n");
        if (player == 'o')
        {
            player = 'x';
            playersock = player2;
        }
        else
        {
            player = 'o';
            playersock = player1;
        }
    }

    close(player1);
    close(player2);
    close(s);

    printf("A szerver futasa veget ert\n");
    system("pause");

    WSACleanup();

    return 0;
}

int allapotkuldes(int socket, char tabla[6][7])
{
    int i, j;
    char tmp[42]; // 6 bájtos "tabla" string + 6x7 bájtos tábla
    strcpy(tmp, "tabla");
    for (i=0;i<6;i++)
        for (j=0;j<7;j++)
            tmp[6 + i*7+j] = tabla[i][j];
    return send(socket, tmp, 48, 0);
}

int ellenorzes(char kliens, char tabla[6][7])
{
    int i,j;

    // átlók ellenõrzése
    for (i=0;i<3;i++)
    {
        for (j=0;j<4;j++)
        {
            if (tabla[i][j]==kliens&&tabla[i+1][j+1]==kliens&&tabla[i+2][j+2]==kliens&&tabla[i+3][j+3]==kliens) return 1; //  / irány
            if (tabla[i][j+3]==kliens&&tabla[i+1][j+2]==kliens&&tabla[i+2][j+1]==kliens&&tabla[i+3][j]==kliens) return 1; //  \ irány
        }
    }

    // vízszintesek ellenõrzése
    for (i=0;i<6;i++)
    {
        for (j=0;j<4;j++)
        {
            if (tabla[i][j]==kliens&&tabla[i][j+1]==kliens&&tabla[i][j+2]==kliens&&tabla[i][j+3]==kliens) return 1; // - irány
        }
    }

    // függõlegesek ellenõrzése
    for (i=0;i<3;i++)
    {
        for (j=0;j<7;j++)
        {
            if (tabla[i][j]==kliens&&tabla[i+1][j]==kliens&&tabla[i+2][j]==kliens&&tabla[i+3][j]==kliens) return 1; // | irány
        }
    }

    return 0;
}

int rak(char kliens, char tabla[6][7], int oszlop, int* korongszam)
{
    int i;
    if (oszlop < 1 || oszlop > 7) return 0;
    for (i=6;i>0;i--)
    {
        if (tabla[i-1][oszlop-1] == ' ')
        {
            tabla[i-1][oszlop-1] = kliens;
            korongszam++;
            return 1;
        }
    }
    return 0;
}
