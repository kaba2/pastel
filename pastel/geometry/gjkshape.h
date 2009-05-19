#ifndef PASTEL_GJKSHAPE_H
#define PASTEL_GJKSHAPE_H

#include "pastel/geometry/geometrylibrary.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

#include "pastel/sys/countedptr.h"

namespace Pastel
{

	template <int N, typename Real>
	class GjkShape
		: public ReferenceCounted
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		typedef CountedPtr<GjkShape> Ptr;

		virtual ~GjkShape() {};

		virtual Point<N, Real> support(const Vector<N, Real>& direction) const = 0;
	};

}

#endif
