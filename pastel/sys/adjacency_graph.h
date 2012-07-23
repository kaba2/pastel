#ifndef PASTEL_ADJACENCY_GRAPH_H
#define PASTEL_ADJACENCY_GRAPH_H

#include "pastel/sys/adjacency_graph_fwd.h"
#include "pastel/sys/adjacency_graph_incidence.h"
#include "pastel/sys/adjacency_graph_vertex.h"
#include "pastel/sys/adjacency_graph_edge.h"

namespace Pastel
{

	template <GraphType::Enum Type = GraphType::Directed, 
		typename VertexData = EmptyClass, typename EdgeData = EmptyClass>
	class Adjacency_Graph
		: public Adjacency_Graph_Fwd<Type, VertexData, EdgeData>
	{
	public:
		Adjacency_Graph()
			: vertexSet_()
			, edgeSet_()
		{
		}

		Adjacency_Graph(Adjacency_Graph&& that)
			: vertexSet_()
			, edgeSet_()
		{
			swap(that);
		}

		Adjacency_Graph& operator=(Adjacency_Graph that)
		{
			swap(that);
			return *this;
		}

		void swap(Adjacency_Graph& that)
		{
			vertexSet_.swap(that.vertexSet_);
			edgeSet_.swap(that.edgeSet_);
		}

		void clear()
		{
			vertexSet_.clear();
			edgeSet_.clear();
		}

		// Vertices

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

		Vertex_Iterator cast(
			const Vertex_ConstIterator& vertex)
		{
			return vertexSet_.erase(vertex, vertex);
		}

		Vertex_Iterator vertexBegin()
		{
			return vertexSet_.begin();
		}

		Vertex_ConstIterator cVertexBegin() const
		{
			return vertexSet_.cbegin();
		}

		Vertex_Iterator vertexEnd()
		{
			return vertexSet_.end();
		}

		Vertex_ConstIterator cVertexEnd() const
		{
			return vertexSet_.cend();
		}

		integer vertices() const
		{
			return vertexSet_.size();
		}

		// Edges

		Edge_Iterator addEdge(
			const Vertex_ConstIterator& from,
			const Vertex_ConstIterator& to,
			EdgeData edgeData = EdgeData())
		{
			Vertex_Iterator mutableFrom = cast(from);
			Vertex_Iterator mutableTo = cast(to);

			// Create the edge.
			edgeSet_.emplace_back(
				Edge(std::move(edgeData)));

			Edge_Iterator edge = edgeSet_.end();
			--edge;

			// Link the edge to the 'from' vertex.
			{
				Incidence* incidence = 
					new Incidence(cast(from), edge);

				mutableFrom->insert<Outgoing>(incidence);
				edge->from_ = incidence;
			}

			// Link the edge to the 'to' vertex.
			{
				Incidence* incidence = 
					new Incidence(cast(to), edge);

				mutableTo->insert<Incoming>(incidence);
				edge->to_ = incidence;
			}

			return edge;
		}

		void removeEdge(
			const Edge_ConstIterator& edge)
		{
			// Link the edge off from the 'from' vertex.
			{
				Vertex_Iterator vertex = cast(edge)->from();
				vertex->erase<Outgoing>(edge->from_);
			}

			// Link the edge off from the 'to' vertex.
			{
				Vertex_Iterator vertex = cast(edge)->to();
				vertex->erase<Incoming>(edge->to_);
			}

			// Remove the edge.
			edgeSet_.erase(edge);
		}

		Edge_Iterator cast(
			const Edge_ConstIterator& edge)
		{
			return edgeSet_.erase(edge, edge);
		}

		Edge_Iterator edgeBegin()
		{
			return edgeSet_.begin();
		}

		Edge_ConstIterator cEdgeBegin() const
		{
			return edgeSet_.cbegin();
		}

		Edge_Iterator edgeEnd()
		{
			return edgeSet_.end();
		}

		Edge_ConstIterator cEdgeEnd() const
		{
			return edgeSet_.cend();
		}

		integer edges() const
		{
			return edgeSet_.size();
		}

	private:
		// TODO: Implement
		Adjacency_Graph(const Adjacency_Graph& that);

		VertexSet vertexSet_;
		EdgeSet edgeSet_;
	};

}

#endif
