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

	template <int N, typename Real>
	class Vector;

	template <int N, typename Real>
	class TemporaryVector;

	namespace Detail
	{

		template <int N, typename Real>
		class VectorBase
			: public boost::equality_comparable1<Vector<N, Real>,
			VectorExpression<N, Real, VectorBase<N, Real> >
			>
		{
		private:
			template <int N, typename Real>
			friend class VectorBase;

		public:
			typedef const VectorBase& StorageType;
			//using ExpressionBase::operator-;

			typedef typename Tuple<N, Real>::Iterator Iterator;
			typedef typename Tuple<N, Real>::ConstIterator ConstIterator;

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
			explicit VectorBase(const Tuple<N, ThatReal>& that)
				: data_(that)
			{
			}

			template <typename ThatReal>
			VectorBase(const VectorBase<N, ThatReal>& that)
				: data_(that.data_)
			{
			}

			VectorBase(const VectorBase& that)
				: data_(that.data_)
			{
			}

			explicit VectorBase(
				const TemporaryVector<N, Real>& that)
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
				<N, ThatReal, Expression>& that)
				: data_(ofDimension(that.size()))
			{
				*this = that;
			}

			~VectorBase()
			{
				enum
				{
					IsBase = boost::is_base_of<VectorBase, Vector<N, Real> >::value
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

			void swap(VectorBase<N, Real>& that)
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
			Vector<N, Real>& operator=(const Real that)
			{
				// We accept basic exception safety for performance.
				data_.set(that);

				return (Vector<N, Real>&)*this;
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
			Vector<N, Real>& operator=(
				const TemporaryVector<N, Real>& that)
			{
				//...
			}
			*/

			Vector<N, Real>& operator=(
				const VectorBase<N, Real>& that)
			{
				// We allow the size of the vector to be
				// changed by an assignment.

				const integer n = that.size();
				if (n != size())
				{
					// In the case we must reallocate, we can
					// as well copy construct, so that there
					// is no redundant initialization.

					Vector<N, Real> copy(that);
					swap(copy);
				}
				else
				{				
					// We accept basic exception safety for performance.

					data_ = that.data_;
				}

				return (Vector<N, Real>&)*this;
			}

			/*
			template <typename ThatReal, typename Expression>
			typename boost::disable_if<
				boost::is_same<Expression, VectorBase<N, Real> >,
				Vector<N, Real>&>::type operator=(
				const VectorExpression<N, ThatReal, Expression>& that)
			*/
			template <typename ThatReal, typename Expression>
			Vector<N, Real>& operator=(
				const VectorExpression<N, ThatReal, Expression>& that)
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

					Vector<N, Real> copy(that);
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

				return (Vector<N, Real>&)*this;
			}

			bool operator==(const Vector<N, Real>& that) const
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
			Vector<N, Real>& operator+=(const Real that)
			{
				Iterator iter = begin();
				const Iterator iterEnd = end();

				while(iter != iterEnd)
				{
					*iter += that;
					++iter;
				}

				return (Vector<N, Real>&)*this;
			}

			// The parameter to this function
			// is deliberately not a reference,
			// because the reference could point
			// to this vector.
			Vector<N, Real>& operator-=(const Real that)
			{
				Iterator iter = begin();
				const Iterator iterEnd = end();

				while(iter != iterEnd)
				{
					*iter -= that;
					++iter;
				}

				return (Vector<N, Real>&)*this;
			}

			// The parameter to this function
			// is deliberately not a reference,
			// because the reference could point
			// to this vector.
			Vector<N, Real>& operator*=(const Real that)
			{
				Iterator iter = begin();
				const Iterator iterEnd = end();

				while(iter != iterEnd)
				{
					*iter *= that;
					++iter;
				}

				return (Vector<N, Real>&)*this;
			}

			// Here the reference is ok because we actually
			// use the parameter's inverse.
			Vector<N, Real>& operator/=(const Real& that)
			{
				return (*this *= Pastel::inverse(that));
			}

			template <typename ThatReal, typename Expression>
			Vector<N, Real>& operator+=(
				const VectorExpression<N, ThatReal, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.involvesNonTrivially(&*data_.begin(), &*data_.end()))
				{
					*this += Vector<N, Real>(that);
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

				return (Vector<N, Real>&)*this;
			}

			template <typename ThatReal, typename Expression>
			Vector<N, Real>& operator-=(
				const VectorExpression<N, ThatReal, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.involvesNonTrivially(&*data_.begin(), &*data_.end()))
				{
					*this -= Vector<N, Real>(that);
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

				return (Vector<N, Real>&)*this;
			}

			template <typename ThatReal, typename Expression>
			Vector<N, Real>& operator*=(
				const VectorExpression<N, ThatReal, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.involvesNonTrivially(&*data_.begin(), &*data_.end()))
				{
					*this *= Vector<N, Real>(that);
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

				return (Vector<N, Real>&)*this;
			}

			template <typename ThatReal, typename Expression>
			Vector<N, Real>& operator/=(
				const VectorExpression<N, ThatReal, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				if (that.involvesNonTrivially(&*data_.begin(), &*data_.end()))
				{
					*this /= Vector<N, Real>(that);
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

				return (Vector<N, Real>&)*this;
			}

			TemporaryVector<N, Real>& asTemporary()
			{
				return (TemporaryVector<N, Real>&)*this;
			}

			Tuple<N, Real>& asTuple()
			{
				return data_;
			}

			const Tuple<N, Real>& asTuple() const
			{
				return data_;
			}

		// Need to be protected instead of private
		// so that Vector<Dynamic, Real> can access
		// data_ to implement setSize().
		protected:
			Tuple<N, Real> data_;
		};

	}

}

#endif
