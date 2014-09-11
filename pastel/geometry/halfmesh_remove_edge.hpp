#ifndef PASTELGEOMETRY_HALFMESH_REMOVE_EDGE_HPP
#define PASTELGEOMETRY_HALFMESH_REMOVE_EDGE_HPP

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
		ENSURE(!edge.empty());

		if (edge.isEnd())
		{
			return cast(edge);
		}

		this->onRemoveEdge(cast(edge));

		Half_Iterator fromToHalf = 
			cast(edge->half());
		ASSERT(!fromToHalf.empty());
		
		Half_Iterator toFromHalf = 
			fromToHalf->pair();

		// Remove the neighbouring polygons.

		if (!fromToHalf->left().empty())
		{
			removePolygon(fromToHalf->left());
		}

		if (!toFromHalf->left().empty())
		{
			removePolygon(toFromHalf->left());
		}

		// Link the from-side of the edge off the model.

		Vertex_Iterator fromVertex = fromToHalf->origin();
		Half_Iterator fromIn = fromToHalf->previous();
		Half_Iterator fromOut = fromToHalf->rotateNext();

		if (fromVertex->half() == fromToHalf)
		{
			fromVertex->half_ = 
				(fromOut == fromToHalf) ?
				Half_Iterator() : fromOut;
		}

		fromIn->next_ = fromOut;
		fromOut->previous_ = fromIn;

		// Link the to-side of the edge off the model.

		Vertex_Iterator toVertex = toFromHalf->origin();
		Half_Iterator toIn = toFromHalf->previous();
		Half_Iterator toOut = toFromHalf->rotateNext();

		if (toVertex->half() == toFromHalf)
		{
			toVertex->half_ = 
				(toOut == toFromHalf) ?
				Half_Iterator() : toOut;
		}

		toIn->next_ = toOut;
		toOut->previous_ = toIn;

		// Deallocate data
		halfSet_.erase(fromToHalf);
		halfSet_.erase(toFromHalf);

		return edgeSet_.erase(edge);
	}

}

#endif
