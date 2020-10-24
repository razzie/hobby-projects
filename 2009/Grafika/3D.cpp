# include "graphics.h"
# include <stdlib.h>
# include <math.h>

#define PI 3.141592653589793238462643

int s = -4;
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

        PONT3D &operator+= (PONT3D p) {
            this->x += p.x;
            this->y += p.y;
            this->z += p.z;
            return *this;
        }

        PONT3D operator- (PONT3D p) {
            PONT3D r;
            r.x = this->x - p.x;
            r.y = this->y - p.y;
            r.z = this->z - p.z;
            return r;
        }

        PONT3D &operator-= (PONT3D p) {
            this->x -= p.x;
            this->y -= p.y;
            this->z -= p.z;
            return *this;
        }

        double operator* (PONT3D p) {
            return this->x * p.x + this->y * p.y + this->z * p.z;
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
};

typedef struct {
    PONT3D *points;
    int *faces;
    int point_num, face_num, points_per_face;
    double a1, a2, a3;
} OBJINFO;

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

PONT2D window_to_viewport(PONT3D pt, window w, bool no_depth = false) {
    if (no_depth)
        return window_to_viewport(pt.x, pt.y, w);
    else
        return window_to_viewport(pt.x * s/(s-pt.z), pt.y * s/(s-pt.z), w);
}

void draw_window(window w) {
    line(w.Umin, w.Vmin, w.Umax, w.Vmin);
    line(w.Umin, w.Vmin, w.Umin, w.Vmax);
    line(w.Umax, w.Vmin, w.Umax, w.Vmax);
    line(w.Umin, w.Vmax, w.Umax, w.Vmax);
}

void draw3D(OBJINFO obj, window w, PONT3D feny = PONT3D(0,0,2*s), bool no_depth = false) {
    int i, j, poly[obj.points_per_face*2], color;
    double mlen, szog;
    PONT2D a;
    PONT3D p1, p2, p3, n, m, p[obj.point_num], centrum(0,0,s);

    for (i=0;i<obj.point_num;i++) p[i] = obj.points[i].rot( obj.a1, obj.a2, obj.a3 );

    for (i=0;i<obj.face_num;i++) {
        p1 = p[obj.faces[i*(obj.points_per_face)]];
        p2 = p[obj.faces[i*(obj.points_per_face)+1]];
        p3 = p[obj.faces[i*(obj.points_per_face)+2]];
        n = normal( p1 , p2 , p3 );
        m = centrum - p1;
        szog = n * m.norm();
        if (szog > 0 && szog < 1) {
             for (j=0;j<obj.points_per_face;j++) {
                 a = window_to_viewport(p[ obj.faces[i*(obj.points_per_face)+j] ], w, no_depth);
                 poly[j*2]   = (int)a.x;
                 poly[j*2+1] = (int)a.y;
             }
             color = ( ( feny.norm() * n + 1) / 2) * 255;
             setrgbpalette(14, color,color,color);
             setcolor( 14 );
             fillpoly(obj.points_per_face, poly);
        }
    }
}


int main( ) {

int gd, gm, page = 0;
gd = VGA; gm = VGAMAX;
initgraph( &gd, &gm, "3D" );

window w1 = { 100, 100, 600, 600 };
window w2 = { 700, 100, 1200, 600 };
PONT3D feny(0, 0, 2*s);
int mousex, mousey;
char c;

PONT3D kocka_pontok[] = {   PONT3D( 1, 1, 1 ),
                            PONT3D(-1, 1, 1 ),
                            PONT3D(-1,-1, 1 ),
                            PONT3D( 1,-1, 1 ),
                            PONT3D( 1, 1,-1 ),
                            PONT3D(-1, 1,-1 ),
                            PONT3D(-1,-1,-1 ),
                            PONT3D( 1,-1,-1 ) };

int kocka_lapok[] = { 5, 1, 0,
                      5, 0, 4,
                      4, 0, 3,
                      4, 3, 7,
                      6, 2, 1,
                      6, 1, 5,
                      7, 3, 2,
                      7, 2, 6,
                      6, 5, 4,
                      6, 4, 7,
                      1, 2, 3,
                      1, 3, 0};

OBJINFO kocka = { kocka_pontok, kocka_lapok, 8, 12, 3, 0, 0, 0 };

do {
    setactivepage(page);
    cleardevice();

    if ( kbhit() ) c = getch();
    else {
        c = 0;
        mousex = egerx;
        mousey = egery;
    }

    switch ( c ) {
        case ' ':
            kocka.a1 += (egerx-mousex)*PI/180;
            kocka.a2 += (egery-mousey)*PI/180;
            mousex = egerx;
            mousey = egery;
            break;
        case 'f':
            feny.x += (egerx-mousex);
            feny.y += (egery-mousey);
            mousex = egerx;
            mousey = egery;
            break;
    }

    draw3D(kocka, w1, feny, true);
    draw3D(kocka, w2, feny);

    setcolor(15);
    draw_window(w1);
    draw_window(w2);

    setvisualpage(page);
    page = 1-page;

} while ( c != 27 );

closegraph( );
return 0;
}
