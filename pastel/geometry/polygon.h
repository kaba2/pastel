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

		Point<N, Real>& operator[](integer index);
		const Point<N, Real>& operator[](integer index) const;

		void swap(Polygon& that);

		integer size() const;

		void pushBack(const Point<N, Real>& that);
		void popBack();

		void insert(integer index, const Point<N, Real>& that);
		void erase(integer index);

		void reserve(integer size);

	private:
		std::vector<Point<N, Real> > data_;
	};

	typedef Polygon<2, real> Polygon2;
	typedef Polygon<3, real> Polygon3;
	typedef Polygon<4, real> Polygon4;

}

#include "pastel/geometry/polygon.hpp"

#endif
