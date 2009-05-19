#ifndef PASTEL_LOCALGEOMETRY_H
#define PASTEL_LOCALGEOMETRY_H

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include "pastel/ray/raylibrary.h"

namespace Pastel
{

	class PASTELRAY LocalGeometry
	{
	public:
		Point3 position_;
		Vector3 dpDu_;
		Vector3 dpDv_;
		Vector3 normal_;
		Vector3 dnDu_;
		Vector3 dnDv_;
	};

}

#endif
