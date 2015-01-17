// Description: Array_PointPolicy class
// Detail: Point from a pointer to coordinates
// Documentation: pointpolicies.txt

#ifndef PASTELSYS_ARRAY_POINTPOLICY_H
#define PASTELSYS_ARRAY_POINTPOLICY_H

#include "pastel/sys/pointpolicy_concept.h"

namespace Pastel
{

	template <typename Real_, int N_ = Dynamic>
	class Array_PointPolicy
	{
	public:
		static PASTEL_CONSTEXPR int N = N_;

		PASTEL_STATIC_ASSERT(N == Dynamic || N >= 0);

		Array_PointPolicy()
			: dimension_(N)
		{
			// We can't allow N == Dynamic here, since
			// then there would be no way to get the
			// dimensionality of the points.
			ENSURE(N >= 0);
		}

		explicit Array_PointPolicy(integer dimension)
			: dimension_(dimension)
		{
			ENSURE(N == Dynamic || N == dimension);
		}

		using Real = Real_;
		using Point = const Real*;
		using ConstIterator = const Real*;
		using Expression = ConstArray_VectorExpression<Real, N>;

		ConstIterator begin(Point point) const
		{
			return point;
		}

		ConstIterator end(Point point) const
		{
			return point + n();
		}

		Expression operator()(Point point) const
		{
			return constVectorExpression<N>(point, n());
		}

		const Real& axis(Point point, integer index) const
		{
			return point[index];
		}

		integer n() const
		{
			// The check here is to enable compile-time
			// optimization when N is not Dynamic.
			return (N >= 0) ? N : dimension_;
		}

		integer dimension(Point point) const
		{
			return n();
		}

	private:
		integer dimension_;
	};

	using Array_PointPolicy1 = Array_PointPolicy<real, 1>;
	using Array_PointPolicy2 = Array_PointPolicy<real, 2>;
	using Array_PointPolicy3 = Array_PointPolicy<real, 3>;
	using Array_PointPolicy4 = Array_PointPolicy<real, 4>;
	using Array_PointPolicyD = Array_PointPolicy<real, Dynamic>;

}

#endif
