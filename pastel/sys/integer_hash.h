// Description: Hashing for fixed-sized integers

#ifndef PASTELSYS_INTEGER_HASH_H
#define PASTELSYS_INTEGER_HASH_H

#include "pastel/sys/integer.h"

namespace Pastel
{

	template <typename Integer_Settings>
	struct Integer_Hash
	{
	public:
		enum
		{
			N = Integer_Settings::N
		};

		explicit Integer_Hash(integer beginBit = 0, integer endBit = N)
		: beginBit_(beginBit)
		, endBit_(endBit)
		{
			PENSURE_OP(beginBit, >=, 0);
			PENSURE_OP(beginBit, <=, endBit);
			PENSURE_OP(endBit, <=, N);
		}

		hash_integer operator()(
			const Integer<Integer_Settings>& that) const
		{
			return that.hash(beginBit_, endBit_);
		}

	private:
		integer beginBit_;
		integer endBit_;
	};

}

#include "pastel/sys/hashing.h"

namespace std
{

	template <typename Integer_Settings>
	struct hash<Pastel::Integer<Integer_Settings>>
	: Pastel::Integer_Hash<Integer_Settings>
	{
	};

}

#endif
