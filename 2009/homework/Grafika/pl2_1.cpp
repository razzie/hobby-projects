# include "graphics.h"
# include <conio.h>
# include <math.h>

#define PI 3.141592653589793238462643

typedef struct point { double x; double y; } POINT; 

int Round( double n ) {         
     return (int)( n + 0.5 );
}

POINT makepoint( double x, double y ) {
      POINT temp;
      temp.x = x;
      temp.y = y;
      return temp;
}

POINT eltol1( POINT p, POINT v ) {
      return makepoint( p.x + v.x, p.y + v.y );
}

void eltol2( POINT *p, POINT v ) {     /* ,,helyben'' */
        (*p).x += v.x;
        (*p).y += v.y;
}

void pelda( ) {
     int r = 100;
     POINT o = makepoint( 200, 200 );
     double rad = PI / 3;
     POINT p = makepoint( r * cos( rad ), r * sin( rad ) );
     
     p = eltol1( p, o );
     /* eltol2( &p, o ); */
     
     circle( Round( o.x ), Round( o.y ), 100 );
     fillellipse( Round( p.x ), Round( p.y ), 5, 5 );
}
     
   
int main() {

int gd, gm;

gd = VGA; gm = VGAMAX;

initgraph( &gd, &gm, "" );

pelda( );

getch( );

closegraph();

return 0;

}
