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

#include <array>
#include <cstdint>
#include <iosfwd>
#include <random>
#include <type_traits>

namespace raz
{
	/*
	 * RandomGenerator is based on the xorshift128+ algorithm
	 * https://en.wikipedia.org/wiki/Xorshift
	 */
	class RandomGenerator
	{
	public:
		typedef uint64_t result_type;
		static constexpr result_type default_seed = 1;
		
		RandomGenerator(result_type value = default_seed)
		{
			seed(value);
		}

		template<class Sseq>
		RandomGenerator(Sseq& seq)
		{
			seed(seq);
		}

		RandomGenerator(const RandomGenerator& other) :
			m_state(other.m_state)
		{
		}

		void seed(result_type value = default_seed)
		{
			m_state[0] = value;
			m_state[1] = 0;
		}

		template<class Sseq>
		void seed(Sseq& seq)
		{
			seq.generate(m_state.begin(), m_state.end());
		}

		result_type operator()()
		{
			uint64_t x = m_state[0];
			uint64_t const y = m_state[1];
			m_state[0] = y;
			x ^= x << 23; // a
			m_state[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
			return (m_state[1] + y);
		}

		void discard(unsigned long long z)
		{
			for (unsigned long long i = 0; i < z; ++i)
			{
				uint64_t x = m_state[0];
				uint64_t const y = m_state[1];
				m_state[0] = y;
				x ^= x << 23; // a
				m_state[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
			}
		}

		bool operator==(const RandomGenerator& other) const
		{
			return (m_state == other.m_state);
		}

		bool operator!=(const RandomGenerator& other) const
		{
			return (m_state != other.m_state);
		}

		template<class Serializer>
		void operator()(Serializer& serializer)
		{
			serializer(m_state[0])(m_state[1]);
		}

		static constexpr result_type min()
		{
			return (0);
		}

		static constexpr result_type max()
		{
			return ((result_type)-1);
		}

		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& ost, const RandomGenerator& gen)
		{
			return (ost << gen.m_state[0] << " " << gen.m_state[1]);
		}

		template<class CharT, class Traits>
		friend std::basic_istream<CharT, Traits>&
			operator>>(std::basic_istream<CharT, Traits>& ist, RandomGenerator& gen)
		{
			return (ist >> gen.m_state[0] >> gen.m_state[1]);
		}

	private:
		std::array<uint64_t, 2> m_state;
	};

	template<class Generator>
	class RandomDistribution
	{
	public:
		RandomDistribution(Generator& generator) :
			m_generator(generator)
		{
		}

		RandomDistribution(const RandomDistribution&) = delete;

		template<class IntType>
		std::enable_if_t<std::is_integral<IntType>::value, IntType>
			operator()(IntType min_value, IntType max_value)
		{
			std::uniform_int_distribution<IntType> dist(min_value, max_value);
			return dist(m_generator);
		}

		template<class RealType>
		std::enable_if_t<std::is_floating_point<RealType>::value, RealType>
			operator()(RealType min_value, RealType max_value)
		{
			std::uniform_real_distribution<RealType> dist(min_value, max_value);
			return dist(m_generator);
		}

	private:
		Generator& m_generator;
	};

	typedef RandomDistribution<RandomGenerator> Random;
}
