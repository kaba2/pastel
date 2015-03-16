#ifndef PASTELSYS_RATIONAL_SIMPLIFY_HPP
#define PASTELSYS_RATIONAL_SIMPLIFY_HPP

#include "pastel/sys/rational/rational.h"
#include "pastel/sys/integer/gcd.h"

namespace Pastel
{

	template <typename Integer>
	void Rational<Integer>::simplify()
	{
		if (!(zero(m_) &&
			zero(n_)))
		{
			// The number is not NaN, so
			// divide by the GCD(m, n)

			Integer theGcd(gcd(m_, n_));

			m_ /= theGcd;
			n_ /= std::move(theGcd);

			if (negative(n_))
			{
				m_ = -m_;
				n_ = -n_;
			}
		}
	}

}

#endif
