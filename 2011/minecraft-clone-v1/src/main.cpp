#include "irrlicht.h"
#include "engine\zonehandler.h"
#include <time.h>

#include "engine\postproc\CRendererPostProc.h"
#include "engine\postproc\CEffectPostProc.h"
#include "engine\postproc\CLensFlarePostProc.h"
//#include "engine\postproc\CTransitionPostProc.h"
//#include "engine\postproc\CSplitPostProc.h"

using namespace irr;
using namespace Engine;

void calcTexCoords(core::vector2df* array, f32 x, f32 y, f32 size = 0.125f)
{
    array[3] = core::vector2df(x,y);
    array[2] = core::vector2df(x+size,y);
    array[1] = core::vector2df(x+size,y+size);
    array[0] = core::vector2df(x,y+size);
}

int main()
{
    IrrlichtDevice *device = createDevice(video::EDT_DIRECT3D9,
        core::dimension2d<u32>(800, 600), 32, false, false);

    if (device == 0)
        return 1;

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);


    scene::ICameraSceneNode *cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.05f);
    smgr->addSkyDomeSceneNode(driver->getTexture("resources/skydome.bmp"));
    scene::ISceneNode* sun = smgr->addBillboardSceneNode(0, core::dimension2df(10.0f,10.0f), core::vector3df(0.0f,128.0f,0.0f));
        sun->setMaterialTexture(0, driver->getTexture("resources/sun.png"));
        sun->setMaterialFlag( video::EMF_LIGHTING, false );


    video::SMaterial material;
        material.setTexture(0, driver->getTexture("resources/texture.bmp"));
        material.setTexture(1, driver->getTexture("resources/reflection.bmp"));
        material.setFlag(video::EMF_BILINEAR_FILTER, false);
        material.Lighting = false;
        material.MaterialType = video::EMT_REFLECTION_2_LAYER;
        material.FogEnable = true;
    video::SMaterial transp_material;
        transp_material.setTexture(0, driver->getTexture("resources/texture_transp.bmp"));
        transp_material.setFlag(video::EMF_BILINEAR_FILTER, false);
        transp_material.Lighting = false;
        transp_material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

    BlockHandler* bh = new BlockHandler(&material,&transp_material);
    ZoneHandler* map = new ZoneHandler(new ZoneGenerator(8), bh, smgr);
        map->setGridSize(10,5);


    BlockProperty p[7], water;
    for (int i=0;i<7;i++)
    {
        p[i] = bh->getProperties(0);
        p[i].drawable = true;
        p[i].solid = true;
        p[i].selectable = true;
        p[i].replaceable = false;
        calcTexCoords(p[i].top, i*0.125, 0);
        calcTexCoords(p[i].bottom, i*0.125, 0);
        calcTexCoords(p[i].side, i*0.125, 0);
        bh->setBlocktype(i+1, p[i]);
    }
    water = bh->getProperties(0);
    water.drawable = true;
    water.transparent = true;
    calcTexCoords(water.top, 0, 0);
    calcTexCoords(water.bottom, 0.125, 0);
    calcTexCoords(water.side, 0.125, 0);
    bh->setBlocktype(8, water);

    map->render();
    map->spawn(ZONE_WIDTH/2,ZONE_WIDTH/2);

    //device->getCursorControl()->setVisible(false);

    srand(0);


    /*core::dimension2du size( 800, 600 );
    IPostProc* ppRenderer = new CRendererPostProc( smgr, size, true, true, video::SColor(255,100,101,140) );
    CEffectPostProc* ppSaturation = new CEffectPostProc( ppRenderer, size, PP_SATURATE, 0.5f );
    ppSaturation->setQuality(PPQ_FAST);
    //ppSaturation->enable(false);
    CEffectPostProc* ppBloom = new CEffectPostProc( ppSaturation, size, PP_BLOOM );
    //ppBloom->enable(false);
    ppBloom->setQuality(PPQ_FAST);
    CLensFlarePostProc* ppLens = new CLensFlarePostProc( ppBloom, size, driver->getTexture("resources/flare.png"), sun, 50.0f );*/

    u32 frames=0;
    while(device->run())
    {
        driver->beginScene(true, true, video::SColor(0,100,100,100));

        //ppLens->render( NULL );
        smgr->drawAll();

        driver->endScene();
        if (++frames==10)
        {
            core::stringw str = L"Irrlicht Engine [";
            str += driver->getName();
            str += L"] FPS: ";
            str += (s32)driver->getFPS();
            str += L", Primitives: ";
            str += (s32)driver->getPrimitiveCountDrawn();

            device->setWindowCaption(str.c_str());
            frames=0;

            map->addToSelectedBlock( Block(rand()%7+2) );
            map->removeSelectedBlock();
        }

        device->yield();
    }

    device->drop();

    return 0;
}
