#include "graphics.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.141592653589793238462643

typedef struct pont
{
    double x, y;
} PONT;

int aktiv_pont(PONT *pontok, int n, int r) {
    for (int i=0; i<n; i++) if ( (pontok[i].x-egerx)*(pontok[i].x-egerx) + (pontok[i].y-egery)*(pontok[i].y-egery) < r*r ) return i;
    return -1;
}

int main( ) {

    int gd, gm, n, r;
    gd = VGA; gm = VGAMED;
    initgraph( &gd, &gm, "" );


    printf("Szogek szama: ");
    scanf("%d",&n);
    printf("Sugar: ");
    scanf("%d",&r);

     float step = 2*PI/n;
     int i, j, center_X, center_Y,page=0,ap;
     center_X = getmaxx() / 2;
     center_Y = getmaxy() / 2;
     PONT* pontok;
     pontok = (PONT*)malloc(n*sizeof(PONT));

     // tömb elõkészítése
     for (i=0;i<n;i++) {
         pontok[i].x=(center_X + (int)(cos(i*step)*r));
         pontok[i].y=(center_Y + (int)(sin(i*step)*r));
     }


     while(!kbhit()){
        setactivepage(page);
        cleardevice();

        for (i=0; i<n; i++) {
            circle(pontok[i].x,pontok[i].y,5);
            for (j=i; j<n; j++) {
                moveto( pontok[i+j].x, pontok[i+j].y );
                lineto( pontok[(i+j+1)%n].x, pontok[(i+j+1)%n].y );
                //delay(5);
            }
        }

        if (!balgomb) ap = aktiv_pont(pontok,n,10);
        if (balgomb) {
            if (ap > 0) {
                pontok[ap].x = egerx;
                pontok[ap].y = egery;
            } else {
                for (i=0;i<n;i++) {
                    pontok[i].x -= center_X;
                    pontok[i].y -= center_Y;
                }
                center_X = egerx;
                center_Y = egery;
                for (i=0;i<n;i++) {
                    pontok[i].x += center_X;
                    pontok[i].y += center_Y;
                }
            }
        }

        setvisualpage(page);
        page=1-page;
    }

    getch();
    closegraph();

    return 0;
}
