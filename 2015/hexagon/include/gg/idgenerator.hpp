/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

/**
 * IDGenerator is a random number generator which returns each possible value
 * only once (then restarts).
 *
 * Credits: Mike McShaffry
 */

#pragma once

#include <cstdint>
#include <random>

namespace gg
{
	typedef uint16_t ID;

	template<class NumberType = ID>
	class IDGenerator
	{
	public:
		IDGenerator() = default;

		IDGenerator(const IDGenerator& other)
		{
			setState(other.getState());
		}

		~IDGenerator() = default;

		NumberType next()
		{
			NumberType val;
			// filling val's bytes with random values one-by-one
			uint8_t* p = reinterpret_cast<uint8_t*>(&val);
			for (size_t i = 0; i < sizeof(NumberType); ++i)
				p[i] = m_generators[i].next();
			return val;
		}

		struct State
		{
			uint32_t step[sizeof(NumberType)];
			uint32_t current[sizeof(NumberType)];
		};

		State getState() const
		{
			State state;
			for (size_t i = 0; i < sizeof(NumberType); ++i)
			{
				state.step[i] = m_generators[i].m_step;
				state.current[i] = m_generators[i].m_current;
			}
		}

		void setState(const State& state)
		{
			for (size_t i = 0; i < sizeof(NumberType); ++i)
			{
				m_generators[i].m_step = state.step[i];
				m_generators[i].m_current = state.current[i];
			}
		}

	private:
		class ByteGenerator
		{
		public:
			ByteGenerator() :
				m_current(0)
			{
				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_int_distribution<uint16_t> dist(0, 255);

				do
				{
					// carefully chosen coefficients
					const uint8_t a = (dist(mt) % 227) + 1;
					const uint8_t b = (dist(mt) % 223) + 1;
					const uint8_t c = (dist(mt) % 211) + 1;

					m_step = (a * 255 * 255) + (b * 255) + c;
					m_step &= ~0xc0000000; // don't let it become too large
					m_step %= 257; // first prime greater than or equal to 255
				} while (m_step == 0);
			}

			uint8_t next()
			{
				do
				{
					m_current += m_step;
					m_current %= 257;
				} while (m_current > 255);
				return static_cast<uint8_t>(m_current);
			}

			uint32_t m_step;
			uint32_t m_current;
		};

		ByteGenerator m_generators[sizeof(NumberType)];
	};
};
