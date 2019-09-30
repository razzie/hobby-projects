/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include "irrlicht.h"
#include "util/grid.hpp"

namespace ui
{
	struct Hexagon
	{
		struct SideInfo
		{
			// top & bottom
			int top : 1;
			int bottom : 1;
			// sides
			int up : 1;
			int down : 1;
			int left1 : 1;
			int left2 : 1;
			int right1 : 1;
			int right2 : 1;
		};

		static void add(
			SideInfo,
			irr::scene::IDynamicMeshBuffer*,
			const irr::core::vector3df&,
			irr::video::SColor = 0xffffffff);

		static void build(
			const Grid&,
			irr::scene::IDynamicMeshBuffer*,
			const irr::core::vector3df&,
			irr::video::SColor = 0xffffffff);
	};
};
