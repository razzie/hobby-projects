/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "util/grid.hpp"
#include "ui/scene/hexagon.hpp"
#include "ui/scene/surface.hpp"
#include "ui/ui.hpp"
#include "ui/factory.hpp"


ui::Surface::Surface(UI* ui, GridPtr grid) :
	irr::scene::ISceneNode(ui->getSceneManager()->getRootSceneNode(), ui->getSceneManager()),
	m_grid(grid),
	m_vertices(irr::video::EVT_STANDARD),
	m_indices(irr::video::EIT_32BIT),
	m_meshbuffer(irr::video::EVT_STANDARD, irr::video::EIT_32BIT)
{
	irr::core::vector3df center(0.5f * (float)grid->getWidth(), 0.5f * (float)grid->getHeight(), 0.5f * (float)grid->getDepth());

	m_meshbuffer.setVertexBuffer(&m_vertices);
	m_meshbuffer.setIndexBuffer(&m_indices);

	Hexagon::build(*m_grid, &m_meshbuffer, -center);

	m_meshbuffer.recalculateBoundingBox();
	m_meshbuffer.setHardwareMappingHint(irr::scene::E_HARDWARE_MAPPING::EHM_STATIC);
	m_meshbuffer.setDirty();

	m_material = ui->getFactory()->getSurfaceMaterial();

	RelativeScale.set({ 1.f, 0.5f, 1.f });
}

ui::Surface::~Surface()
{
}

void ui::Surface::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void ui::Surface::render()
{
	irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
	driver->setMaterial(m_material);
	driver->drawMeshBuffer(&m_meshbuffer);
}

const irr::core::aabbox3d<irr::f32>& ui::Surface::getBoundingBox() const
{
	return m_meshbuffer.getBoundingBox();
}

irr::u32 ui::Surface::getMaterialCount() const
{
	return 1;
}

irr::video::SMaterial& ui::Surface::getMaterial(irr::u32 i)
{
	return m_material;
}
