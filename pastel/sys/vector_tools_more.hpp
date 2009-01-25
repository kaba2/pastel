#ifndef PASTELSYS_VECTOR_TOOLS_MORE_HPP
#define PASTELSYS_VECTOR_TOOLS_MORE_HPP

#include "pastel/sys/vector_tools.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	template <int N, typename Real>
	inline Vector<N, Real> permute(
		const Vector<N, Real>& that,
		const Tuple<N, integer>& permutation)
	{
		Vector<N, Real> result;
		
		for (integer i = 0;i < N;++i)
		{
			result[i] = that[permutation[i]];
		}
		
		return result;
	}

	// Minimum functions

	template <int N, typename Real>
	integer minIndex(
		const Vector<N, Real>& that)
	{
		integer index = 0;
		const Real* currentMin = &that[0];

		for (integer i = 1;i < N;++i)
		{
			const Real* current = &that[i];
			if (*current < *currentMin)
			{
				currentMin = current;
				index = i;
			}
		}

		return index;
	}

    template <int N, typename Real>
	Real min(
		const Vector<N, Real>& that)
	{
		return that[minIndex(that)];
	}

	template <int N, typename Real>
	Vector<N, Real> min(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		Vector<N, Real> result;
		for (integer i = 0;i <  N;++i)
		{
			result[i] = std::min(left[i], right[i]);
		}
		return result;
	}

	template <int N, typename Real>
	Vector<N, Real> min(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		Vector<N, Real> result;
		for (integer i = 0;i <  N;++i)
		{
			result[i] = std::min(left, right[i]);
		}

		return result;
	}

	template <int N, typename Real>
	Vector<N, Real> min(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		Vector<N, Real> result;
		for (integer i = 0;i <  N;++i)
		{
			result[i] = std::min(left[i], right);
		}
		return result;
	}



	// Maximum functions

	template <int N, typename Real>
	integer maxIndex(
		const Vector<N, Real>& that)
	{
		integer index = 0;
		const Real* currentMax = &that[0];

		for (integer i = 1;i < N;++i)
		{
			const Real* current = &that[i];
			if (*currentMax < *current)
			{
				currentMax = current;
				index = i;
			}
		}

		return index;
	}

    template <int N, typename Real>
	Real max(
		const Vector<N, Real>& that)
	{
		return that[maxIndex(that)];
	}

	template <int N, typename Real>
	Vector<N, Real> max(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		Vector<N, Real> result;
		for (integer i = 0;i <  N;++i)
		{
			result[i] = std::max(left[i], right[i]);
		}
		return result;
	}

	template <int N, typename Real>
	Vector<N, Real> max(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		Vector<N, Real> result;
		for (integer i = 0;i <  N;++i)
		{
			result[i] = std::max(left, right[i]);
		}
		return result;
	}

	template <int N, typename Real>
	Vector<N, Real> max(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		Vector<N, Real> result;
		for (integer i = 0;i <  N;++i)
		{
			result[i] = std::max(left[i], right);
		}
		return result;
	}

	template <int N, typename Real>
	inline Vector<N, Real> clamp(
		const Vector<N, Real>& that,
		const Vector<N, Real>& minimum,
		const Vector<N, Real>& maximum)
	{
		Vector<N, Real> result;

		for (integer i = 0;i < N;++i)
		{
			result[i] = Pastel::clamp(that[i], minimum[i], maximum[i]);
		}

		return result;
	}

	// Optimization functions

	template <int N, typename Real>
	void multiplyByPowerOf2(Vector<N, Real>& x,
		const Vector<N, Real>& power)
	{
		for (integer i = 0;i < N;++i)
		{
			Pastel::multiplyByPowerOf2(x[i], power[i]);
		}
	}

	template <int N, typename Real>
	void multiplyByPowerOf2(Vector<N, Real>& x,
		const Real& power)
	{
		for (integer i = 0;i < N;++i)
		{
			Pastel::multiplyByPowerOf2(x[i], power);
		}
	}

	template <int N, typename Real>
	void divideByPowerOf2(Vector<N, Real>& x,
		const Vector<N, Real>& power)
	{
		for (integer i = 0;i < N;++i)
		{
			Pastel::divideByPowerOf2(x[i], power[i]);
		}
	}

	template <int N, typename Real>
	void divideByPowerOf2(Vector<N, Real>& x,
		const Real& power)
	{
		for (integer i = 0;i < N;++i)
		{
			Pastel::divideByPowerOf2(x[i], power);
		}
	}

	// Arithmetic functions

	template <int N, typename Real, typename Expression>
	const VectorInverse<N, Real, Expression>
		inverse(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorInverse<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorAbs<N, Real, Expression>
		abs(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorAbs<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorExp<N, Real, Expression>
		exp(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorExp<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorLog<N, Real, Expression>
		log(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorLog<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		const VectorPow<N, Real, LeftExpression, RightExpression>
		pow(const VectorExpression<N, Real, LeftExpression>& left,
		const VectorExpression<N, Real, RightExpression>& right)
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
		pow(const VectorExpression<N, Real, LeftExpression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return VectorPow<N, Real, LeftExpression,
			VectorConstant<N, Real> >(
			(const LeftExpression&)left,
			VectorConstant<N, Real>(right, left.size()));
	}

	template <int N, typename Real, typename Expression>
	const VectorSqrt<N, Real, Expression>
		sqrt(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorSqrt<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorFloor<N, Real, Expression>
		floor(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorFloor<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorCeil<N, Real, Expression>
		ceil(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorCeil<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorSin<N, Real, Expression>
		sin(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorSin<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorCos<N, Real, Expression>
		cos(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorCos<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorTan<N, Real, Expression>
		tan(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorTan<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorAsin<N, Real, Expression>
		asin(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorAsin<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real, typename Expression>
	const VectorAcos<N, Real, Expression>
		acos(const VectorExpression<N, Real, Expression>& x)
	{
		return VectorAcos<N, Real, Expression>(
			(const Expression&)x);
	}

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		const VectorAtan2<N, Real, LeftExpression, RightExpression>
		atan2(const VectorExpression<N, Real, LeftExpression>& left,
		const VectorExpression<N, Real, RightExpression>& right)
	{
		return VectorAtan2<N, Real,
			LeftExpression, RightExpression>(
			(const LeftExpression&)left,
			(const RightExpression&)right);
	}

}

#endif
