/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <chrono>
#include <cstdint>

namespace gg
{
	class Timer
	{
	public:
		Timer()
		{
			reset();
		}

		Timer(const Timer& t) :
			m_start_time(t.m_start_time),
			m_last_elapsed(t.m_last_elapsed)
		{
		}

		uint64_t getElapsed()
		{
			uint64_t elapsed_from_start =
				std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::steady_clock::now() - m_start_time).count();

			uint64_t elapsed_now = elapsed_from_start - m_last_elapsed;

			m_last_elapsed = elapsed_from_start;

			return elapsed_now;
		}

		uint64_t peekElapsed() const
		{
			uint64_t elapsed_from_start =
				std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::steady_clock::now() - m_start_time).count();

			return elapsed_from_start - m_last_elapsed;
		}

		void reset()
		{
			m_start_time = std::chrono::steady_clock::now();
			m_last_elapsed = 0;
		}

	private:
		std::chrono::steady_clock::time_point m_start_time;
		uint64_t m_last_elapsed;
	};
};
