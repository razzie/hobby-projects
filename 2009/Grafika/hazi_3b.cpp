# include "graphics.h"
# include <stdlib.h>

#define PI 3.141592653589793238462643

double spin(int r, int step) {
       return (double)step / (double)r + PI / 2;
}

int main( ) {

int gd, gm, page = 0;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

int maxX = getmaxx(), maxY = getmaxy();

int r1 = 10, r2 = 30, r3 = 50;
int x = 0, y = maxY-50-r2, n;

while (!kbhit()) {
    setactivepage(page);
    cleardevice();
    
    if (x < maxX) x++;
    
    setcolor(15);
    moveto(0,maxY-50); lineto(maxX,maxY-50); //vonal
    circle(x, y, r2);
    moveto(x, y); lineto( x+cos(spin(r2,x))*r3 , y+sin(spin(r2,x))*r3 );
    
    for (n=0; n<=x; n++) {
        putpixel( n+cos(spin(r2,n))*r1 , y+sin(spin(r2,n))*r1 , 4 );
        putpixel( n+cos(spin(r2,n))*r2 , y+sin(spin(r2,n))*r2 , 2 );
        putpixel( n+cos(spin(r2,n))*r3 , y+sin(spin(r2,n))*r3 , 1 );
    }
    
    setvisualpage(page);
    page = 1-page;
}

getch();

closegraph( );
return 0;
}
