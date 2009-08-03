// Description: Polygon class
// Detail: Allows to work with polygons
// Documentation: shapes.txt

#ifndef PASTEL_POLYGON_H
#define PASTEL_POLYGON_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	template <typename Real, int N>
	class Polygon
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default destructor.

		Polygon<Real, N>& operator=(
			const Polygon& that);

		Point<Real, N>& operator[](integer index);
		const Point<Real, N>& operator[](integer index) const;

		void swap(Polygon& that);

		integer size() const;

		void pushBack(const Point<Real, N>& that);
		void popBack();

		void insert(integer index, const Point<Real, N>& that);
		void erase(integer index);

		void reserve(integer size);

	private:
		std::vector<Point<Real, N> > data_;
	};

	typedef Polygon<real, 2> Polygon2;
	typedef Polygon<real, 3> Polygon3;
	typedef Polygon<real, 4> Polygon4;

}

#include "pastel/geometry/polygon.hpp"

#endif
