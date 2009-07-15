// Description: Some tools for triangle meshes
// Documentation: common.txt

#ifndef PASTEL_MESHCOMPUTATION_H
#define PASTEL_MESHCOMPUTATION_H

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/trianglemesh.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <
		typename VertexKey,
		typename TriangleKey,
		typename Real>
	void computeTriangleNormals(
		TriangleMesh& mesh,
		const PropertyId<VertexKey, Point<3, Real> >& vertexPositionId,
		const PropertyId<TriangleKey, Vector<3, Real> >& triangleNormalId);

	template <
		typename VertexKey,
		typename Real>
	void computeVertexNormals(
		TriangleMesh& mesh,
		const PropertyId<VertexKey, Point<3, Real> >& vertexPositionId,
		const PropertyId<VertexKey, Vector<3, Real> >& vertexNormalId);

	template <
		typename VertexKey,
		typename Real>
	void computeSphereParametrization(
		TriangleMesh& mesh,
		const PropertyId<VertexKey, Point<3, Real> >& vertexPositionId,
		const PropertyId<VertexKey, Point<2, Real> >& vertexUvId);

}

#include "pastel/geometry/meshcomputation.hpp"

#endif
