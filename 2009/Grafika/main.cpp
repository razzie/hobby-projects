#include "graphics.h"
#include <stdio.h>

int main()
{
    int gd,gm,i;

    gd=VGA;gm=VGAHI;

    initgraph(&gd,&gm,"");

    for(i=0;i<=100;i+=10)
    {
        setcolor(i/10);
        fillellipse(200+i,150+i, 50+i/10,50-i/10);
        bar(150+i,250+i, 180+i,280+i);
    }

    moveto(100,300);
    lineto(200,350);
    lineto(150,450);
    
    printf("Valami\n");

    getch();

    closegraph();

    return(0);
}
