#ifndef PASTELGEOMETRY_HALFMESH_INVARIANTS_HPP
#define PASTELGEOMETRY_HALFMESH_INVARIANTS_HPP

#include "pastel/geometry/halfmesh_invariants.h"

namespace Pastel
{
	
	template <
		typename Settings,
		template <typename> class Customization>
	bool testInvariants(const HalfMesh<Settings, Customization>& mesh)
	{
		typedef HalfMesh<Settings, Customization> Mesh;
		
		using Fwd = Mesh;
		PASTEL_FWD(Vertex_ConstIterator);
		PASTEL_FWD(Half_ConstIterator);
		PASTEL_FWD(Edge_ConstIterator);
		PASTEL_FWD(Polygon_ConstIterator);

		// Check vertex invariants
		{
			Vertex_ConstIterator vertex =
				mesh.vertexBegin();
			Vertex_ConstIterator vertexEnd =
				mesh.vertexEnd();
			while (vertex != vertexEnd)
			{
				Half_ConstIterator half = vertex->half();
				if (!half.empty() && half->origin() != vertex)
				{
					// If the half exists, the origin of the half 
					// must be the vertex itself.
					return false;
				}

				++vertex;
			}
		}

		// Check edge invariants
		{
			Edge_ConstIterator edge =
				mesh.edgeBegin();
			Edge_ConstIterator edgeEnd =
				mesh.edgeEnd();
			while (edge != edgeEnd)
			{
				Half_ConstIterator half = edge->half();
				if (half.empty())
				{
					// The half of an edge must exist.
					return false;
				}

				if (half->edge() != edge)
				{
					// The edge of a half of an edge
					// must be the edge itself. 
					return false;
				}

				++edge;
			}
		}

		// Check polygon invariants
		{
			Polygon_ConstIterator polygon =
				mesh.polygonBegin();
			Polygon_ConstIterator polygonEnd =
				mesh.polygonEnd();

			while (polygon != polygonEnd)
			{
				Half_ConstIterator half = polygon->half();
				if (half.empty())
				{
					// The half of a polygon must exist.
					return false;
				}

				if (half->left() != polygon)
				{
					// The left of a half must be the
					// polygon itself.
					return false;
				}

				++polygon;
			}
		}

		// Check half-edge invariants
		{
			Half_ConstIterator half =
				mesh.halfBegin();
			Half_ConstIterator halfEnd =
				mesh.halfEnd();

			while (half != halfEnd)
			{
				Half_ConstIterator previous = half->previous();
				Half_ConstIterator next = half->next();
				Half_ConstIterator pair = half->pair();

				if (previous.empty() ||
					next.empty() ||
					pair.empty() ||
					half->origin().empty() ||
					half->edge().empty())
				{
					// All of the fields of a half-edge
					// must exist.
					return false;
				}

				if (previous->next() != half ||
					next->previous() != half ||
					next->left() != half->left() ||
					previous->left() != half->left() ||
					pair->pair() != half ||
					half->origin() != pair->destination() ||
					pair->origin() != half->destination() ||
					half->edge() != pair->edge())
				{
					// The fields must be linked to together
					// in a bidirectional manner.
					return false;
				}

				++half;
			}
		}

		return true;
	}

}

#endif
