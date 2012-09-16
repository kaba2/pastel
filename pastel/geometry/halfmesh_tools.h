// Description: Basic algorithms for HalfMesh

#ifndef PASTELGEOMETRY_HALFMESH_TOOLS_H
#define PASTELGEOMETRY_HALFMESH_TOOLS_H

#include "pastel/geometry/halfmesh.h"

#include <iostream>
#include <unordered_map>

namespace Pastel
{

	template <typename UserDataPolicy>
	void printVertices(const HalfMesh<UserDataPolicy>& mesh,
		std::ostream& stream)
	{
		typedef HalfMesh<UserDataPolicy> Mesh;

		std::unordered_map<typename Mesh::Vertex,
			integer, typename Mesh::VertexHash> vertexMap;

		integer vertexId = 0;
		typename Mesh::ConstVertexIterator iter(mesh.vertexBegin());
		typename Mesh::ConstVertexIterator iterEnd(mesh.vertexEnd());
		while (iter != iterEnd)
		{
			vertexMap.insert(std::make_pair(*iter, vertexId));

			++vertexId;
			++iter;
		}

		iter = mesh.vertexBegin();
		iterEnd = mesh.vertexEnd();
		while (iter != iterEnd)
		{
			typename Mesh::Vertex vertex(*iter);

			stream << "Vertex " << vertexMap[vertex] << std::endl;

			typename Mesh::Half halfBegin(vertex.half());
			if (!halfBegin.empty())
			{
				typename Mesh::Half half(halfBegin);
				do
				{
					stream << vertexMap[half.destination()] << ", ";

					half = half.rotateNext();
				}
				while (half != halfBegin);
				stream << std::endl;
			}

			++iter;
		}
	}

	template <typename UserDataPolicy>
	bool checkInvariants(const
		HalfMesh<UserDataPolicy>& mesh)
	{
		typedef HalfMesh<UserDataPolicy> Mesh;

		typedef typename Mesh::Vertex Vertex;
		typedef typename Mesh::Half Half;
		typedef typename Mesh::Edge Edge;
		typedef typename Mesh::Polygon Polygon;

		typedef typename Mesh::ConstVertexIterator ConstVertexIterator;
		typedef typename Mesh::ConstHalfIterator ConstHalfIterator;
		typedef typename Mesh::ConstEdgeIterator ConstEdgeIterator;
		typedef typename Mesh::ConstPolygonIterator ConstPolygonIterator;

		// Check vertex invariants

		ConstVertexIterator vertexIter(
			mesh.vertexBegin());
		ConstVertexIterator vertexEnd(
			mesh.vertexEnd());

		while (vertexIter != vertexEnd)
		{
			Vertex vertex(*vertexIter);
			if (vertex.empty())
			{
				return false;
			}
			
			Half half(vertex.half());
			if (!half.empty() && half.origin() != vertex)
			{
				return false;
			}

			++vertexIter;
		}

		// Check edge invariants

		ConstEdgeIterator edgeIter(
			mesh.edgeBegin());
		ConstEdgeIterator edgeEnd(
			mesh.edgeEnd());

		while (edgeIter != edgeEnd)
		{
			Edge edge(*edgeIter);
			if (edge.empty())
			{
				return false;
			}

			Half half(edge.half());
			if (half.empty() || 
				half.edge() != edge)
			{
				return false;
			}

			++edgeIter;
		}

		// Check polygon invariants

		ConstPolygonIterator polygonIter(
			mesh.polygonBegin());
		ConstPolygonIterator polygonEnd(
			mesh.polygonEnd());

		while (polygonIter != polygonEnd)
		{
			Polygon polygon(*polygonIter);
			if (polygon.empty())
			{
				return false;
			}
			
			Half half(polygon.half());
			if (half.empty() || 
				half.left() != polygon)
			{
				return false;
			}

			++polygonIter;
		}

		// Check half-edge invariants

		ConstHalfIterator halfIter(
			mesh.halfBegin());
		ConstHalfIterator halfEnd(
			mesh.halfEnd());

		while (halfIter != halfEnd)
		{
			const Half half(*halfIter);
			if (half.empty())
			{
				return false;
			}

			const Half previous(half.previous());
			const Half next(half.next());
			const Half pair(half.pair());

			if (previous.empty() ||
				next.empty() ||
				pair.empty() ||
				half.origin().empty() ||
				half.edge().empty() ||
				previous.next() != half ||
				next.previous() != half ||
				next.left() != half.left() ||
				previous.left() != half.left() ||
				pair.pair() != half ||
				half.origin() != pair.destination() ||
				pair.origin() != half.destination() ||
				half.edge() != pair.edge())
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
			halfCurrent = halfCurrent.next();
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
			halfCurrent = halfCurrent.previous();
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

			++halfIter;
		}

		return true;
	}

}

#endif
