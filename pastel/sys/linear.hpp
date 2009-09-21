#ifndef PASTEL_LINEAR_HPP
#define PASTEL_LINEAR_HPP

#include "pastel/sys/linear.h"

namespace Pastel
{

	namespace Detail_Linear
	{

		template <typename Type, int N>
		Type linear(
			const Vector<real, N>& position,
			const Tuple<Type, ModifyN<N, 1 << N>::Result>& data, 
			integer index,
			integer axis)
		{
			const integer n = position.size();

			ASSERT_OP(index, >=, 0);
			ASSERT_OP(index, <, n);
			ASSERT_OP(axis, >=, 0);
			ASSERT_OP(axis, <, n);

			if (axis == n - 1)
			{
				return data[index];
			}

			return Pastel::linear(
				linear(position, data, index * 2, axis + 1),
				linear(position, data, index * 2 + (1 << axis), axis + 1),
				position[axis]);
		}

	}

	template <typename Type, int N>
	Type linear(
		const Vector<real, N>& position,
		const Tuple<Type, ModifyN<N, 1 << N>::Result>& data)
	{
		return Detail_Linear::linear(position, data, 0, 0);
	}

	template <
		typename PointType,
		typename Real>
	PointType linear(
			const PointType& startPoint,
			const PointType& endPoint,
			const Real& time)
	{
		return startPoint +
			(endPoint - startPoint) * time;
	}

	template <int N, typename Real>
	Vector<Real, N> linear(
		const Vector<Real, N>& startPoint,
		const Vector<Real, N>& endPoint,
		const PASTEL_NO_DEDUCTION(Real)& time)
	{
		// TODO: Change to a vector expression!
		return startPoint * (1 - time) + endPoint * time;
	}

}

#endif
