/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <cstdint>
#include "irrlicht.h"

namespace gg { class IThread; };
namespace irr { struct SEvent; };
namespace ui { class UI; };

namespace ui
{
	class EventReceiver : public irr::IEventReceiver
	{
	public:
		EventReceiver(UI*);
		virtual ~EventReceiver();
		virtual bool OnEvent(const irr::SEvent& event);

	private:
		UI* m_ui;
		int32_t m_last_mouse_x;
		int32_t m_last_mouse_y;
	};
};
