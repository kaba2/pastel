#ifndef PASTELGEOMETRY_HALFMESH_HPP
#define PASTELGEOMETRY_HALFMESH_HPP

#include "pastel/geometry/halfmesh.h"

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
				ASSERT(!vertex.empty());

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
				Edge_Iterator edge = insertEdge(
					vertexMap[thatEdge->half()->origin()],
					vertexMap[thatEdge->half()->destination()],
					*thatEdge);
				ASSERT(!edge.empty());

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
				ASSERT(!polygon.empty());

				++thatPolygon;
			}
		}

		onConstruction();
	}

	// Modification

	template <
		typename Settings, 
		template <typename> class Customization>
	bool HalfMesh<Settings, Customization>::makeAdjacent(
		const Half_ConstIterator& in, 
		const Half_ConstIterator& out)
	{
		PENSURE(!in->empty());
		PENSURE(!out->empty());
		PENSURE(in->destination() == out->origin());
		PENSURE(in->left()->empty());
		PENSURE(out->left()->empty());

		if (in->next() == out)
		{
			// Nothing to do.
			return true;
		}

		Half_ConstIterator b = in->next();
		Half_ConstIterator d = out->previous();

		// Find a free incident half edge
		// after 'd' and before 'in'.
		Half_ConstIterator g = 
			findFreeIncident(out->origin(), out->pair(), in);
		if (g->empty())
		{
			// There is no such half-edge.
			return false;
		}
		Half_ConstIterator h = g->next();

		in->half_->next_ = out->half_;
		out->half_->previous_ = in->half_;

		g->half_->next_ = b->half_;
		b->half_->previous_ = g->half_;

		d->half_->next_ = h->half_;
		h->half_->previous_ = d->half_;

		return true;
	}

}

#endif
