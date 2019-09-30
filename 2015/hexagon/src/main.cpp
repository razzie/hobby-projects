/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

//#include "gg/logger.hpp"
#include "game.hpp"

int main(int argc, char** argv)
{
	Game game;
	game.setupParameters(argc, argv);
	return game.run();
}
