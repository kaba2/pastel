// Description: Vector class
// Detail: A vector in R^n

#ifndef PASTELSYS_VECTOR_H
#define PASTELSYS_VECTOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/hashing.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/memory_overlaps.h"
#include "pastel/sys/vector/vectorexpression.h"

#include <boost/operators.hpp>

#include <algorithm>

namespace Pastel
{

	template <typename Real, integer N = Dynamic>
	class Vector
		: public VectorExpression<Real, N, Vector<Real, N>>
	{
	private:
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);

		template <typename, integer>
		friend class Vector;

	public:
		using StorageType = const Vector&;
		//using ExpressionBase::operator-;

		using Iterator = typename Tuple<Real, N>::Iterator;
		using ConstIterator = typename Tuple<Real, N>::ConstIterator;

		//! Constructs a zero vector.
		/*!
		This constructor is disabled when N == Dynamic;
		in this case the dimension must be specified.

		Visual Studio 2015 RC has a bug which is
		triggered _at link-time_ by the following 
		constraint. 
		FIX: Uncomment the following constraint when 
		it is fixed in Visual Studio.
		*/
		#ifndef _MSC_VER
		template <
			int N_ = N,
			RequiresC<(N_ >= 0)> = 0
		>
		#endif
		Vector()
			: data_(ofDimension(N), 0)
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

		template <typename ThatReal, integer ThatN>
		explicit Vector(const Tuple<ThatReal, ThatN>& that)
			: data_(that)
		{
			PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);
		}

		template <typename ThatReal, integer ThatN>
		Vector(const Vector<ThatReal, ThatN>& that)
			: data_(that.data_)
		{
			PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);
		}

		Vector(const Vector& that)
			: data_(that.data_)
		{
		}

		template <
			integer N_ = N,
			RequiresC<(N_ > 1)> = 0>
		explicit Vector(const Real& that)
			: data_(that)
		{
		}

		// Allow implicit conversion for 1-d vectors.
		template <
			integer N_ = N,
			RequiresC<(N_ == 1)> = 0>
		Vector(const Real& that)
			: data_(that)
		{
		}

		explicit Vector(const Copy<const Real*>& that)
			: data_(that)
		{
			PASTEL_STATIC_ASSERT(N != Dynamic);
		}

		template <typename ThatReal, integer ThatN, typename Expression>
		Vector(const VectorExpression<ThatReal, ThatN, Expression>& that)
			: data_(ofDimension(that.size()))
		{
			PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);

			integer n = that.size();
			Iterator iter = begin();
			for (integer i = 0;i < n;++i)
			{

				*iter = that[i];
				++iter;
			}
		}

		template <
			integer N_ = N,
			RequiresC<(N_ == 2)> = 0>
		Vector(const Real& x, const Real& y)
		{
			set(x, y);
		}

		template <
			integer N_ = N,
			RequiresC<(N_ == 3)> = 0>
		Vector(const Real& x, const Real& y, const Real& z)
		{
			set(x, y, z);
		}

		template <
			integer N_ = N,
			RequiresC<(N_ == 4)> = 0>
		Vector(const Real& x, const Real& y, 
			const Real& z, const Real& w)
		{
			set(x, y, z, w);
		}

		Vector& operator=(const Vector& that)
		{
			// We allow the size of the vector to be
			// changed by an assignment.

			integer n = that.size();
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

		template <typename Type>
		Vector& operator=(const Type& that)
		{
			return assign(that);
		}

		Vector& operator=(const std::initializer_list<Real>& that)
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

			return *this;
		}

		/*
		template <typename ThatReal, integer ThatN, typename Expression>
		DisableIf<
			(std::is_same<Expression, Vector<ThatReal, ThatN> >>,
			(Vector<Real, N>&)) assign(
			const VectorExpression<ThatReal, ThatN, Expression>& that)
		*/
		template <typename ThatReal, integer ThatN, typename Expression>
		Vector<Real, N>& assign(
			const VectorExpression<ThatReal, ThatN, Expression>& that)
		{
			PASTEL_STATIC_ASSERT(ThatN == N || N == Dynamic || ThatN == Dynamic);

			// We allow the size of the vector to be
			// changed by an assignment.

			integer n = that.size();
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

		Vector<Real, N>& assign(const std::initializer_list<Real>& that)
		{
			data_ = that;
			return *this;
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
			Iterator iterEnd = end();

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
			Iterator iterEnd = end();

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
			Iterator iterEnd = end();

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

		template <typename ThatReal, integer ThatN, typename Expression>
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
				integer n = size();
				for (integer i = 0;i < n;++i)
				{

					*iter += that[i];
					++iter;
				}
			}

			return (Vector<Real, N>&)*this;
		}

		template <typename ThatReal, integer ThatN, typename Expression>
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
				integer n = size();
				for (integer i = 0;i < n;++i)
				{

					*iter -= that[i];
					++iter;
				}
			}

			return (Vector<Real, N>&)*this;
		}

		template <typename ThatReal, integer ThatN, typename Expression>
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
				integer n = size();
				for (integer i = 0;i < n;++i)
				{

					*iter *= that[i];
					++iter;
				}
			}

			return (Vector<Real, N>&)*this;
		}

		template <typename ThatReal, integer ThatN, typename Expression>
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
				integer n = size();
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

		template <
			integer N_ = N,
			RequiresC<(N_ == 2)> = 0>
		void set(
			const Real& x, const Real& y)
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
		}

		template <
			integer N_ = N,
			RequiresC<(N_ == 3)> = 0>
		void set(
			const Real& x, const Real& y, 
			const Real& z)
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
			v[2] = z;
		}

		template <
			integer N_ = N,
			RequiresC<(N_ == 4)> = 0>
		void set(
			const Real& x, const Real& y, 
			const Real& z, const Real& w)
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}

		template <
			integer N_ = N,
			RequiresC<(N_ >= 1)> = 0>
		Real& x()
		{
			return (*this)[0];
		}

		template <
			integer N_ = N,
			RequiresC<(N_ >= 1)> = 0>
		const Real& x() const
		{
			return (*this)[0];
		}

		template <
			integer N_ = N,
			RequiresC<(N_ >= 2)> = 0>
		Real& y()
		{
			return (*this)[1];
		}

		template <
			integer N_ = N,
			RequiresC<(N_ >= 2)> = 0>
		const Real& y() const
		{
			return (*this)[1];
		}

		template <
			integer N_ = N,
			RequiresC<(N_ >= 3)> = 0>
		Real& z()
		{
			return (*this)[2];
		}

		template <
			integer N_ = N,
			RequiresC<(N_ >= 3)> = 0>
		const Real& z() const
		{
			return (*this)[2];
		}

		template <
			integer N_ = N,
			RequiresC<(N_ >= 4)> = 0>
		Real& w()
		{
			return (*this)[3];
		}

		template <
			integer N_ = N,
			RequiresC<(N_ >= 4)> = 0>
		const Real& w() const
		{
			return (*this)[3];
		}

	protected:
		Tuple<Real, N> data_;
	};

	using Vector1 = Vector<real, 1>;
	using Vector2 = Vector<real, 2>;
	using Vector3 = Vector<real, 3>;
	using Vector4 = Vector<real, 4>;
	using VectorD = Vector<real, Dynamic>;

	using Vector1i = Vector<integer, 1>;
	using Vector2i = Vector<integer, 2>;
	using Vector3i = Vector<integer, 3>;
	using Vector4i = Vector<integer, 4>;
	using VectorDi = Vector<integer, Dynamic>;

	using UnitVector1 = Vector1;
	using UnitVector2 = Vector2;
	using UnitVector3 = Vector3;
	using UnitVector4 = Vector4;

	template <typename Real, integer N>
	void swap(Vector<Real, N>& left,
		Vector<Real, N>& right);

	template <typename Real, integer N, typename Expression>
	Vector<Real, N> evaluate(
		const VectorExpression<Real, N, Expression>& that);

	template <typename Real, integer N>
	Tuple<Real, N>& asTuple(
		Vector<Real, N>& that);

	template <typename Real, integer N>
	const Tuple<Real, N>& asTuple(
		const Vector<Real, N>& that);

}

namespace std
{

	template <typename Real, Pastel::integer N>
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

#include "pastel/sys/vector/vector.hpp"

#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/vector/vector_compare.h"
#include "pastel/sys/vector/vector_locator.h"

#endif
