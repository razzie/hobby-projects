/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <cstdint>
#include <iterator>
#include <string>
#include <type_traits>

namespace gg
{
	class ISerializable;

	class IArchive
	{
		template<class T>
		class HasSerializer
		{
			static IArchive& ar;
			static T& t;

			template<class U, class R = decltype(::serialize(ar, t))>
			static std::true_type test(void*);

			template<class>
			static std::false_type test(...);

		public:
			static constexpr bool value = decltype(test<T>(nullptr))::value;
		};

		template<class T>
		class IsContainer
		{
			typedef char                      Yes;
			typedef struct { char array[2]; } No;

			// test C::iterator
			template<class C>
			static constexpr Yes test_iterator(typename C::iterator*);

			template<class C>
			static constexpr No  test_iterator(...);

			static const bool has_iterator = (sizeof(test_iterator<T>(0)) == sizeof(Yes));

			// test C::begin()
			template<class C>
			using begin_signatire_test =
				std::is_same<decltype(&C::begin), typename C::iterator(C::*)()>;

			template<class C>
			static constexpr Yes(&test_begin(std::enable_if_t<begin_signatire_test<C>::value>*));

			template<class C>
			static constexpr No(&test_begin(...));

			static constexpr bool has_begin = (sizeof(test_begin<T>(0)) == sizeof(Yes));

			// test C::end()
			template<class C>
			using end_signatire_test =
				std::is_same<decltype(&C::begin), typename C::iterator(C::*)()>;

			template<class C>
			static constexpr Yes(&test_end(std::enable_if_t<end_signatire_test<C>::value>*));

			template<class C>
			static constexpr No(&test_end(...));

			static constexpr bool has_end = (sizeof(test_end<T>(0)) == sizeof(Yes));

		public:
			static constexpr bool value = has_iterator && has_begin && has_end;
		};

		template<class>
		struct IsStdPair : public std::false_type
		{
		};

		template<class T1, class T2>
		struct IsStdPair<std::pair<T1, T2>> : public std::true_type
		{
		};

	public:
		enum Mode
		{
			SERIALIZE,
			DESERIALIZE
		};

		virtual ~IArchive() = default;
		virtual Mode getMode() const = 0;
		virtual IArchive& operator& (int8_t&) = 0;
		virtual IArchive& operator& (int16_t&) = 0;
		virtual IArchive& operator& (int32_t&) = 0;
		virtual IArchive& operator& (int64_t&) = 0;
		virtual IArchive& operator& (uint8_t&) = 0;
		virtual IArchive& operator& (uint16_t&) = 0;
		virtual IArchive& operator& (uint32_t&) = 0;
		virtual IArchive& operator& (uint64_t&) = 0;
		virtual IArchive& operator& (float&) = 0;
		virtual IArchive& operator& (double&) = 0;
		virtual IArchive& operator& (std::string&) = 0;
		virtual IArchive& operator& (ISerializable&) = 0;
		virtual size_t write(const char* ptr, size_t len) = 0;
		virtual size_t read(char* ptr, size_t len) = 0;

		template<class T>
		std::enable_if_t<HasSerializer<T>::value, IArchive&>
			operator& (T& t)
		{
			::serialize(*this, t);
			return *this;
		}

		template<class Container>
		std::enable_if_t<!HasSerializer<Container>::value && IsContainer<Container>::value, IArchive&>
			operator& (Container& cont)
		{
			if (getMode() == Mode::SERIALIZE)
			{
				uint16_t size = static_cast<uint16_t>(cont.size());
				(*this) & size;
				for (Container::value_type& val : cont)
				{
					(*this) & val;
				}
			}
			else
			{
				uint16_t size;
				(*this) & size;
				for (uint16_t i = 0; i < size; ++i)
				{
					Container::value_type val;
					(*this) & val;
					*std::inserter(cont, cont.end()) = std::move(val);
				}
			}
			return *this;
		}

		template<class T>
		std::enable_if_t<IsStdPair<T>::value, IArchive&>
			operator& (T& pair)
		{
			using first_type = std::remove_const_t<typename T::first_type>;
			return (*this) & const_cast<first_type&>(pair.first) & pair.second;
		}
	};

	class ISerializationError : public std::exception
	{
	public:
		virtual ~ISerializationError() = default;
		virtual const char* what() const = 0;
	};

	class ISerializable
	{
	public:
		virtual ~ISerializable() = default;
		virtual void serialize(IArchive&) = 0;
	};
};
