#ifndef PASTEL_LINEAR_HPP
#define PASTEL_LINEAR_HPP

#include "pastel/sys/linear.h"

namespace Pastel
{

	namespace Detail_Linear
	{

		template <typename Type, int N, typename Iterator>
		Type linear(
			const Vector<real, N>& position,
			const RandomAccessRange<Iterator>& data, 
			integer index,
			integer axis)
		{
			const integer n = position.size();

			ASSERT_OP(index, >=, 0);
			ASSERT_OP(index, <, n);
			ASSERT_OP(axis, >=, 0);
			ASSERT_OP(axis, <=, n);

			if (axis == n)
			{
				return data[index];
			}

			return Pastel::linear<Type>(
				linear<Type>(position, data, index, axis + 1),
				linear<Type>(position, data, index + (1 << axis), axis + 1),
				position[axis]);
		}

	}

	template <int N, typename Iterator>
	typename std::iterator_traits<Iterator>::value_type linear(
		const Vector<real, N>& position,
		const RandomAccessRange<Iterator>& data)
	{
		typedef typename std::iterator_traits<Iterator>::value_type
			Type;

		const integer n = position.size();
		ENSURE_OP(data.size(), >=, (1 << n));

		return Detail_Linear::linear<Type>(position, data, 0, 0);
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

}

#endif
