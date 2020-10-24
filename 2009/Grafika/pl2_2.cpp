#include "graphics.h"
#include <conio.h>

void paint( ) {
     while( !kbhit( ) ) {
         if ( balgomb )
             fillellipse( egerx, egery, 3, 3 );
     }
} 

int main( ) {
    int gd,gm;

    gd=VGA;gm=VGAMAX;

    initgraph(&gd,&gm,"");

    paint( );

    getch();

    closegraph();

    return(0);
}     
