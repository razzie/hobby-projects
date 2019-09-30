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

#include <stdexcept>
#include <typeinfo>

namespace raz
{
	template<class... Types>
	class Storage
	{
	public:
		Storage()
		{
			default_construct<0, 0, Types...>();
		}

		Storage(Types... values)
		{
			construct<0, 0, Types...>(std::forward<Types>(values)...);
		}

		~Storage()
		{
			destruct<0, Types...>();
		}

		unsigned count() const
		{
			return COUNT;
		}

		template<unsigned N, class R = Param<N, Params...>::Type>
		R& get()
		{
			return *reinterpret_cast<R*>(m_ptrs[N]);
		}

		template<unsigned N, class R = Param<N, Params...>::Type>
		const R& get() const
		{
			return *reinterpret_cast<const R*>(m_ptrs[N]);
		}

		char* getPtr(unsigned n)
		{
			if (n >= COUNT) throw std::out_of_range({});
			return m_ptrs[n];
		}

		const char* getPtr(unsigned n) const
		{
			if (n >= COUNT) throw std::out_of_range({});
			return m_ptrs[n];
		}

		const std::type_info& getType(unsigned n) const
		{
			if (n >= COUNT) throw std::out_of_range({});
			return *m_types[n];
		}

	private:
		template<unsigned...>
		struct sum;

		template<unsigned size>
		struct sum<size>
		{
			enum { value = size };
		};

		template<unsigned size, unsigned... sizes>
		struct sum<size, sizes...>
		{
			enum { value = size + sum<sizes...>::value };
		};

		template<unsigned N, size_t offset>
		void construct() {}

		template<unsigned N, size_t offset, class T0, class... Ts>
		void construct(T0 t0, Ts... ts)
		{
			m_ptrs[N] = reinterpret_cast<char*>(new (m_buffer + offset) T0(std::forward<T0>(t0)));
			m_types[N] = &typeid(T0);
			construct<N + 1, offset + sizeof(T0), Ts...>(std::forward<Ts>(ts)...);
		}

		template<unsigned N, size_t offset>
		void default_construct() {}

		template<unsigned N, size_t offset, class T0, class... Ts>
		void default_construct()
		{
			m_ptrs[N] = reinterpret_cast<char*>(new (m_buffer + offset) T0());
			m_types[N] = &typeid(T0);
			default_construct<N + 1, offset + sizeof(T0), Ts...>();
		}

		template<size_t offset>
		void destruct() {}

		template<size_t offset, class T0, class... Ts>
		void destruct()
		{
			reinterpret_cast<T0*>(m_buffer + offset)->~T0();
			destruct<offset + sizeof(T0), Ts...>();
		}

		template<int N, class... T>
		struct Param;

		template<class T0, class... T>
		struct Param<0, T0, T...>
		{
			typedef T0 Type;
		};

		template<int N, class T0, class... T>
		struct Param<N, T0, T...>
		{
			typedef typename Param<N - 1, T...>::Type Type;
		};

		static const unsigned COUNT = sizeof...(Types);
		char  m_buffer[sum<sizeof(Types)...>::value];
		char* m_ptrs[COUNT];
		const std::type_info* m_types[COUNT];
	};
}
