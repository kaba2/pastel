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

	template <int N, typename Real>
	class Point
		: public Detail::PointBase<N, Real>
	{
	private:
		typedef Detail::PointBase<N, Real>
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

		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit Point(const Real& that)
			: Base(that)
		{
		}

		Point(const Point<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
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
			BOOST_STATIC_ASSERT(N == Unbounded || N > 0);
		}
	};

	// Specialization for dimension 1

	template <typename Real>
	class Point<1, Real>
		: public Detail::PointBase<1, Real>
	{
	private:
		enum
		{
			N = 1
		};

		typedef Detail::PointBase<N, Real>
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

		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		// Implicit conversion allowed
		// only for 1d points.
		Point(const Real& that)
			: Base(that)
		{
		}

		Point(const Point<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
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
		: public Detail::PointBase<2, Real>
	{
	private:
		enum
		{
			N = 2
		};

		typedef Detail::PointBase<N, Real>
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

		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit Point(const Real& that)
			: Base(that)
		{
		}

		Point(const Point<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
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
		: public Detail::PointBase<3, Real>
	{
	private:
		enum
		{
			N = 3
		};

		typedef Detail::PointBase<N, Real>
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

		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit Point(const Real& that)
			: Base(that)
		{
		}

		Point(const Point<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
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
		: public Detail::PointBase<4, Real>
	{
	private:
		enum
		{
			N = 4
		};

		typedef Detail::PointBase<N, Real>
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

		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit Point(const Real& that)
			: Base(that)
		{
		}

		Point(const Point<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
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

	// Specialization for unbounded dimension.

	template <typename Real>
	class Point<Unbounded, Real>
		: public Detail::PointBase<Unbounded, Real>
	{
	private:
		enum
		{
			N = Unbounded
		};

		typedef Detail::PointBase<N, Real>
			Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;

		// We force the specification
		// of dimension by prohibiting
		// default construction.
		/*
		Point()
			: Base()
		{
		}
		*/

		// See note 1 in this file.
		explicit Point(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit Point(const Tuple<N, ThatReal>& that)
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

		Point(const Point<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Point(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		Point(const TemporaryPoint<N, Real>& that)
			: Base(that.asVector())
		{
		}

		explicit Point(const TemporaryVector<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit Point(const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		Point<N, Real>& operator=(const TemporaryPoint<N, Real>& that)
		{
			Point<N, Real> copy(that);
			swap(copy);
			return *this;
		}
	};

	template <int N, typename Real>
	void swap(Point<N, Real>& left,
		Point<N, Real>& right);

	template <int N, typename Real, typename Expression>
	TemporaryPoint<N, Real> asPoint(
		const VectorExpression<N, Real, Expression>& that);

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

	template <int N, typename Real>
	class TemporaryPoint
		: public Point<N, Real>
	{
	private:
		typedef Point<N, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		TemporaryPoint()
			: Base(Dimension(N), Real())
		{
		}

		explicit TemporaryPoint(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryPoint(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit TemporaryPoint(const Real& that)
			: Base(that)
		{
		}

		TemporaryPoint(const TemporaryPoint<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryPoint(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit TemporaryPoint(const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		// Used for concept checking.
		~TemporaryPoint()
		{
			BOOST_STATIC_ASSERT(N == Unbounded || N > 0);
		}

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}

		TemporaryVector<N, Real>& asVector()
		{
			return Base::asVector().asTemporary();
		}

		const TemporaryVector<N, Real>& asVector() const
		{
			return ((Vector<N, Real>&)Base::asVector()).asTemporary();
		}
	};

	// Specialization for dimension 1

	template <typename Real>
	class TemporaryPoint<1, Real>
		: public Point<1, Real>
	{
	private:
		enum
		{
			N = 1
		};

		typedef Point<N, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		TemporaryPoint()
			: Base(Dimension(N), Real())
		{
		}

		// See note 1 in this file.
		explicit TemporaryPoint(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryPoint(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit TemporaryPoint(const Real& that)
			: Base(that)
		{
		}

		TemporaryPoint(const TemporaryPoint<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryPoint(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit TemporaryPoint(const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}

		TemporaryVector<N, Real>& asVector()
		{
			return Base::asVector().asTemporary();
		}

		const TemporaryVector<N, Real>& asVector() const
		{
			return ((Vector<N, Real>&)Base::asVector()).asTemporary();
		}
	};

	// Specialization for dimension 2

	template <typename Real>
	class TemporaryPoint<2, Real>
		: public Point<2, Real>
	{
	private:
		enum
		{
			N = 2
		};

		typedef Point<N, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		TemporaryPoint()
			: Base(Dimension(N), Real())
		{
		}

		// See note 1 in this file.
		explicit TemporaryPoint(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryPoint(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit TemporaryPoint(const Real& that)
			: Base(that)
		{
		}

		TemporaryPoint(const TemporaryPoint<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryPoint(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		//! Constructs with the given elements.
		TemporaryPoint(const Real& x, const Real& y)
			: Base(x, y)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit TemporaryPoint(const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}

		TemporaryVector<N, Real>& asVector()
		{
			return Base::asVector().asTemporary();
		}

		const TemporaryVector<N, Real>& asVector() const
		{
			return ((Vector<N, Real>&)Base::asVector()).asTemporary();
		}
	};

	// Specialization for dimension 3

	template <typename Real>
	class TemporaryPoint<3, Real>
		: public Point<3, Real>
	{
	private:
		enum
		{
			N = 3
		};

		typedef Point<N, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		TemporaryPoint()
			: Base(Dimension(N), Real())
		{
		}

		// See note 1 in this file.
		explicit TemporaryPoint(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryPoint(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit TemporaryPoint(const Real& that)
			: Base(that)
		{
		}

		TemporaryPoint(const TemporaryPoint<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryPoint(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		//! Constructs with the given elements.
		TemporaryPoint(const Real& x, const Real& y, const Real& z)
			: Base(x, y, z)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit TemporaryPoint(const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}

		TemporaryVector<N, Real>& asVector()
		{
			return Base::asVector().asTemporary();
		}

		const TemporaryVector<N, Real>& asVector() const
		{
			return ((Vector<N, Real>&)Base::asVector()).asTemporary();
		}
	};

	// Specialization for dimension 4

	template <typename Real>
	class TemporaryPoint<4, Real>
		: public Point<4, Real>
	{
	private:
		enum
		{
			N = 4
		};

		typedef Point<N, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		TemporaryPoint()
			: Base(Dimension(N), Real())
		{
		}

		// See note 1 in this file.
		explicit TemporaryPoint(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryPoint(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit TemporaryPoint(const Real& that)
			: Base(that)
		{
		}

		TemporaryPoint(const TemporaryPoint<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryPoint(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		//! Constructs with the given elements.
		TemporaryPoint(const Real& x, const Real& y,
			const Real& z,	const Real& w)
			: Base(x, y, z, w)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit TemporaryPoint(const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}

		TemporaryVector<N, Real>& asVector()
		{
			return Base::asVector().asTemporary();
		}

		const TemporaryVector<N, Real>& asVector() const
		{
			return ((Vector<N, Real>&)Base::asVector()).asTemporary();
		}
	};

	// Specialization for unbounded dimensions.

	template <typename Real>
	class TemporaryPoint<Unbounded, Real>
		: public Point<Unbounded, Real>
	{
	private:
		enum
		{
			N = Unbounded
		};

		typedef Point<N, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		// We force to specify the dimension
		// of an unbounded vector by
		// not allowing default construction.
		/*
		TemporaryPoint()
			: Base()
		{
		}
		*/

		// See note 1 in this file.
		explicit TemporaryPoint(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryPoint(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		// This function has no meaning for an unbounded
		// vector since it does not tell the number
		// of dimensions. Thus we leave it out.
		/*
		explicit TemporaryPoint(const Real& that)
			: Base(that)
		{
		}
		*/

		TemporaryPoint(const TemporaryPoint<N, Real>& that)
			: Base(that)
		{
		}

		explicit TemporaryPoint(const TemporaryVector<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryPoint(const Point<N, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, typename Expression>
		explicit TemporaryPoint(const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		// Used for concept checking.
		~TemporaryPoint()
		{
			BOOST_STATIC_ASSERT(N == Unbounded || N > 0);
		}

		TemporaryPoint<N, Real>& operator=(const TemporaryPoint<N, Real>& that)
		{
			TemporaryPoint<N, Real> copy(that);
			swap(copy);
			return *this;
		}

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}

		TemporaryVector<N, Real>& asVector()
		{
			return Base::asVector().asTemporary();
		}

		const TemporaryVector<N, Real>& asVector() const
		{
			return ((Vector<N, Real>&)Base::asVector()).asTemporary();
		}
	};

}

#include "pastel/sys/point.hpp"

#endif
