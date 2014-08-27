// Description: Mitchell_Filter class
// Detail: Mitchell-Netravali cubic spline reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_MITCHELL_FILTER_H
#define PASTELGFX_MITCHELL_FILTER_H

#include "pastel/gfx/filter.h"

#include <cmath>

namespace Pastel
{

	//! A Mitchell-Netravali cubic spline reconstruction filter.
	/*
	For information on this filter, see:
	"Reconstruction Filters in Computer Graphics",
	Don P. Mitchell, Arun N. Netravali, 1988.

	This filter is interpolatory if and only if B = 0.
	However, if you want interpolation, then consider using
	Cubic_Filter which is specialized for this case and
	is thus more efficient.

	The default	values comes from the
	experiments of Mitchell and Netravali where they
	concluded that the (subjectively) best filters lie
	on the line 2C + B = 1. They further deemed
	(B = 1/3, C = 1/3) as the best from this line.
	*/
	class Mitchell_Filter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit Mitchell_Filter(
			real b = (real)1 / 3,
			real c = (real)1 / 3)
		: Filter(2, "mitchell")
		, b_(b)
		, c_(c)
		{
		}
		
		virtual ~Mitchell_Filter()
		{
		}

		virtual real evaluateInRange(real x) const
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

			real xAbs = mabs(x);

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

	private:
		Mitchell_Filter(const Mitchell_Filter& that) = delete;
		Mitchell_Filter& operator=(const Mitchell_Filter& that) = delete;

		real b_;
		real c_;
	};

	using MitchellFilterPtr = std::shared_ptr<Mitchell_Filter>;
	using ConstMitchellFilterPtr = std::shared_ptr<const Mitchell_Filter>;

	inline MitchellFilterPtr mitchellFilter(
		real b = (real)1 / 3,
		real c = (real)1 / 3)
	{
		return std::make_shared<Mitchell_Filter>(b, c);
	}

}

#endif
