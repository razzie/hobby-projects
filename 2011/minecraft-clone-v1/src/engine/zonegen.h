#ifndef _ZONEGEN_H_
#define _ZONEGEN_H_

#include "irrlicht.h"
#include "engine\zone.h"

using namespace irr;

namespace Engine
{

class ZoneGenerator
{
public:
    ZoneGenerator(
            u8 water,
            u8 max_height = ZONE_HEIGHT/2+20,
            u8 min_height = ZONE_HEIGHT/2-10,
            u8 water_height = ZONE_HEIGHT/2
    );
    void generate(Zone* zone);
protected:
    u8 water, max_height, min_height, water_height;
};

};

#endif // _ZONEGEN_H_
