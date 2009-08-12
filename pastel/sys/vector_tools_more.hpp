#ifndef PASTEL_VECTOR_TOOLS_MORE_HPP
#define PASTEL_VECTOR_TOOLS_MORE_HPP

#include "pastel/sys/vector_tools.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	template <int N, typename Real, typename Expression>
	inline Vector<Real, N> permute(
		const VectorExpression<Real, N, Expression>& that,
		const Tuple<integer, N>& permutation)
	{
		const integer dimension = that.size();
		PENSURE_OP(dimension, ==, permutation.dimension());

		Vector<Real, N> result(
			ofDimension(dimensions));

		for (integer i = 0;i < dimension;++i)
		{
			result[i] = that[permutation[i]];
		}

		return result;
	}

	// Minimum functions

	template <int N, typename Real, typename Expression>
	integer minIndex(
		const VectorExpression<Real, N, Expression>& that)
	{
		const integer dimension = that.dimension();

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

	template <int N, typename Real, typename Expression>
	Real min(
		const VectorExpression<Real, N, Expression>& that)
	{
		return that[minIndex(that)];
	}

	template <int N, typename Real,
	typename LeftExpression, typename RightExpression>
	Vector<Real, N> min(
		const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right)
	{
		PENSURE_OP(left.size(), ==, right.size());

		const integer size = left.size();
	
		Vector<Real, N> result(ofDimension(size));
		for (integer i = 0;i < size;++i)
		{
			result[i] = std::min(left[i], right[i]);
		}

		return result;
	}

	template <int N, typename Real, typename Expression>
	Vector<Real, N> min(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<Real, N, Expression>& right)
	{
		const integer size = right.size();

		Vector<Real, N> result(ofDimension(size));
		for (integer i = 0;i < size;++i)
		{
			result[i] = std::min(left, right[i]);
		}

		return result;
	}

	template <int N, typename Real, typename Expression>
	Vector<Real, N> min(
		const VectorExpression<Real, N, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		const integer size = left.size();

		Vector<Real, N> result(ofDimension(size));
		for (integer i = 0;i < size;++i)
		{
			result[i] = std::min(left[i], right);
		}

		return result;
	}



	// Maximum functions

	template <int N, typename Real, typename Expression>
	integer maxIndex(
		const VectorExpression<Real, N, Expression>& that)
	{
		const integer dimension = that.dimension();

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

	template <int N, typename Real, typename Expression>
	Real max(
		const VectorExpression<Real, N, Expression>& that)
	{
		return that[maxIndex(that)];
	}

	template <int N, typename Real,
	typename LeftExpression,
	typename RightExpression>
	Vector<Real, N> max(
		const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right)
	{
		PENSURE2(left.size() == right.size(), left.size(), right.size());

		const integer size = left.size();
		Vector<Real, N> result(ofDimension(size));
		
		for (integer i = 0;i < size;++i)
		{
			result[i] = std::max(left[i], right[i]);
		}

		return result;
	}

	template <int N, typename Real, typename Expression>
	Vector<Real, N> max(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<Real, N, Expression>& right)
	{
		const integer size = right.size();
		Vector<Real, N> result(ofDimension(size));

		for (integer i = 0;i < size;++i)
		{
			result[i] = std::max(left, right[i]);
		}

		return result;
	}

	template <int N, typename Real, typename Expression>
	Vector<Real, N> max(
		const VectorExpression<Real, N, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		const integer size = left.size();
		Vector<Real, N> result(ofDimension(size));

		for (integer i = 0;i < size;++i)
		{
			result[i] = std::max(left[i], right);
		}

		return result;
	}

	template <int N, typename Real,
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

		const integer size = that.size();
		Vector<Real, N> result(ofDimension(size));

		for (integer i = 0;i < size;++i)
		{
			result[i] = Pastel::clamp(that[i], minimum[i], maximum[i]);
		}

		return result;
	}

	// Optimization functions

	template <int N, typename Real>
	void multiplyByPowerOf2(
		Vector<Real, N>& x,
		const Vector<Real, N>& power)
	{
		PENSURE2(x.size() == power.size(), x.size(), power.size());

		const integer size = x.size();

		for (integer i = 0;i < size;++i)
		{
			Pastel::multiplyByPowerOf2(x[i], power[i]);
		}
	}

	template <int N, typename Real>
	void multiplyByPowerOf2(
		Vector<Real, N>& x,
		const Real& power)
	{
		PENSURE2(x.size() == power.size(), x.size(), power.size());

		const integer size = x.size();

		for (integer i = 0;i < size;++i)
		{
			Pastel::multiplyByPowerOf2(x[i], power);
		}
	}

	template <int N, typename Real>
	void divideByPowerOf2(
		Vector<Real, N>& x,
		const Vector<Real, N>& power)
	{
		PENSURE2(x.size() == power.size(), x.size(), power.size());

		const integer size = x.size();

		for (integer i = 0;i < size;++i)
		{
			Pastel::divideByPowerOf2(x[i], power[i]);
		}
	}

	template <int N, typename Real>
	void divideByPowerOf2(
		Vector<Real, N>& x,
		const Real& power)
	{
		PENSURE2(x.size() == power.size(), x.size(), power.size());

		const integer size = x.size();

		for (integer i = 0;i < size;++i)
		{
			Pastel::divideByPowerOf2(x[i], power);
		}
	}

	// Arithmetic functions

	template <int N, typename Real, typename Expression>
	const VectorInverse<Real, N, Expression>
		inverse(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorInverse<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorAbs<Real, N, Expression>
		mabs(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorAbs<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorSquare<Real, N, Expression>
		squarev(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorSquare<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorExp<Real, N, Expression>
		exp(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorExp<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorLog<Real, N, Expression>
		log(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorLog<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		const VectorPow<Real, N, LeftExpression, RightExpression>
		pow(const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right)
	{
		return VectorPow<Real, N, LeftExpression, RightExpression>(
			(const LeftExpression&)left,
			(const RightExpression&)right);
	}

	template <int N, typename Real,
		typename LeftExpression>
		const VectorPow<Real, N, 
		LeftExpression, 
		VectorConstant<Real, N> >
		pow(const VectorExpression<Real, N, LeftExpression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return VectorPow<Real, N, LeftExpression, 
			VectorConstant<Real, N> >(
			(const LeftExpression&)left,
			VectorConstant<Real, N>(right, left.size()));
	}

	template <int N, typename Real, typename Expression>
	const VectorSqrt<Real, N, Expression>
		sqrt(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorSqrt<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorFloor<Real, N, Expression>
		floor(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorFloor<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorCeil<Real, N, Expression>
		ceil(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorCeil<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorSin<Real, N, Expression>
		sin(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorSin<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorCos<Real, N, Expression>
		cos(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorCos<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorTan<Real, N, Expression>
		tan(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorTan<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorAsin<Real, N, Expression>
		asin(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorAsin<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorAcos<Real, N, Expression>
		acos(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorAcos<Real, N, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		const VectorAtan2<Real, N, LeftExpression, RightExpression>
		atan2(const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right)
	{
		return VectorAtan2<Real, N, 
			LeftExpression, RightExpression>(
			(const LeftExpression&)left,
			(const RightExpression&)right);
	}

}

#endif
