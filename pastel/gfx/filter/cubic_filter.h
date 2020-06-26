// Description: Cubic_Filter class
// Detail: Cardinal cubic spline reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_CUBIC_FILTER_H
#define PASTELGFX_CUBIC_FILTER_H

#include "pastel/gfx/filter.h"

#include <cmath>

namespace Pastel
{

	//! A cardinal cubic spline reconstruction filter.
	/*!
	Strictly speaking, the filter is a proper reconstruction
	filter only if the values of 'negativeLobeness' are
	in the range [0, 1]. However, we don't restrict the
	usage. This filter is interpolatory. It is equivalent to
	Mitchell_Filter(0, negativeLobeness * (3 / 2)). However,
	this implementation is more efficient.

	The default	value for 'negativeLobeness' comes from the
	experiments of Mitchell and Netravali where they
	concluded that the (subjectively) best filters lie
	on the line 2C + B = 1.	Here B = 0 and C = negativeLobeness * (3 / 2),
	from which the result follows. Coincidentally, this default
	parameter gives a filter that is almost identical to Lanczos_Filter(2).
	*/
	class Cubic_Filter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit Cubic_Filter(dreal negativeLobeness = (dreal)1 / 3)
		: Filter(2, "cubic")
		, d_((negativeLobeness * 3) / 2)
		{
		}

		virtual ~Cubic_Filter()
		{
		}

		virtual dreal evaluateInRange(dreal x) const
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

			dreal xAbs = std::abs(x);

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

	private:
		Cubic_Filter(const Cubic_Filter& that) = delete;
		Cubic_Filter& operator=(const Cubic_Filter& that) = delete;

		dreal d_;
	};

	using CubicFilterPtr = std::shared_ptr<Cubic_Filter>;
	using ConstCubicFilterPtr = std::shared_ptr<const Cubic_Filter>;

	inline CubicFilterPtr cubicFilter(dreal negativeLobeness = (dreal)1 / 3)
	{
		return std::make_shared<Cubic_Filter>(negativeLobeness);
	}

}

#endif
