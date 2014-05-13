#ifndef PASTELGEOMETRY_HALFMESH_INSERT_VERTEX_HPP
#define PASTELGEOMETRY_HALFMESH_INSERT_VERTEX_HPP

#include "pastel/geometry/halfmesh.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	auto HalfMesh<Settings, Customization>::insertVertex(
		const VertexData_Class& data)
	-> Vertex_Iterator
	{
		return vertexSet_.insertBack(data);
	}

}

#endif
