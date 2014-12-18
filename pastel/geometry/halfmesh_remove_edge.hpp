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

		Half_Iterator toFrom =
			fromTo->pair();
		
		// Remove the left polygon.
		removePolygon(fromTo->left());

		// Remove the right polygon.
		removePolygon(toFrom->left());

		// Link the from-side of the edge off the model.
		detachHalf(fromTo);

		// Link the to-side of the edge off the model.
		detachHalf(toFrom);

		// Deallocate half-edges.
		halfSet_.erase(fromTo);
		halfSet_.erase(toFrom);

		// Deallocate edge.
		return edgeSet_.erase(edge);
	}

}

#endif
