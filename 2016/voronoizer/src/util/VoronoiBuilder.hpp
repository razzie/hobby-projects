/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include <boost/polygon/voronoi_builder.hpp>
#include "util/Math2D.hpp"

namespace util
{
	class VoronoiDiagram;

	class VoronoiBuilder
	{
	public:
		typedef Point2D<int> Point;
		typedef Line2D<int> Segment;

		VoronoiBuilder() = default;
		VoronoiBuilder(const VoronoiBuilder&) = delete;
		~VoronoiBuilder() = default;
		void addCell(Point cell);
		void addSegment(Segment segment);
		void addSegments(const std::vector<Point>& points);
		void build(VoronoiDiagram& diagram);

	private:
		boost::polygon::voronoi_builder<int> m_builder;
	};
};
