/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <stdexcept>
#include <typeinfo>

namespace gg
{
	class IStorage
	{
	public:
		virtual ~IStorage() = default;
		virtual unsigned count() const = 0; // number of elements
		virtual char* getPtr(unsigned) = 0;
		virtual const char* getPtr(unsigned) const = 0;
		virtual const std::type_info& getType(unsigned) const = 0;

		template<class T>
		T& get(unsigned n)
		{
			if (typeid(T) != getType(n)) throw std::bad_cast();
			return *reinterpret_cast<T*>(getPtr(n));
		}

		template<class T>
		const T& get(unsigned n) const
		{
			if (typeid(T) != getType(n)) throw std::bad_cast();
			return *reinterpret_cast<const T*>(getPtr(n));
		}
	};

	template<class... Types>
	class Storage : public IStorage
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

		virtual ~Storage()
		{
			destruct<0, Types...>();
		}

		virtual unsigned count() const
		{
			return COUNT;
		}

		virtual char* getPtr(unsigned n)
		{
			if (n >= COUNT) throw std::out_of_range({});
			return m_ptrs[n];
		}

		virtual const char* getPtr(unsigned n) const
		{
			if (n >= COUNT) throw std::out_of_range({});
			return m_ptrs[n];
		}

		virtual const std::type_info& getType(unsigned n) const
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

		static const unsigned COUNT = sizeof...(Types);
		char  m_buffer[sum<sizeof(Types)...>::value];
		char* m_ptrs[COUNT];
		const std::type_info* m_types[COUNT];
	};
};
