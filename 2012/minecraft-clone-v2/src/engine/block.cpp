#include "engine\block.h"
#include "engine\blockhandler.h"
#include "engine\zonehandler.h"

using namespace irr;
using namespace Engine;

Block::Block(u8 ID, u8 data) : ID(ID), data(data)
{
}

Block::Block(u8 ID) : ID(ID), data(0)
{
}

Block::Block() : ID(0), data(0)
{
}

Block::Block(const Block &block) : ID(block.ID), data(block.data)
{
}


BlockEx::BlockEx(const BlockEx& blockex)
 : dummy(blockex.dummy), block(blockex.block), x(blockex.x), y(blockex.y), z(blockex.z), zone(blockex.zone), bh(blockex.bh)
{
}

BlockEx::BlockEx(const Block& block, s16 x, s16 y, s16 z, Zone* zone)
 : dummy(false), block(block), x(x), y(y), z(z), zone(zone), bh(zone->getZoneHandler()->getBlockHandler())
{
}

BlockEx::BlockEx(const Block& block, BlockHandler* bh)
: dummy(true), block(block), x(0), y(0), z(0), zone(NULL), bh(bh)
{
}

BlockEx BlockEx::createDummy(BlockHandler* bh)
{
    return BlockEx(bh->getDummy(),bh);
}

core::vector3di BlockEx::getRelativeCoords()
{
    return core::vector3di((s32)x,(s32)y,(s32)z);
}

core::vector3di BlockEx::getAbsoluteCoords()
{
    if (dummy)
        return getRelativeCoords();
    else
        return core::vector3di(x+zone->X(),y,z+zone->Z());
}

const bool BlockEx::isDummy() const
{
    return dummy;
}

const Block& BlockEx::getBlock() const
{
    return block;
}

BlockProperty BlockEx::getProperties() const
{
    return bh->getProperties(block);
}

video::S3DVertex makeVertex(
        u8 x, u8 y, u8 z,
        const core::vector2df &tcoord,
        const core::vector3df &normal = core::vector3df(0,0,0)
    )
{
    return video::S3DVertex(
                core::vector3df(x,y,z),
                normal,
                video::SColor(255,255,255,255),
                tcoord
    );
}

scene::IMeshBuffer& BlockEx::append()
{
    scene::CDynamicMeshBuffer buffer(video::EVT_STANDARD, video::EIT_32BIT);
    scene::IVertexBuffer* Vertices = &(buffer.getVertexBuffer());
    scene::IIndexBuffer* Indices = &(buffer.getIndexBuffer());

    append(Vertices, Indices);

    return buffer;
}

void BlockEx::append(scene::IVertexBuffer* Vertices, scene::IIndexBuffer* Indices)
{
    u32 v_count;
    core::vector3df normal;
    BlockProperty p = getProperties();

    if ( p.drawable )
    {
        //felsõ lap
        if (faceDrawable( zone->getBlock(x,y+1,z) ))
        {
            normal = core::vector3df(0,1,0);
            Vertices->push_back( makeVertex(x,y+1,z,p.top[0],normal) );
            Vertices->push_back( makeVertex(x,y+1,z+1,p.top[1],normal) );
            Vertices->push_back( makeVertex(x+1,y+1,z+1,p.top[2],normal) );
            Vertices->push_back( makeVertex(x+1,y+1,z,p.top[3],normal) );
            v_count = Vertices->size();
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-3);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-1);
        }
        //alsó lap
        if (y>0 && faceDrawable( zone->getBlock(x,y-1,z) ))
        {
            normal = core::vector3df(0,-1,0);
            Vertices->push_back( makeVertex(x,y,z,p.bottom[0],normal) );
            Vertices->push_back( makeVertex(x+1,y,z,p.bottom[1],normal) );
            Vertices->push_back( makeVertex(x+1,y,z+1,p.bottom[2],normal) );
            Vertices->push_back( makeVertex(x,y,z+1,p.bottom[3],normal) );
            v_count = Vertices->size();
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-3);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-1);
        }
        //bal oldali lap
        if (faceDrawable( zone->getBlock(x-1,y,z) ))
        {
            normal = core::vector3df(-1,0,0);
            Vertices->push_back( makeVertex(x,y+1,z,p.side[0],normal) );
            Vertices->push_back( makeVertex(x,y,z,p.side[1],normal) );
            Vertices->push_back( makeVertex(x,y,z+1,p.side[2],normal) );
            Vertices->push_back( makeVertex(x,y+1,z+1,p.side[3],normal) );
            v_count = Vertices->size();
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-3);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-1);
        }
        //jobb oldai lap
        if (faceDrawable( zone->getBlock(x+1,y,z)))
        {
            normal = core::vector3df(1,0,0);
            Vertices->push_back( makeVertex(x+1,y+1,z,p.side[0],normal) );
            Vertices->push_back( makeVertex(x+1,y+1,z+1,p.side[1],normal) );
            Vertices->push_back( makeVertex(x+1,y,z+1,p.side[2],normal) );
            Vertices->push_back( makeVertex(x+1,y,z,p.side[3],normal) );
            v_count = Vertices->size();
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-3);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-1);
        }
        //szemközti lap
        if (faceDrawable( zone->getBlock(x,y,z-1) ))
        {
            normal = core::vector3df(0,0,-1);
            Vertices->push_back( makeVertex(x,y+1,z,p.side[0],normal) );
            Vertices->push_back( makeVertex(x+1,y+1,z,p.side[1],normal) );
            Vertices->push_back( makeVertex(x+1,y,z,p.side[2],normal) );
            Vertices->push_back( makeVertex(x,y,z,p.side[3],normal) );
            v_count = Vertices->size();
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-3);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-1);
        }
        //hátsó lap
        if (faceDrawable( zone->getBlock(x,y,z+1) ))
        {
            normal = core::vector3df(0,0,1);
            Vertices->push_back( makeVertex(x,y+1,z+1,p.side[0],normal) );
            Vertices->push_back( makeVertex(x,y,z+1,p.side[1],normal) );
            Vertices->push_back( makeVertex(x+1,y,z+1,p.side[2],normal) );
            Vertices->push_back( makeVertex(x+1,y+1,z+1,p.side[3],normal) );
            v_count = Vertices->size();
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-3);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-4);
            Indices->push_back(v_count-2);
            Indices->push_back(v_count-1);
        }
    }
}

bool BlockEx::faceDrawable(const BlockEx& b)
{
    if (b.isDummy()) return false;

    if ( !b.getProperties().drawable ) return true;

    if ( b.getProperties().is_model ) return true;

    return false;
}
