/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#include "gg/logger.hpp"
#include "gg/resource.hpp"
#include "gg/database.hpp"
#include "game.hpp"
#include "backend/backend.hpp"


backend::Backend::Backend(Game* game) :
	m_game(game),
	m_thread(gg::threadmgr.createThread("Backend"))
{
}

backend::Backend::~Backend()
{
}

Game* backend::Backend::getGame()
{
	return m_game;
}

void backend::Backend::start(gg::IThread::Mode mode)
{
	m_thread->setState(State::SINGLE_PLAY);
	m_thread->run(mode);
}

void backend::Backend::stop()
{
	m_thread->finish();
	m_thread->join();
}

backend::Backend::State backend::Backend::getState() const
{
	return State(m_thread->getState());
}

void backend::Backend::setState(State state)
{
	m_thread->setState(gg::IThread::State(state));
}

gg::ThreadPtr backend::Backend::getThread()
{
	return m_thread;
}
