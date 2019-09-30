# include "graphics.h"
# include <stdlib.h>
# include <stdio.h>

int maxX, maxY, maxMax;

int Random( int n ){             /* 0 es n-1 kozti veletlenszamot ad vissza */     
      return rand( ) % n; 
} 

int Round( double n ) {          /* n egeszre kerekitett erteket adja vissza */
     return (int)( n + 0.5 );
}

int max(int a, int b) { // maximum függvény
    if (a>b)
        return a;
    else
        return b;
}

void buildMaxs( ) {      
     maxX = getmaxx( );  
     maxY = getmaxy( );
     maxMax = max(maxX, maxY); // legnagyobb szélesség vagy magasság (kör és ellipszis sugarához)
}

void Part1() {

     while( !kbhit() ) {
            delay(100);
            putpixel(Random(maxX), Random(maxY), 1 + Random(15));
     }
     getch();
     cleardevice();
}

void Part2() {

     while( !kbhit() ) {
            delay(100);
            setcolor(1 + Random(15));
            moveto(Random(maxX), Random(maxY));
            lineto(Random(maxX), Random(maxY));
     }
     getch();
     cleardevice();
}

void Part3() {

     int x, y, r;

     while( !kbhit() ) {
            delay(100);
            setcolor(1 + Random(15));
            {
             x = Random(maxX);
             y = Random(maxY);
             r = Random(maxMax);
            }
            {
             if (x+r > maxX) r=maxX-x;
             if (x-r < 0) r=x;
             if (y+r > maxY) r=maxY-y;
             if (y-r < 0) r=y;
            }
            circle(x, y, r);
     }
     getch();
     cleardevice();
}

void Part4() {

     int x, y, r1, r2;

     while( !kbhit() ) {
            delay(100);
            setcolor(1 + Random(15));
            {
             x = Random(maxX);
             y = Random(maxY);
             r1 = Random(maxMax);
             r2 = Random(maxMax);
            }
            {
             if (x+r1 > maxX) r1=maxX-x;
             if (x-r1 < 0) r1=x;
             if (y+r2 > maxY) r2=maxY-y;
             if (y-r2 < 0) r2=y;
            }
            ellipse(x, y, 0, 0, r1, r2);
     }
     getch();
     cleardevice();
}

void Part5() {

     while( !kbhit() ) {
            delay(100);
            setcolor(1 + Random(15));
            rectangle(Random(maxX), Random(maxY), Random(maxX), Random(maxY));
     }
     getch();
     cleardevice();
}

void Part6() {

     moveto(Random(maxX), Random(maxY));
     while( !kbhit() ) {
            delay(100);
            setcolor(1 + Random(15));
            lineto(Random(maxX), Random(maxY));
     }
     getch();
     cleardevice();
}

int main( ) {

    int gd, gm;
    gd = VGA; gm = VGAMED;
    initgraph( &gd, &gm, "" );

    buildMaxs( );

    Part1();
    Part2();
    Part3();
    Part4();
    Part5();
    Part6();

    closegraph( );
    return EXIT_SUCCESS;
}
