#ifndef PASTELSYS_POINTBASE_H
#define PASTELSYS_POINTBASE_H

#include "pastel/sys/point.h"

#include "pastel/sys/vector.h"

#include <boost/static_assert.hpp>
#include <boost/operators.hpp>

namespace Pastel
{

	namespace Detail
	{

		template <int N, typename Real, typename MyDerived>
		class PointBase
			: boost::equality_comparable<
			MyDerived>
		{
		private:
			template <int N, typename Real, typename MyDerived>
			friend class PointBase;

			typedef MyDerived Derived;

		public:
			//! Constructs with all elements set to zero.
			PointBase()
				: data_(0)
			{
			}

			template <typename ThatReal>
			PointBase(const Tuple<N, ThatReal>& that)
				: data_(that)
			{
			}

			template <typename ThatReal, typename ThatDerived>
			PointBase(const PointBase<N, ThatReal, ThatDerived>& that)
				: data_(that.data_)
			{
			}

			//! Constructs with the given point.
			PointBase(const PointBase& that)
				: data_(that.data_)
			{
			}

			//! Constructs with all elements set to the given value.
			explicit PointBase(const Real& that)
				: data_(that)
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
				BOOST_STATIC_ASSERT(N > 0);
			}

			integer size() const
			{
				return N;
			}

			//! Swaps two points.
			void swap(Derived& that)
			{
				data_.swap(that.data_);
			}

			//! Sets all of the elements to the given value.
			void set(const Real& that)
			{
				data_.set(that);
			}

			bool operator==(const Derived& that) const
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
			Derived& operator+=(const Real& that)
			{
				data_ += that;
				return (Derived&)*this;
			}

			//! Returns the point added by 'that' to all elements.
			Derived operator+(const Real& that) const
			{
				Derived result((Derived&)*this);
				result += that;
				return result;
			}

			//! Subtracts the given value from all elements.
			Derived& operator-=(const Real& that)
			{
				data_ -= that;
				return (Derived&)*this;
			}

			//! Returns the point subtracted by 'that' from all elements.
			Derived operator-(const Real& that) const
			{
				Derived result((Derived&)*this);
				result -= that;
				return result;
			}

			//! Translates the point by 'that'.
			template <typename Expression>
			Derived& operator+=(
				const VectorExpression
				<N, Real, Expression>& that)
			{
				data_ += that;

				return (Derived&)*this;
			}

			//! Translates the point backwards by 'that'.
			template <typename Expression>
			Derived& operator-=(
				const VectorExpression
				<N, Real, Expression>& that)
			{
				data_ -= that;

				return (Derived&)*this;
			}

			//! Returns the difference vector between two points.
			Vector<N, Real> operator-(
				const Derived& that) const
			{
				return data_ - that.data_;
			}

			//! Returns the point translated by 'that'.
			template <typename Expression>
			Derived operator+(
				const VectorExpression
				<N, Real, Expression>& that) const
			{
				Derived result((const Derived&)*this);
				result += that;
				return result;
			}

			//! Returns the point translated backwards by 'that'.
			template <typename Expression>
			Derived operator-(
				const VectorExpression
				<N, Real, Expression>& that) const
			{
				Derived result((const Derived&)*this);
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

		private:
			Vector<N, Real> data_;
		};

	}

}

#endif
