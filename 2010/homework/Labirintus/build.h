#ifndef _BUILD_H_
#define _BUILD_H_

#include "irrlicht.h"
#include "labyrinth.h"

using namespace irr;

typedef struct {
    scene::IAnimatedMesh* mesh;
    video::ITexture* texture;
    video::ITexture* bumpmap;
} WallStruct;

typedef WallStruct FloorStruct;

typedef struct {
    WallStruct *wall_list;
    int wall_count;
    FloorStruct *floor_list;
    int floor_count;
} BuildStruct;

void GenerateNew(const BuildStruct& bs, int size, int levels, int wall1, int wall2, int wall3, bool lights, bool bush, bool collision, bool circle = false);
void Build(const LStruct &l, const BuildStruct& bs, bool light, int level = 0, bool ceiling = false);

#endif
