/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <functional>
#include <stdexcept>
#include <type_traits>
#include "gg/any.hpp"

namespace gg
{
	template<class F>
	class LambdaSignature
	{
		template<class T>
		struct RemoveClass { };

		template<class C, class R, class... Params>
		struct RemoveClass<R(C::*)(Params...)>
		{
			using Type = std::remove_pointer_t<R(*)(Params...)>;
		};

		template<class C, class R, class... Params>
		struct RemoveClass<R(C::*)(Params...) const>
		{
			using Type = std::remove_pointer_t<R(*)(Params...)>;
		};

		template<class T>
		struct GetSignatureImpl
		{
			using Type = typename RemoveClass<
				decltype(&std::remove_reference_t<T>::operator())>::Type;
		};

		template<class R, class... Params>
		struct GetSignatureImpl<R(*)(Params...)>
		{
			using Type = std::remove_pointer_t<R(*)(Params...)>;
		};

	public:
		using Type = typename GetSignatureImpl<F>::Type;
	};

	template<class F>
	using GetLambdaSignature = typename LambdaSignature<F>::Type;


	class FunctionArgBinder
	{
	public:
		template<int I>
		struct placeholder {};

	private:
		template<std::size_t... Is, class F, class... Args>
		static auto _bind(std::index_sequence<Is...>, F const& f, Args&&... args)
			-> decltype(std::bind(f, std::forward<Args>(args)..., placeholder<Is + 1>{}...))
		{
			return std::bind(f, std::forward<Args>(args)..., placeholder<Is + 1>{}...);
		}

		template<class R, class... FArgs, class... Args>
		static auto _bind(std::function<R(FArgs...)> const& f, Args&&... args)
			-> decltype(_bind(std::make_index_sequence<sizeof...(FArgs)-sizeof...(Args)>{}, f, std::forward<Args>(args)...))
		{
			return _bind(std::make_index_sequence<sizeof...(FArgs)-sizeof...(Args)>{}, f, std::forward<Args>(args)...);
		}

		template<class R, class C, class... Args>
		static std::function<R(C*, Args...)> convert(R(C::*func)(Args...))
		{
			return{ std::mem_fn(func) };
		}

		template<class R, class C, class... Args>
		static std::function<R(const C*, Args...)> convert(R(C::*func)(Args...) const)
		{
			return{ std::mem_fn(func) };
		}

	public:
		template<class F, class... Args>
		auto operator()(F const& f, Args&&... args)
			-> decltype(_bind(convert(f), std::forward<Args>(args)...))
		{
			return _bind(convert(f), std::forward<Args>(args)...);
		}
	};

	template<class F, class... Args>
	auto bind(F const& f, Args&&... args)
		-> decltype(FunctionArgBinder()(f, std::forward<Args>(args)...))
	{
		return FunctionArgBinder()(f, std::forward<Args>(args)...);
	}


	class Function
	{
	public:
		Function()
		{
		}

		template<class F>
		Function(F func) :
			m_func(convert(func))
		{
		}

		Function(const Function& func) :
			m_func(func.m_func)
		{
		}

		Function(Function&& func) :
			m_func(std::move(func.m_func))
		{
		}

		template<class F>
		Function& operator=(F func)
		{
			m_func = convert(func);
			return *this;
		}

		Function& operator=(const Function& func)
		{
			m_func = func.m_func;
			return *this;
		}

		Function& operator=(Function&& func)
		{
			m_func = std::move(func.m_func);
			return *this;
		}

		Any operator()(const Any::Array& ar) const
		{
			return m_func(ar);
		}

		template<class... Params>
		Any operator()(Params... params) const
		{
			return m_func({ std::forward<Params>(params)... });
		}

		operator bool() const
		{
			return static_cast<bool>(m_func);
		}
	
	private:
		template<class R>
		static R call(std::function<R()> func, const gg::Any::Array& ar, unsigned skipParams = 0)
		{
			if (ar.size() > skipParams)
				throw std::runtime_error("too many parameters");

			return func();
		}

		template<class R, class Param0, class... Params>
		static R call(std::function<R(Param0, Params...)> func, const gg::Any::Array& ar, unsigned skipParams = 0)
		{
			if (ar.size() <= skipParams)
				throw std::runtime_error("not enough parameters");

			Param0 param0;
			if (!ar[skipParams].cast<Param0>(param0))
				throw std::runtime_error("failed parameter conversion");

			std::function<R(Params...)> lambda =
				[&](Params... params) -> R { return func(param0, params...); };

			return call(lambda, ar, skipParams + 1);
		}


		template<class R, class... Params>
		static std::function<Any(Any::Array)> convert(std::function<R(Params...)> func)
		{
			return ([=](const Any::Array& ar) -> Any { return call(func, ar); });
		}

		template<class... Params>
		static std::function<Any(Any::Array)> convert(std::function<void(Params...)> func)
		{
			return ([=](const Any::Array& ar) -> Any { call(func, ar); return{}; });
		}

		template<class F>
		static std::function<Any(Any::Array)> convert(F func)
		{
			return convert(std::function<GetLambdaSignature<F>>(func));
		}


		std::function<Any(Any::Array)> m_func;
	};
};

namespace std
{
	template<int I>
	struct is_placeholder<gg::FunctionArgBinder::placeholder<I>> : std::integral_constant<int, I> {};
};
