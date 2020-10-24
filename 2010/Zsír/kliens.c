#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define NINCS 0

#define port 1234
#define ip "127.0.0.1"

int main()
{
    int sock;
    struct sockaddr_in a;
    char* kartya[9] = { "", "VII", "VIII", "IX", "X", "Also", "Felso", "Kiraly", "Asz" };
    int pakli[5];
    int rak, len, on = 1; // rak: az hogy hanyadik paklilapomat fogom rakni (1-5)
    char string[100]; // buffer

    //a WSAStartup egy Windows specifikus dolog
    WSADATA wsadata;
    WSAStartup(0x0202, &wsadata);

    a.sin_family = AF_INET;
    a.sin_port = htons(port);
    a.sin_addr.s_addr = inet_addr(ip);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    if (connect(sock, (struct sockaddr *)&a, sizeof(a)) < 0)
    {
        printf("Kapcsolodas sikertelen\n");
        close(sock);
        WSACleanup();
        return 0;
    }
    printf("Kapcsolodva\n");

    while (1)
    {
        len = recv(sock, string, 100, 0);
        if (len == 0)
        {
            printf("Vege a jateknak, a szerver leallt\n");
            break;
        }
        if (len < 0)
        {
            printf("Hiba\n");
            break;
        }

        if (strcmp(string,"varakozas") == 0)
        {
            printf("Varakozas a masik jatekosra\n");
        }
        else if (strcmp(string,"kezdes") == 0)
        {
            printf("A jatek elkezdodott\n");
        }
        else if (strcmp(string,"nyertel") == 0 || strcmp(string,"vesztettel") == 0) // j�t�k v�ge helyzet
        {
            if (strcmp(string,"nyertel") == 0) printf("Nyertel\n");
            else printf("Vesztettel\n");

            printf("Uj jatek? (ujra / vege)\n");
            Teszt: // a Teszt az egy c�mke, �s a goto-val ide j�n vissza (ha rossz l�p�st adunk meg)
            scanf("%s",string);

            if (strcmp(string,"ujra") == 0)
            {
                send(sock, "ujra", 5, 0);
            }
            else if (strcmp(string,"vege") == 0)
            {
                send(sock, "vege", 5, 0);
                break;
            }
            else // ha nem "ujra" �s nem "vege" akkor az hib�s l�p�snek sz�m�t
            {
                printf("Hibas lepes\n");
                goto Teszt; // visszamegy�nk a Teszt c�mk�hez (�jra beolvas�s fog j�nni)
            }
        }
        else if (strcmp(string,"visz") == 0)
        {
            printf("Elvitted ezt a kort\n");
        }
        else if (strcmp(string,"nemvisz") == 0)
        {
            printf("A masik jatekos vitte ezt a kort\n");
        }
        else
        {
            // ha a kapott �zenet a fentiek k�z�l egyikkel se egyezik akkor a m�sik f�l k�rty�t rakott vagy most j�n az els� l�p�s
            // (a rakott k�rtya azonos�t�ja a string 1. b�jtja (string[0]))
            // a string 2. b�jtja (string[1]) lehet 'i' vagy 'n' att�l f�gg�en hogy a m�sik f�l elvitte-e a k�rt vagy sem
            if (string[0] >=1 && string[0] <= 8)
            {
                printf("A masik fel ezt rakta: %s\n", kartya[string[0]]); // pl kartya[8] az "Asz" lesz
                //if (string[1] == 'i') printf("es elvitte a kort.\n");
            }
            // ut�na m�g elk�ldi a szerver a paklim k�rtyaazonos�t�it (szinkroniz�l�s a szerverbeli paklimmal)
            pakli[0] = string[1]; // a pakli 1. eleme a string 2. eleme (mert a string 1. eleme az a k�rtya amit az ellens�g rakott)
            pakli[1] = string[2];
            pakli[2] = string[3];
            pakli[3] = string[4];
            pakli[4] = string[5];
            printf("Melyik kartyadat jatszod ki? (1-5 vagy feladom)\n");
            printf("A paklim: 1:%s, 2:%s, 3:%s, 4:%s, 5:%s\n",kartya[pakli[0]],kartya[pakli[1]],kartya[pakli[2]],kartya[pakli[3]],kartya[pakli[4]]);
            while (1)
            {
                scanf("%s",string);
                if (strcmp(string,"feladom") == 0)
                {
                    send(sock, "feladom", 8, 0);
                    break;
                }
                rak = string[0] - '0'; // sz�mm� alak�tom a bek�r�s els� karakter�t
                if (rak>=1 && rak<=5 && pakli[rak-1] != NINCS) // a sz�m 1 �s 5 k�z�tt van �S a pakliban nem �res jel van az adott sz�mn�l
                {
                    printf("Ezt raktad: %s\n",kartya[pakli[rak-1]]);
                    pakli[rak-1] = NINCS;
                    string[0] = rak; // a buffer 1. b�jtj�ba beteszem hogy a pakli hanyadik lapj�t raktam
                    send(sock, string, 1, 0); //..�s elk�ld�m
                    break;
                }
                printf("Hibas lepes\n");
            }
        }
    }

    close(sock);
    WSACleanup(); // ez is windows specifikus, linux alatt ki lehet t�r�lni

    return 0;
}
