#include "pastel/ray/camera.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	namespace Yun
	{

		Camera::Camera()
			: lens_()
			, objectToWorld_()
		{
		}

		Camera::Camera(const Camera& that)
			: lens_(that.lens_)
			, objectToWorld_(that.objectToWorld_)
		{
		}

		Camera::~Camera()
		{
		}

		Camera& Camera::operator=(const Camera& that)
		{
			lens_ = that.lens_;
			objectToWorld_ = that.objectToWorld_;
			return *this;
		}

		void Camera::swap(Camera& that)
		{
			lens_.swap(that.lens_);
			objectToWorld_.swap(that.objectToWorld_);
		}

		void Camera::setPosition(const Point3& position)
		{
			objectToWorld_.setTranslation(asVector(position));
		}

		Point3 Camera::position() const
		{
			return Point3(
				objectToWorld_.translation());
		}

		void Camera::setLens(const LensRef& lens)
		{
			lens_ = lens;
		}

		Line3 Camera::getLine(const Point2& uv) const
		{
			ENSURE(!lens_.empty());

			const Line3 objectLine(
				lens_->getLine(uv));

			const AffineTransformation3& toWorld =
				objectToWorld_.affineTransform();

			const Line3 cameraLine(
				objectLine.position() * toWorld,
				objectLine.direction() * toWorld);

			return Line3(cameraLine.position(),
				cameraLine.direction() / norm(cameraLine.direction()));
		}

	}

}
