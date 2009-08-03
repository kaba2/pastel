#ifndef PASTEL_VECTOR_TOOLS_MORE_HPP
#define PASTEL_VECTOR_TOOLS_MORE_HPP

#include "pastel/sys/vector_tools.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<Real, N> permute(
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

		return result.asTemporary();
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
	TemporaryVector<Real, N> min(
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

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	TemporaryVector<Real, N> min(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<Real, N, Expression>& right)
	{
		const integer size = right.size();

		Vector<Real, N> result(ofDimension(size));
		for (integer i = 0;i < size;++i)
		{
			result[i] = std::min(left, right[i]);
		}

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	TemporaryVector<Real, N> min(
		const VectorExpression<Real, N, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		const integer size = left.size();

		Vector<Real, N> result(ofDimension(size));
		for (integer i = 0;i < size;++i)
		{
			result[i] = std::min(left[i], right);
		}

		return result.asTemporary();
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
	TemporaryVector<Real, N> max(
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

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	TemporaryVector<Real, N> max(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<Real, N, Expression>& right)
	{
		const integer size = right.size();
		Vector<Real, N> result(ofDimension(size));

		for (integer i = 0;i < size;++i)
		{
			result[i] = std::max(left, right[i]);
		}

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	TemporaryVector<Real, N> max(
		const VectorExpression<Real, N, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		const integer size = left.size();
		Vector<Real, N> result(ofDimension(size));

		for (integer i = 0;i < size;++i)
		{
			result[i] = std::max(left[i], right);
		}

		return result.asTemporary();
	}

	template <int N, typename Real,
	typename ThatExpression,
	typename MinExpression,
	typename MaxExpression>
	inline TemporaryVector<Real, N> clamp(
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

		return result.asTemporary();
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
	const VectorInverse<N, Real, Expression>
		inverse(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorInverse<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorAbs<N, Real, Expression>
		mabs(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorAbs<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorSquare<N, Real, Expression>
		squarev(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorSquare<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorExp<N, Real, Expression>
		exp(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorExp<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorLog<N, Real, Expression>
		log(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorLog<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		const VectorPow<N, Real, LeftExpression, RightExpression>
		pow(const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right)
	{
		return VectorPow<N, Real, LeftExpression, RightExpression>(
			(const LeftExpression&)left,
			(const RightExpression&)right);
	}

	template <int N, typename Real,
		typename LeftExpression>
		const VectorPow<N, Real,
		LeftExpression,
		VectorConstant<N, Real> >
		pow(const VectorExpression<Real, N, LeftExpression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return VectorPow<N, Real, LeftExpression,
			VectorConstant<N, Real> >(
			(const LeftExpression&)left,
			VectorConstant<N, Real>(right, left.size()));
	}

	template <int N, typename Real, typename Expression>
	const VectorSqrt<N, Real, Expression>
		sqrt(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorSqrt<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorFloor<N, Real, Expression>
		floor(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorFloor<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorCeil<N, Real, Expression>
		ceil(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorCeil<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorSin<N, Real, Expression>
		sin(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorSin<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorCos<N, Real, Expression>
		cos(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorCos<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorTan<N, Real, Expression>
		tan(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorTan<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorAsin<N, Real, Expression>
		asin(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorAsin<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorAcos<N, Real, Expression>
		acos(const VectorExpression<Real, N, Expression>& x)
	{
		return VectorAcos<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		const VectorAtan2<N, Real, LeftExpression, RightExpression>
		atan2(const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right)
	{
		return VectorAtan2<N, Real,
			LeftExpression, RightExpression>(
			(const LeftExpression&)left,
			(const RightExpression&)right);
	}

}

#endif
