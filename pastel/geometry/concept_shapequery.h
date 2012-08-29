#ifndef PASTELGEOMETRY_CONCEPT_SHAPEQUERY_H
#define PASTELGEOMETRY_CONCEPT_SHAPEQUERY_H

#if (0 == 1)

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	template <typename ShapeHandle, typename Real, int N>
	class ConceptShapeBoundQuery
	{
	public:
		AlignedBox<Real, N> boundingAlignedBox(
			const ShapeHandle& handle) const;
	};

	template <typename ShapeHandle, typename Real, int N>
	class ConceptIntersectionQuery
	{
	public:
		bool intersect(
			const ConstMemoryView<ShapeHandle>& table,
			const Line<Real, N>& line,
			Real& tClosest,
			integer& indexClosest) const;
	};

}

#endif

#endif
