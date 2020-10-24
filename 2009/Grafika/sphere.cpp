# include "graphics.h"
# include <stdlib.h>
# include <stdio.h>
# include <math.h>

#define PI 3.141592653589793238462643

int s = -20;
int Xmin = -2, Ymin = -2, Xmax = 2, Ymax = 2;

typedef struct {
    int Umin, Vmin, Umax, Vmax;
} window;

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

typedef struct {
    PONT3D pos;
    unsigned char R, G, B;
} LIGHT;
LIGHT default_light = { PONT3D(0,0,2*s), 255, 255, 255 };

PONT3D normal(PONT3D a, PONT3D b, PONT3D c) {
    PONT3D r = (b-a)%(c-a);
    return r.norm();
}

PONT2D window_to_viewport(double x, double y, window w) {
    PONT2D p;
    p.x = (x-Xmin)*(w.Umax-w.Umin)/(Xmax-Xmin)+w.Umin;
    p.y = (y-Ymin)*(w.Vmax-w.Vmin)/(Ymax-Ymin)+w.Vmin;
    return p;
}

PONT2D window_to_viewport(PONT3D pt, window w) {
    return window_to_viewport(pt.x * s/(s-pt.z), pt.y * s/(s-pt.z), w);
}

void draw_window(window w) {
    line(w.Umin, w.Vmin, w.Umax, w.Vmin);
    line(w.Umin, w.Vmin, w.Umin, w.Vmax);
    line(w.Umax, w.Vmin, w.Umax, w.Vmax);
    line(w.Umin, w.Vmax, w.Umax, w.Vmax);
}

PONT3D r_(double phi, double theta, double r) {
    return PONT3D(r*sin(theta)*cos(phi), r*sin(theta)*sin(phi), r*cos(theta));
}

void sphere(window w, double r = 1, PONT3D rotation = PONT3D(0,0,0), LIGHT feny = default_light, int q1=16, int q2=32) {
    int i, theta, phi, poly[8], oldcolor = getcolor();
    double step1 = PI/q1 - 0.00001, step2 = (PI*2)/q2 - 0.00001, szog, color;
    PONT2D vp;
    PONT3D p[4], n;

    setcolor( 14 );

    for (theta = 0; theta < q1; theta++) { // a theta szög 0-tól PI-ig fut
        for (phi = 0; phi < q2; phi++) { // a phi szög 0-tól 2PI-ig fut
            p[0] = ( r_(  phi*step2,     theta*step1,    r) ).rot(rotation.x,rotation.y,rotation.z);
            p[1] = ( r_(  phi*step2,    (theta+1)*step1, r) ).rot(rotation.x,rotation.y,rotation.z);
            p[2] = ( r_( (phi+1)*step2, (theta+1)*step1, r) ).rot(rotation.x,rotation.y,rotation.z);
            p[3] = ( r_( (phi+1)*step2,  theta*step1,    r) ).rot(rotation.x,rotation.y,rotation.z);
            n = normal( p[0] , p[1] , p[2] );
            szog = n * ( PONT3D(0,0,s) - p[0] ).norm();
            if (szog > 0 && szog < 1) {
                for (i=0;i<4;i++) {
                    vp = window_to_viewport(p[i], w);
                    poly[i*2]   = (int)vp.x;
                    poly[i*2+1] = (int)vp.y;
                }
                color = ( feny.pos.norm() * n + 1 ) / 2;
                setrgbpalette( 14, (int)(color*feny.R), (int)(color*feny.G), (int)(color*feny.B) );
                fillpoly( 4, poly );
            }
        }
    }

    setcolor( oldcolor );
}


int main( ) {

int gd, gm, page = 0;
gd = VGA; gm = VGAMAX;
initgraph( &gd, &gm, "3D" );

window w = { 100, 100, 600, 600 };
PONT3D rotation(0,0,0);
int mousex, mousey, q1 = 16, q2 = 32;
char c;

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
        rotation.y -= (egerx-mousex)*PI/180;
        rotation.x += (egery-mousey)*PI/180;
        mousex = egerx;
        mousey = egery;
    }
    if (jobbgomb) {
        default_light.pos.x += (egerx-mousex);
        default_light.pos.y += (egery-mousey);
        mousex = egerx;
        mousey = egery;
    }
    if (!balgomb && !jobbgomb) {
        mousex = egerx;
        mousey = egery;
    }

    sphere(w, 2, rotation, default_light, q1, q2);
    draw_window(w);

    setvisualpage(page);
    page = 1-page;

} while ( c != 27 );

closegraph( );
return 0;
}
