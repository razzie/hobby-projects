/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include "irrlicht.h"
#include "ui/shadercallback.hpp"

class Ground : public irr::scene::ISceneNode
{
private:
	irr::video::SMaterial m_material;
	irr::scene::CVertexBuffer m_vertices;
	irr::scene::CIndexBuffer m_indices;
	irr::scene::CDynamicMeshBuffer m_meshbuffer;

public:
	Ground(irr::scene::ISceneManager* smgr) :
		irr::scene::ISceneNode(smgr->getRootSceneNode(), smgr),
		m_vertices(irr::video::EVT_STANDARD),
		m_indices(irr::video::EIT_16BIT),
		m_meshbuffer(irr::video::EVT_STANDARD, irr::video::EIT_16BIT)
	{
		irr::video::IGPUProgrammingServices* gpu = getSceneManager()->getVideoDriver()->getGPUProgrammingServices();
		ShaderCallback* shader_cb = new ShaderCallback(getSceneManager());
		irr::s32 mat_type = gpu->addHighLevelShaderMaterialFromFiles(
			"../asset/shader/flat.vert", "main", irr::video::EVST_VS_1_1,
			"../asset/shader/flat.frag", "main", irr::video::EPST_PS_1_1,
			shader_cb);
		m_material.MaterialType = (irr::video::E_MATERIAL_TYPE)mat_type;

		//m_material.Lighting = false;
		//m_material.GouraudShading = false;
		//m_material.BackfaceCulling = false;
		//m_material.NormalizeNormals = true;

		m_meshbuffer.setVertexBuffer(&m_vertices);
		m_meshbuffer.setIndexBuffer(&m_indices);
	}

	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	virtual void render()
	{
		irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
		driver->setMaterial(m_material);
		driver->drawMeshBuffer(&m_meshbuffer);
	}

	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const
	{
		return m_meshbuffer.getBoundingBox();
	}

	virtual irr::u32 getMaterialCount() const
	{
		return 1;
	}

	virtual irr::video::SMaterial& getMaterial(irr::u32 i)
	{
		return m_material;
	}
};
