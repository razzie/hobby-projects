#ifndef _BLOCKHANDLER_H_
#define _BLOCKHANDLER_H_

#include "irrlicht.h"
#include "engine\blockproperty.h"

using namespace irr;

namespace Engine
{

class Block;

class BlockHandler
{
public:
    BlockHandler(video::SMaterial* material);
    BlockHandler(video::SMaterial* material, video::SMaterial* transp_material);
    ~BlockHandler();

    void setBlocktype(u8 ID, BlockProperty& p);

    const Block& getDummy() const;

    BlockProperty getProperties(u8 blocktype);
    BlockProperty getProperties(const Block &block);

    video::SMaterial& getMaterial1();
    video::SMaterial& getMaterial2();

private:
    void Initalize();

    video::SMaterial *material, *transp_material;
    BlockProperty BlockTypes[256];
};

};

#endif // _BLOCKHANDLER_H_
