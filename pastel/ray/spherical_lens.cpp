#include "pastel/ray/spherical_lens.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	Spherical_Lens::Spherical_Lens()
		: window_(-constantPi<real>() / 2, -constantPi<real>() / 2,
		constantPi<real>() / 2, constantPi<real>() / 2)
		, radius_(1)
	{
	}

	Spherical_Lens::~Spherical_Lens()
	{
	}

	LensPtr Spherical_Lens::clone() const
	{
		return LensPtr(new Spherical_Lens);
	}

	Vector3 normalizedDerivative(
		const Vector3 pUnit,
		const real pLength,
		const Vector3 dpDu,
		real dlDu)
	{
		// f(u, v) = p / |p| = r
		//
		// (df / du)(u, v) 
		// = [(dp / du)(u, v) |p| - p (d|p| / du)(u, v)] / |p|^2
		// = [(dp / du)(u, v) - (p / |p|) (d|p| / du)(u, v)] / |p|
		// = [(dp / du)(u, v) - r (d|p| / du)(u, v)] / |p|

		return (dpDu - pUnit * dlDu) / pLength;
	}

	Beam Spherical_Lens::beam(
		const Vector2& position) const
	{
		// s = window.min().x() + w * u 
		// t = window.min().y() + h * (v + pi / 2)
		// where
		// w = window.extent().x()
		// h = window.extent().y()

		const Vector2 st = 
			window_.at(position + Vector2(0, constantPi<real>() / 2));

		const real& s = st[0];
		const real& t = st[1];

		const real wr = window_.extent()[0] * radius_;
		const real hr = window_.extent()[1] * radius_;

		const real sCos = std::cos(s);
		const real sSin = std::sin(s);
		const real tCos = std::cos(t);
		const real tSin = std::sin(t);

		const real a = tCos * sCos;
		const real b = tSin * sCos;
		const real c = tCos * sSin;
		const real d = tSin * sSin;

		// (ds / du)(u, v) = w
		// (ds / dv)(u, v) = 0
		// (dt / du)(u, v) = 0
		// (dt / dv)(u, v) = h

		// p(s, t) = (x, y, z)
		// x = -r sin(t) cos(s)
		// y = -r cos(t)
		// z = r sin(t) sin(s)

		const Vector3 rayPosition(
			-radius_ * b,
			-radius_ * tCos,
			radius_ * d);

		// (dp / ds)(s, t):
		// x = r sin(t) sin(t) w
		// y = 0
		// z = r sin(t) cos(s) w

		const Vector3 xRayPosition(
			d * wr,
			0, 
			b * wr);

		// (dp / dt)(s, t):
		// x = -r cos(t) cos(s) h
		// y = r sin(t) h
		// z = r cos(t) sin(s) h
		//
		// l(u, v) = |p(s, t)| = r
		//
		// (dl / ds)(s, t) = 0
		// (dl / dt)(s, t) = 0

		const Vector3 yRayPosition(
			-a * hr,
			tSin * hr,
			c * hr);

		const real length = norm(rayPosition);
		const real invLength = inverse(length);

		const Vector3 rayDirection =
			rayPosition * invLength;

		const Vector3 xRayDirection =
			xRayPosition * invLength;

		const Vector3 yRayDirection =
			yRayPosition * invLength;

		const Beam result(
			Ray3(rayPosition, rayDirection),
			Ray3(xRayPosition, xRayDirection),
			Ray3(yRayPosition, yRayDirection));

		return result;
	}

	void Spherical_Lens::setWindow(
		const AlignedBox2& window)
	{
		window_ = window;
	}

	const AlignedBox2& Spherical_Lens::window() const
	{
		return window_;
	}

	void Spherical_Lens::setRadius(real radius)
	{
		ENSURE_OP(radius, >, 0);

		radius_ = radius;
	}

	real Spherical_Lens::radius() const
	{
		return radius_;
	}

}
