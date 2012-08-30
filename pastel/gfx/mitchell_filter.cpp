#include "pastel/gfx/mitchell_filter.h"

#include <cmath>

namespace Pastel
{

	// Mitchell_Filter

	Mitchell_Filter::Mitchell_Filter(real b, real c)
		: Filter(2, "mitchell")
		, b_(b)
		, c_(c)
	{
	}

	Mitchell_Filter::~Mitchell_Filter()
	{
	}

	real Mitchell_Filter::evaluateInRange(real x) const
	{
		// Let
		// f(x) = ax^3 + bx^2 + cx + d
		// g(x) = a'x^3 + b'x^2 + c'x + d'
		//
		// Now require:
		// f'(0) = 0
		// f'(1) = g'(1)
		// g(2) = 0
		// for all x: sum[i=-oo..oo] (f(x) + f(1 - x) + g(x + 1) + g(2 - x)) = 1
		//
		// For the last equation, one gets four equations from the fact that
		// if a polynomial is to be zero for all x, then it must be the zero
		// polynomial which means that the coefficients must be zero.
		//
		// These give seven equations for 8 unknowns.
		// Represent the equations in matrix form:
		// Mx = b
		// and then reduce
		// [M | b] to reduced row echelon form to
		// find the 2-parameter family of
		// Mitchell-Netravali cubic splines
		// with B and C as the parameter.

		const real xAbs = mabs(x);

		if (xAbs < 1)
		{
			return ((12 - 9 * b_ - 6 * c_) * xAbs +
				(-18 + 12 * b_ + 6 * c_)) * xAbs * xAbs +
				(6 - 2 * b_);
		}

		return (((-b_ - 6 * c_) * xAbs +
			(6 * b_ + 30 * c_)) * xAbs +
			(-12 * b_ - 48 * c_)) * xAbs +
			(8 * b_ + 24 * c_);
	}

}
