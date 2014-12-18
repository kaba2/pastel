// Description: Half-edge structure invariants

#ifndef PASTELGEOMETRY_HALFMESH_INVARIANTS_H
#define PASTELGEOMETRY_HALFMESH_INVARIANTS_H

#include "pastel/geometry/halfmesh/halfmesh.h"

namespace Pastel
{

	//! Returns whether the invariants hold for a half-edge structure.	
	/*!
	Time complexity: O(mesh.vertices() + mesh.edges() + mesh.polygons())
	Exception safety: nothrow
	
	This function is useful only for testing and debugging. For
	a correct implementation this function always returns true.
	*/	
	template <
		typename Settings,
		template <typename> class Customization>
	bool testInvariants(const HalfMesh<Settings, Customization>& mesh);

}

#include "pastel/geometry/halfmesh/halfmesh_invariants.hpp"

#endif
