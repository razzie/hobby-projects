#include "engine\zonegen.h"
#include <time.h>
#include <math.h>

using namespace irr;
using namespace Engine;

float Cosine_Interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1 - cos(ft)) * 0.5;
	return  a*(1-f) + b*f;
}

ZoneGenerator::ZoneGenerator(u8 water, u8 max_height, u8 min_height, u8 water_height)
 : water(water), max_height(max_height), min_height(min_height), water_height(water_height)
{
}

void ZoneGenerator::generate(Zone* zone)
{
    //srand(clock());
    u8 x,y,z;
    f32 p1,p2,p3;
    u8 block, tmp;
    u8 h[12], c[4]; // szomszédos kontrollpontok, saját kpontok
    /*
      10 9_________8 7
      11 |         | 6
         | 3     2 |      Z+
         |         |       |
         | 0     1 |       |
       0 |_________| 5     |_____X+
       1 2         3 4
    */
    h[0] = zone->getHeight(-1,0,true);
    h[1] = zone->getHeight(-1,-1,true);
    h[2] = zone->getHeight(0,-1,true);
    h[3] = zone->getHeight(ZONE_WIDTH-1,-1,true);
    h[4] = zone->getHeight(ZONE_WIDTH,-1,true);
    h[5] = zone->getHeight(ZONE_WIDTH,0,true);
    h[6] = zone->getHeight(ZONE_WIDTH,ZONE_WIDTH-1,true);
    h[7] = zone->getHeight(ZONE_WIDTH,ZONE_WIDTH,true);
    h[8] = zone->getHeight(ZONE_WIDTH-1,ZONE_WIDTH,true);
    h[9] = zone->getHeight(0,ZONE_WIDTH,true);
    h[10] = zone->getHeight(-1,ZONE_WIDTH,true);
    h[11] = zone->getHeight(-1,ZONE_WIDTH-1,true);

    for (int i=0;i<4;i++)
    {
        if (h[i*3] > 0)
        {
            c[i] = h[i*3];
            continue;
        }
        if (h[i*3+1] > 0)
        {
            c[i] = h[i*3+1];
            continue;
        }
        if (h[i*3+2] > 0)
        {
            c[i] = h[i*3+2];
            continue;
        }
        p1 = (rand()%(max_height-min_height)) + min_height;
        p2 = (rand()%(max_height-min_height)) + min_height;
        p3 = (rand()%(max_height-min_height)) + min_height;
        c[i] = (p1 + p2 + p3) / 3;
    }

    block = 1;

    for (x=0;x<ZONE_WIDTH;x++)
    {
        for (z=0;z<ZONE_WIDTH;z++)
        {
            p1 = Cosine_Interpolate(c[0],c[1],(float)(x+1)/ZONE_WIDTH);
            p2 = Cosine_Interpolate(c[3],c[2],(float)(x+1)/ZONE_WIDTH);
            p3 = Cosine_Interpolate(p1,p2,(float)(z+1)/ZONE_WIDTH);
            tmp = (u8)(p3 - 0.5);

            for (y=0;y<=tmp;y++)
            {
                zone->addBlock(x,y,z,Block(block));
            }
            if (tmp < water_height)
            {
                for (y=tmp+1;y<=water_height;y++)
                    zone->addBlock(x,y,z,Block(water));
            }
        }
    }
}
