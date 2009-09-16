#ifndef PASTEL_CAMERA_H
#define PASTEL_CAMERA_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/lens.h"

#include "pastel/geometry/line.h"

#include "pastel/math/transformation.h"

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

		void setPosition(const Vector3& position);
		Vector3 position() const;

		void setLens(const LensPtr& lens);

		Line3 getLine(const Vector2& uv) const;

	private:
		LensPtr lens_;
		Transformation3 objectToWorld_;
	};

	typedef CountedPtr<Camera> CameraPtr;
	typedef CountedPtr<const Camera> ConstCameraPtr;

}

#endif
