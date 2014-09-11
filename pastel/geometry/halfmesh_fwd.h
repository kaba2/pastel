// Description: Types for the half-edge structure

#ifndef PASTELGEOMETRY_HALFMESH_FWD_H
#define PASTELGEOMETRY_HALFMESH_FWD_H

#include "pastel/sys/class.h"
#include "pastel/sys/range.h"
#include "pastel/sys/list.h"

namespace Pastel
{

	namespace HalfMesh_
	{

		template <typename>
		class Vertex;

		template <typename>
		class Half;

		template <typename>
		class Edge;

		template <typename>
		class Polygon;

	}

	template <typename, template <typename> class>
	class HalfMesh;

	template <typename Settings_>
	class HalfMesh_Fwd
	{
	public:
		using Settings = Settings_;
		
		using Fwd = Settings;

		PASTEL_FWD(VertexData);
		PASTEL_FWD(HalfData);
		PASTEL_FWD(EdgeData);
		PASTEL_FWD(PolygonData);
		static PASTEL_CONSTEXPR bool MultipleEdges = Settings::MultipleEdges;
		static PASTEL_CONSTEXPR bool Loops = Settings::Loops;

		using VertexData_Class = As_Class<VertexData>;
		using HalfData_Class = As_Class<HalfData>;
		using EdgeData_Class = As_Class<EdgeData>;
		using PolygonData_Class = As_Class<PolygonData>;

		using Vertex = HalfMesh_::Vertex<Settings>;
		using Half = HalfMesh_::Half<Settings>;
		using Edge = HalfMesh_::Edge<Settings>;
		using Polygon = HalfMesh_::Polygon<Settings>;

		using VertexSet = List_Set<Vertex>;
		using Vertex_Iterator = typename VertexSet::Iterator;
		using Vertex_ConstIterator = typename VertexSet::ConstIterator;

		using HalfSet = List_Set<Half>;
		using Half_Iterator = typename HalfSet::Iterator;
		using Half_ConstIterator = typename HalfSet::ConstIterator;

		using EdgeSet = List_Set<Edge>;
		using Edge_Iterator = typename EdgeSet::Iterator;
		using Edge_ConstIterator = typename EdgeSet::ConstIterator;

		using PolygonSet = List_Set<Polygon>;
		using Polygon_Iterator = typename PolygonSet::Iterator;
		using Polygon_ConstIterator = typename PolygonSet::ConstIterator;

		using InsertEdge_Return =
			typename std::conditional<MultipleEdges,
			Edge_Iterator,
			std::pair<Edge_Iterator, bool>>::type;
	};

}

#endif
