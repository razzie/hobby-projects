# include "graphics.h"
# include <stdlib.h>

void flood_fill(int x, int y, __int8 *minta) {
    int N = minta[0], M = minta[1]; //oszlopok száma (sorméret/szélesség) ÉS sorok száma (magasság)
    if (getpixel(x, y) == 0) {
        putpixel(x, y, minta[(y % M) * N + (x % N) + 2]);
        flood_fill(x+1,y,minta); 
        flood_fill(x-1,y,minta); 
        flood_fill(x,y+1,minta); 
        flood_fill(x,y-1,minta); 
    }
}

int main( ) {

int gd, gm, page = 0, ap;
gd = VGA; gm = VGAMED;
initgraph( &gd, &gm, "" );

int i,j, centerX = getmaxx()/2, centerY = getmaxy()/2;

__int8 ful[] = { 3, 3, // oszlop- és sorszám
                 6, 7, 7, // minta kezdete
                 7,12,12,
                 7,12,12};

__int8 fej[] = { 6, 3,
                 6, 6, 6, 6, 6, 6,
                 6, 6, 6, 6, 6, 6,
                 6, 6, 2, 6, 6,10};

__int8 orr[] = { 8, 8,
                 5, 5, 6, 6, 5, 5, 6, 6,
                 5, 5, 6, 6, 5, 5, 6, 6,
                 6, 6, 4, 4, 6, 6,12,12,
                 6, 6, 4, 4, 6, 6,12,12,
                 5, 5, 6, 6, 5, 5, 6, 6,
                 5, 5, 6, 6, 5, 5, 6, 6,
                 6, 6,12,12, 6, 6, 4, 4,
                 6, 6,12,12, 6, 6, 4, 4};

__int8 fog[] = { 4, 3,
                 7, 9,13, 9,
                 9,13, 9, 7,
                13, 9, 7, 9};

__int8 szem[] = { 4, 2,
                  1, 1, 1, 1,
                  1, 9, 1,15};

circle(centerX, centerY, 50); // feje
{
     line(centerX-75,centerY-75, centerX-50,centerY-25); // bal füle
     line(centerX-75,centerY-75, centerX-25,centerY-50);
     line(centerX-50,centerY-25, centerX-25,centerY-51);
}
{
     line(centerX+75,centerY-75, centerX+50,centerY-25); // jobb füle
     line(centerX+75,centerY-75, centerX+25,centerY-50);
     line(centerX+50,centerY-25, centerX+25,centerY-51);
}
circle(centerX, centerY, 10); // orra
line(centerX-25,centerY+25, centerX+25,centerY+25); // szája
rectangle(centerX-12,centerY+25, centerX+12,centerY+40); // foga
{
     setcolor(15);
     fillellipse(centerX-20,centerY-20, 10,15); // bal szeme
     setcolor(0);
     fillellipse(centerX-17,centerY-20, 5,6);
}
{
     setcolor(15);
     fillellipse(centerX+20,centerY-20, 10,15); // jobb szeme
     setcolor(0);
     fillellipse(centerX+17,centerY-20, 5,6);
}

getch();

// kitöltések
flood_fill(centerX-50,centerY-50,ful);
flood_fill(centerX+50,centerY-50,ful);
flood_fill(centerX,centerY-25,fej);
flood_fill(centerX,centerY,orr);
flood_fill(centerX,centerY+30,fog);
flood_fill(centerX-17,centerY-20,szem);
flood_fill(centerX+17,centerY-20,szem);



getch();

closegraph( );

return 0;

}
