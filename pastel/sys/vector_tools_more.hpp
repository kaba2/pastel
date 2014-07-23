#ifndef PASTELSYS_VECTOR_TOOLS_MORE_HPP
#define PASTELSYS_VECTOR_TOOLS_MORE_HPP

#include "pastel/sys/vector_tools.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	inline Vector<Real, N> permute(
		const VectorExpression<Real, N, Expression>& that,
		const Tuple<integer, N>& permutation)
	{
		integer n = that.size();
		PENSURE_OP(n, ==, permutation.n());

		Vector<Real, N> result(
			ofDimension(n));

		for (integer i = 0;i < n;++i)
		{
			result[i] = that[permutation[i]];
		}

		return result;
	}

	template <typename Real, int N, typename Expression>
	integer minIndex(

		const VectorExpression<Real, N, Expression>& that)
	{
		integer dimension = that.n();

		integer index = 0;
		Real minValue = that[0];

		for (integer i = 1;i < dimension;++i)
		{
			const Real value = that[i];
			if (value < minValue)
			{
				minValue = value;
				index = i;
			}
		}

		return index;
	}

	template <typename Real, int N, typename Expression>
	Real min(

		const VectorExpression<Real, N, Expression>& that)
	{
		return that[minIndex(that)];
	}

	template <typename Real, int N, typename Expression>
	integer maxIndex(
		const VectorExpression<Real, N, Expression>& that)
	{
		integer dimension = that.n();

		integer index = 0;
		Real maxValue = that[0];

		for (integer i = 1;i < dimension;++i)
		{
			const Real value = that[i];
			if (maxValue < value)
			{
				maxValue = value;
				index = i;
			}
		}

		return index;
	}

	template <typename Real, int N, typename Expression>
	Real max(

		const VectorExpression<Real, N, Expression>& that)
	{
		return that[maxIndex(that)];
	}

	template <typename Real, int N,
	typename ThatExpression,
	typename MinExpression,
	typename MaxExpression>
	inline Vector<Real, N> clamp(
		const VectorExpression<Real, N, ThatExpression>& that,
		const VectorExpression<Real, N, MinExpression>& minimum,
		const VectorExpression<Real, N, MaxExpression>& maximum)
	{
		PENSURE2(that.size() == minimum.size(), that.size(), minimum.size());
		PENSURE2(that.size() == maximum.size(), that.size(), maximum.size());

		integer size = that.size();
		Vector<Real, N> result(ofDimension(size));

		for (integer i = 0;i < size;++i)
		{
			result[i] = Pastel::clamp(that[i], minimum[i], maximum[i]);
		}

		return result;
	}

}

#endif
