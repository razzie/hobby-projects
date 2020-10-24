# include "graphics.h"
# include <stdlib.h>

int lnko(int a, int b) {

    int m, temp;
    if (b > a) {
          temp = b;
          b = a;
          a = temp;
    }

    do {
        m = a%b;
        a = temp = b;
        b = m;
    } while(m != 0);
    
    return temp;
}

void fill_the_rectangle(int width, int height) {

     int maxX = getmaxx(), maxY = getmaxy();
     int posX, posY, x, y, p, r;

     posX = maxX/2 - width/2;
     posY = maxY/2 - height/2;

     p = lnko(width,height);
     
     rectangle(posX, posY, posX+width, posY+height);
     for (x=0;x<width;x+=p) {
         for (y=0;y<height;y+=p) {
             r = (int)((double)(p)/2 + 0.5); // sugár
             circle(posX+x+r, posY+y+r, r);
         }
     }
}

int main( ) {

    int gd, gm;
    gd = VGA; gm = VGAMED;
    initgraph( &gd, &gm, "" );

    fill_the_rectangle(150,275);
    
    getch();
    closegraph();
    
    return EXIT_SUCCESS;
}
