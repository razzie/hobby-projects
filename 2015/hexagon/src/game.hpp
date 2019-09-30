/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <cstdint>

class Game
{
public:
	struct Parameters
	{
		unsigned screen_width;
		unsigned screen_hight;
		bool fullscreen;
		/* etc */
	};

	Game();
	~Game();
	Parameters& getParameters();
	const Parameters& getParameters() const;
	void setupParameters(int argc, char** argv);
	int run();

private:
	Parameters m_parameters;
};
