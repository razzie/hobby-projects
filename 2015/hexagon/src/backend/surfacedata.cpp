/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "backend/surfacedata.hpp"

#define SCALE 0.125f


backend::SurfaceData::SurfaceData(unsigned seed, unsigned width, unsigned height, unsigned depth) :
	m_noise(seed),
	m_seed(seed),
	m_width(width),
	m_height(height),
	m_depth(depth),
	m_center_x(0.5f * (float)width),
	m_center_y(0.75f * (float)height),
	m_center_z(0.5f * (float)depth)
{
}

backend::SurfaceData::~SurfaceData()
{
}

bool backend::SurfaceData::get(unsigned _x, unsigned _y, unsigned _z) const
{
	if (_x < m_width &&
		_y < m_height &&
		_z < m_depth)
	{
		float x = (float)_x;
		float y = (float)_y;
		float z = (float)_z;
		float v = m_noise(SCALE * x, SCALE * y, SCALE * z);
		float distx = abs(x - m_center_x) / m_center_x;
		float disty = abs(y - m_center_y) / m_center_y;
		float distz = abs(z - m_center_z) / m_center_z;
		float avg = (distx + disty + distz) / 3.0f;
		avg *= 2.f;
		avg -= .5f;
		v -= avg;
		return (v > 0.f);
	}
	else
	{
		return false;
	}
}

GridPtr backend::SurfaceData::genGrid() const
{
	return genGrid(0, 0, 0, m_width, m_height, m_depth);
}

GridPtr backend::SurfaceData::genGrid(unsigned pos_x, unsigned pos_y, unsigned pos_z, unsigned width, unsigned height, unsigned depth) const
{
	GridPtr grid(new Grid(width, height, depth));

	for (unsigned x = 0; x < width; ++x)
	{
		for (unsigned y = 0; y < height; ++y)
		{
			for (unsigned z = 0; z < depth; ++z)
			{
				grid->set(x, y, z, get(pos_x + x, pos_y + y, pos_z + z));
			}
		}
	}

	return grid;
}
