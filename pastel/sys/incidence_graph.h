#ifndef PASTEL_INCIDENCE_GRAPH_H
#define PASTEL_INCIDENCE_GRAPH_H

#include "pastel/sys/incidence_graph_fwd.h"
#include "pastel/sys/incidence_graph_incidence.h"
#include "pastel/sys/incidence_graph_vertex.h"
#include "pastel/sys/incidence_graph_edge.h"

#include <memory>

namespace Pastel
{

	template <GraphType::Enum Type = GraphType::Directed, 
		typename VertexData = EmptyClass, typename EdgeData = EmptyClass>
	class Incidence_Graph
		: public Incidence_Graph_Fwd<Type, VertexData, EdgeData>
	{
	public:
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

		// Vertices

		//! Adds a new vertex into the graph.
		/*!
		Time complexity:
		constant + vertex-data construction
		
		Exception safety:
		strong
		*/
		Vertex_Iterator addVertex(
			VertexData vertexData = VertexData())
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
		void removeVertex(
			const Vertex_ConstIterator& vertex)
		{
			// Remove all edges incident to this vertex.
			while(vertex->incidentEdges() > 0)
			{
				// Note that self-loops contribute twice
				// to the set of incidences, while the other edges
				// contribute only once. This is why the indices() 
				// is checked repeatedly, rather than being decremented.

				// Also, note that removing an edge removes incidences
				// from the incidenceSet in an arbitrary way.
				// This is why we do the removal so that we always
				// retrieve a fresh incidence iterator.
				removeEdge(vertex->cbegin()->edge());
			}

			// Remove the vertex.
			vertexSet_.erase(vertex);
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
			return vertexSet_.cbegin();
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
			return vertexSet_.cend();
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
		shady, FIX
		*/
		Edge_Iterator addEdge(
			const Vertex_ConstIterator& from,
			const Vertex_ConstIterator& to,
			EdgeData edgeData = EdgeData(),
			bool directed = (Type == GraphType::Directed))
		{
			ENSURE(
				(Type == GraphType::Undirected && !directed) ||
				(Type == GraphType::Directed && directed) ||
				(Type == GraphType::Mixed));

			Vertex_Iterator mutableFrom = cast(from);
			Vertex_Iterator mutableTo = cast(to);

			// Create the edge.
			edgeSet_.emplace_back(
				Edge(std::move(edgeData), directed));

			Edge_Iterator edge = edgeSet_.end();
			--edge;

			// Link the edge to the vertices.
			{
				// Note that the incidence gets the 
				// opposing vertex.
				std::unique_ptr<Incidence> fromIncidence(
					new Incidence(mutableTo, edge));
				std::unique_ptr<Incidence> toIncidence( 
					new Incidence(mutableFrom, edge));

				edge->from_ = fromIncidence.get();
				edge->to_ = toIncidence.get();

				if (edge->directed())
				{
					mutableFrom->insert<Outgoing>(
						fromIncidence.release());
					mutableTo->insert<Incoming>(
						toIncidence.release());
				}
				else
				{
					mutableFrom->insert<Undirected>(
						fromIncidence.release());
					mutableTo->insert<Undirected>(
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
		void removeEdge(
			const Edge_ConstIterator& edge)
		{
			Vertex_Iterator from = cast(edge)->from();
			Vertex_Iterator to = cast(edge)->to();

			// Link the edge off from the vertices.
			if (edge->directed())
			{
				from->erase<Outgoing>(edge->from_);
				to->erase<Incoming>(edge->to_);
			}
			else
			{
				from->erase<Undirected>(edge->from_);
				to->erase<Undirected>(edge->to_);
			}

			// Remove the edge.
			edgeSet_.erase(edge);
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
			return edgeSet_.cbegin();
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
			return edgeSet_.cend();
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

	private:
		// TODO: Implement
		Incidence_Graph(const Incidence_Graph& that);

		VertexSet vertexSet_;
		EdgeSet edgeSet_;
	};

}

#endif
