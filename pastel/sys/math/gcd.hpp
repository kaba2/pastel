#ifndef PASTELSYS_GCD_HPP
#define PASTELSYS_GCD_HPP

#include "pastel/sys/gcd.h"
#include "pastel/sys/number_tests.h"

namespace Pastel
{

	template <typename Integer>
	Integer extendedGcd(
		Integer a, Integer b,
		Integer& x, Integer& y)
	{
		x = 0;
		y = 1;
		Integer xLast = 1;
		Integer yLast = 0;

		while (b != 0)
		{
			Integer quotient = a / b;
	        
			Integer temp = b;
			b = a % b;
			a = temp;
	        
			temp = x;
			x = xLast - quotient * x;
			xLast = temp;
	        
			temp = y;
			y = yLast - quotient * y;
			yLast = temp;
		}

		x = xLast;
		y = yLast;

		return a;
	}

	template <typename Integer>
	Integer gcd(const Integer& left, const Integer& right)
	{
		// This is the binary gcd algorithm.
		// For integers 'left' and 'right' this function
		// actually computes GCD(abs(left), abs(right)).
		// GCD(0, 0) is not defined and results in an error.

		// The binary gcd algorithm works by applying
		// a set of reducing identities until
		// we reach a base case, which is either GCD(0, x) or
		// GCD(x, 0).
		// For the following, keep in mind that GCD is symmetric:
		// GCD(u, v) = GCD(v, u)

		ENSURE(!(zero(left) && zero(right)));

		// For native Integer types.
		using std::abs;

		Integer u = abs(left);
		Integer v = abs(right);

		// GCD(0, x) = GCD(x, 0) = x

		if (zero(u))
		{
			return v;
		}

		if (zero(v))
		{
			return u;
		}

		// If u and v are both even, then
		// GCD(u / 2, v / 2) = 2 * GCD(u, v)

		// Remove the common powers of 2 out of u and v.
		// Store the 'shift' for later multiplication by '2^shift'.

		integer shift = 0;

		while (even(u) && even(v))
		{
			u >>= 1;
			v >>= 1;
			++shift;
		}

		// Either u or v is (or both are) odd now.

		// If u is even and v is odd:
		// GCD(u / 2, v) = GCD(u, v)

		while (even(u))
		{
			u >>= 1;
		}

		// From here on, u is always odd.

		do
		{
			// If u is odd and v is even:
			// GCD(u, v / 2) = GCD(u, v)

			while (even(v))
			{
				v >>= 1;
			}

			// Now u and v are both odd, so 'u - v' is even.

			if (u < v)
			{
				// If u < v and u and v are both odd:
				// GCD(u, (v - u) / 2) = GCD(u, v)
				// The division by 2 is done later.

				v -= u;
			}
			else
			{
				// If u >= v and u and v are both odd:
				// GCD(v, (u - v) / 2) = GCD(u, v)
				// The division by 2 is done later.

				Integer difference(u - v);
				u = v;
				v = difference;
			}

			v >>= 1;
		}
		while (v != 0);

		// Multiply by the powers of 2 removed
		// in the beginning.

		return u << shift;
	}

}

#endif
