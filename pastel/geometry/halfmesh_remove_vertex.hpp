// Description: Removes a vertex.

#ifndef PASTELGEOMETRY_HALFMESH_REMOVE_VERTEX_HPP
#define PASTELGEOMETRY_HALFMESH_REMOVE_VERTEX_HPP

#include "pastel/geometry/halfmesh.h"

namespace Pastel
{

	template <
		typename Settings, 
		template <typename> class Customization>
	auto HalfMesh<Settings, Customization>::removeVertex(
		const Vertex_ConstIterator& vertex)
	-> Vertex_Iterator
	{
		if (vertex.isSentinel() || vertex.empty())
		{
			return cast(vertex);
		}

		this->onRemoveVertex(cast(vertex));

		if (!vertex->isolated())
		{
			// Remove every edge that is connected
			// to this vertex.

			Half_Iterator current;
			Half_Iterator next = vertex->half();

			do
			{
				current = next;
				next = next->rotateNext();
				if (next->edge() == current->edge())
				{
					next = next->rotateNext();
				}
				removeEdge(current->edge());
			}
			while (current != next);
		}

		return vertexSet_.erase(vertex);
	}

}

#endif
