#include "engine\blockhandler.h"
#include "engine\block.h"

#define DUMMY_BLOCK Block(0,1)

using namespace irr;
using namespace Engine;

BlockHandler::BlockHandler(video::SMaterial* material)
 : material(material)
{
    Initalize();
}

void BlockHandler::Initalize()
{
    BlockProperty def;
    def.drawable = false;
    //def.is_step = false;
    def.is_model = false;
    //def.transparent = false;
    def.solid = false;
    def.selectable = false;
    def.replaceable = true;
    def.breakable = true;
    def.commonness = 0;

    for (u32 i=0;i<256;i++)
        BlockTypes[i] = def;
}

void BlockHandler::setBlocktype(u8 ID, BlockProperty& p)
{
    BlockTypes[ID] = p;
}

const Block& BlockHandler::getDummy() const
{
    return DUMMY_BLOCK;
}

Block BlockHandler::getBlock(float n)
{
    /*if ( n < 0.4 ) return Block();
    else if ( n < 0.6 ) return Block(6);
    else if ( n < 0.8 ) return Block(2);
    else if ( n < 1.0 ) return Block(3);
    else return Block(5);*/
    if ( n < 3.1 ) return Block();
    else if ( n < 3.4) return Block(6);
    else if ( n < 3.7 ) return Block(2);
    else if ( n < 4.0 ) return Block(3);
    else return Block(5);
}

BlockProperty BlockHandler::getProperties(u8 blocktype)
{
    return BlockTypes[blocktype];
}

BlockProperty BlockHandler::getProperties(const Block &block)
{
    return BlockTypes[block.ID];
}

video::SMaterial& BlockHandler::getMaterial()
{
    return *material;
}
