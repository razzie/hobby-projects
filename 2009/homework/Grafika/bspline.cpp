# include "graphics.h"
# include <stdlib.h>
# include <math.h>
# include <stdio.h>

typedef struct pont
{
    double x, y;
} PONT;

int aktiv_pont(PONT *pontok, int n, int r) {
    for (int i=0; i<n; i++) if ( (pontok[i].x-egerx)*(pontok[i].x-egerx) + (pontok[i].y-egery)*(pontok[i].y-egery) < r*r ) return i;
    return -1;
}

PONT Q(PONT *p, double t) {
     PONT r;
     r.x = (p[0].x*pow(1-t,3) + p[1].x*(3*pow(t,3)-6*pow(t,2)+4) + p[2].x*(-3*pow(t,3)+3*pow(t,2)+3*t+1) + p[3].x*pow(t,3))/6;
     r.y = (p[0].y*pow(1-t,3) + p[1].y*(3*pow(t,3)-6*pow(t,2)+4) + p[2].y*(-3*pow(t,3)+3*pow(t,2)+3*t+1) + p[3].y*pow(t,3))/6;
     return r;
}

void Bspline(PONT *p) {
     double x, y, t = 0;
     PONT pont = Q(p,t);
     moveto(pont.x,pont.y);
     while (t <= 1) {
           t += 0.01;
           pont = Q(p,t);
           lineto(pont.x,pont.y);
     }
}

int main( ) {

int gd, gm, page = 0;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

PONT p[20], pont;
int ap, i , katt, p_size = 0;
double t;

while (!kbhit()) {
    setactivepage(page);
    cleardevice();

    if (!balgomb) ap = aktiv_pont(p,p_size,10);
    if (ap >= 0 && balgomb) {
        p[ap].x = egerx;
        p[ap].y = egery;
    }

    if (jobbgomb) katt = 1; else katt = (katt==1?2:0);
    if (katt == 2 && p_size < 20) {
             p[p_size].x = egerx;
             p[p_size].y = egery;
             p_size++;
    }

    setcolor(15);
    for (i=0;i<p_size;i++) circle(p[i].x,p[i].y,5);

    for (i=0;i<p_size-1;i++) line(p[i].x,p[i].y, p[i+1].x,p[i+1].y);
    if (p_size >= 4) {
               for (i=0;i<=p_size-4;i++) {
                   setcolor(i%14+1);
                   Bspline(&p[i]);
               }
    }

    setvisualpage(page);
    page = 1-page;
}

getch();
closegraph( );

return 0;
}
