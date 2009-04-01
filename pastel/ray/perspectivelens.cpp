#include "pastel/ray/perspectivelens.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/point_tools.h"
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
		const Point2& imagePosition) const
	{
		const Point3 rayTarget(
			(imagePosition.x() - 0.5) * halfWidth_,
			(imagePosition.y() - 0.5) * halfHeight_,
			0);

		/*
		const Point2 lensCoordinates(
		randomReal(), randomReal());
		const Vector2 lensPosition(
		asVector(sampleDisk(lensCoordinates)));

		const Point3 raySource(
		lensPosition.x() * lensRadius_,
		lensPosition.y() * lensRadius_,

		);
		*/

		const Point3 raySource(centerOfProjection_);

		const Vector3 rayDirection(
			rayTarget - centerOfProjection_);

		return Line3(raySource,
			rayDirection / norm(rayDirection));
	}

	void PerspectiveLens::setCenterOfProjection(
		const Point3& centerOfProjection)
	{
		centerOfProjection_ = centerOfProjection;
	}

	const Point3& PerspectiveLens::centerOfProjection() const
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
