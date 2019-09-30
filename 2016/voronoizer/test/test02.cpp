/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <SFML/Graphics.hpp>
#include <boost/polygon/voronoi_builder.hpp>
#include <boost/polygon/voronoi_diagram.hpp>
#include <raz/random.hpp>
#include <raz/timer.hpp>
#include "util/Math2D.hpp"

static void drawDiagram(sf::RenderTarget& target, const boost::polygon::voronoi_diagram<double>& diagram)
{
	static raz::Timer timer;
	util::Polygon2D<float> vertices;

	//for (auto& cell : diagram.cells())
	for (size_t i = 0, show_cells = /*(timer.peekElapsed() / 100) %*/ diagram.cells().size(); i < show_cells; ++i)
	{
		vertices.points.clear();

		auto& cell = diagram.cells()[i];

		auto edge = cell.incident_edge();
		do
		{
			if (edge->is_primary() && edge->vertex0())
			{
				auto v = edge->vertex0();
				vertices.points.push_back({ (float)v->vertex().x(), (float)v->vertex().y() });
			}

			edge = edge->next();

		} while (edge != cell.incident_edge());

		uint8_t color = (255 * i) / diagram.cells().size();

		sf::ConvexShape shape;
		//shape.setFillColor(color_table[cell.source_index() % 64]);
		//shape.setFillColor(sf::Color(color));
		if (vertices.isCW())
			shape.setFillColor(sf::Color(color, 0, 0));
		else
			shape.setFillColor(sf::Color(0, 0, color));
		shape.setPointCount(vertices.points.size());
		for (size_t i = 0; i < vertices.points.size(); ++i)
		{
			sf::Vector2f v(vertices.points[i].x, vertices.points[i].y);
			shape.setPoint(i, v);
		}

		target.draw(shape);
	}
}


int test02(sf::RenderWindow& window)
{
	raz::RandomGenerator gen((uint64_t)std::time(NULL));
	raz::Random random(gen);

	boost::polygon::voronoi_builder<int> builder;
	for (int i = 0; i < 50; ++i)
		builder.insert_point(random(200, 600), random(200, 600));

	boost::polygon::voronoi_diagram<double> diagram;
	builder.construct(&diagram);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);

		drawDiagram(window, diagram);

		window.display();
	}

	return 0;
}
