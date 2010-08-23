// Description: Vector_PointPolicy class
// Detail: Vector as a model of the PointPolicy concept.
// Documentation: pointpolicy.txt

#ifndef PASTEL_VECTOR_POINTPOLICY_H
#define PASTEL_VECTOR_POINTPOLICY_H

#include "pastel/sys/pointpolicy_concept.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, int N_ = Dynamic>
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
		}

		explicit Vector_PointPolicy(integer dimension)
			: dimension_(dimension)
		{
			PENSURE(N == dimension || N == Dynamic);
		}

		typedef Real Coordinate;
		typedef Vector<Real, N> Object;

		const Coordinate* point(const Object& object) const
		{
			return object.rawBegin();
		}

		const Coordinate& point(const Object& object, integer axis) const
		{
			return object[axis];
		}

		integer dimension() const
		{
			// The check here is to enable compile-time
			// optimization when N is not Dynamic.
			return (N >= 0) ? N : dimension_;
		}

		integer dimension(const Object& object) const
		{
			// The check here is to enable compile-time
			// optimization when N is not Dynamic.
			return (N >= 0) ? N : object.size();
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
