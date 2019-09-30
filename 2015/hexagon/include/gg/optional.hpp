/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace gg
{
	template<class T>
	class Optional
	{
	public:
		Optional() :
			m_engaged(false)
		{
		}

		Optional(Optional&& o) :
			m_engaged(o.m_engaged)
		{
			if (m_engaged)
			{
				new (m_object) T(std::move(*o));

				reinterpret_cast<T*>(m_object)->~T();
				o.m_engaged = false;
			}
		}

		Optional(const Optional& o) :
			m_engaged(o.m_engaged)
		{
			if (m_engaged)
				new (m_object) T(*o);
		}

		template<class... Args>
		Optional(Args... args) :
			m_engaged(true)
		{
			new (m_object) T(std::forward<Args>(args)...);
		}

		~Optional()
		{
			if (m_engaged)
				reinterpret_cast<T*>(m_object)->~T();
		}

		Optional& operator=(const Optional& o)
		{
			if (m_engaged)
				reinterpret_cast<T*>(m_object)->~T();

			m_engaged = o.m_engaged;
			if (m_engaged)
				new (m_object) T(*o);

			return *this;
		}

		Optional& operator=(Optional&& o)
		{
			if (m_engaged)
				reinterpret_cast<T*>(m_object)->~T();

			m_engaged = o.m_engaged;
			if (m_engaged)
			{
				new (m_object) T(std::move(*o));

				reinterpret_cast<T*>(m_object)->~T();
				o.m_engaged = false;
			}

			return *this;
		}

		T& operator*()
		{
			if (!m_engaged)
				throw std::logic_error("bad Optional access");
			return *reinterpret_cast<T*>(m_object);
		}

		const T& operator*() const
		{
			if (!m_engaged)
				throw std::logic_error("bad Optional access");
			return *reinterpret_cast<const T*>(m_object);
		}

		T* operator->()
		{
			if (!m_engaged)
				throw std::logic_error("bad Optional access");
			return reinterpret_cast<T*>(m_object);
		}

		const T* operator->() const
		{
			if (!m_engaged)
				throw std::logic_error("bad Optional access");
			return reinterpret_cast<T*>(m_object);
		}

		operator bool() const
		{
			return m_engaged;
		}

		T& value()
		{
			if (!m_engaged)
				throw std::logic_error("bad Optional access");
			return *reinterpret_cast<T*>(m_object);
		}

		const T& value() const
		{
			if (!m_engaged)
				throw std::logic_error("bad Optional access");
			return *reinterpret_cast<const T*>(m_object);
		}

		template<class U>
		T& valueOr(U& default_value)
		{
			if (m_engaged)
				return *this;
			else
				return default_value;
		}

		template<class U>
		const T& valueOr(const U& default_value) const
		{
			if (m_engaged)
				return *this;
			else
				return default_value;
		}

	private:
		char m_object[sizeof(T)];
		bool m_engaged;
	};
};
