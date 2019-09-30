/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include "util/perlin.hpp"
#include "util/grid.hpp"

namespace backend
{
	class SurfaceData
	{
	public:
		SurfaceData(unsigned seed, unsigned width, unsigned height, unsigned depth);
		~SurfaceData();

		bool get(unsigned x, unsigned y, unsigned z) const;
		GridPtr genGrid() const;
		GridPtr genGrid(
			unsigned x, unsigned y, unsigned z,
			unsigned width, unsigned height, unsigned depth) const;

	private:
		unsigned m_seed;
		mutable PerlinNoise<256> m_noise;
		unsigned m_width;
		unsigned m_height;
		unsigned m_depth;
		float m_center_x;
		float m_center_y;
		float m_center_z;
	};
};
