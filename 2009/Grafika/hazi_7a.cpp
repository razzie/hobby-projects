# include "graphics.h"
# include <stdlib.h>

typedef struct pont
{
    int x, y;
} PONT;

int aktiv_pont(PONT *pontok, int n, int r) {
    for (int i=0; i<n; i++) if ( (pontok[i].x-egerx)*(pontok[i].x-egerx) + (pontok[i].y-egery)*(pontok[i].y-egery) < r*r ) return i;
    return -1;
}

void Hermit(PONT *p) {
     double t = -1;
     int x,y;
     
     setcolor(4);
     moveto(p[0].x, p[0].y);
     while (t <= 2.1) {
           x = (int)(p[0].x*(-1.0/6*t*t*t + 0.5*t*t - 1.0/3*t) + p[1].x*(0.5*t*t*t - t*t - 0.5*t + 1) + p[2].x*(-0.5*t*t*t + 0.5*t*t + t) + p[3].x*(1.0/6*t*t*t - 1.0/6*t));
           y = (int)(p[0].y*(-1.0/6*t*t*t + 0.5*t*t - 1.0/3*t) + p[1].y*(0.5*t*t*t - t*t - 0.5*t + 1) + p[2].y*(-0.5*t*t*t + 0.5*t*t + t) + p[3].y*(1.0/6*t*t*t - 1.0/6*t));
           t+=0.1;
           lineto(x, y);
     }
}


int main( ) {

int gd, gm, page = 0, ap, i;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

PONT p[4] = {50,50, 50,200, 200,200, 200,50};


while (!kbhit()) {
    setactivepage(page);
    cleardevice();
    
    if (!balgomb) ap = aktiv_pont(p,4,10);
    if (ap >= 0 && balgomb) {
        p[ap].x = egerx;
        p[ap].y = egery;
    }
    
    setcolor(15);
    for (i=0;i<3;i++) line(p[i].x,p[i].y, p[i+1].x,p[i+1].y);
    for (i=0;i<4;i++) circle(p[i].x,p[i].y, 5);
    
    Hermit(p);
    
    setvisualpage(page);
    page = 1-page;
}

getch();
closegraph( );

return 0;

}
