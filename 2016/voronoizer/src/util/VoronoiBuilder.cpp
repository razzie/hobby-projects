/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "util/VoronoiBuilder.hpp"
#include "util/VoronoiDiagram.hpp"


void util::VoronoiBuilder::addCell(Point cell)
{
	m_builder.insert_point(cell.x, cell.y);
}

void util::VoronoiBuilder::addSegment(Segment segment)
{
	m_builder.insert_segment(segment.start.x, segment.start.y, segment.end.x, segment.end.y);
}

void util::VoronoiBuilder::addSegments(const std::vector<Point>& points)
{
	for (size_t i = 0, len = points.size(); i != len; ++i)
	{
		Segment segment = { points[i], points[(i + 1) % len] };
		addSegment(segment);
	}
}

void util::VoronoiBuilder::build(VoronoiDiagram& diagram)
{
	m_builder.construct(&diagram.m_diagram);
}
