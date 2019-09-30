/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "backend/backend.hpp"
#include "network/network.hpp"
#include "ui/ui.hpp"
#include "game.hpp"


Game::Game()
{
	m_parameters.screen_width = 1024;
	m_parameters.screen_hight = 768;
	m_parameters.fullscreen = false;
}

Game::~Game()
{

}

Game::Parameters& Game::getParameters()
{
	return m_parameters;
}

const Game::Parameters& Game::getParameters() const
{
	return m_parameters;
}

void Game::setupParameters(int argc, char** argv)
{

}

int Game::run()
{
	backend::Backend _backend(this);
	network::Network _network(this);
	ui::UI _ui(this);

	_backend.start(gg::IThread::Mode::REMOTE);
	_network.start(gg::IThread::Mode::REMOTE);
	_ui.start(gg::IThread::Mode::LOCAL);

	// ui thread stopped: user exited game

	_network.stop();
	_backend.stop();

	return 0;
}
