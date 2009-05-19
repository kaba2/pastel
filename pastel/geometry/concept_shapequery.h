#ifndef PASTEL_CONCEPT_SHAPEQUERY_H
#define PASTEL_CONCEPT_SHAPEQUERY_H

#if (0 == 1)

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/line.h"
#include "pastel/sys/memoryview.h"

namespace Pastel
{

	template <typename ShapeHandle, int N, typename Real>
	class ConceptShapeBoundQuery
	{
	public:
		AlignedBox<N, Real> boundingAlignedBox(
			const ShapeHandle& handle) const;
	};

	template <typename ShapeHandle, int N, typename Real>
	class ConceptIntersectionQuery
	{
	public:
		bool intersect(
			const ConstMemoryView<ShapeHandle>& table,
			const Line<N, Real>& line,
			Real& tClosest,
			integer& indexClosest) const;
	};

}

#endif

#endif
