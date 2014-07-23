#ifndef PASTELSYS_RANDOM_TRIANGLE_HPP
#define PASTELSYS_RANDOM_TRIANGLE_HPP

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

		return std::sqrt(dy) - 1;
	}

	template <typename Real>
	Real randomTriangle(
		const PASTEL_NO_DEDUCTION(Real)& leftWidth,
		const PASTEL_NO_DEDUCTION(Real)& rightWidth)
	{
		PENSURE_OP(leftWidth, >, 0);
		PENSURE_OP(rightWidth, >, 0);

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

		// We assume:
		// mode = 0
		// min = -leftWidth
		// max = rightWidth

		const Real dy = (rightWidth + leftWidth) * random<Real>();
		
		if (dy >= leftWidth)
		{
			return rightWidth - std::sqrt((dy - leftWidth) * rightWidth);
		}

		return std::sqrt(leftWidth * dy) - leftWidth;
	}

	template <typename Real>
	Real trianglePdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		Real xAbs = mabs(x);
		if (xAbs > 1)
		{
			return 0;
		}

		return 1 - xAbs;
	}

	template <typename Real>
	Real trianglePdf(

		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& leftWidth,
		const PASTEL_NO_DEDUCTION(Real)& rightWidth)
	{
		PENSURE_OP(leftWidth, >, 0);
		PENSURE_OP(rightWidth, >, 0);

		if (x < -leftWidth || x > rightWidth)
		{
			return 0;
		}

		Real d = leftWidth + rightWidth;

		if (x < 0)
		{

			return 2 * (x + leftWidth) / (leftWidth * d);
		}

		return 2 * (rightWidth - x) / (rightWidth * d);
	}

}

#endif
