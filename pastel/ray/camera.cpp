#include "pastel/ray/camera.h"
#include "pastel/ray/beam_tools.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/vector_tools.h"


namespace Pastel
{

	Camera::Camera()
		: lens_()
		, objectToWorld_()
	{
	}

	Camera::Camera(const LensPtr& lens)
		: lens_(lens)
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

	void Camera::setPosition(const Vector3& position)
	{
		objectToWorld_.setTranslation(position);
	}

	Vector3 Camera::position() const
	{
		return Vector3(
			objectToWorld_.translation());
	}

	void Camera::setLens(const LensPtr& lens)
	{
		lens_ = lens;
	}

	LensPtr Camera::lens() const
	{
		return lens_;
	}

	Beam Camera::beam(const Vector2& uv) const
	{
		ENSURE(!lens_.empty());

		const Beam lensBeam(
			lens_->beam(uv));

		const Beam beam =
			transform(lensBeam, objectToWorld_.forward());

		// FIX: Normalize ray direction!

		return beam;
	}

}
