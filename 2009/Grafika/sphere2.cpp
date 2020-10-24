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
};

typedef struct {
    PONT3D *points;
    int *faces;
    int point_num, face_num, points_per_face;
    double a1, a2, a3;
} OBJINFO;

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

void draw3D(OBJINFO obj, window w, LIGHT feny = default_light) {
    int i, j, oldcolor = getcolor(), r, g, b;
    int* poly = (int*)malloc(sizeof(int)*obj.points_per_face*2);
    double mlen, szog, color;
    PONT2D a;
    PONT3D p1, p2, p3, n, m, centrum(0,0,s);
    PONT3D* p = (PONT3D*)malloc(sizeof(PONT3D)*obj.point_num);

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
                 a = window_to_viewport(p[ obj.faces[i*(obj.points_per_face)+j] ], w);
                 poly[j*2]   = (int)a.x;
                 poly[j*2+1] = (int)a.y;
             }
             color = ( feny.pos.norm() * n + 1 ) / 2;
             if (color < 0) color = 0;
             r = (int)(color*feny.R);   r = r>255 ? 255 : r;
             g = (int)(color*feny.G);   g = g>255 ? 255 : g;
             b = (int)(color*feny.B);   b = b>255 ? 255 : b;
             setrgbpalette(14, r, g, b);
             setcolor( 14 );
             fillpoly(obj.points_per_face, poly);
        }
    }
    free(poly);
    free(p);
    setcolor( oldcolor );
}

PONT3D r_(double phi, double theta, double r) {
    return PONT3D(r*sin(theta)*cos(phi), r*sin(theta)*sin(phi), r*cos(theta));
}

void make_sphere(OBJINFO *obj, double r=1, int q1=16, int q2=32) { // q1 = függõleges lapok száma, q2 = vízszintes lapok száma
    int theta, phi;
    double step1 = PI/q1, step2 = (PI*2)/q2;
    int point_num = (q1+1)*q2, face_num = q1*q2, cnt = 0;
    // tömbfoglalások
    PONT3D *points = (PONT3D*)malloc(sizeof(PONT3D)*point_num);
    int *faces = (int*)malloc(sizeof(int)*face_num*4); // pont tömbbeli indexeket tárol 4-esével
    // test adatainak feltöltése
    obj->points = points;
    obj->faces = faces;
    obj->point_num = point_num;
    obj->face_num = face_num;
    obj->points_per_face = 4;
    obj->a1 = obj->a2 = obj->a3 = 0; // kezdõ szögek
    // tömbfeltöltések
    for (theta = 0; theta <= q1; theta++) { // a theta szög 0-tól PI-ig fut
        for (phi = 0; phi < q2; phi++) { // a phi szög 0-tól 2PI-ig fut
            if (theta < q1) { // az utolsó sorban nem teljesül
                // lapok feltöltése indexekkel, óramutató járásával ellentétes
                faces[cnt*4]   = cnt; // jelenlegi pont
                faces[cnt*4+1] = cnt+q2; // jelenlegi pont alatt egy sorral
                faces[cnt*4+2] = ((cnt+1)%q2==0)?(cnt+1):(cnt+1+q2); // jelenlegi pont alatt egy sorral + jobbra eggyel
                faces[cnt*4+3] = ((cnt+1)%q2==0)?(cnt+1-q2):(cnt+1); // jelenlegi pont + jobbra eggyel
            }
            points[cnt] = r_(phi*step2, theta*step1, r); // step1 és step2 azért, hogy a ciklusban ne kelljen lebegõpontos számokkal számolni
            cnt++; // counter (kezdõérték: 0)
        }
    }
}

int main( ) {

int r,g,b,q1,q2;
printf("Forgatas: bal egergomb\nFeny mozgatas: jobb egergomb\n\n");

printf("Feny szinenek beallitasa: (0-255)\n");
printf("Piros: "); scanf("%d",&r);
printf("Zold: "); scanf("%d",&g);
printf("Kek: "); scanf("%d",&b);

printf("\nAlakzat beallitasa:\n");
printf("Fuggoleges lapok szama: "); scanf("%d",&q1);
printf("Vizszintes lapok szama: "); scanf("%d",&q2);

int gd, gm, page = 0;
gd = VGA; gm = VGAMAX;
initgraph( &gd, &gm, "3D" );

window w = { 100, 100, 600, 600 };
LIGHT feny = { PONT3D(0, 0, 2*s), r, g, b };
int mousex, mousey;
char c;

// itt keletkezik a göNb
OBJINFO test;
make_sphere(&test, 2, q1, q2);

while ( !kbhit() ) {
    setactivepage(page);
    cleardevice();

    if (balgomb) {
        test.a2 -= (egerx-mousex)*PI/180;
        test.a1 += (egery-mousey)*PI/180;
        mousex = egerx;
        mousey = egery;
    }
    if (jobbgomb) {
        feny.pos.x += (egerx-mousex);
        feny.pos.y += (egery-mousey);
        mousex = egerx;
        mousey = egery;
    }
    if (!balgomb && !jobbgomb) {
        mousex = egerx;
        mousey = egery;
    }

    draw3D(test, w, feny);

    //setcolor(15);
    draw_window(w);

    setvisualpage(page);
    page = 1-page;

}
getch();

closegraph( );
return 0;
}
