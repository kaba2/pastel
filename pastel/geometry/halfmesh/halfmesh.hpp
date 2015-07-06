#ifndef PASTELGEOMETRY_HALFMESH_HPP
#define PASTELGEOMETRY_HALFMESH_HPP

#include "pastel/geometry/halfmesh/halfmesh.h"
#include "pastel/sys/ensure.h"

#include <unordered_map>

namespace Pastel
{

	template <
		typename Settings, 
		template <typename> class Customization>
	HalfMesh<Settings, Customization>::HalfMesh(
		const HalfMesh& that)
	: HalfMesh()
	{
		std::unordered_map<Vertex_ConstIterator, Vertex_Iterator> vertexMap;

		// Construct vertices along with a vertex map.
		{
			Vertex_ConstIterator thatVertex = that.vertexBegin();
			Vertex_ConstIterator thatEnd = that.vertexEnd();
			while(thatVertex != thatEnd)
			{
				Vertex_Iterator vertex = insertVertex(*thatVertex);
				ASSERT(vertex.isNormal());

				vertexMap.insert(
					std::make_pair(thatVertex, vertex));

				++thatVertex;
			}
		}

		// Construct edges.
		{
			Edge_ConstIterator thatEdge = that.edgeBegin();
			Edge_ConstIterator thatEnd = that.edgeEnd();
			while(thatEdge != thatEnd)
			{
				Vertex_ConstIterator origin = 
					thatEdge->half()->origin();
				ASSERT(vertexMap.count(origin));

				Vertex_ConstIterator destination =
					thatEdge->half()->destination();
				ASSERT(vertexMap.count(destination));

				Edge_Iterator edge = insertEdgeEdge(insertEdge(
					vertexMap[origin],
					vertexMap[destination],
					*thatEdge));
				ASSERT(edge.isNormal());
				unused(edge);

				++thatEdge;
			}
		}

		// Construct polygons.
		{
			Polygon_ConstIterator thatPolygon = that.polygonBegin();
			Polygon_ConstIterator thatEnd = that.polygonEnd();
			while(thatPolygon != thatEnd)
			{
				std::vector<Vertex_Iterator> vertexLoop;

				Half_ConstIterator halfBegin = thatPolygon->half();
				Half_ConstIterator half = halfBegin;
				do
				{
					vertexLoop.push_back(vertexMap[half->origin()]);
					half = half.next();
				}
				while(half != halfBegin);

				Polygon_Iterator polygon = 
					insertPolygon(vertexLoop, *thatPolygon);
				ASSERT(polygon.isNormal());
				unused(polygon);

				++thatPolygon;
			}
		}
	}

	// Modification

	template <
		typename Settings, 
		template <typename> class Customization>
	bool HalfMesh<Settings, Customization>::makeAdjacent(
		const Half_ConstIterator& in, 
		const Half_ConstIterator& out)
	{
		PENSURE(!in.empty());
		PENSURE(!out.empty());
		PENSURE(in->destination() == out->origin());
		PENSURE(in->left().empty());
		PENSURE(out->left().empty());

		if (in->next() == out)
		{
			// Nothing to do.
			return true;
		}

		Half_Iterator b = cast(in)->next();
		Half_Iterator d = cast(out)->previous();

		// Find a free incident half edge
		// after 'd' and before 'in'.
		Half_Iterator g = 
			findFreeIncident(out->origin(), out->pair(), in);
		if (g.empty())
		{
			// There is no such half-edge.
			return false;
		}
		Half_Iterator h = g->next();

		cast(in)->next_ = cast(out);
		cast(out)->previous_ = cast(in);

		g->next_ = b;
		b->previous_ = g;

		d->next_ = h;
		h->previous_ = d;

		return true;
	}

}

#endif
