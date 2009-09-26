#include "pastel/dsp/cubic_filter.h"

#include <cmath>

namespace Pastel
{

	// Cubic_Filter

	Cubic_Filter::Cubic_Filter(real negativeLobeness)
		: Filter(2, "cubic")
		, d_((negativeLobeness * 3) / 2)
	{
	}

	Cubic_Filter::~Cubic_Filter()
	{
	}

	real Cubic_Filter::evaluateInRange(real x) const
	{
		// Let
		// f(x) = ax^3 + bx^2 + cx + d
		// g(x) = a'x^3 + b'x^2 + c'x + d'
		//
		// Now require:
		// f(0) = 1
		// f'(0) = 0
		// f(1) = 0
		// f'(1) = g'(1)
		// g(1) = 0
		// g(2) = 0
		// g'(2) = 0
		//
		// These give seven equations for 8 unknowns.
		// Represent the equations in matrix form:
		// Mx = b
		// and then reduce
		// [M | b] to reduced row echelon form to
		// find the 1-parameter family of
		// cardinal cubic splines
		// with d' as the parameter.

		const real xAbs = mabs(x);

		if (xAbs < 1)
		{
			return
				((2 - d_) * xAbs +
				(-3 + d_)) * xAbs * xAbs + 1;
		}

		return
			(((-d_) * xAbs +
			5 * d_) * xAbs +
			(-8 * d_)) * xAbs + 4 * d_;
	}

}
