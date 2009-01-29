#ifndef PASTELGEOMETRY_DELAUNAYTESSELATION_TOOLS_HPP
#define PASTELGEOMETRY_DELAUNAYTESSELATION_TOOLS_HPP

#include "pastel/geometry/delaunaytesselation_tools.h"

#include "pastel/sys/unorderedmap.h"
#include "pastel/sys/tuple_tools.h"

namespace Pastel
{

	template <typename Real, typename DelaunayDataPolicy, typename MeshDataPolicy>
	void convert(
		const DelaunayTesselation<Real, DelaunayDataPolicy>& delaunay,
		FlatSimplexMesh<2, Real, MeshDataPolicy>& mesh)
	{
		typedef FlatSimplexMesh<2, Real, MeshDataPolicy> Mesh;
		typedef typename Mesh::VertexData MeshVertexData;
		typedef typename Mesh::Vertex MeshVertex;
		typedef typename Mesh::Simplex MeshSimplex;
		typedef typename Mesh::ConstVertexIterator MeshVertexIterator;

		typedef DelaunayTesselation<Real, DelaunayDataPolicy> Delaunay;
		typedef typename Delaunay::VertexHash DelaunayVertexHash;
		typedef typename Delaunay::Vertex DelaunayVertex;
		typedef typename Delaunay::Simplex DelaunaySimplex;
		typedef typename Delaunay::ConstVertexIterator DelaunayVertexIterator;
		typedef typename Delaunay::ConstSimplexIterator DelaunaySimplexIterator;

		Mesh meshCopy;

		UnorderedMap<DelaunayVertex, MeshVertex,
			DelaunayVertexHash> vertexMap;

		// Copy vertices and build the vertex map.
		{
			DelaunayVertexIterator iter = delaunay.vertexBegin();
			const DelaunayVertexIterator iterEnd = delaunay.vertexEnd();
			while(iter != iterEnd)
			{
				const MeshVertex vertex = meshCopy.addVertex();
				vertex->position() = iter->position();
				vertex->data() = iter->data();

				vertexMap.insert(std::make_pair(*iter, vertex));

				++iter;
			}
		}

		// Copy simplices.
		{
			DelaunaySimplexIterator iter = delaunay.simplexBegin();
			const DelaunaySimplexIterator iterEnd = delaunay.simplexEnd();
			while(iter != iterEnd)
			{
				const DelaunaySimplex simplex = *iter;

				if (!anyEqual(simplex.pair(), DelaunaySimplex()))
				{
					meshCopy.addSimplex(
						Tuple<3, MeshVertex>(
						vertexMap[simplex.vertex(0)],
						vertexMap[simplex.vertex(1)],
						vertexMap[simplex.vertex(2)]));
				}

				++iter;
			}
		}

		mesh.swap(meshCopy);
	}

}

#endif
