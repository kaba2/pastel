// Description: Detaches a half-edge from its origin.

#ifndef PASTELGEOMETRY_HALFMESH_DETACH_HALF_HPP
#define PASTELGEOMETRY_HALFMESH_DETACH_HALF_HPP

#include "pastel/geometry/halfmesh.h"

namespace Pastel
{
	
	template <
		typename Settings, 
		template <typename> class Customization>
	void HalfMesh<Settings, Customization>::detachHalf(
		const Half_ConstIterator& half)
	{
		if (!half.isNormal())
		{
			return;
		}

		Vertex_Iterator origin = cast(half->origin());
		Half_Iterator in = cast(half->previous());
		Half_Iterator out = cast(half->rotateNext());

		if (origin->half() == half)
		{
			origin->half_ = 
				(out == half) ?
				Half_Iterator() : out;
		}

		in->next_ = out;
		out->previous_ = in;
	}

}

#endif
