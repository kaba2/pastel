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
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	template <typename Real, int N>
	class Vector;

	template <typename Real, int N>
	class TemporaryVector;

	namespace Detail
	{

		template <typename Real, int N>
		class VectorBase
			: public boost::equality_comparable1<Vector<Real, N>,
			VectorExpression<Real, N, VectorBase<Real, N> >
			>
		{
		private:
			template <typename Real, int N>
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
				const Alias<Real*> alias)
				: data_(dimension, alias)
			{
			}

			template <typename ThatReal>
			explicit VectorBase(const Tuple<ThatReal, N>& that)
				: data_(that)
			{
			}

			template <typename ThatReal>
			VectorBase(const VectorBase<ThatReal, N>& that)
				: data_(that.data_)
			{
			}

			VectorBase(const VectorBase& that)
				: data_(that.data_)
			{
			}

			explicit VectorBase(
				const TemporaryVector<Real, N>& that)
				: data_(that.asTuple())
			{
			}

			explicit VectorBase(const Real& x)
				: data_(x)
			{
			}

			template <typename ThatReal, typename Expression>
			explicit VectorBase(
				const VectorExpression
				<ThatReal, N, Expression>& that)
				: data_(ofDimension(that.size()))
			{
				*this = that;
			}

			~VectorBase()
			{
				enum
				{
					IsBase = boost::is_base_of<VectorBase, Vector<Real, N> >::value
				};

				BOOST_STATIC_ASSERT(IsBase);
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

			bool involvesNonTrivially(
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
			Vector<Real, N>& operator=(const Real that)
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

			// This function can't be included because
			// then assignment of a vector expression would
			// be ambiguous. This is because TemporaryVector
			// has an implicit conversion from a vector
			// expression.
			/*
			Vector<Real, N>& operator=(
				const TemporaryVector<Real, N>& that)
			{
				//...
			}
			*/

			Vector<Real, N>& operator=(
				const VectorBase<Real, N>& that)
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

					data_ = that.data_;
				}

				return (Vector<Real, N>&)*this;
			}

			/*
			template <typename ThatReal, typename Expression>
			typename boost::disable_if<
				boost::is_same<Expression, VectorBase<Real, N> >,
				Vector<Real, N>&>::type operator=(
				const VectorExpression<ThatReal, N, Expression>& that)
			*/
			template <typename ThatReal, typename Expression>
			Vector<Real, N>& operator=(
				const VectorExpression<ThatReal, N, Expression>& that)
			{
				// We allow the size of the vector to be
				// changed by an assignment.

				const integer n = that.size();
				if (n != size() ||
					that.involvesNonTrivially(
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

			template <typename ThatReal, typename Expression>
			Vector<Real, N>& operator+=(
				const VectorExpression<ThatReal, N, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.involvesNonTrivially(&*data_.begin(), &*data_.end()))
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

			template <typename ThatReal, typename Expression>
			Vector<Real, N>& operator-=(
				const VectorExpression<ThatReal, N, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.involvesNonTrivially(&*data_.begin(), &*data_.end()))
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

			template <typename ThatReal, typename Expression>
			Vector<Real, N>& operator*=(
				const VectorExpression<ThatReal, N, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.involvesNonTrivially(&*data_.begin(), &*data_.end()))
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

			template <typename ThatReal, typename Expression>
			Vector<Real, N>& operator/=(
				const VectorExpression<ThatReal, N, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.involvesNonTrivially(&*data_.begin(), &*data_.end()))
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

			TemporaryVector<Real, N>& asTemporary()
			{
				return (TemporaryVector<Real, N>&)*this;
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
