# include "graphics.h"
# include <stdlib.h>

typedef struct pont
{
    int x, y;
} PONT;

typedef struct vektor
{
    int x, y;
} VEKTOR;

int aktiv_pont(PONT *pontok, int n, int r) {
    int x,y;
    for (int i=0; i<n; i++) {
        if (i < n/2) { // i < 5
            x = pontok[i].x;
            y = pontok[i].y;
        } else {
            x = pontok[i].x + pontok[i-5].x;
            y = pontok[i].y + pontok[i-5].y;
        }
        if ( (x-egerx)*(x-egerx) + (y-egery)*(y-egery) < r*r ) return i;
    }
    return -1;
}

void Hermit(PONT *p, PONT *r) {
     double t = 0;
     int x,y;

     setcolor(RED);
     moveto(p[0].x, p[0].y);
     while (t <= 1.0) {
           x = (int)(p[0].x*(2*t*t*t - 3*t*t + 1)+p[1].x*(-2*t*t*t + 3*t*t)+(r[0].x-p[0].x)*(t*t*t - 2*t*t + t)+(r[1].x-p[1].x)*(t*t*t - t*t));
           y = (int)(p[0].y*(2*t*t*t - 3*t*t + 1)+p[1].y*(-2*t*t*t + 3*t*t)+(r[0].y-p[0].y)*(t*t*t - 2*t*t + t)+(r[1].y-p[1].y)*(t*t*t - t*t));

           //x = (int)(p[0].x*(1-t)*(1-t)*(1-t)+p[1].x*3*(1-t)*(1-t)+(r[0].x-p[0].x)*(t*t*t - 2*t*t + t)+(r[1].x-p[1].x)*(t*t*t - t*t));
           //y = (int)(p[0].y*(2*t*t*t - 3*t*t + 1)+p[1].y*(-2*t*t*t + 3*t*t)+(r[0].y-p[0].y)*(t*t*t - 2*t*t + t)+(r[1].y-p[1].y)*(t*t*t - t*t));
           /*x = (int)(p[0].x*pow(1-t,3)+
					p[1].x*3*pow(1-t,2)*t+
					p[2].x*pow(1-t,3)*pow(t,2)+
					p[3].x*pow(t,3));
           y = (int)(p[0].y*pow(1-t,3)+
					p[1].y*3*pow(1-t,2)*t+
					p[2].y*pow(1-t,3)*pow(t,2)+
					p[3].y*pow(t,3));*/
           t+=0.1;
           lineto(x, y);
     }
}

void draw_it(PONT *p) {
    int i;

    setcolor(YELLOW);
    for (i=0;i<5;i++) line(p[i].x,p[i].y, (p[i+5].x+p[i].x),(p[i+5].y+p[i].y));

    for (i=0;i<10;i++){
        if(i<5){
                circle(p[i].x,p[i].y, 5);
               }
        if(i>4){
                circle(p[i-5].x+p[i].x,p[i-5].y+p[i].y, 5);//kört rak a vonal végpontjába
                }
        }

    for (i=0;i<4;i++) Hermit(&p[i],&p[i+5]);

}

int main( ) {

int gd, gm, page = 0, ap;
gd = VGA; gm = VGAMAX;
initgraph( &gd, &gm, "" );

PONT p[10] = {50,500, 100,50, 250,200, 280,200, 100,300,
             50,350, 200,100, 450,250, 200,250, 100,350};

while (!kbhit()) {
    setactivepage(page);
    cleardevice();

    if (!balgomb) ap = aktiv_pont(p,10,10);
    if (ap >= 0 && balgomb) {
        if (ap<5) {
            p[ap].x = egerx;
            p[ap].y = egery;
        } else {
            p[ap].x = egerx - p[ap-5].x;
            p[ap].y = egery - p[ap-5].y;
        }
    }

    draw_it(p);

    setvisualpage(page);
    page = 1-page;
}

getch();
closegraph( );

return 0;

}
