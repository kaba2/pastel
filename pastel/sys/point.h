/*!
\file
\brief A class for a point in R^N.
*/

#ifndef PASTELSYS_POINT_H
#define PASTELSYS_POINT_H

#include "pastel/sys/mytypes.h"

#include "pastel/sys/pointbase.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	// General version for dimension n

	//! A point in R^N.

	/*!
	A Point can be converted to a position vector by
	the asVector() function (with zero overhead).
	A Point can be explicitly constructed from a Vector
	(but not implicitly). The low dimensional points
	(N <= 4) have been specialized for convenient
	initialization in constructors and setters.
	*/

	/*
	Point's have been privately derived from Vector's.
	This makes it possible to reuse its efficient
	implementation and especially makes it possible
	to convert a Point into a position vector efficiently
	(actually, with no cost) with the asVector function.
	Curiously recurring template pattern is used to place
	common functionality into a superclass. This makes
	it possible to specialize the low dimensional (N <= 4)
	points for convenient initialization.
	*/

	template <int N, typename Real>
	class Point
		: public Detail::PointBase<N, Real, Point<N, Real> >
	{
	private:
		typedef Detail::PointBase<N, Real, Point<N, Real> >
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		//! Constructs a point with all elements zero.
		Point()
			: Base()
		{
		}

		/*
		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}
		*/

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		//! Constructs with all elements set to the given value.
		explicit Point(const Real& that)
			: Base(that)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		// Used for concept checking.
		~Point()
		{
			BOOST_STATIC_ASSERT(N > 0);
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

	// Specialization for dimension 1

	template <typename Real>
	class Point<1, Real>
		: public Detail::PointBase<1, Real, Point<1, Real> >
	{
	private:
		enum
		{
			N = 1
		};

		typedef Detail::PointBase<N, Real, Point<N, Real> >
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		//! Constructs a point with all elements zero.
		Point()
			: Base()
		{
		}

		/*
		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}
		*/

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		//! Constructs with all elements set to the given value.
		// NOTE: An implicit conversion from scalar allowed.
		Point(const Real& that)
			: Base(that)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<N, ThatReal, Expression>& that)
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
	class Point<2, Real>
		: public Detail::PointBase<2, Real, Point<2, Real> >
	{
	private:
		enum
		{
			N = 2
		};

		typedef Detail::PointBase<N, Real, Point<N, Real> >
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;

		//! Constructs a point with all elements zero.
		Point()
			: Base()
		{
		}

		/*
		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}
		*/

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		//! Constructs with all elements set to the given value.
		explicit Point(const Real& that)
			: Base(that)
		{
		}

		//! Constructs with the given elements.
		Point(const Real& x, const Real& y)
			: Base()
		{
			set(x, y);
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<N, ThatReal, Expression>& that)
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
	class Point<3, Real>
		: public Detail::PointBase<3, Real, Point<3, Real> >
	{
	private:
		enum
		{
			N = 3
		};

		typedef Detail::PointBase<N, Real, Point<N, Real> >
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;

		//! Constructs a point with all elements zero.
		Point()
			: Base()
		{
		}

		/*
		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}
		*/

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		//! Constructs with all elements set to the given value.
		explicit Point(const Real& that)
			: Base(that)
		{
		}

		//! Constructs with the given elements.
		Point(const Real& x, const Real& y, const Real& z)
			: Base()
		{
			set(x, y, z);
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<N, ThatReal, Expression>& that)
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
	class Point<4, Real>
		: public Detail::PointBase<4, Real, Point<4, Real> >
	{
	private:
		enum
		{
			N = 4
		};

		typedef Detail::PointBase<N, Real, Point<N, Real> >
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;

		//! Constructs a point with all elements zero.
		Point()
			: Base()
		{
		}

		/*
		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}
		*/

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		//! Constructs with all elements set to the given value.
		explicit Point(const Real& that)
			: Base(that)
		{
		}

		//! Constructs with the given elements.
		Point(const Real& x, const Real& y,
			const Real& z,	const Real& w)
			: Base()
		{
			set(x, y, z, w);
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<N, ThatReal, Expression>& that)
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

	template <int N, typename Real>
	void swap(Point<N, Real>& left,
		Point<N, Real>& right);

	template <int N, typename Real, typename Expression>
	Point<N, Real> asPoint(const VectorExpression<N, Real, Expression>& that);

	template <int N, typename Real>
	Vector<N, Real>& asVector(Point<N, Real>& that);

	template <int N, typename Real>
	const Vector<N, Real>& asVector(const Point<N, Real>& that);

	template <int N, typename Real>
	Tuple<N, Real>& asTuple(Point<N, Real>& that);

	template <int N, typename Real>
	const Tuple<N, Real>& asTuple(const Point<N, Real>& that);

	typedef Point<1, integer> IPoint1;
	typedef Point<2, integer> IPoint2;
	typedef Point<3, integer> IPoint3;
	typedef Point<4, integer> IPoint4;

	typedef Point<1, real> Point1;
	typedef Point<2, real> Point2;
	typedef Point<3, real> Point3;
	typedef Point<4, real> Point4;

}

#include "pastel/sys/point.hpp"

#endif
