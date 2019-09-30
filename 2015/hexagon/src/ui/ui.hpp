/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <cstdint>
#include "gg/thread.hpp"

class Game;

namespace irr
{
	class IrrlichtDevice;

	namespace video
	{
		class IVideoDriver;
	};

	namespace scene
	{
		class ISceneManager;
		class ICameraSceneNode;
	};
};

template<class T>
std::shared_ptr<T> makeIrrPtr(T* p)
{
	p->grab();
	return std::shared_ptr<T>(p, [](T* p) { p->drop(); });
}

namespace ui
{
	class Factory;

	class UI
	{
	public:
		enum State : uint16_t
		{
			MENU,
			GAME
		};

		UI(Game*);
		~UI();
		Game* getGame();
		void start(gg::IThread::Mode);
		void stop();
		State getState() const;
		void setState(State);
		gg::ThreadPtr getThread();
		irr::IrrlichtDevice* getDevice();
		irr::video::IVideoDriver* getDriver();
		irr::scene::ISceneManager* getSceneManager();
		irr::scene::ICameraSceneNode* getCamera();
		Factory* getFactory();

	private:
		Game* m_game;
		gg::ThreadPtr m_thread;
		irr::IrrlichtDevice* m_device;
		irr::video::IVideoDriver* m_driver;
		irr::scene::ISceneManager* m_smgr;
		irr::scene::ICameraSceneNode* m_cam;
		Factory* m_factory;
	};
};
