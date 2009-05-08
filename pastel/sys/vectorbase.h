#ifndef PASTELSYS_VECTORBASE_H
#define PASTELSYS_VECTORBASE_H

#include "pastel/sys/vector.h"
#include "pastel/sys/vectorexpression.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/tablemodify.h"

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

			VectorBase()
				: data_(0)
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
				const integer n = that.size();
			
				for (integer i = 0;i < n;++i)
				{
					data_[i] = that[i];
				}
			}

			~VectorBase()
			{
				enum
				{
					IsBase = boost::is_base_of<VectorBase, Vector<N, Real> >::value
				};

				BOOST_STATIC_ASSERT(IsBase);
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

			void swap(VectorBase<N, Real>& that)
			{
				data_.swap(that.data_);
			}

			void set(const Real& x)
			{
				*this = x;
			}

			// The parameter to this function
			// is deliberately not a reference,
			// because the reference could point
			// to this vector.
			Vector<N, Real>& operator=(const Real that)
			{
				const integer n = size();
				
				for (integer i = 0;i < n;++i)
				{
					data_[i] = that;
				}

				return (Vector<N, Real>&)*this;
			}

			template <typename ThatReal, typename Expression>
			Vector<N, Real>& operator=(
				const VectorExpression<N, ThatReal, Expression>& that)
			{
				Vector<N, Real> copy(that);
				swap(copy);
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
				const integer n = size();
				
				for (integer i = 0;i < n;++i)
				{
					data_[i] += that;
				}

				return (Vector<N, Real>&)*this;
			}

			// The parameter to this function
			// is deliberately not a reference,
			// because the reference could point
			// to this vector.
			Vector<N, Real>& operator-=(const Real that)
			{
				const integer n = size();
				
				for (integer i = 0;i < n;++i)
				{
					data_[i] -= that;
				}

				return (Vector<N, Real>&)*this;
			}

			// The parameter to this function
			// is deliberately not a reference,
			// because the reference could point
			// to this vector.
			Vector<N, Real>& operator*=(const Real that)
			{
				const integer n = size();
				
				for (integer i = 0;i < n;++i)
				{
					data_[i] *= that;
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

				const integer n = size();
				
				for (integer i = 0;i < n;++i)
				{
					data_[i] += that[i];
				}

				return (Vector<N, Real>&)*this;
			}

			template <typename ThatReal, typename Expression>
			Vector<N, Real>& operator-=(
				const VectorExpression<N, ThatReal, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				const integer n = size();
				
				for (integer i = 0;i < n;++i)
				{
					data_[i] -= that[i];
				}

				return (Vector<N, Real>&)*this;
			}

			template <typename ThatReal, typename Expression>
			Vector<N, Real>& operator*=(
				const VectorExpression<N, ThatReal, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				const integer n = size();
				
				for (integer i = 0;i < n;++i)
				{
					data_[i] *= that[i];
				}

				return (Vector<N, Real>&)*this;
			}

			template <typename ThatReal, typename Expression>
			Vector<N, Real>& operator/=(
				const VectorExpression<N, ThatReal, Expression>& that)
			{
				PENSURE2(that.size() == size(), that.size(), size());

				const integer n = size();
				
				for (integer i = 0;i < n;++i)
				{
					data_[i] /= that[i];
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
		// so that Vector<Unbounded, Real> can access
		// data_ to implement setSize().
		protected:
			Tuple<N, Real> data_;
		};

	}

}

#endif
