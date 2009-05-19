#ifndef PASTEL_DELAUNAYTESSELATION_TOOLS_H
#define PASTEL_DELAUNAYTESSELATION_TOOLS_H

#include "pastel/geometry/delaunaytesselation.h"
#include "pastel/geometry/flatsimplexmesh.h"

namespace Pastel
{

	//! Converts a DelaunayTesselation to a SimplexMesh.
	/*!
	Time complexity:
	O(delaunay.vertices() + delaunay.triangles())

	Exception safety:
	strong
	*/

	template <typename Real, typename DelaunayDataPolicy, typename MeshDataPolicy>
	void convert(
		const DelaunayTesselation<Real, DelaunayDataPolicy>& delaunay,
		FlatSimplexMesh<2, Real, MeshDataPolicy>& mesh);

}

#include "pastel/geometry/delaunaytesselation_tools.hpp"

#endif
