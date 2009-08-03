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
		: public Tuple<Point<Real, N>, M + 1>
	{
	private:
		typedef Tuple<Point<Real, N>, M + 1> Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Point<Real, N>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<Point<Real, N>, M + 1>& that)
			: Base(that)
		{
		}

		Simplex<N, Real, M>& operator=(
			const Tuple<Point<Real, N>, M + 1>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	template <int N, typename Real>
	class Simplex<N, Real, 1>
		: public Tuple<Point<Real, N>, 2>
	{
	private:
		typedef Tuple<Point<Real, N>, 2> Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Point<Real, N>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<Point<Real, N>, 2>& that)
			: Base(that)
		{
		}

		Simplex(
			const Point<Real, N>& a,
			const Point<Real, N>& b)
			: Base(a, b)
		{
		}

		Simplex<N, Real, 1>& operator=(
		const Tuple<Point<Real, N>, 2>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	template <int N, typename Real>
	class Simplex<N, Real, 2>
		: public Tuple<Point<Real, N>, 3>
	{
	private:
		typedef Tuple<Point<Real, N>, 3> Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Point<Real, N>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<Point<Real, N>, 3>& that)
			: Base(that)
		{
		}

		Simplex(
			const Point<Real, N>& a,
			const Point<Real, N>& b,
			const Point<Real, N>& c)
			: Base(a, b, c)
		{
		}

		Simplex<N, Real, 2>& operator=(
		const Tuple<Point<Real, N>, 3>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	template <int N, typename Real>
	class Simplex<N, Real, 3>
		: public Tuple<Point<Real, N>, 4>
	{
	private:
		typedef Tuple<Point<Real, N>, 4> Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Point<Real, N>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<Point<Real, N>, 4>& that)
			: Base(that)
		{
		}

		Simplex(
			const Point<Real, N>& a,
			const Point<Real, N>& b,
			const Point<Real, N>& c,
			const Point<Real, N>& d)
			: Base(a, b, c, d)
		{
		}

		Simplex<N, Real, 3>& operator=(
			const Tuple<Point<Real, N>, 4>& that)
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
