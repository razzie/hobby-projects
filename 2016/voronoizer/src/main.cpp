/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <iostream>
#include <SFML/Graphics.hpp>

int test01(sf::RenderWindow&);
int test02(sf::RenderWindow&);
int test03(sf::RenderWindow&);
int test04(sf::RenderWindow&);

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(800, 800), "", sf::Style::Titlebar | sf::Style::Close, settings);

	//while (window.isOpen())
	//{
	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			window.close();
	//	}

	//	window.clear(sf::Color::White);

	//	window.display();
	//}

	//test01(window);
	//test02(window);
	//test03(window);
	test04(window);

	return 0;
}