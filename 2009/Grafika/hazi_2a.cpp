#include "graphics.h"
#include <math.h>
#include <stdio.h>

#define PI 3.141592653589793238462643

int center_X, center_Y;

void build_centers() {
     center_X = getmaxx() / 2;
     center_Y = getmaxy() / 2;
}

/*double deg2rad(double deg) {
       return (PI/180)*deg;
}*/

void nszogatlok(int n, int R) {
     double step = 2*PI/n;
     int i, j, x, y;
     
     for (i=1; i<n; i++) {
         x = (int)(cos(i*step)*R);
         y = (int)(sin(i*step)*R);
         for (j=1; j<=n-i; j++) {
             moveto( center_X + x, center_Y + y );
             lineto( center_X + (int)(cos( (i+j)*step )*R) , center_Y + (int)(sin( (i+j)*step )*R) );
             delay(100);
         }
     }
}

int main( ) {

    int gd, gm, n, r;
    gd = VGA; gm = VGAMED;
    initgraph( &gd, &gm, "" );

    build_centers();
    
    printf("Szogek szama: "); scanf("%d",&n);
    printf("Sugar: "); scanf("%d",&r);
    
    nszogatlok(n, r);
    
    getch();
    closegraph();
    
    return 0;
}
