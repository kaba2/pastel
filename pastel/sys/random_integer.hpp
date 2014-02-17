#ifndef PASTELSYS_RANDOM_INTEGER_HPP
#define PASTELSYS_RANDOM_INTEGER_HPP

#include "pastel/sys/random_integer.h"
#include "pastel/sys/mt19937.h"

namespace Pastel
{

	namespace Random_
	{

		template <integer Bits>
		class RandomInteger
		{
		};

		template <>
		class RandomInteger<32>
		{
		public:
			uinteger operator()() const
			{
				return Pastel::randomUint32();
			}
		};

		template <>
		class RandomInteger<64>
		{
		public:
			uinteger operator()() const
			{
				return Pastel::randomUint64();
			}
		};
	}

	inline uinteger randomUinteger()
	{
		return Random_::RandomInteger<sizeof(uinteger)* CHAR_BIT>()();
	}

	inline integer randomInteger()
	{
		return (integer)(randomUinteger() >> 1);
	}

	inline integer randomInteger(integer min, integer max)
	{
		// FIX: This is not uniformly distributed!
		return min + (randomInteger() % (max - min + 1));
	}

}

#endif
