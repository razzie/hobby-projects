/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "irrlicht.h"
#include "game.hpp"
#include "shadercallback.hpp"
#include "ui/scene/mousecontroller.hpp"
#include "ui/scene/scene.hpp"
#include "ui/scene/surface.hpp"
#include "ui/eventreceiver.hpp"
#include "ui/factory.hpp"
#include "ui/ui.hpp"
#include "backend/surfacedata.hpp"

using namespace irr;


ui::UI::UI(Game* game) :
	m_game(game),
	m_thread(gg::threadmgr.createThread("UI"))
{
	SIrrlichtCreationParameters params; // constructor sets the defaults
	params.AntiAlias = 16;
	params.Bits = 32;
	//params.DriverType = video::EDT_DIRECT3D9;
	params.DriverType = video::EDT_OPENGL;
	params.Fullscreen = game->getParameters().fullscreen;
	params.WindowSize = core::dimension2du(game->getParameters().screen_width, game->getParameters().screen_hight);
	params.WithAlphaChannel = true;
	params.Vsync = true;

	m_device = createDeviceEx(params);
	m_driver = m_device->getVideoDriver();
	m_smgr = m_device->getSceneManager();
	m_factory = new Factory(this);

	m_device->setEventReceiver(new EventReceiver(this));
	m_device->setResizable(true);
	m_device->setWindowCaption(L"Hexagon");

	m_cam = m_smgr->addCameraSceneNode(0, core::vector3df(0.f, 10.f, -10.f), core::vector3df(0.f, 0.f, 0.f));
	//m_smgr->addLightSceneNode(0, { 0.f, 30.f, 30.f }, { 255, 255, 255, 255 }, 0.5f);
	//m_smgr->addCubeSceneNode();
	backend::SurfaceData surfacedata(1111, 64, 128, 64);
	Surface* surface = new Surface(this, surfacedata.genGrid());

	core::plane3df plane(core::vector3df(0.f, 0.f, 0.f), core::vector3df(0.f, 1.f, 0.f));
	video::SMaterial line_material;
	line_material.Lighting = false;
	line_material.Thickness = 2.f;
}

ui::UI::~UI()
{
	m_cam->drop();
	//m_device->drop();

	delete m_factory;
}

Game* ui::UI::getGame()
{
	return m_game;
}

void ui::UI::start(gg::IThread::Mode mode)
{
	m_thread->setState(State::GAME);
	m_thread->addTask<MouseController, State::GAME>(this);
	m_thread->addTask<Scene, State::GAME>(this);
	m_thread->run(mode);
}

void ui::UI::stop()
{
	m_thread->finish();
	m_thread->join();
}

ui::UI::State ui::UI::getState() const
{
	return State(m_thread->getState());
}

void ui::UI::setState(State state)
{
	m_thread->setState(gg::IThread::State(state));
}

gg::ThreadPtr ui::UI::getThread()
{
	return m_thread;
}

irr::IrrlichtDevice* ui::UI::getDevice()
{
	return m_device;
}

irr::video::IVideoDriver* ui::UI::getDriver()
{
	return m_driver;
}

irr::scene::ISceneManager* ui::UI::getSceneManager()
{
	return m_smgr;
}

irr::scene::ICameraSceneNode* ui::UI::getCamera()
{
	return m_cam;
}

ui::Factory* ui::UI::getFactory()
{
	return m_factory;
}
