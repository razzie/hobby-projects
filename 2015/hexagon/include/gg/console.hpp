/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include "gg/any.hpp"
#include "gg/function.hpp"

#if defined GGCONSOLE_BUILD
#	define GG_API __declspec(dllexport)
#else
#	define GG_API __declspec(dllimport)
#endif

namespace gg
{
	class IConsole : public virtual std::ostream
	{
	public:
		virtual ~IConsole() = default;

		// should be called BEFORE initializing OpenGL or Direct3D context
		virtual bool init() = 0;

		// add a function which can be used inside the console
		virtual bool addFunction(const std::string& fname, Function func, Any::Array&& defaults) = 0;

		template<class F>
		bool addFunction(const std::string& fname, F func)
		{
			Any::Array va;
			SignatureParams<GetLambdaSignature<F>>::setDefaults(va);
			return addFunction(fname, func, std::move(va));
		}

		// completes the missing parts of 'expression' (if possible) and returns cursor position
		virtual unsigned complete(std::string& expression, unsigned cursor_start = 0) const = 0;

		// if returned true: 'expression' was evaluated and its value was copied to 'val'
		// if returned false: 'val' contains the error message as an 'std::string'
		virtual bool exec(const std::string& expression, Any* val = nullptr) const = 0;

		// clears existing output lines
		virtual void clear() = 0;

	protected:
		IConsole() : std::ostream(nullptr) {}

		template<class...>
		class SignatureParams;

		template<class R, class... Params>
		class SignatureParams<R(Params...)>
		{
		public:
			static void setDefaults(Any::Array& ar)
			{
				setDefaultsImpl<0, Params...>(ar);
			}

		private:
			template<int /* placeholder */, class P0, class... Ps>
			static void setDefaultsImpl(gg::Any::Array& ar)
			{
				if (std::is_integral<P0>::value)
					ar.push_back(std::string("0"));
				else if (std::is_floating_point<P0>::value)
					ar.push_back(std::string("0.0"));
				else if (std::is_same<P0, gg::Any::Array>::value)
					ar.push_back(std::string("()"));
				else
					ar.push_back(std::string("\"\""));

				setDefaultsImpl<0, Ps...>(ar);
			}

			template<int>
			static void setDefaultsImpl(gg::Any::Array&) {}
		};
	};

	extern GG_API IConsole& console;
};
