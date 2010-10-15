// Description: Simplex class
// Detail: Allows to work with simplices
// Documentation: shapes.txt

#ifndef PASTEL_SIMPLEX_H
#define PASTEL_SIMPLEX_H

#include "pastel/sys/tuple.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, int N, int M>
	class Simplex
		: public Tuple<Vector<Real, N>, M + 1>
	{
	private:
		typedef Tuple<Vector<Real, N>, M + 1> Base;

	public:
		typedef Real Real_;
		enum 
		{
			N_ = N,
			M_ = M
		};

		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Vector<Real, N>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<Vector<Real, N>, M + 1>& that)
			: Base(that)
		{
		}

		Simplex<Real, N, M>& operator=(
			const Tuple<Vector<Real, N>, M + 1>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	template <typename Real, int N>
	class Simplex<Real, N, 1>
		: public Tuple<Vector<Real, N>, 2>
	{
	private:
		typedef Tuple<Vector<Real, N>, 2> Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Vector<Real, N>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<Vector<Real, N>, 2>& that)
			: Base(that)
		{
		}

		Simplex(
			const Vector<Real, N>& a,
			const Vector<Real, N>& b)
			: Base(a, b)
		{
		}

		Simplex<Real, N, 1>& operator=(
		const Tuple<Vector<Real, N>, 2>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	template <typename Real, int N>
	class Simplex<Real, N, 2>
		: public Tuple<Vector<Real, N>, 3>
	{
	private:
		typedef Tuple<Vector<Real, N>, 3> Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Vector<Real, N>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<Vector<Real, N>, 3>& that)
			: Base(that)
		{
		}

		Simplex(
			const Vector<Real, N>& a,
			const Vector<Real, N>& b,
			const Vector<Real, N>& c)
			: Base(a, b, c)
		{
		}

		Simplex<Real, N, 2>& operator=(
		const Tuple<Vector<Real, N>, 3>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	template <typename Real, int N>
	class Simplex<Real, N, 3>
		: public Tuple<Vector<Real, N>, 4>
	{
	private:
		typedef Tuple<Vector<Real, N>, 4> Base;

	public:
		using Base::operator[];

		Simplex()
			: Base()
		{
		}

		explicit Simplex(
			const Vector<Real, N>& that)
			: Base(that)
		{
		}

		Simplex(
			const Tuple<Vector<Real, N>, 4>& that)
			: Base(that)
		{
		}

		Simplex(
			const Vector<Real, N>& a,
			const Vector<Real, N>& b,
			const Vector<Real, N>& c,
			const Vector<Real, N>& d)
			: Base(a, b, c, d)
		{
		}

		Simplex<Real, N, 3>& operator=(
			const Tuple<Vector<Real, N>, 4>& that)
		{
			Base::operator=(that);
			return *this;
		}
	};

	typedef Simplex<real, 1, 1> Simplex1;
	typedef Simplex<real, 2, 2> Simplex2;
	typedef Simplex<real, 3, 3> Simplex3;
	typedef Simplex<real, 4, 4> Simplex4;

	typedef Simplex<real, 1, 1> Simplex11;
	typedef Simplex<real, 2, 1> Simplex21;
	typedef Simplex<real, 3, 1> Simplex31;
	typedef Simplex<real, 4, 1> Simplex41;

	typedef Simplex<real, 2, 2> Simplex22;
	typedef Simplex<real, 3, 2> Simplex32;
	typedef Simplex<real, 4, 2> Simplex42;

	typedef Simplex<real, 3, 3> Simplex33;
	typedef Simplex<real, 4, 3> Simplex43;

	typedef Simplex<real, 4, 4> Simplex44;

}

#endif
