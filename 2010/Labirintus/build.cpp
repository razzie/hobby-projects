#include "build.h"

using namespace std;
using namespace irr;

extern IrrlichtDevice *device;
extern video::IVideoDriver* driver;
extern scene::ISceneManager* smgr;
extern scene::ICameraSceneNode *cam;
scene::IMetaTriangleSelector* metaselector;

void CollectSelector(scene::IMeshSceneNode *node) {
    scene::ITriangleSelector *selector = smgr->createTriangleSelector(node->getMesh(), node);
    node->setTriangleSelector(selector);
    metaselector->addTriangleSelector(selector);
    selector->drop();
}

scene::IMeshSceneNode* BuildNode(const WallStruct& ws, bool light) {
    scene::IMesh* tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(ws.mesh->getMesh(0));
    scene::IMeshSceneNode* node = smgr->addMeshSceneNode( tangentMesh );
    tangentMesh->drop();
    node->setMaterialTexture( 0, ws.texture );
    if (ws.bumpmap) {
        node->setMaterialTexture(1, ws.bumpmap);
        node->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
    }
    CollectSelector(node);
    if (!light) node->setMaterialFlag(video::EMF_LIGHTING, false);
    return node;
}

scene::IMeshSceneNode* BuildWithAlpha(const WallStruct& ws, bool light) {
    scene::IMeshSceneNode* node = smgr->addMeshSceneNode( ws.mesh->getMesh(0) );
    node->setMaterialTexture( 0, ws.texture );
    node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    CollectSelector(node);
    if (!light) node->setMaterialFlag(video::EMF_LIGHTING, false);
    return node;
}

void Build(const LStruct &l, const BuildStruct& bs, bool light, int level, bool ceiling) {
    scene::IMeshSceneNode* node;
    int i,j,wall_type,floor_type;

    //padló
    for (i=0;i<l.SOR;i++) {
        for (j=0;j<l.OSZLOP;j++) {
            floor_type = l.LP[i][j] - 1;
            if (floor_type >=0 && floor_type < bs.floor_count) {
                node = BuildNode( (WallStruct)(bs.floor_list[floor_type]) , light );
                node->setPosition(core::vector3df(i*100+100,level*51,-j*100-100));
                if (ceiling) {
                    node = BuildNode( (WallStruct)(bs.floor_list[floor_type]) , light );
                    node->setPosition(core::vector3df(i*100+100,level*51+50,-j*100-100));
                    node->setRotation(core::vector3df(180,0,0));
                }
            }
        }
    }

    // vízszintes falak
    for (i=0;i<=l.SOR;i++) {
        for (j=0;j<=l.OSZLOP;j++) {
            wall_type = l.LV[i][j] - 1; // a falak ID-je 1 bázisú (0 = nincs fal), viszont a falak tömbje 0 bázisú
            if (wall_type >= 0 && wall_type < bs.wall_count) { // biztonsági okokból a segmentation fault megelõzésére
                if (wall_type == 0)
                    node = BuildWithAlpha( bs.wall_list[wall_type] , light );
                else
                    node = BuildNode( bs.wall_list[wall_type] , light );
                node->setPosition(core::vector3df(i*100+50,level*51,-j*100));
                node->setRotation(core::vector3df(0,90,0));
            }
        }
    }

    // függõleges falak
    for (i=0;i<=l.SOR;i++) {
        for (j=0;j<=l.OSZLOP;j++) {
            wall_type = l.LF[i][j] - 1;
            if (wall_type >= 0 && wall_type < bs.wall_count) {
                if (wall_type == 0)
                    node = BuildWithAlpha( bs.wall_list[wall_type] , light );
                else
                    node = BuildNode( bs.wall_list[wall_type] , light );
                node->setPosition(core::vector3df(i*100,level*51,-j*100-50));
            }
        }
    }
}

core::vector3df LabyCenter(const LStruct& ls, int level = 0) {
    return core::vector3df( (ls.SOR)*50 , 30+level*51 , -(ls.OSZLOP)*50 );
}

void GenerateNew(const BuildStruct& bs, int size, int levels, int wall1, int wall2, int wall3, bool lights, bool bush, bool collision, bool circle) {
    scene::ISceneManager* smgr2 = smgr->createNewSceneManager();
    if (smgr != device->getSceneManager()) smgr->drop();
    smgr = smgr2;
    device->setInputReceivingSceneManager(smgr);

    smgr->addSkyDomeSceneNode(driver->getTexture("skydome.jpg"),16,8,0.95f,2.0f);

    cam = smgr->addCameraSceneNodeFPS(); // fontos hogy a kamera hamarabb létre legyen hozva mint a labirintus
    scene::ILightSceneNode *light = smgr->addLightSceneNode(cam,core::vector3df(0,0,0),video::SColorf(1.0f, 1.0f, 1.0f, 0.0f), 300.0f);
    light->addAnimator( smgr->createFlyCircleAnimator(core::vector3df(0,0,0), 3.0f, 0.003f) );

    Labyrinth* labi;
    int size1, size2;

    metaselector = smgr->createMetaTriangleSelector();

    for(int n=0;n<levels;++n) {
        switch(size) {
            case 10:
                size1 = size2 = 10;
                break;
            case 20:
                size1 = size2 = 20;
                break;
            case 30:
                size1 = size2 = 30;
                break;
            case 40:
                size1 = size2 = 40;
                break;
            case 2050:
                size1 = 20;
                size2 = 50;
                break;
        }
        labi = new Labyrinth(size1,size2,(n==(levels-1)&&bush ? 1 : wall1),wall2,wall3,circle);
        if (n==levels-1) cam->setPosition( LabyCenter(labi->get(),n) );
        if (n>0) labi->makeHoles();
        Build(labi->get(),bs,lights,n,(n!=levels-1));
        delete labi;
    }

    if (collision) {
        scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(metaselector, cam, core::vector3df(15,15,15), core::vector3df(0,-50,0), core::vector3df(0,10,0));
        cam->addAnimator(anim);
        anim->drop();
    }
    metaselector->drop();
}
