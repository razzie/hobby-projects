#include "irrlicht.h"
#include "engine\zonehandler.h"
#include "XEffects.h"
#include <time.h>
#include <iostream>

using namespace irr;
using namespace Engine;

void calcTexCoords(core::vector2df* array, f32 x, f32 y, f32 size = 0.125f)
{
    array[3] = core::vector2df(x,y);
    array[2] = core::vector2df(x+size,y);
    array[1] = core::vector2df(x+size,y+size);
    array[0] = core::vector2df(x,y+size);
}

class SSAORenderCallback : public IPostProcessingRenderCallback
{
public:
	SSAORenderCallback(irr::s32 materialTypeIn) : materialType(materialTypeIn) {}

	void OnPreRender(EffectHandler* effect)
	{
		video::IVideoDriver* driver = effect->getIrrlichtDevice()->getVideoDriver();
		viewProj = driver->getTransform(video::ETS_PROJECTION) * driver->getTransform(video::ETS_VIEW);
		effect->setPostProcessingEffectConstant(materialType, "mViewProj", viewProj.pointer(), 16);
	}

	void OnPostRender(EffectHandler* effect) {}

	core::matrix4 viewProj;
	s32 materialType;
};

int main()
{
	std::cout   << "Please select the driver type:" << std::endl
                << "1 - OpenGL" << std::endl
                << "2 - Direct3D9" << std::endl;

	char C = '0';
	std::cin >> C;

	video::E_DRIVER_TYPE dType = video::EDT_DIRECT3D9;
	switch(C)
	{
        case '1':
            dType = video::EDT_OPENGL;
            break;
        case '2':
            dType = video::EDT_DIRECT3D9;
            break;
	}

    IrrlichtDevice *device = createDevice(dType, core::dimension2d<u32>(800, 600), 32, false, false);

    if (device == 0)
        return 1;

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);

    // XEffects
	EffectHandler* effect = new EffectHandler(device, core::dimension2du(0, 0), false, false, true);

	core::stringc shaderExt = (device->getVideoDriver()->getDriverType() == video::EDT_DIRECT3D9) ? ".hlsl" : ".glsl";
	//irr::s32 SSAO = effect->addPostProcessingEffectFromFile(core::stringc("shaders/SSAO") + shaderExt);
	irr::s32 BlurH = effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurHP") + shaderExt);
	irr::s32 BlurV = effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurVP") + shaderExt);
	irr::s32 BloomP = effect->addPostProcessingEffectFromFile(core::stringc("shaders/BloomP") + shaderExt);
	//irr::s32 BrightPass = effect->addPostProcessingEffectFromFile(core::stringc("shaders/BrightPass") + shaderExt);
	//irr::s32 ScreenWater = effect->addPostProcessingEffectFromFile(core::stringc("shaders/ScreenWater") + shaderExt);
	//irr::s32 SSAOCombine = effect->addPostProcessingEffectFromFile(core::stringc("shaders/SSAOCombine") + shaderExt);

	//SSAORenderCallback* ssaoCallback = new SSAORenderCallback(SSAO);
	//effect->setPostProcessingRenderCallback(SSAO, ssaoCallback);

	effect->setAmbientColor(video::SColor(255, 32, 32, 32));
	effect->setClearColour(video::SColor(255, 250, 100, 0));
	effect->enableDepthPass(true);

	SShadowLight data=SShadowLight(2048,core::vector3df(0,0,0),core::vector3df(0,0,0),video::SColor(255,255,255,200),0.1f,1300.0f,36000.0f*core::DEGTORAD64,true);
    effect->addShadowLight(data);

	video::ITexture* randVecTexture = effect->generateRandomVectorTexture(core::dimension2du(800, 600));
	effect->setPostProcessingUserTexture(randVecTexture);



    scene::ICameraSceneNode *cam = smgr->addCameraSceneNodeFPS(0, 50.0f, 0.02f);
        cam->setPosition(core::vector3df(0.0,128.0,0.0));
        //cam->setFOV(2.0);
    smgr->addSkyDomeSceneNode(driver->getTexture("skydome.bmp"));


    video::SMaterial material;
        material.setTexture(0, driver->getTexture("texture.bmp"));
        material.setFlag(video::EMF_BILINEAR_FILTER, false);
        material.Lighting = false;
        material.FogEnable = true;

    BlockHandler* bh = new BlockHandler(&material);
    ZoneHandler* map = new ZoneHandler(bh, smgr);
        map->setGridSize(20,6);

    effect->addNodeToDepthPass(map);
    effect->addShadowToNode(map);


    BlockProperty p[7];
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

    map->render();

    //device->getCursorControl()->setVisible(false);

    srand(0);


    u32 frames=0;
    while(device->run())
    {
        driver->beginScene(true, true, video::SColor(0,100,100,100));

        //smgr->drawAll();

        core::vector3df pos, targ = smgr->getActiveCamera()->getPosition();
        pos.Y = 128.0;
        targ.Y = 0.0;
        //core::vector3df vec =  smgr->getActiveCamera()->getTarget() - smgr->getActiveCamera()->getPosition();
        effect->getShadowLight(0).setPosition(pos);
        effect->getShadowLight(0).setTarget(targ);
        //effect->getShadowLight(0).setPosition(smgr->getActiveCamera()->getPosition());
        //effect->getShadowLight(0).setTarget(smgr->getActiveCamera()->getTarget());
        effect->update();

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

            map->addToSelectedBlock( Block(rand()%7+1) );
            //map->removeSelectedBlock();
        }

        device->yield();
    }

    device->drop();

    return 0;
}
