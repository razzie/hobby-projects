#include "mainmenu.h"

extern gui::IGUIFont* font;
extern video::ITexture* background;

MainMenu::MainMenu(IEventReceiver* recv, const BuildStruct& bs)
{
    this->recv = recv;
    this->bs = bs;
    can_continue = false;

    guienv->getSkin()->setFont(font);

    bg = guienv->addImage(background, core::position2d<s32>(0,0));
    bg->setScaleImage(true);

    goNbok[0] = guienv->addButton(core::rect<s32>(0,0,400,50), 0, BUTTON_NEW, L"New", L"Generate a new labyrinth");
    goNbok[1] = guienv->addButton(core::rect<s32>(0,0,400,50), 0, BUTTON_RETURN, L"Continue", L"Return to the game");
    goNbok[2] = guienv->addButton(core::rect<s32>(0,0,400,50), 0, BUTTON_CONFIG, L"Settings", L"Show or hide settings");
    goNbok[3] = guienv->addButton(core::rect<s32>(0,0,400,50), 0, BUTTON_EXIT, L"Exit", L"Exit this game");

    gui::IGUIComboBox *tmp;

    config[0] = guienv->addStaticText(L"Size:", core::rect<s32>(10,10,130,30));
    config[1] = guienv->addStaticText(L"Levels:", core::rect<s32>(140,10,260,30));
    config[2] = guienv->addStaticText(L"Wall type 1:", core::rect<s32>(290,10,410,30));
    config[3] = guienv->addStaticText(L"Wall type 2:", core::rect<s32>(420,10,540,30));
    config[4] = guienv->addStaticText(L"Wall type 3:", core::rect<s32>(550,10,670,30));
    tmp = guienv->addComboBox(core::rect<s32>(10,35,130,70));
        tmp->addItem(L"10x10",10);
        tmp->addItem(L"20x20",20);
        tmp->addItem(L"30x30",30);
        tmp->addItem(L"40x40",40);
        tmp->addItem(L"20x50",2050);
        config[5] = combo[0] = tmp;
    tmp = guienv->addComboBox(core::rect<s32>(140,35,280,70));
        tmp->addItem(L"Single level",1);
        tmp->addItem(L"Multilevel (2)",2);
        tmp->addItem(L"Multilevel (3)",3);
        tmp->addItem(L"Multilevel (4)",4);
        tmp->addItem(L"Multilevel (5)",5);
        config[6] = combo[1] = tmp;
    tmp = guienv->addComboBox(core::rect<s32>(290,35,410,70));
        tmp->addItem(L"Grey", 2);
        tmp->addItem(L"Yellow", 3);
        tmp->addItem(L"Brown", 4);
        tmp->addItem(L"Red", 5);
        config[7] = combo[2] = tmp;
    tmp = guienv->addComboBox(core::rect<s32>(420,35,540,70));
        tmp->addItem(L"Nothing", 0);
        tmp->addItem(L"Grey", 2);
        tmp->addItem(L"Yellow", 3);
        tmp->addItem(L"Brown", 4);
        tmp->addItem(L"Red", 5);
        config[8] = combo[3] = tmp;
    tmp = guienv->addComboBox(core::rect<s32>(550,35,670,70));
        tmp->addItem(L"Nothing", 0);
        tmp->addItem(L"Grey", 2);
        tmp->addItem(L"Yellow", 3);
        tmp->addItem(L"Brown", 4);
        tmp->addItem(L"Red", 5);
        config[9] = combo[4] = tmp;
    config[10] = check[0] = guienv->addCheckBox(true, core::rect<s32>(10,80,40,110));
    config[11] = check[1] = guienv->addCheckBox(true, core::rect<s32>(10,120,40,150));
    config[12] = check[2] = guienv->addCheckBox(true, core::rect<s32>(10,160,40,190));
    config[13] = guienv->addStaticText(L"Enable lights", core::rect<s32>(40,90,220,110));
    config[14] = guienv->addStaticText(L"Generate bush", core::rect<s32>(40,130,220,150));
    config[15] = guienv->addStaticText(L"Collision & gravity", core::rect<s32>(40,170,220,190));
    config[16] = check[3] = guienv->addCheckBox(true, core::rect<s32>(10,200,40,230));
    config[17] = guienv->addStaticText(L"Circle labyrinth", core::rect<s32>(40,210,220,230));
}

MainMenu::~MainMenu()
{
    int i;
    for (i=0;i<4;++i)
    {
        goNbok[i]->remove();
        goNbok[i]->drop();
        delete goNbok[i];
    }
    for (i=0;i<18;++i)
    {
        config[i]->remove();
        config[i]->drop();
        delete config[i];
    }
}

void MainMenu::resize()
{
    core::dimension2d<u32> scr = driver->getScreenSize();
    int Width2 = scr.Width / 2;
    int Height2 = scr.Height / 2;

    bg->setRelativePosition(core::rect<s32>(0,0,scr.Width,scr.Height));

    for (int i=0;i<4;++i)
        goNbok[i]->setRelativePosition(core::position2di(Width2-200,Height2-120+i*60));

    goNbok[1]->setEnabled(can_continue);
}

void MainMenu::start()
{
    int n = 0;
    quit = false;
    resize();

    device->setEventReceiver(this);
    device->getCursorControl()->setVisible(true);

    for(int i=0;i<18;++i) config[i]->setVisible(false);

    while(device->run() && !quit) {
        driver->beginScene(true, true, video::SColor(255,50,0,100));
        guienv->drawAll();
        driver->endScene();
        ++n;
        if(n>100){
            n -= 100;
            resize();
        }
    }

    device->setEventReceiver(recv);
    device->getCursorControl()->setVisible(false);
}

void MainMenu::die()
{
    can_continue = false;
    start();
}

bool MainMenu::OnEvent(const SEvent& event)
{
                if (event.EventType == EET_GUI_EVENT)
                {
                        s32 id = event.GUIEvent.Caller->getID();

                        switch(event.GUIEvent.EventType)
                        {
                        case gui::EGET_BUTTON_CLICKED:
                                switch(id)
                                {
                                case BUTTON_RETURN:
                                        if (can_continue) quit = true;
                                        return true;

                                case BUTTON_NEW:
                                        int size = combo[0]->getItemData( combo[0]->getSelected() );
                                        int levels = combo[1]->getItemData( combo[1]->getSelected() );
                                        int wall1 = combo[2]->getItemData( combo[2]->getSelected() );
                                        int wall2 = combo[3]->getItemData( combo[3]->getSelected() );
                                        int wall3 = combo[4]->getItemData( combo[4]->getSelected() );
                                        bool lights = check[0]->isChecked();
                                        bool bush = check[1]->isChecked();
                                        bool collision = check[2]->isChecked();
                                        bool circle = !check[3]->isChecked();
                                        GenerateNew(bs,size,levels,wall1,wall2,wall3,lights,bush,collision,circle);
                                        can_continue = true;
                                        quit = 1;
                                        return true;

                                case BUTTON_CONFIG:
                                        for(int i=0;i<18;++i) config[i]->setVisible(!config[i]->isVisible());
                                        return true;

                                case BUTTON_EXIT:
                                        device->closeDevice();
                                        return true;

                                default:
                                        return false;
                                }
                                break;

                        default:
                                break;
                        }
                }

                return false;
}
