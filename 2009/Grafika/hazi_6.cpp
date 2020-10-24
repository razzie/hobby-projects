# include "graphics.h"
# include <stdlib.h>
# include <math.h>
# include <stdio.h>

#define PI 3.141592653589793238462643

typedef struct pont {
     int x, y, e;
} PONT;

int centerX, centerY;

PONT pont_(int x, int y) {
     PONT p;
     p.x = x;
     p.y = y;
     p.e = 1;
     return p;
}

PONT metsz_illeszt(PONT p1, PONT p2) {
     PONT retval;
     retval.x =   p1.y * p2.e - p1.e * p2.y;
     retval.y = - p1.x * p2.e + p1.e * p2.x;
     retval.e =   p1.x * p2.y - p1.y * p2.x;
     return retval;
}

void draw_poly(PONT *poly, int n) {
     for (int i=0;i<n;i++) line(poly[i].x/poly[i].e,poly[i].y/poly[i].e, poly[(i+1)%n].x/poly[(i+1)%n].e,poly[(i+1)%n].y/poly[(i+1)%n].e);
}

void fill_poly(PONT *poly, int n) {
     int p[n*2];
     for (int i=0;i<n;i++) {
         if (poly[i].e == 0) return;
         p[i*2] = poly[i].x/poly[i].e;
         p[i*2+1] = poly[i].y/poly[i].e;
     }
     fillpoly(n, p);
}

bool belul_van(PONT side, PONT pont) {
     if (side.x*pont.x + side.y*pont.y + side.e*pont.e >= 0) return true;
     else return false;
}

void do_the_thing(PONT *poly, int n, PONT *frame, int n2, double angle, double size) {
     int i,j,k;
     double x,y;
     PONT t1[n*2],t2[n*2],side;

     for (i=0;i<n;i++) {
         x = poly[i].x/poly[i].e - centerX;
         y = poly[i].y/poly[i].e - centerY;
         t1[i].x = (int)( ( x*cos(angle)-y*sin(angle) )*size ) + centerX;
         t1[i].y = (int)( ( x*sin(angle)+y*cos(angle) )*size ) + centerY;
         t1[i].e = 1;
     }

     for (j=0;j<2;j++) { // <- minden baj forrása x_x
         k = 0;
         side = metsz_illeszt(frame[j],frame[(j+1)%n2]);
         for (i=0;i<n;i++) {
             if (belul_van(side,t1[i]) && belul_van(side,t1[(i+1)%n])) {
                t2[k] = t1[(i+1)%n]; k++;
             }
             if (belul_van(side,t1[i]) && !belul_van(side,t1[(i+1)%n])) {
                t2[k] = metsz_illeszt( metsz_illeszt(t1[i],t1[(i+1)%n]) , side ); k++;
             }
             if (!belul_van(side,t1[i]) && belul_van(side,t1[(i+1)%n])) {
                t2[k] = metsz_illeszt( metsz_illeszt(t1[i],t1[(i+1)%n]) , side ); k++;
                t2[k] = t1[(i+1)%n]; k++;
             }
         }
         for (i=0;i<k;i++) { t1[i] = t2[i]; printf("t1[%d]: (%d,%d)\n",i,t1[i].x/t1[i].e,t1[i].y/t1[i].e); }
         n = k;
         printf("^ j = %d\n\n",j);
     }

     fill_poly(t1, n);
}


int main( ) {

int gd, gm, page = 0;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

centerX = getmaxx()/2;
centerY = getmaxy()/2;

PONT poly[6];
for (int i=0;i<6;i++) {
    poly[i].x = (int)(centerX+cos(i*PI/3)*150);
    poly[i].y = (int)(centerY+sin(i*PI/3)*150);
    poly[i].e = 1;
}

PONT frame[] = {centerX-100, centerY-100, 1,
                centerX+100, centerY-100, 1,
                centerX+100, centerY+100, 1,
                centerX-100, centerY+100, 1};

double angle = 0, size = 1;

while (!kbhit()) {
    setactivepage(page);
    cleardevice();

    //angle += (PI/1800);
    //size += 0.001;

    setcolor(15);
    draw_poly(frame, 4);

    setcolor(13);
    do_the_thing(poly, 6, frame, 4, angle, size);

    setvisualpage(page);
    page = 1-page;
}

getch();
closegraph( );

return 0;
}
