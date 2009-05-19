#ifndef PASTEL_PIECEWISELINEAR_H
#define PASTEL_PIECEWISELINEAR_H

#include "pastel/sys/smallmap.h"

namespace Pastel
{

	template <typename Real, typename Point = Real>
	class PiecewiseLinear
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		void insert(const Real& time,
			const Point& value);

		void clear();

		Point operator()(const Real& time) const;

	private:
		typedef SmallMap<Real, Point> Container;

		Container map_;
	};

}

#include "pastel/math/piecewiselinear.hpp"

#endif
