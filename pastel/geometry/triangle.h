// Description: Triangle class
// Detail: Allows to work with triangles
// Documentation: shapes.txt

#ifndef PASTEL_TRIANGLE_H
#define PASTEL_TRIANGLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/point.h"

#include "pastel/geometry/simplex.h"

namespace Pastel
{

	template <typename Real, int N = Dynamic>
	class Triangle
		: public Simplex<Real, N, 2>
	{
	private:
		typedef Simplex<Real, N, 2> Base;

	public:
		using Base::operator[];

		Triangle()
			: Base()
		{
		}

		Triangle(
			const Tuple<Point<Real, N>, 3>& that)
			: Base(that)
		{
		}

		Triangle(
			const Point<Real, N>& a,
			const Point<Real, N>& b,
			const Point<Real, N>& c)
			: Base(a, b, c)
		{
		}

		Triangle<Real, N>& operator=(
			const Tuple<Point<Real, N>, 3>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	typedef Triangle<real, 1> Triangle1;
	typedef Triangle<real, 2> Triangle2;
	typedef Triangle<real, 3> Triangle3;
	typedef Triangle<real, 4> Triangle4;
	typedef Triangle<real, Dynamic> TriangleD;

/*
	template <typename Real, int N>
	class Triangle
		: public Tuple<Point<Real, N>, 3>
	{
	private:
		typedef Tuple<Point<Real, N>, 3> Base;

	public:
		using Base::operator[];

		Triangle()
			: Base()
		{
		}

		Triangle(
			const Tuple<Point<Real, N>, 3>& that)
			: Base(that)
		{
		}

		Triangle(
			const Point<Real, N>& a,
			const Point<Real, N>& b,
			const Point<Real, N>& c)
			: Base(a, b, c)
		{
		}

		Triangle<Real, N>& operator=(
			const Tuple<Point<Real, N>, 3>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	typedef Triangle<real, 1> Triangle1;
	typedef Triangle<real, 2> Triangle2;
	typedef Triangle<real, 3> Triangle3;
	typedef Triangle<real, 4> Triangle4;

*/

}

#endif
