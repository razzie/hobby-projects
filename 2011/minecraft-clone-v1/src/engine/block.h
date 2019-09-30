#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "irrlicht.h"
#include "engine\blockhandler.h"

using namespace irr;

namespace Engine
{

class Block
{
public:
    Block(u8 ID, u8 data);
    Block(u8 ID);
    Block();
    Block(const Block &block);

    bool operator== (const Block &block) const;
    bool operator!= (const Block &block) const;

    u8 getID() const;
    u8 getData() const;
    void setData(u8 data);

private:
    u8 ID;
    u8 data;
};

class Zone;

class BlockEx
{
public:
    BlockEx(const BlockEx& blockex);
    BlockEx(const Block& block, s16 x, s16 y, s16 z, Zone* zone);
    static BlockEx createDummy(BlockHandler* bh);

    core::vector3di getRelativeCoords();
    core::vector3di getAbsoluteCoords();

    const bool isDummy() const;
    const Block& getBlock() const;
    BlockProperty getProperties() const;
    scene::IMeshBuffer& append();
    void append(scene::IVertexBuffer* Vertices, scene::IIndexBuffer* Indices);

private:
    bool dummy;
    const Block& block;
    const s16 x;
    const s16 y;
    const s16 z;
    Zone* zone;
    BlockHandler* bh;

    BlockEx(const Block& block, BlockHandler* bh);

    bool faceDrawable(const BlockEx& b);
};

};

#endif // _BLOCK_H_
