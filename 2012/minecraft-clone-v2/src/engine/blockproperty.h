#ifndef _BLOCKPROPERTY_H_
#define _BLOCKPROPERTY_H_

#include "irrlicht.h"

using namespace irr;

namespace Engine
{

struct BlockProperty
{
    bool drawable;
    //bool is_step;
    bool is_model;
    //bool transparent;
    bool solid;
    bool selectable;
    bool replaceable;
    bool breakable;
    u16 commonness;
    core::vector2df top[4], bottom[4], side[4]; // textúra koordináták
    scene::IMesh* mesh;
};

};

#endif // _BLOCKPROPERTY_H_
