#ifndef PASTELMATH_POLYNOMIAL_HPP
#define PASTELMATH_POLYNOMIAL_HPP

#include "pastel/math/polynomial/polynomial.h"

namespace Pastel
{

	template <typename Real>
	Polynomial<Real>::Polynomial(integer size)
		: data_(size, Real(0))
		, epsilon_(0.001)
	{
	}

	template <typename Real>
	void Polynomial<Real>::swap(Polynomial& that)
	{
		data_.swap(that.data_);
	}

	template <typename Real>
	void Polynomial<Real>::setSize(integer size)
	{
		ENSURE_OP(size, >=, 1);

		data_.resize(size, Real(0));
	}

	template <typename Real>
	integer Polynomial<Real>::size() const
	{
		return data_.size();
	}

	template <typename Real>
	void Polynomial<Real>::trim()
	{
		setSize(degree() + 1);
	}

	template <typename Real>
	void Polynomial<Real>::set(
		integer index, const Real& that)
	{
		PENSURE_OP(index, >=, 0);

		if (index >= data_.size())
		{
			data_.resize(index + 1, Real(0));
		}

		data_[index] = that;
	}

	template <typename Real>
	Real Polynomial<Real>::operator()(
		const Real& x) const
	{
		integer n = size();
		Real result = data_[n - 1];

		for (integer i = n - 2;i >= 0;--i)
		{

			result *= x;
			result += data_[i];
		}

		return result;
	}

	template <typename Real>
	Real& Polynomial<Real>::operator[](
		integer index)
	{
		PENSURE2(index >= 0 && index < size(), index, size());
		return data_[index];
	}

	template <typename Real>
	const Real& Polynomial<Real>::operator[](
		integer index) const
	{
		PENSURE2(index >= 0 && index < size(), index, size());
		return data_[index];
	}

	template <typename Real>
	Polynomial<Real>& Polynomial<Real>::operator+=(
		const Real& that)
	{
		data_[0] += that;

		return *this;
	}

	template <typename Real>
	Polynomial<Real>& Polynomial<Real>::operator-=(
		const Real& that)
	{
		data_[0] -= that;

		return *this;
	}

	template <typename Real>
	Polynomial<Real>& Polynomial<Real>::operator*=(
		const Real& that)
	{
		integer n = size();
		for (integer i = 0;i < n;++i)
		{

			data_[i] *= that;
		}

		return *this;
	}

	template <typename Real>
	Polynomial<Real>& Polynomial<Real>::operator/=(
		const Real& that)
	{
		return (*this *= inverse(that));
	}

	template <typename Real>
	Polynomial<Real>& Polynomial<Real>::operator+=(
		const Polynomial& that)
	{
		if (that.size() > size())
		{
			data_.resize(that.size(), Real(0));
		}

		integer n = std::min(size(), that.size());
		for (integer i = 0;i < n;++i)
		{
			data_[i] += that.data_[i];
		}


		return *this;
	}

	template <typename Real>
	Polynomial<Real>& Polynomial<Real>::operator-=(
		const Polynomial& that)
	{
		if (that.size() > size())
		{
			data_.resize(that.size(), Real(0));
		}

		integer n = std::min(size(), that.size());
		for (integer i = 0;i < n;++i)
		{
			data_[i] -= that.data_[i];
		}


		return *this;
	}

	template <typename Real>
	Polynomial<Real>& Polynomial<Real>::operator*=(
		const Polynomial& that)
	{
		Polynomial result(size() + that.size());

		integer n = size();
		integer m = that.size();

		for (integer i = 0;i < n;++i)
		{
			for (integer k = 0;k < m;++k)
			{

				result[i + k] += data_[i] * that.data_[k];
			}
		}

		swap(result);

		return *this;
	}

	template <typename Real>
	Polynomial<Real>& Polynomial<Real>::operator<<=(
		integer index)
	{
		PENSURE_OP(index, >=, 0);

		integer n = degree();
		if (n + index >= size())
		{
			setSize(n + index + 1);
		}

		for (integer i = n + index;i >= index;--i)
		{
			data_[i] = data_[i - index];
		}
		for (integer i = index - 1;i >= 0;--i)
		{
			data_[i] = 0;
		}


		return *this;
	}

	template <typename Real>
	Polynomial<Real>& Polynomial<Real>::operator>>=(
		integer index)
	{
		PENSURE_OP(index, >=, 0);

		integer n = size();

		for (integer i = 0;i < n - index;++i)
		{
			data_[i] = data_[i + index];
		}
		for (integer i = n - index;i < n;++i)
		{
			data_[i] = 0;
		}


		return *this;
	}

	template <typename Real>
	bool Polynomial<Real>::operator==(
		const Polynomial& that) const
	{
		integer n = degree();
		integer m = that.degree();

		if (n != m)
		{
			return false;
		}

		for (integer i = 0;i < n;++i)
		{
			if (mabs(data_[i] - that.data_[i]) > epsilon_)
			{
				return false;
			}
		}

		return true;
	}

	template <typename Real>

	void Polynomial<Real>::setEpsilon(const Real& epsilon)
	{
		epsilon_ = epsilon;
	}

	template <typename Real>
	const Real& Polynomial<Real>::epsilon() const
	{
		return epsilon_;
	}

	template <typename Real>
	integer Polynomial<Real>::degree() const
	{
		integer n = size();
		for (integer i = n - 1;i >= 1;--i)
		{
			if (mabs(data_[i]) > epsilon_)
			{
				return i;
			}
		}

		return 0;
	}

}

#endif
