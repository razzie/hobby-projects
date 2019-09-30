/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include "irrlicht.h"

class ShaderCallback : public irr::video::IShaderConstantSetCallBack
{
public:
	ShaderCallback(irr::scene::ISceneManager* smgr) : m_smgr(smgr)
	{
	}

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		irr::video::IVideoDriver* driver = services->getVideoDriver();

		irr::core::matrix4 world = driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("world", world.pointer(), 16);

		irr::core::matrix4 world_view_proj;
		world_view_proj = driver->getTransform(irr::video::ETS_PROJECTION);
		world_view_proj *= driver->getTransform(irr::video::ETS_VIEW);
		world_view_proj *= driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("world_view_proj", world_view_proj.pointer(), 16);

		//core::vector3df light_pos(50.f, 50.f, 0.f);
		irr::core::vector3df light_pos = m_smgr->getActiveCamera()->getPosition();
		services->setVertexShaderConstant("light_pos", reinterpret_cast<irr::f32*>(&light_pos), 3);

		irr::video::SColorf light_color(1.f, 1.f, 1.f);
		services->setVertexShaderConstant("light_color", reinterpret_cast<irr::f32*>(&light_color), 4);
	}

private:
	irr::scene::ISceneManager* m_smgr;
};
