#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include <irrlicht.h>
#include "build.h"

using namespace irr;

extern IrrlichtDevice *device;
extern video::IVideoDriver* driver;
extern gui::IGUIEnvironment* guienv;

class MainMenu : public IEventReceiver {
private:
    bool quit;
    bool can_continue;
    gui::IGUIImage* bg;
    gui::IGUIButton* goNbok[4];
    gui::IGUIElement* config[18];
    gui::IGUIComboBox* combo[5];
    gui::IGUICheckBox* check[4];
    enum { BUTTON_RETURN, BUTTON_NEW, BUTTON_CONFIG, BUTTON_EXIT };
    BuildStruct bs;
    void resize();
public:
    IEventReceiver* recv;
    MainMenu(IEventReceiver* recv, const BuildStruct& bs);
    ~MainMenu();
    void start();
    void die();
    bool OnEvent(const SEvent& event);
};

#endif
