#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

void rajzol();

int main()
{
    WSADATA wsadata;
    WSAStartup(0x0202, &wsadata);

    struct sockaddr_in sa;
    int s; // socket
    int on = 1, rcv;
    char buffer[100];

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(4000); // port
    sa.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip

    if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        printf("Nem tudtam csatlakozni\n");
        close(s);
        WSACleanup();
        return 0;
    }

    while ( 1 == 1 )
    {
        rcv = recv(s, buffer, 100, 0);
        if (rcv <= 0)
        {
            printf("Hiba a kapcsolatban\n");
            break;
        }

        if (strcmp(buffer,"kilep") == 0)
        {
            printf("Jatek vege\n");
            break;
        }
        else if (strcmp(buffer,"lepj") == 0)
        {
            printf("Melyik oszlopba teszel korongot? (1-7 vagy feladom)\n");
            scanf("%s",buffer);
            send(s, buffer, strlen(buffer)+1, 0);
        }
        else if (strcmp(buffer,"tabla") == 0)
        {
            rajzol(&buffer[6]); // a buffer "tabla" utáni részét átpasszolom a rajzoló függvénynek
        }
        else
        {
            printf("%s\n",buffer);
        }
    }

    close(s);
    system("pause");

    WSACleanup();

    return 0;
}

void rajzol(char* tabla)
{
    int i,j;
    printf("\nJatek allasa:\n");
    for(i=0;i<6;i++)
    {
        for(j=0;j<7;j++)
        {
            printf("[%c]", tabla[i*7+j]); // úgy járom be a táblát mintha kétdimenziós tömb lenne
        }
        printf("\n");
    }
}
