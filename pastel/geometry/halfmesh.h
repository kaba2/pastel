/*!
\file
\brief A class for storing a half-edge structure polygon mesh.
*/

#ifndef PASTELGEOMETRY_HALFMESH_H
#define PASTELGEOMETRY_HALFMESH_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/poolallocator.h"
#include "pastel/sys/possiblyemptymember.h"
#include "pastel/sys/unorderedset.h"

#include <boost/operators.hpp>

#include <vector>

namespace Pastel
{

	//! Half-edge structure polygon mesh.
	/*!
	class DataPolicy
	{
	public:
		// You can expand the interface
		// of HalfMesh by defining your functions
		// here.
		void yourOwnInterfaceExtension();

	protected:
		// The following must be implemented
		// for the HalfMesh to work.
		// Being implementation details, they should be protected.

		// Define a user data type as an EmptyClass
		// if no user data is desired.

		typedef undefined_type VertexData;
		typedef undefined_type HalfData;
		typedef undefined_type EdgeData;
		typedef undefined_type PolygonData;

		// If a user data type is defined as an
		// EmptyClass, a zero pointer is passed
		// to the following functions. The
		// implementation must handle these cases
		// correctly.

		void constructVertex(VertexData* vertex);
		void constructHalf(HalfData* half);
		void constructEdge(EdgeData* edge);
		void constructPolygon(PolygonData* polygon);

		void destructVertex(VertexData* vertex);
		void destructHalf(HalfData* half);
		void destructEdge(EdgeData* edge);
		void destructPolygon(PolygonData* polygon);
	};

	The 'DataPolicy' is used to control the construction
	and destruction of user data. Because inheritance is
	used, you can expand the interface of HalfMesh by
	simply defining your own functions in the policy.
	If no user data is needed, memory can be saved by defining
	the desired user data type as an EmptyClass.
	In this case the construction and
	destruction functions are passed a zero pointer.
	The implementation of the policy should make sure that
	this is handled correctly.

	We have chosen not to support parallel edges
	(multiple edges with same end-vertices) or
	edge loops (edges that start and end at the same vertex).
	Attempting to add these kind of edges results
	in 'addEdge()' returning an empty edge.

	Note that the time complexities given are specific
	to my implementation. Specifically, the "on average"
	suffix to many of the functions stem from the fact
	that a hash table is used to store the elements and
	many of its operations have complexity "constant on average".
	Also, when removing elements, the allocator used
	takes a logarithmic time to deallocate an element.
	*/

	template <typename DataPolicy>
	class HalfMesh
		: public DataPolicy
	{
	private:
		// A compiler bug prevents us from doing this.
		/*
		using typename DataPolicy::VertexData;
		using typename DataPolicy::HalfData;
		using typename DataPolicy::EdgeData;
		using typename DataPolicy::PolygonData;
		*/

		// So we get around it by doing this.
		typedef typename DataPolicy::VertexData VertexData;
		typedef typename DataPolicy::HalfData HalfData;
		typedef typename DataPolicy::EdgeData EdgeData;
		typedef typename DataPolicy::PolygonData PolygonData;

		class VertexBody;
		class HalfBody;
		class EdgeBody;
		class PolygonBody;

	public:
		class Vertex;
		class Half;
		class Edge;
		class Polygon;

		class VertexHash;
		class HalfHash;
		class EdgeHash;
		class PolygonHash;

	private:
		typedef UnorderedSet<Vertex, VertexHash> VertexSet;
		typedef typename VertexSet::iterator VertexIterator;

		typedef UnorderedSet<Half, HalfHash> HalfSet;
		typedef typename HalfSet::iterator HalfIterator;

		typedef UnorderedSet<Edge, EdgeHash> EdgeSet;
		typedef typename EdgeSet::iterator EdgeIterator;

		typedef UnorderedSet<Polygon, PolygonHash> PolygonSet;
		typedef typename PolygonSet::iterator PolygonIterator;

	public:
		typedef typename VertexSet::const_iterator
			ConstVertexIterator;
		typedef typename HalfSet::const_iterator
			ConstHalfIterator;
		typedef typename EdgeSet::const_iterator
			ConstEdgeIterator;
		typedef typename PolygonSet::const_iterator
			ConstPolygonIterator;

	public:
		//! Constructs an empty mesh.
		/*!
		Time complexity:
		constant

		Exception safety:
		strong
		*/
		HalfMesh();

		//! Constructs with a copy from another mesh.
		/*!
		Exception safety:
		strong
		*/
		HalfMesh(const HalfMesh& that);

		//! Destructs the mesh.
		/*!
		Exception safety:
		nothrow
		*/
		~HalfMesh();

		//! Copies another mesh to this mesh.
		/*!
		Exception safety:
		strong
		*/
		HalfMesh<DataPolicy>&
			operator=(const HalfMesh& that);

		//! Swaps two half-edge meshes.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		void swap(HalfMesh& that);

		//! Remove all data of the mesh.
		/*!
		Exception safety:
		nothrow
		*/
		void clear();

		// Iterators

		//! Returns an iterator to the beginning of the vertex set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstVertexIterator vertexBegin() const;

		//! Returns an iterator to the end of the vertex set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstVertexIterator vertexEnd() const;

		//! Returns an iterator to the beginning of the half-edge set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstHalfIterator halfBegin() const;

		//! Returns an iterator to the end of the half-edge set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstHalfIterator halfEnd() const;

		//! Returns an iterator to the beginning of the edge set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstEdgeIterator edgeBegin() const;

		//! Returns an iterator to the end of the edge set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstEdgeIterator edgeEnd() const;

		//! Returns an iterator to the beginning of the polygon set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstPolygonIterator polygonBegin() const;

		//! Returns an iterator to the end of the polygon set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		ConstPolygonIterator polygonEnd() const;

		//! Searches for a half-edge from 'fromVertex' to 'toVertex'.
		/*!
		Returns:
		The half-edge if found, or an empty half-edge
		if not found.

		Time complexity:
		O(number of edges around 'fromVertex')

		Exception safety:
		nothrow
		*/
		Half findHalf(
			Vertex fromVertex,
			Vertex toVertex) const;

		// Modification

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
		bool makeAdjacent(Half in, Half out);

		//! Adds a vertex.
		/*!
		Returns:
		The new vertex.

		Time complexity:
		constant on average

		Exception safety:
		strong
		*/
		Vertex addVertex();

		//! Adds an edge between two vertices.
		/*!
		fromVertex, toVertex:
		The vertices between which to add the new edge.

		outAlreadyExisted:
		If a non-zero pointer is passed,
		is filled with the information whether
		the edge already existed or not.

		Returns:
		The new edge if successful,
		otherwise an empty edge.
		If the edge already existed,
		that edge is returned.

		Preconditions:
		!fromVertex.empty()
		!toVertex.empty()

		Time complexity:
		O(edges around 'fromVertex') +
		O(edges around 'toVertex') on average

		Exception safety:
		strong
		*/
		Edge addEdge(
			Vertex fromVertex,
			Vertex toVertex,
			bool* outAlreadyExisted = 0);

		//! Adds a polygon to the given boundary loop.
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
		Polygon addPolygon(
			const std::vector<Half>& halfLoop);

		//! Adds a triangle to the given vertices.
		/*!
		This is a convenience function that simply calls the more general
		'addPolygon(const std::vector<Vertex>&)'. See its documentation.
		*/
		Polygon addPolygon(
			Vertex aVertex,
			Vertex bVertex,
			Vertex cVertex);

		//! Adds a quadrangle to the given vertices.
		/*!
		This is a convenience function that simply calls the more general
		'addPolygon(const std::vector<Vertex>&)'. See its documentation.
		*/
		Polygon addPolygon(
			Vertex aVertex,
			Vertex bVertex,
			Vertex cVertex,
			Vertex dVertex);

		//! Adds a polygon to the given vertex loop.
		/*!
		Time complexity:
		sum_i O(edges around vertexLoop[i]) +
		O(vertexLoop.size()) on average

		Exception safety:
		strong
		*/
		Polygon addPolygon(
			const std::vector<Vertex>& vertexLoop);

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
		void removeVertex(Vertex vertex);

		//! Removes an edge and all its neighbouring polygons.
		/*!
		Time complexity:
		O(edges in the left polygon) +
		O(edges in the right polygon) +
		O(log(edges())) on average

		Exception safety:
		nothrow
		*/
		void removeEdge(Edge edge);

		//! Removes a polygon.
		/*!
		Time complexity:
		O(edges in the polygon) on average

		Exception safety:
		nothrow
		*/
		void removePolygon(Polygon polygon);

		//! Returns the number of vertices.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		integer vertices() const;

		//! Returns the number of edges.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		integer edges() const;

		//! Returns the number of polygons.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		integer polygons() const;

	private:
		Half findFreeIncident(Vertex vertex) const;
		Half findFreeIncident(Vertex vertex,
			Half startingFrom,
			Half andBefore) const;

		// Allocation

		Vertex allocateVertex();
		Half allocateHalf();
		Edge allocateEdge();
		Polygon allocatePolygon();

		// Deallocation

		void deallocateVertex(Vertex vertex);
		void deallocateHalf(Half half);
		void deallocateEdge(Edge edge);
		void deallocatePolygon(Polygon polygon);

		VertexSet vertexSet_;
		HalfSet halfSet_;
		EdgeSet edgeSet_;
		PolygonSet polygonSet_;

		PoolAllocator vertexAllocator_;
		PoolAllocator halfAllocator_;
		PoolAllocator edgeAllocator_;
		PoolAllocator polygonAllocator_;

		// Unfortunately we need to define these inner classes here.
		// Moving them outside of HalfMesh triggers a
		// compiler bug. You could avoid the bug by
		// moving the inner class definitions out, and
		// then also moving the member
		// function definitions of the inner classes out.
		// But I think this is clear enough.

		class VertexBody
			: public PossiblyEmptyMember<VertexData>
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			VertexBody()
				: PossiblyEmptyMember<VertexData>()
				, half_(0)
			{
			}

			HalfBody* half_;
		};

		class HalfBody
			: public PossiblyEmptyMember<HalfData>
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			HalfBody()
				: PossiblyEmptyMember<HalfData>()
				, next_(0)
				, previous_(0)
				, pair_(0)
				, origin_(0)
				, edge_(0)
				, left_(0)
			{
			}

			HalfBody* next_;
			HalfBody* previous_;
			HalfBody* pair_;
			VertexBody* origin_;
			EdgeBody* edge_;
			PolygonBody* left_;
		};

		class EdgeBody
			: public PossiblyEmptyMember<EdgeData>
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			EdgeBody()
				: PossiblyEmptyMember<EdgeData>()
				, half_(0)
			{
			}

			HalfBody* half_;
		};

		class PolygonBody
			: public PossiblyEmptyMember<PolygonData>
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			PolygonBody()
				: PossiblyEmptyMember<PolygonData>()
				, half_(0)
			{
			}

			HalfBody* half_;
		};

	public:
		class Half
			: boost::less_than_comparable<Half
			, boost::equality_comparable<Half
			> >
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			friend class HalfMesh<DataPolicy>;
			friend class HalfHash;

			Half()
				: half_(0)
			{
			}

			explicit Half(HalfBody* half)
				: half_(half)
			{
			}

			void clear()
			{
				half_ = 0;
			}

			HalfData* operator->() const
			{
				PENSURE(half_);

				return half_->data();
			}

			HalfData& operator*() const
			{
				PENSURE(half_);

				return *half_->data();
			}

			HalfData& operator()() const
			{
				PENSURE(half_);

				return *half_->data();
			}

			Half rotateNext() const
			{
				PENSURE(half_);

				ASSERT(half_->pair_);
				ASSERT(half_->pair_->next_);

				return Half(half_->pair_->next_);
			}

			Half rotatePrevious() const
			{
				PENSURE(half_);

				ASSERT(half_->previous_);
				ASSERT(half_->previous_->pair_);

				return Half(half_->previous_->pair_);
			}

			Half next() const
			{
				PENSURE(half_);

				ASSERT(half_->next_);

				return Half(half_->next_);
			}

			Half previous() const
			{
				PENSURE(half_);

				ASSERT(half_->previous_);

				return Half(half_->previous_);
			}

			Half pair() const
			{
				PENSURE(half_);

				ASSERT(half_->pair_);

				return Half(half_->pair_);
			}

			Edge edge() const
			{
				PENSURE(half_);

				ASSERT(half_->edge_);

				return Edge(half_->edge_);
			}

			Polygon left() const
			{
				PENSURE(half_);

				return Polygon(half_->left_);
			}

			Polygon right() const
			{
				PENSURE(half_);

				ASSERT(half_->pair_);
				ASSERT(half_->pair_->left_);

				return Polygon(half_->pair_->left_);
			}

			Vertex origin() const
			{
				PENSURE(half_);

				ASSERT(half_->origin_);

				return Vertex(half_->origin_);
			}

			Vertex destination() const
			{
				PENSURE(half_);

				ASSERT(half_->pair_);
				ASSERT(half_->pair_->origin_);

				return Vertex(half_->pair_->origin_);
			}

			bool free() const
			{
				return left().empty();
			}

			bool empty() const
			{
				return (half_ == 0);
			}

			bool operator<(const Half& that) const
			{
				return half_ < that.half_;
			}

			bool operator==(const Half& that) const
			{
				return half_ == that.half_;
			}

		private:
			HalfBody* half_;
		};

		class Vertex
			: boost::less_than_comparable<Vertex
			, boost::equality_comparable<Vertex
			> >
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			friend class HalfMesh<DataPolicy>;
			friend class VertexHash;

			Vertex()
				: vertex_(0)
			{
			}

			explicit Vertex(VertexBody* vertex)
				: vertex_(vertex)
			{
			}

			void clear()
			{
				vertex_ = 0;
			}

			VertexData& operator*() const
			{
				PENSURE(vertex_);

				return *vertex_->data();
			}

			VertexData* operator->() const
			{
				PENSURE(vertex_);

				return vertex_->data();
			}

			VertexData& operator()() const
			{
				PENSURE(vertex_);

				return *vertex_->data();
			}

			Half half() const
			{
				PENSURE(vertex_);

				return Half(vertex_->half_);
			}

			bool free() const
			{
				PENSURE(vertex_);

				if (isolated())
				{
					return true;
				}

				Half begin(vertex_->half_);
				Half current(begin);
				do
				{
					if (current.left().empty())
					{
						return true;
					}
					current = current.rotateNext();
				}
				while (current != begin);

				return false;
			}

			bool empty() const
			{
				return (vertex_ == 0);
			}

			bool isolated() const
			{
				PENSURE(vertex_);

				return (vertex_->half_ == 0);
			}

			bool operator<(const Vertex& that) const
			{
				return vertex_ < that.vertex_;
			}

			bool operator==(const Vertex& that) const
			{
				return vertex_ == that.vertex_;
			}

		private:
			VertexBody* vertex_;
		};

		class Edge
			: boost::less_than_comparable<Edge
			, boost::equality_comparable<Edge
			> >
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			friend class HalfMesh<DataPolicy>;
			friend class EdgeHash;

			Edge()
				: edge_(0)
			{
			}

			explicit Edge(EdgeBody* edge)
				: edge_(edge)
			{
			}

			void clear()
			{
				edge_ = 0;
			}

			EdgeData& operator*() const
			{
				PENSURE(edge_);

				return *edge_->data();
			}

			EdgeData* operator->() const
			{
				PENSURE(edge_);

				return edge_->data();
			}

			EdgeData& operator()() const
			{
				PENSURE(edge_);

				return *edge_->data();
			}

			Half half() const
			{
				PENSURE(edge_);

				return Half(edge_->half_);
			}

			bool empty() const
			{
				return (edge_ == 0);
			}

			bool operator<(const Edge& that) const
			{
				return edge_ < that.edge_;
			}

			bool operator==(const Edge& that) const
			{
				return edge_ == that.edge_;
			}

		private:
			EdgeBody* edge_;
		};

		class Polygon
			: boost::less_than_comparable<Polygon
			, boost::equality_comparable<Polygon
			> >
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			friend class HalfMesh<DataPolicy>;
			friend class PolygonHash;

			Polygon()
				: polygon_(0)
			{
			}

			explicit Polygon(PolygonBody* polygon)
				: polygon_(polygon)
			{
			}

			void clear()
			{
				polygon_ = 0;
			}

			PolygonData& operator*() const
			{
				PENSURE(polygon_);

				return *polygon_->data();
			}

			PolygonData* operator->() const
			{
				PENSURE(polygon_);

				return polygon_->data();
			}

			PolygonData& operator()() const
			{
				PENSURE(polygon_);

				return *polygon_->data();
			}

			Half half() const
			{
				PENSURE(polygon_);

				return Half(polygon_->half_);
			}

			bool empty() const
			{
				return (polygon_ == 0);
			}

			bool operator<(const Polygon& that) const
			{
				return polygon_ < that.polygon_;
			}

			bool operator==(const Polygon& that) const
			{
				return polygon_ == that.polygon_;
			}

		private:
			PolygonBody* polygon_;
		};

		class VertexHash
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			VertexHash()
				: hash_()
			{
			}

			std::size_t operator()(const Vertex& vertex) const
			{
				return hash_(vertex.vertex_);
			}

		private:
			hash<VertexBody*> hash_;
		};

		class HalfHash
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			HalfHash()
				: hash_()
			{
			}

			std::size_t operator()(const Half& half) const
			{
				return hash_(half.half_);
			}

		private:
			hash<HalfBody*> hash_;
		};

		class EdgeHash
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			EdgeHash()
				: hash_()
			{
			}

			std::size_t operator()(const Edge& edge) const
			{
				return hash_(edge.edge_);
			}

		private:
			hash<EdgeBody*> hash_;
		};

		class PolygonHash
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			PolygonHash()
				: hash_()
			{
			}

			std::size_t operator()(const Polygon& polygon) const
			{
				return hash_(polygon.polygon_);
			}

		private:
			hash<PolygonBody*> hash_;
		};
	};

}

#include "pastel/geometry/halfmesh.hpp"

#endif
