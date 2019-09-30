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

int kliens = 0; // lehet 0 vagy 1, az �ppen aktu�lis j�t�kos
int fopakli[32], fopakli_lapok; // fopakli_lapok : lapok sz�ma a f�pakliban (ha kiveszek egyet akkor cs�kken)

void masikjatekos() // j�t�kosv�lt�s
{
    kliens = (kliens+1)%2;
}

int jatekos() // �ppen aktu�lis j�t�kos 1 vagy 2;
{
    return kliens+1;
}

void ujpakli() // a f�paklit �jra felt�lti ahonnan majd h�zni kell
{
    int lap, i, tmp[8]; // a tmp-ben tartom nyilv�n, hogy melyik k�rtyalap h�nyszor j�tt ki a randomb�l (kell a pakli felt�lt�shez)
    for (i=0;i<8;++i) tmp[i] = 0; // felt�lti null�kkal a tmp t�mb�t

    for (i=0;i<32;++i)
    {
        do
        {
            lap = rand()%8; // kital�lok egy v�letlen lapot 0-7;
        }
        while (tmp[lap] >= 4); // ha ez a felt�tel igaz akkor visszemegy a do-hoz �s �j lapot tal�l ki // minden k�rtya 4x lehet

        tmp[lap]++; // ebb�l a lapb�l m�g egyet betett�nk a pakliba (tmp[lap] n� eggyel)
        fopakli[i] = lap+1;
    }

    fopakli_lapok = 32;
}

int paklimeret(int pakli[5]) // megadjuk neki vmelyik j�t�kos paklij�t, �s megmondja h�ny lap van benne
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
    char tmp[6]; // buffer - 1. b�jt az el�zm�ny lap + a tov�bbi 5 b�jt a pakli lapjai

    tmp[0] = elozmeny; // melyik lapot rakta a m�sik j�t�kos (1-8)
    for (i=0;i<5;++i) tmp[1+i] = pakli[i]; // ugyan�gy mint a kliensn�l (pl string[5] = pakli[4])

    return send(socket, tmp, 6, 0);
}

int pont(int asztal[], int lapok) // megsz�molja hogy az asztalra rakott lapok k�z�tt h�ny X �s �sz van
{
    int i, pont = 0;
    for (i=0;i<lapok;++i)
    {
        if (asztal[i] == X || asztal[i] == ASZ) pont++;
    }
    return pont;
}

// 0: m�g nincs v�ge a j�t�knak, 1: j�t�kos1 nyert, 2: j�t�kos2 nyert
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
        else // ha egyenl� pontjuk van
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
    int szerver, jatekos1, jatekos2, j; // j: ideiglenesen a j�t�kos1 vagy j�t�kos2 is lehet
    struct sockaddr_in a;
    int pakli1[5], pakli2[5], asztal[20], lapok; // lapok: h�ny lap van az asztalra kit�ve
    int pontok1, pontok2;
    int utolso_lap, utolso_uto;
    int i, n, len, on = 1; // len: send �s recv f�ggv�nyek visszat�r�si �rt�kei vannak benne, hibavizsg�lathoz kell
    int jatek_vege, uj_jatek; // sz�ml�l�k, akkor kellenek amikor v�ge a j�tszm�nak �s a szerver megn�zi hogy akarnak-e m�g j�tszani
    char string[100]; // buffer

    WSADATA wsadata;
    WSAStartup(0x0202, &wsadata);

    srand(time(NULL));

    szerver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(szerver, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on); // sizeof on (az on azt int t�pus�), azt adja vissza hogy az int t�pus h�ny b�jtos
    setsockopt(szerver, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    a.sin_family = AF_INET;
    a.sin_port = htons(port);
    a.sin_addr.s_addr = INADDR_ANY;

    if (bind(szerver, (struct sockaddr *)&a, sizeof(a)) < 0) // ha a bind f�ggv�ny visszat�r�si �rt�ke kisebb mint 0, akkor az hib�t jelent
    {
        printf("Kapcsolodas sikertelen\n");
        close(szerver);
        WSACleanup();
        return 0;
    }
    printf("A szerver elindult\n");

    // -- j�t�kosok fogad�sa --
    listen(szerver, 2);
    jatekos1 = accept(szerver, (struct sockaddr*)NULL, NULL);
    send(jatekos1, "varakozas", 10, 0);

    listen(szerver, 2);
    jatekos2 = accept(szerver, (struct sockaddr*)NULL, NULL);


    Start: // a Start az egy c�mke, a goto-val ide vissza tudunk j�nni (itt kezd�dik �j j�t�k)
    printf("Indul a jatek\n");
    kliens = 0;
    lapok = 0; // v�ltoz�kat minden �j j�t�k elej�n alaphelyzetbe �ll�tom
    jatek_vege = 0;
    uj_jatek = 0;
    ujpakli(); // �j paklit k�sz�t

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

    //megmondjuk az els� j�t�kosnak hogy kezdjen
    lepjen(jatekos1, 0, pakli1); // jatekos1 l�pjen, 0: nem volt el�zm�ny, pakli1: a saj�t pakli�t k�ldj�k

    while (1)
    {
        j = (jatekos()==1 ? jatekos1 : jatekos2); // j az�rt kell, hogy az eg�sz k�d ne szerepeljen egym�s alatt 2x k�l�n a j�t�kos1 �s j�t�kos2-vel is
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
            jatek_vege++; // jatek_vege: azt sz�molja hogy h�nyan k�ldtek ujra-t vagy vege-t
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

            /************** lap kidob�sa az asztalra ****************/
            if (jatekos() == 1)
            {
                // utolso_lap: a legfels� lap az asztalon
                // string[0] a pakli hanyadik lapja (1-5)
                utolso_lap = pakli1[string[0]-1]; // ki kell vonni egyet mert a t�mbben 0-4-ig van indexelve
                fopakli_lapok--;
                pakli1[string[0]-1] = fopakli[fopakli_lapok];
            }
            else
            {
                utolso_lap = pakli2[string[0]-1];
                fopakli_lapok--;
                pakli2[string[0]-1] = fopakli[fopakli_lapok];
            }
            asztal[lapok] = utolso_lap; // konkr�tan most dobjuk ki az asztalra
            lapok++; // az asztalon l�v� lapok sz�ma eggyel n�
            /********************************************************/

            /******************** �t�shelyzet **********************/
            if (utolso_lap == asztal[0] || utolso_lap == VII) // ha �t�k (megfelel� lap VAGY hetes)
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
            else if (lapok > 1) // ha nem tudtam �tni
            {
                if (utolso_uto == 1)
                {
                    pontok1 += pont(asztal, lapok);
                    lapok = 0; // mivel elvittem mindent, �jra 0 lap van az asztalon
                    send(jatekos1, "visz", 5, 0);
                    send(jatekos2, "nemvisz", 8, 0);
                    if (nyertesvizsgalat(jatekos1,jatekos2,pakli1,pakli2,pontok1,pontok2,utolso_uto) == 0)
                    {
                        lepjen(jatekos1, 0, pakli1); // csak akkor utas�tjuk �j l�p�sre ha nincs nyer�shelyzet
                    }
                    kliens = 0; // azt jelenti hogy az 1-es j�t�kos k�vetkezzen majd
                    continue;
                }
                else
                {
                    pontok2 += pont(asztal, lapok);
                    lapok = 0; // mivel elvittem mindent, �jra 0 lap van az asztalon
                    send(jatekos1, "nemvisz", 8, 0);
                    send(jatekos2, "visz", 5, 0);
                    if (nyertesvizsgalat(jatekos1,jatekos2,pakli1,pakli2,pontok1,pontok2,utolso_uto) == 0)
                    {
                        lepjen(jatekos2, 0, pakli2); // csak akkor utas�tjuk �j l�p�sre ha nincs nyer�shelyzet
                    }
                    kliens = 1; // azt jelenti hogy az 2-es j�t�kos k�vetkezzen majd
                    continue;
                }
            }
            /**************************************************************************/
        }


        masikjatekos(); // j�t�koscsere
    }

    close(szerver);
    close(jatekos1);
    close(jatekos2);
    WSACleanup();

    printf("A szerver lezarta a kapcsolatokat es kilep\n");

    return 0;
}
