# include "graphics.h"
# include <stdlib.h>
# include <math.h>

typedef struct pont
{
    double x, y;
} PONT;

int aktiv_pont(PONT *pontok, int n, int r) {
    for (int i=0; i<n; i++) if ( (pontok[i].x-egerx)*(pontok[i].x-egerx) + (pontok[i].y-egery)*(pontok[i].y-egery) < r*r ) return i;
    return -1;
}

int faktor(int n) {
    int i,r=1;
    for (i=2;i<=n;i++) r*=i;
    return r;
}

int C(int n, int k) {
    return faktor(n)/(faktor(k)*faktor(n-k));
}

double B(int n, int i, double t) {
    return C(n,i)*pow(t,i)*pow(1-t,n-i);
}

PONT Q(int n, PONT *p, double t) {
    PONT r;
    int i;
    double x=0,y=0;
    for (i=0;i<=n;i++) {
        x += p[i].x*B(n,i,t);
        y += p[i].y*B(n,i,t);
    }
    r.x = x;
    r.y = y;
    return r;
}

PONT ittjarok(PONT *p, int size, double t) {
    int i, n = size-1;
    PONT p2[n];
    for (i=0;i<n;i++) {
        line(p[i].x,p[i].y, p[i+1].x,p[i+1].y);
        p2[i].x = (1.0-t)*p[i].x + t*p[i+1].x;
        p2[i].y = (1.0-t)*p[i].y + t*p[i+1].y;
    }
    if (n == 1)
        return p2[0];
    else
        return ittjarok(p2,n,t);
}

int main( ) {

int gd, gm, page = 0, ap, i;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

PONT p[] = {50,50, 200,50, 200,200, 50,200}, pont;
int p_size = 4;

double t, t2 = 0, step = 0.005;


while (!kbhit()) {
    setactivepage(page);
    cleardevice();

    if (!balgomb) ap = aktiv_pont(p,p_size,10);
    if (ap >= 0 && balgomb) {
        p[ap].x = egerx;
        p[ap].y = egery;
    }

    t = 0;
    setcolor(2);
    moveto(p[0].x,p[0].y);
    while (t <= 1.01) {
        pont = Q(p_size-1,p,t);
        lineto(pont.x,pont.y);
        t+=0.01;
    }

    setcolor(15);
    for (i=0;i<p_size;i++) circle(p[i].x,p[i].y,5);

    /*t2 += step;
    if (t2 > 1) {
           t2 = 1;
           step *= -1;
    }
    if (t2 < 0) {
           t2 = 0;
           step *= -1;
    }

    setcolor(4);
    pont = ittjarok(p,p_size,t2);
    fillellipse(pont.x,pont.y,4,4);
    */

    setvisualpage(page);
    page = 1-page;
}

getch();
closegraph( );

return 0;
}
