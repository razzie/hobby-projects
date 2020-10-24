// Nagyon fontos, hogy a linker be�ll�t�sokn�l meg kell adni a 'libws2_32.a' -t!
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
        Protokoll magyar�zat:

        (fogadott �zenetn�l)
        Az els� b�jt jelenti az �zenet t�pus�t
         - 'v' : v�rakoz�s a m�sik j�t�kosra
         - 'j' : a j�t�k elkezd�d�tt, a m�sodik b�jt ilyenkor 1 vagy 2 (1. vagy 2. j�t�kos vagy)
         - 'd' : dobt�l, a m�sodik b�jt a dobott sz�m (1-6), a k�vetkez� n�gy b�jt az 1. j�t�kos, majd a 2. j�t�kos k�t b�buj�nak �ll�sa
         - 'x' : j�t�k v�ge, a m�sodik b�jt lehet 1 vagy 2 (az 1. vagy 2. j�t�kos nyert)

        (k�ld�tt �zenetn�l)
         - 'f' : felad�s
         - 'l' : l�p�s, a m�sodik b�jt 1 vagy 2 (1. vagy 2. b�buval l�pek)
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
        switch (buffer[0]) // az els� b�jt
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
