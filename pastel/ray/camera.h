// Description: Camera
// Documentation: pastelray.txt

#ifndef PASTELRAY_CAMERA_H
#define PASTELRAY_CAMERA_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/lens.h"

#include "pastel/math/affinebijection.h"

#include "pastel/geometry/line.h"

namespace Pastel
{

	class PASTELRAY Camera
		: public ReferenceCounted
	{
	public:
		Camera();
		explicit Camera(const LensPtr& lens);
		Camera(const Camera& that);
		~Camera();
		Camera& operator=(const Camera& that);

		void swap(Camera& that);

		void setPosition(const Vector3& position);
		Vector3 position() const;

		void setLens(const LensPtr& lens);
		LensPtr lens() const;

		Beam beam(const Vector2& uv) const;

	private:
		LensPtr lens_;
		AffineBijection3 objectToWorld_;
	};

}

#endif
