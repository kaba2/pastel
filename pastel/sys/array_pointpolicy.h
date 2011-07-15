// Description: Array_PointPolicy class
// Detail: Point from a pointer to coordinates
// Documentation: pointpolicies.txt

#ifndef PASTEL_ARRAY_POINTPOLICY_H
#define PASTEL_ARRAY_POINTPOLICY_H

#include "pastel/sys/pointpolicy_concept.h"
#include "pastel/sys/array_vectorexpression.h"

namespace Pastel
{

	template <typename Real_, int N_ = Dynamic>
	class Array_PointPolicy
	{
	public:
		enum
		{
			N = N_
		};

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

		typedef Real_ Real;
		typedef const Real* Point;
		typedef const Real* ConstIterator;
		typedef ConstArray_VectorExpression<Real, N> Expression;

		ConstIterator begin(Point point) const
		{
			return point;
		}

		ConstIterator end(Point point) const
		{
			return point + dimension();
		}

		Expression operator()(Point point) const
		{
			return constVectorExpression<N>(point, dimension());
		}

		const Real& axis(Point point, integer index) const
		{
			return point[index];
		}

		integer dimension() const
		{
			// The check here is to enable compile-time
			// optimization when N is not Dynamic.
			return (N >= 0) ? N : dimension_;
		}

		integer dimension(Point point) const
		{
			return dimension();
		}

	private:
		integer dimension_;
	};

	typedef Array_PointPolicy<real, 1> Array_PointPolicy1;
	typedef Array_PointPolicy<real, 2> Array_PointPolicy2;
	typedef Array_PointPolicy<real, 3> Array_PointPolicy3;
	typedef Array_PointPolicy<real, 4> Array_PointPolicy4;
	typedef Array_PointPolicy<real, Dynamic> Array_PointPolicyD;

}

#endif
