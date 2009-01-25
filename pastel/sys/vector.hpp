#ifndef PASTELSYS_VECTOR_HPP
#define PASTELSYS_VECTOR_HPP

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real>
	class Vector<Unbounded, Real>
		: public VectorExpression<Unbounded, Real,
		Vector<Unbounded, Real> >
		, boost::equality_comparable<
		Vector<Unbounded, Real> >
	{
	private:
        enum
        {
            N = Unbounded
        };

        typedef VectorExpression<N, Real, Vector<Unbounded, Real> > Base;

	public:
		using Base::operator-;

		Vector()
			: Base()
			, data_(0)
		{
		}

		Vector(const Vector& that)
			: Base()
			, data_(that.data_)
		{
		}

		explicit Vector(const Real& x)
			: Base()
			, data_(x)
		{
		}

		template <typename Expression>
		explicit Vector(
			const VectorExpression
			<N, Real, Expression>& that)
			: Base()
			, data_()
		{
            *this = that;
		}

		~Vector()
		{
		}

		operator Tuple<N, Real>&()
		{
			return data_;
		}

		operator const Tuple<N, Real>&() const
		{
			return data_;
		}

        void setSize(integer size)
        {
            ENSURE1(size >= 0, size);
            data_.setSize(size);
        }

        integer size() const
        {
            return data_.size();
        }

		void swap(Vector& that)
		{
            data_.swap(that.data_);
		}

        void clear()
        {
            data_.clear();
        }

		void set(const Real& x)
		{
            data_ = x;
		}

		Vector& operator=(const Real& that)
		{
            data_ = that;

			return *this;
		}

		template <typename Expression>
		Vector& operator=(
			const VectorExpression<N, Real, Expression>& that)
		{
            const integer n = size();
            for (integer i = 0;i < n;++i)
            {
                data_[i] = that[i];
            }

			return *this;
		}

		bool operator==(const Vector& that) const
		{
            const integer n = size();

			for (integer i = 0;i < n;++i)
			{
				if (data_[i] != that.data_[i])
				{
					return false;
				}
			}

			return true;
		}

		Real& operator[](integer index)
		{
			return data_[index];
		}

		const Real& operator[](integer index) const
		{
			return data_[index];
		}

		Vector& operator+=(const Real& that)
		{
            const integer n = size();
            for (integer i = 0;i < n;++i)
            {
                data_[i] += that;
            }

			return *this;
		}

		Vector& operator-=(const Real& that)
		{
            const integer n = size();
            for (integer i = 0;i < n;++i)
            {
                data_[i] -= that;
            }

			return *this;
		}

		Vector& operator*=(const Real& that)
		{
            const integer n = size();
            for (integer i = 0;i < n;++i)
            {
                data_[i] *= that;
            }

			return *this;
		}

		Vector& operator/=(const Real& that)
		{
			Real invThat(Pastel::inverse(that));

            const integer n = size();
            for (integer i = 0;i < n;++i)
            {
                data_[i] *= invThat;
            }

			return *this;
		}

		template <typename Expression>
		Vector& operator+=(
			const VectorExpression<N, Real, Expression>& that)
		{
            const integer n = size();
            for (integer i = 0;i < n;++i)
            {
                data_[i] += that[i];
            }

			return *this;
		}

		template <typename Expression>
		Vector& operator-=(
			const VectorExpression<N, Real, Expression>& that)
		{
            const integer n = size();
            for (integer i = 0;i < n;++i)
            {
                data_[i] -= that[i];
            }

			return *this;
		}

		template <typename Expression>
		Vector& operator*=(
			const VectorExpression<N, Real, Expression>& that)
		{
            const integer n = size();
            for (integer i = 0;i < n;++i)
            {
                data_[i] *= that[i];
            }

			return *this;
		}

		template <typename Expression>
		Vector& operator/=(
			const VectorExpression<N, Real, Expression>& that)
		{
            const integer n = size();
            for (integer i = 0;i < n;++i)
            {
                data_[i] /= that[i];
            }

			return *this;
		}

	private:
		Tuple<N, Real> data_;
	};

    template <int N, typename Real>
	void swap(Vector<N, Real>& left,
		Vector<N, Real>& right)
	{
        left.swap(right);
	}

	template <int N, typename Real, typename Expression>
	Vector<N, Real> evaluate(
		const VectorExpression<N, Real, Expression>& that)
	{
		return Vector<N, Real>(that);
	}

	template <int N, typename Real>
	Tuple<N, Real> asTuple(Vector<N, Real>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	const Tuple<N, Real> asTuple(const Vector<N, Real>& that)
	{
		return that.asTuple();
	}

}

#endif
