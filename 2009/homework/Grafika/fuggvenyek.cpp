# include "graphics.h"
# include <stdlib.h>
# include <math.h>

#define PI 3.141592653589793238462643

int Xmin = -10, Ymin = -10, Xmax = 10, Ymax = 10;

typedef struct {
    int Umin, Vmin, Umax, Vmax;
} window;

typedef struct {
    double x, y;
} PONT2D;

PONT2D window_to_viewport(double x, double y, window w) {
    PONT2D p;
    p.x = (x-Xmin)*(w.Umax-w.Umin)/(Xmax-Xmin)+w.Umin;
    p.y = (y-Ymin)*(w.Vmax-w.Vmin)/(Ymax-Ymin)+w.Vmin;
    return p;
}

void line_vw(double x1, double y1, double x2, double y2, window w) {
     PONT2D a, b;
     a = window_to_viewport(x1,y1,w);
     b = window_to_viewport(x2,y2,w);
     line((int)a.x,(int)a.y,(int)b.x,(int)b.y);
}

void draw_window(window w) {
    line(w.Umin, w.Vmin, w.Umax, w.Vmin);
    line(w.Umin, w.Vmin, w.Umin, w.Vmax);
    line(w.Umax, w.Vmin, w.Umax, w.Vmax);
    line(w.Umin, w.Vmax, w.Umax, w.Vmax);
}

void fuggveny(window w, double (*fv)(double x), double xs, double ys) {
     PONT2D p;
     double y, t;
     bool starting = true;
     // koordináta tengelyek
     setcolor(2);
     line_vw(Xmin, Ymin+Ymax, Xmax, Ymin+Ymax, w);
     line_vw(Xmin+Xmax, Ymin, Xmin+Xmax, Ymax, w);
     // beosztás : x jobbra-balra , y le-fel
     setcolor(15);
     for (t=Xmin+Xmax;t<=Xmax;t+=xs) line_vw(t, Ymin+Ymax-0.2, t, Ymin+Ymax+0.2, w);
     for (t=Xmin+Xmax;t>=Xmin;t-=xs) line_vw(t, Ymin+Ymax-0.2, t, Ymin+Ymax+0.2, w);
     for (t=Ymin+Ymax;t<=Ymax;t+=ys) line_vw(Xmin+Xmax-0.2, t, Xmin+Xmax+0.2, t, w);
     for (t=Ymin+Ymax;t>=Ymin;t-=ys) line_vw(Xmin+Xmax-0.2, t, Xmin+Xmax+0.2, t, w);
     // függvényrajzolás
     setcolor(4);
     for (t=Xmin;t<=Xmax;t+=0.01) {
         y = (*fv)(t);
         p = window_to_viewport(t, y, w);
         if (y < Ymin || y > Ymax || starting) {
               starting = false;
               moveto((int)p.x, (int)p.y);
         } else {
               lineto((int)p.x, (int)p.y);
         }
     }
}

double negyzet(double x) {
       return x*x;
}

double kob(double x) {
       return x*x*x;
}

int main( ) {

int gd, gm, page = 0, i;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

getch(); //Itt átméretezhetem az ablakot

int maxX = getmaxx(), maxY = getmaxy();

window windows[4] = { { 0, maxY/2, maxX/2, 0 },
                      { maxX/2, maxY/2, maxX, 0 },
                      { 0, maxY, maxX/2, maxY/2 },
                      { maxX/2, maxY, maxX, maxY/2 } }; 

while ( !kbhit() ) {
    setactivepage(page);
    cleardevice();
    
    setcolor(15);
    for(i=0;i<4;i++) draw_window(windows[i]);
    
    fuggveny(windows[0], negyzet, 1, 1);
    fuggveny(windows[1], kob, 1, 1);
    fuggveny(windows[2], sin, PI/2, 1);
    fuggveny(windows[3], tan, PI/2, 1);
    
    setvisualpage(page);
    page = 1-page;
    
}
getch();

closegraph( );
return 0;
}
