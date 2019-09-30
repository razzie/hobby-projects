#include "engine\zone.h"
#include "engine\zonehandler.h"

//#define SHADOW 150

using namespace irr;
using namespace Engine;

/*video::S3DVertex makeVertex(
        u8 x, u8 y, u8 z,
        const core::vector2df &tcoord,
        const core::vector3df &normal = core::vector3df(0,0,0),
        u8 light = 255,
        u8 alpha = 255
    )
{
    //if (alpha < 255) light = 255;
    return video::S3DVertex(
                core::vector3df(x,y,z),
                normal,
                video::SColor(alpha,light,light,light),
                tcoord
    );
}

void Zone::build()
{
    if (!initalized) Init();
    scene::IVertexBuffer* Vertices;
    scene::IIndexBuffer* Indices;
    scene::IVertexBuffer& buffer_vb = buffer->getVertexBuffer();
    scene::IIndexBuffer& buffer_ib = buffer->getIndexBuffer();
    scene::IVertexBuffer& transp_vb = transp->getVertexBuffer();
    scene::IIndexBuffer& transp_ib = transp->getIndexBuffer();
    buffer_vb.set_used(0);
    buffer_ib.set_used(0);
    transp_vb.set_used(0);
    transp_ib.set_used(0);
    BlockHandler *bh = map->getBlockHandler();
    block_count = 0;
    u32 height = 0, height_transp = 0, minheight = 0, minheight_transp = 0, v_count;
    u8 light, alpha;
    int x,y,z;
    core::vector3df normal;

    for (x=0;x<ZONE_WIDTH;x++)
    {
        for (z=0;z<ZONE_WIDTH;z++)
        {
            for (y=heightmap[x][z];y>=0;y--)
            {
                Block b = blocks[x][y][z];
                BlockProperty p = bh->getProperties(b);
                if ( p.cube )
                {
                    block_count++;
                    if (!p.transparent)
                    {
                        if (y>height) height = y;
                        minheight = y;
                        Vertices = &buffer_vb;
                        Indices = &buffer_ib;
                        alpha = 255;
                    }
                    else
                    {
                        if (y>height_transp) height_transp = y;
                        minheight_transp = y;
                        Vertices = &transp_vb;
                        Indices = &transp_ib;
                        alpha = 50;
                    }
                    //felsõ lap
                    if (b.faceDrawable( getBlock(x,y+1,z), bh ))
                    {
                        normal = core::vector3df(0,1,0);
                        if ( y < lightmap[x][z] ) light = 255-SHADOW;
                        else light = 255;
                        Vertices->push_back( makeVertex(x,y+1,z,p.top[0],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x,y+1,z+1,p.top[1],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y+1,z+1,p.top[2],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y+1,z,p.top[3],normal,light,alpha) );
                        v_count = Vertices->size();
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-3);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-1);
                    }
                    //alsó lap
                    if ( y>0 && b.faceDrawable( getBlock(x,y-1,z), bh ))
                    {
                        normal = core::vector3df(0,-1,0);
                        light = 255-SHADOW;
                        Vertices->push_back( makeVertex(x,y,z,p.bottom[0],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y,z,p.bottom[1],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y,z+1,p.bottom[2],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x,y,z+1,p.bottom[3],normal,light,alpha) );
                        v_count = Vertices->size();
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-3);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-1);
                    }
                    //bal oldali lap
                    if (b.faceDrawable( getBlock(x-1,y,z), bh ))
                    {
                        normal = core::vector3df(-1,0,0);
                        if ( y < getHeight(x-1,z,true) ) light = 255-SHADOW;
                        else light = 255;
                        Vertices->push_back( makeVertex(x,y+1,z,p.side[0],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x,y,z,p.side[1],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x,y,z+1,p.side[2],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x,y+1,z+1,p.side[3],normal,light,alpha) );
                        v_count = Vertices->size();
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-3);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-1);
                    }
                    //jobb oldai lap
                    if (b.faceDrawable( getBlock(x+1,y,z), bh ))
                    {
                        normal = core::vector3df(1,0,0);
                        if ( y < getHeight(x+1,z,true) ) light = 255-SHADOW;
                        else light = 255;
                        Vertices->push_back( makeVertex(x+1,y+1,z,p.side[0],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y+1,z+1,p.side[1],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y,z+1,p.side[2],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y,z,p.side[3],normal,light,alpha) );
                        v_count = Vertices->size();
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-3);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-1);
                    }
                    //szemközti lap
                    if (b.faceDrawable( getBlock(x,y,z-1), bh ))
                    {
                        normal = core::vector3df(0,0,-1);
                        if ( y < getHeight(x,z-1,true) ) light = 255-SHADOW;
                        else light = 255;
                        Vertices->push_back( makeVertex(x,y+1,z,p.side[0],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y+1,z,p.side[1],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y,z,p.side[2],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x,y,z,p.side[3],normal,light,alpha) );
                        v_count = Vertices->size();
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-3);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-1);
                    }
                    //hátsó lap
                    if (b.faceDrawable( getBlock(x,y,z+1), bh ))
                    {
                        normal = core::vector3df(0,0,1);
                        if ( y < getHeight(x,z+1,true) ) light = 255-SHADOW;
                        else light = 255;
                        Vertices->push_back( makeVertex(x,y+1,z+1,p.side[0],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x,y,z+1,p.side[1],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y,z+1,p.side[2],normal,light,alpha) );
                        Vertices->push_back( makeVertex(x+1,y+1,z+1,p.side[3],normal,light,alpha) );
                        v_count = Vertices->size();
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-3);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-4);
                        Indices->push_back(v_count-2);
                        Indices->push_back(v_count-1);
                    }
                }
                else
                {
                    // modellek rajzolása késõbb
                }
            }
        }
    }
    buffer->setBoundingBox(
        core::aabbox3df(
                core::vector3df(0,minheight,0),
                core::vector3df(ZONE_WIDTH,height,ZONE_WIDTH)
        )
    );
    buffer->setDirty(scene::EBT_VERTEX_AND_INDEX);
    transp->setBoundingBox(
        core::aabbox3df(
                core::vector3df(0,minheight_transp,0),
                core::vector3df(ZONE_WIDTH,height_transp,ZONE_WIDTH)
        )
    );
    transp->setDirty(scene::EBT_VERTEX_AND_INDEX);
    dirty = false;
}*/

void Zone::build()
{
    if (!initalized) Init();
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
                core::vector3df(ZONE_WIDTH,height,ZONE_WIDTH)
        )
    );
    buffer->setDirty(scene::EBT_VERTEX_AND_INDEX);
    transp->setBoundingBox(
        core::aabbox3df(
                core::vector3df(0,minheight_transp,0),
                core::vector3df(ZONE_WIDTH,height_transp,ZONE_WIDTH)
        )
    );
    transp->setDirty(scene::EBT_VERTEX_AND_INDEX);
    dirty = false;
}

/*
   Y
   |   Z
   |  /
   | /
   |/_ _ _ X
*/
