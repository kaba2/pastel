#ifndef PASTELSYS_VECTOR_TOOLS_HPP
#define PASTELSYS_VECTOR_TOOLS_HPP

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/random.h"

#include "pastel/sys/mathfunctions.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	std::ostream& operator<<(std::ostream& stream,
		const Vector<N, Real>& vector)
	{
        const integer size = vector.size();

		for (integer i = 0;i < size;++i)
		{
			stream << vector[i] << " ";
		}

		return stream;
	}

	template <int N, typename Real>
	std::istream& operator>>(std::istream& stream,
		Vector<N, Real>& vector)
	{
        const integer size = vector.size();

		for (integer i = 0;i < size;++i)
		{
			stream >> vector[i];
		}

		return stream;
	}

	template <int N, typename Real, typename Expression>
	Real sum(const VectorExpression<N, Real, Expression>& x)
	{
        const integer size = x.size();

		Real result(x[0]);

		for (integer i = 1;i < size;++i)
		{
			result += x[i];
		}

		return result;
	}

	template <int N, typename Real, typename Expression>
	Real product(const VectorExpression<N, Real, Expression>& x)
	{
        const integer size = x.size();

		Real result(x[0]);

		for (integer i = 1;i < size;++i)
		{
			result *= x[i];
		}

		return result;
	}

	template <int N, typename Real>
	Vector<N, Real> unitAxis(integer index)
	{
		Vector<N, Real> result((Real)0);
		result[index] = (Real)1;
		return result;
	}

	template <int N, typename Real, typename Expression>
	inline Vector<N - 1, Real> shrink(
		const VectorExpression<N, Real, Expression>& that)
	{
		BOOST_STATIC_ASSERT(N > 1 || N == Unbounded);

        enum
        {
            NResult = (N == Unbounded) ? Unbounded : N - 1
        };

        const integer size = that.size();

		Vector<NResult, Real> result(size - 1);
		for (int i = 0;i < size - 1;++i)
		{
			result[i] = that[i];
		}
		return result;
	}

	template <int N, typename Real, typename Expression>
	inline Vector<N + 1, Real> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<N, Real, Expression>& right)
	{
        const integer size = right.size();

        enum
        {
            NResult = (N == Unbounded) ? Unbounded : N + 1
        };

		Vector<NResult, Real> result(size + 1);

		result[0] = left;
		for (int i = 1;i < size + 1;++i)
		{
			result[i] = right[i - 1];
		}

		return result;
	}

	template <int N, typename Real, typename Expression>
	inline Vector<N + 1, Real> extend(
		const VectorExpression<N, Real, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
        const integer size = left.size();

        enum
        {
            NResult = (N == Unbounded) ? Unbounded : N + 1
        };

        Vector<N + 1, Real> result(size + 1);

		for (int i = 0;i < size;++i)
		{
			result[i] = left[i];
		}
		result[size] = right;

		return result;
	}

	template <int N, typename Real,
		typename Expression>
		inline Real dot(
		const VectorExpression<N, Real, Expression>& that)
    {
        return sum(that * that);
    }

    template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		inline Real dot(
		const VectorExpression<N, Real, LeftExpression>& left,
		const VectorExpression<N, Real, RightExpression>& right)
	{
		return sum(left * right);
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N > 1), Real>::type 
		norm(const Vector<N, Real>& that)
	{
		//return std::sqrt(dot(that, that));

		// If v[0] != 0,
		// then
		// sqrt(v[0]^2 + ... v[N - 1]^2) = 
		// |v[0]| sqrt(1 + (v[1] / v[0])^2 + ... + (v[N - 1] / v[0])^2))
		//
		// This works for any component, so
		// pick the one with the maximum absolute value.
		// This is done to avoid overflow.

		const Vector<N, Real> absThat = abs(that);

		const integer m = maxIndex(absThat);

		const Real pivot = absThat[m];

		// EPSILON
		if (pivot == 0)
		{
			return 0;
		}

		const Real invPivot = inverse(pivot);

		Real sum = 1;
		for (integer i = 0;i < m;++i)
		{
			const Real value = absThat[i] * invPivot;
			sum += value * value;
		}
		for (integer i = m + 1;i < N;++i)
		{
			const Real value = absThat[i] * invPivot;
			sum += value * value;
		}

		return pivot * std::sqrt(sum);
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N == 1), Real>::type 
		norm(const Vector<N, Real>& that)
	{
		return std::abs(that[0]);
	}

	template <int N, typename Real>
	Real normManhattan(const Vector<N, Real>& that)
	{
		return sum(abs(that));
	}

	template <int N, typename Real>
	Real powerSum(const Vector<N, Real>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric)
	{
		PENSURE(metric >= 1);
		return sum(pow(abs(that), metric));
	}

	template <int N, typename Real>
	Real norm(const Vector<N, Real>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric)
	{
		PENSURE1(metric >= 1, metric);

		//return pow(powerSum(that, metric), inverse(metric));

		// If v[0] != 0,
		// then
		// root_p(v[0]^p + ... v[N - 1]^p) = 
		// |v[0]| root_p(1 + (v[1] / v[0])^p + ... + (v[N - 1] / v[0])^p))
		//
		// This works for any component, so
		// pick the one with the maximum absolute value.
		// This is done to avoid overflow.

		const Vector<N, Real> absThat = abs(that);

		const integer m = maxIndex(absThat);

		const Real pivot = absThat[m];

		// EPSILON
		if (pivot == 0)
		{
			return 0;
		}

		const Real invPivot = inverse(pivot);

		Real sum = 1;
		for (integer i = 0;i < m;++i)
		{
			sum += std::pow(absThat[i] * invPivot, metric);
		}
		for (integer i = m + 1;i < N;++i)
		{
			sum += std::pow(absThat[i] * invPivot, metric);
		}

		return pivot * std::pow(sum, inverse(metric));
	}

	template <int N, typename Real>
	Real normInfinity(const Vector<N, Real>& that)
	{
		return max(evaluate(abs(that)));
	}

	template <int N, typename Real>
	Vector<N, Real> normalize(const Vector<N, Real>& that)
	{
		return that / norm(that);
	}

	template <typename Real>
	Vector<2, Real> cross(const Vector<2, Real>& that)
	{
		return Vector<2, Real>(-that[1], that[0]);
	}

	template <typename Real>
	Vector<3, Real> cross(
		const Vector<3, Real>& x,
		const Vector<3, Real>& y)
	{
		return Vector<3, Real>(
			x[1] * y[2] - x[2] * y[1],
			x[2] * y[0] - x[0] * y[2],
			x[0] * y[1] - x[1] * y[0]);
	}

	// Comparison functions

	template <int N, typename Real>
	inline bool allEqual(
		const Vector<N, Real>& that)
	{
		return Pastel::allEqual(asTuple(that));
	}

	template <int N, typename Real>
	inline bool allEqual(
		const Vector<N, Real>& left, 
		const Vector<N, Real>& right)
	{
		return Pastel::allEqual(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool allEqual(
		const Vector<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::allEqual(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return Pastel::allEqual(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const Vector<N, Real>& left, 
		const Vector<N, Real>& right)
	{
		return Pastel::anyEqual(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const Vector<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyEqual(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return Pastel::anyEqual(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const Vector<N, Real>& left, 
		const Vector<N, Real>& right)
	{
		return Pastel::anyLess(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Vector<N, Real>& right)
	{
		return Pastel::anyLess(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const Vector<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyLess(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Vector<N, Real>& left, 
		const Vector<N, Real>& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Vector<N, Real>& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Vector<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Vector<N, Real>& left, 
		const Vector<N, Real>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Vector<N, Real>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Vector<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Vector<N, Real>& left, 
		const Vector<N, Real>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Vector<N, Real>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Vector<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const Vector<N, Real>& left, 
		const Vector<N, Real>& right)
	{
		return Pastel::allLess(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Vector<N, Real>& right)
	{
		return Pastel::allLess(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const Vector<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::allLess(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Vector<N, Real>& left, 
		const Vector<N, Real>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Vector<N, Real>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Vector<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Vector<N, Real>& left, 
		const Vector<N, Real>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Vector<N, Real>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Vector<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Vector<N, Real>& left, 
		const Vector<N, Real>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Vector<N, Real>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Vector<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(left, right));
	}

}

#endif
