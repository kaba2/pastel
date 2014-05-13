#ifndef PASTELGEOMETRY_HALFMESH_STREAM_HPP
#define PASTELGEOMETRY_HALFMESH_STREAM_HPP

#include "pastel/geometry/halfmesh_stream.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	std::ostream& operator<<(
		std::ostream& stream,
		const HalfMesh<Settings, Customization>& mesh)
	{
		using Fwd = HalfMesh<Settings, Customization>;
		PASTEL_FWD(Vertex_ConstIterator);
		PASTEL_FWD(Half_ConstIterator);

		std::unordered_map<Vertex_ConstIterator,
			integer> vertexMap;

		{
			integer vertexId = 0;
			Vertex_ConstIterator vertex = mesh.vertexBegin();
			Vertex_ConstIterator end = mesh.vertexEnd();
			while (vertex != end)
			{
				vertexMap.emplace(vertex, vertexId);

				++vertexId;
				++vertex;
			}
		}

		Vertex_ConstIterator vertex = mesh.vertexBegin();
		Vertex_ConstIterator end = mesh.vertexEnd();
		while (vertex != end)
		{
			stream << "Vertex " << vertexMap[vertex] << std::endl;

			Half_ConstIterator halfBegin = vertex.half();
			if (!halfBegin.empty())
			{
				Half_ConstIterator half = halfBegin;
				do
				{
					stream << vertexMap[half.destination()] << ", ";
					half = half.rotateNext();
				}
				while (half != halfBegin);
				stream << std::endl;
			}

			++vertex;
		}

		return stream;
	}

}

#endif
