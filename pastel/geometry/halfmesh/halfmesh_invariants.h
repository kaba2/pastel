// Description: Half-edge structure invariants

#ifndef PASTELGEOMETRY_HALFMESH_INVARIANTS_H
#define PASTELGEOMETRY_HALFMESH_INVARIANTS_H

#include "pastel/geometry/halfmesh/halfmesh.h"

namespace Pastel
{
	
	//! Returns whether the invariants hold for a half-edge structure.	
	/*!
	Time complexity: O(mesh.vertices() + mesh.edges() + mesh.polygons())
	Exception safety: nothrow
	
	This function is useful only for testing and debugging. For
	a correct implementation this function always returns true.
	*/	
	template <
		typename Settings,
		template <typename> class Customization>
	bool testInvariants(const HalfMesh<Settings, Customization>& mesh)
	{
		using Mesh = HalfMesh<Settings, Customization>;
		
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
				if (vertex.empty())
				{
					// A vertex must exist.
					return false;
				}

				if (!vertex.isNormal())
				{
					// A vertex must be normal.
					return false;
				}

				if (vertex->isolated() != vertex->half().empty())
				{
					// An isolated vertex must not be connected 
					// to any half-edge.
					return false;
				}

				if (vertex->isolated() && !vertex->free())
				{
					// An isolated vertex must be free.
					return false;
				}

				Half_ConstIterator half = vertex->half();
				if (!half.empty() && half->origin() != vertex)
				{
					// If the half-edge exists, the origin of the half-edge
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
				if (edge.empty())
				{
					// An edge always exist.
					return false;
				}

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
				if (polygon.empty())
				{
					// An polygon always exist.
					return false;
				}

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
				if (half.empty())
				{
					// A half-edge always exist.
					return false;
				}

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
