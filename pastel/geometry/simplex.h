// Description: Simplex class
// Detail: Allows to work with simplices
// Documentation: shapes.txt

#ifndef PASTEL_SIMPLEX_H
#define PASTEL_SIMPLEX_H

#include "pastel/sys/tuple.h"

#include "pastel/sys/point.h"

namespace Pastel
{

	template <int N, typename Real, int M>
	class Simplex
		: public Tuple<M + 1, Point<N, Real> >
	{
	private:
		typedef Tuple<M + 1, Point<N, Real> > Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Point<N, Real>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<M + 1, Point<N, Real> >& that)
			: Base(that)
		{
		}

		Simplex<N, Real, M>& operator=(
			const Tuple<M + 1, Point<N, Real> >& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	template <int N, typename Real>
	class Simplex<N, Real, 1>
		: public Tuple<2, Point<N, Real> >
	{
	private:
		typedef Tuple<2, Point<N, Real> > Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Point<N, Real>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<2, Point<N, Real> >& that)
			: Base(that)
		{
		}

		Simplex(
			const Point<N, Real>& a,
			const Point<N, Real>& b)
			: Base(a, b)
		{
		}

		Simplex<N, Real, 1>& operator=(
		const Tuple<2, Point<N, Real> >& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	template <int N, typename Real>
	class Simplex<N, Real, 2>
		: public Tuple<3, Point<N, Real> >
	{
	private:
		typedef Tuple<3, Point<N, Real> > Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Point<N, Real>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<3, Point<N, Real> >& that)
			: Base(that)
		{
		}

		Simplex(
			const Point<N, Real>& a,
			const Point<N, Real>& b,
			const Point<N, Real>& c)
			: Base(a, b, c)
		{
		}

		Simplex<N, Real, 2>& operator=(
		const Tuple<3, Point<N, Real> >& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	template <int N, typename Real>
	class Simplex<N, Real, 3>
		: public Tuple<4, Point<N, Real> >
	{
	private:
		typedef Tuple<4, Point<N, Real> > Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Point<N, Real>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<4, Point<N, Real> >& that)
			: Base(that)
		{
		}

		Simplex(
			const Point<N, Real>& a,
			const Point<N, Real>& b,
			const Point<N, Real>& c,
			const Point<N, Real>& d)
			: Base(a, b, c, d)
		{
		}

		Simplex<N, Real, 3>& operator=(
			const Tuple<4, Point<N, Real> >& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	typedef Simplex<1, real, 1> Simplex1;
	typedef Simplex<2, real, 2> Simplex2;
	typedef Simplex<3, real, 3> Simplex3;
	typedef Simplex<4, real, 4> Simplex4;

	typedef Simplex<1, real, 1> Simplex11;
	typedef Simplex<2, real, 1> Simplex21;
	typedef Simplex<3, real, 1> Simplex31;
	typedef Simplex<4, real, 1> Simplex41;

	typedef Simplex<2, real, 2> Simplex22;
	typedef Simplex<3, real, 2> Simplex32;
	typedef Simplex<4, real, 2> Simplex42;

	typedef Simplex<3, real, 3> Simplex33;
	typedef Simplex<4, real, 3> Simplex43;

	typedef Simplex<4, real, 4> Simplex44;

}

#endif
