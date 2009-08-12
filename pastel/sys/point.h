// Description: Point class
// Detail: Allows to work with points in affine n-space

#ifndef PASTEL_POINT_H
#define PASTEL_POINT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/pointbase.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	/*
	Note 1:
	Each point class contains one constructor
	which takes a parameter typed Dimension.
	The purpose of the Dimension type is
	to disambiguate the constructors
	from the 2d initialization constructor 
	and single value initialization constructor
	of fixed dimensional vectors.
	This constructor is actually needed only 
	for unbounded vectors.
	However, to enable generic algorithms
	which work with both unbounded and fixed
	dimensional vectors, we also implement 
	this constructor for the fixed dimensional vectors.
	*/

	// General version for dimension n

	//! A point in R^N.

	template <typename Real, int N>
	class Point
		: public Detail::PointBase<Real, N>
	{
	private:
		typedef Detail::PointBase<Real, N>
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;

		Point()
			: Base(Dimension(N), Real())
		{
		}

		// See note 1 in this file.
		explicit Point(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		explicit Point(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit Point(const Tuple<ThatReal, N>& that)
			: Base(that)
		{
		}

		explicit Point(const Real& that)
			: Base(that)
		{
		}

		explicit Point(const Copy<const Real*>& that)
			: Base(that)
		{
		}

		Point(const Point<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<ThatReal, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<ThatReal, N, Expression>& that)
			: Base(that)
		{
		}

		// Used for concept checking.
		~Point()
		{
			BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
		}
	};

	// Specialization for dimension 1

	template <typename Real>
	class Point<Real, 1>
		: public Detail::PointBase<Real, 1>
	{
	private:
		enum
		{
			N = 1
		};

		typedef Detail::PointBase<Real, N>
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;

		Point()
			: Base(Dimension(N), Real())
		{
		}

		// See note 1 in this file.
		explicit Point(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		explicit Point(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit Point(const Tuple<ThatReal, N>& that)
			: Base(that)
		{
		}

		// Implicit conversion allowed
		// only for 1d points.
		Point(const Real& that)
			: Base(that)
		{
		}

		explicit Point(const Copy<const Real*>& that)
			: Base(that)
		{
		}

		Point(const Point<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<ThatReal, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<ThatReal, N, Expression>& that)
			: Base(that)
		{
		}

		Real& x()
		{
			return (*this)[0];
		}

		const Real& x() const
		{
			return (*this)[0];
		}
	};

	// Specialization for dimension 2

	template <typename Real>
	class Point<Real, 2>
		: public Detail::PointBase<Real, 2>
	{
	private:
		enum
		{
			N = 2
		};

		typedef Detail::PointBase<Real, N>
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;

		Point()
			: Base(Dimension(N), Real())
		{
		}

		// See note 1 in this file.
		explicit Point(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		explicit Point(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit Point(const Tuple<ThatReal, N>& that)
			: Base(that)
		{
		}

		explicit Point(const Real& that)
			: Base(that)
		{
		}

		explicit Point(const Copy<const Real*>& that)
			: Base(that)
		{
		}

		Point(const Point<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<ThatReal, N>& that)
			: Base(that)
		{
		}

		//! Constructs with the given elements.
		Point(const Real& x, const Real& y)
			: Base(Dimension(N), Real())
		{
			set(x, y);
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<ThatReal, N, Expression>& that)
			: Base(that)
		{
		}

		//! Assigns the given elements.
		void set(const Real& x, const Real& y)
		{
			(*this)[0] = x;
			(*this)[1] = y;
		}

		Real& x()
		{
			return (*this)[0];
		}

		const Real& x() const
		{
			return (*this)[0];
		}

		Real& y()
		{
			return (*this)[1];
		}

		const Real& y() const
		{
			return (*this)[1];
		}
	};

	// Specialization for dimension 3

	template <typename Real>
	class Point<Real, 3>
		: public Detail::PointBase<Real, 3>
	{
	private:
		enum
		{
			N = 3
		};

		typedef Detail::PointBase<Real, N>
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;

		Point()
			: Base(Dimension(N), Real())
		{
		}

		// See note 1 in this file.
		explicit Point(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		explicit Point(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit Point(const Tuple<ThatReal, N>& that)
			: Base(that)
		{
		}

		explicit Point(const Real& that)
			: Base(that)
		{
		}

		explicit Point(const Copy<const Real*>& that)
			: Base(that)
		{
		}

		Point(const Point<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<ThatReal, N>& that)
			: Base(that)
		{
		}

		//! Constructs with the given elements.
		Point(const Real& x, const Real& y, const Real& z)
			: Base(Dimension(N), Real())
		{
			set(x, y, z);
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<ThatReal, N, Expression>& that)
			: Base(that)
		{
		}

		//! Assigns the given elements.
		void set(const Real& x, const Real& y, const Real& z)
		{
			(*this)[0] = x;
			(*this)[1] = y;
			(*this)[2] = z;
		}

		Real& x()
		{
			return (*this)[0];
		}

		const Real& x() const
		{
			return (*this)[0];
		}

		Real& y()
		{
			return (*this)[1];
		}

		const Real& y() const
		{
			return (*this)[1];
		}

		Real& z()
		{
			return (*this)[2];
		}

		const Real& z() const
		{
			return (*this)[2];
		}
	};

	// Specialization for dimension 4

	template <typename Real>
	class Point<Real, 4>
		: public Detail::PointBase<Real, 4>
	{
	private:
		enum
		{
			N = 4
		};

		typedef Detail::PointBase<Real, N>
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;

		Point()
			: Base(Dimension(N), Real())
		{
		}

		// See note 1 in this file.
		explicit Point(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		explicit Point(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit Point(const Tuple<ThatReal, N>& that)
			: Base(that)
		{
		}

		explicit Point(const Real& that)
			: Base(that)
		{
		}

		explicit Point(const Copy<const Real*>& that)
			: Base(that)
		{
		}

		Point(const Point<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<ThatReal, N>& that)
			: Base(that)
		{
		}

		//! Constructs with the given elements.
		Point(const Real& x, const Real& y,
			const Real& z,	const Real& w)
			: Base(Dimension(N), Real())
		{
			set(x, y, z, w);
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<ThatReal, N, Expression>& that)
			: Base(that)
		{
		}

		//! Assigns the given elements.
		void set(const Real& x, const Real& y,
			const Real& z, const Real& w)
		{
			(*this)[0] = x;
			(*this)[1] = y;
			(*this)[2] = z;
			(*this)[3] = w;
		}

		Real& x()
		{
			return (*this)[0];
		}

		const Real& x() const
		{
			return (*this)[0];
		}

		Real& y()
		{
			return (*this)[1];
		}

		const Real& y() const
		{
			return (*this)[1];
		}

		Real& z()
		{
			return (*this)[2];
		}

		const Real& z() const
		{
			return (*this)[2];
		}

		Real& w()
		{
			return (*this)[3];
		}

		const Real& w() const
		{
			return (*this)[3];
		}
	};

	// Specialization for unbounded dimension.

	template <typename Real>
	class Point<Real, Dynamic>
		: public Detail::PointBase<Real, Dynamic>
	{
	private:
		enum
		{
			N = Dynamic
		};

		typedef Detail::PointBase<Real, N>
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;
		using Base::swap;

		Point()
			: Base()
		{
		}

		// See note 1 in this file.
		explicit Point(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		explicit Point(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		// See note 1 in this file.
		explicit Point(
			const Dimension& dimension,
			const Alias<Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit Point(const Tuple<ThatReal, N>& that)
			: Base(that)
		{
		}

		// This function has no meaning for an unbounded
		// vector since it does not tell the number
		// of dimensions. Thus we leave it out.
		/*
		explicit Point(const Real& that)
			: Base(that)
		{
		}
		*/

		Point(const Point<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<ThatReal, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<ThatReal, N, Expression>& that)
			: Base(that)
		{
		}
	};

	template <int N, typename Real>
	void swap(Point<Real, N>& left,
		Point<Real, N>& right);

	template <int N, typename Real, typename Expression>
	Point<Real, N> asPoint(
		const VectorExpression<Real, N, Expression>& that);

	template <int N, typename Real>
	Vector<Real, N>& asVector(
		Point<Real, N>& that);

	template <int N, typename Real>
	const Vector<Real, N>& asVector(
		const Point<Real, N>& that);

	template <int N, typename Real>
	Tuple<Real, N>& asTuple(
		Point<Real, N>& that);

	template <int N, typename Real>
	const Tuple<Real, N>& asTuple(
		const Point<Real, N>& that);

	typedef Point<integer, 1> Point1i;
	typedef Point<integer, 2> Point2i;
	typedef Point<integer, 3> Point3i;
	typedef Point<integer, 4> Point4i;
	typedef Point<integer, Dynamic> PointDi;

	typedef Point<real, 1> Point1;
	typedef Point<real, 2> Point2;
	typedef Point<real, 3> Point3;
	typedef Point<real, 4> Point4;
	typedef Point<real, Dynamic> PointD;

}

#include "pastel/sys/point.hpp"

#endif
