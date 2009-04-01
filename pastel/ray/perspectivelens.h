#ifndef PASTELRAY_PERPSPECTIVELENS_H
#define PASTELRAY_PERPSPECTIVELENS_H

#include "pastel/sys/mytypes.h"
#include "pastel/ray/raylibrary.h"
#include "pastel/ray/lens.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/point.h"
#include "pastel/geometry/line.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	class PASTELRAY PerspectiveLens
		: public Lens
	{
	public:
		PerspectiveLens();
		virtual ~PerspectiveLens();

		virtual LensPtr clone() const;

		virtual Line3 getLine(const Point2& imagePosition) const;

		void setCenterOfProjection(
			const Point3& centerOfProjection);
		const Point3& centerOfProjection() const;

		void setHalfWidth(real halfWidth);
		real halfWidth() const;

		void setHalfHeight(real halfHeight);
		real halfHeight() const;

		void setAperture(real aperture);
		real aperture() const;

	private:
		real halfWidth_;
		real halfHeight_;
		Point3 centerOfProjection_;
		real lensRadius_;
		real focalDistance_;
	};

}

#endif
