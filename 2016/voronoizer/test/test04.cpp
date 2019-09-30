/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <SFML/Graphics.hpp>
#include <raz/random.hpp>
#include "util/Math2D.hpp"
#include "util/VoronoiBuilder.hpp"
#include "util/VoronoiDiagram.hpp"

static void generateCells(const util::Polygon2D<float>& bounds, std::vector<sf::ConvexShape>& out)
{
	if (bounds.points.size() == 0)
		return;

	raz::RandomGenerator gen((uint64_t)std::time(NULL));
	raz::Random random(gen);

	util::BoundingBox2D<float> bbox;
	bbox.update(bounds);
	//bbox.min_x += 20.f;
	//bbox.min_y += 20.f;
	//bbox.max_x -= 20.f;
	//bbox.max_y -= 20.f;
	//bbox.max_y = (bbox.getHeight() / 3.f) + bbox.min_y;

	float center_y = (bbox.min_y + bbox.max_y) / 2.f;

	util::VoronoiBuilder builder;

	for (int cells = 0; cells < 100; )
	{
		util::Point2D<int> cell;
		cell.x = random((int)bbox.min_x, (int)bbox.max_x);
		cell.y = random((int)bbox.min_y, (int)bbox.max_y);

		if (bounds.contains({ (float)cell.x, (float)cell.y }))
		{
			builder.addCell(cell);
			++cells;
		}
	}

	util::VoronoiDiagram diagram;
	builder.build(diagram);

	std::vector<util::VoronoiDiagram::Edge> edges;
	util::Polygon2D<float> cell_points;
	util::Polygon2D<float> clipped_cell_points;

	for (size_t i = 0, len = diagram.getCellCount(); i < len; ++i)
	{
		auto cell = diagram.getCell(i);

		edges.clear();
		cell_points.points.clear();
		clipped_cell_points.points.clear();

		cell.getEdges(edges);

		for (auto edge : edges)
		{
			//util::VoronoiDiagram::Vertex v(nullptr);
			//if (edge.getVertex1(v))
			//{
			//	cell_points.points.push_back(v.getPoint());
			//}

			auto p = edge.getPoint1();
			p.y += (p.y - center_y) * 2.f;
			cell_points.points.push_back(p);
		}

		if (cell_points.points.size() > 0)
		{
			if (cell_points.isCCW())
			{
				std::reverse(cell_points.points.begin(), cell_points.points.end());
			}

			util::Polygon2D<float>::intersect(bounds, cell_points, clipped_cell_points.points);

			if (clipped_cell_points.points.size() > 0 && clipped_cell_points.getArea() > 500)
			{
				sf::ConvexShape cell(clipped_cell_points.points.size());
				uint8_t color = random(128, 196);
				cell.setFillColor(sf::Color(color, color, color));

				for (size_t j = 0, len = clipped_cell_points.points.size(); j < len; ++j)
				{
					cell.setPoint(j, sf::Vector2f(clipped_cell_points.points[j].x, clipped_cell_points.points[j].y));
				}

				out.push_back(std::move(cell));
			}
		}
	}
}


int test04(sf::RenderWindow& window)
{
	raz::RandomGenerator gen((uint64_t)std::time(NULL));
	raz::Random random(gen);

	util::Polygon2D<float> bounds;
	// top
	bounds.points.push_back({ 250.f, 50.f });
	bounds.points.push_back({ 350.f, 50.f });
	bounds.points.push_back({ 450.f, 50.f });
	bounds.points.push_back({ 550.f, 50.f });
	// right border
	bounds.points.push_back({ 550.f, 200.f });
	bounds.points.push_back({ 550.f, 300.f });
	bounds.points.push_back({ 550.f, 400.f });
	bounds.points.push_back({ 550.f, 500.f });
	bounds.points.push_back({ 550.f, 600.f });
	// bottom
	bounds.points.push_back({ 550.f, 750.f });
	bounds.points.push_back({ 450.f, 750.f });
	bounds.points.push_back({ 350.f, 750.f });
	bounds.points.push_back({ 250.f, 750.f });
	// left border
	bounds.points.push_back({ 250.f, 600.f });
	bounds.points.push_back({ 250.f, 500.f });
	bounds.points.push_back({ 250.f, 400.f });
	bounds.points.push_back({ 250.f, 300.f });
	bounds.points.push_back({ 250.f, 200.f });

	for (auto& p : bounds.points)
	{
		p.x += random(-30, 30);
		p.y += random(-10, 10);
	}

	std::vector<sf::ConvexShape> cells;
	generateCells(bounds, cells);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);

		for (auto& cell : cells)
			window.draw(cell);

		//for (size_t i = 0, show_cells = (timer.peekElapsed() / 500) % cells.size(); i < show_cells; ++i)
		//{
		//	window.draw(cells[i]);
		//	//std::cout << show_cells << std::endl;
		//}

		window.display();
	}

	return 0;
}
