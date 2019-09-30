/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "gg/thread.hpp"
#include "../events.hpp"
#include "ui/events.hpp"

static gg::LocalEventDefinition<UI_EVENTS_BASE + 1, ui::KeyInput> s_key_event;
gg::IEventDefinition<ui::KeyInput>& ui::key_event = s_key_event;

static gg::LocalEventDefinition<UI_EVENTS_BASE + 2, ui::MouseInput> s_mouse_event;
gg::IEventDefinition<ui::MouseInput>& ui::mouse_event = s_mouse_event;

static gg::LocalEventDefinition<UI_EVENTS_BASE + 3, ui::GUIAction> s_gui_event;
gg::IEventDefinition<ui::GUIAction>& ui::gui_event = s_gui_event;
