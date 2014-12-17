// Description: Removes an edge.

#ifndef PASTELGEOMETRY_HALFMESH_MERGE_HPP
#define PASTELGEOMETRY_HALFMESH_MERGE_HPP

#include "pastel/geometry/halfmesh.h"

namespace Pastel
{


	template <
		typename Settings, 
		template <typename> class Customization>
	auto HalfMesh<Settings, Customization>::removeEdge(
		const Edge_ConstIterator& edge)
	-> Edge_Iterator
	{
		if (edge.isEnd() || edge.empty())
		{
			return cast(edge);
		}

		this->onRemoveEdge(cast(edge));

		Half_Iterator fromTo = 
			cast(edge->half());
		ASSERT(!fromTo.empty());
		
		Edge_Iterator nextEdge = std::next(cast(edge));

		// Remove the left polygon.
		removePolygon(fromTo->left());

		// Merge the left polygon to the right polygon.
		merge(fromTo);

		return nextEdge;
	}

}

#endif
