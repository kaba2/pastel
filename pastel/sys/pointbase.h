#ifndef PASTELSYS_POINTBASE_H
#define PASTELSYS_POINTBASE_H

#include "pastel/sys/point.h"

#include "pastel/sys/vector.h"

#include <boost/static_assert.hpp>
#include <boost/operators.hpp>

namespace Pastel
{

	template <int N, typename Real>
	class Point;

	template <int N, typename Real>
	class TemporaryPoint;

	namespace Detail
	{

		template <int N, typename Real>
		class PointBase
			: boost::equality_comparable<Point<N, Real> >
		{
		private:
			template <int N, typename Real>
			friend class PointBase;

		public:
			explicit PointBase(
				const Dimension& dimension, 
				const Real& that)
				: data_(dimension, that)
			{
			}

			explicit PointBase(
				const Real& that)
				: data_(that)
			{
			}

			template <typename ThatReal>
			PointBase(const Tuple<N, ThatReal>& that)
				: data_(that)
			{
			}

			template <typename ThatReal>
			PointBase(const PointBase<N, ThatReal>& that)
				: data_(that.data_)
			{
			}

			//! Constructs with the given point.
			PointBase(const PointBase& that)
				: data_(that.data_)
			{
			}

			//! Constructs from a vector.
			template <typename ThatReal, typename Expression>
			explicit PointBase(const VectorExpression
				<N, ThatReal, Expression>& that)
				: data_(that)
			{
			}

			~PointBase()
			{
				BOOST_STATIC_ASSERT(N == Unbounded || N > 0);
			}

			void setSize(integer size, const Real& that = Real())
			{
				data_.setSize(size, that);
			}

			integer size() const
			{
				return data_.size();
			}

			//! Swaps two points.
			void swap(Point<N, Real>& that)
			{
				data_.swap(that.data_);
			}

			//! Sets all of the elements to the given value.
			void set(const Real& that)
			{
				data_.set(that);
			}

			bool operator==(const Point<N, Real>& that) const
			{
				return data_ == that.data_;
			}

			//! Returns the index:th element.
			Real& operator[](integer index)
			{
				return data_[index];
			}

			//! Returns the index:th element.
			const Real& operator[](integer index) const
			{
				return data_[index];
			}

			//! Adds the given value to all elements.
			Point<N, Real>& operator+=(const Real& that)
			{
				data_ += that;
				return (Point<N, Real>&)*this;
			}

			//! Returns the point added by 'that' to all elements.
			TemporaryPoint<N, Real> operator+(const Real& that) const
			{
				TemporaryPoint<N, Real> result((Point<N, Real>&)*this);
				result += that;
				return result;
			}

			//! Subtracts the given value from all elements.
			Point<N, Real>& operator-=(const Real& that)
			{
				data_ -= that;
				return (Point<N, Real>&)*this;
			}

			//! Returns the point subtracted by 'that' from all elements.
			TemporaryPoint<N, Real> operator-(const Real& that) const
			{
				TemporaryPoint<N, Real> result((Point<N, Real>&)*this);
				result -= that;
				return result;
			}

			//! Translates the point by 'that'.
			template <typename Expression>
			Point<N, Real>& operator+=(
				const VectorExpression
				<N, Real, Expression>& that)
			{
				data_ += that;

				return (Point<N, Real>&)*this;
			}

			//! Translates the point backwards by 'that'.
			template <typename Expression>
			Point<N, Real>& operator-=(
				const VectorExpression
				<N, Real, Expression>& that)
			{
				data_ -= that;

				return (Point<N, Real>&)*this;
			}

			//! Returns the difference vector between two points.
			const VectorSubtraction<N, Real, 
				VectorBase<N, Real>,
				VectorBase<N, Real>
				> operator-(const Point<N, Real>& that) const
			//TemporaryVector<N, Real> operator-(const Point<N, Real>& that) const
			{
				return data_ - that.data_;
			}

			//! Returns the point translated by 'that'.
			template <typename Expression>
			TemporaryPoint<N, Real> operator+(
				const VectorExpression
				<N, Real, Expression>& that) const
			{
				TemporaryPoint<N, Real> result((const Point<N, Real>&)*this);
				result += that;
				return result;
			}

			//! Returns the point translated backwards by 'that'.
			template <typename Expression>
			TemporaryPoint<N, Real> operator-(
				const VectorExpression
				<N, Real, Expression>& that) const
			{
				TemporaryPoint<N, Real> result((const Point<N, Real>&)*this);
				result -= that;
				return result;
			}

			//! Interprets the point as a position vector.
			Vector<N, Real>& asVector()
			{
				return data_;
			}

			//! Interprets the point as a position vector.
			const Vector<N, Real>& asVector() const
			{
				return data_;
			}

			//! Interprets the point as a temporary.
			TemporaryPoint<N, Real>& asTemporary()
			{
				return (TemporaryPoint<N, Real>&)*this;
			}

		private:
			Vector<N, Real> data_;
		};

	}

}

#endif
