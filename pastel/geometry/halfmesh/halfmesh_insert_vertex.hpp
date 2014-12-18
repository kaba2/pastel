// Description: Inserts a vertex.

#ifndef PASTELGEOMETRY_HALFMESH_INSERT_VERTEX_HPP
#define PASTELGEOMETRY_HALFMESH_INSERT_VERTEX_HPP

#include "pastel/geometry/halfmesh/halfmesh.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	template <typename... Type>
	auto HalfMesh<Settings, Customization>::insertVertex(
		Type&&... data)
	-> Vertex_Iterator
	{
		Vertex_Iterator vertex = 
			vertexSet_.insertBack(std::forward<Type>(data)...);
		this->onInsertVertex(vertex);
		return vertex;
	}

}

#endif
