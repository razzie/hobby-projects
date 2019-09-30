/*
Copyright (C) 2016 - Gábor "Razzie" Görzsöny

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*/

#pragma once

#include <chrono>
#include <cstdint>

namespace raz
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
