#ifndef _ZONE_H_
#define _ZONE_H_

#define ZONE_WIDTH 16
#define ZONE_HEIGHT 128

#include "irrlicht.h"
#include "engine\zoneid.h"
#include "engine\block.h"

using namespace irr;

namespace Engine
{

class ZoneHandler;

class ZoneGenerator;

class Zone
{
public:
    Zone(ZoneHandler *map, ZoneID ID);
    ~Zone();

    ZoneID getID();
    s32 X();
    s32 Z();

    u8 getHeight(s16 x, s16 z, bool light = false);
    BlockEx getBlock(s16 x, s16 y, s16 z);
    void addBlock(u8 x, u8 y, u8 z, Block block, bool playerAction = false);
    void removeBlock(u8 x, u8 y, u8 z, bool playerAction = false);
    void* getBlockBuffer();
    u32 getBlockCount();

    void makeDirty();
    void Init();
    void deInit();
    void build();
    void render(bool transparents);

    ZoneHandler* getZoneHandler();
    const core::matrix4& getTransform();
    const core::aabbox3df getBoundingBox(bool transparent);
    scene::IMeshBuffer* getMeshBuffer(bool transparent);

private:
    ZoneID ID;
    ZoneHandler *map;

    Zone* neighbours[4]; // (x-1)(x+1)(z-1)(z+1)
    void updateNeighbours();

    Block blocks[ZONE_WIDTH][ZONE_HEIGHT][ZONE_WIDTH];
    u8 heightmap[ZONE_WIDTH][ZONE_WIDTH];
    u8 lightmap[ZONE_WIDTH][ZONE_WIDTH];

    bool dirty;
    bool initalized;

    scene::CDynamicMeshBuffer *buffer;
    scene::CDynamicMeshBuffer *transp;
    core::matrix4 transform;
    u32 block_count;
};

};

#endif // _ZONE_H_
