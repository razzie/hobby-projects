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

int det2x2(int x11, int x12, int x21, int x22) {
    return (x11 * x22) - (x12 * x21);
}

int max(int a, int b) {
    return a>b?a:b;
}

int min(int a, int b) {
    return a<b?a:b;
}

int main( ) {

int gd, gm, page = 0, ap;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

PONT pontok[4] = {50,50, 200,200, 50,200, 200,50};

int a1, a2, b1, b2, c1, c2, metszes_x, metszes_y, detA;

while (!kbhit()) {
    setactivepage(page);
    cleardevice();
    
    if (!balgomb) ap = aktiv_pont(pontok,4,10);
    if (ap >= 0 && balgomb) {
        pontok[ap].x = egerx;
        pontok[ap].y = egery;
    }

    a1 = pontok[0].y - pontok[1].y;
    b1 = pontok[1].x - pontok[0].x;
    a2 = pontok[2].y - pontok[3].y;
    b2 = pontok[3].x - pontok[2].x;

    c1 = a1 * pontok[0].x + b1 * pontok[0].y;
    c2 = a2 * pontok[2].x + b2 * pontok[2].y;
    
    detA = det2x2(a1, b1, a2, b2);
    
    moveto(pontok[0].x, pontok[0].y); lineto(pontok[1].x, pontok[1].y);
    moveto(pontok[2].x, pontok[2].y); lineto(pontok[3].x, pontok[3].y);
        
    if (detA) {
        metszes_x = det2x2(c1, b1, c2, b2) / detA;
        metszes_y = det2x2(a1, c1, a2, c2) / detA;
        if (metszes_x >= min(pontok[0].x,pontok[1].x) && metszes_x <= max(pontok[0].x,pontok[1].x) && metszes_y >= min(pontok[0].y,pontok[1].y) && metszes_y <= max(pontok[0].y,pontok[1].y)
         && metszes_x >= min(pontok[2].x,pontok[3].x) && metszes_x <= max(pontok[2].x,pontok[3].x) && metszes_y >= min(pontok[2].y,pontok[3].y) && metszes_y <= max(pontok[2].y,pontok[3].y)) {
           fillellipse(metszes_x, metszes_y, 5, 5);
        }
    }
    
    setvisualpage(page);
    page = 1-page;
}

getch();

closegraph( );

return 0;

}
