// Description: Pinhole lens
// Documentation: lenses.txt

#ifndef PASTEL_PINHOLE_LENS_H
#define PASTEL_PINHOLE_LENS_H

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/lens.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	class PASTELRAY Pinhole_Lens
		: public Lens
	{
	public:
		Pinhole_Lens();
		virtual ~Pinhole_Lens();

		virtual LensPtr clone() const;

		virtual Beam beam(const Vector2& position) const;

		void setWidth(real width);
		real width() const;

		void setHeight(real height);
		real height() const;

		void setDistance(real distance);
		real distance() const;

	private:
		real width_;
		real height_;
		real distance_;
	};

}

#endif
