#include "graphics.h"
#include <stdlib.h>

int maxX, maxY;

void build_maxs( ) {
    maxX = getmaxx( );
    maxY = getmaxy( );
}

void gomboc( ) {
     build_maxs();
     
     int n = 0;
     int color = 15; // v�ltoz�k be�ll�t�sa
     int page = 0;
     int r = 50;
     int x = 300, food_x = rand()%(maxX-r);
     int y = 200, food_y = rand()%(maxY-r);
     int novX = 1;
     int novY = 1;
     int bal_click = 0, jobb_click = 0;

     while( !kbhit( ) ) {
         setactivepage( page );
         cleardevice( );
         
         n++; // ellen�rz�sek
         if (n > 1000) {
               n = 0;
               build_maxs();
               if (food_x > maxX-r) food_x = maxX-r;
               if (food_y > maxY-r) food_y = maxY-r;
         }
         
         if ( balgomb ) bal_click = 1; // bal eg�rgomb lenyom�s�nak �s elenged�s�nek vizsg�lata
            else if ( bal_click == 1 ) bal_click = 2;
         if ( jobbgomb ) jobb_click = 1; // jobb eg�rgomb lenyom�s�nak �s elenged�s�nek vizsg�lata
            else if ( jobb_click == 1 ) jobb_click = 2;
         
         setcolor( color ); // g�mb�c rajzol�sa
         fillellipse( x, y, r, r );
         setcolor( 0 );
         fillellipse( x-r/2, y-20, 10, 10 );
         fillellipse( x+r/2, y-20, 10, 10 );
         setlinestyle( 0, 0, 3 );
         arc( x, y, 180, 0, r-10 );
         setcolor( 15 ); // �tel rajzol�sa
         fillellipse( food_x, food_y, 5, 5 );
         
         if ( (x-food_x)*(x-food_x) + (y-food_y)*(y-food_y) < r*r && r*2 < maxX-5 && r*2 < maxY-5 ) { // �tellel val� �tk�z�s vizsg�lata (Pythagoras t�tel)
              r += 5;
              food_x = rand()%(maxX-r*2)+r;
              food_y = rand()%(maxY-r*2)+r;
              color = 1 + rand()%15;
         }
         
         if ( x + r >= maxX || x - r <= 0 || jobb_click == 2 ) { // f�gg�leges �tk�z�s
            if ( x + r > maxX ) x = maxX - r;
            if ( x - r < 0 ) x = r;
            novX *= -1;
            color = 1 + rand()%15;
            jobb_click = 0;
         }
         if ( y + r >= maxY || y - r <= 0 || bal_click == 2 ) { // v�zszintes �tk�z�s
            if ( y + r > maxY ) y = maxY - r;
            if ( y - r < 0 ) y = r;
            novY *= -1;
            color = 1 + rand()%15;
            bal_click = 0;
         }
         x += novX;
         y += novY;
         
         setvisualpage( page );
         page = 1 - page;
     }
} 

int main( ) {
    int gd,gm;
    gd=VGA;gm=VGAMAX;
    initgraph(&gd,&gm,"");
    
    gomboc( );
    
    getch();
    closegraph();
    
    return(0);
}     
            
