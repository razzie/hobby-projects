/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <iosfwd>
#include <locale>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace gg
{
	template<class T>
	class StreamManipulator
	{
	public:
		typedef std::ios&(*Manipulator)(std::ios&, T);

		StreamManipulator(Manipulator m, T t) :
			m_manip(m), m_value(t)
		{
		}

		friend std::ios& operator<< (std::ios& io, const StreamManipulator<T>& m)
		{
			return m.m_manip(io, m.m_value);
		}

	private:
		Manipulator m_manip;
		T m_value;
	};

	template<class T>
	class IstreamManipulator
	{
	public:
		typedef std::istream&(*Manipulator)(std::istream&, T);

		IstreamManipulator(Manipulator m, T t) :
			m_manip(m), m_value(t)
		{
		}

		friend std::istream& operator<< (std::istream& io, const IstreamManipulator<T>& m)
		{
			return m.m_manip(io, m.m_value);
		}

	private:
		Manipulator m_manip;
		T m_value;
	};

	template<class T>
	class OstreamManipulator
	{
	public:
		typedef std::ostream&(*Manipulator)(std::ostream&, T);

		OstreamManipulator(Manipulator m, T t) :
			m_manip(m), m_value(t)
		{
		}

		friend std::ostream& operator<< (std::ostream& io, const OstreamManipulator<T>& m)
		{
			return m.m_manip(io, m.m_value);
		}

	private:
		Manipulator m_manip;
		T m_value;
	};


	template<class T>
	OstreamManipulator<T> hex(T t)
	{
		OstreamManipulator<T>::Manipulator m =
			[](std::ostream& o, T t) -> std::ostream&
			{
				const char map[] = "0123456789abcdef";
				const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&t);
				for (size_t i = 0; i < sizeof(T); ++i)
				{
					unsigned first = ptr[i] / 16;
					unsigned second = ptr[i] - (first * 16);
					o << map[first] << map[second];
				}
				return o;
			};

		return OstreamManipulator<T>(m, t);
	}

	inline IstreamManipulator<char> delimiter(char d)
	{
		struct CustomLocale : std::ctype<char>
		{
			std::ctype_base::mask m_rc[table_size];

			CustomLocale(char delim) :
				std::ctype<char>(get_table(static_cast<unsigned char>(delim)))
			{
			}

			const std::ctype_base::mask* get_table(unsigned char delim)
			{
				memset(m_rc, 0, sizeof(std::ctype_base::mask) * table_size);
				m_rc[delim] = std::ctype_base::space;
				m_rc['\n'] = std::ctype_base::space;
				return &m_rc[0];
			}
		};

		IstreamManipulator<char>::Manipulator m =
			[](std::istream& i, char d) -> std::istream&
			{
				i.imbue(std::locale(i.getloc(), new CustomLocale(d)));
				return i;
			};

		return IstreamManipulator<char>(m, d);
	}

	inline IstreamManipulator<char> next(char d)
	{
		IstreamManipulator<char>::Manipulator m =
			[](std::istream& i, char d) -> std::istream&
			{
#pragma push_macro("max")
#undef max
				i.ignore(std::numeric_limits<std::streamsize>::max(), d);
#pragma pop_macro("max")
				return i;
			};

		return IstreamManipulator<char>(m, d);
	}

	inline std::istream& nextLine(std::istream& i)
	{
		return (i << next('\n'));
	}


	template<class Param>
	std::tuple<Param> parse(std::istream& i, char* d = nullptr)
	{
		Param p;
		if (d) i << delimiter(*d);
		if (!static_cast<bool>(i >> extract(p)))
			throw std::logic_error("parse error: " + typeid(Param).name());
		return std::tuple<Param> { p };
	}

	template<class Param1, class Param2, class... Params>
	std::tuple<Param1, Param2, Params...> parse(std::istream& i, char* d = nullptr)
	{
		if (d) i << delimiter(*d);
		auto a = parse<Param1>(i);
		auto b = parse<Param2, Params...>(i);
		return std::tuple_cat(a, b);
	}

	template<class... Params>
	std::tuple<Params...> parse(std::string str, char* d = nullptr)
	{
		std::stringstream ss(str);
		if (d) ss << delimiter(*d);
		return parse<Params...>(ss);
	}


	class Any;

	namespace __fallback
	{
		template<class T, class = std::enable_if_t<std::is_same<T, Any>::value>>
		std::false_type operator<< (std::ostream&, const T&);

		template<class T>
		std::true_type operator>> (std::istream&, T&);
	}

	template<class T>
	class HasStreamInserter
	{
		static std::true_type  test(std::ostream&);
		static std::false_type test(...);

		static std::ostream &s;
		static std::remove_reference_t<T>& t;

		static constexpr bool check()
		{
			using namespace __fallback;
			return decltype(test(s << t))::value;
		}

	public:
		static bool const value = check();
	};

	template<class T>
	class HasStreamExtractor
	{
		static std::true_type  test(std::istream&);
		static std::false_type test(...);

		static std::istream &s;
		static std::remove_reference_t<T>& t;

		static constexpr bool check()
		{
			using namespace __fallback;
			return decltype(test(s >> t))::value;
		}

	public:
		static const bool value = check();
	};


	template<class T>
	class StreamInsert
	{
		template<class U, bool>
		struct Insert
		{
			static std::ostream& apply(std::ostream& s, const U& value)
			{
				s << value;
				return s;
			}
		};

		template<class U>
		struct Insert<U, false>
		{
			static std::ostream& apply(std::ostream& s, const U& value)
			{
				s << typeid(T).name();
				return s;
			}
		};

	public:
		static std::ostream& apply(std::ostream& s, const T& value)
		{
			return Insert<T, HasStreamInserter<T>::value>::apply(s, value);
		}

		class Wrapper
		{
		public:
			Wrapper(const T& value) : m_value(value)
			{
			}

			friend std::ostream& operator<< (std::ostream& s, const Wrapper& w)
			{
				return apply(s, w.m_value);
			}

		private:
			const T& m_value;
		};
	};

	template<class T>
	typename StreamInsert<T>::Wrapper insert(const T& value)
	{
		return typename StreamInsert<T>::Wrapper(value);
	}


	template<class T>
	class StreamExtract
	{
		template<class U, bool>
		struct Extract
		{
			static std::istream& apply(std::istream& s, U& value)
			{
				s >> value;
				return s;
			}
		};

		// special case for std::string
		template<>
		struct Extract<std::string, true>
		{
			static std::istream& apply(std::istream& s, std::string& value)
			{
				std::getline(s, value);
				return s;
			}
		};

		template<class U>
		struct Extract<U, false>
		{
			static std::istream& apply(std::istream& s, U& value)
			{
				return s;
			}
		};

	public:
		static std::istream& apply(std::istream& s, T& value)
		{
			return Extract<T, HasStreamExtractor<T>::value>::apply(s, value);
		}

		class Wrapper
		{
		public:
			Wrapper(T& value) : m_value(value)
			{
			}

			friend std::istream& operator>> (std::istream& s, Wrapper& w)
			{
				return apply(s, w.m_value);
			}

		private:
			T& m_value;
		};
	};

	template<class T>
	typename StreamExtract<T>::Wrapper extract(T& value)
	{
		return typename StreamExtract<T>::Wrapper(value);
	}
};
