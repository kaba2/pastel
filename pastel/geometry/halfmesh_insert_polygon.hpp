#ifndef PASTELGEOMETRY_HALFMESH_INSERT_POLYGON_HPP
#define PASTELGEOMETRY_HALFMESH_INSERT_POLYGON_HPP

#include "pastel/geometry/halfmesh.h"

#include <type_traits>

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	template <
		typename Half_Range,
		typename... Type>
	auto HalfMesh<Settings, Customization>::insertPolygon(
		const Half_Range& halfSet,
		Type&&... data)
	-> PASTEL_ENABLE_IF((IsConvertible<Half_Range, Half_ConstIterator>), Polygon_Iterator)
	{
		if (halfSet.empty())
		{
			// Nothing to do.
			return polygonEnd();
		}

		// Check that the half-edges are free and form a chain.
		{
			auto i = std::begin(halfSet);
			auto end = std::end(halfSet);
			while (i != end)
			{
				auto j = std::next(i);
				if (j == end)
				{
					j = std::begin(halfSet);
				}

				Half_ConstIterator current = *i;
				ENSURE(current.isNormal());

				Half_ConstIterator next = *j;
				ENSURE(next.isNormal());

				bool formsLoop =
					(current->destination() == next->origin());
				ENSURE(formsLoop);

				if (!current->free())
				{
					// The polygon would introduce
					// a non-manifold condition.
					return polygonEnd();
				}

				++i;
			}
		}

		// Try to reorder the links to get a proper orientation.
		{
			auto i = std::begin(halfSet);
			auto end = std::end(halfSet);
			while (i != end)
			{
				auto j = std::next(i);
				if (j == end)
				{
					j = std::begin(halfSet);
				}

				if (!makeAdjacent(*i, *j))
				{
					// The polygon would introduce
					// a non-manifold condition.
					return polygonEnd();
				}
				
				++i;
			}
		}

		// Create the polygon.
		Polygon_Iterator polygon =
			polygonSet_.insertBack(std::forward<Type>(data)...);

		// Link the polygon.
		{
			// Link the polygon to a half-edge.
			polygon->half_ = *std::begin(halfSet);

			// Link the half-edges to the polygon.
			for (auto&& half : halfSet)
			{
				half->left_ = polygon;
			}
		}

		this->onInsertPolygon(polygon);

		// Return the polygon.
		return polygon;
	}

	template <
		typename Settings, 
		template <typename> class Customization>
	template <
		typename Vertex_Range,
		typename... Type>
	auto HalfMesh<Settings, Customization>::insertPolygon(
		const Vertex_Range& vertexSet,
		Type&&... data)
	-> PASTEL_ENABLE_IF((IsConvertible<Vertex_Range, Vertex_ConstIterator>), Polygon_Iterator)
	{
		ENSURE(!vertexSet.empty());

		// Check that the vertices are free and normal.
		integer n = vertexSet.size();
		for (integer i = 0;i < n;++i)
		{
			ENSURE(vertexSet[i].isNormal());

			if (!vertexSet[i]->free())
			{
				return polygonEnd();
			}
		}

		// Find the existing half-edges.
		// Check that they are free.

		std::vector<Half_Iterator> halfSet;
		halfSet.reserve(n);

		for (integer i = 0;i < n;++i)
		{
			integer j = i + 1;
			if (j == n)
			{
				j = 0;
			}

			Half_Iterator half = 
				findHalf(vertexSet[i], vertexSet[j]);

			if (!half.empty() && !half->free())
			{
				// A non-manifold condition.
				return polygonEnd();
			}

			halfSet.push_back(half);
		}

		// This array will contain the
		// newly created edges so that
		// we can roll back in case of exceptions.

		std::vector<Edge_Iterator> edgeSet;
		edgeSet.reserve(n);

		Polygon_Iterator polygon;

		try
		{
			for (integer i = 0;i < n;++i)
			{
				integer j = i + 1;
				if (j == n)
				{
					j = 0;
				}

				if (halfSet[i].empty())
				{
					Edge_Iterator newEdge = insertEdge(
						vertexSet[i], vertexSet[j]);
					ASSERT(newEdge.isNormal());

					Half_Iterator newHalf = newEdge->half();
					if (newHalf->origin() != vertexSet[i])
					{
						newHalf = newHalf->pair();
						ASSERT(newHalf->origin() == vertexSet[i]);
					}

					halfSet[i] = newHalf;

					try
					{
						edgeSet.push_back(newEdge);
					}
					catch(...)
					{
						// Roll back the edge addition.
						// Continue handling in the outer
						// level...
						removeEdge(newEdge);
						throw;
					}
				}
			}

			polygon = insertPolygon(halfSet, std::forward<Type>(data)...);
			if (polygon.empty())
			{
				// Note this case should not throw
				// an exception.

				// Roll back edge additions.

				integer edges = edgeSet.size();
				for (integer i = 0;i < edges;++i)
				{
					removeEdge(edgeSet[i]);
				}

				return polygonEnd();
			}
		}
		catch(...)
		{
			// Roll back edge additions.

			integer edges = edgeSet.size();
			for (integer i = 0;i < edges;++i)
			{
				removeEdge(edgeSet[i]);
			}

			throw;
		}

		return polygon;
	}

}

#endif
