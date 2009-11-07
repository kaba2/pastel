#ifndef PASTEL_RAYFORWARD_H
#define PASTEL_RAYFORWARD_H

#include "pastel/ray/raylibrary.h"

#include "pastel/sys/countedptr.h"

namespace Pastel
{

	class PASTELRAY Thing;
	typedef CountedPtr<Thing> ThingPtr;

	class PASTELRAY Shape_Thing;
	typedef CountedPtr<Shape_Thing> Shape_ThingPtr;

	class PASTELRAY Camera;
	typedef CountedPtr<Camera> CameraPtr;

	class PASTELRAY Shape;
	typedef CountedPtr<Shape> ShapePtr;

	class PASTELRAY Lens;
	typedef CountedPtr<Lens> LensPtr;

	class PASTELRAY Brdf;
	typedef CountedPtr<Brdf> BrdfPtr;

}

#endif
