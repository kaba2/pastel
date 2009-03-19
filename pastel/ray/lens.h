#ifndef PASTELRAY_LENS_H
#define PASTELRAY_LENS_H

#include "pastel/sys/mytypes.h"
#include "pastel/ray/raylibrary.h"
#include "pastel/sys/countedptr.h"
#include "pastel/sys/point.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	class PASTELRAY Lens
		: public ReferenceCounted
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		virtual ~Lens();

		virtual CountedPtr<Lens> clone() const = 0;

		virtual Line3 getLine(const Point2& uv) const = 0;
	};

	typedef CountedPtr<Lens> LensRef;
	typedef CountedPtr<const Lens> ConstLensRef;

}

#endif
