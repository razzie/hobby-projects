/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "gg/logger.hpp"
#include "irrlicht.h"
#include "game.hpp"
#include "ui/ui.hpp"
#include "ui/factory.hpp"
#include "ui/events.hpp"
#include "ui/shadercallback.hpp"
//#include "ui/ground.hpp"
//#include "ui/events.hpp"
#include "ui/scene/mousecontroller.hpp"
#include "ui/scene/scene.hpp"

using namespace irr;


static void takeScreenshot(irr::IrrlichtDevice* device)
{
	irr::video::IVideoDriver* const driver = device->getVideoDriver();

	//get image from the last rendered frame 
	irr::video::IImage* const image = driver->createScreenShot();
	if (image) //should always be true, but you never know. ;) 
	{
		//construct a filename, consisting of local time and file extension 
		irr::c8 filename[64];
		snprintf(filename, 64, "screenshot_%u.png", device->getTimer()->getRealTime());

		//write screenshot to file 
		if (!driver->writeImageToFile(image, filename))
			device->getLogger()->log(L"Failed to take screenshot.", irr::ELL_WARNING);

		//Don't forget to drop image since we don't need it anymore. 
		image->drop();
	}
}


ui::Scene::Scene(UI* ui) :
	m_ui(ui),
	m_device(m_ui->getDevice()),
	m_driver(m_ui->getDriver()),
	m_smgr(m_ui->getSceneManager()),
	m_cam(m_ui->getCamera())
{
}

ui::Scene::~Scene()
{
}

void ui::Scene::onStart(gg::ITaskOptions& options)
{
	options.subscribe(key_event);
}

void ui::Scene::onEvent(gg::ITaskOptions& options, gg::EventPtr e)
{
	if (e->is(key_event))
	{
		const KeyInput* event;
		e->get(event);

		if (!event->consumed)
		{
			event->consumed = true;

			EKEY_CODE key = static_cast<EKEY_CODE>(event->key_code);
			switch (key)
			{
			case EKEY_CODE::KEY_ESCAPE:
				options.getThread().finish();
				break;

			case EKEY_CODE::KEY_KEY_R:
				break;

			case EKEY_CODE::KEY_SPACE:
				m_cam->setPosition({ 0.f, 50.f, -50.f });
				m_cam->setTarget({ 0.f, 0.f, 0.f });
				break;

			case EKEY_CODE::KEY_F12:
				takeScreenshot(m_device);
				break;
			}
		}
	}
}

void ui::Scene::onUpdate(gg::ITaskOptions& options)
{
	if (m_device->run())
	{
		//m_driver->beginScene(true, true, { 255, 128, 196, 255 });
		m_driver->beginScene(true, true, { 255, 196, 196, 196 });
		m_smgr->drawAll();

		/*m_driver->setMaterial(m_ui->getFactory()->getLineMaterial());
		m_driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
		m_driver->draw3DLine({ 0.f,0.f,0.f }, { 1.f,0.f,0.f }, 0xffff0000);
		m_driver->draw3DLine({ 0.f,0.f,0.f }, { 0.f,1.f,0.f }, 0xff00ff00);
		m_driver->draw3DLine({ 0.f,0.f,0.f }, { 0.f,0.f,1.f }, 0xff0000ff);*/

		m_driver->endScene();
	}
	else
	{
		options.getThread().finish();
	}
}

void ui::Scene::onStateChange(gg::ITaskOptions& options, gg::IThread::State old_state, gg::IThread::State new_state)
{
}

void ui::Scene::onError(gg::ITaskOptions& options, std::exception& e)
{
	gg::log << e.what() << std::endl;
}

void ui::Scene::onFinish(gg::ITaskOptions& options)
{
}
