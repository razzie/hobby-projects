#include "graphics.h"
#include <math.h>
#include <stdio.h>

#define PI 3.141592653589793238462643

typedef struct pont
{
    double x, y;
} PONT;

int aktiv_pont(PONT *pontok, int n, int r) {
    for (int i=0; i<n; i++) if ( (pontok[i].x-egerx)*(pontok[i].x-egerx) + (pontok[i].y-egery)*(pontok[i].y-egery) < r*r ) return i;
    return -1;
}

void nszogatlok(int n, int R) {
     float step = 2*PI/n;
     int i, j, x, y,x2,y2, center_X, center_Y,page=0,ap;
     center_X = getmaxx() / 2;
     center_Y = getmaxy() / 2;
     PONT pontok[12],p[4]={center_X,center_Y};


     for (i=1; i<n; i++) {
         x = (int)(cos(i*step)*R);
         y = (int)(sin(i*step)*R);


         for (j=1; j<=n-i; j++) {
         x2=(center_X + x);
         y2=(center_Y + y);
         pontok[i].x=x2;
         pontok[i].y=y2;
         }
     }

             while(!kbhit()){
              setactivepage(page);
              cleardevice();
             for (i=1; i<n; i++) {
                 circle(pontok[i].x,pontok[i].y,5);
                 for (j=1; j<=n; j++) {
             moveto( pontok[i].x, pontok[i].y );
             lineto( pontok[(i+j)%n].x, pontok[(i+j)%n].y);
             }

     }





            // delay(50);



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

}

int main( ) {

    int gd, gm, n, r;
    gd = VGA; gm = VGAMED;
    initgraph( &gd, &gm, "" );


    printf("Szogek szama: ");
    scanf("%d",&n);
    printf("Sugar: ");
    scanf("%d",&r);

    nszogatlok(n, r);

    getch();
    closegraph();

    return 0;
}



