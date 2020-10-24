# include "graphics.h"
# include <stdlib.h>

void WritePixel(int x, int y, int szin) {
    setcolor(szin);
    floodfill(x*20+10,y*20+10,15);
}

void Vonal(int x1, int y1, int x2, int y2, int szin) {
    int a,b,x,y,d,deltaE,deltaNE;
    a=y1-y2;
    b=x2-x1;
    d=2*a+b;
    deltaE=2*a;
    deltaNE=2*(a+b);
    x=x1;
    y=y1;
    WritePixel(x,y,szin);
    while(x<x2) {
        if (d>=0) {
                d+=deltaE;
                x++;
        } else {
                d+=deltaNE;
                x++;
                y++;
        }
        WritePixel(x,y,szin);
    }
}

void CirclePoints(int u, int v, int xp, int yp, int szin) {
     WritePixel(u+xp,v+yp,szin);
     WritePixel(u+yp,v+xp,szin);
     WritePixel(u+yp,v-xp,szin);
     WritePixel(u+xp,v-yp,szin);
     WritePixel(u-xp,v-yp,szin);
     WritePixel(u-yp,v-xp,szin);
     WritePixel(u-yp,v+xp,szin);
     WritePixel(u-xp,v+yp,szin);
}

void CircleFirst(int u, int v, int r, int szin) {
    int xp,yp,d;
    xp=0;
    yp=r;
    d=1-r;
    CirclePoints(u,v,xp,yp,szin);
    while (yp>xp) {
        if (d<0) {
                d+=xp*2+3;
                xp++;
        } else {
                d+=(xp-yp)*2+5;
                xp++;
                yp--;
        }
        CirclePoints(u,v,xp,yp,szin);
    }
}

int main( ) {

int gd, gm, page = 0;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

int maxX = getmaxx(), maxY = getmaxy();
int i, j;

for (i=20;i<maxX;i+=20) {
    line(i,0,i,maxY);
}
for (j=20;j<maxY;j+=20) {
    line(0,j,maxX,j);
}

CircleFirst(10,10, 5, 2);
Vonal(1,1, 10,10, 4);


getch();

closegraph( );
return 0;
}

