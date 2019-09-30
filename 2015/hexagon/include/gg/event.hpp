/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

/**
 * Events enable communication between processes or threads of a single
 * process. Once an event is created, its type and parameters cannot be
 * modified. This helps avoiding race conditions.
 *
 * The preferred way of defining an event is having an extern IEventDefinition
 * reference in a public header file and putting the implementation to a source
 * file:
 *
 * myevents.hpp
 * ------------
 * extern gg::IEventDefinition<int, char>& foo_event;
 *
 * myevents.cpp
 * ------------
 * static gg::SerializableEventDefinition<TYPE, int, char> _foo_event;
 * gg::IEventDefinition<int, char>& foo_event = _foo_event;
 *
 * main.cpp
 * --------
 * auto event = foo_event(1, 'a', 2.34f);
 * auto first = foo_event.get<0>(event); // return type is determined
 * auto second = event->get<char>(1); // must specify return type
 * auto third = event->get(third_tag); // where third_tag's type is gg::IEvent::Tag<2, float>
 * // ..do stuff..
 * thread->sendEvent(event);
 *
 * NOTE:
 * For implemented events and event definitions include one of the following
 * headers: gg/network.hpp or gg/thread.hpp
 */

#pragma once

#include <cstdint>
#include <memory>
#include "gg/serializable.hpp"
#include "gg/storage.hpp"

namespace gg
{
	class IEventDefinitionBase;

	class IEvent : public ISerializable
	{
	public:
		template<unsigned N, class T>
		class Tag
		{
		public:
			static const unsigned Param = N;
			typedef T Type;
		};

		typedef uint16_t Type;

		virtual ~IEvent() = default;
		virtual Type getType() const = 0;
		virtual const IStorage& getParams() const = 0;
		virtual void serialize(IArchive&) = 0;

		bool is(const IEventDefinitionBase&) const;

		template<class T>
		const T& get(unsigned n) const
		{
			return getParams().get<T>(n);
		}

		template<class Tag>
		const typename Tag::Type& get(Tag tag = {})
		{
			return getParams().get<typename Tag::Type>(Tag::Param);
		}

		template<class Tag>
		void get(const Tag*& tag)
		{
			tag = &getParams().get<typename Tag::Type>(Tag::Param);
		}
	};

	typedef std::shared_ptr<IEvent> EventPtr;

	class IEventDefinitionBase
	{
	public:
		virtual ~IEventDefinitionBase() = default;
		virtual IEvent::Type getType() const = 0;
		virtual EventPtr operator()() const = 0;
		virtual EventPtr operator()(IArchive&) const = 0;
	};

	template<class... Params>
	class IEventDefinition : public IEventDefinitionBase
	{
	public:
		virtual ~IEventDefinition() = default;
		virtual IEvent::Type getType() const = 0;
		virtual EventPtr operator()() const = 0;
		virtual EventPtr operator()(IArchive&) const = 0;
		virtual EventPtr operator()(Params... params) const = 0;

		template<unsigned N, class R = Param<N, Params...>::Type>
		static const R& get(EventPtr event)
		{
			return event->get<R>(N);
		}

		template<class Tag>
		static const typename Tag::Type& get(EventPtr event)
		{
			return event->get(Tag {});
		}

	private:
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
	};

	inline bool IEvent::is(const IEventDefinitionBase& def) const
	{
		return (getType() == def.getType());
	}


	template<class... Types>
	class SerializableStorage : public Storage<Types...>, public ISerializable
	{
	public:
		SerializableStorage() = default;
		SerializableStorage(Types... values) : Storage(std::forward<Types>(values)...) {}
		virtual ~SerializableStorage() = default;
		virtual void serialize(IArchive& packet) { serialize<0, Types...>(packet, *this); }

	private:
		template<size_t N>
		static void serialize(IArchive& packet, IStorage& storage)
		{
		}

		template<size_t N, class Type0, class... Types>
		static void serialize(IArchive& packet, IStorage& storage)
		{
			packet & storage.get<Type0>(N);
			serialize<N + 1, Types...>(packet, storage);
		}
	};

	template<IEvent::Type EventType, class... Params>
	class SerializableEvent : public IEvent
	{
	public:
		SerializableEvent() = default;

		SerializableEvent(Params... params) :
			m_params(std::forward<Params>(params)...)
		{
		}

		virtual ~SerializableEvent() = default;
		virtual Type getType() const { return EventType; }
		virtual const IStorage& getParams() const { return m_params; }
		virtual void serialize(IArchive& packet) { m_params.serialize(packet); }

	private:
		SerializableStorage<Params...> m_params;
	};

	template<IEvent::Type EventType, class... Params>
	class SerializableEventDefinition : public IEventDefinition<Params...>
	{
	public:
		typedef SerializableEvent<EventType, Params...> Event;

		virtual IEvent::Type getType() const
		{
			return EventType;
		}

		virtual EventPtr operator()() const
		{
			return EventPtr(new Event());
		}

		virtual EventPtr operator()(IArchive& ar) const
		{
			try
			{
				EventPtr event(new Event());
				event->serialize(ar);
				return event;
			}
			catch (ISerializationError&)
			{
				return {};
			}
		}

		virtual EventPtr operator()(Params... params) const
		{
			return EventPtr(new Event(std::forward<Params>(params)...));
		}
	};


	template<IEvent::Type EventType, class... Params>
	class LocalEvent : public IEvent
	{
	public:
		LocalEvent() = default;

		LocalEvent(Params... params) :
			m_params(std::forward<Params>(params)...)
		{
		}

		virtual ~LocalEvent() = default;
		virtual IEvent::Type getType() const { return EventType; }
		virtual const IStorage& getParams() const { return m_params; }
		virtual void serialize(IArchive&) {}

	private:
		Storage<Params...> m_params;
	};

	template<IEvent::Type EventType, class... Params>
	class LocalEventDefinition : public IEventDefinition<Params...>
	{
	public:
		typedef LocalEvent<EventType, Params...> Event;

		virtual IEvent::Type getType() const
		{
			return EventType;
		}

		virtual EventPtr operator()() const
		{
			return EventPtr(new Event());
		}

		virtual EventPtr operator()(IArchive& ar) const
		{
			return {};
		}

		virtual EventPtr operator()(Params... params) const
		{
			return EventPtr(new Event(std::forward<Params>(params)...));
		}
	};
};
