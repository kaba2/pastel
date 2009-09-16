#include "pastel/ray/perspectivelens.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/callfunction.h"

namespace Pastel
{

	PerspectiveLens::PerspectiveLens()
		: halfWidth_(1)
		, halfHeight_(1)
		, centerOfProjection_(0, 0, -1)
		, lensRadius_(0)
		, focalDistance_(10)
	{
	}

	PerspectiveLens::~PerspectiveLens()
	{
	}

	LensPtr PerspectiveLens::clone() const
	{
		return LensPtr(new PerspectiveLens);
	}

	Line3 PerspectiveLens::getLine(
		const Vector2& imagePosition) const
	{
		const Vector3 rayTarget(
			(imagePosition.x() - 0.5) * halfWidth_,
			(imagePosition.y() - 0.5) * halfHeight_,
			0);

		/*
		const Vector2 lensCoordinates(
		random<real>(), random<real>());
		const Vector2 lensPosition(
		sampleDisk(lensCoordinates));

		const Vector3 raySource(
		lensPosition.x() * lensRadius_,
		lensPosition.y() * lensRadius_,

		);
		*/

		const Vector3 raySource(centerOfProjection_);

		const Vector3 rayDirection(
			rayTarget - centerOfProjection_);

		return Line3(raySource,
			rayDirection / norm(rayDirection));
	}

	void PerspectiveLens::setCenterOfProjection(
		const Vector3& centerOfProjection)
	{
		centerOfProjection_ = centerOfProjection;
	}

	const Vector3& PerspectiveLens::centerOfProjection() const
	{
		return centerOfProjection_;
	}

	void PerspectiveLens::setHalfWidth(real halfWidth)
	{
		halfWidth_ = halfWidth;
	}

	real PerspectiveLens::halfWidth() const
	{
		return halfWidth_;
	}

	void PerspectiveLens::setHalfHeight(real halfHeight)
	{
		halfHeight_ = halfHeight;
	}

	real PerspectiveLens::halfHeight() const
	{
		return halfHeight_;
	}

	void PerspectiveLens::setAperture(real aperture)
	{
		lensRadius_ = aperture;
	}

	real PerspectiveLens::aperture() const
	{
		return lensRadius_;
	}

}
