#ifndef _ZONEHANDLER_H_
#define _ZONEHANDLER_H_

#include "irrlicht.h"
#include "engine\zone.h"
#include "engine\zonegen.h"
#include <map>

#define ZONE_MAP_ID 1

using namespace irr;

namespace Engine
{

class ZoneHandler : public scene::ISceneNode
{
public:
    ZoneHandler(ZoneGenerator* gen, BlockHandler *bh, scene::ISceneManager *smgr);
    ~ZoneHandler();

    ZoneID getZoneID(s32 x, s32 z);
    Zone* getZone(const ZoneID &z);
    Zone* getZone(s32 x, s32 z);
    bool zoneValid(const ZoneID &z);

    u8 getHeight(s32 x, s32 z, bool light = false);
    BlockEx getBlock(core::vector3df v);
    BlockEx getBlock(s32 x, s32 y, s32 z);
    void addBlock(s32 x, s32 y, s32 z, Block block, bool playerAction);
    void removeBlock(s32 x, s32 y, s32 z, bool playerAction);

    BlockEx getSelectedBlock();
    void addToSelectedBlock(Block block);
    void removeSelectedBlock();
    //void triggerSelectedBlock();

    BlockHandler* getBlockHandler();
    scene::ISceneManager* getSceneManager();

    void setGridSize(u8 mem_gridsize, u8 view_gridsize);
    void spawn(s32 x, s32 z);

    void OnRegisterSceneNode();
    void render();
    const core::aabbox3d<f32>& getBoundingBox() const;
    u32 getMaterialCount() const;
    video::SMaterial& getMaterial(u32 i);

private:
    std::map<ZoneID, Zone*> zones;
    ZoneGenerator *gen;
    BlockHandler *bh;
    u8 mem_gridsize, view_gridsize;
    scene::ISceneManager *smgr;
    core::aabbox3d<f32> Box;
    video::SMaterial own_material;
    ZoneID camzone;
    core::vector3df cam_prev;
    core::vector3di selected, to_place;
    bool selection;

    Zone* generateZone(const ZoneID& z);
    void removeZone(const ZoneID& z);
    bool zoneInRange(Zone* zone, s32 range);
    void drawBlockOutline(s32 x, s32 y, s32 z);
    bool catchBlock();

    static core::vector3di getBlockCoordinateFromVector(core::vector3df v);
    static bool BoxInFrustum(const core::matrix4& transform, const core::aabbox3df& box, scene::SViewFrustum frust);
};

};

#endif // _ZONEHANDLER_H_
