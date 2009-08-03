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

	template <int N, typename Real>
	class Polygon
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default destructor.

		Polygon<N, Real>& operator=(
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

	typedef Polygon<2, real> Polygon2;
	typedef Polygon<3, real> Polygon3;
	typedef Polygon<4, real> Polygon4;

}

#include "pastel/geometry/polygon.hpp"

#endif
