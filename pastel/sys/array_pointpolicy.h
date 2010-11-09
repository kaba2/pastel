// Description: Array_PointPolicy class
// Detail: Native array as a model of the PointPolicy concept.
// Documentation: pointpolicy.txt

#ifndef PASTEL_ARRAY_POINTPOLICY_H
#define PASTEL_ARRAY_POINTPOLICY_H

#include "pastel/sys/pointpolicy_concept.h"
#include "pastel/sys/array_vectorexpression.h"

namespace Pastel
{

	template <typename Real, int N_ = Dynamic>
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
			PENSURE(N >= 0);
		}

		explicit Array_PointPolicy(integer dimension)
			: dimension_(dimension)
		{
			PENSURE(N == Dynamic || N == dimension);
		}

		typedef Real Coordinate;
		typedef const Coordinate* Object;
		typedef const Coordinate* ConstIterator;
		typedef ConstArray_VectorExpression<Real, N> Expression;

		ConstIterator begin(Object object) const
		{
			return object;
		}

		ConstIterator end(Object object) const
		{
			return object + dimension();
		}

		Expression operator()(Object object) const
		{
			return constVectorExpression<N>(object, dimension());
		}

		integer dimension() const
		{
			// The check here is to enable compile-time
			// optimization when N is not Dynamic.
			return (N >= 0) ? N : dimension_;
		}

		integer dimension(Object object) const
		{
			// The check here is to enable compile-time
			// optimization when N is not Dynamic.
			return (N >= 0) ? N : dimension_;
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
