/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "gg/network.hpp"
#include "game.hpp"
#include "network/network.hpp"


network::Network::Network(Game* game) :
	m_game(game),
	m_thread(gg::threadmgr.createThread("Network"))
{
}

network::Network::~Network()
{
}

Game* network::Network::getGame()
{
	return m_game;
}

void network::Network::start(gg::IThread::Mode mode)
{
	m_thread->setState(State::NONE);
	m_thread->run(mode);
}

void network::Network::stop()
{
	m_thread->finishTasks();
	m_thread->join();
}

network::Network::State network::Network::getState() const
{
	return State(m_thread->getState());
}

void network::Network::setState(State state)
{
	m_thread->setState(gg::IThread::State(state));
}

gg::ThreadPtr network::Network::getThread()
{
	return m_thread;
}
