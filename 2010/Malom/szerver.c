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

#define BUFFERLEN 25

char allapot[3][8];

void allapot_kuldes(char* buffer, SOCKET s)
{
    int i,j;
    for(i=0;i<3;++i)
        for(j=0;j<8;++j)
            buffer[1+i*8+j] = allapot[i][j];
    buffer[0] = 5;
    send(s, buffer, 25, 0);
}

int korongok(char kliens)
{
    int i,j,szam = 0;
    for (i=0;i<3;++i)
        for (j=0;j<8;++j)
            if (allapot[i][j] == kliens) szam++;
    return szam;
}

int helytelen(int x, int y)
{
    if (x < 0 || x > 2 || y < 0 || y > 7) return 1;
    return 0;
}

int lerak(char kliens, int x, int y)
{
    if (helytelen(x,y)) return 0;
    if (allapot[x][y] != ' ') return 0;
    allapot[x][y] = kliens;
    return 1;
}

int lep(char kliens, int x, int y, int n, int m)
{
    if (helytelen(x,y)||helytelen(n,m)) return 0;
    if (allapot[x][y] != kliens) return 0;
    if (x == n) // a jelenlegi és cél négyzet ugyanaz
    {
        if ( (y+1)%8 != m && (y+8-1)%8 != m ) return 0; //túl messzire lépés
    }
    else
    {
        if ( x-n < -1 || x-n > 1 ) return 0; //túl messzire lépés
        if ( y != m ) return 0; //máshova lépés
        if ( y == BAL_FELSO || y == JOBB_FELSO || y == JOBB_ALSO || y == BAL_ALSO ) return 0; //átlósan nem lehet lépni
    }
    allapot[x][y] = ' ';
    allapot[n][m] = kliens;
    return 1;
}

int levesz(char kliens, int x, int y)
{
    if (helytelen(x,y)) return 0;
    if (allapot[x][y] != ' ' && allapot[x][y] != kliens)
    {
        allapot[x][y] = ' ';
        return 1;
    }
    return 0;
}

int malom(char kliens)
{
    int i;
    for (i=0;i<3;++i)
    {
        if (allapot[i][BAL_FELSO] == kliens && allapot[i][FELSO] == kliens && allapot[i][JOBB_FELSO] == kliens) return 1;
        if (allapot[i][JOBB_FELSO] == kliens && allapot[i][JOBB] == kliens && allapot[i][JOBB_ALSO] == kliens) return 1;
        if (allapot[i][BAL_FELSO] == kliens && allapot[i][BAL] == kliens && allapot[i][BAL_ALSO] == kliens) return 1;
        if (allapot[i][BAL_ALSO] == kliens && allapot[i][ALSO] == kliens && allapot[i][JOBB_ALSO] == kliens) return 1;
    }
    if (allapot[KULSO][BAL] == kliens && allapot[KOZEPSO][BAL] == kliens && allapot[BELSO][BAL] == kliens) return 1;
    if (allapot[KULSO][JOBB] == kliens && allapot[KOZEPSO][JOBB] == kliens && allapot[BELSO][JOBB] == kliens) return 1;
    if (allapot[KULSO][FELSO] == kliens && allapot[KOZEPSO][FELSO] == kliens && allapot[BELSO][FELSO] == kliens) return 1;
    if (allapot[KULSO][ALSO] == kliens && allapot[KOZEPSO][ALSO] == kliens && allapot[BELSO][ALSO] == kliens) return 1;
    return 0;
}

void hiba(SOCKET s)
{
    char hiba = 7;
    send(s, &hiba, 1, 0);
}

int main()
{
    WSADATA wsadata;
    SOCKET s, c1, c2;
    SOCKADDR_IN addr, c1_addr, c2_addr;
    int rc,i,j,on = 1;
    int c1_addr_len = sizeof(SOCKADDR_IN);
    int c2_addr_len = sizeof(SOCKADDR_IN);
    char buffer[BUFFERLEN];
    int lerakasok = 0, most_jon = 0;

    //a WSAStartup egy Windows specifikus dolog
    int error = WSAStartup(0x0202, &wsadata);
    if (error || wsadata.wVersion != 0x0202)
    {
        printf("Nem sikerult az inicializalas\n");
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

    for (i=0;i<3;++i)
        for(j=0;j<8;++j)
            allapot[i][j] = ' '; // feltöltés szóközökkel

    // elsõ kliens fogadása
    listen(s, 2);
    c1 = accept(s, (struct sockaddr*) &c1_addr, &c1_addr_len);
    printf("Az elso jatekos csatlakozott\n");
    buffer[0] = 0;
    send(c1, buffer, 1, 0);

    // második kliens fogadása
    listen(s, 2);
    c2 = accept(s, (struct sockaddr*) &c2_addr, &c2_addr_len);
    printf("A masodik jatekos csatlakozott\n");

    //kezdõdik a játék
    buffer[0] = 1;
    send(c1, buffer, 1, 0);
    send(c2, buffer, 1, 0);
    printf("A jatek elkezdodott\n");

    while ( 1 )
    {
        if (lerakasok < 18) buffer[0] = 2;
        else buffer[0] = 3;
        send( (most_jon==0 ? c1 : c2), buffer, 1, 0);

        Varakozas:
        {
            rc = recv( (most_jon==0 ? c1 : c2), buffer, 25, 0); //várom a választ
            //printf("Debug: %c %c %c %c (jatekos%d)\n",buffer[1]+'0',buffer[2]+'0',buffer[3]+'0',buffer[4]+'0',most_jon+1);
            if (rc == 0)
            {
                printf("Jatekos%d kilepett. Leallas..\n", most_jon+1);
                return 0;
            }
            if (rc == SOCKET_ERROR)
            {
                printf("Hiba tortent!\n");
                return 0;
            }
            switch (buffer[0])
            {
            case 0:
                buffer[0] = 6; //feladás
                buffer[1] = 0;
                send( (most_jon==0 ? c2 : c1), buffer, 2, 0);
                buffer[1] = 1;
                send( (most_jon==0 ? c1 : c2), buffer, 2, 0);
                printf("Jatekos%d feladta. Jatek vege.\n", most_jon+1);
                return 0;
                break;
            case 1:
                if ( lerak((most_jon==0 ? 'o' : 'x'),buffer[1],buffer[2]) )
                {
                    allapot_kuldes( buffer , (most_jon==0 ? c2 : c1) );
                    lerakasok++;
                    printf("Jatekos%d korongot rakott le\n", most_jon+1);
                    if (malom(most_jon==0 ? 'o' : 'x'))
                    {
                        buffer[0] = 4;
                        send( (most_jon==0 ? c1 : c2), buffer, 1, 0);
                        printf("Jatekos%d malmot csinalt\n", most_jon+1);
                        goto Varakozas;
                    }
                }
                else
                {
                    buffer[0] = 7; // hiba
                    buffer[1] = 2; // bábut kell lerakni (megint)
                    send( (most_jon==0 ? c1 : c2), buffer, 2, 0);
                    printf("Jatekos%d hibasan lepett\n", most_jon+1);
                    goto Varakozas;
                }
                break;
            case 2:
                if ( lep((most_jon==0 ? 'o' : 'x'),buffer[1],buffer[2],buffer[3],buffer[4]) )
                {
                    allapot_kuldes( buffer , (most_jon==0 ? c2 : c1) );
                    printf("Jatekos%d lepett\n", most_jon+1);
                    if (malom(most_jon==0 ? 'o' : 'x'))
                    {
                        buffer[0] = 4;
                        send( (most_jon==0 ? c1 : c2), buffer, 1, 0);
                        printf("Jatekos%d malmot csinalt\n", most_jon+1);
                        goto Varakozas;
                    }
                }
                else
                {
                    buffer[0] = 7; // hiba
                    buffer[1] = 3; // lépni kell (megint)
                    send( (most_jon==0 ? c1 : c2), buffer, 2, 0);
                    printf("Jatekos%d hibasan lepett\n", most_jon+1);
                    goto Varakozas;
                }
                break;
            case 3:
                if ( levesz((most_jon==0 ? 'o' : 'x'),buffer[1],buffer[2]) )
                {
                    allapot_kuldes( buffer , (most_jon==0 ? c2 : c1) );
                    printf("Jatekos%d egy ellenseges korongot vett le\n", most_jon+1);
                    if (korongok(most_jon==0 ? 'o' : 'x') < 3)
                    {
                        buffer[0] = 6; //gyõzelem
                        buffer[1] = 0;
                        send( (most_jon==0 ? c1 : c2), buffer, 2, 0);
                        buffer[1] = 1;
                        send( (most_jon==0 ? c2 : c1), buffer, 2, 0);
                        printf("Jatekos%d gyozott!\n", most_jon+1);
                    }
                }
                else
                {
                    buffer[0] = 7; // hiba
                    buffer[1] = 4; // bábut kell lerakni (megint)
                    send( (most_jon==0 ? c1 : c2), buffer, 2, 0);
                    printf("Jatekos%d hibasan lepett\n", most_jon+1);
                    goto Varakozas;
                }
                break;
            }
        }
        most_jon = (most_jon+1)%2;
    }

    WSACleanup();

    return 0;
}
