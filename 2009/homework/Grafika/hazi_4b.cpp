# include "graphics.h"
# include <stdlib.h>

typedef struct pont
{
    int x, y, e;
} PONT;

int aktiv_pont(PONT *pontok, int n, int r) {
    for (int i=0; i<n; i++) if ( (pontok[i].x-egerx)*(pontok[i].x-egerx) + (pontok[i].y-egery)*(pontok[i].y-egery) < r*r ) return i;
    return -1;
}

PONT metsz_illeszt(PONT p1, PONT p2) {
     PONT retval;
     retval.x =   p1.y * p2.e - p1.e * p2.y;
     retval.y = - p1.x * p2.e + p1.e * p2.x;
     retval.e =   p1.x * p2.y - p1.y * p2.x;     
     return retval;
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

PONT pontok[4] = {50,50,1, 200,200,1, 50,200,1, 200,50,1}, metszes;
int metszes_x, metszes_y;

while (!kbhit()) {
    setactivepage(page);
    cleardevice();
    
    if (!balgomb) ap = aktiv_pont(pontok,4,10);
    if (ap >= 0 && balgomb) {
        pontok[ap].x = egerx;
        pontok[ap].y = egery;
    }

    line(pontok[0].x, pontok[0].y, pontok[1].x, pontok[1].y);
    line(pontok[2].x, pontok[2].y, pontok[3].x, pontok[3].y);
    
    metszes = metsz_illeszt( metsz_illeszt( pontok[0] , pontok[1] ) , metsz_illeszt( pontok[2] , pontok[3] ) );
    
    if (metszes.e != 0) {
        metszes_x = metszes.x / metszes.e;
        metszes_y = metszes.y / metszes.e;
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
