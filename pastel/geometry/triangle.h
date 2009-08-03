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

	template <int N, typename Real>
	class Triangle
		: public Simplex<N, Real, 2>
	{
	private:
		typedef Simplex<N, Real, 2> Base;

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

		Triangle<N, Real>& operator=(
			const Tuple<Point<Real, N>, 3>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	typedef Triangle<1, real> Triangle1;
	typedef Triangle<2, real> Triangle2;
	typedef Triangle<3, real> Triangle3;
	typedef Triangle<4, real> Triangle4;
	typedef Triangle<Dynamic, real> TriangleD;

/*
	template <int N, typename Real>
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

		Triangle<N, Real>& operator=(
			const Tuple<Point<Real, N>, 3>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	typedef Triangle<1, real> Triangle1;
	typedef Triangle<2, real> Triangle2;
	typedef Triangle<3, real> Triangle3;
	typedef Triangle<4, real> Triangle4;

*/

}

#endif
