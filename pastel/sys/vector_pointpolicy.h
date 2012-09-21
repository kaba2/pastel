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
		enum
		{
			N = N_
		};

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

		typedef Real_ Real;
		typedef Vector<Real, N> Point;
		typedef const Real* ConstIterator;
		typedef ConstArray_VectorExpression<Real, N> Expression;

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

		const Real& axis(const Point& point, integer axis) const
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

	typedef Vector_PointPolicy<real, 1> Vector_PointPolicy1;
	typedef Vector_PointPolicy<real, 2> Vector_PointPolicy2;
	typedef Vector_PointPolicy<real, 3> Vector_PointPolicy3;
	typedef Vector_PointPolicy<real, 4> Vector_PointPolicy4;
	typedef Vector_PointPolicy<real, Dynamic> Vector_PointPolicyD;

}

#endif
