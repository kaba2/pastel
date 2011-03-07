#ifndef PASTEL_BINOMIAL_HPP
#define PASTEL_BINOMIAL_HPP

#include "pastel/sys/binomial.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real>
	Real binomial(integer i, integer n)
	{
		// i <= n
		// binomial(i, n) = n! / [(n - i)!i!]
		// = prod[a = n-i+1...n](a) / prod[a = 1...i](a)

		// So the computation takes 2*i + 1 operations.
		// Because binomial(i, n) == binomial(n - i, n),
		// we can minimize the amount of operations by
		// computing the one with the smaller second operand.

		// binomial(i, n) = prod[a = 0...i-1]((n - a)/(i - a))
		// For accuracy, interleave the multiplications and
		// divisions.

		PENSURE_OP(n, >=, 0);

		if (i < 0 || i > n)
		{
			return 0;
		}

		if (n - i < i)
		{
			i = n - i;
		}

		if (i == 0 || n == 0)
		{
			return 1;
		}

		Real b(1);
		Real c(n);
		Real d(i);

		for (integer a = 0;a < i;++a)
		{
			b *= c;
			b /= d;
			--c;
			--d;
		}

		return b;
	}

}

#endif
