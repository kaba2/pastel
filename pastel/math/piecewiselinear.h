// Description: PiecewiseLinear class
// Detail: Allows to work with piecewise linear functions
// Documentation: interpolation.txt

#ifndef PASTEL_PIECEWISELINEAR_H
#define PASTEL_PIECEWISELINEAR_H

#include "pastel/sys/smallmap.h"

namespace Pastel
{

	template <typename Real, typename Vector = Real>
	class PiecewiseLinear
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		void insert(const Real& time,
			const Vector& value);

		void clear();

		Vector operator()(const Real& time) const;

	private:
		typedef SmallMap<Real, Vector> Container;

		Container map_;
	};

}

#include "pastel/math/piecewiselinear.hpp"

#endif
