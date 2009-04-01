#ifndef PASTELRAY_LIGHT_H
#define PASTELRAY_LIGHT_H

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/spectrum.h"
#include "pastel/sys/countedptr.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	class PASTELRAY Light
		: public ReferenceCounted
	{
	public:
		Light();
		Light(const Light& that);
		virtual ~Light();
		Light& operator=(const Light& that);

		virtual void setPosition(const Point3& position) = 0;
		virtual Point3 getPosition() const = 0;

		virtual Vector3 sampleDirection() const = 0;
		virtual Spectrum radiance(const Vector3& direction) const = 0;
	};

	typedef CountedPtr<Light> LightPtr;
	typedef CountedPtr<const Light> ConstLightPtr;

}

#endif
