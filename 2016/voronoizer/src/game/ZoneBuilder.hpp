/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include <vector>
#include <boost/polygon/voronoi_builder.hpp>
#include <raz/random.hpp>
#include "util/Math2D.hpp"

namespace game
{
	struct HubNode;
	class Zone;

	class ZoneBuilder
	{
	public:
		typedef util::Point2D<int> Point;
		typedef util::Polygon2D<int> Polygon;
		typedef util::BoundingBox2D<int> BoundingBox;

		ZoneBuilder(const std::vector<Point>& edges);
		ZoneBuilder(int radius, int edges, float angle = 0.f, Point position = { 0, 0 });
		~ZoneBuilder();
		int getArea() const;
		bool addCell(Point cell);
		void addRandomCells(raz::Random& random, int num);
		void build(Zone& zone);
		void reset();

	private:
		Polygon m_edges;
		BoundingBox m_bbox;
		boost::polygon::voronoi_builder<int> m_builder;
	};
};
