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
				if (vertex.empty())
				{
					return false;
				}

				Half_ConstIterator half = vertex->half();
				if (!half.empty() && half->origin() != vertex)
				{
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
					return false;
				}

				Half_ConstIterator half = edge->half();
				if (half.empty() ||
					half->edge() != edge)
				{
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
					return false;
				}

				Half_ConstIterator half = polygon->half();
				if (half.empty() ||
					half->left() != polygon)
				{
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
					return false;
				}

				Half_ConstIterator previous = half->previous();
				Half_ConstIterator next = half->next();
				Half_ConstIterator pair = half->pair();

				if (previous.empty() ||
					next.empty() ||
					pair.empty() ||
					half->origin().empty() ||
					half->edge().empty() ||
					previous->next() != half ||
					next->previous() != half ||
					next->left() != half->left() ||
					previous->left() != half->left() ||
					pair->pair() != half ||
					half->origin() != pair->destination() ||
					pair->origin() != half->destination() ||
					half->edge() != pair->edge())
				{
					return false;
				}

				/*
				const integer MAX_PERIMETER = 100000;

				{
				integer perimeterCount = 0;
				Half halfCurrent(half);
				Half halfBegin(half);
				do
				{
				++perimeterCount;
				if (REPORT(perimeterCount == MAX_PERIMETER))
				{
				return false;
				}
				halfCurrent = halfCurrent->next();
				}
				while (halfCurrent != halfBegin);
				}

				{
				integer perimeterCount = 0;
				Half halfCurrent(half);
				Half halfBegin(half);
				do
				{
				++perimeterCount;
				if (REPORT(perimeterCount == MAX_PERIMETER))
				{
				return false;
				}
				halfCurrent = halfCurrent->previous();
				}
				while (halfCurrent != halfBegin);
				}

				{
				integer perimeterCount = 0;
				Half halfCurrent(half);
				Half halfBegin(half);
				do
				{
				++perimeterCount;
				if (REPORT(perimeterCount == MAX_PERIMETER))
				{
				return false;
				}
				halfCurrent = halfCurrent.rotateNext();
				}
				while (halfCurrent != halfBegin);
				}

				{
				integer perimeterCount = 0;
				Half halfCurrent(half);
				Half halfBegin(half);
				do
				{
				++perimeterCount;
				if (REPORT(perimeterCount == MAX_PERIMETER))
				{
				return false;
				}
				halfCurrent = halfCurrent.rotatePrevious();
				}
				while (halfCurrent != halfBegin);
				}
				*/

				++half;
			}
		}

		return true;
	}

}

#endif
