// Nagyon fontos, hogy a linker beállításoknál meg kell adni a 'libws2_32.a' -t!
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // a random miatt kell
#include <winsock2.h>

#define PORT 500

#define BUFFER 6

int main()
{
    SOCKET s, k1, k2;
    SOCKADDR_IN addr;
    int rc, on = 1;
    int jatekos = 1;
    int kocka;
    int j1_b1 = 1, j1_b2 = 1, j2_b1 = 1, j2_b2 = 1; // bábuk állása
    char buffer[BUFFER];

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
    addr.sin_addr.s_addr = 0;

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    if (bind(s, (SOCKADDR *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        printf("A szerver inditasa sikertelen\n");
        return 0;
    }

    // elsõ kliens fogadása
    listen(s, 2);
    k1 = accept(s, NULL, NULL);
    printf("Az 1. jatekos csatlakozott\n");
    buffer[0] = 'v';
    send(k1, buffer, 1, 0);

    // második kliens fogadása
    listen(s, 2);
    k2 = accept(s, NULL, NULL);
    printf("A 2. jatekos csatlakozott\n");

    //kezdõdik a játék
    buffer[0] = 'j';
    buffer[1] = 1; //az elsõ játékosnak megmondom, hogy õ az 1.
    send(k1, buffer, 2, 0);
    buffer[1] = 2; //a második játékosnak..... 2.
    send(k2, buffer, 2, 0);
    printf("A jatek kezdodik\n");

    srand(time(NULL));

    while ( 1 )
    {
        kocka = rand()%6 + 1;
        buffer[0] = 'd'; //dobás történt
        buffer[1] = kocka;
        buffer[2] = j1_b1; //játékos1_bábu1 stb stb..
        buffer[3] = j1_b2;
        buffer[4] = j2_b1;
        buffer[5] = j2_b2;
        send( (jatekos==1 ? k1 : k2), buffer, 6, 0);
        rc = recv( (jatekos==1 ? k1 : k2), buffer, 6, 0); // fogadás
        if (rc == 0)
        {
            printf("Hiba! A(z) %d. jatekos kilepett.\n", jatekos);
            return 0;
        }
        if (rc == SOCKET_ERROR)
        {
            printf("Hiba tortent!\n");
            return 0;
        }
        switch (buffer[0])
        {
            case 'f':
                buffer[0] = 'x'; //véget vetünk a játéknak, mert vki feladta
                buffer[1] = (jatekos==1 ? 2 : 1); //ha az 1-es játékos adta fel, akkor a 2-es nyer
                send(k1, buffer, 2, 0);
                send(k2, buffer, 2, 0);
                printf("A(z) %d. jatekos feladta. Jatek vege.\n", jatekos);
                return 0;
                break;
            case 'l':
                if (jatekos == 1) // az 1. játékos lépett
                {
                    if (buffer[1] == 1)
                    {
                        j1_b1 += kocka;
                        if (j1_b1 < 50 && j1_b1 == j1_b2) j1_b2 = 1;
                        if (j1_b1 < 50 && j1_b1 == j2_b1) j2_b1 = 1;
                        if (j1_b1 < 50 && j1_b1 == j2_b2) j2_b2 = 1;
                    }
                    else
                    {
                        j1_b2 += kocka;
                        if (j1_b2 < 50 && j1_b2 == j1_b1) j1_b1 = 1;
                        if (j1_b2 < 50 && j1_b2 == j2_b1) j2_b1 = 1;
                        if (j1_b2 < 50 && j1_b2 == j2_b2) j2_b2 = 1;
                    }
                    if (j1_b1 > 49 && j1_b2 > 49) // az 1. játékos nyert
                    {
                        buffer[0] = 'x'; //valaki nyert
                        buffer[1] = 1; //az 1. játékos nyert
                        send(k1, buffer, 2, 0);
                        send(k2, buffer, 2, 0);
                        printf("Az elso jatekos nyert. Jatek vege.\n");
                        return 0;
                        break;
                    }
                }
                else // a 2. játékos lépett
                {
                    if (buffer[1] == 1)
                    {
                        j2_b1 += kocka;
                        if (j2_b1 < 50 && j2_b1 == j1_b1) j1_b1 = 1;
                        if (j2_b1 < 50 && j2_b1 == j1_b2) j1_b2 = 1;
                        if (j2_b1 < 50 && j2_b1 == j2_b2) j2_b2 = 1;
                    }
                    else
                    {
                        j2_b2 += kocka;
                        if (j2_b2 < 50 && j2_b2 == j1_b1) j1_b1 = 1;
                        if (j2_b2 < 50 && j2_b2 == j1_b2) j1_b2 = 1;
                        if (j2_b2 < 50 && j2_b2 == j2_b1) j2_b1 = 1;
                    }
                    if (j2_b1 > 49 && j2_b2 > 49) // a 2. játékos nyert
                    {
                        buffer[0] = 'x'; //valaki nyert
                        buffer[1] = 2; //a 2. játékos nyert
                        send(k1, buffer, 2, 0);
                        send(k2, buffer, 2, 0);
                        printf("A masodik jatekos nyert. Jatek vege.\n");
                        return 0;
                        break;
                    }
                }
                break;
        }
        jatekos++;
        if (jatekos > 2) jatekos -= 2;
    }

    WSACleanup();

    return 0;
}
