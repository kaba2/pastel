// Description: Merges left and right polygons of a half-edge.

#ifndef PASTELGEOMETRY_HALFMESH_MERGE_HALF_HPP
#define PASTELGEOMETRY_HALFMESH_MERGE_HALF_HPP

#include "pastel/geometry/halfmesh.h"

namespace Pastel
{
	
	template <
		typename Settings, 
		template <typename> class Customization>
	auto HalfMesh<Settings, Customization>::merge(
		const Half_ConstIterator& half)
	-> Polygon_Iterator
	{
		ENSURE(!half.empty());

		if (half.isEnd())
		{
			return Polygon_Iterator();
		}

		Edge_Iterator edge = cast(half->edge());

		Half_Iterator fromTo = cast(half);
		Polygon_Iterator left = fromTo->left();

		Half_Iterator toFrom = fromTo->pair();
		Polygon_Iterator right = toFrom->left();

		if (right != left)
		{
			// Remove the right polygon.
			removePolygon(right);

			// Connect the half-edges in the right
			// polygon to the left polygon; this 
			// expands the left polygon.
			Half_Iterator h = toFrom;
			do
			{
				h->left() = left;
				h = h->next();
			}
			while(h != toFrom);
		}

		// Note that we notify the user before
		// removing the half-edges; the user has
		// to have a consistent view of the data
		// at all times.
		onRemoveEdge(edge);

		// Link the from-side of the edge off the model.
		detachHalf(fromTo);

		// Link the to-side of the edge off the model.
		detachHalf(toFrom);

		// Deallocate half-edges.
		halfSet_.erase(fromTo);
		halfSet_.erase(toFrom);

		// Deallocate edge.
		edgeSet_.erase(edge);

		// Return the expanded polygon.
		return left;
	}

}

#endif
