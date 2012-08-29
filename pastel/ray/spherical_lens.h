// Description: Spherical lens
// Documentation: lenses.txt

#ifndef PASTELRAY_SPHERICAL_LENS_H
#define PASTELRAY_SPHERICAL_LENS_H

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/lens.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	class PASTELRAY Spherical_Lens
		: public Lens
	{
	public:
		Spherical_Lens();
		virtual ~Spherical_Lens();

		virtual LensPtr clone() const;

		virtual Beam beam(const Vector2& position) const;

		void setWindow(const AlignedBox2& window);
		const AlignedBox2& window() const;

		void setRadius(real radius);
		real radius() const;

	private:
		AlignedBox2 window_;
		real radius_;
	};

	typedef CountedPtr<Spherical_Lens> Spherical_LensPtr;

}

#endif
