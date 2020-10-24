# include "graphics.h"
# include <stdlib.h>
# include <stdio.h>
# include <math.h>

#define PI 3.141592653589793238462643

int maxX, maxY;

typedef struct {
    double x, y;
} PONT2D;

class PONT3D {
    public:
        double x, y, z;

        PONT3D () {
            this->x = 0;
            this->y = 0;
            this->z = 0;
        }

        PONT3D (const PONT3D &p) {
            this->x = p.x;
            this->y = p.y;
            this->z = p.z;
        }

        PONT3D (double x, double y, double z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        PONT3D operator+ (PONT3D p) {
            PONT3D r;
            r.x = this->x + p.x;
            r.y = this->y + p.y;
            r.z = this->z + p.z;
            return r;
        }

        PONT3D operator+ (double n) {
            PONT3D r;
            r.x = this->x + n;
            r.y = this->y + n;
            r.z = this->z + n;
            return r;
        }

        PONT3D &operator+= (PONT3D p) {
            this->x += p.x;
            this->y += p.y;
            this->z += p.z;
            return *this;
        }

        PONT3D &operator+= (double n) {
            this->x += n;
            this->y += n;
            this->z += n;
            return *this;
        }

        PONT3D operator- (PONT3D p) {
            PONT3D r;
            r.x = this->x - p.x;
            r.y = this->y - p.y;
            r.z = this->z - p.z;
            return r;
        }

        PONT3D operator- (double n) {
            PONT3D r;
            r.x = this->x - n;
            r.y = this->y - n;
            r.z = this->z - n;
            return r;
        }

        PONT3D &operator-= (PONT3D p) {
            this->x -= p.x;
            this->y -= p.y;
            this->z -= p.z;
            return *this;
        }

        PONT3D &operator-= (double n) {
            this->x -= n;
            this->y -= n;
            this->z -= n;
            return *this;
        }

        double operator* (PONT3D p) {
            return this->x * p.x + this->y * p.y + this->z * p.z;
        }

        PONT3D operator* (double n) {
            return PONT3D( x*n , y*n , z*n );
        }

        PONT3D &operator*= (double n) {
            this->x *= n;
            this->y *= n;
            this->z *= n;
            return *this;
        }

        PONT3D operator% (PONT3D p) {
            PONT3D r( this->y*p.z - this->z*p.y, this->z*p.x - this->x*p.z, this->x*p.y - this->y*p.x );
            return r;
        }

        double len() {
            return sqrt(x*x + y*y + z*z);
        }

        PONT3D &norm() {
            double len = this->len();
            this->x /= len;
            this->y /= len;
            this->z /= len;
            return *this;
        }

        PONT3D rot(double a1, double a2, double a3) {
            PONT3D p = *this;
            p = PONT3D( p.x , p.y*cos(a1)-p.z*sin(a1) , p.y*sin(a1)+p.z*cos(a1) );
            p = PONT3D( p.x*cos(a2)+p.z*sin(a2) , p.y , -p.x*sin(a2)+p.z*cos(a2) );
            p = PONT3D( p.x*cos(a3)-p.y*sin(a3) , p.x*sin(a3)+p.y*cos(a3) , p.z );
            return p;
        }

        PONT3D rot(PONT3D angles) {
            return rot(angles.x, angles.y, angles.z);
        }
};

PONT2D axonometry(PONT3D P) {
    PONT2D r;
    r.x = -100 * P.x + 150 * P.y + 0 * P.z + maxX / 2;
    r.y = 100 * P.x + 120 * P.y + -140 * P.z + maxY / 2;
    return r;
}

PONT3D r_(double phi, double theta, double r) {
    return PONT3D(r*sin(theta)*cos(phi), r*sin(theta)*sin(phi), r*cos(theta));
}

void ax_line(PONT3D p1, PONT3D(p2)) {
    PONT2D a = axonometry(p1), b = axonometry(p2);
    line((int)a.x, (int)a.y, (int)b.x, (int)b.y);
}

void sphere(double r = 1, int q1 = 16, int q2 = 16, double a1 = 0, double a2 = 0, double a3 = 0) {
    int theta, phi;
    double step1 = PI/q1 - 0.00001, step2 = (PI)/q2 - 0.00001;
    PONT2D p;

    setcolor ( 15 );
    ax_line( PONT3D(0,0,0) , PONT3D(0,0,2).rot(a1,a2,a3) );
    ax_line( PONT3D(0,0,0) , PONT3D(0,2,0).rot(a1,a2,a3) );
    ax_line( PONT3D(0,0,0) , PONT3D(2,0,0).rot(a1,a2,a3) );

    setcolor ( 2 );
    for (theta = 0; theta <= q1; theta++) { // a theta szög 0-tól PI-ig fut
        p = axonometry( r_( 0, theta*step1, r ).rot(a1,a2,a3) );
        moveto((int)p.x, (int)p.y);
        for (phi = 0; phi <= q2; phi++) { // a phi szög 0-tól 2PI-ig fut
            p = axonometry( r_( phi*step2, theta*step1, r ).rot(a1,a2,a3) );
            lineto((int)p.x, (int)p.y);
        }
    }
    for (phi = 0; phi <= q2; phi++) {
        p = axonometry( r_( phi*step2, 0, r ).rot(a1,a2,a3) );
        moveto((int)p.x, (int)p.y);
        for (theta = 0; theta <= q1; theta++) {
            p = axonometry( r_( phi*step2, theta*step1, r ).rot(a1,a2,a3) );
            lineto((int)p.x, (int)p.y);
        }
    }
}

int main( ) {

int gd, gm, page = 0;
gd = VGA; gm = VGAMAX;
initgraph( &gd, &gm, "3D" );

int mousex, mousey, q1 = 16, q2 = 32;
double a1 = 0, a2 = 0;
char c;

maxX = getmaxx();
maxY = getmaxy();

do {
    setactivepage(page);
    cleardevice();

    if ( kbhit() ) c = getch();
    else c = 0;

    switch ( c ) {
        case 'q':
                if ( q1 > 2 ) q1--;
                break;
        case 'w':
                q1++;
                break;
        case 'a':
                if ( q2 > 2 ) q2--;
                break;
        case 's':
                q2++;
                break;
    }

    if (balgomb) {
        a2 -= (egerx-mousex)*PI/180;
        a1 -= (egery-mousey)*PI/180;
        mousex = egerx;
        mousey = egery;
    } else {
        mousex = egerx;
        mousey = egery;
    }

    sphere(1, q1, q2, a1, a2);

    setvisualpage(page);
    page = 1-page;

} while ( c != 27 );

closegraph( );
return 0;
}
