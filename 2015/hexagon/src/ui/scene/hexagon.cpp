/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "ui/scene/hexagon.hpp"


void ui::Hexagon::add(Hexagon::SideInfo side_info, irr::scene::IDynamicMeshBuffer* meshbuf, const irr::core::vector3df& pos, irr::video::SColor color)
{
	enum VtxIndex : unsigned
	{
		TOP,
		TOP_UP1,
		TOP_UP2,
		TOP_RIGHT,
		TOP_DOWN2,
		TOP_DOWN1,
		TOP_LEFT,
		BOTTOM,
		BOTTOM_UP1,
		BOTTOM_UP2,
		BOTTOM_RIGHT,
		BOTTOM_DOWN2,
		BOTTOM_DOWN1,
		BOTTOM_LEFT
	};

	int vertex_indices[14] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	irr::video::S3DVertex vertex_buf[14] =
	{
		// top
		irr::video::S3DVertex(0.f, 0.5f, 0.f,  0.f, 0.5f, 0.f,  0xffffffff,  0.f, 0.f),
		// top - top 1
		irr::video::S3DVertex(-0.4f, 0.5f, 0.5f,  -0.4f, 0.5f, 0.5f,  0xffffffff,  0.f, 0.f),
		// top - top 2
		irr::video::S3DVertex(0.4f, 0.5f, 0.5f,  0.4f, 0.5f, 0.5f,  0xffffffff,  0.f, 0.f),
		// top - right
		irr::video::S3DVertex(0.6f, 0.5f, 0.f,  0.5f, 0.5f, 0.f,  0xffffffff,  0.f, 0.f),
		// top - bottom 2
		irr::video::S3DVertex(0.4f, 0.5f, -0.5f,  0.4f, 0.5f, -0.5f,  0xffffffff,  0.f, 0.f),
		// top - bottom 1
		irr::video::S3DVertex(-0.4f, 0.5f, -0.5f,  -0.4f, 0.5f, -0.5f,  0xffffffff,  0.f, 0.f),
		// top - left
		irr::video::S3DVertex(-0.6f, 0.5f, 0.f,  -0.5f, 0.5f, 0.f,  0xffffffff,  0.f, 0.f),
		// bottom
		irr::video::S3DVertex(0.f, -0.5f, 0.f,  0.f, -0.5f, 0.f,  0xffffffff,  0.f, 0.f),
		// bottom - top 1
		irr::video::S3DVertex(-0.4f, -0.5f, 0.5f,  -0.4f, -0.5f, 0.5f,  0xffffffff,  0.f, 0.f),
		// bottom - top 2
		irr::video::S3DVertex(0.4f, -0.5f, 0.5f,  0.4f, -0.5f, 0.5f,  0xffffffff,  0.f, 0.f),
		// bottom - right
		irr::video::S3DVertex(0.6f, -0.5f, 0.f,  0.5f, -0.5f, 0.f,  0xffffffff,  0.f, 0.f),
		// bottom - bottom 2
		irr::video::S3DVertex(0.4f, -0.5f, -0.5f,  0.4f, -0.5f, -0.5f,  0xffffffff,  0.f, 0.f),
		// bottom - bottom 1
		irr::video::S3DVertex(-0.4f, -0.5f, -0.5f,  -0.4f, -0.5f, -0.5f,  0xffffffff,  0.f, 0.f),
		// bottom - left
		irr::video::S3DVertex(-0.6f, -0.5f, 0.f,  -0.5f, -0.5f, 0.f,  0xffffffff,  0.f, 0.f)
	};

	irr::scene::IVertexBuffer& vertices = meshbuf->getVertexBuffer();
	irr::scene::IIndexBuffer& indices = meshbuf->getIndexBuffer();

	auto vtx_index = [&](VtxIndex vtx) -> unsigned
	{
		unsigned vtx_index = vertex_indices[vtx];
		if (vtx_index == -1)
		{
			vtx_index = vertex_indices[vtx] = vertices.size();
			irr::video::S3DVertex v = vertex_buf[vtx];
			v.Color = color;
			//v.Pos *= 1.2;
			v.Pos += pos;
			vertices.push_back(v);
		}
		return vtx_index;
	};

	auto triangle = [&](VtxIndex vtx1, VtxIndex vtx2, VtxIndex vtx3)
	{
		indices.push_back(vtx_index(vtx1));
		indices.push_back(vtx_index(vtx2));
		indices.push_back(vtx_index(vtx3));
	};

#define VERTEX(vtx) \
	vertex_buf[VtxIndex::vtx]

	if (!side_info.top)
	{
		VERTEX(TOP_UP1).Normal.Y = 0.f;
		VERTEX(TOP_UP2).Normal.Y = 0.f;
		VERTEX(TOP_RIGHT).Normal.Y = 0.f;
		VERTEX(TOP_DOWN2).Normal.Y = 0.f;
		VERTEX(TOP_DOWN1).Normal.Y = 0.f;
		VERTEX(TOP_LEFT).Normal.Y = 0.f;
	}

	if (!side_info.bottom)
	{
		VERTEX(BOTTOM_UP1).Normal.Y = 0.f;
		VERTEX(BOTTOM_UP2).Normal.Y = 0.f;
		VERTEX(BOTTOM_RIGHT).Normal.Y = 0.f;
		VERTEX(BOTTOM_DOWN2).Normal.Y = 0.f;
		VERTEX(BOTTOM_DOWN1).Normal.Y = 0.f;
		VERTEX(BOTTOM_LEFT).Normal.Y = 0.f;
	}

	/*if (side_info.top)
	{
		if (side_info.left1 && side_info.up)
			VERTEX(TOP_UP1).Pos.Y -= 0.5f;

		if (side_info.up && side_info.right1)
			VERTEX(TOP_UP2).Pos.Y -= 0.5f;

		if (side_info.right1 && side_info.right2)
			VERTEX(TOP_RIGHT).Pos.Y -= 0.5f;

		if (side_info.right2 && side_info.down)
			VERTEX(TOP_DOWN2).Pos.Y -= 0.5f;

		if (side_info.down && side_info.left2)
			VERTEX(TOP_DOWN1).Pos.Y -= 0.5f;

		if (side_info.left2 && side_info.left1)
			VERTEX(TOP_LEFT).Pos.Y -= 0.5f;
	}
	else if (side_info.bottom)
	{
		if (side_info.left1 && side_info.up)
			VERTEX(BOTTOM_UP1).Pos.Y += 0.5f;

		if (side_info.up && side_info.right1)
			VERTEX(BOTTOM_UP2).Pos.Y += 0.5f;

		if (side_info.right1 && side_info.right2)
			VERTEX(BOTTOM_RIGHT).Pos.Y += 0.5f;

		if (side_info.right2 && side_info.down)
			VERTEX(BOTTOM_DOWN2).Pos.Y += 0.5f;

		if (side_info.down && side_info.left2)
			VERTEX(BOTTOM_DOWN1).Pos.Y += 0.5f;

		if (side_info.left2 && side_info.left1)
			VERTEX(BOTTOM_LEFT).Pos.Y += 0.5f;
	}*/

#undef VERTEX

#define TRIANGLE(vtx1, vtx2, vtx3) \
	triangle(VtxIndex::vtx1, VtxIndex::vtx2, VtxIndex::vtx3)

	if (side_info.top)
	{
		TRIANGLE(TOP, TOP_UP1, TOP_UP2);
		TRIANGLE(TOP, TOP_UP2, TOP_RIGHT);
		TRIANGLE(TOP, TOP_RIGHT, TOP_DOWN2);
		TRIANGLE(TOP, TOP_DOWN2, TOP_DOWN1);
		TRIANGLE(TOP, TOP_DOWN1, TOP_LEFT);
		TRIANGLE(TOP, TOP_LEFT, TOP_UP1);
	}

	if (side_info.up)
	{
		TRIANGLE(TOP_UP2, TOP_UP1, BOTTOM_UP1);
		TRIANGLE(TOP_UP2, BOTTOM_UP1, BOTTOM_UP2);
	}

	if (side_info.right1)
	{
		TRIANGLE(TOP_RIGHT, TOP_UP2, BOTTOM_UP2);
		TRIANGLE(TOP_RIGHT, BOTTOM_UP2, BOTTOM_RIGHT);
	}

	if (side_info.right2)
	{
		TRIANGLE(TOP_DOWN2, TOP_RIGHT, BOTTOM_RIGHT);
		TRIANGLE(TOP_DOWN2, BOTTOM_RIGHT, BOTTOM_DOWN2);
	}

	if (side_info.down)
	{
		TRIANGLE(TOP_DOWN1, TOP_DOWN2, BOTTOM_DOWN2);
		TRIANGLE(TOP_DOWN1, BOTTOM_DOWN2, BOTTOM_DOWN1);
	}

	if (side_info.left2)
	{
		TRIANGLE(TOP_LEFT, TOP_DOWN1, BOTTOM_DOWN1);
		TRIANGLE(TOP_LEFT, BOTTOM_DOWN1, BOTTOM_LEFT);
	}

	if (side_info.left1)
	{
		TRIANGLE(TOP_UP1, TOP_LEFT, BOTTOM_LEFT);
		TRIANGLE(TOP_UP1, BOTTOM_LEFT, BOTTOM_UP1);
	}

	if (side_info.bottom)
	{
		TRIANGLE(BOTTOM, BOTTOM_UP2, BOTTOM_UP1);
		TRIANGLE(BOTTOM, BOTTOM_RIGHT, BOTTOM_UP2);
		TRIANGLE(BOTTOM, BOTTOM_DOWN2, BOTTOM_RIGHT);
		TRIANGLE(BOTTOM, BOTTOM_DOWN1, BOTTOM_DOWN2);
		TRIANGLE(BOTTOM, BOTTOM_LEFT, BOTTOM_DOWN1);
		TRIANGLE(BOTTOM, BOTTOM_UP1, BOTTOM_LEFT);
	}

#undef TRIANGLE
}

void ui::Hexagon::build(const Grid& grid, irr::scene::IDynamicMeshBuffer* meshbuf, const irr::core::vector3df& trans, irr::video::SColor color)
{
	for (int x = 0; x < (int)grid.getWidth(); ++x)
	{
		for (int y = 0; y < (int)grid.getHeight(); ++y)
		{
			for (int z = 0; z < (int)grid.getDepth(); ++z)
			{
				if (grid.get(x, y, z))
				{
					Hexagon::SideInfo side_info;
					side_info.top = !grid.get(x, y + 1, z);
					side_info.bottom = !grid.get(x, y - 1, z);
					side_info.up = !grid.get(x, y, z + 1);
					side_info.down = !grid.get(x, y, z - 1);
					if (x % 2 == 0)
					{
						side_info.left1 = !grid.get(x - 1, y, z + 1);
						side_info.left2 = !grid.get(x - 1, y, z);
						side_info.right1 = !grid.get(x + 1, y, z + 1);
						side_info.right2 = !grid.get(x + 1, y, z);
					}
					else
					{
						side_info.left1 = !grid.get(x - 1, y, z);
						side_info.left2 = !grid.get(x - 1, y, z - 1);
						side_info.right1 = !grid.get(x + 1, y, z);
						side_info.right2 = !grid.get(x + 1, y, z - 1);
					}

					irr::core::vector3df pos((float)x, (float)y, (float)z);
					if (x % 2 == 0)
						pos.Z += 0.5f;

					Hexagon::add(side_info, meshbuf, pos + trans, color);
				}

			}
		}
	}
}
