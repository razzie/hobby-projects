/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "util/grid.hpp"


Grid::Grid(unsigned width, unsigned height, unsigned depth) :
	m_width(width),
	m_height(height),
	m_depth(depth),
	m_data((width * height * depth) / 8 + 1)
{
}

Grid::~Grid()
{
}

unsigned Grid::getWidth() const
{
	return m_width;
}

unsigned Grid::getHeight() const
{
	return m_height;
}

unsigned Grid::getDepth() const
{
	return m_depth;
}

bool Grid::get(int x, int y, int z) const
{
	if ((unsigned)x < m_width &&
		(unsigned)y < m_height &&
		(unsigned)z < m_depth)
	{
		auto at = (z * m_width * m_height) + (y * m_width) + x;
		return (m_data[at / 8] & (1U << (at % 8)));
	}
	else
	{
		return false;
	}
}

void Grid::set(int x, int y, int z, bool val)
{
	if ((unsigned)x < m_width &&
		(unsigned)y < m_height &&
		(unsigned)z < m_depth)
	{
		auto at = (z * m_width * m_height) + (y * m_width) + x;
		if (val)
			m_data[at / 8] |= (1U << (at % 8));
		else
			m_data[at / 8] &= ~(1U << (at % 8));
	}
}
