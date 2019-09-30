/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include "gg/thread.hpp"

namespace ui
{
	class UI;

	class MouseController : public gg::ITask
	{
	public:
		MouseController(UI*);
		virtual ~MouseController();
		virtual void onStart(gg::ITaskOptions&);
		virtual void onEvent(gg::ITaskOptions&, gg::EventPtr);
		virtual void onUpdate(gg::ITaskOptions&);

	private:
		UI* m_ui;
	};
};
