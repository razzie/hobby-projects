/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <cstdint>
#include <memory>
#include "gg/event.hpp"

namespace irr
{
	namespace gui
	{
		class IGUIElement;
	};
};

namespace ui
{
	struct KeyInput;
	struct KeyInput : public gg::IEvent::Tag<0, KeyInput>
	{
		wchar_t character;
		uint16_t key_code; // irr::EKEY_CODE
		bool pressed_down : 1;
		bool shift : 1;
		bool control : 1;
		mutable volatile bool consumed : 1;
	};

	struct MouseInput;
	struct MouseInput : public gg::IEvent::Tag<0, MouseInput>
	{
		int32_t x;
		int32_t y;
		int32_t x_delta;
		int32_t y_delta;
		float wheel;
		bool shift : 1;
		bool control : 1;
		bool left_button : 1;
		bool right_button : 1;
		bool middle_button : 1;
		mutable volatile bool consumed : 1;
	};

	struct GUIAction;
	struct GUIAction : public gg::IEvent::Tag<0, GUIAction>
	{
		std::shared_ptr<irr::gui::IGUIElement> caller;
		std::shared_ptr<irr::gui::IGUIElement> element;
		uint16_t action;
	};

	extern gg::IEventDefinition<KeyInput>& key_event;
	extern gg::IEventDefinition<MouseInput>& mouse_event;
	extern gg::IEventDefinition<GUIAction>& gui_event;
};
