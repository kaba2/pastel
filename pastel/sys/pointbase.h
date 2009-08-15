// Description: PointBase class
// Detail: CRTP base class for Point

#ifndef PASTEL_POINTBASE_H
#define PASTEL_POINTBASE_H

#include "pastel/sys/point.h"

#include "pastel/sys/vector.h"

#include <boost/static_assert.hpp>
#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Real, int N = Dynamic>
	class Point;

	namespace Detail
	{

		template <typename Real, int N>
		class PointBase
			: boost::equality_comparable<Point<Real, N> >
		{
		private:
			template <typename Real, int N>
			friend class PointBase;

		public:
			PointBase()
				: data_()
			{
			}

			explicit PointBase(
				const Dimension& dimension, 
				const Real& that)
				: data_(dimension, that)
			{
			}

			explicit PointBase(
				const Dimension& dimension, 
				const Alias<Real*>& alias)
				: data_(dimension, alias)
			{
			}

			explicit PointBase(
				const Dimension& dimension,
				const Copy<const Real*>& that)
				: data_(dimension, that)
			{
			}

			explicit PointBase(
				const Real& that)
				: data_(that)
			{
			}

			explicit PointBase(
				const Copy<const Real*>& that)
				: data_(that)
			{
			}

			template <typename ThatReal>
			explicit PointBase(const Tuple<ThatReal, N>& that)
				: data_(that)
			{
			}

			template <typename ThatReal>
			PointBase(const PointBase<ThatReal, N>& that)
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
				<ThatReal, N, Expression>& that)
				: data_(that)
			{
			}

			~PointBase()
			{
				BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
			}

			void setSize(integer size, const Real& that = Real())
			{
				data_.setSize(size, that);
			}

			integer size() const
			{
				return data_.size();
			}

			integer dimension() const
			{
				return data_.size();
			}

			//! Swaps two points.
			void swap(Point<Real, N>& that)
			{
				data_.swap(that.data_);
			}

			//! Sets all of the elements to the given value.
			void set(const Real& that)
			{
				data_.set(that);
			}

			bool operator==(const Point<Real, N>& that) const
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

			//! Returns the address of the first element.
			Real* rawBegin()
			{
				return data_.rawBegin();
			}

			//! Returns the address of the first element.
			const Real* rawBegin() const
			{
				return data_.rawBegin();
			}

			//! Returns the address of the one-past last element.
			Real* rawEnd()
			{
				return data_.rawEnd();
			}

			//! Returns the address of the one-past last element.
			const Real* rawEnd() const
			{
				return data_.rawEnd();
			}

			//! Adds the given value to all elements.
			Point<Real, N>& operator+=(const Real& that)
			{
				data_ += that;
				return (Point<Real, N>&)*this;
			}

			//! Returns the point added by 'that' to all elements.
			Point<Real, N> operator+(const Real& that) const
			{
				Point<Real, N> result((Point<Real, N>&)*this);
				result += that;
				return result;
			}

			//! Subtracts the given value from all elements.
			Point<Real, N>& operator-=(const Real& that)
			{
				data_ -= that;
				return (Point<Real, N>&)*this;
			}

			//! Returns the point subtracted by 'that' from all elements.
			Point<Real, N> operator-(const Real& that) const
			{
				Point<Real, N> result((Point<Real, N>&)*this);
				result -= that;
				return result;
			}

			//! Translates the point by 'that'.
			template <typename Expression>
			Point<Real, N>& operator+=(
				const VectorExpression
				<Real, N, Expression>& that)
			{
				data_ += that;

				return (Point<Real, N>&)*this;
			}

			//! Translates the point backwards by 'that'.
			template <typename Expression>
			Point<Real, N>& operator-=(
				const VectorExpression
				<Real, N, Expression>& that)
			{
				data_ -= that;

				return (Point<Real, N>&)*this;
			}

			//! Returns the difference vector between two points.
			const VectorSubtraction<Real, N, 
				Detail::VectorBase<Real, N>, 
				Detail::VectorBase<Real, N>
				> operator-(const Point<Real, N>& that) const
			//Vector<N, Real> operator-(const Point<Real, N>& that) const
			{
				return data_ - that.data_;
			}

			//! Returns the point translated by 'that'.
			template <typename Expression>
			Point<Real, N> operator+(
				const VectorExpression
				<Real, N, Expression>& that) const
			{
				Point<Real, N> result((const Point<Real, N>&)*this);
				result += that;
				return result;
			}

			//! Returns the point translated backwards by 'that'.
			template <typename Expression>
			Point<Real, N> operator-(
				const VectorExpression
				<Real, N, Expression>& that) const
			{
				Point<Real, N> result((const Point<Real, N>&)*this);
				result -= that;
				return result;
			}

			//! Interprets the point as a tuple.
			Tuple<Real, N>& asTuple()
			{
				return data_.asTuple();
			}

			//! Interprets the point as a Tuple.
			const Tuple<Real, N>& asTuple() const
			{
				return data_.asTuple();
			}

			//! Interprets the point as a position vector.
			Vector<Real, N>& asVector()
			{
				return data_;
			}

			//! Interprets the point as a position vector.
			const Vector<Real, N>& asVector() const
			{
				return data_;
			}

		private:
			Vector<Real, N> data_;
		};

	}

}

#endif
