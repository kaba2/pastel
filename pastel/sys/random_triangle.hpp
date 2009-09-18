#ifndef PASTEL_RANDOM_TRIANGLE_HPP
#define PASTEL_RANDOM_TRIANGLE_HPP

#include "pastel/sys/random_triangle.h"

namespace Pastel
{

	template <typename Real>
	Real randomTriangle()
	{
		// See the more general function
		// for derivation.

		const Real dy = 2 * random<Real>();
		
		if (dy >= 1)
		{
			return 1 - std::sqrt(dy - 1);
		}

		return sqrt(dy) - 1;
	}

	template <typename Real>
	Real randomTriangle(
		const PASTEL_NO_DEDUCTION(Real)& min,
		const PASTEL_NO_DEDUCTION(Real)& mode,
		const PASTEL_NO_DEDUCTION(Real)& max)
	{
		PENSURE_OP(min, <, mode);
		PENSURE_OP(mode, <, max);

		/*
		Denote
		m = mode - min
		n = max - mode
		d = max - min

		The area A of the triangle 
		(min, 0), (mode, h), (max, 0)
		is
		A = d * h / 2
		
		Restricting A = 1 gives
		1 = d * h / 2
		<=>
		h = 2 / d

		The pdf u(x) of the triangle distribution is
		as follows:

		If x < min:
		u(x) = 0

		If min <= x < mode:
		u(x) = h * (x - min) / m
		= 2 * (x - min) / (m d)
		
		If mode <= x <= max:
		u(x) = h * (max - x) / n
		= 2 * (max - x) / (n d)
		
		If x > max:
		u(x) = 0

		The cdf f(x) of the triangle distribution is
		as follows:

		If x < min:
		f(x) = 0

		If min <= x < mode:
		f(x) = (x - min) * (h * (x - min) / m) / 2
		= h * (x - min)^2 / (2m)
		= (h / 2) * ((x - min)^2 / m)
		= (1 / d) * ((x - min)^2 / m)

		If mode <= x <= max:
		f(x) = h * m / 2 + (max - x) * (h * (max - x) / n) / 2
		= h * m / 2 + h * (max - x)^2 / (2n)
		= (h / 2) (m + (max - x)^2 / n)
		= (1 / d) (m + (max - x)^2 / n)

		If x > max:
		f(x) = 1

		Invert f(x) in the domain [min, max]:

		If min <= x < mode:
		y = (1 / d) * ((x - min)^2 / m)
		<=>
		d m y = (x - min)^2
		<=>
		sqrt(d m y) = x - min
		<=>
		x = min + sqrt(d m y)

		If mode <= x <= max:
		y = (1 / d) (m + (max - x)^2 / n)
		<=>
		d y = m + (max - x)^2 / n
		<=>
		d y - m = (max - x)^2 / n
		<=>
		(d y - m) n = (max - x)^2
		<=>
		sqrt((d y - m) n) = max - x
		<=>
		x = max - sqrt((d y - m) n)
		*/

		const Real d = max - min;
		const Real m = mode - min;

		const Real dy = d * random<Real>();
		
		if (dy >= m)
		{
			return max - std::sqrt((dy - m) * (d - m));
		}

		return min + sqrt(m * dy);
	}

	template <typename Real>
	Real trianglePdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		const Real xAbs = mabs(x);
		if (xAbs > 1)
		{
			return 0;
		}

		return 1 - xAbs;
	}

	template <typename Real>
	Real trianglePdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& min,
		const PASTEL_NO_DEDUCTION(Real)& mode,
		const PASTEL_NO_DEDUCTION(Real)& max)
	{
		if (x < min || x > max)
		{
			return 0;
		}

		const Real d = max - min;

		if (x < mode)
		{
			return 2 * (x - min) / ((mode - min) * d);
		}

		return 2 * (max - x) / ((max - mode) * d);
	}

}

#endif
