#include "pastel/ray/pinhole_lens.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/callfunction.h"

namespace Pastel
{

	Pinhole_Lens::Pinhole_Lens()
		: width_(1)
		, height_(1)
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
			(position.x() - 0.5) * width_,
			(position.y() - 0.5) * height_,
			distance_);
		
		const real length = norm(rayPosition);

		const Vector3 rayDirection =
			rayPosition / length;

		const Vector3 xRayPosition(
			width_,	0, 0);

		const Vector3 yRayPosition(
			0, height_, 0);

		const Vector3 xRayDirection =
			Vector3(width_ / length, 0, 0) - 
			rayDirection * (square(width_ / length) * (position.x() - 0.5));

		const Vector3 yRayDirection =
			Vector3(height_ / length, 0, 0) - 
			rayDirection * (square(height_ / length) * (position.y() - 0.5));

		const Beam result(
			Ray3(rayPosition, rayDirection),
			Ray3(xRayPosition, xRayDirection),
			Ray3(yRayPosition, yRayDirection));

		return result;
	}

	void Pinhole_Lens::setWidth(real width)
	{
		width_ = width;
	}

	real Pinhole_Lens::width() const
	{
		return width_;
	}

	void Pinhole_Lens::setHeight(real height)
	{
		height_ = height;
	}

	real Pinhole_Lens::height() const
	{
		return height_;
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
