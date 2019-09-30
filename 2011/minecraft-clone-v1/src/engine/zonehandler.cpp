#include "irrlicht.h"
#include "engine\zonehandler.h"

using namespace irr;
using namespace Engine;

ZoneHandler::ZoneHandler(ZoneGenerator *gen, BlockHandler *bh, scene::ISceneManager *smgr)
 : scene::ISceneNode(smgr->getRootSceneNode(), smgr, ZONE_MAP_ID)
 , gen(gen), bh(bh), smgr(smgr), mem_gridsize(10), view_gridsize(5)
{
    setAutomaticCulling(scene::EAC_OFF);
    own_material.Lighting = false;
    own_material.Thickness = 2.0f;
    selection = false;
}

ZoneHandler::~ZoneHandler()
{
    std::map<ZoneID,Zone*>::iterator i;
    for (i=zones.begin();i!=zones.end();i++)
    {
        delete i->second;
    }
    zones.clear();
}

ZoneID ZoneHandler::getZoneID(s32 x, s32 z)
{
    s32 x_ = (x<0 && x%ZONE_WIDTH) ? -ZONE_WIDTH : 0;
    s32 z_ = (z<0 && z%ZONE_WIDTH) ? -ZONE_WIDTH : 0;
    return ZoneID( (x/ZONE_WIDTH)*ZONE_WIDTH+x_ , (z/ZONE_WIDTH)*ZONE_WIDTH+z_ );
}

Zone* ZoneHandler::getZone(const ZoneID &z)
{
    std::map<ZoneID,Zone*>::iterator i = zones.find(z);
    if (i == zones.end())
        return NULL;
    else
        return i->second;
}

Zone* ZoneHandler::getZone(s32 x, s32 z)
{
    std::map<ZoneID,Zone*>::iterator i = zones.find( getZoneID(x,z) );
    if (i == zones.end())
        return NULL;
    else
        return i->second;
}

bool ZoneHandler::zoneValid(const ZoneID &z)
{
    std::map<ZoneID,Zone*>::iterator i = zones.find(z);
    return i != zones.end();
}

u8 ZoneHandler::getHeight(s32 x, s32 z, bool light)
{
    if(Zone *zone = getZone(x,z))
        return zone->getHeight(x-zone->X(),z-zone->Z(),light);
    else
        return 0;
}

BlockEx ZoneHandler::getBlock(core::vector3df v)
{
    return getBlock( v.X<0?v.X-1:v.X , v.Y , v.Z<0?v.Z-1:v.Z );
}

BlockEx ZoneHandler::getBlock(s32 x, s32 y, s32 z)
{
    if(Zone *zone = getZone(x,z))
        return zone->getBlock(x-zone->X(),y,z-zone->Z());
    else
        return BlockEx::createDummy(bh);
}

void ZoneHandler::addBlock(s32 x, s32 y, s32 z, Block block, bool playerAction)
{
    if(Zone *zone = getZone(x,z))
        zone->addBlock(x-zone->X(),y,z-zone->Z(),block,playerAction);
}

void ZoneHandler::removeBlock(s32 x, s32 y, s32 z, bool playerAction)
{
    if(Zone *zone = getZone(x,z))
        zone->removeBlock(x-zone->X(),y,z-zone->Z(),playerAction);
}

BlockEx ZoneHandler::getSelectedBlock()
{
    if (selection)
        return getBlock(selected.X,selected.Y,selected.Z);
    else
        return BlockEx::createDummy(bh);
}

void ZoneHandler::addToSelectedBlock(Block block)
{
    if (selection)
    {
        addBlock(to_place.X,to_place.Y,to_place.Z,block,true);
    }
}

void ZoneHandler::removeSelectedBlock()
{
    if (selection)
    {
        removeBlock(selected.X,selected.Y,selected.Z,true);
    }
}

BlockHandler* ZoneHandler::getBlockHandler()
{
    return bh;
}

scene::ISceneManager* ZoneHandler::getSceneManager()
{
    return smgr;
}

void ZoneHandler::setGridSize(u8 mem_gridsize, u8 view_gridsize)
{
    this->mem_gridsize = mem_gridsize;
    this->view_gridsize = view_gridsize;
}

void ZoneHandler::spawn(s32 x, s32 z)
{
    Zone* zone;
    if (zone = getZone(x,z))
    {
        smgr->getActiveCamera()->setPosition( core::vector3df(x+0.5f,zone->getHeight(x,z)+3,z+0.5f) );
    }
}

Zone* ZoneHandler::generateZone(const ZoneID& z)
{
    Zone* tmpzone;
    zones[z] = tmpzone = new Zone(this,z);
    gen->generate(tmpzone);
    return tmpzone;
}

void ZoneHandler::removeZone(const ZoneID& z)
{
}

bool ZoneHandler::zoneInRange(Zone* zone, s32 range)
{
    scene::ICameraSceneNode *cam = smgr->getActiveCamera();
    ZoneID tmp = getZoneID(cam->getPosition().X, cam->getPosition().Z);
    s32 x = zone->X(), z = zone->Z(), x_ = tmp.X(), z_ = tmp.Z();
    range *= ZONE_WIDTH;

    if (x >= x_-range && x <= x_+range && z >= z_-range && x <= z_+range)
        return true;
    else
        return false;
}

core::vector3di ZoneHandler::getBlockCoordinateFromVector(core::vector3df v)
{
    core::vector3di r;
    r.X = v.X<0 ? v.X-1 : v.X;
    r.Z = v.Z<0 ? v.Z-1 : v.Z;
    r.Y = v.Y;
    return r;
}

bool ZoneHandler::catchBlock()
{
    selection = false;
    scene::ICameraSceneNode *cam = smgr->getActiveCamera();
    core::vector3df pos = cam->getPosition(), v, v_prev, rel_targ = cam->getTarget()-pos;
    core::vector3di b;

    if (pos.Y < 0 || pos.Y > ZONE_HEIGHT)
    {
        selection = false;
        return false;
    }

    for (f64 i=0.0f;i<0.1f;i+=0.001f)
    {
        v = rel_targ*i + pos;
        b = getBlockCoordinateFromVector(v);
        if ( getBlock(b.X,b.Y,b.Z).getProperties().selectable )
        {
            selected = b;
            to_place = getBlockCoordinateFromVector(v_prev);
            selection = true;
            break;
        }
        v_prev = v;
    }

    return selection;
}

void ZoneHandler::drawBlockOutline(s32 x, s32 y, s32 z)
{
    video::IVideoDriver* driver = smgr->getVideoDriver();
    driver->draw3DBox(
                core::aabbox3df(core::vector3df(x,y,z), core::vector3df(x+1,y+1,z+1)),
                video::SColor(255,255,255,255)
                );
}

void ZoneHandler::OnRegisterSceneNode()
{
    if (IsVisible)
        smgr->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

bool ZoneHandler::BoxInFrustum(const core::matrix4& transform, const core::aabbox3df& box, scene::SViewFrustum frust)
{
    core::matrix4 invTrans(transform, core::matrix4::EM4CONST_INVERSE);
    frust.transform(invTrans);

    core::vector3df edges[8];
    box.getEdges(edges);

    for (s32 i=0; i<scene::SViewFrustum::VF_PLANE_COUNT; ++i)
    {
        bool boxInFrustum=false;
        for (u32 j=0; j<8; ++j)
        {
            if (frust.planes[i].classifyPointRelation(edges[j]) != core::ISREL3D_FRONT)
            {
                boxInFrustum=true;
                break;
            }
        }

        if (!boxInFrustum)
            return false;
    }

    return true;
}

void ZoneHandler::render()
{
    video::IVideoDriver* driver = smgr->getVideoDriver();

    ZoneID zone;
    Zone* tmpzone;
    scene::ICameraSceneNode *cam = smgr->getActiveCamera();
    core::vector3df cam_pos = cam->getPosition();
    ZoneID tmp = getZoneID(cam_pos.X, cam_pos.Z);
    s32 x = tmp.X(), z = tmp.Z(), i, j, grid = view_gridsize*ZONE_WIDTH;

    // a kamera nem mehet bele a 'solid' kockákba
    core::vector3di block = getBlockCoordinateFromVector(cam_pos);
    if ( getBlock(block.X,block.Y,block.Z).getProperties().solid )
        cam->setPosition(cam_pos + (cam_prev-cam_pos)*2);
    cam_prev = cam_pos;

    // kijelölt blokk keresése
    driver->setMaterial(own_material);
    driver->setTransform(video::ETS_WORLD, core::matrix4());
    if (catchBlock())
        drawBlockOutline(selected.X,selected.Y,selected.Z);

    // normál blokkok renderelése
    driver->setMaterial(bh->getMaterial1());
    for (i=x-grid;i<=x+grid;i+=ZONE_WIDTH)
    {
        for (j=z-grid;j<=z+grid;j+=ZONE_WIDTH)
        {
            zone = getZoneID(i,j);
            if (tmpzone = getZone(zone))
            {
                if ( BoxInFrustum(tmpzone->getTransform(), tmpzone->getBoundingBox(false), *cam->getViewFrustum()) )
                    tmpzone->render(false);
            }
            else
            {
                tmpzone = generateZone(zone);
            }
        }
    }

    // átlátszó blokkok renderelése
    driver->setMaterial(bh->getMaterial2());
    for (i=x-grid;i<=x+grid;i+=ZONE_WIDTH)
    {
        for (j=z-grid;j<=z+grid;j+=ZONE_WIDTH)
        {
            tmpzone = getZone(i,j);
            if ( BoxInFrustum(tmpzone->getTransform(), tmpzone->getBoundingBox(true), *cam->getViewFrustum()) )
                tmpzone->render(true);
        }
    }

    // memóriakezelés
    bool firstrun = camzone.invalid();
    if (tmp != camzone || firstrun)
    {
        std::map<ZoneID,Zone*>::iterator i;
        for (i=zones.begin();i!=zones.end();i++)
        {
            if ( !zoneInRange(i->second,mem_gridsize) )
                i->second->deInit();
        }

        camzone = tmp;
    }
}

const core::aabbox3d<f32>& ZoneHandler::getBoundingBox() const
{
    return Box;
}

u32 ZoneHandler::getMaterialCount() const
{
    return 1;
}

video::SMaterial& ZoneHandler::getMaterial(u32 i)
{
    if (i == 0)
        return bh->getMaterial1();
    else
        return bh->getMaterial2();
}
