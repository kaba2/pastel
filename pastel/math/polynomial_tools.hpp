#ifndef PASTELMATH_POLYNOMIAL_TOOLS_HPP
#define PASTELMATH_POLYNOMIAL_TOOLS_HPP

#include "pastel/math/polynomial_tools.h"

#include "pastel/sys/random.h"

namespace Pastel
{

	template <typename Real>
	void differentiate(Polynomial<Real>& that)
	{
		const integer n = that.size();

		for (integer i = 0;i < n - 1;++i)
		{
			that[i] = that[i + 1] * (i + 1);
		}
		that[n - 1] = 0;

		that.setSize(n - 1);
	}

	template <typename Real>
	Polynomial<Real> derivative(const Polynomial<Real>& that)
	{
		Polynomial<Real> result(that);
		differentiate(result);
		return result;
	}

	template <typename Real>
	void divide(
		const Polynomial<Real>& that,
		const Polynomial<Real>& divider,
		Polynomial<Real>& quotient,
		Polynomial<Real>& remainder)
	{
		const integer n = that.degree();
		const integer m = divider.degree();

		quotient = Polynomial<Real>();
		remainder = that;

		const Real invDividerLead = inverse(divider[m]);

		for (integer i = n;i >= m;--i)
		{
			if (mabs(remainder[i]) > that.epsilon())
			{
				const Real quotientCoefficient = remainder[i] * invDividerLead;
				quotient.set(i - m, quotientCoefficient);
				remainder -= (divider << (i - m)) * quotientCoefficient;
			}
		}

		for (integer i = n;i >= m;--i)
		{
			remainder[i] = 0;
		}

		remainder.trim();
	}

	template <typename Real>
	Polynomial<Real> quotient(
		const Polynomial<Real>& that,
		const Polynomial<Real>& divider)
	{
		Polynomial<Real> result;
		Polynomial<Real> remainder;
		divide(that, divider, result, remainder);
		return result;
	}

	template <typename Real>
	Polynomial<Real> remainder(
		const Polynomial<Real>& that,
		const Polynomial<Real>& divider)
	{
		Polynomial<Real> result;
		Polynomial<Real> remainder;
		divide(that, divider, result, remainder);
		return remainder;
	}

	template <typename Real>
	void sturmSequence(
		const Polynomial<Real>& that,
		std::vector<Polynomial<Real> > sturm)
	{
		std::vector<Polynomial<Real> > result;
		result.push_back(that);
		result.push_back(derivative(that));

		while(!result.back().zero())
		{
			result.push_back(
				-remainder(result[result.size() - 2],
				result.back()));
		}
	}

	template <typename Real>
	integer roots(
		const std::vector<Polynomial<Real> >& sturm,
		const Real& max)
	{
		if (sturm.empty())
		{
			return 0;
		}

		integer signChanges = 0;
		const integer n = sturm.size();
		bool previousSign = sturm[0](max) > 0;

		for (integer i = 1;i < n;++i)
		{
			const bool sign = sturm[i](max) > 0;
			if (sign != previousSign)
			{
				++signChanges;
			}
			previousSign = sign;
		}

		return signChanges;
	}

	template <typename Real>
	integer roots(
		const std::vector<Polynomial<Real> >& sturm,
		const Real& min, const Real& max)
	{
		return Pastel::roots(sturm, max) - Pastel::roots(sturm, min);
	}

	template <typename Real>
	Polynomial<Real> randomPolynomial(
		integer degree)
	{
		ENSURE_OP(degree, >=, 0);

		Polynomial<Real> result(degree + 1);
		for (integer i = 0;i < degree + 1;++i)
		{
			result[i] = random<Real>();
		}

		return result;
	}

	template <typename Real>
	std::ostream& operator<<(
		std::ostream& stream,
		const Polynomial<Real>& that)
	{
		const integer n = that.degree();

		stream << "[";
		stream << that[n];
		for (integer i = n - 1;i >= 0;--i)
		{
			stream << ", " << that[i];
		}
		stream << "]";

		return stream;
	}

}

#endif
