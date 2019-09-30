/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include "irrlicht.h"

namespace ui
{
	class UI;

	class Factory
	{
	public:
		Factory(UI* ui);
		~Factory();
		const irr::video::SMaterial& getLineMaterial();
		const irr::video::SMaterial& getFlatShadedMaterial();
		const irr::video::SMaterial& getSurfaceMaterial();

	private:
		UI* m_ui;
		irr::video::SMaterial m_line_mat;
		irr::video::SMaterial m_flat_mat;
		irr::video::SMaterial m_surface_mat;
	};
};
