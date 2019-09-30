/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <SFML/Graphics.hpp>
#include "game/Zone.hpp"
#include "game/ZoneBuilder.hpp"

static void drawZone(const game::Zone& zone, sf::RenderTarget& target)
{
	sf::CircleShape circle;
	circle.setFillColor(sf::Color::White);
	circle.setOutlineThickness(1.0f);
	circle.setOutlineColor(sf::Color::Black);

	for (auto& path : zone.getPaths())
	{
		auto a = static_cast<const game::HubNode*>(zone.getNode(path.hub1))->position;
		auto b = static_cast<const game::HubNode*>(zone.getNode(path.hub2))->position;

		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(a.x, a.y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(b.x, b.y), sf::Color::Black)
		};

		target.draw(line, 2, sf::Lines);
	}

	for (auto& hub : zone.getHubs())
	{
		circle.setPosition({ hub.position.x - hub.radius, hub.position.y - hub.radius });
		circle.setRadius(hub.radius);
		target.draw(circle);
	}
}


int test01(sf::RenderWindow& window)
{
	raz::RandomGenerator gen((uint64_t)std::time(NULL));
	raz::Random random(gen);

	game::ZoneBuilder builder(380, 6, 0.f, { 400, 400 });
	builder.addRandomCells(random, 50);

	game::Zone zone(0, nullptr);
	builder.build(zone);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);

		drawZone(zone, window);

		window.display();
	}

	return 0;
}
