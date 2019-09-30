#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>

#define NINCS 0
#define VII 1
#define VIII 2
#define IX 3
#define X 4
#define ALSO 5
#define FELSO 6
#define KIRALY 7
#define ASZ 8

#define port 1234

int kliens = 0; // lehet 0 vagy 1, az éppen aktuális játékos
int fopakli[32], fopakli_lapok; // fopakli_lapok : lapok száma a fõpakliban (ha kiveszek egyet akkor csökken)

void masikjatekos() // játékosváltás
{
    kliens = (kliens+1)%2;
}

int jatekos() // éppen aktuális játékos 1 vagy 2;
{
    return kliens+1;
}

void ujpakli() // a fõpaklit újra feltölti ahonnan majd húzni kell
{
    int lap, i, tmp[8]; // a tmp-ben tartom nyilván, hogy melyik kártyalap hányszor jött ki a randomból (kell a pakli feltöltéshez)
    for (i=0;i<8;++i) tmp[i] = 0; // feltölti nullákkal a tmp tömböt

    for (i=0;i<32;++i)
    {
        do
        {
            lap = rand()%8; // kitalálok egy véletlen lapot 0-7;
        }
        while (tmp[lap] >= 4); // ha ez a feltétel igaz akkor visszemegy a do-hoz és új lapot talál ki // minden kártya 4x lehet

        tmp[lap]++; // ebbõl a lapból még egyet betettünk a pakliba (tmp[lap] nõ eggyel)
        fopakli[i] = lap+1;
    }

    fopakli_lapok = 32;
}

int paklimeret(int pakli[5]) // megadjuk neki vmelyik játékos pakliját, és megmondja hány lap van benne
{
    int n = 0, i;
    for (i=0;i<5;++i)
    {
        if (pakli[i] != NINCS)
        {
            n++;
        }
    }
    return n;
}

int lepjen(int socket, int elozmeny, int pakli[5])
{
    int i;
    char tmp[6]; // buffer - 1. bájt az elõzmény lap + a további 5 bájt a pakli lapjai

    tmp[0] = elozmeny; // melyik lapot rakta a másik játékos (1-8)
    for (i=0;i<5;++i) tmp[1+i] = pakli[i]; // ugyanúgy mint a kliensnél (pl string[5] = pakli[4])

    return send(socket, tmp, 6, 0);
}

int pont(int asztal[], int lapok) // megszámolja hogy az asztalra rakott lapok között hány X és Ász van
{
    int i, pont = 0;
    for (i=0;i<lapok;++i)
    {
        if (asztal[i] == X || asztal[i] == ASZ) pont++;
    }
    return pont;
}

// 0: még nincs vége a játéknak, 1: játékos1 nyert, 2: játékos2 nyert
int nyertesvizsgalat(int jatekos1, int jatekos2, int pakli1[5], int pakli2[5], int pontok1, int pontok2, int utolso_uto)
{
    if (pontok1 >= 5)
    {
        send(jatekos1, "nyertel", 8, 0);
        send(jatekos2, "vesztettel", 11, 0);
        printf("Jatekos1 nyert\n");
        return 1;
    }
    if (pontok2 >= 5)
    {
        send(jatekos1, "vesztettel", 11, 0);
        send(jatekos2, "nyertel", 8, 0);
        printf("Jatekos2 nyert\n");
        return 1;
    }

    if (paklimeret(pakli1) == 0 && paklimeret(pakli2) == 0) // mindenkinek elfogyott a lapja
    {
        printf("Mindenkinek elfogyott a lapja\n");

        if (pontok1 > pontok2)
        {
            send(jatekos1, "nyertel", 8, 0);
            send(jatekos2, "vesztettel", 11, 0);
            printf("Jatekos1 nyert\n");
            return 1;
        }
        else if(pontok1 < pontok2)
        {
            send(jatekos1, "vesztettel", 11, 0);
            send(jatekos2, "nyertel", 8, 0);
            printf("Jatekos2 nyert\n");
            return 2;
        }
        else // ha egyenlõ pontjuk van
        {
            if (utolso_uto == 1)
            {
                send(jatekos1, "nyertel", 8, 0);
                send(jatekos2, "vesztettel", 11, 0);
                printf("Jatekos1 nyert\n");
                return 1;
            }
            if (utolso_uto == 2)
            {
                send(jatekos1, "vesztettel", 11, 0);
                send(jatekos2, "nyertel", 8, 0);
                printf("Jatekos2 nyert\n");
                return 2;
            }
        }
    }
    return 0;
}

int main()
{
    int szerver, jatekos1, jatekos2, j; // j: ideiglenesen a játékos1 vagy játékos2 is lehet
    struct sockaddr_in a;
    int pakli1[5], pakli2[5], asztal[20], lapok; // lapok: hány lap van az asztalra kitéve
    int pontok1, pontok2;
    int utolso_lap, utolso_uto;
    int i, n, len, on = 1; // len: send és recv függvények visszatérési értékei vannak benne, hibavizsgálathoz kell
    int jatek_vege, uj_jatek; // számlálók, akkor kellenek amikor vége a játszmának és a szerver megnézi hogy akarnak-e még játszani
    char string[100]; // buffer

    WSADATA wsadata;
    WSAStartup(0x0202, &wsadata);

    srand(time(NULL));

    szerver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(szerver, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on); // sizeof on (az on azt int típusú), azt adja vissza hogy az int típus hány bájtos
    setsockopt(szerver, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    a.sin_family = AF_INET;
    a.sin_port = htons(port);
    a.sin_addr.s_addr = INADDR_ANY;

    if (bind(szerver, (struct sockaddr *)&a, sizeof(a)) < 0) // ha a bind függvény visszatérési értéke kisebb mint 0, akkor az hibát jelent
    {
        printf("Kapcsolodas sikertelen\n");
        close(szerver);
        WSACleanup();
        return 0;
    }
    printf("A szerver elindult\n");

    // -- játékosok fogadása --
    listen(szerver, 2);
    jatekos1 = accept(szerver, (struct sockaddr*)NULL, NULL);
    send(jatekos1, "varakozas", 10, 0);

    listen(szerver, 2);
    jatekos2 = accept(szerver, (struct sockaddr*)NULL, NULL);


    Start: // a Start az egy címke, a goto-val ide vissza tudunk jönni (itt kezdõdik új játék)
    printf("Indul a jatek\n");
    kliens = 0;
    lapok = 0; // változókat minden új játék elején alaphelyzetbe állítom
    jatek_vege = 0;
    uj_jatek = 0;
    ujpakli(); // új paklit készít

    for (i=0;i<5;++i)
    {
        fopakli_lapok--;
        pakli1[i] = fopakli[fopakli_lapok];
    }
    send(jatekos1, "kezdes", 7, 0);
    pontok1 = 0;

    for (i=0;i<5;++i)
    {
        fopakli_lapok--;
        pakli2[i] = fopakli[fopakli_lapok];
    }
    send(jatekos2, "kezdes", 7, 0);
    pontok2 = 0;

    //megmondjuk az elsõ játékosnak hogy kezdjen
    lepjen(jatekos1, 0, pakli1); // jatekos1 lépjen, 0: nem volt elõzmény, pakli1: a saját pakliát küldjük

    while (1)
    {
        j = (jatekos()==1 ? jatekos1 : jatekos2); // j azért kell, hogy az egész kód ne szerepeljen egymás alatt 2x külön a játékos1 és játékos2-vel is
        len = recv(j, string, 100, 0);
        if (len == 0)
        {
            printf("Megszunt a kapcsolat\n");
            break;
        }
        if (len < 0)
        {
            printf("Hiba\n");
            break;
        }

        if (strcmp(string,"feladom") == 0)
        {
            if (jatekos() == 1)
            {
                send(jatekos1, "vesztettel", 11, 0);
                send(jatekos2, "nyertel", 8, 0);
                printf("Jatekos1 feladta\n");
            }
            else
            {
                send(jatekos1, "nyertel", 8, 0);
                send(jatekos2, "vesztettel", 11, 0);
                printf("Jatekos2 feladta\n");
            }
        }
        else if (strcmp(string,"ujra") == 0)
        {
            jatek_vege++; // jatek_vege: azt számolja hogy hányan küldtek ujra-t vagy vege-t
            uj_jatek++;
            if (jatek_vege == 2)
            {
                if (uj_jatek == 2)
                {
                    goto Start;
                }
                else
                {
                    break;
                }
            }
        }
        else if (strcmp(string,"vege") == 0)
        {
            jatek_vege++;
            if (jatek_vege == 2)
            {
                break;
            }
        }
        else
        {
            printf("Jatekos%d lepett\n",jatekos());

            /************** lap kidobása az asztalra ****************/
            if (jatekos() == 1)
            {
                // utolso_lap: a legfelsõ lap az asztalon
                // string[0] a pakli hanyadik lapja (1-5)
                utolso_lap = pakli1[string[0]-1]; // ki kell vonni egyet mert a tömbben 0-4-ig van indexelve
                fopakli_lapok--;
                pakli1[string[0]-1] = fopakli[fopakli_lapok];
            }
            else
            {
                utolso_lap = pakli2[string[0]-1];
                fopakli_lapok--;
                pakli2[string[0]-1] = fopakli[fopakli_lapok];
            }
            asztal[lapok] = utolso_lap; // konkrétan most dobjuk ki az asztalra
            lapok++; // az asztalon lévõ lapok száma eggyel nõ
            /********************************************************/

            /******************** ütéshelyzet **********************/
            if (utolso_lap == asztal[0] || utolso_lap == VII) // ha ütök (megfelelõ lap VAGY hetes)
            {
                utolso_uto = jatekos();

                if (jatekos() == 1)
                {
                    lepjen(jatekos2, utolso_lap, pakli2);
                }
                else
                {
                    lepjen(jatekos1, utolso_lap, pakli1);
                }
            }
            else if (lapok > 1) // ha nem tudtam ütni
            {
                if (utolso_uto == 1)
                {
                    pontok1 += pont(asztal, lapok);
                    lapok = 0; // mivel elvittem mindent, újra 0 lap van az asztalon
                    send(jatekos1, "visz", 5, 0);
                    send(jatekos2, "nemvisz", 8, 0);
                    if (nyertesvizsgalat(jatekos1,jatekos2,pakli1,pakli2,pontok1,pontok2,utolso_uto) == 0)
                    {
                        lepjen(jatekos1, 0, pakli1); // csak akkor utasítjuk új lépésre ha nincs nyeréshelyzet
                    }
                    kliens = 0; // azt jelenti hogy az 1-es játékos következzen majd
                    continue;
                }
                else
                {
                    pontok2 += pont(asztal, lapok);
                    lapok = 0; // mivel elvittem mindent, újra 0 lap van az asztalon
                    send(jatekos1, "nemvisz", 8, 0);
                    send(jatekos2, "visz", 5, 0);
                    if (nyertesvizsgalat(jatekos1,jatekos2,pakli1,pakli2,pontok1,pontok2,utolso_uto) == 0)
                    {
                        lepjen(jatekos2, 0, pakli2); // csak akkor utasítjuk új lépésre ha nincs nyeréshelyzet
                    }
                    kliens = 1; // azt jelenti hogy az 2-es játékos következzen majd
                    continue;
                }
            }
            /**************************************************************************/
        }


        masikjatekos(); // játékoscsere
    }

    close(szerver);
    close(jatekos1);
    close(jatekos2);
    WSACleanup();

    printf("A szerver lezarta a kapcsolatokat es kilep\n");

    return 0;
}
