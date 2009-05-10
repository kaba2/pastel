#ifndef PASTELGEOMETRY_TRIANGLE_H
#define PASTELGEOMETRY_TRIANGLE_H

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
			const Tuple<3, Point<N, Real> >& that)
			: Base(that)
		{
		}

		Triangle(
			const Point<N, Real>& a,
			const Point<N, Real>& b,
			const Point<N, Real>& c)
			: Base(a, b, c)
		{
		}

		Triangle<N, Real>& operator=(
			const Tuple<3, Point<N, Real> >& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	typedef Triangle<1, real> Triangle1;
	typedef Triangle<2, real> Triangle2;
	typedef Triangle<3, real> Triangle3;
	typedef Triangle<4, real> Triangle4;
	typedef Triangle<Dynamic, real> DynamicTriangle;

/*
	template <int N, typename Real>
	class Triangle
		: public Tuple<3, Point<N, Real> >
	{
	private:
		typedef Tuple<3, Point<N, Real> > Base;

	public:
		using Base::operator[];

		Triangle()
			: Base()
		{
		}

		Triangle(
			const Tuple<3, Point<N, Real> >& that)
			: Base(that)
		{
		}

		Triangle(
			const Point<N, Real>& a,
			const Point<N, Real>& b,
			const Point<N, Real>& c)
			: Base(a, b, c)
		{
		}

		Triangle<N, Real>& operator=(
			const Tuple<3, Point<N, Real> >& that)
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
