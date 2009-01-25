#ifndef PASTELMATH_PIECEWISELINEAR_H
#define PASTELMATH_PIECEWISELINEAR_H

#include <map>

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
		typedef std::map<Real, Point> Container;
		typedef typename Container::iterator Iterator;
		typedef typename Container::const_iterator ConstIterator;

		std::map<Real, Point> map_;
	};

}

#include "pastel/math/piecewiselinear.hpp"

#endif
