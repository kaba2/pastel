// Description: Polygon class

#ifndef PASTELGEOMETRY_POLYGON_H
#define PASTELGEOMETRY_POLYGON_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/vector.h"

#include <vector>

namespace Pastel
{

	template <typename Real, int N = Dynamic>
	class Polygon
	{
	public:
		using Real_ = Real;
		static constexpr int N_ = N;

		// Using default constructor.
		// Using default copy constructor.
		// Using default destructor.

		Polygon<Real, N>& operator=(
			const Polygon& that);

		Vector<Real, N>& operator[](integer index);
		const Vector<Real, N>& operator[](integer index) const;

		void swap(Polygon& that);

		integer size() const;

		void pushBack(const Vector<Real, N>& that);
		void popBack();

		void insert(integer index, const Vector<Real, N>& that);
		void erase(integer index);

		void reserve(integer size);

	private:
		std::vector<Vector<Real, N> > data_;
	};

	using Polygon2 = Polygon<dreal, 2>;
	using Polygon3 = Polygon<dreal, 3>;
	using Polygon4 = Polygon<dreal, 4>;

}

#include "pastel/geometry/shape/polygon.hpp"

#endif
