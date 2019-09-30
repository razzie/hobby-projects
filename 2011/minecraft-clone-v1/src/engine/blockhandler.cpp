#include "engine\blockhandler.h"
#include "engine\block.h"

#define DUMMY_BLOCK Block(0,1)

using namespace irr;
using namespace Engine;

BlockHandler::BlockHandler(video::SMaterial* material)
 : material(material), transp_material(material)
{
    Initalize();
}

BlockHandler::BlockHandler(video::SMaterial* material, video::SMaterial* transp_material)
 : material(material), transp_material(transp_material)
{
    Initalize();
}

void BlockHandler::Initalize()
{
    BlockProperty def;
    def.drawable = false;
    def.is_step = false;
    def.is_model = false;
    def.transparent = false;
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

BlockProperty BlockHandler::getProperties(u8 blocktype)
{
    return BlockTypes[blocktype];
}

BlockProperty BlockHandler::getProperties(const Block &block)
{
    return BlockTypes[block.getID()];
}

video::SMaterial& BlockHandler::getMaterial1()
{
    return *material;
}

video::SMaterial& BlockHandler::getMaterial2()
{
    return *transp_material;
}
