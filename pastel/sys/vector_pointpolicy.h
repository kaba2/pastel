// Description: Vector_PointPolicy class
// Detail: Point from a Vector
// Documentation: pointpolicies.txt

#ifndef PASTELSYS_VECTOR_POINTPOLICY_H
#define PASTELSYS_VECTOR_POINTPOLICY_H

#include "pastel/sys/pointpolicy_concept.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real_, int N_ = Dynamic>
	class Vector_PointPolicy
	{
	public:
		static PASTEL_CONSTEXPR int N = N_;

		Vector_PointPolicy()
			: dimension_(N)
		{
			ENSURE_OP(N, >=, 0);
		}

		explicit Vector_PointPolicy(integer dimension)
			: dimension_(dimension)
		{
			ENSURE(N == dimension || N == Dynamic);
		}

		using Real = Real_;
		using Point = Vector<Real, N>;
		using ConstIterator = const Real*;
		using Expression = ConstArray_VectorExpression<Real, N>;

		ConstIterator begin(const Point& point) const
		{
			return point.rawBegin();
		}

		ConstIterator end(const Point& point) const
		{
			return point.rawEnd();
		}

		Expression operator()(const Point& point) const
		{
			return constVectorExpression<N>(
				point.rawBegin(), n());
		}

		const Real& pointAxis(const Point& point, integer axis) const
		{
			return point[axis];
		}

		integer n() const
		{
			// The check here is to enable compile-time
			// optimization when N is not Dynamic.
			return (N >= 0) ? N : dimension_;
		}

		integer dimension(const Point& point) const
		{
			// The check here is to enable compile-time
			// optimization when N is not Dynamic.
			return (N >= 0) ? N : point.size();
		}
	
	private:
		integer dimension_;
	};

	using Vector_PointPolicy1 = Vector_PointPolicy<real, 1>;
	using Vector_PointPolicy2 = Vector_PointPolicy<real, 2>;
	using Vector_PointPolicy3 = Vector_PointPolicy<real, 3>;
	using Vector_PointPolicy4 = Vector_PointPolicy<real, 4>;
	using Vector_PointPolicyD = Vector_PointPolicy<real, Dynamic>;

}

#endif
