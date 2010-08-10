// Description: Array_PointPolicy class
// Detail: Points represented by coordinate pointers.
// Documentation: pointpolicy.txt

#ifndef PASTEL_ARRAY_POINTPOLICY_H
#define PASTEL_ARRAY_POINTPOLICY_H

#include "pastel/geometry/pointpolicy_concept.h"

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
			PENSURE(N >= 0);
		}

		explicit Array_PointPolicy(integer dimension)
			: dimension_(dimension)
		{
			PENSURE(N == Dynamic || N == dimension);
		}

		typedef Real Coordinate;
		typedef const Coordinate* Object;

		// We do not use a const-reference here, since a
		// pointer is transmitted faster by value.
		const Coordinate* point(Object object) const
		{
			return object;
		}

		const Coordinate& point(Object object, integer axis) const
		{
			return object[axis];
		}

		integer dimension(Object object) const
		{
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
