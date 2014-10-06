#ifndef PASTELSYS_LINEAR_HPP
#define PASTELSYS_LINEAR_HPP

#include "pastel/sys/linear.h"

namespace Pastel
{

	namespace Linear_
	{

		template <typename Type, int N, typename Vector_Range>
		Type linear(
			const Vector<real, N>& position,
			const Vector_Range& data, 
			integer index,
			integer axis)
		{
			integer n = position.size();

			ASSERT_OP(index, >=, 0);
			ASSERT_OP(index, <, ((integer)1 << n));
			ASSERT_OP(axis, >=, 0);
			ASSERT_OP(axis, <=, n);

			if (axis == n)
			{
				return data[index];
			}

			return Pastel::linear<Type>(
				linear<Type>(position, data, index, axis + 1),
				linear<Type>(position, data, index + ((integer)1 << axis), axis + 1),
				position[axis]);
		}

	}

	template <int N, typename Vector_Range>
	typename boost::range_value<Vector_Range>::type linear(
		const Vector<real, N>& position,
		const Vector_Range& data)
	{
		typedef typename boost::range_value<Vector_Range>::type
			Type;

		integer n = position.size();
		ENSURE_OP((integer)data.size(), >=, ((integer)1 << (integer)n));

		return Linear_::linear<Type>(position, data, 0, 0);
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

	template <typename Real, int N>
	Vector<Real, N> linear(
		const Vector<Real, N>& startPoint,
		const Vector<Real, N>& endPoint,
		const PASTEL_NO_DEDUCTION(Real)& time)
	{
		// TODO: Change to a vector expression!
		return startPoint * (1 - time) + endPoint * time;
	}

	template <typename Real, int N>
	Vector<Real, N> midpoint(
		const Vector<Real, N>& startPoint,
		const Vector<Real, N>& endPoint)
	{
		return linear(startPoint, endPoint, (Real)0.5);
	}

}

#endif
