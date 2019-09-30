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

#include <cstdint>
#include <cstring> // memset & memcpy
#include <iterator>
#include <stdexcept>

namespace raz
{
	template<size_t N>
	class Bitset
	{
	public:
		class TrueBitIterator : public std::iterator<std::input_iterator_tag, size_t, size_t, const size_t*, size_t>
		{
		public:
			TrueBitIterator(const TrueBitIterator& other) :
				m_data(other.m_data), m_bit(other.m_bit), m_last_bit(other.m_last_bit)
			{
			}

			TrueBitIterator& operator=(const TrueBitIterator& other)
			{
				m_data = other.m_data;
				m_bit = other.m_bit;
				m_last_bit = other.m_last_bit;
				return *this;
			}

			TrueBitIterator& operator++()
			{
				if (m_bit < m_last_bit)
				{
					++m_bit;
					findNext();
				}
				return *this;
			}

			TrueBitIterator operator++(int)
			{
				TrueBitIterator retval = *this;
				++(*this);
				return retval;
			}

			bool operator==(const TrueBitIterator& other) const
			{
				return (m_bit == other.m_bit && m_data == other.m_data && m_last_bit == other.m_last_bit);
			}

			bool operator!=(const TrueBitIterator& other) const
			{
				return !(*this == other);
			}

			reference operator*() const
			{
				return static_cast<reference>(m_bit);
			}

		private:
			const uint32_t* m_data;
			size_t m_bit;
			size_t m_last_bit;

			friend class Bitset;

			TrueBitIterator(const uint32_t* data, size_t bit, size_t last_bit) :
				m_data(data), m_bit(bit), m_last_bit(last_bit)
			{
				findNext();
			}

			void findNext()
			{
				while (true)
				{
					if (m_bit >= m_last_bit)
					{
						m_bit = m_last_bit;
						return;
					}

					uint32_t value = m_data[m_bit / 32] >> (m_bit % 32);

					if (value == 0)
					{
						m_bit += (32 - (m_bit % 32));
						continue;
					}

					if ((value & 1) != 0)
						break;

					++m_bit;
				}
			}
		};

		class FalseBitIterator : public std::iterator<std::input_iterator_tag, size_t, size_t, const size_t*, size_t>
		{
		public:
			FalseBitIterator(const FalseBitIterator& other) :
				m_data(other.m_data), m_bit(other.m_bit), m_last_bit(other.m_last_bit)
			{
			}

			FalseBitIterator& operator=(const FalseBitIterator& other)
			{
				m_data = other.m_data;
				m_bit = other.m_bit;
				m_last_bit = other.m_last_bit;
				return *this;
			}

			FalseBitIterator& operator++()
			{
				if (m_bit < m_last_bit)
				{
					++m_bit;
					findNext();
				}
				return *this;
			}

			FalseBitIterator operator++(int)
			{
				FalseBitIterator retval = *this;
				++(*this);
				return retval;
			}

			bool operator==(const FalseBitIterator& other) const
			{
				return (m_bit == other.m_bit && m_data == other.m_data && m_last_bit == other.m_last_bit);
			}

			bool operator!=(const FalseBitIterator& other) const
			{
				return !(*this == other);
			}

			reference operator*() const
			{
				return static_cast<reference>(m_bit);
			}

		private:
			const uint32_t* m_data;
			size_t m_bit;
			size_t m_last_bit;

			friend class Bitset;

			FalseBitIterator(const uint32_t* data, size_t bit, size_t last_bit) :
				m_data(data), m_bit(bit), m_last_bit(last_bit)
			{
				findNext();
			}

			void findNext()
			{
				while (true)
				{
					if (m_bit >= m_last_bit)
					{
						m_bit = m_last_bit;
						return;
					}

					uint32_t value = ~m_data[m_bit / 32] >> (m_bit % 32);

					if (value == 0)
					{
						m_bit += (32 - (m_bit % 32));
						continue;
					}

					if ((value & 1) != 0)
						break;

					++m_bit;
				}
			}
		};

		class TrueBits
		{
		public:
			TrueBits(const TrueBits& other) : m_bitset(other.m_bitset)
			{
			}

			size_t count() const
			{
				size_t count = 0;

				for (size_t i = 0; i < DATA_SIZE; ++i)
					count += countTrueBits(m_bitset->m_data[i]);

				return count;
			}

			TrueBitIterator begin() const
			{
				return TrueBitIterator(m_bitset->m_data, 0, N);
			}

			TrueBitIterator end() const
			{
				return TrueBitIterator(m_bitset->m_data, N, N);
			}

		private:
			friend class Bitset;

			TrueBits(const Bitset* bitset) : m_bitset(bitset)
			{
			}

			const Bitset* m_bitset;
		};

		class FalseBits
		{
		public:
			FalseBits(const FalseBits& other) : m_bitset(other.m_bitset)
			{
			}

			size_t count() const
			{
				size_t count = 0;

				for (size_t i = 0; i < DATA_SIZE; ++i)
					count += countFalseBits(m_bitset->m_data[i]);

				return N - count;
			}

			FalseBitIterator begin() const
			{
				return FalseBitIterator(m_bitset->m_data, 0, N);
			}

			FalseBitIterator end() const
			{
				return FalseBitIterator(m_bitset->m_data, N, N);
			}

		private:
			friend class Bitset;

			FalseBits(const Bitset* bitset) : m_bitset(bitset)
			{
			}

			const Bitset* m_bitset;
		};

		Bitset()
		{
			reset();
		}

		Bitset(const Bitset& other)
		{
			std::memcpy(m_data, other.m_data, sizeof(m_data));
		}

		bool isset(size_t pos) const
		{
			if (pos >= N)
				throw std::out_of_range({});

			return (m_data[pos / 32] & (1u << (pos % 32))) != 0;
		}

		void set(size_t pos)
		{
			if (pos >= N)
				throw std::out_of_range({});

			m_data[pos / 32] |= (1u << (pos % 32));
		}

		void unset(size_t pos)
		{
			if (pos >= N)
				throw std::out_of_range({});

			m_data[pos / 32] &= ~(1u << (pos % 32));
		}

		void reset()
		{
			std::memset(m_data, 0, sizeof(m_data));
		}

		const void* data() const
		{
			return m_data;
		}

		size_t size() const
		{
			return sizeof(m_data);
		}

		TrueBits truebits() const
		{
			return TrueBits(this);
		}

		FalseBits falsebits() const
		{
			return FalseBits(this);
		}

		template<class Serializer>
		void operator()(Serializer& serializer)
		{
			for (size_t i = 0; i < DATA_SIZE; ++i)
				serializer(m_data[i]);
		}

	private:
		static constexpr size_t DATA_SIZE = ((N - 1) / 32) + 1;
		uint32_t m_data[DATA_SIZE];

		static size_t countTrueBits(uint32_t x)
		{
			x = x - ((x >> 1) & 0x55555555);
			x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
			x = (x + (x >> 4)) & 0x0F0F0F0F;
			x = x + (x >> 8);
			x = x + (x >> 16);
			return x & 0x0000003F;
		}
	};
}
