// Description: Half-edge structure

#ifndef PASTELGEOMETRY_HALFMESH_H
#define PASTELGEOMETRY_HALFMESH_H

#include "pastel/geometry/halfmesh_concepts.h"
#include "pastel/geometry/halfmesh_fwd.h"
#include "pastel/geometry/halfmesh_vertex.h"
#include "pastel/geometry/halfmesh_half.h"
#include "pastel/geometry/halfmesh_edge.h"
#include "pastel/geometry/halfmesh_polygon.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/class.h"
#include "pastel/sys/list.h"

#include <boost/operators.hpp>

#include <vector>
#include <unordered_set>

namespace Pastel
{

	//! Half-edge structure
	template <
		typename Settings, 
		template <typename> class Customization_>
	class HalfMesh
		: public Customization_<Settings>
	{
	public:
		using Customization = Customization_<Settings>;

		using Fwd = HalfMesh_Fwd<Settings>;

		PASTEL_FWD(Vertex);
		PASTEL_FWD(Half);
		PASTEL_FWD(Edge);
		PASTEL_FWD(Polygon);
		
		PASTEL_FWD(VertexData_Class);
		PASTEL_FWD(HalfData_Class);
		PASTEL_FWD(EdgeData_Class);
		PASTEL_FWD(PolygonData_Class);
		static PASTEL_CONSTEXPR bool MultipleEdges = Fwd::MultipleEdges;
		static PASTEL_CONSTEXPR bool Loops = Fwd::Loops;

		PASTEL_FWD(VertexSet);
		PASTEL_FWD(Vertex_Iterator);
		PASTEL_FWD(Vertex_ConstIterator);

		PASTEL_FWD(HalfSet);
		PASTEL_FWD(Half_Iterator);
		PASTEL_FWD(Half_ConstIterator);

		PASTEL_FWD(EdgeSet);
		PASTEL_FWD(Edge_Iterator);
		PASTEL_FWD(Edge_ConstIterator);

		PASTEL_FWD(PolygonSet);
		PASTEL_FWD(Polygon_Iterator);
		PASTEL_FWD(Polygon_ConstIterator);

		PASTEL_FWD(InsertEdge_Return);

		template <typename Range, typename To>
		struct IsConvertible
		{
			static PASTEL_CONSTEXPR bool value =
				std::is_convertible<decltype(*std::begin(std::declval<Range>())), To>::value;
		};

		//! Constructs an empty mesh.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		HalfMesh()
		: vertexSet_()
		, halfSet_()
		, edgeSet_()
		, polygonSet_()
		{
		}

		//! Move-constructs from another mesh.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		HalfMesh(HalfMesh&& that)
		: HalfMesh()
		{
			swap(that);
		}

		//! Copy-constructs from another mesh.
		/*!
		Time complexity: O(vertices() + edges() + polygons())
		Exception safety: strong
		*/
		HalfMesh(const HalfMesh& that);

		//! Destructs the mesh.
		/*!
		Exception safety:
		nothrow
		*/
		~HalfMesh()
		{
			clear();
		}

		//! Assigns from another mesh.
		/*!
		Time complexity:
		O(1), if moved,
		O(vertices() + edges() + polygons()), otherwise.

		Exception safety: strong
		*/
		HalfMesh& operator=(HalfMesh that)
		{
			swap(that);
			return *this;
		}

		//! Swaps two half-edge meshes.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(HalfMesh& that)
		{
			Customization::swap(that);
			vertexSet_.swap(that.vertexSet_);
			halfSet_.swap(that.halfSet_);
			edgeSet_.swap(that.edgeSet_);
			polygonSet_.swap(that.polygonSet_);
		}

		//! Remove all data of the mesh.
		/*!
		Time complexity: O(vertices() + edges() + polygons())
		Exception safety: nothrow
		*/
		void clear()
		{
			this->onClear();

			vertexSet_.clear();
			halfSet_.clear();
			edgeSet_.clear();
			polygonSet_.clear();
		}

		// Iterators

		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Vertex_, vertexBegin, vertexSet_.begin());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Vertex_, vertexEnd, vertexSet_.end());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Vertex_, vertexLast, std::prev(vertexEnd()));
		
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Half_, halfBegin, halfSet_.begin());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Half_, halfEnd, halfSet_.end());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Half_, halfLast, std::prev(halfEnd()));
		
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Edge_, edgeBegin, edgeSet_.begin());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Edge_, edgeEnd, edgeSet_.end());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Edge_, edgeLast, std::prev(edgeEnd()));
		
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Polygon_, polygonBegin, polygonSet_.begin());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Polygon_, polygonEnd, polygonSet_.end());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Polygon_, polygonLast, std::prev(polygonEnd()));

		//! Searches for a half-edge from 'from' to 'to'.
		/*!
		Time complexity:
		O(n)
		where
		n is the number of edges around 'from'.

		Exception safety:
		nothrow
		*/
		Half_ConstIterator findHalf(
			const Vertex_ConstIterator& from,
			const Vertex_ConstIterator& to) const;

		Half_Iterator findHalf(
			const Vertex_ConstIterator& from,
			const Vertex_ConstIterator& to)
		{
			return cast(addConst(*this).findHalf(from, to));
		}

		//! Makes 'in' and 'out' oriented neighbours.
		/*!
		in, out:
		The half-edges to make adjacent.

		Returns:
		Whether the half-edges could be made adjacent
		(sometimes this is not possible, meaning it
		would lead to a non-representable non-manifold
		configuration).

		Preconditions:
		!in.empty()
		!out.empty()
		in.destination() == out.origin()
		in.left().empty()
		out.left().empty()

		Postconditions:
		If returns true, in.next() == out
		and out.previous() == in.
		Otherwise, nothing is changed.

		Time complexity:
		O(edges around out.origin())

		Exception safety:
		nothrow
		*/
		bool makeAdjacent(
			const Half_ConstIterator& in, 
			const Half_ConstIterator& out);

		//! Inserts an isolated vertex.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		template <typename... Type>
		Vertex_Iterator insertVertex(
			Type&&... data);

		//! Removes a vertex and all its neighbouring edges.
		/*!
		Time complexity:
		Let E = {e[i]} be the set of edges connected to the
		vertex to be removed. Let P = {p[i] = e[i].left()}.
		Then the complexity is
		sum_i O(edges in p[i]) + |E| O(log(edges())) on average.

		Exception safety:
		nothrow
		*/
		Vertex_Iterator removeVertex(
			const Vertex_ConstIterator& vertex);

		//! Casts away the constness of an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Vertex_Iterator cast(
			const Vertex_ConstIterator& that)
		{
			return vertexSet_.cast(that);
		}

		//! Returns the number of vertices.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		integer vertices() const
		{
			return vertexSet_.size();
		}

		//! Casts away the constness of an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Half_Iterator cast(
			const Half_ConstIterator& that)
		{
			return halfSet_.cast(that);
		}

		//! Inserts an edge between two vertices.
		/*!
		from, to:
		The vertices between which to insert the new edge.

		data:
		The edge-data.

		returns:
		If multiple edges are allowed between two vertices,
		then an edge-iterator-boolean pair, where the boolean
		tells whether an edge was created. 
		If multiple edges are not allowed between two vertices,
		then an edge-iterator to the created edge, or to a
		possibly existing edge.
		The returned edge-iterator will be empty in the cases
		where the edge can not be represented by the half-edge
		structure.

		Preconditions:
		from.isNormal()
		to.isNormal()

		Time complexity:
		O(edges around 'from') + 
		O(edges around 'to')

		Exception safety:
		strong
		*/
		template <typename... Type>
		InsertEdge_Return insertEdge(
			const Vertex_ConstIterator& from,
			const Vertex_ConstIterator& to,
			Type&&... data);

		//! Removes an edge and all its neighbouring polygons.
		/*!
		Time complexity:
		O(edges in the left polygon) +
		O(edges in the right polygon)

		Exception safety:
		nothrow
		*/
		Edge_Iterator removeEdge(
			const Edge_ConstIterator& edge);

		//! Casts away the constness of an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Edge_Iterator cast(
			const Edge_ConstIterator& that)
		{
			return edgeSet_.cast(that);
		}

		//! Returns the number of edges.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer edges() const
		{
			return edgeSet_.size();
		}

		//! Inserts a polygon to the given boundary loop.
		/*!
		Returns:
		The added polygon if successful,
		otherwise an empty polygon.

		Preconditions:
		For all i: !halfLoop[i].empty()

		A polygon can be added if the following conditions hold:
		For all i: halfLoop[i].destination() == halfLoop[(i + 1) % halfLoop.size()].origin()
		For all i: halfLoop[i].left().empty()
		For all i: halfLoop[i].origin().free()

		Time complexity:
		sum_i O(edges around halfLoop[i].origin()) on average

		Exception safety:
		strong
		*/
		template <
			typename Half_Range,
			typename... Type>
		auto insertPolygon(
			const Half_Range& halfSet,
			Type&&... data)
		-> EnableIf<IsConvertible<Half_Range, Half_ConstIterator>, Polygon_Iterator>;

		//! Inserts a polygon to the given vertex loop.
		/*!
		Time complexity:
		sum_i O(edges around vertexLoop[i]) +
		O(vertexLoop.size()) on average

		Exception safety:
		strong
		*/
		template <
			typename Vertex_Range,
			typename... Type>
		auto insertPolygon(
			const Vertex_Range& vertexSet,
			Type&&... data)
		-> EnableIf<IsConvertible<Vertex_Range, Vertex_ConstIterator>, Polygon_Iterator>;
		
		//! Removes a polygon.
		/*!
		Time complexity:
		O(edges in the polygon) on average

		Exception safety:
		nothrow
		*/
		Polygon_Iterator removePolygon(
			const Polygon_ConstIterator& polygon);

		//! Casts away the constness of an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Polygon_Iterator cast(
			const Polygon_ConstIterator& that)
		{
			return polygonSet_.cast(that);
		}

		//! Returns the number of polygons.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer polygons() const
		{
			return polygonSet_.size();
		}

		//! Merges polygons across the half-edge.
		/*!
		Time complexity: O(n)
		where
		n is the number of vertices in half.righ().

		Exception safety: nothrow

		The half, half.pair(), and half.edge() are removed.
		The half.left() is extended, and half.right() is 
		removed (unless half.left() == half.right()).

		returns:
		half.left()
		*/
		auto merge(const Half_ConstIterator& half)
			-> Polygon_Iterator;

	private:
		//! Detaches a half-edge from its origin.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void detachHalf(
			const Half_ConstIterator& half);

		Half_ConstIterator findFreeIncident(
			const Vertex_ConstIterator& vertex) const;
		
		Half_Iterator findFreeIncident(
			const Vertex_ConstIterator& vertex)
		{
			return cast(addConst(*this).findFreeIncident(vertex));
		}

		Half_ConstIterator findFreeIncident(
			const Vertex_ConstIterator& vertex,
			const Half_ConstIterator& startingFrom,
			const Half_ConstIterator& andBefore) const;

		Half_Iterator findFreeIncident(
			const Vertex_ConstIterator& vertex,
			const Half_ConstIterator& startingFrom,
			const Half_ConstIterator& andBefore)
		{
			return cast(addConst(*this).findFreeIncident(
				vertex, startingFrom, andBefore));
		}

		struct PairTag {};
		struct SingleTag {};

		InsertEdge_Return insertEdgeReturn(
			const Edge_Iterator& edge,
			bool created) const
		{
			using Tag = typename std::conditional<
				MultipleEdges, SingleTag, PairTag>::type;
			return insertEdgeReturn(edge, created, Tag());
		}

		std::pair<Edge_Iterator, bool> insertEdgeReturn(
			const Edge_Iterator& edge, 
			bool created,
			PairTag) const
		{
			return std::make_pair(edge, created);
		}

		Edge_Iterator insertEdgeReturn(
			const Edge_Iterator& edge, 
			bool created,
			SingleTag) const
		{
			return edge;
		}

		VertexSet vertexSet_;
		HalfSet halfSet_;
		EdgeSet edgeSet_;
		PolygonSet polygonSet_;
	};

}

namespace Pastel
{

	template <
		typename VertexData_ = void,
		typename HalfData_ = void,
		typename EdgeData_ = void,
		typename PolygonData_ = void,
		bool MultipleEdges_ = true,
		bool Loops_ = true>
	class HalfMesh_Settings
	{
	public:
		using VertexData = VertexData_;
		using HalfData = HalfData_;
		using EdgeData = EdgeData_;
		using PolygonData = PolygonData_;
		static PASTEL_CONSTEXPR bool MultipleEdges = MultipleEdges_;
		static PASTEL_CONSTEXPR bool Loops = Loops_;
	};

	template <
		typename VertexData_ = void,
		typename HalfData_ = void,
		typename EdgeData_ = void,
		typename PolygonData_ = void,
		bool MultipleEdges_ = true,
		bool Loops_ = true,
		template <typename> class Customization_ = Empty_HalfMesh_Customization>
	using HalfEdge = HalfMesh<HalfMesh_Settings<
		VertexData_, HalfData_, EdgeData_, PolygonData_,
		MultipleEdges_, Loops_>, Customization_>;

}

#include "pastel/geometry/halfmesh.hpp"
#include "pastel/geometry/halfmesh_detach_half.hpp"
#include "pastel/geometry/halfmesh_find.hpp"
#include "pastel/geometry/halfmesh_merge_half.hpp"
#include "pastel/geometry/halfmesh_remove_edge.hpp"
#include "pastel/geometry/halfmesh_remove_polygon.hpp"
#include "pastel/geometry/halfmesh_remove_vertex.hpp"
#include "pastel/geometry/halfmesh_insert_edge.hpp"
#include "pastel/geometry/halfmesh_insert_polygon.hpp"
#include "pastel/geometry/halfmesh_insert_vertex.hpp"
#include "pastel/geometry/halfmesh_invariants.h"
#include "pastel/geometry/halfmesh_stream.h"

#endif
