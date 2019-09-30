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

/*
Original public domain code:
http://beej.us/guide/bgnet/examples/ieee754.c
*/

#pragma once

#include <cstdint>

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

#pragma warning(push)
#pragma warning(disable: 4244) // possible loss of data

namespace raz
{
	inline uint64_t pack754(long double f, unsigned bits, unsigned expbits)
	{
		long double fnorm;
		int shift;
		long long sign, exp, significand;
		unsigned significandbits = bits - expbits - 1; // -1 for sign bit

		if (f == 0.0) return 0; // get this special case out of the way

		// check sign and begin normalization
		if (f < 0) { sign = 1; fnorm = -f; }
		else { sign = 0; fnorm = f; }

		// get the normalized form of f and track the exponent
		shift = 0;
		while (fnorm >= 2.0) { fnorm /= 2.0; shift++; }
		while (fnorm < 1.0) { fnorm *= 2.0; shift--; }
		fnorm = fnorm - 1.0;

		// calculate the binary form (non-float) of the significand data
		significand = fnorm * ((1LL << significandbits) + 0.5f);

		// get the biased exponent
		exp = shift + ((1 << (expbits - 1)) - 1); // shift + bias

		// return the final answer
		return (sign << (bits - 1)) | (exp << (bits - expbits - 1)) | significand;
	}

	inline long double unpack754(uint64_t i, unsigned bits, unsigned expbits)
	{
		long double result;
		long long shift;
		unsigned bias;
		unsigned significandbits = bits - expbits - 1; // -1 for sign bit

		if (i == 0) return 0.0;

		// pull the significand
		result = (i&((1LL << significandbits) - 1)); // mask
		result /= (1LL << significandbits); // convert back to float
		result += 1.0f; // add the one back on

		// deal with the exponent
		bias = (1 << (expbits - 1)) - 1;
		shift = ((i >> significandbits)&((1LL << expbits) - 1)) - bias;
		while (shift > 0) { result *= 2.0; shift--; }
		while (shift < 0) { result /= 2.0; shift++; }

		// sign it
		result *= (i >> (bits - 1)) & 1 ? -1.0 : 1.0;

		return result;
	}
}

#pragma warning(pop)
