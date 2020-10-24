#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

int kuld(int socket, char buffer[])
{
    return send(socket, buffer, strlen(buffer)+1, 0);
}

int rak(int sor, int oszlop, char tabla[10][10])
{
    if (tabla[sor][oszlop] == ' ')
    {
        tabla[sor][oszlop] = 'o';
        return 1; // tudtam rakni
    }
    return 0; // nem tudtam rakni
}

int helyzetmutatas(int socket, char tabla1[10][10], char tabla2[10][10])
{
    int i,j;
    char tmp[206]; // 6 bájtos "lepes" szó + 10x10bájtos tábla1 + 10x10 bájtos tábla2

    strcpy(tmp,"lepes"); // a "lepes" -ot berakom a tmp elejére

    for (i=0;i<10;i++)
    {
        for (j=0;j<10;j++)
        {
            // az elsõ táblából mindent átrakok, mert az az enyém
            tmp[6 + i*10+j] = tabla1[i][j];

            // a második táblából csak a találatokat mutatom
            if (tabla2[i][j] == 'x') tmp[106 + i*10+j] = 'x';
            else tmp[106 + i*10+j] = ' ';
        }
    }

    return send(socket, tmp, 206, 0);
}

int torpedo(int sor, int oszlop, char tabla[10][10])
{
    if (tabla[sor][oszlop] == 'o')
    {
        tabla[sor][oszlop] = 'x';
        return 1; // talált
    }
    else if(tabla[sor][oszlop] == ' ')
    {
        tabla[sor][oszlop] = '.'; // megjelölöm h itt már próbálkoztam
        return 0; // nem talált
    }
    else
    {
        return -1; // már volt
    }
}

int main()
{
    WSADATA wsadata;
    WSAStartup(0x0202, &wsadata);

    int s, aktiv1, aktiv2, en, ellen;
    struct sockaddr_in addr;
    int r, on = 1;
    int sor, oszlop, hajok1 = 0, hajok2 = 0, lerakasok = 0;
    char buffer[100];
    char tabla1[10][10], tabla2[10][10];
    int jatekos = 0;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    r = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0)
    {
        printf("Sikertelen inditas\n");
        close(s);
        WSACleanup();
        return 0;
    }
    printf("A szerver elindult.\n");

    listen(s, 2);
    aktiv1 = accept(s, NULL, NULL);
    kuld(aktiv1, "Varunk a masik jatekosra");
    printf("Az elso kliens sikeresen csatlakozott\n");

    listen(s, 2);
    aktiv2 = accept(s, NULL, NULL);
    printf("A masodik kliens sikeresen csatlakozott\n");

    kuld(aktiv1, "Elindult a jatek");
    kuld(aktiv2, "Elindult a jatek");
    printf("Elindult a jatek\n");

    memset(tabla1, ' ', 100);
    memset(tabla2, ' ', 100);

    while ( 1 )
    {
        if (jatekos == 0)
        {
            en = aktiv1;
            ellen = aktiv2;
            helyzetmutatas(aktiv1,tabla1,tabla2);
        }
        else
        {
            en = aktiv2;
            ellen = aktiv1;
            helyzetmutatas(aktiv2,tabla2,tabla1);
        }

        r = recv(en, buffer, 100, 0);
        if (r <= 0)
        {
            printf("Hiba tortent\n");
            break;
        }

        if (strcmp(buffer,"feladom") == 0)
        {
            printf("A(z) %d. jatekos feladta a jatekot\n",jatekos+1);
            kuld(en,    "Feladtad a jatekot");
            kuld(ellen, "A masik jatekos feladta");
            break;
        }
        else
        {
            sscanf(buffer,"%d,%d",&sor,&oszlop);
            if (sor>=1 && sor<=10 && oszlop>=1 && oszlop<=10)
            {
                sor--; //azért mert a tábla 0,0 kezdõindexû
                oszlop--;

                if (lerakasok < 10)
                {
                    if (lerakasok < 2) kuld(en, "A hajok lerakasa kovetkezik");

                    if (jatekos == 0)
                    {
                        if (rak(sor,oszlop,tabla1))
                        {
                            lerakasok++;
                            hajok1++;
                            if (lerakasok > 8) kuld(aktiv1, "A tamadas szakasz kovetkezik");
                        }
                        else
                        {
                            kuld(aktiv1,"hiba");
                            continue;
                        }
                    }
                    else
                    {
                        if (rak(sor,oszlop,tabla2))
                        {
                            lerakasok++;
                            hajok2++;
                            if (lerakasok > 8) kuld(aktiv2, "A tamadas szakasz kovetkezik");
                        }
                        else
                        {
                            kuld(aktiv2,"hiba");
                            continue;
                        }
                    }
                }
                else
                {
                    if (jatekos == 0)
                    {
                        r = torpedo(sor,oszlop,tabla2);
                        if (r == 0)
                        {
                            printf("Jatekos 1 nem talalt\n");
                            kuld(aktiv1,"Nem talalt");
                        }
                        else if (r == 1)
                        {
                            printf("Jatekos 1 talalt\n");
                            kuld(aktiv1,"Talalt");
                            hajok2--;
                            if (hajok2 == 0)
                            {
                                kuld(aktiv1, "Gratulalok, gyoztel!");
                                kuld(aktiv2, "Vesztettel");
                                break;
                            }
                        }
                        else
                        {
                            kuld(aktiv1,"hiba");
                            continue;
                        }
                    }
                    else
                    {
                        r = torpedo(sor,oszlop,tabla1);
                        if (r == 0)
                        {
                            printf("Jatekos 2 nem talalt\n");
                            kuld(aktiv2,"Nem talalt");
                        }
                        else if (r == 1)
                        {
                            printf("Jatekos 2 talalt\n");
                            kuld(aktiv2,"Talalt");
                            hajok1--;
                            if (hajok1 == 0)
                            {
                                kuld(aktiv1, "Vesztettel");
                                kuld(aktiv2, "Gratulalok, gyoztel!");
                                break;
                            }
                        }
                        else
                        {
                            kuld(aktiv2,"hiba");
                            continue;
                        }
                    }
                }
            }
            else
            {
                kuld(en,"hiba");
                continue;
            }
        }

        jatekos++;
        jatekos %= 2; // jatekos = jatekos%2;    %2  : kettõvel való osztás maradéka (0 vagy 1 lehet)
    }

    printf("Kapcsolatok bontasa es kilepes\n");

    close(aktiv1);
    close(aktiv2);
    close(s);

    WSACleanup();

    return 0;
}
