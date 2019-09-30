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
#include <tuple>
#include <utility>
#include "raz/callback.hpp"
#include "raz/serialization.hpp"

namespace raz
{
	typedef uint32_t EventType;

	template<unsigned N>
	struct EventNamedParam
	{
		static const unsigned ParamNumber = N;
	};

	template<EventType Type, class... Params>
	class Event : public std::tuple<Params...>
	{
	public:
		typedef Callback<Event> Callback;
		
		class CallbackSystem : public raz::CallbackSystem<Event>
		{
		public:
			static EventType getEventType()
			{
				return Event::getType();
			}

			template<class Serializer>
			void handleSerialized(Serializer& s)
			{
				Event e(s);
				(*this)(e);
			}
		};

		template<class _, class Serializer = EnableSerializer<_>>
		Event(Serializer& s) : std::tuple<Params...>()
		{
			if (s.getMode() == ISerializer::Mode::DESERIALIZE)
				serialize(s);
			else
				throw SerializationError();
		}

		Event(Params... params) : std::tuple<Params...>(std::forward<Params>(params)...)
		{
		}

		static EventType getType()
		{
			return Type;
		}

		template<size_t N>
		auto get() -> decltype(std::get<N>(*this))
		{
			return std::get<N>(*this);
		}

		template<size_t N>
		auto get() const -> decltype(std::get<N>(*this))
		{
			return std::get<N>(*this);
		}

		template<class NamedParam, size_t N = NamedParam::ParamNumber>
		auto get() -> decltype(std::get<N>(*this))
		{
			return std::get<N>(*this);
		}

		template<class NamedParam, size_t N = NamedParam::ParamNumber>
		auto get() const -> decltype(std::get<N>(*this))
		{
			return std::get<N>(*this);
		}

		template<class NamedParam, size_t N = NamedParam::ParamNumber>
		void get(NamedParam*& tag)
		{
			tag = &get<N>();
		}

		template<class NamedParam, size_t N = NamedParam::ParamNumber>
		void get(const NamedParam*& tag) const
		{
			tag = &get<N>();
		}

		template<class _, class Serializer = EnableSerializer<_>>
		void operator()(Serializer& s)
		{
			serialize(s);
		}

	private:
		template<class Serializer, size_t N = 0>
		void serialize(Serializer& s)
		{
			s(get<N>());
			if (N < sizeof...(Params)) serialize<Serializer, N + 1>(s);
		}
	};

	namespace literal
	{
		static constexpr uint64_t stringhash(const char* str, const uint64_t h = 5381)
		{
			return (str[0] == 0) ? h : stringhash(&str[1], h * 33 + str[0]);
		}

		constexpr EventType operator"" _event(const char* evt, size_t)
		{
			return (EventType)stringhash(evt);
		}
	}

	template<class... Events>
	class EventDispatcher // the given event callback systems must always live longer
	{
	public:
		EventDispatcher(typename Events::CallbackSystem&... callback_system) :
			m_callback_systems((&callback_system)...)
		{
		}

		template<class Event>
		void handle(const Event& e)
		{
			handleInternal<Event, 0, Events...>(e);
		}

		template<class Serializer>
		void handleSerialized(EventType type, Serializer& s)
		{
			handleSerializedInternal<Serializer, 0, Events...>(type, s);
		}

	protected:
		EventDispatcher(std::tuple<typename Events::CallbackSystem*...> callback_systems) :
			m_callback_systems(callback_systems)
		{
		}

	private:
		std::tuple<typename Events::CallbackSystem*...> m_callback_systems;

		template<class Event, size_t N>
		void handleInternal(const Event&)
		{
		}

		template<class Event, size_t N, class RegEvent0, class... RegEvents>
		std::enable_if_t<std::is_same<Event, RegEvent0>::value>
			handleInternal(const Event& e)
		{
			auto cb = std::get<N>(m_callback_systems);
			cb->handle(e);

			handleInternal<Event, N + 1, RegEvents...>(e);
		}

		template<class Event, size_t N, class RegEvent0, class... RegEvents>
		std::enable_if_t<!std::is_same<Event, RegEvent0>::value>
			handleInternal(const Event& e)
		{
			handleInternal<Event, N + 1, RegEvents...>(e);
		}

		template<class Serializer, size_t N>
		void handleSerializedInternal(EventType, Serializer&)
		{
		}

		template<class Serializer, size_t N, class RegEvent0, class... RegEvents>
		void handleSerializedInternal(EventType type, Serializer& s)
		{
			auto cb = std::get<N>(m_callback_systems);
			if (cb->getEventType() == type)
				cb->handleSerialized<Serializer>(s);

			handleSerializedInternal<Serializer, N + 1, RegEvents...>(type, s);
		}
	};

	template<class... Events>
	class EventSystem : public EventDispatcher<Events...>
	{
	public:
		EventSystem() :
			EventDispatcher(getCallbackSystemPointers())
		{
		}

		template<class Event>
		typename Event::CallbackSystem& getSystemByEvent(Event* = nullptr)
		{
			return std::get<typename Event::CallbackSystem>(m_callback_systems);
		}

		template<class Callback>
		typename Callback::ValueType::CallbackSystem& getSystemByCallback(Callback* = nullptr)
		{
			return std::get<typename Callback::ValueType::CallbackSystem>(m_callback_systems);
		}

	private:
		std::tuple<typename Events::CallbackSystem...> m_callback_systems;

		template<size_t... I>
		std::tuple<typename Events::CallbackSystem*...> getCallbackSystemPointers(std::index_sequence<I...>)
		{
			return std::tuple<typename Events::CallbackSystem*...>((&std::get<I>(m_callback_systems))...);
		}

		std::tuple<typename Events::CallbackSystem*...> getCallbackSystemPointers()
		{
			return getCallbackSystemPointers(std::make_index_sequence<sizeof...(Events)>());
		}
	};
}
