// Description: Incidence graph

#ifndef PASTELSYS_INCIDENCE_GRAPH_H
#define PASTELSYS_INCIDENCE_GRAPH_H

#include "pastel/sys/incidence_graph_fwd.h"
#include "pastel/sys/incidence_graph_incidence.h"
#include "pastel/sys/incidence_graph_vertex.h"
#include "pastel/sys/incidence_graph_edge.h"
#include "pastel/sys/iteratoraddress_hash.h"

#include <unordered_map>
#include <memory>

namespace Pastel
{

	//! Incidence graph
	template <
		GraphType::Enum Type = GraphType::Directed, 
		typename VertexData = void, 
		typename EdgeData = void>
	class Incidence_Graph
	{
	public:
		typedef Incidence_Graph_Fwd<Type, VertexData, EdgeData> Fwd;

		PASTEL_FWD(Vertex);
		PASTEL_FWD(VertexSet);
		PASTEL_FWD(Vertex_Iterator);
		PASTEL_FWD(Vertex_ConstIterator);
		PASTEL_FWD(VertexData_Class);

		PASTEL_FWD(Edge);
		PASTEL_FWD(EdgeSet);
		PASTEL_FWD(Edge_Iterator);
		PASTEL_FWD(Edge_ConstIterator);
		PASTEL_FWD(EdgeData_Class);

		PASTEL_FWD(Incidence_Link);
		PASTEL_FWD(Incidence);
		PASTEL_FWD(Incidence_Iterator);
		PASTEL_FWD(Incidence_ConstIterator);

		enum
		{
			Outgoing = Fwd::Outgoing,
			Incoming = Fwd::Incoming,
			Undirected = Fwd::Undirected,
			IncidenceTypes = Fwd::IncidenceTypes
		};

		//! Constructs an empty graph.
		/*!
		Time complexity:
		constant

		Exception safety:
		strong
		*/
		Incidence_Graph()
			: vertexSet_()
			, edgeSet_()
		{
		}

		//! Copy-constructs from another graph.
		/*!
		Time complexity:
		O(n + m)
		where
		n is the number of vertices, and
		m is the number of edges.

		Exception safety:
		strong
		*/
		Incidence_Graph(const Incidence_Graph& that)
			: vertexSet_()
			, edgeSet_()
		{
			std::unordered_map<
				Vertex_ConstIterator,
				Vertex_ConstIterator,
				IteratorAddress_Hash> vertexMap;

			Incidence_Graph copy;

			for (auto vertex = that.cVertexBegin();
				vertex != that.cVertexEnd();
				++vertex)
			{
				vertexMap[vertex] = 
					copy.addVertex(*vertex);
			}

			for (auto edge = that.cEdgeBegin();
				edge != that.cEdgeEnd();
				++edge)
			{
				copy.addEdge(
					vertexMap[edge->from()],
					vertexMap[edge->to()],
					*edge);
			}

			swap(copy);
		}

		//! Move-constructs from another graph.
		/*!
		Time complexity:
		constant

		Exception safety:
		strong
		*/
		Incidence_Graph(Incidence_Graph&& that)
			: vertexSet_()
			, edgeSet_()
		{
			swap(that);
		}

		//! Assigns from another graph.
		/*!
		Time complexity:
		O(n + m), if 'that' is copy-constructed
		constant, if 'that' is move-constructed
		where 
		n is the number of vertices, and
		m is the number of edges.

		Exception safety:
		strong
		*/
		Incidence_Graph& operator=(Incidence_Graph that)
		{
			swap(that);
			return *this;
		}

		//! Swaps two graphs.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		void swap(Incidence_Graph& that)
		{
			vertexSet_.swap(that.vertexSet_);
			edgeSet_.swap(that.edgeSet_);
		}

		//! Removes all vertices and edges.
		/*!
		Time complexity:
		O(n + m)
		where 
		n is the number of vertices, and
		m is the number of edges.

		Exception safety:
		nothrow
		*/
		void clear()
		{
			vertexSet_.clear();
			edgeSet_.clear();
		}

		//! Removes all edges.
		/*!
		Time complexity:
		O(m)
		where 
		m is the number of edges.

		Exception safety:
		nothrow
		*/
		void clearEdges()
		{
			while(!edgeSet_.empty())
			{
				removeEdge(edgeSet_.cbegin());
			}
		}

		// Vertices

		//! Adds a new vertex into the graph.
		/*!
		Time complexity:
		constant + vertex-data construction
		
		Exception safety:
		strong
		*/
		Vertex_Iterator addVertex(
			VertexData_Class vertexData = VertexData_Class())
		{
			// Construct the vertex...
			vertexSet_.emplace_back(
				Vertex(std::move(vertexData)));

			// ... and return it.
			Vertex_Iterator vertex = vertexSet_.end();
			--vertex;
			return vertex;
		}

		//! Removes a vertex from the graph.
		/*!
		Time complexity:
		O(m)
		where
		m is the number of edges incident to the vertex.

		Exception safety:
		nothrow
		*/
		Vertex_Iterator removeVertex(
			const Vertex_ConstIterator& vertex)
		{
			// Remove all edges incident to this vertex.
			while(vertex->allEdges() > 0)
			{
				// Note that self-loops contribute twice
				// to the set of incidences, while the other edges
				// contribute only once. This is why the indices() 
				// is checked repeatedly, rather than being decremented.

				// Also, note that removing an edge removes incidences
				// from the incidenceSet in an arbitrary way.
				// This is why we do the removal so that we always
				// retrieve a fresh incidence iterator.
				removeEdge(vertex->cAllBegin()->edge());
			}

			// Remove the vertex.
			// The cast is needed for GCC's non-C++11 libraries.
			return vertexSet_.erase(cast(vertex));
		}

		//! Casts away constness of a vertex.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		Vertex_Iterator cast(
			const Vertex_ConstIterator& vertex)
		{
			return vertexSet_.erase(vertex, vertex);
		}

		//! Returns the first iterator of the vertex set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		Vertex_Iterator vertexBegin()
		{
			return vertexSet_.begin();
		}

		//! Returns the first iterator of the vertex set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		Vertex_ConstIterator cVertexBegin() const
		{
			return ((Incidence_Graph&)*this).vertexSet_.begin();
		}

		//! Returns the end-iterator of the vertex-set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		Vertex_Iterator vertexEnd()
		{
			return vertexSet_.end();
		}

		//! Returns the end-iterator of the vertex-set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		Vertex_ConstIterator cVertexEnd() const
		{
			return ((Incidence_Graph&)*this).vertexSet_.end();
		}

		//! Returns the number of vertices in the graph.
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

		// Edges

		//! Adds a new edge into the graph.
		/*!
		Time complexity:
		constant + edge-data construction

		Exception safety:
		strong

		Note:
		For mixed graphs, the created edge is undirected.
		*/
		Edge_Iterator addEdge(
			const Vertex_ConstIterator& from,
			const Vertex_ConstIterator& to,
			EdgeData_Class edgeData = EdgeData_Class())
		{
			Vertex_Iterator mutableFrom = cast(from);
			Vertex_Iterator mutableTo = cast(to);

			// Create the incidences. Note that the incidence 
			// gets the opposing vertex. We are creating
			// the incidences before the edge, because we want
			// strong exception safety.
			std::unique_ptr<Incidence> fromIncidence(
				new Incidence(mutableTo));
			std::unique_ptr<Incidence> toIncidence( 
				new Incidence(mutableFrom));

			// For directed graphs, the edge is of course
			// always directed. For undirected and mixed
			// graphs, the default is to create undirected
			// edges.
			const bool directed = 
				(Type == GraphType::Directed);

			// Create the edge.
			edgeSet_.emplace_back(
				Edge(std::move(edgeData), directed));

			Edge_Iterator edge = edgeSet_.end();
			--edge;

			fromIncidence->edge_ = edge;
			edge->from_ = fromIncidence.get();

			toIncidence->edge_ = edge;
			edge->to_ = toIncidence.get();

			// Link the edge to the vertices.
			{
				if (edge->directed())
				{
					mutableFrom->insert(Outgoing,
						fromIncidence.release());
					mutableTo->insert(Incoming,
						toIncidence.release());
				}
				else
				{
					mutableFrom->insert(Undirected,
						fromIncidence.release());
					mutableTo->insert(Undirected,
						toIncidence.release());
				}
			}

			return edge;
		}

		//! Removes an edge from the graph.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		Edge_Iterator removeEdge(
			const Edge_ConstIterator& edge)
		{
			Vertex_Iterator from = cast(edge)->from();
			Vertex_Iterator to = cast(edge)->to();

			// Link the edge off from the vertices.
			if (edge->directed())
			{
				from->erase(Outgoing, edge->from_);
				to->erase(Incoming, edge->to_);
			}
			else
			{
				from->erase(Undirected, edge->from_);
				to->erase(Undirected, edge->to_);
			}

			// Remove the edge.
			return edgeSet_.erase(cast(edge));
		}

		//! Reverses the directionality of an edge.
		/*!
		Time complexity:
		constant
		
		Exception safety:
		nothrow
		*/
		void reverse(
			const Edge_ConstIterator& edge)
		{
			Vertex_Iterator from =
				cast(edge->from());
			Vertex_Iterator to =
				cast(edge->to());

			if (edge->directed())
			{
				from->move(Outgoing, Incoming,
					cast(edge)->from_);
				to->move(Incoming, Outgoing,
					cast(edge)->to_);
			}

			std::swap(cast(edge)->from_, cast(edge)->to_);
		}

		//! Sets the directness of an edge.
		/*!
		Time complexity:
		constant
		
		Exception safety:
		nothrow
		*/
		void setDirected(
			const Edge_ConstIterator& edge,
			bool directed)
		{
			ENSURE(Type == GraphType::Mixed);

			if (edge->directed() == directed)
			{
				return;
			}

			Vertex_Iterator from =
				cast(edge->from());
			Vertex_Iterator to =
				cast(edge->to());

			if (directed)
			{
				// Turn an undirected edge into 
				// a directed edge.
				from->move(Undirected, Outgoing,
					cast(edge)->from_);
				to->move(Undirected, Incoming,
					cast(edge)->to_);
			}
			else
			{
				// Turn a directed edge into 
				// an undirected edge.
				from->move(Outgoing, Undirected,
					cast(edge)->from_);
				to->move(Incoming, Undirected,
					cast(edge)->to_);
			}

			cast(edge)->setDirected(directed);
		}

		//! Casts away constness of an edge.
		/*!
		Time complexity:
		constant
		
		Exception safety:
		nothrow
		*/
		Edge_Iterator cast(
			const Edge_ConstIterator& edge)
		{
			return edgeSet_.erase(edge, edge);
		}

		//! Returns the first iterator of the edge set.
		/*!
		Time complexity:
		constant
		
		Exception safety:
		nothrow
		*/
		Edge_Iterator edgeBegin()
		{
			return edgeSet_.begin();
		}

		//! Returns the first iterator of the edge set.
		/*!
		Time complexity:
		constant
		
		Exception safety:
		nothrow
		*/
		Edge_ConstIterator cEdgeBegin() const
		{
			return ((Incidence_Graph&)*this).edgeSet_.begin();
		}

		//! Returns the end-iterator of the edge set.
		/*!
		Time complexity:
		constant
		
		Exception safety:
		nothrow
		*/
		Edge_Iterator edgeEnd()
		{
			return edgeSet_.end();
		}

		//! Returns the end-iterator of the edge set.
		/*!
		Time complexity:
		constant
		
		Exception safety:
		nothrow
		*/
		Edge_ConstIterator cEdgeEnd() const
		{
			return ((Incidence_Graph&)*this).edgeSet_.end();
		}

		//! Returns the number of edges.
		/*!
		Time complexity:
		constant
		
		Exception safety:
		nothrow
		*/
		integer edges() const
		{
			return edgeSet_.size();
		}

		// Global operations

		//! Merges a graph into this graph.
		/*!
		Time complexity:
		constant
		
		Exception safety:
		nothrow
		*/
		void merge(Incidence_Graph& that)
		{
			// Merge the vertex sets.
			vertexSet_.splice(
				vertexSet_.end(),
				that.vertexSet_);

			// Merge the edge sets.
			edgeSet_.splice(
				edgeSet_.end(),
				that.edgeSet_);
		}

	private:
		VertexSet vertexSet_;
		EdgeSet edgeSet_;
	};

}

#endif
