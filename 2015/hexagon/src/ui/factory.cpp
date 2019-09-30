/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "ui/shadercallback.hpp"
#include "ui/ui.hpp"
#include "ui/factory.hpp"

ui::Factory::Factory(UI* ui) :
	m_ui(ui)
{
	irr::video::IGPUProgrammingServices* gpu = ui->getSceneManager()->getVideoDriver()->getGPUProgrammingServices();
	ShaderCallback* shader_cb;
	irr::s32 mat_type;

	// initialize line material
	m_line_mat.ZBuffer = false;
	m_line_mat.Lighting = false;
	m_line_mat.Thickness = 2.f;

	// initialize flat shaded material
	shader_cb = new ShaderCallback(ui->getSceneManager());
	mat_type = gpu->addHighLevelShaderMaterialFromFiles(
		"../asset/shader/flat.vert", "main", irr::video::EVST_VS_1_1,
		"../asset/shader/flat.frag", "main", irr::video::EPST_PS_1_1,
		shader_cb);
	m_flat_mat.MaterialType = (irr::video::E_MATERIAL_TYPE)mat_type;

	// initialize surface material
	shader_cb = new ShaderCallback(ui->getSceneManager());
	mat_type = gpu->addHighLevelShaderMaterialFromFiles(
		"../asset/shader/surface.vert", "main", irr::video::EVST_VS_1_1,
		"../asset/shader/surface.frag", "main", irr::video::EPST_PS_1_1,
		shader_cb);
	m_surface_mat.MaterialType = (irr::video::E_MATERIAL_TYPE)mat_type;
}

ui::Factory::~Factory()
{
}

const irr::video::SMaterial& ui::Factory::getLineMaterial()
{
	return m_line_mat;
}

const irr::video::SMaterial& ui::Factory::getSurfaceMaterial()
{
	return m_surface_mat;
}
