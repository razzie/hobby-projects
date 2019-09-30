# include "graphics.h"
# include <conio.h>

typedef struct pont
{
    int x, y;
} PONT;

int main()
{
int gd,gm;
PONT poly[6] = { 100,80, 500,175, 320,330 };
gd=VGA;gm=VGAMED;
initgraph(&gd,&gm,"");
setcolor( 4 );
fillpoly( 3, (int*)poly );
getch();
closegraph();
return(0);
}
