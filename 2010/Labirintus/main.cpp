#include <iostream>
#include "irrlicht.h"
#include "labyrinth.h"
#include "build.h"
#include "mainmenu.h"

#define WALL_COUNT 5
#define FLOOR_COUNT 10

using namespace std;
using namespace irr;

IrrlichtDevice *device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
gui::IGUIEnvironment* guienv;
scene::ICameraSceneNode *cam;

gui::IGUIFont* font;
video::ITexture* background;

class MenuListener : public IEventReceiver {
private:
    MainMenu* menu;

public:
    MenuListener(MainMenu* menu)
    {
        this->menu = menu;
    }

    bool OnEvent(const SEvent& event)
    {
        if (event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_ESCAPE)
        {
            menu->start();
            return true;
        }
        smgr->postEventFromUser(event);
    }
};

int main()
{
    device = createDevice( video::EDT_DIRECT3D9, core::dimension2d<u32>(800, 600), 32, false, false, false, 0);
    device->setWindowCaption(L"Labirintus");
    device->getFileSystem()->addFolderFileArchive("../texturak/");
    device->getFileSystem()->addFolderFileArchive("../falak/");
    device->getFileSystem()->addFolderFileArchive("../padlok/");
    device->setResizable(true);

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    font = guienv->getFont("font.png");
    background = driver->getTexture("background.jpg");

    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

    //töNbök
    scene::IAnimatedMesh *fal, *bokor, *padlo, *lyuk;

    fal = smgr->getMesh("falak.3DS");
    bokor = smgr->getMesh("bokor.3DS");
    padlo = smgr->getMesh("padlo.3DS");
    lyuk = smgr->getMesh("lyuk.3DS");

    video::ITexture *t1, *t2, *t3, *t4;
    video::ITexture *t1_hmap, *t2_hmap, *t3_hmap, *t4_hmap;
    video::ITexture *b;
    video::ITexture *p1, *p2, *p3, *p4;
    video::ITexture *p1_hmap, *p2_hmap, *p3_hmap, *p4_hmap;

    t1 = driver->getTexture("t1.jpg");
        t1_hmap = driver->getTexture("t1_hmap.bmp"); driver->makeNormalMapTexture(t1_hmap, 10.0f);
    t2 = driver->getTexture("t2.jpg");
        t2_hmap = driver->getTexture("t2_hmap.bmp"); driver->makeNormalMapTexture(t2_hmap, 10.0f);
    t3 = driver->getTexture("t3.jpg");
        t3_hmap = driver->getTexture("t3_hmap.bmp"); driver->makeNormalMapTexture(t3_hmap, 10.0f);
    t4 = driver->getTexture("t4.jpg");
        t4_hmap = driver->getTexture("t4_hmap.bmp"); driver->makeNormalMapTexture(t4_hmap, 10.0f);
    b = driver->getTexture("level.tga");
    p1 = driver->getTexture("p1.jpg");
        p1_hmap = driver->getTexture("p1_hmap.bmp"); driver->makeNormalMapTexture(p1_hmap, 5.0f);
    p2 = driver->getTexture("p2.jpg");
        p2_hmap = driver->getTexture("p2_hmap.bmp"); driver->makeNormalMapTexture(p2_hmap, 5.0f);
    p3 = driver->getTexture("p3.jpg");
        p3_hmap = driver->getTexture("p3_hmap.bmp"); driver->makeNormalMapTexture(p3_hmap, 5.0f);
    p4 = driver->getTexture("p4.jpg");
        p4_hmap = driver->getTexture("p4_hmap.bmp"); driver->makeNormalMapTexture(p4_hmap, 5.0f);

    WallStruct wall_list[WALL_COUNT] = { {bokor,b,NULL} , {fal,t1,t1_hmap} , {fal,t2,t2_hmap} , {fal,t3,t3_hmap} , {fal,t4,t4_hmap} };
    FloorStruct floor_list[FLOOR_COUNT] = { {padlo,p2,p2_hmap} , {padlo,p1,p1_hmap} , {padlo,p2,p2_hmap} , {padlo,p3,p3_hmap} , {padlo,p4,p4_hmap} , {lyuk,p3,p3_hmap} , {lyuk,p1,p1_hmap} , {lyuk,p2,p2_hmap} , {lyuk,p3,p3_hmap} , {lyuk,p4,p4_hmap} };
    BuildStruct bs = {wall_list,WALL_COUNT,floor_list,FLOOR_COUNT};


    MainMenu* menu = new MainMenu(NULL, bs);
    menu->recv = new MenuListener(menu);
    menu->start();

    while(device->run()) {
        driver->beginScene(true, true, video::SColor(255,100,101,140));
        smgr->drawAll();
        driver->endScene();
        if (cam->getPosition().Y < -500) menu->die();
    }
    device->drop();

    return 0;
}
