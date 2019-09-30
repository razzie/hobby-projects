#include "engine\zone.h"
#include "engine\zonehandler.h"

using namespace irr;
using namespace Engine;

Zone::Zone(ZoneHandler *map, ZoneID ID)
 : ID(ID), map(map)
 , block_count(0)
 , transform(core::IdentityMatrix)
{
    initalized = false;
    Init();

    neighbours[0] = NULL;
    neighbours[1] = NULL;
    neighbours[2] = NULL;
    neighbours[3] = NULL;

    transform.setTranslation( core::vector3df(ID.X(),0,ID.Z()) );

    for (u8 x=0;x<ZONE_WIDTH;x++)
        for (u8 z=0;z<ZONE_WIDTH;z++)
        {
            heightmap[x][z] = 0;
            lightmap[x][z] = 0;
        }
}

Zone::~Zone()
{
    deInit();
}

s32 Zone::X()
{
    return ID.X();
}

s32 Zone::Z()
{
    return ID.Z();
}

u8 Zone::getHeight(s16 x, s16 z, bool light)
{
    if (x>=0 && x<ZONE_WIDTH && z>=0 && z<ZONE_WIDTH)
        return light ? lightmap[x][z] : heightmap[x][z];
    else
        return map->getHeight(x+ID.X(),z+ID.Z(),light);
}

BlockEx Zone::getBlock(s16 x, s16 y, s16 z)
{
    if (y<0 && y>=ZONE_HEIGHT)
        return BlockEx::createDummy(map->getBlockHandler());
    else if (x>=0 && x<ZONE_WIDTH && z>=0 && z<ZONE_WIDTH)
        return BlockEx(blocks[x][y][z], x,y,z, this);
    else
        //return map->getBlock(x+ID.X(),y,z+ID.Z());
        {
            if (neighbours[0] && x<0) return neighbours[0]->getBlock(x+ZONE_WIDTH,y,z);
            if (neighbours[1] && x>=ZONE_WIDTH) return neighbours[1]->getBlock(x-ZONE_WIDTH,y,z);
            if (neighbours[2] && z<0) return neighbours[2]->getBlock(x,y,z+ZONE_WIDTH);
            if (neighbours[3] && z>=ZONE_WIDTH) return neighbours[3]->getBlock(x,y,z-ZONE_WIDTH);
        }
    return BlockEx::createDummy(map->getBlockHandler());
}

void Zone::addBlock(u8 x, u8 y, u8 z, Block block, bool playerAction)
{
    bool accept = false;
    Block tmp = blocks[x][y][z];
    BlockProperty p = map->getBlockHandler()->getProperties(block);
    Zone *zone;

    if (playerAction)
    {
        if ( map->getBlockHandler()->getProperties(tmp).replaceable )
        {
            accept = true;
        }
    }
    else
    {
        accept = true;
    }

    if ( accept )
    {
        dirty = true;
        if ( x==0 && (zone = map->getZone(ID.X()-ZONE_WIDTH,ID.Z())) ) zone->makeDirty();
        if ( x==ZONE_WIDTH-1 && (zone = map->getZone(ID.X()+ZONE_WIDTH,ID.Z())) ) zone->makeDirty();
        if ( z==0 && (zone = map->getZone(ID.X(),ID.Z()-ZONE_WIDTH)) ) zone->makeDirty();
        if ( z==ZONE_WIDTH-1 && (zone = map->getZone(ID.X(),ID.Z()+ZONE_WIDTH)) ) zone->makeDirty();

        blocks[x][y][z] = block;

        if ( p.drawable && heightmap[x][z] < y ) heightmap[x][z] = y;
        if ( p.drawable && !p.transparent && lightmap[x][z] < y ) lightmap[x][z] = y;
    }
}

void Zone::removeBlock(u8 x, u8 y, u8 z, bool playerAction)
{
    bool accept = false;
    Zone *zone;
    BlockProperty p = map->getBlockHandler()->getProperties(blocks[x][y][z]), p2;

    if (playerAction)
    {
        if ( p.breakable )
        {
            accept = true;
        }
    }
    else if (blocks[x][y][z].getID() != 0)
    {
        accept = true;
    }

    if ( accept )
    {
        dirty = true;
        if ( x==0 && (zone = map->getZone(ID.X()-ZONE_WIDTH,ID.Z())) ) zone->makeDirty();
        if ( x==ZONE_WIDTH-1 && (zone = map->getZone(ID.X()+ZONE_WIDTH,ID.Z())) ) zone->makeDirty();
        if ( z==0 && (zone = map->getZone(ID.X(),ID.Z()-ZONE_WIDTH)) ) zone->makeDirty();
        if ( z==ZONE_WIDTH-1 && (zone = map->getZone(ID.X(),ID.Z()+ZONE_WIDTH)) ) zone->makeDirty();

        blocks[x][y][z] = Block();

        if ( heightmap[x][z] == y )
        {
            for (int i=y-1;i>=0;i--)
            {
                p2 = map->getBlockHandler()->getProperties(blocks[x][i][z]);
                if ( p2.drawable )
                {
                    heightmap[x][z] = i;
                    break;
                }
            }
        }
        if ( lightmap[x][z] == y )
        {
            for (int i=y-1;i>=0;i--)
            {
                p2 = map->getBlockHandler()->getProperties(blocks[x][i][z]);
                if ( p2.drawable && !p2.transparent )
                {
                    lightmap[x][z] = i;
                    break;
                }
            }
        }
    }
}

void* Zone::getBlockBuffer()
{
    return (void*)(&blocks[0][0][0]);
}

u32 Zone::getBlockCount()
{
    return block_count;
}

void Zone::makeDirty()
{
    dirty = true;
}

void Zone::Init()
{
    if (!initalized)
    {
        buffer = new scene::CDynamicMeshBuffer(video::EVT_STANDARD, video::EIT_32BIT);
        transp = new scene::CDynamicMeshBuffer(video::EVT_STANDARD, video::EIT_32BIT);
        buffer->setHardwareMappingHint(scene::EHM_STATIC);
        transp->setHardwareMappingHint(scene::EHM_STATIC);
        initalized = true;
        dirty = true;
    }
}

void Zone::deInit()
{
    if (initalized)
    {
        video::IVideoDriver* driver = map->getSceneManager()->getVideoDriver();
        driver->removeHardwareBuffer(buffer);
        driver->removeHardwareBuffer(transp);
        buffer->drop();
        transp->drop();
        dirty = true;
        initalized = false;
    }
}

void Zone::build()
{
    if (!initalized) Init();
    updateNeighbours();
    buffer->getVertexBuffer().set_used(0);
    buffer->getIndexBuffer().set_used(0);
    transp->getVertexBuffer().set_used(0);
    transp->getIndexBuffer().set_used(0);
    block_count = 0;
    u32 height = 0, height_transp = 0, minheight = 0, minheight_transp = 0;
    int x,y,z;

    for (x=0;x<ZONE_WIDTH;x++)
    {
    for (z=0;z<ZONE_WIDTH;z++)
    {
    for (y=heightmap[x][z];y>=0;y--)
    {
        BlockEx b = getBlock(x,y,z);
        BlockProperty p = b.getProperties();
        if ( p.drawable )
        {
            block_count++;
            if (!p.transparent)
            {
                if (y>height) height = y;
                minheight = y;
                b.append(&(buffer->getVertexBuffer()),&(buffer->getIndexBuffer()));
            }
            else
            {
                if (y>height_transp) height_transp = y;
                minheight_transp = y;
                b.append(&(transp->getVertexBuffer()), &(transp->getIndexBuffer()));
            }
        }
    }
    }
    }
    buffer->setBoundingBox(
        core::aabbox3df(
                core::vector3df(0,minheight,0),
                core::vector3df(ZONE_WIDTH,height+1,ZONE_WIDTH)
        )
    );
    buffer->setDirty(scene::EBT_VERTEX_AND_INDEX);
    transp->setBoundingBox(
        core::aabbox3df(
                core::vector3df(0,minheight_transp,0),
                core::vector3df(ZONE_WIDTH,height_transp+1,ZONE_WIDTH)
        )
    );
    transp->setDirty(scene::EBT_VERTEX_AND_INDEX);
    dirty = false;
}

void Zone::render(bool transparents)
{
    video::IVideoDriver* driver = map->getSceneManager()->getVideoDriver();
    driver->setTransform(video::ETS_WORLD, transform);

    if (dirty) build();

    if (!transparents)
        driver->drawMeshBuffer(buffer);
    else
        driver->drawMeshBuffer(transp);
}

ZoneHandler* Zone::getZoneHandler()
{
    return map;
}

const core::matrix4& Zone::getTransform()
{
    return transform;
}

const core::aabbox3df Zone::getBoundingBox(bool transparent)
{
    if (initalized)
    {
        if (!transparent)
            return buffer->getBoundingBox();
        else
            return transp->getBoundingBox();
    }
    else
        return core::aabbox3df( core::vector3df(0,0,0),core::vector3df(ZONE_WIDTH,ZONE_HEIGHT,ZONE_WIDTH) );
}

scene::IMeshBuffer* Zone::getMeshBuffer(bool transparent)
{
    if (initalized)
    {
        if (!transparent)
            return buffer;
        else
            return transp;
    }
    else
        return NULL;
}

void Zone::updateNeighbours()
{
    if (!neighbours[0]) neighbours[0] = map->getZone(X()-ZONE_WIDTH,Z());
    if (!neighbours[1]) neighbours[1] = map->getZone(X()+ZONE_WIDTH,Z());
    if (!neighbours[2]) neighbours[2] = map->getZone(X(),Z()-ZONE_WIDTH);
    if (!neighbours[3]) neighbours[3] = map->getZone(X(),Z()+ZONE_WIDTH);
}
