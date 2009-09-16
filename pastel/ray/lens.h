#ifndef PASTEL_LENS_H
#define PASTEL_LENS_H

#include "pastel/sys/mytypes.h"
#include "pastel/ray/raylibrary.h"
#include "pastel/sys/countedptr.h"
#include "pastel/sys/vector.h"

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

		virtual Line3 getLine(const Vector2& uv) const = 0;
	};

	typedef CountedPtr<Lens> LensPtr;
	typedef CountedPtr<const Lens> ConstLensPtr;

}

#endif
