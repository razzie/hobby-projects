// Nagyon fontos, hogy a linker beállításoknál meg kell adni a 'libws2_32.a' -t!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define IP_ADDR "127.0.0.1"
#define PORT 500

#define BUFFER 6

int main()
{
    SOCKET s;
    SOCKADDR_IN addr;
    int rc, on = 1;
    char buffer[BUFFER], bevitel[10];

    //a WSAStartup egy Windows specifikus dolog
    WSADATA wsadata;
    int error = WSAStartup(0x0202, &wsadata);
    if (error || wsadata.wVersion != 0x0202)
    {
        printf("Hiba tortent az inditas soran\n");
        return 0;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP_ADDR);

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    if (connect(s, (SOCKADDR *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        printf("Nem lehet kapcsolodni!\n");
        return 0;
    }

    /**
        Protokoll magyarázat:

        (fogadott üzenetnél)
        Az elsõ bájt jelenti az üzenet típusát
         - 'v' : várakozás a másik játékosra
         - 'j' : a játék elkezdõdött, a második bájt ilyenkor 1 vagy 2 (1. vagy 2. játékos vagy)
         - 'd' : dobtál, a második bájt a dobott szám (1-6), a következõ négy bájt az 1. játékos, majd a 2. játékos két bábujának állása
         - 'x' : játék vége, a második bájt lehet 1 vagy 2 (az 1. vagy 2. játékos nyert)

        (küldött üzenetnél)
         - 'f' : feladás
         - 'l' : lépés, a második bájt 1 vagy 2 (1. vagy 2. bábuval lépek)
    **/

    while ( 1 )
    {
        rc = recv(s, buffer, BUFFER, 0);
        if (rc == 0)
        {
            printf("Megszakadt a kapcsolat\n");
            return 0;
        }
        if (rc == SOCKET_ERROR)
        {
            printf("Egy hiba miatt a program leall\n");
            return 0;
        }
        switch (buffer[0]) // az elsõ bájt
        {
            case 'v':
                printf("Varakozas a masik jatekosra\n");
                break;
            case 'j':
                printf("A jatek elindult\n");
                printf("Te vagy a(z) %d. jatekos\n",buffer[1]);
                break;
            case 'd':
                printf("A dobasod: %d\n", buffer[1]);
                printf("Jatekallas:\n");
                printf(" - 1. jatekos\n");
                printf("    - 1. babu helye: %d\n", buffer[2]);
                printf("    - 2. babu helye: %d\n", buffer[3]);
                printf(" - 2. jatekos\n");
                printf("    - 1. babu helye: %d\n", buffer[4]);
                printf("    - 2. babu helye: %d\n", buffer[5]);
                printf("Lehetseges lepesek: 1, 2 vagy feladom\n");
                printf("(1: elso babu, 2: masodik babu, feladom: jatek feladasa)\n");
                Ujra:
                {
                    scanf("%s",bevitel);
                    if (strcmp(bevitel,"1") == 0)
                    {
                        buffer[0] = 'l'; //l: lepes
                        buffer[1] = 1; //az elso babuval
                        send(s, buffer, 2, 0);
                    }
                    else if (strcmp(bevitel,"2") == 0)
                    {
                        buffer[0] = 'l';
                        buffer[1] = 2;
                        send(s, buffer, 2, 0);
                    }
                    else if (strcmp(bevitel,"feladom") == 0)
                    {
                        buffer[0] = 'f';
                        send(s, buffer, 1, 0);
                    }
                    else
                    {
                        printf("Hibas lepes!\n");
                        goto Ujra;
                    }
                }
                break;
            case 'x':
                printf("Vege a jateknak!\n");
                printf("A(z) %d. jatekos nyert\n", buffer[1]);
                return 0;
                break;
        }
    }

    WSACleanup();

    return 0;
}
