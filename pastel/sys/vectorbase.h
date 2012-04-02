// Description: VectorBase class
// Detail: CRTP base class for Vector

#ifndef PASTEL_VECTORBASE_H
#define PASTEL_VECTORBASE_H

#include "pastel/sys/vector.h"
#include "pastel/sys/vectorexpression.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/tablemodify.h"
#include "pastel/sys/commafiller.h"
#include "pastel/sys/memory_overlaps.h"

#include <algorithm>

#include <boost/operators.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace Pastel
{

	template <typename Real, int N = Dynamic>
	class Vector;

	namespace Detail
	{

		template <typename Real, int N>
		class VectorBase
			: public boost::equality_comparable1<Vector<Real, N>,
			VectorExpression<Real, N, VectorBase<Real, N> >
			>
		{
		private:
			template <typename, int>
			friend class VectorBase;

		public:
			typedef const VectorBase& StorageType;
			//using ExpressionBase::operator-;

			typedef typename Tuple<Real, N>::Iterator Iterator;
			typedef typename Tuple<Real, N>::ConstIterator ConstIterator;

			VectorBase()
				: data_(ofDimension((N == Dynamic) ? 0 : N), 0)
			{
			}

			explicit VectorBase(
				const Dimension& dimension,
				const Real& that = Real())
				: data_(dimension, that)
			{
			}

			explicit VectorBase(
				const Dimension& dimension,
				const Copy<const Real*>& that)
				: data_(dimension, that)
			{
			}

			explicit VectorBase(
				const Dimension& dimension,
				const Alias<Real*> alias)
				: data_(dimension, alias)
			{
			}

			template <typename ThatReal, int ThatN>
			explicit VectorBase(const Tuple<ThatReal, ThatN>& that)
				: data_(that)
			{
				PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);
			}

			template <typename ThatReal, int ThatN>
			VectorBase(const VectorBase<ThatReal, ThatN>& that)
				: data_(that.data_)
			{
				PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);
			}

			VectorBase(const VectorBase& that)
				: data_(that.data_)
			{
			}

			explicit VectorBase(const Real& that)
				: data_(that)
			{
			}

			explicit VectorBase(const Copy<const Real*>& that)
				: data_(that)
			{
				PASTEL_STATIC_ASSERT(N != Dynamic);
			}

			template <typename ThatReal, int ThatN, typename Expression>
			explicit VectorBase(
				const VectorExpression
				<ThatReal, ThatN, Expression>& that)
				: data_(ofDimension(that.size()))
			{
				PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);

				const integer n = that.size();
				Iterator iter = begin();
				for (integer i = 0;i < n;++i)
				{
					*iter = that[i];
					++iter;
				}
			}

			~VectorBase()
			{
				enum
				{
					IsBase = std::is_base_of<VectorBase, Vector<Real, N> >::value
				};

				PASTEL_STATIC_ASSERT(IsBase);
				PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
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

			Iterator begin()
			{
				return data_.begin();
			}

			ConstIterator begin() const
			{
				return data_.begin();
			}

			Iterator end()
			{
				return data_.end();
			}

			ConstIterator end() const
			{
				return data_.end();
			}

			bool involves(
				const void* memoryBegin,
				const void* memoryEnd) const
			{
				return Pastel::memoryOverlaps(
					memoryBegin, memoryEnd,
					&*begin(), &*end());
			}

			bool evaluateBeforeAssignment(
				const void* memoryBegin,
				const void* memoryEnd) const
			{
				return false;
			}

			void swap(VectorBase<Real, N>& that)
			{
				data_.swap(that.data_);
			}

			void set(const Real& that)
			{
				data_.set(that);
			}

			// The parameter to this function
			// is deliberately not a reference,
			// because the reference could point
			// to this vector.
			Vector<Real, N>& assign(const Real that)
			{
				// We accept basic exception safety for performance.
				data_.set(that);

				return (Vector<Real, N>&)*this;
			}

			CommaFiller<Real, Iterator> operator|=(
				const Real& that)
			{
				return data_ |= that;
			}

			// This function can't be inherited as operator=().
			Vector<Real, N>& assign(
				const Vector<Real, N>& that)
			{
				// We allow the size of the vector to be
				// changed by an assignment.

				const integer n = that.size();
				if (n != size())
				{
					// In the case we must reallocate, we can
					// as well copy construct, so that there
					// is no redundant initialization.

					Vector<Real, N> copy(that);
					swap(copy);
				}
				else
				{				
					// We accept basic exception safety for performance.

					data_ = ((const VectorBase&)that).data_;
				}

				return (Vector<Real, N>&)*this;
			}

			template <typename ThatReal, int ThatN, typename Expression>
			PASTEL_DISABLE_IF(
				(std::is_same<Expression, VectorBase<ThatReal, ThatN> >),
				(Vector<Real, N>&)) assign(
				const VectorExpression<ThatReal, ThatN, Expression>& that)
			/*
			template <typename ThatReal, int ThatN, typename Expression>
			Vector<Real, N>& assign(
				const VectorExpression<ThatReal, ThatN, Expression>& that)
			*/
			{
				PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);

				// We allow the size of the vector to be
				// changed by an assignment.

				const integer n = that.size();
				if (n != size() ||
					that.evaluateBeforeAssignment(
					&*data_.begin(), &*data_.end()))
				{
					// In the case we must reallocate, we can
					// as well copy construct, so that there
					// is no redundant initialization.
					
					// Of course, if the expression involves
					// this vector as a non-trivial subexpression,
					// we must copy construct anyway.

					Vector<Real, N> copy(that);
					swap(copy);
				}
				else
				{				
					// We accept basic exception safety for performance.

					Iterator iter = begin();
					for (integer i = 0;i < n;++i)
					{
						*iter = that[i];
						++iter;
					}
				}

				return (Vector<Real, N>&)*this;
			}

			bool operator==(const Vector<Real, N>& that) const
			{
				return data_ == that.data_;
			}

			Real& operator[](integer index)
			{
				return data_[index];
			}

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

			// The parameter to this function
			// is deliberately not a reference,
			// because the reference could point
			// to this vector.
			Vector<Real, N>& operator+=(const Real that)
			{
				Iterator iter = begin();
				const Iterator iterEnd = end();

				while(iter != iterEnd)
				{
					*iter += that;
					++iter;
				}

				return (Vector<Real, N>&)*this;
			}

			// The parameter to this function
			// is deliberately not a reference,
			// because the reference could point
			// to this vector.
			Vector<Real, N>& operator-=(const Real that)
			{
				Iterator iter = begin();
				const Iterator iterEnd = end();

				while(iter != iterEnd)
				{
					*iter -= that;
					++iter;
				}

				return (Vector<Real, N>&)*this;
			}

			// The parameter to this function
			// is deliberately not a reference,
			// because the reference could point
			// to this vector.
			Vector<Real, N>& operator*=(const Real that)
			{
				Iterator iter = begin();
				const Iterator iterEnd = end();

				while(iter != iterEnd)
				{
					*iter *= that;
					++iter;
				}

				return (Vector<Real, N>&)*this;
			}

			// Here the reference is ok because we actually
			// use the parameter's inverse.
			Vector<Real, N>& operator/=(const Real& that)
			{
				return (*this *= Pastel::inverse(that));
			}

			template <typename ThatReal, int ThatN, typename Expression>
			Vector<Real, N>& operator+=(
				const VectorExpression<ThatReal, ThatN, Expression>& that)
			{
				PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.evaluateBeforeAssignment(&*data_.begin(), &*data_.end()))
				{
					*this += Vector<Real, N>(that);
				}
				else
				{
					Iterator iter = begin();
					const integer n = size();
					for (integer i = 0;i < n;++i)
					{
						*iter += that[i];
						++iter;
					}
				}

				return (Vector<Real, N>&)*this;
			}

			template <typename ThatReal, int ThatN, typename Expression>
			Vector<Real, N>& operator-=(
				const VectorExpression<ThatReal, ThatN, Expression>& that)
			{
				PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.evaluateBeforeAssignment(&*data_.begin(), &*data_.end()))
				{
					*this -= Vector<Real, N>(that);
				}
				else
				{
					Iterator iter = begin();
					const integer n = size();
					for (integer i = 0;i < n;++i)
					{
						*iter -= that[i];
						++iter;
					}
				}

				return (Vector<Real, N>&)*this;
			}

			template <typename ThatReal, int ThatN, typename Expression>
			Vector<Real, N>& operator*=(
				const VectorExpression<ThatReal, ThatN, Expression>& that)
			{
				PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.evaluateBeforeAssignment(&*data_.begin(), &*data_.end()))
				{
					*this *= Vector<Real, N>(that);
				}
				else
				{
					Iterator iter = begin();
					const integer n = size();
					for (integer i = 0;i < n;++i)
					{
						*iter *= that[i];
						++iter;
					}
				}

				return (Vector<Real, N>&)*this;
			}

			template <typename ThatReal, int ThatN, typename Expression>
			Vector<Real, N>& operator/=(
				const VectorExpression<ThatReal, ThatN, Expression>& that)
			{
				PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.evaluateBeforeAssignment(&*data_.begin(), &*data_.end()))
				{
					*this /= Vector<Real, N>(that);
				}
				else
				{
					Iterator iter = begin();
					const integer n = size();
					for (integer i = 0;i < n;++i)
					{
						*iter /= that[i];
						++iter;
					}
				}

				return (Vector<Real, N>&)*this;
			}

			Tuple<Real, N>& asTuple()
			{
				return data_;
			}

			const Tuple<Real, N>& asTuple() const
			{
				return data_;
			}

		// Need to be protected instead of private
		// so that Vector<Real, Dynamic> can access
		// data_ to implement setSize().
		protected:
			Tuple<Real, N> data_;
		};

	}

}

#endif
