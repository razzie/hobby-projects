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

void Hermit(PONT *p, PONT *r) {
     double t = 0;
     int x,y;
     
     setcolor(4);
     moveto(p[0].x, p[0].y);
     while (t <= 1.0) {
           x = (int)(p[0].x*(2*t*t*t - 3*t*t + 1)+p[1].x*(-2*t*t*t + 3*t*t)+(r[0].x-p[0].x)*(t*t*t - 2*t*t + t)+(r[1].x-p[1].x)*(t*t*t - t*t));
           y = (int)(p[0].y*(2*t*t*t - 3*t*t + 1)+p[1].y*(-2*t*t*t + 3*t*t)+(r[0].y-p[0].y)*(t*t*t - 2*t*t + t)+(r[1].y-p[1].y)*(t*t*t - t*t));
           t+=0.1;
           lineto(x, y);
     }
}

void draw_it(PONT *p) {
    int i;
    
    setcolor(15);
    for (i=0;i<4;i++) line(p[i].x,p[i].y, p[i+4].x,p[i+4].y);
    
    for (i=0;i<8;i++) circle(p[i].x,p[i].y, 5);
    
    for (i=0;i<3;i++) Hermit(&p[i],&p[i+4]);

}

int main( ) {

int gd, gm, page = 0, ap;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

PONT p[8] = {50,50, 200,50, 50,200, 200,200,
             50,100, 200,100, 50,250, 200,250};


while (!kbhit()) {
    setactivepage(page);
    cleardevice();
    
    if (!balgomb) ap = aktiv_pont(p,8,10);
    if (ap >= 0 && balgomb) {
        p[ap].x = egerx;
        p[ap].y = egery;
    }
    
    draw_it(p);
    
    setvisualpage(page);
    page = 1-page;
}

getch();
closegraph( );

return 0;

}
