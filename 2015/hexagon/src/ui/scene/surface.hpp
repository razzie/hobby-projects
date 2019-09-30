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
	class UI;

	class Surface : public irr::scene::ISceneNode
	{
	public:
		Surface(UI* ui, GridPtr grid);
		virtual ~Surface();
		virtual void OnRegisterSceneNode();
		virtual void render();
		virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
		virtual irr::u32 getMaterialCount() const;
		virtual irr::video::SMaterial& getMaterial(irr::u32 i);

	private:
		GridPtr m_grid;
		irr::video::SMaterial m_material;
		irr::scene::CVertexBuffer m_vertices;
		irr::scene::CIndexBuffer m_indices;
		irr::scene::CDynamicMeshBuffer m_meshbuffer;
	};
};
