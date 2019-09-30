#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

void mutat(char sajat[], char ellenfel[])
{
    int i,j;
    printf("\n--------- Sajatjaim: ---------    --------- Ellenfele: ---------\n");
    for(i=0;i<10;i++)
    {
        for(j=0;j<10;j++) printf("[%c]", sajat[i*10+j]);
        printf("    ");
        for(j=0;j<10;j++) printf("[%c]", ellenfel[i*10+j]);
        printf("\n");
    }
}

int main()
{
    WSADATA wsadata;
    WSAStartup(0x0202, &wsadata);

    struct sockaddr_in addr;
    int s;
    int on = 1, r = 0;
    char buffer[206];

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // szerver ip
    addr.sin_port = htons(5000); // szerver port

    r = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0)
    {
        printf("Sikertelen csatlakozas %d,%d\n",WSAGetLastError(),r);
        close(s);
        WSACleanup();
        return 0;
    }

    while ( 1 )
    {
        r = recv(s, buffer, 206, 0);
        if (r == 0)
        {
            printf("Vege a jateknak\n");
            break;
        }
        if (r < 0)
        {
            printf("Hiba tortent\n");
            break;
        }

        if (strcmp(buffer,"lepes") == 0)
        {
            mutat(&buffer[6],&buffer[106]);
            printf("Te kovetkezel\nLepes modja: (sor,oszlop) vagy (feladom)\n");
            scanf("%s",buffer);
            send(s, buffer, strlen(buffer)+1, 0);
        }
        else if (strcmp(buffer,"hiba") == 0)
        {
            printf("Hibas lepes! Probald ujra\n");
            scanf("%s",buffer);
            send(s, buffer, strlen(buffer)+1, 0);
        }
        else
        {
            printf("%s\n",buffer);
        }
    }

    close(s);

    WSACleanup();

    return 0;
}
