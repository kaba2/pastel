#ifndef PASTEL_VECTOR_TOOLS_HPP
#define PASTEL_VECTOR_TOOLS_HPP

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
	TemporaryVector<N, Real> unitAxis(integer index)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		BOOST_STATIC_ASSERT(N > 0);

		PENSURE2(index >= 0 && index < N, index, N);

		Vector<N, Real> result(0);
		result[index] = 1;

		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> unitAxis(
		integer dimension, integer index)
	{
		PENSURE1(dimension > 0, dimension);
		PENSURE(dimension == Dynamic || dimension == N);
		PENSURE2(index >= 0 && index < dimension, index, dimension);

		Vector<N, Real> result(ofDimension(dimension), 0);
		result[index] = 1;

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<PASTEL_ADD_N(N, -1), Real> shrink(
		const VectorExpression<N, Real, Expression>& that)
	{
		BOOST_STATIC_ASSERT(N > 1 || N == Dynamic);

		const integer size = that.size();

		Vector<PASTEL_ADD_N(N, -1), Real> result(size - 1);
		for (int i = 0;i < size - 1;++i)
		{
			result[i] = that[i];
		}

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<PASTEL_ADD_N(N, -1), Real> shrink(
		const VectorExpression<N, Real, Expression>& that,
		integer index)
	{
		BOOST_STATIC_ASSERT(N > 1 || N == Dynamic);

		const integer size = that.size();

		Vector<PASTEL_ADD_N(N, -1), Real> result(size - 1);
		for (integer i = 0;i < index;++i)
		{
			result[i] = that[i];
		}
		for  (integer i = index;i < size - 1;++i)
		{
			result[i] = that[i + 1];
		}

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<PASTEL_ADD_N(N, 1), Real> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<N, Real, Expression>& right)
	{
		const integer size = right.size();

		Vector<PASTEL_ADD_N(N, 1), Real> result(size + 1);

		result[0] = left;
		for (int i = 1;i < size + 1;++i)
		{
			result[i] = right[i - 1];
		}

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<PASTEL_ADD_N(N, 1), Real> extend(
		const VectorExpression<N, Real, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		const integer size = left.size();

		Vector<PASTEL_ADD_N(N, 1), Real> result(size + 1);

		for (int i = 0;i < size;++i)
		{
			result[i] = left[i];
		}
		result[size] = right;

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<PASTEL_ADD_N(N, 1), Real> extend(
		const VectorExpression<N, Real, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right,
		integer index)
	{
		const integer size = left.size();

		PENSURE2(index >= 0 && index < size, index, size);

		Vector<PASTEL_ADD_N(N, 1), Real> result(size + 1);
		for (integer i = 0;i < index;++i)
		{
			result[i] = left[i];
		}
		result[index] = right;
		for (integer i = index + 1;i < size;++i)
		{
			result[i] = left[i - 1];
		}

		return result.asTemporary();
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

	template <int N, typename Real, typename Expression>
	typename boost::enable_if_c<(N > 1 || N == Dynamic), Real>::type
		norm(const VectorExpression<N, Real, Expression>& that)
	{
		return std::sqrt(dot(that, that));
	}

	template <int N, typename Real, typename Expression>
	typename boost::enable_if_c<(N == 1), Real>::type
		norm(const VectorExpression<N, Real, Expression>& that)
	{
		return mabs(that[0]);
	}

	template <int N, typename Real, typename Expression>
	Real normManhattan(const VectorExpression<N, Real, Expression>& that)
	{
		return sum(mabs(that));
	}

	template <int N, typename Real, typename Expression>
	Real powerSum(
		const VectorExpression<N, Real, Expression>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric)
	{
		PENSURE(metric >= 1);
		return sum(pow(mabs(that), metric));
	}

	template <int N, typename Real, typename Expression>
	Real pNorm(
		const VectorExpression<N, Real, Expression>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric)
	{
		PENSURE1(metric >= 1, metric);

		return pow(powerSum(that, metric), inverse(metric));
	}

	template <int N, typename Real, typename Expression>
	Real normInfinity(const VectorExpression<N, Real, Expression>& that)
	{
		return max(mabs(that));
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> normalize(const TemporaryVector<N, Real>& that)
	{
		Vector<N, Real> result = that;
		result /= norm(result);
		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> normalize(const Vector<N, Real>& that)
	{
		return that / norm(that);
	}

	template <typename Real, typename Expression>
	TemporaryVector<2, Real> cross(
		const VectorExpression<2, Real, Expression>& that)
	{
		Vector<2, Real> result(-that[1], that[0]);
		return result.asTemporary();
	}

	template <typename Real, typename ExpressionX,
	typename ExpressionY>
	TemporaryVector<3, Real> cross(
		const VectorExpression<3, Real, ExpressionX>& x,
		const VectorExpression<3, Real, ExpressionY>& y)
	{
		Vector<3, Real> result(
			x[1] * y[2] - x[2] * y[1],
			x[2] * y[0] - x[0] * y[2],
			x[0] * y[1] - x[1] * y[0]);

		return result.asTemporary();
	}

}

#endif
