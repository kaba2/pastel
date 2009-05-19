#ifndef PASTEL_CAMERA_H
#define PASTEL_CAMERA_H

#include "pastel/sys/mytypes.h"
#include "pastel/ray/raylibrary.h"
#include "pastel/ray/lens.h"

#include "pastel/geometry/line.h"
#include "pastel/math/transformation.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	class PASTELRAY Camera
		: public ReferenceCounted
	{
	public:
		Camera();
		Camera(const Camera& that);
		~Camera();
		Camera& operator=(const Camera& that);

		void swap(Camera& that);

		void setPosition(const Point3& position);
		Point3 position() const;

		void setLens(const LensPtr& lens);

		Line3 getLine(const Point2& uv) const;

	private:
		LensPtr lens_;
		Transformation3 objectToWorld_;
	};

	typedef CountedPtr<Camera> CameraPtr;
	typedef CountedPtr<const Camera> ConstCameraPtr;

}

#endif
