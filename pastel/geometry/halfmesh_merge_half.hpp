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
		if (!half.isNormal())
		{
			return Polygon_Iterator();
		}

		Half_Iterator fromTo = cast(half);
		Polygon_Iterator left = fromTo->left();

		Half_Iterator toFrom = fromTo->pair();
		Polygon_Iterator right = toFrom->left();

		if (left.empty() || right.empty() ||
			left == right)
		{
			return Polygon_Iterator();
		}

		Edge_Iterator edge = cast(half->edge());

		// Remove the right polygon.
		removePolygon(right);

		// Detach the left polygon.
		detachPolygon(left);

		if (left->half() == fromTo)
		{
			// The left polygon references a to-be-removed half-edge.
			
			ASSERT(half->next() != fromTo);

			// Provide the left polygon with the next half-edge.
			left->half_ = fromTo->next();
		}

		// Remove the edge.
		removeEdge(edge);

		// Mark the merged polygon.
		linkPolygon(left->half(), left);

		// Notify the insertion of a new polygon.
		onInsertPolygon(left);

		// Return the expanded polygon.
		return left;
	}

	template <
		typename Settings, 
		template <typename> class Customization>
	void HalfMesh<Settings, Customization>::detachPolygon(
		const Polygon_ConstIterator& polygon)
	{
		// Act as if the left polygon were to be removed.
		onRemovePolygon(cast(polygon));

		// Detach the left polygon.
		linkPolygon(polygon->half(), Polygon_Iterator());
	}

	template <
		typename Settings, 
		template <typename> class Customization>
	void HalfMesh<Settings, Customization>::linkPolygon(
		const Half_ConstIterator& half,
		const Polygon_ConstIterator& left)
	{
		Half_Iterator h = cast(half);
		do
		{
			h->left_ = cast(left);
			h = h->next();
		}
		while(h != half);

		if (!left.empty())
		{
			cast(left)->half_ = cast(half);
		}
	}

}

#endif
