// Description: A beam of light
// Documentation: pastelray.txt

#ifndef PASTELRAY_BEAM_H
#define PASTELRAY_BEAM_H

#include "pastel/ray/raylibrary.h"

#include "pastel/geometry/ray.h"

namespace Pastel
{

	class PASTELRAY Beam
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Beam();
		Beam(const Ray3& ray,
			const Ray3& ddx,
			const Ray3& ddy);
			
		Ray3& ray();
		const Ray3& ray() const;

		Ray3& ddx();
		const Ray3& ddx() const;

		Ray3& ddy();
		const Ray3& ddy() const;

	private:
		Ray3 ray_;
		Ray3 xRay_;
		Ray3 yRay_;
	};

}

#endif
