/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <memory>
#include <vector>

class Grid
{
public:
	Grid(unsigned width, unsigned height, unsigned depth);
	~Grid();
	unsigned getWidth() const;
	unsigned getHeight() const;
	unsigned getDepth() const;
	bool get(int x, int y, int z) const;
	void set(int x, int y, int z, bool val);

private:
	unsigned m_width;
	unsigned m_height;
	unsigned m_depth;
	std::vector<char> m_data;
};

typedef std::shared_ptr<Grid> GridPtr;
