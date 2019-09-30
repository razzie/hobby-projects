# include "graphics.h"
# include <stdlib.h>
//# include <conio.h> 
   

int maxX, maxY;

int Random( int n ){             /* 0 es n-1 kozti veletlenszamot ad vissza */     
      return rand( ) % n; 
} 

int Round( double n ) {          /* n egeszre kerekitett erteket adja vissza */
     return (int)( n + 0.5 );
}

void buildMaxs( ) {      
     maxX = getmaxx( );  
     maxY = getmaxy( );
}

void randomPixels( ) {

    while( !kbhit( ) ) {         /* amig nem tortenik billentyuleutes... */

        putpixel( Random( maxX ), Random( maxY ), Random(15) );

        delay( 100 );           /* egy masodpercnyi kesleltetes */
    }
    getch( );

    cleardevice( );    
}
   
int main( ) {

int gd, gm;

gd = VGA; gm = VGAMED;

initgraph( &gd, &gm, "" );      /* csak ezutan tortenhet BGI hivatkozas!!!!        */

buildMaxs( );                   /* pl. ez az eljaras hasznalja a getmaxx() fgv.-t, */ 

                                /* ezert az initgraph kiadasa elott nem hivhatjuk meg */
//randomPixels( );
rectangle(maxX/2-50,maxY/2-50,maxX/2+50,maxY/2+50);
circle(maxX/2,maxY/2,50);
getch();

closegraph( );

return 0;

}
