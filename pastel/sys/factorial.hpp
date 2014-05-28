#ifndef PASTELSYS_FACTORIAL_HPP
#define PASTELSYS_FACTORIAL_HPP

#include "pastel/sys/factorial.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	namespace Factorial_
	{

		static PASTEL_CONSTEXPR integer RealTableSize = 257;
		static PASTEL_CONSTEXPR integer IntegerTableSize = 21;

		//! Returns the logarithm of the factorial.
		/*!
		Preconditions:
		i >= 0
		*/
		PASTELSYS real64 lnFactorialReal64(integer i);

		//! Returns the factorial.
		/*!
		Preconditions:
		i <= 0 < 21

		It holds that 20! <= 2^64 - 1 < 21!.
		*/
		inline uint64 factorialUint64(integer i)
		{
			// factorial_[n] = n!
			// Computed with Mathematica.
			// It holds that 20! <= 2^64 - 1 < 21!.
			uint64 factorialSet[IntegerTableSize] =
			{
				1, 
				1, 
				2, 
				6, 
				24, 
				120, 
				720, 
				5040, 
				40320, 
				362880, 
				3628800, 
				39916800,
				479001600, 
				6227020800, 
				87178291200, 
				1307674368000, 
				20922789888000,
				355687428096000, 
				6402373705728000, 
				121645100408832000,
				2432902008176640000
			};

			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, IntegerTableSize);

			return factorialSet[i];
		}

	}

	template <typename Real>
	Real factorial(integer i)
	{
		PENSURE_OP(i, >=, 0);
		if (i < Factorial_::IntegerTableSize)
		{
			return Factorial_::factorialUint64(i);
		}

		return gamma<Real>(i + 1);
	}

	template <typename Real>
	Real lnFactorial(integer i)
	{
		PENSURE_OP(i, >=, 0);

		if (i < Factorial_::RealTableSize)
		{
			return Factorial_::lnFactorialReal64(i);
		}

		return lnGamma<Real>(i + 1);
	}

}

#endif
