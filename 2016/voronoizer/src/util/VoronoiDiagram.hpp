/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include <boost/polygon/voronoi_diagram.hpp>
#include "util/Math2D.hpp"

namespace util
{
	class VoronoiDiagram
	{
	public:
		typedef Point2D<float> Point;
		class Edge;

		class Vertex
		{
		public:
			typedef const boost::polygon::voronoi_diagram<double>::vertex_type InternalType;

			Vertex(InternalType* vertex);
			Vertex(const Vertex& other);
			Point getPoint() const;
			void getEdges(std::vector<Edge>& edges);
			size_t getUserData() const;
			void setUserData(size_t data) const;

		private:
			InternalType* m_vertex;
		};

		class Edge
		{
		public:
			typedef const boost::polygon::voronoi_diagram<double>::edge_type InternalType;

			Edge(InternalType* edge);
			Edge(const Edge& other);
			bool getVertex1(Vertex& v1) const;
			bool getVertex2(Vertex& v2) const;
			Point getPoint1() const;
			Point getPoint2() const;
			size_t getUserData() const;
			void setUserData(size_t data) const;

		private:
			InternalType* m_edge;
		};

		class Cell
		{
		public:
			typedef const boost::polygon::voronoi_diagram<double>::cell_type InternalType;

			Cell(InternalType* cell);
			Cell(const Cell& other);
			Point getPoint() const;
			void getEdges(std::vector<Edge>& edges);
			size_t getUserData() const;
			void setUserData(size_t data) const;

		private:
			InternalType* m_cell;
		};

		VoronoiDiagram() = default;
		VoronoiDiagram(const VoronoiDiagram&) = delete;
		~VoronoiDiagram() = default;
		Vertex getVertex(size_t n) const;
		size_t getVertexCount() const;
		Edge getEdge(size_t n) const;
		size_t getEdgeCount() const;
		Cell getCell(size_t n) const;
		size_t getCellCount() const;

	private:
		friend class VoronoiBuilder;

		boost::polygon::voronoi_diagram<double> m_diagram;
	};
};
