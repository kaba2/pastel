#include "pastel/ray/pinhole_lens.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/callfunction.h"

namespace Pastel
{

	Pinhole_Lens::Pinhole_Lens()
		: halfWidth_(1)
		, halfHeight_(1)
		, distance_(1)
	{
	}

	Pinhole_Lens::~Pinhole_Lens()
	{
	}

	LensPtr Pinhole_Lens::clone() const
	{
		return LensPtr(new Pinhole_Lens);
	}

	Beam Pinhole_Lens::beam(
		const Vector2& position) const
	{
		const Vector3 rayPosition(
			(position.x() - 0.5) * halfWidth_,
			(position.y() - 0.5) * halfHeight_,
			distance_);
		
		const real length = norm(rayPosition);

		const Vector3 rayDirection =
			rayPosition / length;

		const Vector3 xRayPosition(
			halfWidth_,	0, 0);

		const Vector3 yRayPosition(
			0, halfHeight_, 0);

		const Vector3 xRayDirection =
			Vector3(halfWidth_ / length, 0, 0) - 
			rayDirection * (square(halfWidth_ / length) * (position.x() - 0.5));

		const Vector3 yRayDirection =
			Vector3(halfHeight_ / length, 0, 0) - 
			rayDirection * (square(halfHeight_ / length) * (position.y() - 0.5));

		const Beam result(
			Ray3(rayPosition, rayDirection),
			Ray3(xRayPosition, xRayDirection),
			Ray3(yRayPosition, yRayDirection));

		return result;
	}

	void Pinhole_Lens::setHalfWidth(real halfWidth)
	{
		halfWidth_ = halfWidth;
	}

	real Pinhole_Lens::halfWidth() const
	{
		return halfWidth_;
	}

	void Pinhole_Lens::setHalfHeight(real halfHeight)
	{
		halfHeight_ = halfHeight;
	}

	real Pinhole_Lens::halfHeight() const
	{
		return halfHeight_;
	}

	void Pinhole_Lens::setDistance(real distance)
	{
		distance_ = distance;
	}

	real Pinhole_Lens::distance() const
	{
		return distance_;
	}

}
