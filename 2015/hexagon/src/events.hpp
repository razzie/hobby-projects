/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <cstdint>
#include "gg/event.hpp"

const gg::IEvent::Type INTERNAL_EVENTS_BASE = 0;
const gg::IEvent::Type BACKEND_EVENTS_BASE = 100;
const gg::IEvent::Type NETWORK_EVENTS_BASE = 200;
const gg::IEvent::Type UI_EVENTS_BASE = 300;
