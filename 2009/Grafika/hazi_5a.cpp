# include "graphics.h"
# include <stdlib.h>
#include <stdio.h>

typedef struct pont
{
    int x, y, e;
} PONT;

PONT metsz_illeszt(PONT p1, PONT p2) {
     PONT retval;
     retval.x =   p1.y * p2.e - p1.e * p2.y;
     retval.y = - p1.x * p2.e + p1.e * p2.x;
     retval.e =   p1.x * p2.y - p1.y * p2.x;     
     return retval;
}

PONT pont_(int x, int y) {
     PONT p;
     p.x = x; p.y = y; p.e = 1;
     return p;
}

PONT egyenes(int x1, int y1, int x2, int y2) {
     /*PONT p1, p2;
     p1.x = x1; p1.y = y1;
     p2.x = x2; p2.y = y2;
     p1.e = p2.e = 1;
     return metsz_illeszt( p1 , p2 );*/
     return metsz_illeszt( pont_(x1,y1) , pont_(x2,y2) );
}

void rendez(PONT *t, int n, bool m) { // tömb, elemek száma, mód
     PONT tmp;
     int i, j;
     
     for (i=1;i<n;i++){
         tmp=t[i];
         j=i-1;
         for (;j>=0 && (m==true ? t[j].x/t[j].e>tmp.x/tmp.e : t[j].y/t[j].e>tmp.y/tmp.e);j--) t[j+1] = t[j];
         t[j+1] = tmp;
     }
}

int max(int a, int b) {
    return a>b?a:b;
}

int min(int a, int b) {
    return a<b?a:b;
}

int main( ) {

int gd, gm, gomb;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

int i, j, mn, centerX = getmaxx()/2, centerY = getmaxy()/2, x1, y1, x2, y2;
PONT m[20], mp;
bool mode; // rendezés módja (true = x szerint / false = y szerint)

PONT k[] = { centerX-50,centerY-50,1, // korona
             centerX+50,centerY-50,1,
             centerX+70,centerY-100,1,
             centerX+25,centerY-75,1,
             centerX,centerY-100,1,
             centerX-25,centerY-75,1,
             centerX-70,centerY-100,1};

PONT f[] = { centerX-50,centerY-50,1, // fej
             centerX+50,centerY-50,1,
             centerX+75,centerY,1,
             centerX+50,centerY+100,1,
             centerX-50,centerY+100,1,
             centerX-75,centerY,1};

// rajzolás
for (i=0;i<7;i++) line(k[i].x,k[i].y, k[(i+1)%7].x,k[(i+1)%7].y);
for (i=0;i<6;i++) line(f[i].x,f[i].y, f[(i+1)%6].x,f[(i+1)%6].y);

while (!kbhit()) {

    if (balgomb) gomb = 1; else gomb = (gomb==1?2:0); // bal egérgomb felengedése
    
    if (gomb == 2) {
       x1 = rand()%getmaxx(); y1 = rand()%getmaxy();
       x2 = egerx; y2 = egery;
       if (egerx == x1) mode = false; else mode = true;
       if (mode==true?x2<x1:y2<y1) { // szakasz végpontjainak rendezése
          x2 = x1; x1 = egerx;
          y2 = y1; y1 = egery;
       }
       
       putpixel(0,0,0);
       setcolor(15); setlinestyle(0,0,1); line(x1,y1,x2,y2);
       
       // koronán átmenõ nyilak
       m[0] = pont_(x1,y1); m[1] = pont_(x2,y2);
       mn = 2;
       for (i=0;i<7;i++) {
           mp = metsz_illeszt( egyenes(x1,y1,x2,y2) , egyenes(k[i].x,k[i].y,k[(i+1)%7].x,k[(i+1)%7].y) );
           if (mp.e != 0 && mp.x/mp.e >= min(k[i].x,k[(i+1)%7].x) && mp.x/mp.e <= max(k[i].x,k[(i+1)%7].x) && mp.y/mp.e >= min(k[i].y,k[(i+1)%7].y) && mp.y/mp.e <= max(k[i].y,k[(i+1)%7].y)) {
                    m[mn] = mp;
                    mn++;         
           }
       }
       rendez(m, mn, mode); setlinestyle(0,0,3); setcolor(2);
       for(j=0;(mode==true ? m[j].x/m[j].e!=x1 : m[j].y/m[j].e!=y1);j++);
       for(i=j;(mode==true ? m[i].x/m[i].e!=x2 : m[i].y/m[i].e!=y2);i++) {
          //setcolor(i%2==1 ? 2 : 15);
          if (i%2==1) line(m[i].x/m[i].e,m[i].y/m[i].e, m[i+1].x/m[i+1].e,m[i+1].y/m[i+1].e);
       }
       
       // fejen átmenõ nyilak
       m[0] = pont_(x1,y1); m[1] = pont_(x2,y2);
       mn = 2;
       for (i=0;i<6;i++) {
           mp = metsz_illeszt( egyenes(x1,y1,x2,y2) , egyenes(f[i].x,f[i].y,f[(i+1)%7].x,f[(i+1)%7].y) );
           if (mp.e != 0 && mp.x/mp.e >= min(f[i].x,f[(i+1)%7].x) && mp.x/mp.e <= max(f[i].x,f[(i+1)%7].x) && mp.y/mp.e >= min(f[i].y,f[(i+1)%7].y) && mp.y/mp.e <= max(f[i].y,f[(i+1)%7].y)) {
                    m[mn] = mp;
                    mn++;         
           }
       }
       rendez(m, mn, mode); setlinestyle(0,0,3); setcolor(4);
       for(j=0;(mode==true ? m[j].x/m[j].e!=x1 : m[j].y/m[j].e!=y1);j++);
       for(i=j;(mode==true ? m[i].x/m[i].e!=x2 : m[i].y/m[i].e!=y2);i++) {
          //setcolor(i%2==1 ? 4 : 15);
          if (i%2==1) line(m[i].x/m[i].e,m[i].y/m[i].e, m[i+1].x/m[i+1].e,m[i+1].y/m[i+1].e);
       }
    }
}

getch();
closegraph( );

return 0;
}
