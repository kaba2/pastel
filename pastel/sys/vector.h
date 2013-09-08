// Description: Vector class
// Detail: A vector in R^n

#ifndef PASTELSYS_VECTOR_H
#define PASTELSYS_VECTOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/hashing.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/commafiller.h"
#include "pastel/sys/memory_overlaps.h"
#include "pastel/sys/vectorexpression.h"

#include <boost/operators.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <algorithm>

namespace Pastel
{

	template <typename Real, int N = Dynamic>
	class Vector
		: public VectorExpression<Real, N, Vector<Real, N>>
	{
	private:
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);

		template <typename, int>
		friend class Vector;

		struct Enabler {};

	public:
		typedef const Vector& StorageType;
		//using ExpressionBase::operator-;

		typedef typename Tuple<Real, N>::Iterator Iterator;
		typedef typename Tuple<Real, N>::ConstIterator ConstIterator;

		Vector()
			: data_(ofDimension((N == Dynamic) ? 0 : N), 0)
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Real& that = Real())
			: data_(dimension, that)
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: data_(dimension, that)
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Alias<Real*> alias)
			: data_(dimension, alias)
		{
		}

		template <typename ThatReal, int ThatN>
		explicit Vector(const Tuple<ThatReal, ThatN>& that)
			: data_(that)
		{
			PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);
		}

		template <typename ThatReal, int ThatN>
		Vector(const Vector<ThatReal, ThatN>& that)
			: data_(that.data_)
		{
			PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);
		}

		Vector(const Vector& that)
			: data_(that.data_)
		{
		}

		template <int N_ = N>
		explicit Vector(const Real& that, 
			PASTEL_ENABLE_IF_C(N_ > 1, Enabler) = Enabler())
			: data_(that)
		{
		}

		// Allow implicit conversion for 1-d vectors.
		template <int N_ = N>
		Vector(const Real& that, 
			PASTEL_ENABLE_IF_C(N_ == 1, Enabler) = Enabler())
			: data_(that)
		{
		}

		explicit Vector(const Copy<const Real*>& that)
			: data_(that)
		{
			PASTEL_STATIC_ASSERT(N != Dynamic);
		}

		template <typename ThatReal, int ThatN, typename Expression>
		Vector(
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

		template <int N_ = N>
		Vector(const Real& x, const Real& y, 
			PASTEL_ENABLE_IF_C_P(N_ == 2))
		{
			set(x, y);
		}

		template <int N_ = N>
		Vector(const Real& x, const Real& y, const Real& z, 
			PASTEL_ENABLE_IF_C_P(N_ == 3))
		{
			set(x, y, z);
		}

		template <int N_ = N>
		Vector(const Real& x, const Real& y, 
			const Real& z, const Real& w,
			PASTEL_ENABLE_IF_C_P(N_ == 4))
		{
			set(x, y, z, w);
		}

		Vector& operator=(const Vector& that)
		{
			return assign(that);
		}

		template <typename Type>
		Vector& operator=(const Type& that)
		{
			return assign(that);
		}

		void setSize(integer size, const Real& that = Real())
		{
			data_.setSize(size, that);
		}

		integer size() const
		{
			return data_.size();
		}

		integer n() const
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

		void swap(Vector<Real, N>& that)
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

				data_ = that.data_;
			}

			return *this;
		}

		template <typename ThatReal, int ThatN, typename Expression>
		PASTEL_DISABLE_IF(
			(std::is_same<Expression, Vector<ThatReal, ThatN> >),
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

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ == 2, void) set(
			const Real& x, const Real& y)
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
		}

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ == 3, void) set(
			const Real& x, const Real& y, 
			const Real& z)
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
			v[2] = z;
		}

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ == 4, void) set(
			const Real& x, const Real& y, 
			const Real& z, const Real& w)
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ >= 1, Real&) x()
		{
			return (*this)[0];
		}

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ >= 1, const Real&) x() const
		{
			return (*this)[0];
		}

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ >= 2, Real&) y()
		{
			return (*this)[1];
		}

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ >= 2, const Real&) y() const
		{
			return (*this)[1];
		}

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ >= 3, Real&) z()
		{
			return (*this)[2];
		}

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ >= 3, const Real&) z() const
		{
			return (*this)[2];
		}

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ >= 4, Real&) w()
		{
			return (*this)[3];
		}

		template <int N_ = N>
		PASTEL_ENABLE_IF_C(N_ >= 4, const Real&) w() const
		{
			return (*this)[3];
		}

	protected:
		Tuple<Real, N> data_;
	};

	typedef Vector<real, 1> Vector1;
	typedef Vector<real, 2> Vector2;
	typedef Vector<real, 3> Vector3;
	typedef Vector<real, 4> Vector4;
	typedef Vector<real, Dynamic> VectorD;

	typedef Vector<integer, 1> Vector1i;
	typedef Vector<integer, 2> Vector2i;
	typedef Vector<integer, 3> Vector3i;
	typedef Vector<integer, 4> Vector4i;
	typedef Vector<integer, Dynamic> VectorDi;

	typedef Vector1 UnitVector1;
	typedef Vector2 UnitVector2;
	typedef Vector3 UnitVector3;
	typedef Vector4 UnitVector4;

	template <typename Real, int N>
	void swap(Vector<Real, N>& left,
		Vector<Real, N>& right);

	template <typename Real, int N, typename Expression>
	Vector<Real, N> evaluate(
		const VectorExpression<Real, N, Expression>& that);

	template <typename Real, int N>
	Tuple<Real, N>& asTuple(
		Vector<Real, N>& that);

	template <typename Real, int N>
	const Tuple<Real, N>& asTuple(
		const Vector<Real, N>& that);

}

namespace std
{

	template <typename Real, int N>
	struct hash<Pastel::Vector<Real, N>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Vector<Real, N>& that) const
		{
			return Pastel::computeHashMany(
				Pastel::range(that.begin(), that.end()));
		}
	};

}

#include "pastel/sys/vector.hpp"

#include "pastel/sys/vector_compare.h"

#endif
