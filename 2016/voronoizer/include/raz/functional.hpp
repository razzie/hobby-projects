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

#include <functional>
#include <stdexcept>
#include <type_traits>

namespace raz
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
};

namespace std
{
	template<int I>
	struct is_placeholder<raz::FunctionArgBinder::placeholder<I>> : std::integral_constant<int, I> {};
};
