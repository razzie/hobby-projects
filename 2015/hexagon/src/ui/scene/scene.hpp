/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include "gg/thread.hpp"
#include "ui/ui.hpp"

namespace ui
{
	class Scene : public gg::ITask
	{
	public:
		Scene(UI*);
		virtual ~Scene();
		virtual void onStart(gg::ITaskOptions&);
		virtual void onEvent(gg::ITaskOptions&, gg::EventPtr);
		virtual void onUpdate(gg::ITaskOptions&);
		virtual void onStateChange(gg::ITaskOptions&, gg::IThread::State old_state, gg::IThread::State new_state);
		virtual void onError(gg::ITaskOptions&, std::exception&);
		virtual void onFinish(gg::ITaskOptions&);

	private:
		UI* m_ui;
		irr::IrrlichtDevice* m_device;
		irr::video::IVideoDriver* m_driver;
		irr::scene::ISceneManager* m_smgr;
		irr::scene::ICameraSceneNode* m_cam;
	};
};
