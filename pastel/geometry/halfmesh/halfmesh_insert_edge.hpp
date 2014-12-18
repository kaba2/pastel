// Description: Inserts an edge.

#ifndef PASTELGEOMETRY_HALFMESH_INSERT_EDGE_HPP
#define PASTELGEOMETRY_HALFMESH_INSERT_EDGE_HPP

#include "pastel/geometry/halfmesh/halfmesh.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	template <typename... Type>
	auto HalfMesh<Settings, Customization>::insertEdge(
		const Vertex_ConstIterator& from, 
		const Vertex_ConstIterator& to,
		Type&&... data)
		-> InsertEdge_Return
	{
		ENSURE(from.isNormal());
		ENSURE(to.isNormal());

		if (!Loops)
		{
			if (from == to)
			{
				return insertEdgeReturn(edgeEnd(), false);
			}
		}

		if (!MultipleEdges)
		{
			Half_Iterator existingHalf = findHalf(from, to);
			if (!existingHalf.empty())
			{
				return insertEdgeReturn(existingHalf->edge(), false);
			}
		}

		// Check that the vertices are free. If not, the edge 
		// addition is not possible.

		// This will be reused later..
		Half_Iterator fromFree;

		if (!from->isolated())
		{
			fromFree = findFreeIncident(from);
			if (fromFree.empty())
			{
				return insertEdgeReturn(edgeEnd(), false);
			}
		}

		if (!to->isolated())
		{
			Half_Iterator toFreeHalf = findFreeIncident(to);
			if (toFreeHalf.empty())
			{
				return insertEdgeReturn(edgeEnd(), false);
			}
		}

		// Allocate data

		integer rollback = 0;

		Edge_Iterator edge;
		Half_Iterator fromTo;
		Half_Iterator toFrom;

		try
		{
			edge = edgeSet_.insertBack(std::forward<Type>(data)...);
			++rollback;

			fromTo = halfSet_.insertBack();
			++rollback;

			toFrom = halfSet_.insertBack();
			++rollback;
		}
		catch(...)
		{
			switch(rollback)
			{
			case 2:
				halfSet_.erase(fromTo);
				// Fall-through
			case 1:
				edgeSet_.erase(edge);
				break;
			};

			throw;
		};

		// Initialize data

		{
			edge->half_ = fromTo;
		}

		{
			fromTo->next_ = toFrom;
			fromTo->previous_ = toFrom;

			fromTo->pair_ = toFrom;
			fromTo->origin_ = cast(from);
			fromTo->edge_ = edge;
			fromTo->left_ = Polygon_Iterator();
		}

		{
			toFrom->next_ = fromTo;
			toFrom->previous_ = fromTo;

			toFrom->pair_ = fromTo;
			toFrom->origin_ = cast(to);
			toFrom->edge_ = edge;
			toFrom->left_ = Polygon_Iterator();
		}

		// Link the from-side of the edge.

		if (from->isolated())
		{
			cast(from)->half_ = fromTo;
		}
		else
		{
			Half_Iterator fromIn = fromFree;

			ASSERT(!fromIn.empty());

			Half_Iterator fromOut = fromIn->next();

			fromIn->next_ = fromTo;
			fromTo->previous_ = fromIn;

			toFrom->next_ = fromOut;
			fromOut->previous_ = toFrom;
		}

		// Link the to-side of the edge.

		if (to->isolated())
		{
			cast(to)->half_ = toFrom;
		}
		else
		{
			// 'toFreeHalf' can't be reused here,
			// because the perimeter has changed
			// due to the linking of the from-side.
			Half_Iterator toIn = findFreeIncident(to);
			ASSERT(!toIn.empty());

			Half_Iterator toOut = toIn->next();

			toIn->next_ = toFrom;
			toFrom->previous_ = toIn;

			fromTo->next_ = toOut;
			toOut->previous_ = fromTo;
		}

		this->onInsertEdge(edge);

		return insertEdgeReturn(edge, true);
	}

}

#endif
