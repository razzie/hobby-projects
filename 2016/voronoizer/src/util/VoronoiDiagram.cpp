/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "util/VoronoiDiagram.hpp"


util::VoronoiDiagram::Vertex::Vertex(InternalType* vertex) :
	m_vertex(vertex)
{
}

util::VoronoiDiagram::Vertex::Vertex(const Vertex& other) :
	m_vertex(other.m_vertex)
{
}

util::VoronoiDiagram::Point util::VoronoiDiagram::Vertex::getPoint() const
{
	return Point{ (float)m_vertex->vertex().x(), (float)m_vertex->vertex().y() };
}

void util::VoronoiDiagram::Vertex::getEdges(std::vector<Edge>& edges)
{
	const typename InternalType::voronoi_edge_type* edge = m_vertex->incident_edge();

	do
	{
		if (edge->is_primary())
			edges.push_back(Edge(edge));

		edge = edge->rot_next();

	} while (edge != m_vertex->incident_edge());
}

size_t util::VoronoiDiagram::Vertex::getUserData() const
{
	return m_vertex->color();
}

void util::VoronoiDiagram::Vertex::setUserData(size_t data) const
{
	m_vertex->color(data);
}


util::VoronoiDiagram::Edge::Edge(InternalType* edge) :
	m_edge(edge)
{
}

util::VoronoiDiagram::Edge::Edge(const Edge& other) :
	m_edge(other.m_edge)
{
}

bool util::VoronoiDiagram::Edge::getVertex1(Vertex& v1) const
{
	if (!m_edge->vertex0())
		return false;

	v1 = Vertex(m_edge->vertex0());
	return true;
}

bool util::VoronoiDiagram::Edge::getVertex2(Vertex& v2) const
{
	if (!m_edge->vertex1())
		return false;

	v2 = Vertex(m_edge->vertex1());
	return true;
}

util::VoronoiDiagram::Point util::VoronoiDiagram::Edge::getPoint1() const
{
	if (m_edge->vertex0())
	{
		auto v = m_edge->vertex0()->vertex();
		return { (float)v.x(), (float)v.y() };
	}
	else
	{
		using point_type = boost::polygon::detail::point_2d<double>;

		const typename InternalType::voronoi_cell_type *cell1 = m_edge->cell();
		const typename InternalType::voronoi_cell_type *cell2 = m_edge->twin()->cell();
		point_type point1 = cell1->point0();
		point_type point2 = cell2->point0();

		if (point1 == point2) {
			if (cell1->contains_segment())
				point1 = cell1->point1();
			else
				point2 = cell2->point1();
		}

		return { (float)point1.x(), (float)point1.y() };
	}
}

util::VoronoiDiagram::Point util::VoronoiDiagram::Edge::getPoint2() const
{
	if (m_edge->vertex1())
	{
		auto v = m_edge->vertex1()->vertex();
		return{ (float)v.x(), (float)v.y() };
	}
	else
	{
		using point_type = boost::polygon::detail::point_2d<double>;

		const typename InternalType::voronoi_cell_type *cell1 = m_edge->cell();
		const typename InternalType::voronoi_cell_type *cell2 = m_edge->twin()->cell();
		point_type point1 = cell1->point0();
		point_type point2 = cell2->point0();

		if (point1 == point2) {
			if (cell1->contains_segment())
				point1 = cell1->point1();
			else
				point2 = cell2->point1();
		}

		return{ (float)point2.x(), (float)point2.y() };
	}
}

size_t util::VoronoiDiagram::Edge::getUserData() const
{
	return m_edge->color();
}

void util::VoronoiDiagram::Edge::setUserData(size_t data) const
{
	m_edge->color(data);
}


util::VoronoiDiagram::Cell::Cell(InternalType* cell) :
	m_cell(cell)
{
}

util::VoronoiDiagram::Cell::Cell(const Cell& other) :
	m_cell(other.m_cell)
{
}

util::VoronoiDiagram::Point util::VoronoiDiagram::Cell::getPoint() const
{
	return Point{ (float)m_cell->point0().x(), (float)m_cell->point0().y()  };
}

void util::VoronoiDiagram::Cell::getEdges(std::vector<Edge>& edges)
{
	const typename InternalType::voronoi_edge_type* edge = m_cell->incident_edge();

	do
	{
		if (edge->is_primary())
			edges.push_back(Edge(edge));

		edge = edge->next();

	} while (edge != m_cell->incident_edge());
}

size_t util::VoronoiDiagram::Cell::getUserData() const
{
	return m_cell->color();
}

void util::VoronoiDiagram::Cell::setUserData(size_t data) const
{
	m_cell->color(data);
}


util::VoronoiDiagram::Vertex util::VoronoiDiagram::getVertex(size_t n) const
{
	return Vertex(&m_diagram.vertices()[n]);
}

size_t util::VoronoiDiagram::getVertexCount() const
{
	return m_diagram.vertices().size();
}

util::VoronoiDiagram::Edge util::VoronoiDiagram::getEdge(size_t n) const
{
	return Edge(&m_diagram.edges()[n]);
}

size_t util::VoronoiDiagram::getEdgeCount() const
{
	return m_diagram.edges().size();
}

util::VoronoiDiagram::Cell util::VoronoiDiagram::getCell(size_t n) const
{
	return Cell(&m_diagram.cells()[n]);
}

size_t util::VoronoiDiagram::getCellCount() const
{
	return m_diagram.cells().size();
}
