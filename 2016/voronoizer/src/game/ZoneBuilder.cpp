/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <boost/polygon/voronoi_diagram.hpp>
#include "game/Zone.hpp"
#include "game/ZoneBuilder.hpp"


game::ZoneBuilder::ZoneBuilder(const std::vector<Point>& edges) :
	m_edges{ edges }
{
	m_bbox.update(m_edges);
	reset();
}

game::ZoneBuilder::ZoneBuilder(int radius, int edges, float angle, Point position)
{
	m_edges.initNgon(radius, edges, angle, position);
	m_bbox.update(m_edges);
	reset();
}

game::ZoneBuilder::~ZoneBuilder()
{
}

int game::ZoneBuilder::getArea() const
{
	return m_edges.getArea();
}

bool game::ZoneBuilder::addCell(Point cell)
{
	if (!m_edges.contains(cell))
		return false;

	m_builder.insert_point(cell.x, cell.y);

	return true;
}

void game::ZoneBuilder::addRandomCells(raz::Random& random, int num)
{
	for (int i = 0; i < num; ++i)
	{
		Point cell;
		do
		{
			cell.x = random(m_bbox.min_x, m_bbox.max_x);
			cell.y = random(m_bbox.min_y, m_bbox.max_y);
		} while (!addCell(cell));
	}
}

void game::ZoneBuilder::build(Zone& zone)
{
	boost::polygon::voronoi_diagram<double> diagram;
	m_builder.construct(&diagram);

	auto getCloseVertex = [&](Zone::Position pos) -> const boost::polygon::voronoi_vertex<double>*
	{
		for (auto& v : diagram.vertices())
		{
			if (v.color() != 0 && pos.getDistanceFromSq({ (float)v.vertex().x(), (float)v.vertex().y() }) < 256.f)
				return &v;
		}

		return nullptr;
	};

	auto areNeighbourHubs = [&](NodeID hub1, NodeID hub2) -> bool
	{
		for (auto path_id : zone.getNode(hub1)->connections)
		{
			auto path = static_cast<const PathNode*>(zone.getNode(path_id));
			if (path->hub1 == hub2 || path->hub2 == hub2)
				return true;
		}

		return false;
	};

	auto colorToNodeID = [&](boost::polygon::voronoi_vertex<double>::color_type color) -> NodeID
	{
		return *reinterpret_cast<NodeID*>(&color);
	};

	auto nodeIDtoColor = [&](NodeID id) -> boost::polygon::voronoi_vertex<double>::color_type
	{
		boost::polygon::voronoi_vertex<double>::color_type color = 0;
		*reinterpret_cast<NodeID*>(&color) = id;
		return color;
	};

	for (auto& vertex : diagram.vertices())
	{
		vertex.color(0);
	}
	
	for (auto& vertex : diagram.vertices())
	{
		Zone::Position pos = { (float)vertex.vertex().x(), (float)vertex.vertex().y() };

		auto duplicate_hub = getCloseVertex(pos);
		if (duplicate_hub)
		{
			vertex.color(duplicate_hub->color());
		}
		else
		{
			const HubNode* hub = zone.addHub(pos);
			if (hub)
				vertex.color(nodeIDtoColor(hub->id));
		}
	}

	for (auto edge = diagram.edges().begin(), end = diagram.edges().end(); edge != end; edge += 2)
	//for (auto& edge : diagram.edges())
	{
		auto v1 = edge->vertex0();
		auto v2 = edge->vertex1();

		if (v1 && v2)
		{
			auto hub1 = colorToNodeID(v1->color());
			auto hub2 = colorToNodeID(v2->color());

			if (!areNeighbourHubs(hub1, hub2))
				zone.addPath(hub1, hub2);
		}
	}
}

void game::ZoneBuilder::reset()
{
	m_builder.clear();

	for (size_t i = 0, len = m_edges.points.size(); i != len; ++i)
	{
		auto p1 = m_edges.points[i];
		auto p2 = m_edges.points[(i + 1) % len];
		m_builder.insert_segment(p1.x, p1.y, p2.x, p2.y);
	}
}
