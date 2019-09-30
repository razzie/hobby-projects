/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <SFML/Graphics.hpp>
#include <raz/random.hpp>
#include <raz/timer.hpp>
#include "util/Math2D.hpp"

static void drawPolygon(const util::Polygon2D<float>& polygon, sf::Color color, sf::RenderTarget& target)
{
	if (polygon.points.size() > 0)
	{
		sf::CircleShape shape(2.f);
		shape.setFillColor(color);
		shape.setPosition(polygon.points.front().x - 2.f, polygon.points.front().y - 2.f);
		target.draw(shape);
	}

	for (size_t i = 0, len = polygon.points.size(); i < len; ++i)
	{
		auto a = polygon.points[i];
		auto b = polygon.points[(i + 1) % len];

		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(a.x, a.y), color),
			sf::Vertex(sf::Vector2f(b.x, b.y), color)
		};

		target.draw(line, 2, sf::Lines);
	}
}


int test03(sf::RenderWindow& window)
{
	raz::RandomGenerator gen((uint64_t)std::time(NULL));
	raz::Random random(gen);

	raz::Timer timer;

	util::Polygon2D<float> bounds;
	// top
	bounds.points.push_back({ 250.f, 50.f });
	bounds.points.push_back({ 550.f, 50.f });
	// right border
	bounds.points.push_back({ 550.f, 200.f });
	bounds.points.push_back({ 550.f, 400.f });
	bounds.points.push_back({ 550.f, 600.f });
	// bottom
	bounds.points.push_back({ 550.f, 750.f });
	bounds.points.push_back({ 250.f, 750.f });
	// left border
	bounds.points.push_back({ 250.f, 600.f });
	bounds.points.push_back({ 250.f, 400.f });
	bounds.points.push_back({ 250.f, 200.f });

	for (auto& p : bounds.points)
	{
		p.x += random(-50, 50);
		p.y += random(-50, 50);
	}

	util::Polygon2D<float> cell;

	util::Polygon2D<float> intersection;


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);

		float speed = 0.0005f * timer.peekElapsed();

		cell.points.clear();
		cell.initNgon(150.f, 6, speed * 10.f, { 400.f + std::sin(speed) * 200.f, 400.f + std::cos(speed) * 200.f });

		intersection.points.clear();
		util::Polygon2D<float>::intersect(bounds, cell, intersection.points);

		drawPolygon(bounds, sf::Color::Red, window);
		drawPolygon(cell, sf::Color::Blue, window);
		drawPolygon(intersection, sf::Color::Black, window);

		window.display();
	}

	return 0;
}
