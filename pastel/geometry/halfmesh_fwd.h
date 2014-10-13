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

		struct Vertex_Tag;
		using VertexData_Class = Class<VertexData, Vertex_Tag>;

		struct Half_Tag;
		using HalfData_Class = Class<HalfData, Half_Tag>;

		struct Edge_Tag;
		using EdgeData_Class = Class<EdgeData, Edge_Tag>;

		struct Polygon_Tag;
		using PolygonData_Class = Class<PolygonData, Polygon_Tag>;

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
			typename std::conditional<
				MultipleEdges,
				Edge_Iterator,
				std::pair<Edge_Iterator, bool>
			>::type;
	};

}

namespace Pastel
{

	template <typename Settings>
	class Empty_HalfMesh_Customization
	{
	protected:
		using Fwd = HalfMesh_Fwd<Settings>;
		PASTEL_FWD(Vertex_Iterator);
		PASTEL_FWD(Half_Iterator);
		PASTEL_FWD(Edge_Iterator);
		PASTEL_FWD(Polygon_Iterator);

		Empty_HalfMesh_Customization() {}
		void swap(Empty_HalfMesh_Customization& that) {}

		void onClear() {}
		void onInsertVertex(const Vertex_Iterator& vertex) {}
		void onRemoveVertex(const Vertex_Iterator& vertex) {}
		void onInsertEdge(const Edge_Iterator& edge) {}
		void onRemoveEdge(const Edge_Iterator& edge) {}
		void onInsertPolygon(const Polygon_Iterator& polygon) {}
		void onRemovePolygon(const Polygon_Iterator& polygon) {}

	private:
		Empty_HalfMesh_Customization(const Empty_HalfMesh_Customization& that) = delete;
		Empty_HalfMesh_Customization(Empty_HalfMesh_Customization&& that) = delete;
		Empty_HalfMesh_Customization& operator=(Empty_HalfMesh_Customization) = delete;
	};

}

#endif
