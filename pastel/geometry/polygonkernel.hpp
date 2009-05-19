#ifndef PASTEL_POLYGONKERNEL_HPP
#define PASTEL_POLYGONKERNEL_HPP

#include "pastel/geometry/polygonkernel.h"

#include "pastel/sys/ensure.h"

#include "pastel/geometry/polygon.h"
#include "pastel/geometry/plane.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/geometry/intersect_line_plane.h"

namespace Pastel
{

	template <integer N, typename Real>
	Polygon<N, Real> clip(
		const Polygon<N, Real>& input,
		const Plane<N, Real>& plane)
	{
		Polygon<N, Real> result;

		const integer vertices = input.size();

		if (vertices == 0)
		{
			return result;
		}

		result.reserve(input.size());

		bool previousSide = dot(input[vertices - 1] - plane.position(), plane.normal()) >= 0;
		integer previous = vertices - 1;

		for (integer i = 0;i < vertices;++i)
		{
			const bool currentSide = dot(input[i] - plane.position(), plane.normal()) >= 0;

			if (currentSide != previousSide)
			{
				real t = 0;
				const Line<N, Real> ray(input[previous], input[i] - input[previous]);

				const bool intersected = intersect(ray, plane, t);
				ASSERT(intersected);

				result.pushBack(ray.at(t));
			}
			if (currentSide)
			{
				result.pushBack(input[i]);
			}

			previousSide = currentSide;
			previous = i;
		}

		return result;
	}

	template <typename Real>
	Polygon<2, Real> polygonKernel(const Polygon<2, Real>& input)
	{
		Polygon<2, Real> result(input);
		const integer vertices = input.size();

		for (integer i = 0;i < vertices;++i)
		{
			const integer next = i == vertices - 1 ? 0 : i + 1;
			const Vector<2, Real> normal(cross(input[next] - input[i]));
			result = clip(result,
				Plane<2, Real>(input[i], normal / norm(normal)));
		}

		return result;
	}

}

#endif
